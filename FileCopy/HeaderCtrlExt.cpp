//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   HeaderCtrlExt.cpp
//* Author   :   liujiquan
//* DateTime :   7/10/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "HeaderCtrlExt.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CHeaderCtrlExt
IMPLEMENT_DYNAMIC(CHeaderCtrlExt, CHeaderCtrl)
CHeaderCtrlExt::CHeaderCtrlExt()
{
	m_BkColor = RGB(255, 255, 255);
	m_TextColor = RGB(0, 0, 0);
	m_Font = NULL;
	m_bMouseTrack = TRUE;
	m_Height = 0;
}

CHeaderCtrlExt::~CHeaderCtrlExt()
{
}

BEGIN_MESSAGE_MAP(CHeaderCtrlExt, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()

void CHeaderCtrlExt::PreSubclassWindow()
{
	CHeaderCtrl::PreSubclassWindow();
}

void CHeaderCtrlExt::SetOwnerDraw(BOOL bOwnerDraw/* = TRUE */)
{
	HDITEM item;
	item.mask = HDI_FORMAT;
	int nCount = GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		GetItem(i, &item);
		item.fmt |= HDF_OWNERDRAW;
		SetItem(i, &item);
	}
}

LRESULT CHeaderCtrlExt::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, wParam, lParam);
	HD_LAYOUT& hdl = *(HD_LAYOUT*)lParam;
	RECT *rect = hdl.prc;
	WINDOWPOS *winPos = hdl.pwpos;

	winPos->cy = m_Height;
	rect->top = m_Height;

	return lResult;
}

void CHeaderCtrlExt::DrawImage(HDC hDC, const CRect &rc)
{

}

void CHeaderCtrlExt::DrawText(HDC hDC, const CRect &rc, LPCTSTR pszText, UINT format)
{
	CRect rect(rc);
	int bkMode = ::SetBkMode(hDC, TRANSPARENT);
	COLORREF oldColor = ::SetTextColor(hDC, m_TextColor);
	HGDIOBJ oldFont = ::SelectObject(hDC, m_Font);

	UINT nAlign = DT_VCENTER | DT_SINGLELINE;
	switch (format)
	{
	case LVCFMT_LEFT:
		nAlign |= DT_LEFT;	break;
	case LVCFMT_CENTER:
		nAlign |= DT_CENTER;	break;
	case LVCFMT_RIGHT:
		nAlign |= DT_RIGHT;	break;
	case LVCFMT_JUSTIFYMASK:
		nAlign |= DT_LEFT;	break;
	default:
		nAlign |= DT_LEFT;	break;
	}
	::DrawText(hDC, pszText, _tcslen(pszText), rect, nAlign);

	::SetBkMode(hDC, bkMode);
	::SetTextColor(hDC, oldColor);
	::SelectObject(hDC, oldFont);
}

void CHeaderCtrlExt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);

	int nItem = (int)lpDrawItemStruct->itemID;
	HDC hdc = lpDrawItemStruct->hDC;
	CRect rectMain(lpDrawItemStruct->rcItem);
	CRect rect(lpDrawItemStruct->rcItem);
	rect.DeflateRect(1, 1);
	
	UINT nState = lpDrawItemStruct->itemState;
	UINT nAction = lpDrawItemStruct->itemAction;
	CDC pDC;
	pDC.Attach(hdc);
	COLORREF oldBkColor = pDC.GetBkColor();
	COLORREF oldTextColor = pDC.GetTextColor();

	// Fill
	if ((nState & ODS_SELECTED))						// Clicked
	{
		CRect rcFrame1(rectMain);
		CRect rcFrame2(rectMain);
		rcFrame1.bottom = rcFrame1.bottom - (rcFrame1.Height() * 3 / 5);
		rcFrame2.top = rcFrame1.bottom;

		pDC.FillSolidRect(&rcFrame1, UpCOLOR2);
		pDC.FillSolidRect(&rcFrame2, DownCOLOR2);
		pDC.DrawEdge(&rectMain, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		if (m_bHover.nItem == nItem && m_bHover.bHover)	// Hover
		{
			CRect rcFrame1(rectMain);
			CRect rcFrame2(rectMain);
			rcFrame1.bottom = rcFrame1.bottom - (rcFrame1.Height() * 3 / 5);
			rcFrame2.top = rcFrame1.bottom;

			pDC.FillSolidRect(&rcFrame1, UpCOLOR1);
			pDC.FillSolidRect(&rcFrame2, DownCOLOR1);
// 2015.07.23 Ver1.00 [Modify Style] Start
			pDC.DrawEdge(&rectMain, EDGE_RAISED, BF_RECT);
// 2015.07.23 Ver1.00 [Modify Style] End
		}
		else											// Normal
		{		
// 2015.07.23 Ver1.00 [Modify Style] Start
//			pDC.FillSolidRect(&rectMain, oldBkColor);
			pDC.FillSolidRect(&rectMain, RGB(230, 233,241));
			pDC.DrawEdge(&rectMain, EDGE_RAISED, BF_RECT);
// 2015.07.23 Ver1.00 [Modify Style] End
			
		}	
	}

	// Draw
	HDITEM item = {0};
	item.mask = HDI_LPARAM;
	GetItem(nItem, &item);
	
	HEADDITEM* headitem = (PHEADDITEM)item.lParam;
	if (headitem->bCheckBox)
	{
		if (headitem->bCheckStatus)
			pDC.DrawFrameControl(::GetCenterRect(rect, CHECKBOX_SIZE, CHECKBOX_SIZE), DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
		else
			pDC.DrawFrameControl(::GetCenterRect(rect, CHECKBOX_SIZE, CHECKBOX_SIZE), DFC_BUTTON, DFCS_BUTTONCHECK);
	}
	else
	{
		UINT nAlign = DT_VCENTER | DT_SINGLELINE;
		switch (headitem->fmt)
		{
		case LVCFMT_LEFT:
			nAlign |= DT_LEFT;	break;
		case LVCFMT_CENTER:
			nAlign |= DT_CENTER;	break;
		case LVCFMT_RIGHT:
			nAlign |= DT_RIGHT;	break;
		case LVCFMT_JUSTIFYMASK:
			nAlign |= DT_LEFT;	break;
		default:
			nAlign |= DT_LEFT;	break;
		}
		int nMode = pDC.SetBkMode(TRANSPARENT);
		rect.DeflateRect(4, 0);
		pDC.DrawText(headitem->szText, _tcslen(headitem->szText), rect, nAlign);
		pDC.SetBkMode(nMode);
	}

	pDC.SetTextColor(oldTextColor);
	pDC.SetBkColor(oldBkColor);
	pDC.Detach();
}

BOOL CHeaderCtrlExt::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSELEAVE)
	{
		m_bHover.bHover = false; 

		if (m_bHover.nItem != -1)
			Invalidate();
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		HDHITTESTINFO info;
		info.pt = pMsg->pt;
		ScreenToClient(&info.pt);
		info.flags = 0;
		HitTest(&info);
		m_bHover.nItem = info.iItem;
		m_bHover.bHover = true;
		
		if (m_bHover.nItem != -1)
			Invalidate();
	}

	return CHeaderCtrl::PreTranslateMessage(pMsg);
}
