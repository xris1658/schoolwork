
// USBDeviceManagerGUIDlg.h: 头文件
//

#pragma once
#include "CCustomProgressCtrl.h"

#include <vector>
#include <map>

// CUSBDeviceManagerGUIDlg 对话框
class CUSBDeviceManagerGUIDlg : public CDialogEx
{
// 构造
public:
	CUSBDeviceManagerGUIDlg(CWnd* pParent = nullptr);	// 标准构造函数
private:
    CONST CHAR unit_name[9] = " KMGTPEZ";
    DWORD GetUsbDrives();
    void RefreshUsbDevice();
    void Unselect();
    void Select();
    void GetDiskSpace(unsigned short int letter);
    short int IsUsbDrive(unsigned short int letter/*A=0, ..., Z=25*/);
    void FixSizeAndUnit(long double& size, unsigned short int& unit);
    inline unsigned short int getWarningThreshold() { return progressDiskSpaceIndicator.getAlertThreshold(); }
    inline void setWarningThreshold(unsigned int _warningThreshold) { progressDiskSpaceIndicator.setAlertThreshold(_warningThreshold); }
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBDEVICEMANAGERGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClickedButtonRefresh();
    afx_msg void OnLvnItemActivateListDevice(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickListDevice(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonCopy();
    afx_msg void OnBnClickedButtonAble();
    afx_msg void OnClickedButtonDelete();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnClickedButtonOptions();
    afx_msg void OnBnClickedButtonPcb();
    afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
private:
    CButton buttonRefresh;
    CListCtrl listDevice;
    CButton buttonAble;
    CButton buttonCopy;
    CButton buttonDelete;
    CButton buttonOptions;
    CButton buttonProcessInfo;
    CEdit textInfo;
    CStatic labelAll;
    CStatic labelAvailable;
    CStatic labelName;
    CStatic labelUsed;
    CDiskSpaceIndicator progressDiskSpaceIndicator;
    std::vector<CWnd*> usbControls;
    static const UINT m_taskbarBtnCreatedMsg;
private:
    void UpdateControlRect();
};
