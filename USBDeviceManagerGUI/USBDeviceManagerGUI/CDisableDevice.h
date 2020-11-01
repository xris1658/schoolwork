#pragma once


// CDisableDevice 对话框

class CDisableDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CDisableDevice)

public:
	CDisableDevice(CWnd* pParent = nullptr);   // 标准构造函数
    CDisableDevice(CWnd* pParent, TCHAR letter);
	virtual ~CDisableDevice();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISABLE_DEVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CComboBox comboType;
    CEdit editTypeHelp;
    CButton buttonOperate;
    CButton buttonClose;
    afx_msg void OnSelchangeComboType();
    virtual void OnOK();
public:
    enum DisableType {TYPE_SAFE_REMOVE=0, TYPE_UNMOUNT=1, TYPE_DISABLE_DEVICE=2} disableType;
    TCHAR letter;
    bool disabled = false;
    static const unsigned int dosPathLength = 1024;
    TCHAR dosPath[dosPathLength];
    virtual BOOL OnInitDialog();
};
