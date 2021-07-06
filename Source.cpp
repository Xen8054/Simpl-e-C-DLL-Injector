#include <Windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <fstream>

//I MADE THIS SHIT IN LIKE 5 MINUTES (okay more like a lil less than 10)
//char set must be set to Multi-Byte

std::string TargetProcess;
std::string dlllll;

DWORD ProcessID()
{
	HANDLE hwnddddd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processentry; //proc entry
	processentry.dwSize = sizeof(processentry);
	do {
		if (!strcmp(processentry.szExeFile, TargetProcess.c_str())) {
			DWORD pid = processentry.th32ProcessID;
			CloseHandle(hwnddddd);
			return pid;
		}
	} 
	while
		(Process32Next(hwnddddd, &processentry));
}

int Inject(std::string DLL) { //shit-ish injection method, im well aware but if you can make something better idk why you're here
	/* simple way to check if the dll exists */
	std::ifstream dll_file("./" + DLL);
	if (!dll_file) {
		std::cout << "DLL CANNOT BE FOUND: \"" << DLL << "\"\n\nClosing in 5 seconds...";
		Sleep(5000);
		return 1;
	}

	/* injecting */
	LPCSTR dll = DLL.c_str();
	DWORD process;
	char DLL_[MAX_PATH];
	GetFullPathName(dll, MAX_PATH, DLL_, 0);
	process = ProcessID();
	HANDLE Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, process);
	LPVOID AllocatedMemory = VirtualAllocEx(Process, 0, sizeof(DLL_), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(Process, AllocatedMemory, DLL_, sizeof(DLL_), 0);
	CreateRemoteThread(Process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, AllocatedMemory, 0, 0);
	CloseHandle(Process);

	std::cout << "Successfully Injected \""<< DLL <<"\" into Target Process \""<< TargetProcess <<"\"";

	Sleep(1250);

	return 0;
}

int main() {
	SetConsoleTitleA("DLL Injector");
	std::cout << "DLL Name (inlude .dll at the end): ";
	std::getline(std::cin, dlllll);
	std::cout << "Target Process (inlude .exe at the end): ";
	std::getline(std::cin, TargetProcess);
	Inject(dlllll); /* you need to always include the ".dll" at the end */
}