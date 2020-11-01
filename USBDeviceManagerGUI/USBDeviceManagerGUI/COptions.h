#pragma once
#include "CCustomProgressCtrl.h"

// COptions dialog

class COptions : public CDialogEx
{
	DECLARE_DYNAMIC(COptions)

public:
	COptions(CWnd* pParent = nullptr);   // standard constructor
    COptions(CWnd* pParent, CDiskSpaceIndicator* _indicator);
	virtual ~COptions();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CDiskSpaceIndicator* indicator;
public:
    CStatic labelAlertThreshold;
    virtual BOOL OnInitDialog();
    CMFCColorButton colorNormal;
    CMFCColorButton colorAlert;
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnApply();
    CButton buttonApply;
    afx_msg void OnBnClickedColorButton();
    CEdit editAlertThreshold;
    afx_msg void OnEnChangeEditAlertThreshold();
};
