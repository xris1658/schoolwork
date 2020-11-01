#include "pch.h"
#include "resource.h"
#include "CCopyStatus.h"
#include "USBDeviceManagerGUI.h"
#include "USBDeviceManagerCore.h"
#include "USBDeviceManagerGUIDlg.h"

#include <string>

IMPLEMENT_DYNAMIC(CCopyStatus, CDialogEx)

// 用于显示复制进度的 CCopyStatus 对话框
CCopyStatus::CCopyStatus(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_COPY_STATUS, pParent) {}
CCopyStatus::CCopyStatus(CWnd* pParent, LPTSTR _arraySource, LPTSTR _arrayDest) : CCopyStatus(pParent)
{
    arraySource = _arraySource;
    arrayDest = _arrayDest;
    //fileCopy.mainWindow = this->mainWindow;
    //fileCopy.pMainWnd = this->pMainWnd;
    fileCopy.Init(arraySource, arrayDest, 0);
}
CCopyStatus::~CCopyStatus() {}
void CCopyStatus::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LABEL_FROM, labelFrom);
    DDX_Control(pDX, IDC_SOURCEDIR, linkSource);
    DDX_Control(pDX, IDC_LABEL_TO, labelTo);
    DDX_Control(pDX, IDC_DESTFOLDER, linkDest);
    DDX_Control(pDX, IDC_LABEL_STATUS, labelStatus);
    DDX_Control(pDX, IDC_PROGRESS_STATUS, progressStatus);
    ready = true;
}
BEGIN_MESSAGE_MAP(CCopyStatus, CDialogEx)
    ON_MESSAGE(WM_COPYFILE_NOTIFY, &CCopyStatus::OnCopyfileNotify)
    ON_NOTIFY(NM_CLICK, IDC_SOURCEDIR, &CCopyStatus::OnNMClickSourcedir)
END_MESSAGE_MAP()

BOOL CCopyStatus::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CString source(arraySource);
    int slashDir=source.ReverseFind('\\');
    source.Delete(slashDir, source.GetLength() - slashDir);
    //linkSource.SetItemUrl(0, source);
    slashDir = source.ReverseFind('\\');
    source.Delete(0, slashDir+1);
    //source.Insert(0, TEXT("<a>"));
    //source.Append(TEXT("</a>"));
    linkSource.SetWindowText(source);
    CString& dest = source;
    dest.Empty();
    dest = arrayDest;
    slashDir = dest.ReverseFind('\\');
    dest.Delete(slashDir, dest.GetLength() - slashDir);
    //linkDest.SetItemUrl(0, dest);
    slashDir = dest.ReverseFind('\\');
    dest.Delete(0, slashDir + 1);
    //dest.Insert(0, TEXT("<a>"));
    //dest.Append(TEXT("</a>"));
    linkDest.SetWindowText(dest);
    fileCopy.SetHNotifyWnd(this->m_hWnd);
    fileCopy.SetPNotifyWnd((LPVOID)this);
    progressStatus.SetRange(0, 100);
    fileCopy.mainWindow = this->mainWindow;
    fileCopy.pMainWnd = this->pMainWnd;
    fileCopy.Begin();
    return TRUE;
}

afx_msg
BOOL CCopyStatus::PreTranslateMessage(MSG* pMsg)
{
    return CDialogEx::PreTranslateMessage(pMsg);
}
LRESULT CCopyStatus::OnCopyfileNotify(WPARAM wParam, LPARAM lParam)
{
    if (wParam == 1)
    {
        int lower, upper;
        progressStatus.GetRange(lower, upper);
        progressStatus.SetPos(upper);
        MessageBox(TEXT("已成功复制文件。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
        CUSBDeviceManagerGUIDlg* dlg = (CUSBDeviceManagerGUIDlg*)pMainWnd;
        dlg->m_pTl3->SetProgressState(dlg->m_hWnd, TBPF_NOPROGRESS);
        OnOK();
    }
    else
    {
        ErrorInfo();
        CUSBDeviceManagerGUIDlg* dlg = (CUSBDeviceManagerGUIDlg*)pMainWnd;
        dlg->m_pTl3->SetProgressState(dlg->m_hWnd, TBPF_NOPROGRESS);
        OnCancel();
    }
    return 0;
}

void CCopyStatus::UpdateValue(double value)
{
    int lower, upper;
    progressStatus.GetRange(lower, upper);
    double _lower=lower;
    double _upper=upper;
    double progressValue = _lower + (upper - lower)*value;
    progressStatus.SetPos(progressValue);
    int labelValue = value * 100;
    if (labelValue != currentPercentage)
    {
#ifdef _UNICODE
        std::wstring temp = TEXT("已完成 ");
        temp.append(std::to_wstring(labelValue));
        temp.append(1, '%');
#else
        std::string temp = TEXT("已完成 ");
        temp.append(std::to_string(labelValue));
        temp.append(1, '%');
#endif
        labelStatus.SetWindowText(temp.c_str());
        currentPercentage = labelValue;
    }
}

void CCopyStatus::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    int ret=MessageBox(TEXT("确定要取消复制操作吗？"), TEXT("提示"), MB_YESNO | MB_ICONQUESTION);
    if (ret == IDYES)
    {
        fileCopy.Cancel();
        CDialogEx::OnCancel();
    }
    return;
}


void CCopyStatus::OnNMClickSourcedir(NMHDR *pNMHDR, LRESULT *pResult)
{
//    CString _source; linkSource.GetItemUrl(0, _source);
//#ifdef _UNICODE
//    CStringA source = (CStringA)_source;
//#else
//    CString& source = _source;
//#endif
//    source.Insert(0, "explorer ");
//    std::string strSource(source);
//    if(strSource.back()==':') strSource.append(1, '\\');
//    system(strSource.c_str());
    *pResult = 0;
}
