#ifndef WINDOWS_PORT
#define WINDOWS_PORT

#ifdef _WIN32

#ifndef pid_t
typedef int pid_t;
#endif

pid_t getppid();

#endif

int execute_cmd_lp(const char *file, const char *arg0, ...);
int execute_cmd_vp(const char *file, const char *argv[]);

#endif
