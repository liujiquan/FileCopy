//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   ProgressDialog.h
//* Author   :   liujiquan
//* DateTime :   7/9/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __PROGRESSDIALOG_H_2459134951B94A1895E650CFD52F4215__
#define __PROGRESSDIALOG_H_2459134951B94A1895E650CFD52F4215__
#pragma once

#define Jintutiao_seconds	40
#define MAX_RANGE			1000
// Define
#define Progress_WIDTH		100
#define Progress_HEIGHT		100
#define Progress_Hei		0
#define LINE_WID			4.0
#define MIN_INNER			18
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
// GDI
#define PI	3.1415926535897932354626
#define CircleMaxDegree			360
#define CircleMaxHalfDegree		(CircleMaxDegree/2)
#define Step					30
#define MaxIndex				(CircleMaxDegree / Step)

#define Color2					RGB(207, 197, 197)
#define Color1					RGB(128, 128, 128)
#define COLOR_BEGIN				0xE5
#define COLOR_END				0x2C
#define COLOR_Step				((double)(COLOR_END - COLOR_BEGIN) / (double)MaxIndex)
#define Caption_Color			Color(255, 255, 0, 0)
#define Caption_RGB				RGB(255, 0, 0)
#define Caption_Size			11

#define CIRCLE_EVENT			2

class CProgressDlgExt : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlgExt)
public:
	CProgressDlgExt(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlgExt();
	enum { IDD = IDD_DIALOG_PROGRESS };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	BOOL				m_bRunning;
	CString				m_strCaption;
private:
	int					m_width;
	int					m_height;
	// 2
	HDC					m_BackHDC;		
	HBITMAP				m_BackBitmap;		
	HGDIOBJ				m_BackOldBmp;	
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
	CPoint	GetPoint(CPoint center, int radius, double angle);
	PointF	GetPoint2(CPoint center, int radius, double angle);
	RECT	GetRect(CPoint center, int innerRadius);
	RectF	GetRect2(CPoint center, int innerRadius);
	COLORREF GetColor(int index);
	Color	GetColor2(int index);
	void	OnDrawWindow();
private:
	int			m_Transparency;	
	void		SetTransparencyValue(int iTransparency);
public:
	void		SetTransparency(int iTransparency);
	void		SetCaption(CString strCaption){ m_strCaption = strCaption; }
	void		Start();
	void		Stop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
#endif//__PROGRESSDIALOG_H_2459134951B94A1895E650CFD52F4215__
