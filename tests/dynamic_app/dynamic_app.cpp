// dynamic_app.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LoadTests.h"

void PrintUsage() 
{
	wprintf(_T("Usage:\n"));
	wprintf(_T("\tdynamic [resolve:[true|false]]\n"));
	wprintf(_T("\t<resolve> - [OPTIONAL] Resolves callstacks before unloading the dynamic DLL.\n"));
}

// Leaks 6 memory allocations
void LeakDuplicateLeaks() 
{
	// For testing aggregation
	for (int i = 0; i < 3; ++i)
	{
		int* tmp = new int(0x63);
		tmp;
	}
	for (int i = 0; i < 3; ++i)
	{
		int* tmp = new int(0x63);
		tmp;
	}
	// Should report 6 memory leaks
}

// VLD internal API
#ifdef _DEBUG
extern "C" {
	__declspec(dllimport) SIZE_T VLDGetLeaksCount (BOOL includingInternal = FALSE);
}
#else
#define VLDGetLeaksCount() 0
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	wprintf(_T("======================================\n"));
	wprintf(_T("==\n"));
	wprintf(_T("==    VLD Tests: Dynamic DLL Loading  \n"));
	wprintf(_T("==\n"));
	wprintf(_T("======================================\n"));

	bool resolve = true;
	if (argc == 2)
	{
		resolve = _tcsicmp(_T("true"), argv[1]) == 0;
	} 

	RunLoaderTests(resolve);    // leaks 18
	int leaks = (int)VLDGetLeaksCount();
	RunMFCLoaderTests(resolve); // leaks 7
	leaks = (int)VLDGetLeaksCount();
	LeakDuplicateLeaks();       // leaks 6
	leaks = (int)VLDGetLeaksCount();
	// ..................Total:    31 leaks total

	int diff = 31 - leaks;
	return diff;
}
