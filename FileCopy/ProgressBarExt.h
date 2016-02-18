//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   ProgressBatExt.h
//* Author   :   liujiquan
//* DateTime :   7/23/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __PROGRESSBAREXT_H_2459134951B94A1895E650CFD52F4215__
#define __PROGRESSBAREXT_H_2459134951B94A1895E650CFD52F4215__
#pragma once

#define Jintutiao_seconds	30
// Define
#define Progress_WIDTH		50
#define Progress_HEIGHT		50
#define LINE_WID			1.0
#define MIN_INNER			6
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
// GDI
#define PI	3.1415926535897932354626
#define CircleMaxDegree			360
#define CircleMaxHalfDegree		(CircleMaxDegree/2)
#define Step					30
#define MaxIndex				(CircleMaxDegree / Step)
// Color
#define COLOR_BEGIN				0xE5
#define COLOR_END				0x2C
#define COLOR_Step				((double)(COLOR_END - COLOR_BEGIN) / (double)MaxIndex)
// Size
#define ELLIPSE_WID				4
// Event
#define CIRCLE_EVENT			2

class CProgressBarExt : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressBarExt)
public:
	CProgressBarExt(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressBarExt();
	enum { IDD = IDD_DIALOG_PROGRESS };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	BOOL				m_bRunning;
private:
	int					m_width;
	int					m_height;
	// 3
	HFONT				m_TipFont;			// TipFont
	HBRUSH				m_Dlgbrush;
public:
	void SetRect(int wid, int hei);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
private:
	ULONG_PTR m_gdiplusToken;

	CPoint	m_Center;			
	int		m_OutterRadius;	
	int		m_InnerRadius;	
	int		m_Index;		
private:
	CPoint		GetPoint(CPoint center, int radius, double angle);
	PointF		GetPoint2(CPoint center, int radius, double angle);
	Color		GetColor2(int index);
	Rect		GetRect();
public:
	void		Start();
	void		Stop();
	afx_msg		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
#endif//__PROGRESSBAREXT_H_2459134951B94A1895E650CFD52F4215__
