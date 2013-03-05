// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

static void printcur()
{
	HWINSTA r = GetProcessWindowStation();
	printf("winsta: %x\n", r);

	HDESK curdesk = GetThreadDesktop(GetCurrentThreadId());
	printf("desktop: %x\n", curdesk);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) 
	{
		printf("Usage: runhidden <path to executable>\n");
		return -2;
	}
	//printcur();

	/*HWINSTA winSta = CreateWindowStation(_T("hiddenWinSta"), 0, 0, NULL);
	if (NULL == winSta) 
	{
		printf("Failed to create window station: %d\n", GetLastError());
		return -1;
	}
	if (!SetProcessWindowStation(winSta)) 
	{
		printf("Failed to set window station: %d\n", GetLastError());
		return -1;
	}*/

	HDESK desktop = CreateDesktop(_T("hiddenDesktop"), NULL, NULL, 0, DESKTOP_CREATEWINDOW, NULL);
	if (NULL == desktop)
	{
		printf("Failed to create desktop: %d\n", GetLastError());
		return -1;
	}

	if (!OpenDesktop(_T("hiddenDesktop"), 0, TRUE, GENERIC_ALL))
	{
		printf("Failed to open desktop: %d\n", GetLastError());
		//return -1;
	}

	/*if (!SetThreadDesktop(desktop)) 
	{
		printf("Failed to set desktop: %d\n", GetLastError());
	}*/

	//HWINSTA r = GetProcessWindowStation();
	//printf("winsta: %x %x\n", r, winSta);

	//HDESK curdesk = GetThreadDesktop(GetCurrentThreadId());
	//printf("desktop: %x %x\n", curdesk, desktop);

	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.lpDesktop = _T("hiddenDesktop");
    ZeroMemory( &pi, sizeof(pi) );
	DWORD res = CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (0 == res) 
	{
		printf("failed to launch: %d\n", GetLastError());
		return -1;
	}
	WaitForSingleObject(pi.hProcess, -1);
	
	CloseDesktop(desktop);
	//CloseDesktop(desktop);
	//CloseWindowStation(winSta);

	return 0;
}

