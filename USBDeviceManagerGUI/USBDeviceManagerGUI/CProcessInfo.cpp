// CProcessInfo.cpp: 实现文件
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "CProcessInfo.h"
#include "afxdialogex.h"
#include "resource.h"


// CProcessInfo 对话框

IMPLEMENT_DYNAMIC(CProcessInfo, CDialogEx)

CProcessInfo::CProcessInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESSINFO, pParent)
{

}
CProcessInfo::CProcessInfo(CWnd* pParent, CString _pathName, CString _startupTime, CString _pid)
    : CDialogEx(IDD_PROCESSINFO, pParent)
{
    pathName = _pathName;
    startupTime = _startupTime;
    pid = _pid;
}

CProcessInfo::~CProcessInfo()
{
}

void CProcessInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PROCESSINFO, listProcessInfo);
}


BEGIN_MESSAGE_MAP(CProcessInfo, CDialogEx)
END_MESSAGE_MAP()


// CProcessInfo 消息处理程序


BOOL CProcessInfo::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    listProcessInfo.InsertColumn(0, TEXT("信息类型"), 0, 100);
    listProcessInfo.InsertColumn(1, TEXT("信息值"), 0, 600);
    listProcessInfo.InsertItem(LVIF_TEXT, 0, TEXT("程序路径名"), 0, 0, 0, 0);
    listProcessInfo.SetItemText(0, 1, pathName);
    listProcessInfo.InsertItem(LVIF_TEXT, 1, TEXT("启动时间"), 0, 0, 0, 0);
    listProcessInfo.SetItemText(1, 1, startupTime);
    listProcessInfo.InsertItem(LVIF_TEXT, 2, TEXT("进程ID"), 0, 0, 0, 0);
    listProcessInfo.SetItemText(2, 1, pid);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
