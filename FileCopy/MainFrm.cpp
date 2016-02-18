// MainFrm.cpp : implementation of the CMainFrame class
#include "stdafx.h"
#include "FileCopy.h"
#include "Util.h"
#include "MainFrm.h"
#include "FileCopyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString g_TabPageName[TABPAGECOUNT];
// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
// 2015.07.14 Ver1.00 [Delete Customsize] End
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_EXITSIZEMOVE()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolBarInit)
	ON_COMMAND(IDC_COMBOX_DIFFDAY, &CMainFrame::OnClickComboBox)
	ON_CBN_SELCHANGE(IDC_COMBOX_DIFFDAY,&CMainFrame::OnComboxChange)
// 2015.07.23 Ver1.00 [Modify Style] Start
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
// 2015.07.23 Ver1.00 [Modify Style] End
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_KANA, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SELFILE, &CMainFrame::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MAXFILE, &CMainFrame::OnUpdateKeyIndicator)
	ON_WM_TIMER()
	ON_MESSAGE(WM_STATUS, OnSetStatus)
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // info
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	ID_INDICATOR_CAPS,		// Cap Lock
	ID_INDICATOR_NUM,		// Num Lock
	ID_INDICATOR_SCRL,		// Scroll Lock
	ID_INDICATOR_KANA,		// Kana Lock
	ID_INDICATOR_SELFILE,	// file selected
	ID_INDICATOR_MAXFILE,	// file count
	ID_INDICATOR_TIME,		// Time
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
};

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
// 2015.07.23 Ver1.00 [Modify Style] Start
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
// 2015.07.23 Ver1.00 [Modify Style] End	
	m_width = MAX_WID;
	m_height = MAX_HEI;

	m_hFont.CreateFont(
		14,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("SimSun"));			// lpszFacename
	m_DiffdayCmb = NULL;
	m_nDiffDayIndex = -1;
// 2015.07.18 Ver1.00[International] Start
	m_hMenu = NULL;
// 2015.07.18 Ver1.00[International] End
}

CMainFrame::~CMainFrame()
{
	m_hFont.DeleteObject();
	if ( NULL != m_DiffdayCmb )
	{
		delete m_DiffdayCmb;
		m_DiffdayCmb = NULL;
	}
// 2015.07.18 Ver1.00[International] Start
	if (m_hMenu)
	{
		::DeleteObject(m_hMenu);
		m_hMenu = NULL;
	}
// 2015.07.18 Ver1.00[International] End
}

LRESULT CMainFrame::OnToolBarInit(WPARAM wp,LPARAM lp)
{   
	if ( NULL == m_DiffdayCmb )
	{
		m_DiffdayCmb = new CMFCToolBarComboBoxButton(IDC_COMBOX_DIFFDAY, GetCmdMgr()->GetCmdImage(IDC_COMBOX_DIFFDAY, FALSE), CBS_DROPDOWNLIST, 120);
	}
	else
	{
		m_DiffdayCmb->RemoveAllItems();
	}
	m_DiffdayCmb->EnableWindow(TRUE);
	m_DiffdayCmb->SetCenterVert();
	m_DiffdayCmb->SetDropDownHeight(200);
	m_DiffdayCmb->SetFlatMode();
// 2015.07.18 Ver1.00[International] Start
//	m_DiffdayCmb->AddItem(_T("今日"));
//	m_DiffdayCmb->AddItem(_T("1週間以内"));
//	m_DiffdayCmb->AddItem(_T("1か月以内"));
//	m_DiffdayCmb->AddItem(_T("全て"));
	m_DiffdayCmb->AddItem(DllLoadIDResource(IDS_Diffday_Today));
	m_DiffdayCmb->AddItem(DllLoadIDResource(IDS_Diffday_Week));
	m_DiffdayCmb->AddItem(DllLoadIDResource(IDS_Diffday_Month));
	m_DiffdayCmb->AddItem(DllLoadIDResource(IDS_Diffday_All));
// 2015.07.18 Ver1.00[International] End
	m_DiffdayCmb->SelectItem(0);

	int nRet = m_wndToolBar.InsertSeparator(1);
	nRet = m_wndToolBar.InsertButton (*m_DiffdayCmb, 2);
// 2015.07.18 Ver1.00[International] Start
//	m_wndToolBar.SetButtonText(nRet, _T("更新日時"));
	m_wndToolBar.SetButtonText(nRet, DllLoadIDResource(IDS_ModifiedDate));
// 2015.07.18 Ver1.00[International] End

	return 0;
}

// 2015.07.21 Ver1.00 [TabPage] Start
void CMainFrame::SwitchToView(int nTabPage, BOOL bFirst/* = TRUE*/)
{
	// 4-ファイル最新日付
	m_nDiffDay = GetDiffDay(theApp.GetTabPage());
	CMFCToolBarComboBoxButton*	pSourceCombox = m_DiffdayCmb->GetByCmd(IDC_COMBOX_DIFFDAY);
	if (pSourceCombox)
	{
		switch (m_nDiffDay)
		{
		case 0:
		case DIFFOneDay:
			pSourceCombox->SelectItem(0);	m_nDiffDayIndex = 0;		break;
		case DIFFOneWeek:
			pSourceCombox->SelectItem(1);	m_nDiffDayIndex = 1;		break;
		case DIFFOneMonth:
			pSourceCombox->SelectItem(2);	m_nDiffDayIndex = 2;		break;
		case DIFFAll:
			pSourceCombox->SelectItem(3);	m_nDiffDayIndex = 3;		break;
		default:
			pSourceCombox->SelectItem(0);	m_nDiffDayIndex = 0;		break;
		}
	}
// 2015.07.18 Ver1.00[International] Start
//	CString strWindowTitle = _T("リリースツール－") + g_TabPageName[theApp.GetTabPage()];
	CString strWindowTitle = DllLoadIDResource(IDS_Title) + _T("-") + g_TabPageName[theApp.GetTabPage()];
// 2015.07.18 Ver1.00[International] End
	SetWindowText(strWindowTitle);
}
// 2015.07.21 Ver1.00 [TabPage] End

void CMainFrame::OnClickComboBox()
{
}

void CMainFrame::OnComboxChange()
{
}

int CMainFrame::GetComBoxCurSel()
{		
	CMFCToolBarComboBoxButton* pSrcCombo = CMFCToolBarComboBoxButton::GetByCmd(IDC_COMBOX_DIFFDAY, FALSE);
	if (pSrcCombo)
	{
		m_nDiffDayIndex = pSrcCombo->GetCurSel();
	}

	return m_nDiffDayIndex;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->cx = m_width;
	lpCreateStruct->cx = m_height;;

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// set the visual manager used to draw all user interface elements
// 2015.07.23 Ver1.00 [Modify Style] Start
	OnApplicationLook(theApp.m_nAppLook);
// 2015.07.23 Ver1.00 [Modify Style] End
//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.SetFont(&m_hFont);
	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SEARCH))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	CString strCustomize;
//	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
//	ASSERT(bNameValid);
//	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
// 2015.07.14 Ver1.00 [Delete Customsize] End

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	SetTimer(TIMER_EVENT, 1000, NULL);
	m_wndStatusBar.SetPaneWidth(STATUS_SELFILE_INDEX, 60);
	m_wndStatusBar.SetPaneWidth(STATUS_MAXFILE_INDEX, 60);
	m_wndStatusBar.SetPaneWidth(STATUS_TIME_INDEX, 110);
// 2015.07.24 Ver1.00 [CMFCStatusBar] End

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	// Enable toolbar and docking window menu replacement
//	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);
//	// enable quick (Alt+drag) toolbar customization
//	CMFCToolBar::EnableQuickCustomization();
// 2015.07.14 Ver1.00 [Delete Customsize] End
	m_wndMenuBar.SetHeight(25);
	m_wndToolBar.EnableTextLabels(TRUE);
	m_wndToolBar.GetButton(10)->SetVisible(FALSE);

	return 0;
}
// 2015.07.18 Ver1.00[International] Start
void	CMainFrame::OnInitResourceLanguage()
{
	// Title
	CString strCaption = DllLoadIDResource(IDS_Title);
	SetWindowText(strCaption);
	// ToolBar
	m_wndToolBar.RemoveAllButtons();
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SEARCH);
	m_wndToolBar.GetButton(10)->SetVisible(FALSE);

	// MenuBar
	if (m_hMenu)
	{
		::DeleteObject(m_hMenu);
		m_hMenu = NULL;
	}
	m_hMenu = LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_wndMenuBar.CreateFromMenu(m_hMenu, TRUE, TRUE);
	// StatusBar

	RecalcLayout ();
}	
// 2015.07.18 Ver1.00[International] End

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;
// 2015.07.18 Ver1.00[International] Start
//	cs.lpszName = _T(" リリースツール ");
// 2015.07.18 Ver1.00[International] End

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
// 2015.07.14 Ver1.00 [Delete Customsize] Start
//void CMainFrame::OnViewCustomize()
//{
//	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
//	pDlgCust->EnableUserDefinedToolbars();
//	pDlgCust->Create();
//}
// 2015.07.14 Ver1.00 [Delete Customsize] End

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	BOOL bNameValid;
//	CString strCustomize;
//	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
//	ASSERT(bNameValid);
//
//	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
// 2015.07.14 Ver1.00 [Delete Customsize] End

	return lres;

	return 0;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
// 2015.07.14 Ver1.00 [Delete Customsize] Start
//	// enable customization button for all user toolbars
//	BOOL bNameValid;
//	CString strCustomize;
//	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
//	ASSERT(bNameValid);
//
//	for (int i = 0; i < iMaxUserToolbars; i ++)
//	{
//		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
//		if (pUserToolbar != NULL)
//		{
//			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//		}
//	}
// 2015.07.14 Ver1.00 [Delete Customsize] End

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	if ( (m_resizetype & SC_MAXIMIZE) == SC_MAXIMIZE || (m_resizetype & SC_RESTORE) == SC_RESTORE)
	{
		CFileCopyView* pView = (CFileCopyView*)GetActiveView();
		if (pView)
		{
			pView->OnInitControlPos(cx, cy - 50);
		}
	}
	CFrameWndEx::OnSize(nType, cx, cy);
}

void CMainFrame::OnExitSizeMove()
{
	CFileCopyView* pView = (CFileCopyView*)GetActiveView();
	if (pView)
		pView->OnInitControlPos();

//	CFrameWndEx::OnExitSizeMove();
	CFrameWndEx::OnExitSizeMove(0, 0);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	m_resizetype = nID;
	CFrameWndEx::OnSysCommand(nID, lParam);
}
// 2015.07.23 Ver1.00 [Modify Style] Start
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}
// 2015.07.23 Ver1.00 [Modify Style] End
// 2015.07.23 Ver1.00 [Modify Style] Start
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
// 2015.07.23 Ver1.00 [Modify Style] End

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
void CMainFrame::OnUpdateKeyIndicator(CCmdUI* pCmdUI)
{
	ENSURE_ARG(pCmdUI != NULL);
	UINT nVK;
	UINT flag = 0x0001;

	switch (pCmdUI->m_nID)
	{
	case ID_INDICATOR_CAPS:
		nVK = VK_CAPITAL;
		break;

	case ID_INDICATOR_NUM:
		nVK = VK_NUMLOCK;
		break;

	case ID_INDICATOR_SCRL:
		nVK = VK_SCROLL;
		break;

	case ID_INDICATOR_KANA:
		nVK = VK_KANA;
		break;

	case ID_INDICATOR_TIME:
		pCmdUI->Enable();

		return;
	case ID_INDICATOR_SELFILE:
		pCmdUI->Enable();
		return;

	case ID_INDICATOR_MAXFILE:
		pCmdUI->Enable();
		return;

	default:
		TRACE(traceAppMsg, 0, "Warning: OnUpdateKeyIndicator - unknown indicator 0x%04X.\n",
			pCmdUI->m_nID);
		pCmdUI->ContinueRouting();
		return; // not for us
	}

	pCmdUI->Enable(::GetKeyState(nVK) & flag);
	// enable static text based on toggled key state
	ASSERT(pCmdUI->m_bEnableChanged);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (TIMER_EVENT == nIDEvent)
	{
		if (m_wndStatusBar.GetSafeHwnd() && m_wndStatusBar.GetCount() > STATUS_TIME_INDEX)
		{
			m_wndStatusBar.SetPaneText(STATUS_TIME_INDEX, GetCurrentTimeString());
		}
	}
	
	CFrameWndEx::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnSetStatus(WPARAM wparam, LPARAM lparam)
{
	CString strSelFile, strMaxFile;
// 2015.07.18 Ver1.00[International] Start
//	strSelFile.Format(_T(" 選択%d個 "), (int)wparam);
//	strMaxFile.Format(_T(" 合計%d個 "), (int)lparam);
	strSelFile.Format(DllLoadIDResource(IDS_Status_SelFile), (int)wparam);
	strMaxFile.Format(DllLoadIDResource(IDS_Status_MaxFile), (int)lparam);
// 2015.07.18 Ver1.00[International] End

	m_wndStatusBar.SetPaneText(STATUS_SELFILE_INDEX, strSelFile);
	m_wndStatusBar.SetPaneText(STATUS_MAXFILE_INDEX, strMaxFile);

	return 0;
}
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
