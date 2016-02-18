#pragma once
#include "resource.h"
#include "FileCopyDoc.h"
#include "afxwin.h"
#include "ListCtrlExt.h"

class CFileCopyView : public CFormView
{
protected: // create from serialization only
	CFileCopyView();
	DECLARE_DYNCREATE(CFileCopyView)

public:
	enum{ IDD = IDD_FILECOPY_FORM };

// Attributes
public:
	CFileCopyDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CFileCopyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	BOOL				m_bInitDialog;
	int					m_width;				// width
	int					m_height;				// height
	CString				m_strSourcePath;		// 開発環?E
	CString				m_strTargetPath;		// 評価環?E
	CStringArray		m_strFilterArr;			// フィルタ規則
	int					m_nDiffDay;				// ファイル最新日
	BOOL				m_AutoRefresh;			// プログラムの起動時の自動検索
		
	CListCtrlExt		m_SourceListCtrl;		// File List
// 2015.07.23 Ver1.00 [Progressbar] Start
//	CProgressDlgExt		m_progressDlgExt;		// Status
	CProgressBarExt		m_progressDlgExt;		// Status
// 2015.07.23 Ver1.00 [Progressbar] End
	HANDLE				m_hThread;
// 2015.07.21 Ver1.00 [TabPage] Start
	int					m_nCurTabPage;			// Page Index
// 2015.07.21 Ver1.00 [TabPage] End
public:
	void OnInitListCtrl();
	void OnInitControlPos(int width = -1, int height = -1);
	long OnInitFormView();
	void OnGetIniSetting();
// 2015.07.21 Ver1.00 [TabPage] Start
	void SwitchToView(int nTabPage, BOOL bFirst = TRUE);
// 2015.07.21 Ver1.00 [TabPage] End
// 2015.07.18 Ver1.00[International] Start
	void	OnInitResourceLanguage();
// 2015.07.18 Ver1.00[International] End 
public:
	static DWORD WINAPI CopyThread_(LPVOID lparam);	// CopyThread_ Thread
	UINT CopyThread();								// CopyThread Function
	void OnCopy();
	afx_msg void OnShow();
	afx_msg void OnFileCopy();
	afx_msg void OnSetting();
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonUnselctall();
};

#ifndef _DEBUG  // debug version in FileCopyView.cpp
inline CFileCopyDoc* CFileCopyView::GetDocument() const
   { return reinterpret_cast<CFileCopyDoc*>(m_pDocument); }
#endif

