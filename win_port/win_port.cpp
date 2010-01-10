//  InteLib                                    http://www.intelib.org
//  The file win_port/win_port.cpp
//
//  Copyright (c) Denis Klychkov, 2010
//
//
//  This is free software, licensed under GNU LGPL v.2.1
//  See the file COPYING for further details.
//
//  THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//  Please see the file WARRANTY for the detailed explanation.

#include "win_port.hpp"

#ifdef _WIN32

#include <windows.h>
#include <tlhelp32.h>

#else

#include <unistd.h>
#include <sys/wait.h>

#endif

#include <stdarg.h>

#ifdef _WIN32
// Not necessary for *nix
pid_t getppid()
{
    int pid = GetCurrentProcessId();
    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(h, &pe))
    {
        do
        {
            if (pe.th32ProcessID == pid) return pe.th32ParentProcessID;
        }
        while(Process32Next(h, &pe));
    }
    CloseHandle(h);
    return -1;
}

#endif

int execute_cmd_vp(const char *file, char * const argv[])
{
#ifdef _WIN32
// Windows-like process execution

    // First - create a cmd line (concatenation of all args)
    char *cmd;
    int cmd_length = 0;
    for (int i = 0; argv[i]; i++)
        cmd_length += strlen(argv[i] + 1);
    
    cmd = new char[cmd_length];
    char *p = cmd;
    for (char * const *arg = argv; *arg; arg++)
    {
        const char *g = *arg;
        while (*g) *p++ = *g++;
        *p++ = ' ';
    }
    *(p-1) = '\0';

    STARTUPINFO startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION pInfo;
    
    BOOL res = CreateProcess(file, cmd, NULL, NULL, FALSE,
        NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &pInfo);

    if (res)
    {
        // Waiting for termination
        res = WaitForSingleObject(pInfo.hProcess, INFINITE) == WAIT_OBJECT_0;
        CloseHandle(pInfo.hProcess);
    }
    
    delete []cmd;
    if (res == 0) return -1;
    
    return 0;
    
#else
// UNIX-like process execution

    pid_t child_pid = fork();
    int status;
    
    if (child_pid == -1) return -1;
    else if (child_pid == 0)
    {
        execvp(file, argv);
        return -1;
    }
    else
    {
        wait(&status);
        if ( !(WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) ) return -1;
    }
    return 0;
#endif
}

int execute_cmd_lp(const char *file, char * const arg0, ...)
{
    va_list vl;
    int argc = 0;
    
    va_start(vl, arg0);
    while (va_arg(vl, char*)) argc++;
    va_end(vl);
    
    char **argv = new char*[argc + 1];

    va_start(vl, arg0);
    for (int i = 0; i < argc; i++)
        argv[i] = va_arg(vl, char*);
    va_end(vl);
    
    int res = execute_cmd_vp(file, argv);
    delete []argv;
    return res;
}
