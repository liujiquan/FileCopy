
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	HMENU						m_hMenu;
	CMFCMenuBar					m_wndMenuBar;
	CMFCToolBar					m_wndToolBar;
	CMFCToolBarImages			m_UserImages;
	CMFCToolBarComboBoxButton*	m_DiffdayCmb;
	CFont						m_hFont;
	int							m_nDiffDay;
	int							m_nDiffDayIndex;
public:
	CMFCStatusBar				m_wndStatusBar;
public:
	afx_msg LRESULT OnToolBarInit(WPARAM,LPARAM);
	afx_msg void OnClickComboBox();
	afx_msg void OnComboxChange();
	int		GetComBoxCurSel();
// 2015.07.21 Ver1.00 [TabPage] Start
	void	SwitchToView(int nTabPage, BOOL bFirst = TRUE);
// 2015.07.21 Ver1.00 [TabPage] End
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	afx_msg void OnViewCustomize();
// 2015.07.14 Ver1.00 [Delete Customsize] End
// 2015.07.23 Ver1.00 [Modify Style] Start
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
// 2015.07.23 Ver1.00 [Modify Style] End
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnExitSizeMove();
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnSetStatus(WPARAM wparam, LPARAM lparam);
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
	DECLARE_MESSAGE_MAP()
private:
	int		m_width;
	int		m_height;
	UINT	m_resizetype;
public:
// 2015.07.18 Ver1.00[International] Start
	void			OnInitResourceLanguage();
// 2015.07.18 Ver1.00[International] End 
};


