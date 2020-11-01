
// USBDeviceManagerGUI.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once
/*
*/
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CUSBDeviceManagerGUIApp:
// 有关此类的实现，请参阅 USBDeviceManagerGUI.cpp
//

class CUSBDeviceManagerGUIApp : public CWinApp
{
public:
	CUSBDeviceManagerGUIApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
};

extern CUSBDeviceManagerGUIApp theApp;

void ErrorInfo();