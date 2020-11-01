// CCopy.cpp: 实现文件
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "CCopy.h"
#include "afxdialogex.h"
#include "resource.h"

#include <shellapi.h>
#include <shobjidl_core.h>

#include <string>




// CCopy 对话框

IMPLEMENT_DYNAMIC(CCopy, CDialogEx)

CCopy::CCopy(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COPY, pParent)
{
}

CCopy::~CCopy()
{
}

void CCopy::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LABEL_SOURCE, labelSource);
    DDX_Control(pDX, IDC_LABEL_DEST, labelDest);
    DDX_Control(pDX, IDC_EDIT_SOURCE, editSource);
    DDX_Control(pDX, IDC_EDIT_DEST, editDest);
    DDX_Control(pDX, IDC_BUTTON_FILE, buttonFile);
    DDX_Control(pDX, IDC_BUTTON_FOLDER, buttonFolder);
    DDX_Control(pDX, IDC_BUTTON_BROWSE, buttonBrowse);
}

BOOL CCopy::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
    MessageBox(TEXT("检测到了设备变化。你可能需要重新打开窗口。"), TEXT("警告"), MB_OK | MB_ICONWARNING);
    return 0;
}

BEGIN_MESSAGE_MAP(CCopy, CDialogEx)
    ON_BN_CLICKED(IDOK, &CCopy::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CCopy::OnClickedButtonFile)
    ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CCopy::OnClickedButtonFolder)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CCopy::OnClickedButtonBrowse)
    ON_WM_DEVICECHANGE()
//    ON_WM_SIZING()
END_MESSAGE_MAP()

// CCopy 消息处理程序

void CCopy::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if (source_type == SOURCE_TYPE::UNDEFINED)
    {
        MessageBox(TEXT("没有选择文件或文件夹。"), TEXT("提示"), MB_OK | MB_ICONWARNING);
    }
    else if (editDest.GetWindowTextLength() == 0)
    {
        INT_PTR result = MessageBox(TEXT("未选择目标路径。\n要复制到根目录吗？"), TEXT("确认"), MB_YESNO | MB_ICONQUESTION);
        if (result == IDYES)
        {
            TCHAR path[4] = TEXT(" :\\");
            path[0] = letter;
            editDest.SetWindowText(path);
        }
    }
    if(editDest.GetWindowTextLength() != 0)
    {
        CString source, dest;
        editSource.GetWindowText(source);
        editDest.GetWindowText(dest);
        //注意: SHFILEOPSTRUCT中的源和目标必须以双零结尾 (与字符集无关)
        TCHAR* arraySource = new TCHAR[source.GetLength()+2];
        for (int i = 0; i < source.GetLength(); ++i) arraySource[i] = source[i];
        arraySource[source.GetLength()] = arraySource[source.GetLength()+1] = 0;
        TCHAR* arrayDest = new TCHAR[dest.GetLength()+2];
        for (int i = 0; i < dest.GetLength(); ++i) arrayDest[i] = dest[i];
        arrayDest[dest.GetLength()] = arrayDest[dest.GetLength()+1] = 0;
        if (source_type == SOURCE_TYPE::TYPE_FILE)
        {
            SHFILEOPSTRUCT fileOperation;
            fileOperation.pFrom = arraySource;
            fileOperation.pTo = arrayDest;
            fileOperation.wFunc = FO_COPY;
            fileOperation.fFlags = FOF_SIMPLEPROGRESS;
            TCHAR progressTitle[] = TEXT("正在复制文件...");
            fileOperation.lpszProgressTitle = progressTitle;

            int ret = SHFileOperation(&fileOperation);

            if (ret == 0)
            {
                MessageBox(TEXT("成功复制了文件。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
                OnOK();
            }
            else
            {
                MessageBox(TEXT("Error"), TEXT("Error"), MB_OK | MB_ICONERROR);
            }
            delete[] arraySource;
            delete[] arrayDest;
            CDialogEx::OnOK();
        }
        else if (source_type == SOURCE_TYPE::TYPE_FOLDER)
        {
            SHFILEOPSTRUCT folderOperation;
            folderOperation.pFrom = arraySource;
            folderOperation.pTo = arrayDest;
            folderOperation.wFunc = FO_COPY;
            folderOperation.fFlags = FOF_SIMPLEPROGRESS;
            TCHAR progressTitle[] = TEXT("正在复制文件夹...");
            folderOperation.lpszProgressTitle = progressTitle;

            int ret=SHFileOperation(&folderOperation);
            
            if (ret == 0)
            {
                MessageBox(TEXT("成功复制了文件夹。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
                OnOK();
            }
            else
            {
                MessageBox(TEXT("Error"), TEXT("Error"), MB_OK | MB_ICONERROR);
            }
            delete[] arraySource;
            delete[] arrayDest;
            OnOK();
        }
    }
}


void CCopy::OnClickedButtonFile()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog selectFile(TRUE, TEXT("所有文件(*.*)|*.*|||"), 0,
        OFN_DONTADDTORECENT
        | OFN_FILEMUSTEXIST
        | OFN_PATHMUSTEXIST
        | OFN_SHOWHELP,
        TEXT("所有文件(*.*)|*.*|||"),
        0, 0, TRUE);
    if (selectFile.DoModal() == IDOK)
    {
        editSource.SetWindowText(selectFile.GetPathName());
        source_type = SOURCE_TYPE::TYPE_FILE;
    }
}


void CCopy::OnClickedButtonFolder()
{
    TCHAR path[4] = TEXT(" :\\");
    path[0] = letter;
    CFolderPickerDialog selectFolder(path, 0, NULL, 0);
    if (selectFolder.DoModal() == IDOK)
    {
        editSource.SetWindowText(selectFolder.GetPathName());
        source_type = SOURCE_TYPE::TYPE_FOLDER;
    }
}


void CCopy::OnClickedButtonBrowse()
{
    TCHAR path[4] = TEXT(" :\\");
    path[0] = letter;
    CFolderPickerDialog selectDest(path, 0, NULL, 0);
    if (selectDest.DoModal() == IDOK)
    {
        editDest.SetWindowText(selectDest.GetPathName());
    }
}
