#include "stdafx.h"
#include "ReadWriteData.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

#define ToDoEncrypt 1 // ���������� � ������
#define ToDoDecrypt 2 // ������������ � ����������
#define ToDoCancel NULL // ������ �� ������
#define PasswordLenght 30 // ����� ������

int numCPU = 0; // ���������� ���� ����������
HANDLE hfInputFile = NULL; // ����� ��������� �����
DWORD IFSectorPerCluster, IFBytesPerSector, IFNumberOfFreeClusters, IFTotalNumberOfClusters, IFSizeOfCluster; // ��������� �����, �� ������� �������� �������� ����
HANDLE hfOutputFile = NULL; // ����� ���������� �����
DWORD OFSectorPerCluster, OFBytesPerSector, OFNumberOfFreeClusters, OFTotalNumberOfClusters, OFSizeOfCluster; // ��������� �����, �� ������� �������� ��������� ����
HANDLE hfTempFile = NULL; // ����� ���������� ����� (������������, ��� ����� ��� ������ / ����������)

wchar_t bufferInputFilePath[1025]; // ������ ���� � ��� ��������� ����� � UNICODE
char bufferInputFileName[1025]; // ������ ���� � ��� ��������� ����� � ANSI
wchar_t bufferOutputFilePath[1025]; // ������ ���� � ��� ���������� ����� � UNICODE
char bufferOutputFileName[1025]; // ������ ���� � ��� ���������� ����� � ANSI
wchar_t bufferTempFilePath[1025]; // ������ ���� � ��� ���������� ����� � UNICODE
char bufferTempFileName[1025]; // ������ ���� � ��� ���������� ����� � ANSI

int buf_dimension = 0; // ����������� ������� ���������. ���������� � ���������� ������
wchar_t *IFbuf; // ��������� �� ����� ��������� �����
int IFbuf_counter = 0; // ���������� ��������� � ������ ��������� �����
wchar_t *OFbuf; // ��������� �� ����� ���������� �����
int OFbuf_counter = 0; // ���������� ��������� � ������ ���������� �����
wchar_t *lpPathBuffer; // ��������� �� ����� ���������� �����

ReadWriteData::ReadWriteData(void)
{
}


ReadWriteData::~ReadWriteData(void)
{
}



// ����������� ���������� ���� ����������
void GetCPUCount(HWND _window)	// ��������� ���������� ���� ����������
{
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	numCPU = siSysInfo.dwNumberOfProcessors;
	if (numCPU == 1)
	{
		MessageBox(_window, L"��������� ����� ������ 1 ����", L"��������� �������", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
	}
	else
	{
		//wchar_t str[20];
		//_itow_s(numCPU, str, 10); // �������������� int � wchar_t: ����� (int), ����� (���� ���������), ������� ��������� (����������)
		//MessageBox(_window,str,L"���������� ���� ����������",MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
		setlocale(LC_CTYPE, "Russian");
		cout << "���������� ���� ����������: " << numCPU << endl << endl;
		setlocale(LC_ALL, "C");
	}
}

// �������� ��������� �����
bool GetInputFN(HWND _window)
{
	OPENFILENAME openInputFileName;

	memset(bufferInputFilePath, 0, 1025 * sizeof(wchar_t)); // ������� ������
	memset(&openInputFileName, 0, sizeof(OPENFILENAME)); // ��������� ������������� - ��� ����� - �� ��������� ��� �������������� ������ ���������

	openInputFileName.lStructSize = sizeof(OPENFILENAME);
	openInputFileName.hwndOwner = _window;
	openInputFileName.lpstrFilter = L"All\0*.*\0Text\0*.txt\0"; // ������ ��� ������, NULL - ������ �� ����������
	openInputFileName.nFilterIndex = 1;
	openInputFileName.lpstrFile = bufferInputFilePath; // ���������� ������ ���� � ��� ���������� �����
	openInputFileName.nMaxFile = 1024; // ������ ������ (� ������ - ��� ANSI, � �������� ��� Unicode), ������������� ������ ���� � ��� ���������� ����� - �� ������� ������, ��� sizeof(bufferInputFilePath)
	openInputFileName.lpstrTitle = L"�������� �������� ����";
	openInputFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&openInputFileName))
	{
		MessageBox(_window, L"���� �� ������!", L"��������� �������", MB_ICONINFORMATION);
		return FALSE;
	}

	// ����������� ������ ���� � ��� ����� �� LPCWSTR � char* � �������� � bufferInputFileName - ����������� ��� ������ / ����������
	memset(bufferInputFileName, 0, 1025 * sizeof(char)); // ������� ������
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferInputFilePath, -1, bufferInputFileName, sizeof(bufferInputFileName), NULL, NULL); // �������������� �����

	// ������� ��������� �����, �� ������� ���������� ��������� ����
	wchar_t bIFN[1025];
	wcscpy_s(bIFN, 1025, bufferInputFilePath);
	bIFN[3] = '\0'; //  ������, ���������� ��� �����, ������ ������������� ����� ("C:\"+"\0"), ������� ������ 0 � 3 �������, ������ �� 0, ������� ��� ����� ������ ���� �� ����� �����
	if (GetDiskFreeSpace(bIFN, &IFSectorPerCluster, &IFBytesPerSector, &IFNumberOfFreeClusters, &IFTotalNumberOfClusters) == 0)
	{
		MessageBox(_window, L"������ ����������� ���������� ����� ��� ��������� �����!", L"��������� �������", MB_ICONINFORMATION);
		return FALSE;
	}
	// ��������� ������ ��������
	IFSizeOfCluster = IFSectorPerCluster * IFBytesPerSector;

	return TRUE;
}

// �������� (��������) ���������� �����
bool GetOutputFN(HWND _window)
{
	OPENFILENAME openOutputFileName;

	memset(bufferOutputFilePath, 0, 1025 * sizeof(wchar_t)); // ������� ������
	memset(&openOutputFileName, 0, sizeof(OPENFILENAME)); // ��������� ������������� - ��� ����� - �� ��������� ��� �������������� ������ ���������

	openOutputFileName.lStructSize = sizeof(OPENFILENAME);
	openOutputFileName.hwndOwner = _window;
	openOutputFileName.lpstrFilter = L"All\0*.*\0Text\0*.txt\0"; // ������ ��� ������, NULL - ������ �� ����������
	openOutputFileName.nFilterIndex = 1;
	openOutputFileName.lpstrFile = bufferOutputFilePath; // ���������� ������ ���� � ��� ���������� �����
	openOutputFileName.nMaxFile = 1024; // ������ ������ (� ������ - ��� ANSI, � �������� ��� Unicode), ������������� ������ ���� � ��� ���������� ����� - �� ������� ������, ��� sizeof(bufferOutputFilePath)
	openOutputFileName.lpstrTitle = L"�������� ��������� ����";
	openOutputFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&openOutputFileName))
	{
		MessageBox(_window, L"���� �� ������!", L"��������� �������", MB_ICONINFORMATION);
		return FALSE;
	}

	// ����������� ������ ���� � ��� ����� �� LPCWSTR � char* � �������� � bufferOutputFileName - ����������� ��� ������ / ����������
	memset(bufferOutputFileName, 0, 1025 * sizeof(char)); // ������� ������
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferOutputFilePath, -1, bufferOutputFileName, sizeof(bufferOutputFileName), NULL, NULL); // �������������� �����

	// ������� ��������� �����, �� ������� ���������� ��������� ����
	wchar_t bOFN[1025];
	wcscpy_s(bOFN, 1025, bufferOutputFilePath);
	bOFN[3] = '\0'; //  ������, ���������� ��� �����, ������ ������������� ����� ("C:\"+"\0"), ������� ������ 0 � 3 �������, ������ �� 0, ������� ��� ����� ������ ���� �� ����� �����
	if (GetDiskFreeSpace(bOFN, &OFSectorPerCluster, &OFBytesPerSector, &OFNumberOfFreeClusters, &OFTotalNumberOfClusters) == 0)
	{
		MessageBox(_window, L"������ ����������� ���������� ����� ��� ���������� �����!", L"��������� �������", MB_ICONINFORMATION);
		return FALSE;
	}
	// ��������� ������ ��������
	OFSizeOfCluster = OFSectorPerCluster * OFBytesPerSector;

	return TRUE;
}

// �������� ���������� ����� (������������, ��� ����� ��� ������ / ����������)
bool GetTempFN(HWND _window)
{
	memset(bufferTempFilePath, 0, 1025 * sizeof(wchar_t)); // ������� ������
	// ������� ��������� ����
	GetTempPath(buf_dimension,		// ����� ������
		lpPathBuffer);				// ����� ��� ���� 

	// �������� ��������� ����. 
	GetTempFileName(lpPathBuffer,	// ������� ��� ��������� ������ 
		L"NEW",						// ������� ����� ���������� ����� 
		0,							// ������� ���������� ��� 
		bufferTempFilePath);		// ����� ��� �����

	// ����������� ������ ���� � ��� ����� �� LPCWSTR � char* � �������� � bufferTempFileName - ����������� ��� ������ / ����������
	memset(bufferTempFileName, 0, 1025 * sizeof(char)); // ������� ������
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferTempFilePath, -1, bufferTempFileName, sizeof(bufferTempFileName), NULL, NULL); // �������������� �����

	return TRUE;
}

// ����� ��������
int ToDoSelect(HWND _window)
{
	DWORD dwMessageBoxResult;

	dwMessageBoxResult = MessageBox(
		NULL,																							// ���������� ���� ���������
		L"������ � ����������� (��) | ������������� � ���������� (���) | ������ �� ������ (������)",	// ����� � ���� ���������
		L"�������� ��������",																			// ��������� � ���� ���������
		MB_YESNOCANCEL |																				// ����� ����� ���� ��������� (������ ��, ���, ������)
		MB_DEFBUTTON1 |																					// ��������� ������
		MB_ICONQUESTION |																				// ��� ������
		MB_DEFAULT_DESKTOP_ONLY
		);
	switch (dwMessageBoxResult)
	{
	case IDYES:
		return ToDoEncrypt;
	case IDNO:
		return ToDoDecrypt;
	case IDCANCEL:
		return ToDoCancel;
	}
	return ToDoCancel;
}


// ���������� ����� ������� ���� �����
int nok(int a, int b)
{
	int max = b;
	for (int i = max; i > 0; i++){

		if ((i % a == 0) && (i % b == 0))
			return i;
	}
	return 1;
}


// �������� ��� ���������� ������ ���������
void WhenProgramShutdown()
{
	// ������ �����
	DeleteFile(bufferTempFilePath);
}

int _tmain(int argc, _TCHAR* argv[])
{
/*	int ToDoSelector; // ��� ������: 1 - ��������� ���������� � ������, 2 - ��������� ������������ � ����������, NULL - ������ �� ������
	DWORD dwStart = 0; // ������ ������ �������������� ����� (��� ����� �����������, ��������� � ������� ������ �������)
	DWORD dwEnd = 0; // ������ ��������� �������������� ����� (��� ����� �����������, ��������� � ������� ������ �������)

	// ��������� ���������� ���� ����������
	GetCPUCount(NULL);

	// ��������� ������ ���� � ��� ��������� �����, ��������� �����
	if (!GetInputFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// ��������� ������ ���� � ��� ���������� �����, ��������� �����
	if (!GetOutputFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// ��������� ������ ������, ��� ���������� ����� ������� �� �������� ��������� ������, �� ������� �������� ��������� �������� � ��������� �����
	buf_dimension = nok(IFSizeOfCluster, OFSizeOfCluster);

	// �������� ����� ��������� �����
	IFbuf = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));
	// �������� ����� ���������� �����
	OFbuf = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));
	// �������� ����� ���������� �����
	lpPathBuffer = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));

	// ��������� ������ ���� � ��� ���������� �����
	if (!GetTempFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// ������� �������� (��������� ������ � ����������� / ��������� ������������� � ���������� / ������ �� ������)
	ToDoSelector = ToDoSelect(NULL);
	switch (ToDoSelector)
	{
	case ToDoEncrypt:
	{
						setlocale(LC_CTYPE, "Russian");
						cout << "��������� ������ � �����������" << endl << endl;
						setlocale(LC_ALL, "C");
						GetPass();
						dwStart = GetTickCount();

						if (!SyncEncode(NULL))
							break;

						dwEnd = GetTickCount();
						setlocale(LC_CTYPE, "Russian");
						cout << "����� �������������� ����� " << dwEnd - dwStart << " �����������" << endl << endl;
						setlocale(LC_ALL, "C");
						break;
	}
	case ToDoDecrypt:
	{
						setlocale(LC_CTYPE, "Russian");
						cout << "��������� ������������� � ����������" << endl << endl;
						setlocale(LC_ALL, "C");
						GetPass();
						dwStart = GetTickCount();

						if (!SyncDecode(NULL))
							break;

						dwEnd = GetTickCount();
						setlocale(LC_CTYPE, "Russian");
						cout << "����� �������������� ����� " << dwEnd - dwStart << " �����������" << endl << endl;
						setlocale(LC_ALL, "C");
						break;
	}
	case ToDoCancel:
	{
					   setlocale(LC_CTYPE, "Russian");
					   cout << "������ �� ������" << endl << endl;
					   setlocale(LC_ALL, "C");
					   break;
	}
	}

	// �������� ��� ���������� ������ ���������
	WhenProgramShutdown();
*/	return 0;
}



