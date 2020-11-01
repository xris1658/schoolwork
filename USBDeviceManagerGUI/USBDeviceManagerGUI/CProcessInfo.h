#pragma once


// CProcessInfo 对话框

class CProcessInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessInfo)

public:
	CProcessInfo(CWnd* pParent = nullptr);   // 标准构造函数
    CProcessInfo(CWnd* pParent, CString pathName, CString startupTime, CString pid);
	virtual ~CProcessInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
    CString pathName;
    CString startupTime;
    CString pid;
public:
    virtual BOOL OnInitDialog();
    CListCtrl listProcessInfo;
};
