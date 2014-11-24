//#include "stdafx.h"
#include "ReadWriteData.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

// определение количества ядер процессора
void ReadWriteData::GetCPUCount(HWND _window)	// определим количество ядер процессора
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
bool ReadWriteData::GetInputFN(HWND _window)
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
bool ReadWriteData::GetOutputFN(HWND _window)
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
bool ReadWriteData::GetTempFN(HWND _window)
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