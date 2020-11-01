// CDisableDevice.cpp: 实现文件
//

#include "pch.h"
#include "USBDeviceManagerGUI.h"
#include "CDisableDevice.h"
#include "afxdialogex.h"
#include "resource_2.h"

#include <cfgmgr32.h>
#include <winioctl.h>
#include <WinBase.h>

// CDisableDevice 对话框

IMPLEMENT_DYNAMIC(CDisableDevice, CDialogEx)

CDisableDevice::CDisableDevice(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISABLE_DEVICE, pParent)
{
}

CDisableDevice::CDisableDevice(CWnd* pParent, TCHAR _letter)
    : CDialogEx(IDD_DISABLE_DEVICE, pParent)
{
    letter = _letter;
}

CDisableDevice::~CDisableDevice()
{
}

void CDisableDevice::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_TYPE, comboType);
    DDX_Control(pDX, IDC_EDIT_TYPE_HELP, editTypeHelp);
    DDX_Control(pDX, IDOK, buttonOperate);
    DDX_Control(pDX, IDCANCEL, buttonClose);
}


BEGIN_MESSAGE_MAP(CDisableDevice, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDisableDevice::OnSelchangeComboType)
END_MESSAGE_MAP()


// CDisableDevice 消息处理程序


void CDisableDevice::OnSelchangeComboType()
{
    // TODO: 在此添加控件通知处理程序代码
    switch (comboType.GetCurSel())
    {
    case 0:
        disableType = DisableType::TYPE_SAFE_REMOVE;
        break;
    case 1:
        disableType = DisableType::TYPE_UNMOUNT;
        break;
    case 2:
        disableType = DisableType::TYPE_DISABLE_DEVICE;
        break;
    }
    editTypeHelp.SetWindowText(disableTypeInfo[comboType.GetCurSel()]);
}


void CDisableDevice::OnOK()
{
    if (disabled)
    {
        TCHAR devName[4] = TEXT(" :\\");
        devName[0] = letter;
        devName[3] = 0;
        TCHAR driveLetter[3] = TEXT(" :");
        driveLetter[0] = letter;
        if (disableType == DisableType::TYPE_UNMOUNT)
        {
            BOOL result = DefineDosDevice(DDD_RAW_TARGET_PATH, driveLetter, dosPath);
            if (!result)
            {
                ErrorInfo();
                return;
            }
            TCHAR uniqueVolumeName[dosPathLength] = {0};
            result = GetVolumeNameForVolumeMountPoint(devName, uniqueVolumeName, dosPathLength);
            if (!result)
            {
                ErrorInfo();
                return;
            }
            result = DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_REMOVE_DEFINITION |
                DDD_EXACT_MATCH_ON_REMOVE, driveLetter, dosPath);
            if (!result)
            {
                ErrorInfo();
                return;
            }
            result = SetVolumeMountPoint(devName, uniqueVolumeName);
            if (!result)
            {
                ErrorInfo();
                return;
            }
            MessageBox(TEXT("成功添加了驱动器号。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
            buttonOperate.SetWindowText(TEXT("禁用(&D)"));
            buttonClose.EnableWindow(TRUE);
            comboType.EnableWindow(TRUE);
            disabled = false;
        }
    }
    else
    {
        INT_PTR result = MessageBox(TEXT("确定要执行禁用操作吗？\n请确保设备没有正在执行的操作再继续。"), TEXT("提示"), MB_YESNO | MB_ICONWARNING);
        if (result == IDYES)
        {
            if (disableType == DisableType::TYPE_SAFE_REMOVE)
            {
#pragma region 安全删除硬件
                //TCHAR path[] = TEXT("\\\\.\\ :");
                //path[4] = letter;
                //HANDLE hDevice = CreateFile(
                //    path,
                //    GENERIC_READ | GENERIC_WRITE,
                //    FILE_SHARE_READ | FILE_SHARE_WRITE,
                //    NULL,
                //    OPEN_ALWAYS,
                //    FILE_ATTRIBUTE_NORMAL,
                //    NULL);
                //if (hDevice == INVALID_HANDLE_VALUE)
                //{
                //    ErrorInfo();
                //    MessageBox(TEXT("未能成功获取设备。"), TEXT("错误"), MB_OK | MB_ICONERROR);
                //    return;
                //}
                //DWORD bytesReturned;
                //OVERLAPPED overlapped;
                //BOOL result = DeviceIoControl(
                //    hDevice,
                //    (DWORD)FSCTL_LOCK_VOLUME,
                //    NULL,
                //    0,
                //    NULL,
                //    0,
                //    &bytesReturned,
                //    &overlapped);
                //if (!result)
                //{
                //    MessageBox(TEXT("1"), TEXT("Error"), MB_OK | MB_ICONERROR);
                //    ErrorInfo();
                //    (void)CloseHandle(hDevice);
                //    return;
                //}
                //result = DeviceIoControl(
                //    hDevice,
                //    (DWORD)FSCTL_DISMOUNT_VOLUME,
                //    NULL,
                //    0,
                //    NULL,
                //    0,
                //    &bytesReturned,
                //    &overlapped
                //);
                //if (!result)
                //{
                //    MessageBox(TEXT("2"), TEXT("Error"), MB_OK | MB_ICONERROR);
                //    ErrorInfo();
                //    (void)CloseHandle(hDevice);
                //    return;
                //}
                //const unsigned int inputLength = 0x0000ffff;
                //char input[inputLength];
                //result = DeviceIoControl
                //(
                //    hDevice,
                //    (DWORD)IOCTL_STORAGE_MEDIA_REMOVAL,
                //    input,
                //    inputLength,
                //    NULL,
                //    0,
                //    &bytesReturned,
                //    &overlapped
                //);
                //if (!result)
                //{
                //    fprintf(stderr, "FSCTL_LOCK_VOLUME Error: %ld\n", GetLastError());
                //    (void)CloseHandle(hDevice);
                //    return;
                //}
                //result = DeviceIoControl
                //(
                //    hDevice,
                //    IOCTL_STORAGE_EJECT_MEDIA,
                //    NULL,
                //    0,
                //    NULL,
                //    0,
                //    &bytesReturned,
                //    &overlapped
                //);
                //if (!result)
                //{
                //    fprintf(stderr, "FSCTL_LOCK_VOLUME Error: %ld\n", GetLastError());
                //    (void)CloseHandle(hDevice);
                //    return;
                //}
                //MessageBox(TEXT("成功安全删除了硬件。窗口即将关闭。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
                //CDialogEx::OnOK();
#pragma endregion

            }
            else if (disableType == DisableType::TYPE_UNMOUNT)
            {
                TCHAR devName[4] = TEXT(" :");
                devName[0] = letter;
                devName[2] = devName[3] = 0;
                QueryDosDevice(devName, dosPath, dosPathLength);
                devName[2] = '\\';
                BOOL result = DeleteVolumeMountPoint(devName);
                if (!result)
                {
                    ErrorInfo();
                    return;
                }
                MessageBox(TEXT("成功删除了驱动器号。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
                buttonOperate.SetWindowText(TEXT("启用(&E)"));
                buttonClose.EnableWindow(FALSE);
                comboType.EnableWindow(FALSE);
                disabled = true;
            }
            else if (disableType == DisableType::TYPE_DISABLE_DEVICE)
            {
                
            }
        }
    }
}


BOOL CDisableDevice::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    comboType.SetCurSel(1);
    disableType = DisableType::TYPE_UNMOUNT;
    editTypeHelp.SetWindowText(disableTypeInfo[1]);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
