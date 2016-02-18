//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   ProgressBatExt.cpp
//* Author   :   liujiquan
//* DateTime :   7/23/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "ProgressBarExt.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CProgressBarExt dialog
IMPLEMENT_DYNAMIC(CProgressBarExt, CDialogEx)
CProgressBarExt::CProgressBarExt(CWnd* pParent /*=NULL*/): CDialogEx(CProgressBarExt::IDD, pParent)
{
	GdiplusStartupInput gidplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gidplusStartupInput, NULL);

	m_bRunning = FALSE;
	m_Dlgbrush = NULL;
}

CProgressBarExt::~CProgressBarExt()
{
	GdiplusShutdown(m_gdiplusToken);

	if (m_Dlgbrush)
	{
		::DeleteObject(m_Dlgbrush);
		m_Dlgbrush = NULL;
	}
}

void CProgressBarExt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProgressBarExt, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// -----------------------------------------------------------//
// Function :   CProgressBarExt::OnInitDialog
// Return   :   BOOL
// Comment  :   OnInitDialog
// -----------------------------------------------------------//
BOOL CProgressBarExt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_width = Progress_WIDTH;
	m_height = Progress_HEIGHT;
	
	MoveWindow(0, 0, m_width, m_height);
	CenterWindow();
	// 3.
	int nSize = m_height > m_width ? m_width : m_height;

	m_Center.x = m_width / 2;
	m_Center.y = m_height / 2;
	m_OutterRadius = nSize / 2;
	m_InnerRadius = m_OutterRadius / 4 <= MIN_INNER ? MIN_INNER : m_OutterRadius / 4;
	m_Index = 0;

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
// Function :   CProgressBarExt::SetRect
// Param    :   int wid
//              int hei
// Return   :   void
// Comment  :   SetRect
// -----------------------------------------------------------//
void CProgressBarExt::SetRect(int wid, int hei)
{
	m_width = wid;
	m_height = hei;
}

// -----------------------------------------------------------//
// Function :   CProgressBarExt::OnTimer
// Param    :   UINT_PTR nIDEvent
// Return   :   void
// Comment  :   OnTimer
// -----------------------------------------------------------//
void CProgressBarExt::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == CIRCLE_EVENT)
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
// Function :   CProgressBarExt::GetColor2
// Param    :   int index
// Return   :   Color
// Comment  :   GetColor2
// -----------------------------------------------------------//
Color CProgressBarExt::GetColor2(int index)
{
	int nColorRGB = COLOR_BEGIN + COLOR_Step * index;

	return Color(nColorRGB, nColorRGB, nColorRGB);
}


Rect CProgressBarExt::GetRect()
{
	Rect	rect;
	Gdiplus::PointF innerPoint = GetPoint2(m_Center, m_InnerRadius, 0);
	Gdiplus::PointF outterPoint = GetPoint2(m_Center, m_OutterRadius, 0);

	rect.X = m_Center.x - ELLIPSE_WID / 2;
	rect.Y = outterPoint.Y;
	rect.Width = ELLIPSE_WID;
	rect.Height = innerPoint.Y - outterPoint.Y;

	return rect;
}

// -----------------------------------------------------------//
// Function :   CProgressBarExt::GetPoint
// Param    :   CPoint center
//              int radius
//              double angle
// Return   :   CPoint
// Comment  :   GetPoint
// -----------------------------------------------------------//
CPoint CProgressBarExt::GetPoint(CPoint center, int radius, double angle)
{
	double  dblAngle = PI * angle / CircleMaxHalfDegree;
	int xPos = center.x + radius * (float)sin(dblAngle);
	int yPos = center.y - radius * (float)cos(dblAngle);

	return CPoint(xPos, yPos);
}

// -----------------------------------------------------------//
// Function :   CProgressBarExt::GetPoint2
// Param    :   CPoint center
//              int radius
//              double angle
// Return   :   PointF
// Comment  :   GetPoint2
// -----------------------------------------------------------//
PointF CProgressBarExt::GetPoint2(CPoint center, int radius, double angle)
{
	CPoint point = GetPoint(center, radius, angle);

	return PointF(point.x, point.y);
}

// -----------------------------------------------------------//
// Function :   CProgressBarExt::OnPaint
// Return   :   void
// Comment  :   OnPaint
// -----------------------------------------------------------//
void CProgressBarExt::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps);

	Gdiplus::Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Matrix matrix;
	Gdiplus::SolidBrush lineBrush(NULL);
	
	// rotate
	matrix.RotateAt(m_Index * Step, PointF(m_Center.x, m_Center.y));
	graphics.SetTransform(&matrix);
	for (int i = m_Index; i - m_Index < MaxIndex; i++)
	{
		// draw
		lineBrush.SetColor(GetColor2(i - m_Index));
		graphics.FillEllipse(&lineBrush, GetRect());

		// rotate
		matrix.RotateAt(Step, PointF(m_Center.x, m_Center.y));
		graphics.SetTransform(&matrix);
	}

	::EndPaint(m_hWnd, &ps);
}

// -----------------------------------------------------------//
// Function :   void	CProgressBarExt::Start
// Return   :   
// Comment  :   Start
// -----------------------------------------------------------//
void CProgressBarExt::Start()
{
	
	SetTimer(CIRCLE_EVENT, Jintutiao_seconds, NULL);
	ShowWindow(SW_SHOW);
	if (GetParent() && GetParent()->GetSafeHwnd())
	{
		CenterWindow(GetParent());
		GetParent()->EnableWindow(FALSE);
	}
}

// -----------------------------------------------------------//
// Function :   void	CProgressBarExt::Stop
// Return   :   
// Comment  :   Stop
// -----------------------------------------------------------//
void CProgressBarExt::Stop()
{
	KillTimer(CIRCLE_EVENT);
	ShowWindow(SW_HIDE);
	if (GetParent() && GetParent()->GetSafeHwnd())
	{
		CenterWindow(GetParent());
		GetParent()->EnableWindow(TRUE);
	}
}


HBRUSH CProgressBarExt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
