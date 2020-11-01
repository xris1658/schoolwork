#pragma once


// CDelete 对话框

class CDelete : public CDialogEx
{
	DECLARE_DYNAMIC(CDelete)

public:
	CDelete(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDelete();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString* pSource;
    CStatic labelSource;
    CEdit editSource;
    CButton buttonFile;
    CButton buttonFolder;
    char letter;
    enum SOURCE_TYPE { UNDEFINED, TYPE_FILE, TYPE_FOLDER } source_type = SOURCE_TYPE::UNDEFINED;
    afx_msg void OnBnClickedOk();
    afx_msg void OnClickedButtonFile();
    afx_msg void OnClickedButtonFolder();
    afx_msg void OnBnClickedDeletePermanently();
    int DeleteOperation(bool deletePermanently=true);
};
