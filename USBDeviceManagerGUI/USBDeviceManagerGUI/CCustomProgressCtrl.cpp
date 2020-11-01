// CCustomProgressCtrl.cpp : implementation file
//

#include "pch.h"
#include "CCustomProgressCtrl.h"

// CCustomProgressCtrl

IMPLEMENT_DYNAMIC(CCustomProgressCtrl, CProgressCtrl)

CCustomProgressCtrl::CCustomProgressCtrl(COLORREF _borderColor, COLORREF _backgroundColor, COLORREF _foregroundColor)
    : borderColor(_borderColor), backgroundColor(_backgroundColor), foregroundColor(_foregroundColor) {}

CCustomProgressCtrl::~CCustomProgressCtrl() {}


BEGIN_MESSAGE_MAP(CCustomProgressCtrl, CProgressCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CCustomProgressCtrl message handlers
void CCustomProgressCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CProgressCtrl::OnPaint() for painting messages
    CPen borderPen;
    borderPen.CreatePen(PS_SOLID, 1*2, borderColor);
    //HBRUSH bgBrush = CreateSolidBrush(backgroundColor);
    //HBRUSH fgBrush = CreateSolidBrush(foregroundColor);
    CBrush bgBrush; bgBrush.CreateSolidBrush(backgroundColor);
    CBrush fgBrush; fgBrush.CreateSolidBrush(foregroundColor);
    RECT backgroundRect; this->GetClientRect(&backgroundRect);
    RECT foregroundRect = backgroundRect;
    ++foregroundRect.left; ++foregroundRect.top;
    --foregroundRect.right; --foregroundRect.bottom;
    int lower, upper, pos;
    GetRange(lower, upper);
    pos=GetPos();
    double value = (double)(pos-lower) / (double)(upper-lower);
    int width = (int)((foregroundRect.right - foregroundRect.left)*value);
    foregroundRect.right = foregroundRect.left + width;
    dc.SelectObject(bgBrush);
    dc.FillRect(&backgroundRect, &bgBrush);
    dc.SelectObject(borderPen);
    dc.MoveTo(backgroundRect.left, backgroundRect.top); //top-left
    dc.LineTo(backgroundRect.left, backgroundRect.bottom); //down
    dc.LineTo(backgroundRect.right, backgroundRect.bottom); //right
    dc.LineTo(backgroundRect.right, backgroundRect.top); //up
    dc.LineTo(backgroundRect.left, backgroundRect.top); //left
    dc.SelectObject(fgBrush);
    dc.FillRect(&foregroundRect, &fgBrush);
}

// CDiskSpaceIndicator

IMPLEMENT_DYNAMIC(CDiskSpaceIndicator, CCustomProgressCtrl)

CDiskSpaceIndicator::CDiskSpaceIndicator(COLORREF _borderColor, COLORREF _backgroundColor, COLORREF _normalForegroundColor, COLORREF _alertForegroundColor, unsigned int _alertThreshold)
    : CCustomProgressCtrl(_borderColor, _backgroundColor, _normalForegroundColor)
{
    foregroundColor = normalForegroundColor;
    alertForegroundColor = _alertForegroundColor;
    alertThreshold = _alertThreshold;
}

CDiskSpaceIndicator::~CDiskSpaceIndicator() {}

BEGIN_MESSAGE_MAP(CDiskSpaceIndicator, CCustomProgressCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CDiskSpaceIndicator::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CProgressCtrl::OnPaint() for painting messages
    CPen borderPen;
    borderPen.CreatePen(PS_SOLID, 1 * 2, borderColor);
    //HBRUSH bgBrush = CreateSolidBrush(backgroundColor);
    //HBRUSH fgBrush = CreateSolidBrush(foregroundColor);
    CBrush bgBrush; bgBrush.CreateSolidBrush(backgroundColor);
    RECT backgroundRect; this->GetClientRect(&backgroundRect);
    RECT foregroundRect = backgroundRect;
    ++foregroundRect.left; ++foregroundRect.top;
    --foregroundRect.right; --foregroundRect.bottom;
    int lower, upper, pos;
    GetRange(lower, upper);
    pos = GetPos();
    double value = (double)(pos - lower) / (double)(upper - lower);
    CBrush fgBrush; fgBrush.CreateSolidBrush((value>=alertThreshold*0.01)? alertForegroundColor : normalForegroundColor);
    int width = (int)((foregroundRect.right - foregroundRect.left)*value);
    foregroundRect.right = foregroundRect.left + width;
    dc.SelectObject(bgBrush);
    dc.FillRect(&backgroundRect, &bgBrush);
    dc.SelectObject(borderPen);
    dc.MoveTo(backgroundRect.left, backgroundRect.top); //top-left
    dc.LineTo(backgroundRect.left, backgroundRect.bottom); //down
    dc.LineTo(backgroundRect.right, backgroundRect.bottom); //right
    dc.LineTo(backgroundRect.right, backgroundRect.top); //up
    dc.LineTo(backgroundRect.left, backgroundRect.top); //left
    dc.SelectObject(fgBrush);
    dc.FillRect(&foregroundRect, &fgBrush);
}