#pragma once

// CCustomProgressCtrl

class CCustomProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CCustomProgressCtrl)

public:
    CCustomProgressCtrl(
        COLORREF _borderColor=RGB(188, 188, 188),
        COLORREF _backgroundColor=RGB(230, 230, 230),
        COLORREF _foregroundColor=RGB(218, 38, 38));
	virtual ~CCustomProgressCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
protected:
    COLORREF borderColor;
    COLORREF backgroundColor;
    COLORREF foregroundColor;
};

class CDiskSpaceIndicator : public CCustomProgressCtrl
{
    DECLARE_DYNAMIC(CDiskSpaceIndicator)
public:
    CDiskSpaceIndicator(
        COLORREF _borderColor=RGB(188, 188, 188),
        COLORREF _backgroundColor=RGB(230, 230, 230),
        COLORREF _normalForegroundColor=RGB(38, 160, 218),
        COLORREF _alertForegroundColor=RGB(218, 38, 38),
        unsigned int _alertThreshold=90);
    virtual ~CDiskSpaceIndicator();
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
public:
    inline unsigned int getAlertThreshold() { return alertThreshold; }
    void setAlertThreshold(unsigned int threshold) { alertThreshold = threshold; }
    inline COLORREF getBackgroundColor() { return backgroundColor; }
    void setBackgroundColor(COLORREF color) { backgroundColor = color; }
    inline COLORREF getNormalForegroundColor() { return normalForegroundColor; }
    void setNormalForegroundColor(COLORREF color) { normalForegroundColor = color; }
    inline COLORREF getAlertForegroundColor() { return alertForegroundColor; }
    void setAlertForegroundColor(COLORREF color) { alertForegroundColor = color; }
protected:
    COLORREF& normalForegroundColor=foregroundColor;
    COLORREF alertForegroundColor;
    unsigned int alertThreshold;
};