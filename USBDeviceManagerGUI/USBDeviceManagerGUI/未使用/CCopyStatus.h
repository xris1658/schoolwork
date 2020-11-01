#pragma once
//#include <afxdialogex.h>
#include "USBDeviceManagerCore.h"
#include "resource.h"
class CCopyStatus : public CDialogEx
{
    DECLARE_DYNAMIC(CCopyStatus)
public:
    CCopyStatus(CWnd* pParent = nullptr);
    CCopyStatus(CWnd* pParent, LPTSTR _arraySource, LPTSTR _arrayDest);
    virtual ~CCopyStatus();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_COPY_STATUS };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
public:
    CStatic labelFrom;
    CLinkCtrl linkSource;
    CStatic labelTo;
    CLinkCtrl linkDest;
    CStatic labelStatus;
    CProgressCtrl progressStatus;
    LPTSTR arraySource;
    LPTSTR arrayDest;
    BOOL PreTranslateMessage(MSG* pMsg);
    bool ready=false;
    HWND mainWindow;
    CWnd* pMainWnd;
    int currentPercentage=0;
    void UpdateValue(double value);
private:
    FileCopy fileCopy;
protected:
    afx_msg LRESULT OnCopyfileNotify(WPARAM wParam, LPARAM lParam);
    virtual void OnCancel();
public:
    afx_msg void OnNMClickSourcedir(NMHDR *pNMHDR, LRESULT *pResult);
};
