// OS_Lab4.cpp: определяет точку входа для консольного приложения.
//

/*
Задание

Написать программу (консольное приложение), которая заданный пользователем файл шифрует с применением метода гаммирования на основе пользовательского пароля и сразу же сжимает методом RLE или выполняет обратное действие (распаковывает и расшифровывает заданный файл) Выбор выполняемого действия необходимо реализовать через вызов соответствующей функции MessageBox. Данные, полученные в результате преобразований, записываются в выходной файл (также выбирается пользователем).
При написании программы необходимо создавать не менее 2-ух потоков (в случае 1-процессороной системы перед созданием потоков выдается с помощью функции MessageBox предупреждение) и осуществить соответствующую программную оптимизацию с целью повышения производительности за счет использования дополнительных потоков.
Пользователь вводит имена входного и выходного файлов, пароль и выбирает действие.
Для выбора имени входного файла использовать функцию GetOpenFileName, для выбора имени выходного файла использовать функцию GetSaveFileName, имя выходного файла вводится перед началом процесса преобразования. При открытии файла ОБЯЗАТЕЛЬНО проверить удачность этой операции.
Ввод-вывод осуществляется ТОЛЬКО с применением функций WinAPI.
Размер буфера при выполнении операций ввода-вывода НЕОБХОДИМО выбрать равным размеру кластера на диске, с которым производится работа (имя диска содержится в полном пути к файлу). Если входной и выходной файл расположены на дисках, размеры кластеров которых различны, то размер буфера должен представлять собой НАИМЕНЬШЕЕ ОБЩЕЕ КРАТНОЕ для этих размеров кластеров.
Ввод пароля производится обычными средствами консольного приложения. Предполагается, что длина пароля не превышает 30 символов.
Все остальные информационные сообщения в программе выводятся на экран (в консоли). Программа должна выводить на экран время, затраченное на процесс преобразования файла.

При написании программы использовать ООП или процедурно-ориентированное программирование (каждое действие реализуется в виде функции, если это целесообразно). Все данные в функцию передаются через ее аргументы.
Вся выводимая информация должна сопровождаться пояснениями.

Краткое описание предложенных методов шифрования и сжатия

Шифрование методом гаммирования

Суть метода состоит в том, что символы шифруемого текста последовательно складываются с символами некоторой специальной последовательности, называемой гаммой (в качестве этой гаммы может выступать пароль пользователя). Иногда такой метод представляют как наложение гаммы на исходный текст, поэтому он получил название "гаммирование".
Наложение гаммы можно осуществить несколькими способами, например по формуле:
,
где ,  и   очередные символы шифрованного, исходного текста и пароля пользователя, соответственно; ^  операция побитового исключающего «ИЛИ». Расшифровка происходит по этой же формуле. Символы пароля пользователя последовательно перебираются от 1-го до последнего, а затем снова происходит переход на 1-ый символ и т.д.

Сжатие методом RLE

Метода RLE (Run Length Encoding  кодирование по длинам серий)  один из широко распространенных и очень простых способов кодирования, который применяется для сжатия растровых графических изображений (в форматах графических файлов BMP, PCX, TIF, GIF).
Суть метода заключается в замене цепочек (последовательностей) повторяющихся байт двумя значениями: количеством повторений одного байта информации и, собственно, самим значением байта информации.
Если имеется некоторая последовательность неповторяющихся байт, то ставится символ с кодом 0, затем  количество неповторяющихся байт и дальше идет сама последовательность.

*/

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>


#define ToDoEncrypt 1 // шифрование и сжатие
#define ToDoDecrypt 2 // дешифрование и распаковка
#define ToDoCancel NULL // ничего не делать
#define PasswordLenght 30 // длина пароля

#define THREADCOUNT 2									// количество потоков
#define NumThreadForWriteToBufer 0						// порядковый номер потока для записи в буфер
#define NumThreadForReadFromBufer 1						// порядковый номер потока для чтения из буфера
#define WaitingPeriod INFINITE							// период ожидания бесконечен


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

char password[PasswordLenght]; // пароль

using std::cin;
using std::cout;
using std::endl;

// RLE //////////////////////////////////////////////////////
int byte_stored_status = FALSE;
int val_byte_stored;

FILE *source_file, *dest_file;

// Псевдопроцедуры для RLE
#define end_of_data()  (byte_stored_status?FALSE:!(byte_stored_status=((val_byte_stored=fgetc(source_file))!=EOF)))
#define read_byte()  (byte_stored_status?byte_stored_status=FALSE,(unsigned char)val_byte_stored:(unsigned char)fgetc(source_file))
#define write_byte(byte)  ((void)fputc((byte),dest_file))
#define write_array(array,byte_nb_to_write)  ((void)fwrite((array),1,(byte_nb_to_write),dest_file))
#define write_block(byte,time_nb)  { unsigned char array_to_write[129];\
	(void)memset(array_to_write, (byte), (time_nb)); \
	write_array(array_to_write, (time_nb)); \
}

void rle1encoding()
/* Returned parameters: None
Action: Compresses with RLE type 1 method all bytes read by the function 'read_byte'
Errors: An input/output error could disturb the running of the program
*/
{
	register unsigned char byte1, byte2, frame_size,
		array[129];

	if (!end_of_data())
	{
		byte1 = read_byte();    /* Is there at least a byte to analyze? */
		frame_size = 1;
		if (!end_of_data())
			/* Are there at least two bytes to analyze? */
		{
			byte2 = read_byte();
			frame_size = 2;
			do {
				if (byte1 == byte2)
					/* Is there a repetition? */
				{
					while ((!end_of_data()) && (byte1 == byte2) && (frame_size < 129))
					{
						byte2 = read_byte();
						frame_size++;
					}
					if (byte1 == byte2)
						/* Do we meet only a sequence of bytes? */
					{
						write_byte(126 + frame_size);
						write_byte(byte1);
						if (!end_of_data())
						{
							byte1 = read_byte();
							frame_size = 1;
						}
						else frame_size = 0;
					}
					else   /* No, then don't handle the last byte */
					{
						write_byte(125 + frame_size);
						write_byte(byte1);
						byte1 = byte2;
						frame_size = 1;
					}
					if (!end_of_data())
					{
						byte2 = read_byte();
						frame_size = 2;
					}
				}
				else        /* Prepare the array of comparisons
							where will be stored all the identical bytes */
				{
					*array = byte1;
					array[1] = byte2;
					while ((!end_of_data()) && (array[frame_size - 2] != array[frame_size - 1]) && (frame_size < 128))
					{
						array[frame_size] = read_byte();
						frame_size++;
					}
					if (array[frame_size - 2] == array[frame_size - 1])
						/* Do we meet a sequence of all different bytes followed by identical byte? */
					{ /* Yes, then don't count the two last bytes */
						write_byte(frame_size - 3);
						write_array(array, frame_size - 2);
						byte1 = array[frame_size - 2];
						byte2 = byte1;
						frame_size = 2;
					}
					else {
						write_byte(frame_size - 1);
						write_array(array, frame_size);
						if (end_of_data())
							frame_size = 0;
						else {
							byte1 = read_byte();
							if (end_of_data())
								frame_size = 1;
							else {
								byte2 = read_byte();
								frame_size = 2;
							}
						}
					}
				}
			} while ((!end_of_data()) || (frame_size >= 2));
		}
		if (frame_size == 1)
		{
			write_byte(0);
			write_byte(byte1);
		}
	}
}

void rle1decoding()
/* Returned parameters: None
Action: Decompresses with RLE type 1 method all bytes read by the function read_byte
Erreurs: An input/output error could disturb the running of the program
*/
{
	unsigned char header;
	register unsigned char i;

	while (!end_of_data())
	{
		header = read_byte();
		switch (header & 128)
		{
		case 0:if (!end_of_data())
			for (i = 0; i <= header; i++)
				write_byte(read_byte());
			/* else INVALID FILE */
			break;
		case 128:if (!end_of_data())
			write_block(read_byte(), (header & 127) + 2);
			/* else INVALID FILE */
		}
	}
}
/////////////////////////////////////////////////////////////

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

// ввод пароля с клавиатуры с выводом на экран звездочек вместо вводимых символов
void GetPass()
{
	char c;
	setlocale(LC_CTYPE, "Russian");
	cout << "Введите пароль (максимум 30 символов):\n";
	setlocale(LC_ALL, "C");
	for (int i = 0; i < PasswordLenght - 1; ++i)
	{
		c = _getch();
		if (c == '\r')
		{
			break;
		}
		password[i] = c;
		_putch('*');
	}
	password[PasswordLenght - 1] = '\0';
	puts("\r\n\n");
	//_getch(); // ждем ввода символа с клавиатуры
}

// действия при завершении работы программы
void WhenProgramShutdown()
{
	// убрать мусор
	DeleteFile(bufferTempFilePath);
}



// сжатие по алгоритму RLE и кодирование методом гаммирования
// сжимаем входящий файл во временный файл
// кодируем временный файл методом гаммирования и записываем результат в исходящий файл
bool SyncEncode(HWND _window)
{
	// сжатие RLE
	errno_t err_source_file, err_dest_file;
	err_source_file = fopen_s(&source_file, bufferInputFileName, "rb");
	if (err_source_file != 0)
	{
		setlocale(LC_CTYPE, "Russian");
		cout << "Ошибка открытия входящего файла в алгоритме RLE перед кодированием" << endl << endl;
		setlocale(LC_ALL, "C");
		WhenProgramShutdown();
		return FALSE;
	}
	err_dest_file = fopen_s(&dest_file, bufferTempFileName, "wb+");
	if (err_dest_file != 0)
	{
		setlocale(LC_CTYPE, "Russian");
		cout << "Ошибка открытия временного файла в алгоритме RLE перед кодированием" << endl << endl;
		setlocale(LC_ALL, "C");
		WhenProgramShutdown();
		return FALSE;
	}
	rle1encoding();
	fclose(source_file);
	fclose(dest_file);

	// кодирование методом гаммирования
	hfTempFile = CreateFile(bufferTempFilePath,	// имя файла 
		GENERIC_READ,							// открыт для чтения
		FILE_SHARE_READ,						// совместное чтение
		NULL,									// защита по умолчанию 
		OPEN_EXISTING,							// только существующий файл
		FILE_ATTRIBUTE_NORMAL,					// атрибуты обычного файла 
		NULL);									// шаблона атрибутов нет 

	if (hfTempFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_window, L"Создание временного файла", L"Ошибка", MB_ICONHAND | MB_OK);
		return FALSE;
	}

	hfOutputFile = CreateFile(bufferOutputFilePath, // имя файла
		GENERIC_WRITE, // открыт для записи
		FILE_SHARE_WRITE, // совместная запись
		NULL, // защита по умолчанию
		OPEN_ALWAYS, // как действовать (если файл не существует - он создается)
		FILE_ATTRIBUTE_NORMAL, // атрибуты обычного файла
		NULL); // шаблона атрибутов нет
	if (hfOutputFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_window, L"Создание исходящего файла", L"Ошибка", MB_ICONHAND | MB_OK);
		return FALSE;
	}

	DWORD  dwBytesRead, dwBytesWritten;
	do
	{
		if (ReadFile(hfTempFile,			// хедер временного файла - буфера
			IFbuf,							// буфер, принимающий считываемые данные
			buf_dimension,					// размерность буфера, принимающего данные
			&dwBytesRead,					// сколько байт реально прочитано
			NULL))							// ссылка на структуру OVERLAPPED, которая должна существовать и быть проинициализирована при асинхронном чтении, при синхронном чтении - NULL
		{
			//Gamma(0, buf_dimension);		// кодирование - декодирование данных методом гаммирования



			HANDLE aThread[THREADCOUNT];
			DWORD ThreadID;

			// создаем поток кодирования данных методом гаммирования(первая половина буфера)
			aThread[NumThreadForWriteToBufer] = CreateThread(
				NULL,       // атрибуты безопасности по умолчанию
				0,          // размер стека по умолчанию
				(LPTHREAD_START_ROUTINE)ThrGammaLO,
				NULL,       // без передачи аргументов функции
				0,          // начинать выполнение немедленно после создания потока
				&ThreadID); // указатель на 32-разрядную переменную, которая получит идентификатор созданного потока

			if (aThread[NumThreadForWriteToBufer] == NULL)
			{
				printf("CreateThread WriteToBufer error: %d\n", GetLastError());
				return 1;
			}

			// создаем поток кодирования данных методом гаммирования(вторая половина буфера)
			aThread[NumThreadForReadFromBufer] = CreateThread(
				NULL,       // атрибуты безопасности по умолчанию
				0,          // размер стека по умолчанию
				(LPTHREAD_START_ROUTINE)ThrGammaHI,
				NULL,       // без передачи аргументов функции
				0,          // начинать выполнение немедленно после создания потока
				&ThreadID); // указатель на 32-разрядную переменную, которая получит идентификатор созданного потока

			if (aThread[NumThreadForReadFromBufer] == NULL)
			{
				printf("CreateThread ReadFromBufer error: %d\n", GetLastError());
				return 1;
			}

			// к этому моменту потоки созданы и запущены

			// ждем завершения роботы обоих потоков
			WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, WaitingPeriod);

			// закрываем потоки
			for (int i = 0; i < THREADCOUNT; i++)
				CloseHandle(aThread[i]);






			WriteFile(hfOutputFile,			// хедер исходящего файла
				OFbuf,						// буфер, содержащий данные для записи
				dwBytesRead,				// размерность буфера, содержащего данные для записи (сколько байт из буфера писать - если в процессе преобразования размер файла изменяется (упаковка, распаковка) - использовать strlen(OFBuf))
				&dwBytesWritten,			// сколько байт реально записано
				NULL);						// ссылка на структуру OVERLAPPED, которая должна существовать и быть проинициализирована при асинхроной записи, при синхронной записи - NULL
		}
	} while (dwBytesRead == buf_dimension);

	CloseHandle(hfTempFile);
	CloseHandle(hfOutputFile);

	return TRUE;
}

// декодирование методом гаммирования и распаковка по алгоритму RLE
// Читаем входящий файл в буфер
// Декодируем методом гаммирования 
// Записываем буфер во временный файл
// Распаковываем временный файл в исходящий по алгоритму RLE
bool SyncDecode(HWND _window)
{
	// декодирование методом гаммирования
	hfInputFile = CreateFile(bufferInputFilePath,	// имя файла
		GENERIC_READ,							// открыт для чтения
		FILE_SHARE_READ,						// совместное чтение
		NULL,									// защита по умолчанию
		OPEN_EXISTING,							// только существующий файл
		FILE_ATTRIBUTE_NORMAL,					// атрибуты обычного файла
		NULL);									// шаблона атрибутов нет
	if (hfInputFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_window, L"Открытие входящего файла", L"Ошибка", MB_ICONHAND | MB_OK);
		return FALSE;
	}

	hfTempFile = CreateFile(bufferTempFilePath,	// имя файла 
		GENERIC_WRITE,							// открыт для записи
		FILE_SHARE_WRITE,						// совместная запись
		NULL,									// защита по умолчанию 
		CREATE_ALWAYS,							// переписывать существующий файл
		FILE_ATTRIBUTE_NORMAL,					// атрибуты обычного файла 
		NULL);									// шаблона атрибутов нет 

	if (hfTempFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_window, L"Создание временного файла", L"Ошибка", MB_ICONHAND | MB_OK);
		return FALSE;
	}

	DWORD  dwBytesRead, dwBytesWritten;
	do
	{
		if (ReadFile(hfInputFile, // хедер входящего файла
			IFbuf, // буфер, принимающий считываемые данные
			buf_dimension, // размерность буфера, принимающего данные
			&dwBytesRead, // сколько байт реально прочитано
			NULL)) // ссылка на структуру OVERLAPPED, которая должна существовать и быть проинициализирована при асинхронном чтении, при синхронном чтении - NULL
		{
			//Gamma(0, buf_dimension); // кодирование - декодированиеrpq данных методом гаммирования




			HANDLE aThread[THREADCOUNT];
			DWORD ThreadID;

			// создаем поток декодирования данных методом гаммирования(первая половина буфера)
			aThread[NumThreadForWriteToBufer] = CreateThread(
				NULL,       // атрибуты безопасности по умолчанию
				0,          // размер стека по умолчанию
				(LPTHREAD_START_ROUTINE)ThrGammaLO,
				NULL,       // без передачи аргументов функции
				0,          // начинать выполнение немедленно после создания потока
				&ThreadID); // указатель на 32-разрядную переменную, которая получит идентификатор созданного потока

			if (aThread[NumThreadForWriteToBufer] == NULL)
			{
				printf("CreateThread WriteToBufer error: %d\n", GetLastError());
				return 1;
			}

			// создаем поток декодирования данных методом гаммирования(вторая половина буфера)
			aThread[NumThreadForReadFromBufer] = CreateThread(
				NULL,       // атрибуты безопасности по умолчанию
				0,          // размер стека по умолчанию
				(LPTHREAD_START_ROUTINE)ThrGammaHI,
				NULL,       // без передачи аргументов функции
				0,          // начинать выполнение немедленно после создания потока
				&ThreadID); // указатель на 32-разрядную переменную, которая получит идентификатор созданного потока

			if (aThread[NumThreadForReadFromBufer] == NULL)
			{
				printf("CreateThread ReadFromBufer error: %d\n", GetLastError());
				return 1;
			}

			// к этому моменту потоки созданы и запущены

			// ждем завершения роботы обоих потоков
			WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, WaitingPeriod);

			// закрываем потоки
			for (int i = 0; i < THREADCOUNT; i++)
				CloseHandle(aThread[i]);






			WriteFile(hfTempFile, // хедер временного файла - буфера
				OFbuf, // буфер, содержащий данные для записи
				dwBytesRead, // размерность буфера, содержащего данные для записи (сколько байт из буфера писать - если в процессе преобразования размер файла изменяется (упаковка, распаковка) - использовать strlen(OFBuf))
				&dwBytesWritten, // сколько байт реально записано
				NULL);  // ссылка на структуру OVERLAPPED, которая должна существовать и быть проинициализирована при асинхроной записи, при синхронной записи - NULL
		}
	} while (dwBytesRead == buf_dimension);

	CloseHandle(hfInputFile);
	CloseHandle(hfTempFile);

	// распаковка RLE
	errno_t err_source_file, err_dest_file;
	err_source_file = fopen_s(&source_file, bufferTempFileName, "rb");
	if (err_source_file != 0)
	{
		setlocale(LC_CTYPE, "Russian");
		cout << "Ошибка открытия временного файла в алгоритме RLE после декодирования" << endl << endl;
		setlocale(LC_ALL, "C");
		WhenProgramShutdown();
		return FALSE;
	}
	err_dest_file = fopen_s(&dest_file, bufferOutputFileName, "wb+");
	if (err_dest_file != 0)
	{
		setlocale(LC_CTYPE, "Russian");
		cout << "Ошибка открытия исходящего файла в алгоритме RLE после декодирования" << endl << endl;
		setlocale(LC_ALL, "C");
		WhenProgramShutdown();
		return FALSE;
	}
	rle1decoding();
	fclose(source_file);
	fclose(dest_file);

	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int ToDoSelector; // что делать: 1 - выполнять шифрование и сжатие, 2 - выполнять дешифрование и распаковку, NULL - ничего не делать
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
	return 0;
}
