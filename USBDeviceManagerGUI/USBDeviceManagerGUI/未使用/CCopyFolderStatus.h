#pragma once


// CCopyFolderStatus dialog

class CCopyFolderStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CCopyFolderStatus)

public:
	CCopyFolderStatus(CWnd* pParent = nullptr);   // standard constructor
    CCopyFolderStatus(CWnd* pParent, LPSHFILEOPSTRUCT pFolderOperation);
	virtual ~CCopyFolderStatus();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COPY_FOLDER_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CLinkCtrl linkSource;
    CLinkCtrl linkDest;
    CStatic labelFileStatus;
    CStatic labelAllStatus;
    CProgressCtrl progressFileStatus;
    CProgressCtrl progressAllStatus;
    LPSHFILEOPSTRUCT pFolderOperation;
    virtual BOOL OnInitDialog();
};
