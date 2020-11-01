// CDelete.cpp: 实现文件
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "CDelete.h"
#include "afxdialogex.h"
#include <winerror.h>

#pragma warning(disable : 4018) // “<”: 有符号/无符号不匹配

// CDelete 对话框

IMPLEMENT_DYNAMIC(CDelete, CDialogEx)

CDelete::CDelete(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETE, pParent)
{

}

CDelete::~CDelete()
{
}

void CDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LABEL_SOURCE, labelSource);
    DDX_Control(pDX, IDC_EDIT_SOURCE, editSource);
    DDX_Control(pDX, IDC_BUTTON_FILE, buttonFile);
    DDX_Control(pDX, IDC_BUTTON_FOLDER, buttonFolder);
}


BEGIN_MESSAGE_MAP(CDelete, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDelete::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CDelete::OnClickedButtonFile)
    ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CDelete::OnClickedButtonFolder)
    ON_BN_CLICKED(IDC_DELETE_PERMANENTLY, &CDelete::OnBnClickedDeletePermanently)
END_MESSAGE_MAP()

void CDelete::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if (source_type == SOURCE_TYPE::UNDEFINED)
    {
        MessageBox(TEXT("没有选择文件或文件夹。"), TEXT("提示"), MB_OK | MB_ICONWARNING);
        return;
    }
    editSource.GetWindowText(*pSource);
    CDialogEx::OnOK();
}

void CDelete::OnClickedButtonFile()
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

void CDelete::OnClickedButtonFolder()
{
    // TODO: 在此添加控件通知处理程序代码
    CFolderPickerDialog selectFolder;
    if (selectFolder.DoModal() == IDOK)
    {
        editSource.SetWindowText(selectFolder.GetPathName());
        source_type = SOURCE_TYPE::TYPE_FOLDER;
    }
}

void CDelete::OnBnClickedDeletePermanently()
{
    if (source_type == SOURCE_TYPE::UNDEFINED)
    {
        MessageBox(TEXT("没有选择文件或文件夹。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
        return;
    }
    INT_PTR confirmDelete = MessageBox(TEXT("警告：此操作不可逆！\n确定要永久删除文件吗？\n选择“是”以永久删除，选择“否”不执行任何操作。\n注：本程序不为用户的误操作承担责任！"), TEXT("选择"), MB_YESNO | MB_ICONWARNING);
    if (confirmDelete == IDYES)
    {
        int result = DeleteOperation();
        if (result==0)
        {
            MessageBox(TEXT("成功删除了选定的内容。如果误删，您可以尝试使用数据恢复工具进行恢复。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
            OnOK();
        }
        else MessageBox(TEXT("未能删除文件。"), TEXT("错误"), MB_OK | MB_ICONERROR);
    }
}

int CDelete::DeleteOperation(bool deletePermanently)
{
    SHFILEOPSTRUCT fileOperation = {0};
    CString source; editSource.GetWindowText(source);
    TCHAR* arraySource = new TCHAR[source.GetLength() + 2];
    for (int i = 0; i < source.GetLength(); ++i) arraySource[i] = source[i];
    arraySource[source.GetLength()] = arraySource[source.GetLength() + 1] = 0;
    fileOperation.pFrom = arraySource;
    fileOperation.pTo = NULL;
    fileOperation.fFlags = FOF_SIMPLEPROGRESS | FOF_NOCONFIRMATION;
    fileOperation.wFunc = FO_DELETE;
    int result = SHFileOperation(&fileOperation);
    delete[] arraySource;
    return result;
}
