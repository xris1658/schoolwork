
// USBDeviceManagerGUIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "USBDeviceManagerGUI.h"
#include "USBDeviceManagerGUIDlg.h"
#include "CCopy.h"
#include "CDelete.h"
#include "afxdialogex.h"
#include "resource.h"
#include "resource_2.h"
#include "COptions.h"
#include "CProcessInfo.h"
#include "CDisableDevice.h"

#include <Windows.h>
#include <winioctl.h>
#include <shellapi.h>
#include <ShObjIdl_core.h>
#include <Psapi.h>
#include <winuser.h>
#include <cfgmgr32.h>

#include <bitset>
#include <string>

#pragma warning(disable : 4244) // l.234: “初始化”: 从“ULONGLONG”转换到“long double”，可能丢失数据
//丢失数据的情况不可能出现。
//ULONGLONG为64位无符号整数，long double 在x86架构上通常为80位浮点。

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUSBDeviceManagerGUIDlg 对话框

CUSBDeviceManagerGUIDlg::CUSBDeviceManagerGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USBDEVICEMANAGERGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

short int CUSBDeviceManagerGUIDlg::IsUsbDrive(unsigned short int letter/*A=0, ..., Z=25*/)
{
    TCHAR path[] = TEXT("\\\\.\\ :");
    path[4] = 'A' + letter; // 路径示例: \\:\C:
    HANDLE hDevice = CreateFile(
        path,
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        ErrorInfo();
        return -1;
    }
    else
    {
        STORAGE_PROPERTY_QUERY query;
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;
        PSTORAGE_DESCRIPTOR_HEADER pDevDescHeader = (PSTORAGE_DESCRIPTOR_HEADER)malloc(sizeof(STORAGE_DESCRIPTOR_HEADER));
        if (pDevDescHeader == NULL)
        {
            fprintf(stderr, "Error when creating device description header.\n");
            return -1;
        }
        else
        {
            DWORD readed;
            BOOL result = DeviceIoControl(
                hDevice,
                IOCTL_STORAGE_QUERY_PROPERTY,
                &query,
                sizeof query,
                pDevDescHeader,
                sizeof(STORAGE_DESCRIPTOR_HEADER),
                &readed,
                NULL);
            if (!result)
            {
                fprintf(stderr, "IOCTL_STORAGE_QUERY_PROPERTY Error: %ld\n", GetLastError());
                free(pDevDescHeader);
                (void)CloseHandle(hDevice);
                return -1;
            }
            else
            {
                DWORD devDescLength = pDevDescHeader->Size;
                PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)malloc(devDescLength);
                if (NULL == pDevDesc)
                {
                    fprintf(stderr, "Error when creating device discriptor.\n");
                    free(pDevDescHeader);
                    return -1;
                }
                else
                {
                    BOOL result = DeviceIoControl(
                        hDevice,
                        IOCTL_STORAGE_QUERY_PROPERTY,
                        &query,
                        sizeof query,
                        pDevDesc,
                        devDescLength,
                        &readed,
                        NULL);
                    if (!result)
                    {
                        fprintf(stderr, "IOCTL_STORAGE_QUERY_PROPERTY Error: %ld\n", GetLastError());
                        free(pDevDescHeader);
                        free(pDevDesc);
                        (void)CloseHandle(hDevice);
                        return -1;
                    }
                    else
                    {
                        short int ret;
                        STORAGE_BUS_TYPE type = pDevDesc->BusType;
                        if (type == STORAGE_BUS_TYPE::BusTypeUsb)
                        {
                            ret = 1;
                        }
                        else ret = 0;
                        free(pDevDescHeader);
                        free(pDevDesc);
                        (void)CloseHandle(hDevice);
                        return ret;
                    }
                }
            }
        }
    }
}

void CUSBDeviceManagerGUIDlg::FixSizeAndUnit(long double & size, unsigned short int & unit)
{
    while (size >= 1000)
    {
        size /= 1024;
        ++unit;
    }
}

DWORD CUSBDeviceManagerGUIDlg::GetUsbDrives()
{
    DWORD _drive_flag = GetLogicalDrives();
    if (_drive_flag != 0)
    {
        std::bitset<32> drive_flag(_drive_flag);
        for (unsigned int i = 0; i < 26; ++i)
        {
            if (drive_flag[i])
            {
                short int result = IsUsbDrive(i);
                if (result == 0) drive_flag.set(i, false);
                else if (result < 0) return DWORD(-1);
            }
        }
        return drive_flag.to_ulong();
    }
    return 0;
}

void CUSBDeviceManagerGUIDlg::RefreshUsbDevice()
{
    listDevice.DeleteAllItems();
    TCHAR letter[] = TEXT(" :");
    DWORD res = GetUsbDrives();
    for (unsigned int i = 0; i < 26 && res != 0; ++i)
    {
        if (res % 2 != 0)
        {
            letter[0] = 'A' + i;
            const unsigned int volume_name_length = 64;
            TCHAR volume_name[volume_name_length];
            GetVolumeInformation(letter, volume_name, volume_name_length, NULL, NULL, NULL, NULL, 0);
            CString name_and_letter = volume_name;
            name_and_letter.Append(TEXT(" ("));
            name_and_letter.Append(letter);
            name_and_letter.AppendChar(')');
            listDevice.InsertItem(LVIF_TEXT, listDevice.GetItemCount(), name_and_letter, 0, 0, 0, 0);
        }
        res /= 2;
    }
    Unselect();
}

void CUSBDeviceManagerGUIDlg::Unselect()
{
    for (std::vector<CWnd*>::iterator it = usbControls.begin(); it != usbControls.end(); ++it)
    {
        CWnd* control = *it;
        control->EnableWindow(FALSE);
    }
    int itemCount=listDevice.GetItemCount();
#ifdef _UNICODE
    std::wstring temp = std::to_wstring(itemCount);
#else
    std::string temp = std::to_string(itemCount);
#endif
    temp.append(TEXT(" 个项目"));
    labelName.SetWindowText(temp.c_str());
    labelAll.SetWindowText(TEXT(""));
    labelAvailable.SetWindowText(TEXT(""));
    labelUsed.SetWindowText(TEXT(""));
    progressDiskSpaceIndicator.SetPos(0);
}

void CUSBDeviceManagerGUIDlg::Select()
{
    for (std::vector<CWnd*>::iterator it = usbControls.begin(); it != usbControls.end(); ++it)
    {
        CWnd* control = *it;
        control->EnableWindow(TRUE);
    }
    labelName.SetWindowText(listDevice.GetItemText(listDevice.GetSelectionMark(), 0));
    CString item_text = listDevice.GetItemText(listDevice.GetSelectionMark(), 0);
    unsigned int letter_index=item_text.Find('(', 0)+1;
    char letter = item_text[letter_index]-'A';
    GetDiskSpace(letter);
}

void CUSBDeviceManagerGUIDlg::GetDiskSpace(unsigned short int letter)
{
    TCHAR lpRootPathName[4] = {(TCHAR)('A' + letter), ':', '\\', 0};
    ULARGE_INTEGER _freeBytesAvailableToCaller, _totalNumberOfBytes, _totalNumberOfFreeBytes;
    BOOL getDiskFreeSpaceExStat = GetDiskFreeSpaceEx(lpRootPathName,
        &_freeBytesAvailableToCaller,
        &_totalNumberOfBytes,
        &_totalNumberOfFreeBytes);
    if (getDiskFreeSpaceExStat)
    {
        ULONGLONG freeBytesAvailableToCaller = _freeBytesAvailableToCaller.QuadPart;
        ULONGLONG totalNumberOfBytes = _totalNumberOfBytes.QuadPart;
        ULONGLONG totalNumberOfFreeBytes = _totalNumberOfFreeBytes.QuadPart;
        long double totalSize = totalNumberOfBytes;
        unsigned short int totalSizeUnit = 0;
        long double freeSize = totalNumberOfFreeBytes;
        unsigned short int freeSizeUnit = 0;
        long double usedSize = totalNumberOfBytes - totalNumberOfFreeBytes;
        unsigned short int usedSizeUnit = 0;
        FixSizeAndUnit(totalSize, totalSizeUnit);
        FixSizeAndUnit(freeSize,  freeSizeUnit);
        FixSizeAndUnit(usedSize,  usedSizeUnit);
#ifdef _UNICODE
        //total
        std::wstring temp = TEXT("总大小: ");
        temp.append(std::to_wstring(totalSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[totalSizeUnit]);
        temp.append(1, 'B');
        labelAll.SetWindowText(temp.c_str());
        temp.clear();
        //used
        temp = TEXT("已用: ");
        temp.append(std::to_wstring(usedSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[usedSizeUnit]);
        temp.append(1, 'B');
        labelUsed.SetWindowText(temp.c_str());
        temp.clear();
        //available
        temp = TEXT("可用: ");
        temp.append(std::to_wstring(freeSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[freeSizeUnit]);
        temp.append(1, 'B');
        labelAvailable.SetWindowText(temp.c_str());
#else
        //total
        std::string temp = TEXT("总大小: ");
        temp.append(std::to_string(totalSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[totalSizeUnit]);
        temp.append(1, 'B');
        labelAll.SetWindowText(temp.c_str());
        temp.clear();
        //used
        temp = TEXT("已用: ");
        temp.append(std::to_string(usedSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[usedSizeUnit]);
        temp.append(1, 'B');
        labelUsed.SetWindowText(temp.c_str());
        temp.clear();
        //available
        temp = TEXT("可用: ");
        temp.append(std::to_string(freeSize).substr(0, 4));
        if (temp.back() == '.') temp.pop_back();
        temp.append(1, ' ');
        temp.append(1, unit_name[freeSizeUnit]);
        temp.append(1, 'B');
        labelAvailable.SetWindowText(temp.c_str());
#endif

        progressDiskSpaceIndicator.SetPos((int)(usedSize/totalSize*100.0));
    }
    else ErrorInfo();
    return;
}

void CUSBDeviceManagerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_REFRESH, buttonRefresh);
    DDX_Control(pDX, IDC_LIST_DEVICE, listDevice);
    DDX_Control(pDX, IDC_BUTTON_ABLE, buttonAble);
    DDX_Control(pDX, IDC_BUTTON_COPY, buttonCopy);
    DDX_Control(pDX, IDC_BUTTON_DELETE, buttonDelete);
    DDX_Control(pDX, IDC_BUTTON_OPTIONS, buttonOptions);
    DDX_Control(pDX, IDC_BUTTON_PCB, buttonProcessInfo);
    DDX_Control(pDX, IDC_EDIT_INFO, textInfo);
    DDX_Control(pDX, IDC_LABEL_ALL, labelAll);
    DDX_Control(pDX, IDC_LABEL_AVAILABLE, labelAvailable);
    DDX_Control(pDX, IDC_LABEL_NAME, labelName);
    DDX_Control(pDX, IDC_LABEL_USED, labelUsed);
    DDX_Control(pDX, IDC_PROGRESS_DISKSPACEINDICATOR, progressDiskSpaceIndicator);
}

BEGIN_MESSAGE_MAP(CUSBDeviceManagerGUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_DEVICECHANGE()
    ON_WM_MOUSEMOVE()
    ON_WM_MOVING()
    ON_WM_SIZING()
    ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CUSBDeviceManagerGUIDlg::OnClickedButtonRefresh)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_DEVICE, &CUSBDeviceManagerGUIDlg::OnLvnItemActivateListDevice)
    ON_NOTIFY(NM_CLICK, IDC_LIST_DEVICE, &CUSBDeviceManagerGUIDlg::OnNMClickListDevice)
    ON_BN_CLICKED(IDC_BUTTON_COPY, &CUSBDeviceManagerGUIDlg::OnBnClickedButtonCopy)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CUSBDeviceManagerGUIDlg::OnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_OPTIONS, &CUSBDeviceManagerGUIDlg::OnClickedButtonOptions)
    ON_BN_CLICKED(IDC_BUTTON_PCB, &CUSBDeviceManagerGUIDlg::OnBnClickedButtonPcb)
    ON_BN_CLICKED(IDC_BUTTON_ABLE, &CUSBDeviceManagerGUIDlg::OnBnClickedButtonAble)
END_MESSAGE_MAP()

// CUSBDeviceManagerGUIDlg 消息处理程序

BOOL CUSBDeviceManagerGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    listDevice.InsertColumn(0, TEXT("磁盘"), 0, 100);
    RefreshUsbDevice();
    usbControls = std::vector<CWnd*>{ &buttonAble, &buttonCopy, &buttonDelete, &labelAll, &labelAvailable, &labelUsed, &progressDiskSpaceIndicator };
    Unselect();
    progressDiskSpaceIndicator.SetRange(0, 100);
    UpdateControlRect();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CUSBDeviceManagerGUIDlg::OnDeviceChange(UINT a, DWORD_PTR b)
{
    CWnd::OnDeviceChange(a, b);
    RefreshUsbDevice();
    return 0;
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
// 对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

void CUSBDeviceManagerGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CUSBDeviceManagerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUSBDeviceManagerGUIDlg::OnClickedButtonRefresh()
{
    RefreshUsbDevice();
}

void CUSBDeviceManagerGUIDlg::OnLvnItemActivateListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    Select();
    *pResult = 0;
}

void CUSBDeviceManagerGUIDlg::OnNMClickListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    if (listDevice.GetSelectedCount() == 0) Unselect();
    else if (listDevice.GetSelectedCount() == 1) Select();
    else if (listDevice.GetSelectedCount() > 1)
    {
        Unselect();
        labelName.SetWindowText(TEXT("选中了多个项目"));
    }
    *pResult = 0;
}

void CUSBDeviceManagerGUIDlg::OnBnClickedButtonCopy()
{
    CString source, dest;
    // TODO: 在此添加控件通知处理程序代码
    CCopy dlgCopy(this);
    CString item_text = listDevice.GetItemText(listDevice.GetSelectionMark(), 0);
    unsigned int letter_index = item_text.Find('(', 0) + 1;
    dlgCopy.letter = item_text[letter_index];
    dlgCopy.pSource = &source;
    dlgCopy.pDest = &dest;
    dlgCopy.DoModal();
}

void CUSBDeviceManagerGUIDlg::OnClickedButtonDelete()
{
    CString source;
    // TODO: 在此添加控件通知处理程序代码
    CDelete dlgDelete(this);
    CString item_text = listDevice.GetItemText(listDevice.GetSelectionMark(), 0);
    unsigned int letter_index = item_text.Find('(', 0) + 1;
    TCHAR letter = item_text[letter_index];
    dlgDelete.pSource = &source;
    dlgDelete.DoModal();
}

bool PointInRect(POINT& p, RECT& rect)
{
    return p.x >= rect.left && p.y >= rect.top && p.x <= rect.right && p.y <= rect.bottom;
}

void CUSBDeviceManagerGUIDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rectWnd;
    GetWindowRect(rectWnd);
    POINT p;
    GetCursorPos(&p);
    if (PointInRect(p, rectWnd))
    {
        for (unsigned int i = 0; i < 13; ++i)
        {
            if (PointInRect(p, rectControls[i]))
            {
                textInfo.SetWindowText(info[i]);
                break;
            }
        }
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

void CUSBDeviceManagerGUIDlg::OnClickedButtonOptions()
{
    // TODO: Add your control notification handler code here
    COptions options(nullptr, &(this->progressDiskSpaceIndicator));
    options.DoModal();
}

void CUSBDeviceManagerGUIDlg::OnBnClickedButtonPcb()
{
    // TODO: 在此添加控件通知处理程序代码
    HANDLE handleProcess = GetCurrentProcess();
    DWORD id = GetProcessId(handleProcess);
    DWORD processId = GetCurrentProcessId();
    TCHAR fileName[MAX_PATH];
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL)
    {
        MessageBox(TEXT("未能获取进程信息。"), TEXT("错误"), MB_OK | MB_ICONERROR);
    }
    else
    {
        HMODULE hModule;
        DWORD cbNeeded;
        FILETIME createTime, exitTime, kernelTime, userTime;
        SYSTEMTIME stCreation, lstCreation;
        BOOL getTime = GetProcessTimes(handleProcess, &createTime, &exitTime, &kernelTime, &userTime);
        if (!getTime)
        {
            MessageBox(TEXT("未能获取进程的启动时间。"), TEXT("错误"), MB_OK | MB_ICONERROR);
        }
        FileTimeToSystemTime(&createTime, &stCreation);
        SystemTimeToTzSpecificLocalTime(NULL, &stCreation, &lstCreation);
#ifdef _UNICODE
        std::wstring temp(std::to_wstring(lstCreation.wYear));
        temp.append(TEXT("年"));
        temp.append(std::to_wstring(lstCreation.wMonth));
        temp.append(TEXT("月"));
        temp.append(std::to_wstring(lstCreation.wDay));
        temp.append(TEXT("日 "));
        if (lstCreation.wHour < 10) temp.append(1, '0');
        temp.append(std::to_wstring(lstCreation.wHour));
        temp.append(TEXT(":"));
        if (lstCreation.wMinute < 10) temp.append(1, '0');
        temp.append(std::to_wstring(lstCreation.wMinute));
        temp.append(TEXT(":"));
        if (lstCreation.wSecond < 10) temp.append(1, '0');
        temp.append(std::to_wstring(lstCreation.wSecond));
#else
        std::string temp(std::to_string(lstCreation.wYear));
        temp.append(TEXT("年"));
        temp.append(std::to_string(lstCreation.wMonth));
        temp.append(TEXT("月"));
        temp.append(std::to_string(lstCreation.wDay));
        temp.append(TEXT("日 "));
        if (lstCreation.wHour < 10) temp.append(1, '0');
        temp.append(std::to_string(lstCreation.wHour));
        temp.append(TEXT(":"));
        if (lstCreation.wMinute < 10) temp.append(1, '0');
        temp.append(std::to_string(lstCreation.wMinute));
        temp.append(TEXT(":"));
        if (lstCreation.wSecond < 10) temp.append(1, '0');
        temp.append(std::to_string(lstCreation.wSecond));
#endif
        CString csTime(temp.c_str());
        if (EnumProcessModules(hProcess, &hModule, sizeof(HMODULE), &cbNeeded))
        {
            if (GetModuleFileNameEx(hProcess, hModule, fileName, MAX_PATH))
            {
                CString pathName(fileName);
#ifdef _UNICODE
                std::wstring stringProcId = std::to_wstring(processId);
#else
                std::string stringProcId = std::to_string(processId);
#endif
                CString csProcId(stringProcId.c_str());
                CProcessInfo processInfo(nullptr, pathName, csTime, csProcId);
                processInfo.DoModal();
            }
        }
    }
}

void CUSBDeviceManagerGUIDlg::UpdateControlRect()
{
    for (unsigned int i = 0; i < 13; ++i)
    {
        //GetDlgItem(id[i])->GetWindowRect(rectControls + i);
        CWnd* control = GetDlgItem(id[i]);
        control->GetWindowRect(rectControls + i);
    }

}

void CUSBDeviceManagerGUIDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
    CDialogEx::OnMoving(fwSide, pRect);
    UpdateControlRect();
    // TODO: 在此处添加消息处理程序代码
}

void CUSBDeviceManagerGUIDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
    CDialogEx::OnSizing(fwSide, pRect);
    UpdateControlRect();
    // TODO: 在此处添加消息处理程序代码
}

void CUSBDeviceManagerGUIDlg::OnBnClickedButtonAble()
{
    // TODO: 在此添加控件通知处理程序代码
    CString item_text = listDevice.GetItemText(listDevice.GetSelectionMark(), 0);
    unsigned int letter_index = item_text.Find('(', 0) + 1;
    TCHAR letter = item_text[letter_index];
    CDisableDevice disableDevice(nullptr, letter);
    disableDevice.DoModal();
}
