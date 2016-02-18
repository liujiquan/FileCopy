//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   ListCtrlExt.h
//* Author   :   liujiquan
//* DateTime :   7/3/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __LISTCTRLEXT_H_2459134951B94A1895E650CFD52F4215__
#define __LISTCTRLEXT_H_2459134951B94A1895E650CFD52F4215__
#pragma once
#include "Util.h"
// 2015.07.23 Ver1.00 [Progressbar] Start
//#include "ProgressDialog.h"
#include "ProgressBarExt.h"
// 2015.07.23 Ver1.00 [Progressbar] End
#include "HeaderCtrlExt.h"

/************************************************************************/
/* Define                                                               */
/************************************************************************/
#define COLUMN_CHECKBOX_MINSIZE	20
#define COLUMN_NAME_OFFSET	5
#define COLUMN_CHECKBOX	0
#define COLUMN_NAME		1
#define COLUMN_SIZE		2
#define COLUMN_MODIFIED	3
#define COLUMN_MASK		4

#define COLUMN_NAME_CHECKBOX	_T("")
// 2015.07.18 Ver1.00[International] Start
//#define COLUMN_NAME_NAME		_T("名前")
//#define COLUMN_NAME_SIZE		_T(" サイズ ")
//#define COLUMN_NAME_MODIFIED	_T("更新日時")
// 2015.07.18 Ver1.00[International] End

/////////////////////////////////////////////////////////////////
// CListCtrlExt
class CListCtrlExt : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlExt)
		
public:
	CListCtrlExt();
	virtual ~CListCtrlExt();
private:
	int				m_nSortColumn;			// Sort Column
	BOOL				m_bSortAscending;			// Sort Asc/Desc
	CString			m_strRootPath;			// RootPath
	int				m_nDiffDay;				// DiffDay
	BOOL				n_bCreate;
	CStringArray		m_FilterArr;
// 2015.07.23 Ver1.00 [Progressbar] Start
//	CProgressDlgExt	m_progressDlgExt;			// Status
	CProgressBarExt	m_progressDlgExt;			// Status
// 2015.07.23 Ver1.00 [Progressbar] End
	HANDLE			m_hThread;

	CHeaderCtrlExt	m_HeaderCtrl;
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetRootPath(CString strRootPath);			// SetPath
	void DisplayFiles();
	void SetDiffDay(int diffDay);
	void SetFilter(CStringArray& filterArr);
	void Sort(int iColumn, BOOL bAscending);		// Sort
	void SetAllCheck();
	void SetAllUnCheck();
	long GetCheckCount();
	void SetHeaderHeight(int height){	m_HeaderCtrl.SetHeight(height); }
	BOOL BoolAllCheck();
	void SetCheckExt(int nIndex);
private: 
	static DWORD WINAPI DisplayFilesThread_(LPVOID lparam);				// DisplayFiles Thread
	UINT DisplayFilesThread();											// DisplayFiles Function
public:
	void OnSetColumns();												// InserColumn
	static int CALLBACK OnCompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData);		// Sort
	HRESULT EnumSubObjects(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList);	// EnumSubObjects
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);			// DrawItem
	virtual BOOL PreTranslateMessage(MSG* pMsg);							// PreTranslateMessage
	void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);					// OnColumnClick 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);					// OnLButtonDown
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);				// OnLButtonDblClk
	virtual void PreSubclassWindow();

	DWORD SetExtendedStyle(_In_ DWORD dwNewStyle);
	int InsertColumn(int nCol,  const LVCOLUMN* pColumn);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	BOOL DeleteColumn(int nCol);
	void SetHeaderOwnerDraw(BOOL bOwnerDraw = TRUE);

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	void OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
// 2015.07.24 Ver1.00 [File Filter] Start
	BOOL FilterDiffDay(long nDiffDay);
	BOOL FilterFileType(CString strFileName);
// 2015.07.24 Ver1.00 [File Filter] End
// 2015.07.18 Ver1.00[International] Start
	void	OnInitResourceLanguage();
// 2015.07.18 Ver1.00[International] End 
};

#endif//__LISTCTRLEXT_H_2459134951B94A1895E650CFD52F4215__

