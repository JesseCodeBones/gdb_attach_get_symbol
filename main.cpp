#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <thread>
#include <signal.h>

#ifdef _MSC_VER
#define DYNAMIC_SYMBOL __declspec(dllexport)
#else
#define DYNAMIC_SYMBOL __attribute__((visibility("default")))
#endif

DYNAMIC_SYMBOL void *sharePtr = nullptr;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
static int32_t getProcessId() noexcept {
#ifdef _WIN32
  return GetCurrentProcessId();
#else
  return getpid();
#endif
}

void sendDebugSignal(){
#ifdef __clang__
  __builtin_debugtrap();
#elif defined _MSC_VER
  __debugbreak();
#else
#ifdef __x86_64__
  asm("int3");
#elif (defined __arm64__) || (defined __aarch64__) || (defined _M_ARM64)
  __builtin_trap();
#endif
#endif
}



int main(int, char **) {

  signal(SIGTRAP, [](int sig) {
    std::cout << "no debugger, pass debug signal\n";
  });

  auto reAllocFun = [](std::size_t size) {
    sharePtr = realloc(sharePtr, size);
    if (sharePtr == nullptr) {
      sharePtr = malloc(size);
    } else {
      void* tempPtr = sharePtr;
      sharePtr = malloc(size);
      free(tempPtr);
    }
    sendDebugSignal();
  };
  int i = 0;
  std::cout << "process ID=" << getProcessId() << std::endl;
  for (;;) {
    i++;
    reAllocFun(sizeof(int));
    int *p = static_cast<int *>(sharePtr);
    *p = i;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000U));
    std::cout << std::hex << p << "-" << std::dec << *p << std::endl;
  }
}
