//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   HeaderCtrlExt.h
//* Author   :   liujiquan
//* DateTime :   7/10/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __HEADERCTRLEXT_H_2459134951B94A1895E650CFD52F4215__
#define __HEADERCTRLEXT_H_2459134951B94A1895E650CFD52F4215__
#pragma once
#include <vector>
#include <array>
using namespace std;
// CHeaderCtrlExt
#define CHECK_BOX_BUTTON WM_USER+1001
#define CHECKBOX_SIZE		14
#define UpCOLOR1			RGB(227, 247 ,255)
#define DownCOLOR1			RGB(187, 235 ,254)

#define UpCOLOR2			RGB(188, 228 ,249)
#define DownCOLOR2			RGB(139, 209 ,246)
typedef struct HEADDITEM{
	int			nCol;			// nIndex
	TCHAR		szText[MAX_PATH];	// caption
	int			fmt;				// DT_LEFT
	bool			bCheckBox;		// CheckBox
	bool			bCheckStatus;		// CheckStatus
	HEADDITEM()
	{
		memset(this, 0x00, sizeof(HEADDITEM));
	}
	HEADDITEM& operator = (const HEADDITEM& _this)
	{
		memset(this, 0x00, sizeof(HEADDITEM));

		this->nCol = _this.nCol;
		memcpy(this->szText, _this.szText, MAX_PATH * sizeof(TCHAR));
		this->fmt = _this.fmt;
		this->bCheckBox = _this.bCheckBox;
		this->bCheckStatus = _this.bCheckStatus;

		return *this;
	}
}HEADDITEM, *PHEADDITEM;
struct HotLight{
	int		nItem;
	bool		bHover;
};
class CHeaderCtrlExt : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlExt)

public:
	CHeaderCtrlExt();
	virtual ~CHeaderCtrlExt();
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
private:
	int				m_Height;
	COLORREF		m_TextColor;
	COLORREF		m_BkColor;
	HFONT			m_Font;
	HotLight		m_bHover;
	BOOL			m_bMouseTrack;
public:
	void SetHeight(int height){ m_Height = height; }
	void SetFont(HFONT font){ m_Font = font; }
	void SetOwnerDraw(BOOL bOwnerDraw = TRUE);
public:
	LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
	void DrawImage(HDC hDC, const CRect &rc);
	void DrawText(HDC hDC, const CRect &rc, LPCTSTR pszText, UINT format);

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
#endif//__HEADERCTRLEXT_H_2459134951B94A1895E650CFD52F4215__

