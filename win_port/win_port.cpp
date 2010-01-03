#include "win_port.hpp"
#include <windows.h>
#include <tlhelp32.h>

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
