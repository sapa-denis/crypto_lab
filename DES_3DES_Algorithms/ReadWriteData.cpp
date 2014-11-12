#include "stdafx.h"
#include "ReadWriteData.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

#define ToDoEncrypt 1 // шифрование и сжатие
#define ToDoDecrypt 2 // дешифрование и распаковка
#define ToDoCancel NULL // ничего не делать
#define PasswordLenght 30 // длина пароля

int numCPU = 0; // количество ядер процессора
HANDLE hfInputFile = NULL; // хендл входящего файла
DWORD IFSectorPerCluster, IFBytesPerSector, IFNumberOfFreeClusters, IFTotalNumberOfClusters, IFSizeOfCluster; // параметры диска, на котором хранится входящий файл
HANDLE hfOutputFile = NULL; // хендл исходящего файла
DWORD OFSectorPerCluster, OFBytesPerSector, OFNumberOfFreeClusters, OFTotalNumberOfClusters, OFSizeOfCluster; // параметры диска, на котором хранится исходящий файл
HANDLE hfTempFile = NULL; // хендл временного файла (используется, как буфер при сжатии / распаковке)

wchar_t bufferInputFilePath[1025]; // полный путь и имя входящего файла в UNICODE
char bufferInputFileName[1025]; // полный путь и имя входящего файла в ANSI
wchar_t bufferOutputFilePath[1025]; // полный путь и имя исходящего файла в UNICODE
char bufferOutputFileName[1025]; // полный путь и имя исходящего файла в ANSI
wchar_t bufferTempFilePath[1025]; // полный путь и имя временного файла в UNICODE
char bufferTempFileName[1025]; // полный путь и имя временного файла в ANSI

int buf_dimension = 0; // размерность буферов входящего. исходящего и временного файлов
wchar_t *IFbuf; // указатель на буфер входящего файла
int IFbuf_counter = 0; // количество элементов в буфере входящего файла
wchar_t *OFbuf; // указатель на буфер исходящего файла
int OFbuf_counter = 0; // количество элементов в буфере исходящего файла
wchar_t *lpPathBuffer; // указатель на буфер временного файла

ReadWriteData::ReadWriteData(void)
{
}


ReadWriteData::~ReadWriteData(void)
{
}



// определение количества ядер процессора
void GetCPUCount(HWND _window)	// определим количество ядер процессора
{
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	numCPU = siSysInfo.dwNumberOfProcessors;
	if (numCPU == 1)
	{
		MessageBox(_window, L"Процессор имеет только 1 ядро", L"Сообщение системы", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
	}
	else
	{
		//wchar_t str[20];
		//_itow_s(numCPU, str, 10); // преобразование int в wchar_t: число (int), буфер (куда сохранить), система счисления (десятичная)
		//MessageBox(_window,str,L"Количество ядер процессора",MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
		setlocale(LC_CTYPE, "Russian");
		cout << "Количество ядер процессора: " << numCPU << endl << endl;
		setlocale(LC_ALL, "C");
	}
}

// открытие входящего файла
bool GetInputFN(HWND _window)
{
	OPENFILENAME openInputFileName;

	memset(bufferInputFilePath, 0, 1025 * sizeof(wchar_t)); // очистка буфера
	memset(&openInputFileName, 0, sizeof(OPENFILENAME)); // начальная инициализация - все нулем - по умолчанию для неиспользуемых членов структуры

	openInputFileName.lStructSize = sizeof(OPENFILENAME);
	openInputFileName.hwndOwner = _window;
	openInputFileName.lpstrFilter = L"All\0*.*\0Text\0*.txt\0"; // фильтр для выбора, NULL - фильтр не установлен
	openInputFileName.nFilterIndex = 1;
	openInputFileName.lpstrFile = bufferInputFilePath; // возвращает полный путь и имя выбранного файла
	openInputFileName.nMaxFile = 1024; // размер буфера (в байтах - для ANSI, в символах для Unicode), возвращающего полный путь и имя выбранного файла - на единицу меньше, чем sizeof(bufferInputFilePath)
	openInputFileName.lpstrTitle = L"Выберите входящий файл";
	openInputFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&openInputFileName))
	{
		MessageBox(_window, L"Файл не выбран!", L"Сообщение системы", MB_ICONINFORMATION);
		return FALSE;
	}

	// преобразуем полный путь и имя файла из LPCWSTR к char* и сохраним в bufferInputFileName - потребуется при сжатии / распаковке
	memset(bufferInputFileName, 0, 1025 * sizeof(char)); // очистка буфера
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferInputFilePath, -1, bufferInputFileName, sizeof(bufferInputFileName), NULL, NULL); // преобразование типов

	// получим параметры диска, на котором расположен выбранный файл
	wchar_t bIFN[1025];
	wcscpy_s(bIFN, 1025, bufferInputFilePath);
	bIFN[3] = '\0'; //  строка, содержащая имя диска, должна заканчиваться нулем ("C:\"+"\0"), добавим символ 0 в 3 позицию, считая от 0, обрезав тем самым полный путь до имени диска
	if (GetDiskFreeSpace(bIFN, &IFSectorPerCluster, &IFBytesPerSector, &IFNumberOfFreeClusters, &IFTotalNumberOfClusters) == 0)
	{
		MessageBox(_window, L"Ошибка определения параметров диска для входящего файла!", L"Сообщение системы", MB_ICONINFORMATION);
		return FALSE;
	}
	// определим размер кластера
	IFSizeOfCluster = IFSectorPerCluster * IFBytesPerSector;

	return TRUE;
}

// открытие (создание) исходящего файла
bool GetOutputFN(HWND _window)
{
	OPENFILENAME openOutputFileName;

	memset(bufferOutputFilePath, 0, 1025 * sizeof(wchar_t)); // очистка буфера
	memset(&openOutputFileName, 0, sizeof(OPENFILENAME)); // начальная инициализация - все нулем - по умолчанию для неиспользуемых членов структуры

	openOutputFileName.lStructSize = sizeof(OPENFILENAME);
	openOutputFileName.hwndOwner = _window;
	openOutputFileName.lpstrFilter = L"All\0*.*\0Text\0*.txt\0"; // фильтр для выбора, NULL - фильтр не установлен
	openOutputFileName.nFilterIndex = 1;
	openOutputFileName.lpstrFile = bufferOutputFilePath; // возвращает полный путь и имя выбранного файла
	openOutputFileName.nMaxFile = 1024; // размер буфера (в байтах - для ANSI, в символах для Unicode), возвращающего полный путь и имя выбранного файла - на единицу меньше, чем sizeof(bufferOutputFilePath)
	openOutputFileName.lpstrTitle = L"Выберите исходящий файл";
	openOutputFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&openOutputFileName))
	{
		MessageBox(_window, L"Файл не выбран!", L"Сообщение системы", MB_ICONINFORMATION);
		return FALSE;
	}

	// преобразуем полный путь и имя файла из LPCWSTR к char* и сохраним в bufferOutputFileName - потребуется при сжатии / распаковке
	memset(bufferOutputFileName, 0, 1025 * sizeof(char)); // очистка буфера
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferOutputFilePath, -1, bufferOutputFileName, sizeof(bufferOutputFileName), NULL, NULL); // преобразование типов

	// получим параметры диска, на котором расположен выбранный файл
	wchar_t bOFN[1025];
	wcscpy_s(bOFN, 1025, bufferOutputFilePath);
	bOFN[3] = '\0'; //  строка, содержащая имя диска, должна заканчиваться нулем ("C:\"+"\0"), добавим символ 0 в 3 позицию, считая от 0, обрезав тем самым полный путь до имени диска
	if (GetDiskFreeSpace(bOFN, &OFSectorPerCluster, &OFBytesPerSector, &OFNumberOfFreeClusters, &OFTotalNumberOfClusters) == 0)
	{
		MessageBox(_window, L"Ошибка определения параметров диска для исходящего файла!", L"Сообщение системы", MB_ICONINFORMATION);
		return FALSE;
	}
	// определим размер кластера
	OFSizeOfCluster = OFSectorPerCluster * OFBytesPerSector;

	return TRUE;
}

// создание временного файла (используется, как буфер при сжатии / распаковке)
bool GetTempFN(HWND _window)
{
	memset(bufferTempFilePath, 0, 1025 * sizeof(wchar_t)); // очистка буфера
	// Получим временный путь
	GetTempPath(buf_dimension,		// длина буфера
		lpPathBuffer);				// буфер для пути 

	// Создадим временный файл. 
	GetTempFileName(lpPathBuffer,	// каталог для временных файлов 
		L"NEW",						// префикс имени временного файла 
		0,							// создаем уникальное имя 
		bufferTempFilePath);		// буфер для имени

	// преобразуем полный путь и имя файла из LPCWSTR к char* и сохраним в bufferTempFileName - потребуется при сжатии / распаковке
	memset(bufferTempFileName, 0, 1025 * sizeof(char)); // очистка буфера
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bufferTempFilePath, -1, bufferTempFileName, sizeof(bufferTempFileName), NULL, NULL); // преобразование типов

	return TRUE;
}

// выбор действия
int ToDoSelect(HWND _window)
{
	DWORD dwMessageBoxResult;

	dwMessageBoxResult = MessageBox(
		NULL,																							// дескриптор окна владельца
		L"Сжатие и кодирование (Да) | Декодирование и распаковка (Нет) | Ничего не делать (Отмена)",	// текст в окне сообщений
		L"Выберите действие",																			// заголовок в окне сообщений
		MB_YESNOCANCEL |																				// флаги стиля окна сообщений (кнопки Да, Нет, Отмена)
		MB_DEFBUTTON1 |																					// выделеная кнопка
		MB_ICONQUESTION |																				// тип иконки
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


// наименьшее общее кратное двух чисел
int nok(int a, int b)
{
	int max = b;
	for (int i = max; i > 0; i++){

		if ((i % a == 0) && (i % b == 0))
			return i;
	}
	return 1;
}


// действия при завершении работы программы
void WhenProgramShutdown()
{
	// убрать мусор
	DeleteFile(bufferTempFilePath);
}

int _tmain(int argc, _TCHAR* argv[])
{
/*	int ToDoSelector; // что делать: 1 - выполнять шифрование и сжатие, 2 - выполнять дешифрование и распаковку, NULL - ничего не делать
	DWORD dwStart = 0; // момент начала преобразования файла (как число миллисекунд, прошедшее с момента старта системы)
	DWORD dwEnd = 0; // момент окончания преобразования файла (как число миллисекунд, прошедшее с момента старта системы)

	// определим количество ядер процессора
	GetCPUCount(NULL);

	// определим полный путь и имя входящего файла, параметры диска
	if (!GetInputFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// определим полный путь и имя исходящего файла, параметры диска
	if (!GetOutputFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// установим размер буфера, как наименьшее общее кратное от размеров кластеров дисков, на которых хранятся выбранные входящий и исходящий файлы
	buf_dimension = nok(IFSizeOfCluster, OFSizeOfCluster);

	// создадим буфер входящего файла
	IFbuf = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));
	// создадим буфер исходящего файла
	OFbuf = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));
	// создадим буфер временного файла
	lpPathBuffer = (wchar_t *)malloc(buf_dimension * sizeof (wchar_t));

	// определим полный путь и имя временного файла
	if (!GetTempFN(NULL))
	{
		WhenProgramShutdown();
		return 0;
	}

	// выберем действие (выполнять сжатие и кодирование / выполнять декодирование и распаковку / ничего не делать)
	ToDoSelector = ToDoSelect(NULL);
	switch (ToDoSelector)
	{
	case ToDoEncrypt:
	{
						setlocale(LC_CTYPE, "Russian");
						cout << "Выполняем сжатие и кодирование" << endl << endl;
						setlocale(LC_ALL, "C");
						GetPass();
						dwStart = GetTickCount();

						if (!SyncEncode(NULL))
							break;

						dwEnd = GetTickCount();
						setlocale(LC_CTYPE, "Russian");
						cout << "Время преобразования файла " << dwEnd - dwStart << " миллисекунд" << endl << endl;
						setlocale(LC_ALL, "C");
						break;
	}
	case ToDoDecrypt:
	{
						setlocale(LC_CTYPE, "Russian");
						cout << "Выполняем декодирование и распаковку" << endl << endl;
						setlocale(LC_ALL, "C");
						GetPass();
						dwStart = GetTickCount();

						if (!SyncDecode(NULL))
							break;

						dwEnd = GetTickCount();
						setlocale(LC_CTYPE, "Russian");
						cout << "Время преобразования файла " << dwEnd - dwStart << " миллисекунд" << endl << endl;
						setlocale(LC_ALL, "C");
						break;
	}
	case ToDoCancel:
	{
					   setlocale(LC_CTYPE, "Russian");
					   cout << "Ничего не делаем" << endl << endl;
					   setlocale(LC_ALL, "C");
					   break;
	}
	}

	// действия при завершении работы программы
	WhenProgramShutdown();
*/	return 0;
}



