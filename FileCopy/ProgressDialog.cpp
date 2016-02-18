//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   ProgressDialog.cpp
//* Author   :   liujiquan
//* DateTime :   7/9/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "ProgressDialog.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CProgressDlgExt dialog
IMPLEMENT_DYNAMIC(CProgressDlgExt, CDialogEx)
CProgressDlgExt::CProgressDlgExt(CWnd* pParent /*=NULL*/): CDialogEx(CProgressDlgExt::IDD, pParent)
{
	GdiplusStartupInput gidplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gidplusStartupInput, NULL);

	m_Transparency = 0;
	m_bRunning = FALSE;
	m_strCaption = _T("");
	m_Dlgbrush = NULL;
}

CProgressDlgExt::~CProgressDlgExt()
{
	GdiplusShutdown(m_gdiplusToken);

	if (m_Dlgbrush)
	{
		::DeleteObject(m_Dlgbrush);
		m_Dlgbrush = NULL;
	}
}

void CProgressDlgExt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProgressDlgExt, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::OnInitDialog
// Return   :   BOOL
// Comment  :   OnInitDialog
// -----------------------------------------------------------//
BOOL CProgressDlgExt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_width = Progress_WIDTH;
	m_height = Progress_HEIGHT;
	
	m_TipFont = (HFONT)GetFont()->Detach();
	
	MoveWindow(0, 0, m_width, m_height);
	CenterWindow();
	// 3.
	int nSize = m_height > m_width ? m_width : m_height;

	m_Center.x = m_width / 2;
	m_Center.y = m_height / 2;
	m_OutterRadius = nSize / 2;
	m_InnerRadius = m_OutterRadius / 4 <= MIN_INNER ? MIN_INNER : m_OutterRadius / 4;
	m_Index = 0;

	SetTransparencyValue(m_Transparency);
	COLORREF maskColor = RGB(255, 255, 255);
	SetWindowLong(this->GetSafeHwnd(),
		GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), 
		GWL_EXSTYLE) | 0x80000);			
	SetLayeredWindowAttributes(maskColor, 255, 1);

	m_Dlgbrush = CreateSolidBrush(RGB(255, 255, 255));

	return TRUE;
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::SetRect
// Param    :   int wid
//              int hei
// Return   :   void
// Comment  :   SetRect
// -----------------------------------------------------------//
void CProgressDlgExt::SetRect(int wid, int hei)
{
	m_width = wid;
	m_height = hei;
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::OnTimer
// Param    :   UINT_PTR nIDEvent
// Return   :   void
// Comment  :   OnTimer
// -----------------------------------------------------------//
void CProgressDlgExt::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == jindutiao2_EVENT)
	{
		if (++m_Index == MaxIndex)
		{
			m_Index = 0;
		}
		Invalidate(TRUE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::GetColor
// Param    :   int index
// Return   :   COLORREF
// Comment  :   GetColor
// -----------------------------------------------------------//
COLORREF CProgressDlgExt::GetColor(int index)
{
	Color color = GetColor2(index);

	return color.ToCOLORREF();
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::GetColor2
// Param    :   int index
// Return   :   Color
// Comment  :   GetColor2
// -----------------------------------------------------------//
Color CProgressDlgExt::GetColor2(int index)
{
	int nColorRGB = COLOR_BEGIN + COLOR_Step * index;

	return Color(nColorRGB, nColorRGB, nColorRGB);
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::GetPoint
// Param    :   CPoint center
//              int radius
//              double angle
// Return   :   CPoint
// Comment  :   GetPoint
// -----------------------------------------------------------//
CPoint CProgressDlgExt::GetPoint(CPoint center, int radius, double angle)
{
	double  dblAngle = PI * angle / CircleMaxHalfDegree;
	int xPos = center.x + radius * (float)sin(dblAngle);
	int yPos = center.y - radius * (float)cos(dblAngle);

	return CPoint(xPos, yPos);
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::GetPoint2
// Param    :   CPoint center
//              int radius
//              double angle
// Return   :   PointF
// Comment  :   GetPoint2
// -----------------------------------------------------------//
PointF CProgressDlgExt::GetPoint2(CPoint center, int radius, double angle)
{
	CPoint point = GetPoint(center, radius, angle);

	return PointF(point.x, point.y);
}

// -----------------------------------------------------------//
// Function :   RECT	CProgressDlgExt::GetRect
// Param    :   CPoint center
//              int innerRadius
// Return   :   
// Comment  :   GetRect
// -----------------------------------------------------------//
RECT	CProgressDlgExt::GetRect(CPoint center, int innerRadius)
{
	RECT rect;
	rect.left = center.x - innerRadius;
	rect.top = center.y - innerRadius;
	rect.right = rect.left + innerRadius * 2;
	rect.bottom = rect.top + innerRadius * 2;

	return rect;
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::GetRect2
// Param    :   CPoint center
//              int innerRadius
// Return   :   RectF
// Comment  :   GetRect2
// -----------------------------------------------------------//
RectF CProgressDlgExt::GetRect2(CPoint center, int innerRadius)
{
	return RectF(center.x - innerRadius, center.y - innerRadius, innerRadius * 2, innerRadius * 2);
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::OnPaint
// Return   :   void
// Comment  :   OnPaint
// -----------------------------------------------------------//
void CProgressDlgExt::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps);

	Gdiplus::Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	for (int i = m_Index; i - m_Index < MaxIndex; i++)
	{
		PointF innerPoint = GetPoint2(m_Center, m_InnerRadius, i * Step);
		PointF outterPoint = GetPoint2(m_Center, m_OutterRadius, i * Step);
		Gdiplus::Pen Linepen(GetColor2(i - m_Index), LINE_WID);

		graphics.DrawLine(&Linepen, innerPoint, outterPoint);
	}

	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Font font(hdc, m_TipFont);

	// StringFroamt
	Gdiplus::StringFormat stringFormat;
	if (stringFormat.GetLastStatus() == Ok)
	{
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	Gdiplus::SolidBrush linGrBrush(Caption_Color);

	graphics.DrawString(m_strCaption, m_strCaption.GetLength(),
		&font, GetRect2(m_Center, m_InnerRadius), &stringFormat, &linGrBrush);

	::EndPaint(m_hWnd, &ps);
}

// -----------------------------------------------------------//
// Function :   void	CProgressDlgExt::SetTransparency
// Param    :   int iTransparency
// Return   :   
// Comment  :   SetTransparency
// -----------------------------------------------------------//
void	CProgressDlgExt::SetTransparency(int iTransparency)
{
	m_Transparency = iTransparency;
}

// -----------------------------------------------------------//
// Function :   CProgressDlgExt::SetTransparencyValue
// Param    :   int iTransparency
// Return   :   void
// Comment  :   SetTransparencyValue
// -----------------------------------------------------------//
void CProgressDlgExt::SetTransparencyValue(int iTransparency)
{
	DWORD dwExStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	if (m_Transparency == 0)
	{
		dwExStyle &= ~WS_EX_LAYERED;
	}
	else
	{
		dwExStyle |= WS_EX_LAYERED;
	}
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle);

	SetLayeredWindowAttributes(0, 255 * iTransparency / 100, LWA_ALPHA);
}

// -----------------------------------------------------------//
// Function :   void	CProgressDlgExt::Start
// Return   :   
// Comment  :   Start
// -----------------------------------------------------------//
void	CProgressDlgExt::Start()
{
	SetTimer(jindutiao2_EVENT, Jintutiao_seconds, NULL);
	ShowWindow(SW_SHOW);
}

// -----------------------------------------------------------//
// Function :   void	CProgressDlgExt::Stop
// Return   :   
// Comment  :   Stop
// -----------------------------------------------------------//
void	CProgressDlgExt::Stop()
{
	KillTimer(jindutiao2_EVENT);
	ShowWindow(SW_HIDE);
}


HBRUSH CProgressDlgExt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)  
	{
		return m_Dlgbrush;
	}
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(125, 0, 0));
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	return hbr;
}
