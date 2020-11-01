// COptions.cpp : implementation file
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "COptions.h"
#include "afxdialogex.h"
#include "resource.h"

#include <string>

// COptions dialog

IMPLEMENT_DYNAMIC(COptions, CDialogEx)

COptions::COptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{

}
COptions::COptions(CWnd* pParent, CDiskSpaceIndicator* _indicator)
    : CDialogEx(IDD_OPTIONS, pParent)
{
    indicator = _indicator;
}

COptions::~COptions()
{
}

void COptions::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON_NORMAL, colorNormal);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON_ALERT, colorAlert);
    DDX_Control(pDX, IDC_BUTTON_APPLY, buttonApply);
    DDX_Control(pDX, IDC_EDIT_ALERT_THRESHOLD, editAlertThreshold);
}


BEGIN_MESSAGE_MAP(COptions, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &COptions::OnApply)
    ON_BN_CLICKED(IDC_MFCCOLORBUTTON_NORMAL, &COptions::OnBnClickedColorButton)
    ON_BN_CLICKED(IDC_MFCCOLORBUTTON_ALERT, &COptions::OnBnClickedColorButton)
    ON_EN_CHANGE(IDC_EDIT_ALERT_THRESHOLD, &COptions::OnEnChangeEditAlertThreshold)
END_MESSAGE_MAP()


// COptions message handlers


BOOL COptions::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    unsigned int alertThreshold = indicator->getAlertThreshold();
    COLORREF normalColor = indicator->getNormalForegroundColor();
    COLORREF alertColor = indicator->getAlertForegroundColor();
    colorNormal.SetColor(normalColor);
    colorAlert.SetColor(alertColor);
    buttonApply.EnableWindow(FALSE);
#ifdef _UNICODE
    std::wstring stringThreshold = std::to_wstring(indicator->getAlertThreshold());
#else
    std::string stringThreshold = std::to_string(indicator->getAlertThreshold());
#endif
    editAlertThreshold.SetWindowText(CString(stringThreshold.c_str()));
    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE
}

template<typename T>
bool Between(T value, T lower, T upper) { return value >= lower && value <= upper; }

void COptions::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    CString temp;
    editAlertThreshold.GetWindowText(temp);
    int value = 0;
    for (int i = 0; i < temp.GetLength(); ++i)
    {
        value *= 10;
        value += temp[i] - '0';
    }
    if (!Between<int>(value, 1, 100))
    {
        if (value <= 0) editAlertThreshold.SetWindowText(TEXT("1"));
        else editAlertThreshold.SetWindowText(TEXT("100"));
        MessageBox(TEXT("���漶��Ҫ���ֵΪ1-100֮�����������������ӽ���ֵ��"), TEXT("����"), MB_OK | MB_ICONERROR);
        return;
    }
    if(buttonApply.IsWindowEnabled()) OnApply();
    CDialogEx::OnOK();
}


void COptions::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    CDialogEx::OnCancel();
}


void COptions::OnApply()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString temp;
    editAlertThreshold.GetWindowText(temp);
    int value = 0;
    for (int i = 0; i < temp.GetLength(); ++i)
    {
        value *= 10;
        value += temp[i] - '0';
    }
    if (!Between<int>(value, 1, 100))
    {
        if (value <= 0) editAlertThreshold.SetWindowText(TEXT("1"));
        else editAlertThreshold.SetWindowText(TEXT("100"));
        MessageBox(TEXT("���漶��Ҫ���ֵΪ1-100֮�����������������ӽ���ֵ��"), TEXT("����"), MB_OK | MB_ICONERROR);
        return;
    }
    indicator->setAlertThreshold(value);
    indicator->setNormalForegroundColor(colorNormal.GetColor());
    indicator->setAlertForegroundColor(colorAlert.GetColor());
    indicator->SetPos(indicator->GetPos());
    indicator->UpdateWindow();
    buttonApply.EnableWindow(FALSE);
}


void COptions::OnBnClickedColorButton()
{
    buttonApply.EnableWindow(TRUE);
}

void COptions::OnEnChangeEditAlertThreshold()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
    buttonApply.EnableWindow(TRUE);
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}
