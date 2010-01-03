#ifndef WINDOWS_PORT
#define WINDOWS_PORT

#ifdef _WIN32

#ifndef pid_t
typedef int pid_t;
#endif

pid_t getppid();

#endif

#endif
