#pragma once
class ReadWriteData
{
public:
	ReadWriteData(void);
	
private:
	void GetCPUCount(HWND _window);
	bool GetTempFN(HWND _window);
	bool GetOutputFN(HWND _window);
	bool GetInputFN(HWND _window);
};

