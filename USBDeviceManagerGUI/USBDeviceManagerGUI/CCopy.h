#pragma once
#include "USBDeviceManagerGUI.h"

// CCopy 对话框

class CCopy : public CDialogEx
{
	DECLARE_DYNAMIC(CCopy)

public:
	CCopy(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCopy();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COPY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	DECLARE_MESSAGE_MAP()
public:
    CString* pSource;
    CString* pDest;
    CStatic labelSource;
    CStatic labelDest;
    CEdit editSource;
    CEdit editDest;
    CButton buttonFile;
    CButton buttonFolder;
    CButton buttonBrowse;
    enum SOURCE_TYPE {UNDEFINED, TYPE_FILE, TYPE_FOLDER} source_type=SOURCE_TYPE::UNDEFINED;
    HWND mainWindow;
    TCHAR letter;
    afx_msg void OnBnClickedOk();
    afx_msg void OnClickedButtonFile();
    afx_msg void OnClickedButtonFolder();
    afx_msg void OnClickedButtonBrowse();
};