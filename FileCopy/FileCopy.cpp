
// FileCopy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "FileCopy.h"
#include "MainFrm.h"
#include "DlgProcSelect.h"
#include "FileCopyDoc.h"
#include "FileCopyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
// CFileCopyApp
BEGIN_MESSAGE_MAP(CFileCopyApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFileCopyApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_PROSELECT, &CFileCopyApp::OnFileProcsel)
END_MESSAGE_MAP()

// CFileCopyApp construction
CFileCopyApp::CFileCopyApp()
{
// 2015.07.18 Ver1.00[International] Start
	g_language = GetLanguage();
	switch(g_language)
	{
	case 0:
		::SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));	break;
	case 1:
		::SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));	break;
	case 2:
		::SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_DEFAULT));	break;
	default:
		::SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));	break;
	}
// 2015.07.18 Ver1.00[International] End
	m_bHiColorIcons = TRUE;
	
	SetAppID(_T("FileCopy.AppID.NoVersion"));
	m_nTabPage = -1;
}
// The one and only CFileCopyApp object
CFileCopyApp theApp;
// CFileCopyApp initialization
BOOL CFileCopyApp::InitInstance()
{
	CWinAppEx::InitInstance();
	
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFileCopyDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CFileCopyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
	OnFileProcsel();

	return TRUE;
}

//---------------------------------------------------------------------------//
// 関数名：	「処理選択」ダイアログボックス処理
// 引数：	なし
// 説明：	「処理選択」ダイアログボックスからの戻り値により画面の切替処理を行う．
// 戻り値：	なし
//---------------------------------------------------------------------------//
void CFileCopyApp::OnFileProcsel() 
{
	CDlgProcSelect dlgSelProc;
	int nReturn = dlgSelProc.DoModal();
	BOOL bFirst = m_nTabPage == -1;

	if (nReturn == IDOK)
	{
		m_nTabPage = dlgSelProc.GetTabPage();
		if (m_nTabPage == -1)
		{
			m_pMainWnd->SendMessage(WM_CLOSE);
			return;
		}
		// MainFrame
		CMainFrame* mainframe = (CMainFrame*)m_pMainWnd;
		if (mainframe)
		{
			mainframe->SwitchToView(m_nTabPage, bFirst);
			// View
			CFileCopyView* view = (CFileCopyView*) mainframe->GetActiveView();
			if (view)
			{
				view->SwitchToView(m_nTabPage, bFirst);
			}
		}
	}
	else					// 終了
	{
		m_pMainWnd->SendMessage(WM_CLOSE);
	}
}

int CFileCopyApp::ExitInstance()
{
// 2015.07.23 Ver1.00 [Modify Style] Start
	CleanState(NULL);
// 2015.07.23 Ver1.00 [Modify Style] End

	return CWinAppEx::ExitInstance();
}
// CFileCopyApp message handlers
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnClose(){	OnOK();	}
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
void CFileCopyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
// CFileCopyApp customization load/save methods
void CFileCopyApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}
void CFileCopyApp::LoadCustomState()
{
}
void CFileCopyApp::SaveCustomState()
{
}