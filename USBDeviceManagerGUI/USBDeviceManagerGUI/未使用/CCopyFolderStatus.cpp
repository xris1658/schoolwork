// CCopyFolderStatus.cpp : implementation file
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "CCopyFolderStatus.h"
#include "afxdialogex.h"


// CCopyFolderStatus dialog

IMPLEMENT_DYNAMIC(CCopyFolderStatus, CDialogEx)

CCopyFolderStatus::CCopyFolderStatus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COPY_FOLDER_STATUS, pParent)
{

}

CCopyFolderStatus::CCopyFolderStatus(CWnd* pParent, LPSHFILEOPSTRUCT _pFolderOperation)
    : CDialogEx(IDD_COPY_FOLDER_STATUS, pParent)
{
    pFolderOperation = _pFolderOperation;
}

CCopyFolderStatus::~CCopyFolderStatus()
{
}

void CCopyFolderStatus::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SOURCEDIR, linkSource);
    DDX_Control(pDX, IDC_DESTFOLDER, linkDest);
    DDX_Control(pDX, IDC_LABEL_FILE_STATUS, labelFileStatus);
    DDX_Control(pDX, IDC_LABEL_ALL_STATUS, labelAllStatus);
    DDX_Control(pDX, IDC_PROGRESS_FILE_STATUS, progressFileStatus);
    DDX_Control(pDX, IDC_PROGRESS_ALL_STATUS, progressAllStatus);
}


BEGIN_MESSAGE_MAP(CCopyFolderStatus, CDialogEx)
END_MESSAGE_MAP()


// CCopyFolderStatus message handlers


BOOL CCopyFolderStatus::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    pFolderOperation->hwnd = m_hWnd;
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
