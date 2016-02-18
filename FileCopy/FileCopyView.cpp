#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "FileCopy.h"
#endif

#include "FileCopyDoc.h"
#include "FileCopyView.h"
#include "MainFrm.h"
#include "SettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
// CFileCopyView
IMPLEMENT_DYNCREATE(CFileCopyView, CFormView)
BEGIN_MESSAGE_MAP(CFileCopyView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_MFCBTN_REFRESH, &CFileCopyView::OnShow)
	ON_COMMAND(ID_FILE_COPY, &CFileCopyView::OnFileCopy)
	ON_COMMAND(ID_SETTING, &CFileCopyView::OnSetting)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CFileCopyView::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECTALL, &CFileCopyView::OnBnClickedButtonUnselctall)
END_MESSAGE_MAP()
// CFileCopyView construction/destruction
CFileCopyView::CFileCopyView()
	: CFormView(CFileCopyView::IDD)
{
	// TODO: add construction code here
	m_strSourcePath = _T("");
	m_strTargetPath = _T("");
	m_nDiffDay = 0;
	m_hThread = NULL;
	m_bInitDialog = FALSE;
	m_AutoRefresh = FALSE;
// 2015.07.21 Ver1.00 [TabPage] Start
	m_nCurTabPage = -1;
// 2015.07.21 Ver1.00 [TabPage] End
}

CFileCopyView::~CFileCopyView()
{
	
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::DoDataExchange
// Param    :   CDataExchange* pDX
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CFileCopyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_SourceListCtrl);
// 2015.07.08 Ver1.00 [Delete m_DiffdayCmb]Start
//	DDX_Control(pDX, IDC_COMBO_DIFFDAY, m_DiffdayCmb);
// 2015.07.08 Ver1.00 [Delete m_DiffdayCmb]End
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::PreCreateWindow
// Param    :   CREATESTRUCT& cs
// Return   :   BOOL
// Comment  :   
// -----------------------------------------------------------//
BOOL CFileCopyView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnGetIniSetting
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CFileCopyView::OnGetIniSetting()
{
	// 1-開発環境
	m_strSourcePath = GetSourceRootString(m_nCurTabPage);
	// 2-評価環境
	m_strTargetPath = GetTargetRootString(m_nCurTabPage);
	// 3-フィルタ規則
//	m_strFilter = GetFilter();
	m_strFilterArr.RemoveAll();
	GetFilter(m_nCurTabPage, m_strFilterArr);
	// 4-ファイル最新日付
	m_nDiffDay = GetDiffDay(m_nCurTabPage);
	// 5-プログラムの起動時の自動検索
	m_AutoRefresh = GetAutoRefresh(m_nCurTabPage);
}

void CFileCopyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	// OnInitFormView
	OnInitFormView();
	
	m_bInitDialog = TRUE;
}
// -----------------------------------------------------------//
// Function :   CFileCopyView::OnInitFormView
// Return   :   long
// Comment  :   
// -----------------------------------------------------------//
long CFileCopyView::OnInitFormView()
{
	// 1-FileList
	OnInitListCtrl();
	// 2:Control Position
	OnInitControlPos();
	// 3:Progress Dialog
	m_progressDlgExt.Create(IDD_DIALOG_PROGRESS, this);
	
	return 0;
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnInitListCtrl
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CFileCopyView::OnInitListCtrl()
{
	m_SourceListCtrl.OnSetColumns();
	m_SourceListCtrl.ModifyStyle(0, LVS_OWNERDRAWFIXED);
	m_SourceListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_UNDERLINEHOT);
	m_SourceListCtrl.SetHeaderHeight(25);
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnInitControlPos
// Return   :   long
// Comment  :   
// -----------------------------------------------------------//
void CFileCopyView::OnInitControlPos(int width /*= -1*/, int height/* = -1*/)
{
	if (width == -1 && height == -1)
	{
		CRect rect;
		GetClientRect(&rect);
		m_width = rect.Width();
		m_height = rect.Height();
	}
	else
	{
		m_width = width;
		m_height = height;
	}
	// Source
	SCROLLBARINFO info = { 0 };
	GetScrollBarInfo(OBJID_VSCROLL, &info);
	if (info.rgstate[0] & STATE_SYSTEM_INVISIBLE)
	{
		m_SourceListCtrl.MoveWindow(0, 0, m_width - 30, m_height);
	}
	else
	{
		m_SourceListCtrl.MoveWindow(0, 0, m_width, m_height);
	}

	CRect rectSource;
	m_SourceListCtrl.GetClientRect(&rectSource);
	m_SourceListCtrl.SetColumnWidth(COLUMN_CHECKBOX, rectSource.Width() * 1 / 20 < COLUMN_CHECKBOX_MINSIZE ? COLUMN_CHECKBOX_MINSIZE : rectSource.Width() * 1 / 20);
	m_SourceListCtrl.SetColumnWidth(COLUMN_NAME, rectSource.Width() * 14 / 20);
	m_SourceListCtrl.SetColumnWidth(COLUMN_SIZE, rectSource.Width() * 2 / 20);
	m_SourceListCtrl.SetColumnWidth(COLUMN_MODIFIED, rectSource.Width() * 3 / 20);
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnShow
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CFileCopyView::OnShow()
{
	if (m_bInitDialog && m_strSourcePath.IsEmpty())
	{
		m_SourceListCtrl.DeleteAllItems();
// 2015.07.18 Ver1.00[International] Start
//		MessageBox(_T(" 開発環境を設定してください。 "), _T(" 刷新ファイル "));
		MessageBox(DllLoadIDResource(IDS_MSGInfo_RefreshFile), DllLoadIDResource(IDS_MSGTitle_Release));
// 2015.07.18 Ver1.00[International] End
		return;
	}
// 2015.07.08 Ver1.00 [Delete m_DiffdayCmb]Start
//	int nIndex = m_DiffdayCmb.GetCurSel();
	CMainFrame* mainFrame = (CMainFrame*)AfxGetMainWnd();
	if (mainFrame == NULL)
		return;
	int nIndex = mainFrame->GetComBoxCurSel();
// 2015.07.08 Ver1.00 [Delete m_DiffdayCmb]End
	switch (nIndex)
	{
	case 0:
		m_nDiffDay = DIFFOneDay;	break;
	case 1:
		m_nDiffDay = DIFFOneWeek;	break;
	case 2:
		m_nDiffDay = DIFFOneMonth;	break;
	case 3:
		m_nDiffDay = DIFFAll;	break;
	default:
		m_nDiffDay = DIFFOneDay;	break;
	}

	::SetDiffDay(m_nCurTabPage, m_nDiffDay);

	m_SourceListCtrl.SetRootPath(m_strSourcePath);	// 開発環境
	m_SourceListCtrl.SetDiffDay(m_nDiffDay);		// ファイル最新日付
	m_SourceListCtrl.SetFilter(m_strFilterArr);		// フィルタ規則

	m_SourceListCtrl.DisplayFiles();				// Show
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnBnClickedButtonSelectall
// Return   :   void
// Comment  :   OnBnClickedButtonSelectall
// -----------------------------------------------------------//
void CFileCopyView::OnBnClickedButtonSelectall()
{
	m_SourceListCtrl.SetAllCheck();
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnBnClickedButtonUnselctall
// Return   :   void
// Comment  :   OnBnClickedButtonUnselctall
// -----------------------------------------------------------//
void CFileCopyView::OnBnClickedButtonUnselctall()
{
	m_SourceListCtrl.SetAllUnCheck();
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnRButtonUp
// Param    :   UINT /* nFlags */
//              CPoint point
// Return   :   void
// Comment  :   OnRButtonUp
// -----------------------------------------------------------//
void CFileCopyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnContextMenu
// Param    :   CWnd* /* pWnd */
//              CPoint point
// Return   :   void
// Comment  :   OnContextMenu
// -----------------------------------------------------------//
void CFileCopyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFileCopyView diagnostics

#ifdef _DEBUG
void CFileCopyView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFileCopyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFileCopyDoc* CFileCopyView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileCopyDoc)));
	return (CFileCopyDoc*)m_pDocument;
}
#endif //_DEBUG
// CFileCopyView message handlers
#define COPY_Status_OK			0
#define COPY_Status_Aborted		1
#define COPY_Status_Error		2
// -----------------------------------------------------------//
// Function :   CFileCopyView::CopyThread_
// Param    :   LPVOID lparam
// Return   :   DWORDWINAPI 
// Comment  :   CopyThread_
// -----------------------------------------------------------//
DWORD WINAPI CFileCopyView::CopyThread_(LPVOID lparam)
{
	CFileCopyView* _this = (CFileCopyView*)lparam;
	if (_this)
	{
		return _this->CopyThread();
	}

	return 0;
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::CopyThread
// Return   :   UINT
// Comment  :   CopyThread Function
// -----------------------------------------------------------//
UINT CFileCopyView::CopyThread()
{
// 2015.07.18 Ver1.00[International] Start
	// 线程中国际化出现问题
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
	// 1:Check
	if (m_strTargetPath.IsEmpty())
	{
// 2015.07.18 Ver1.00[International] Start
//		MessageBox(_T(" 評価環境を設定してください。 "), _T(" リリース "));
		MessageBox(DllLoadIDResource(IDS_MSGInfo_Release), DllLoadIDResource(IDS_MSGTitle_Release));
// 2015.07.18 Ver1.00[International] End
		return 0;
	}
	int nCount = m_SourceListCtrl.GetCheckCount();
	if (nCount == 0)
	{
		return 0;
	}

	// 2:Begin
	m_progressDlgExt.Start();

	HRESULT hr = S_OK;
	LVITEM lvItem = { 0 };
	CString strFilePath = _T("");
	LPAFX_SHELLITEMINFO2 pItem = NULL;
	int nResult = COPY_Status_OK;
	int nFileOK = 0;

	::CoInitialize(NULL);
	// 2.1:リリース
	for (int nIndex = 0; nIndex < m_SourceListCtrl.GetItemCount(); nIndex++)
	{
		if (m_SourceListCtrl.GetCheck(nIndex) == FALSE)
			continue;
		// init
		memset(&lvItem, 0x00, sizeof(LVITEM));
		strFilePath = _T("");
		pItem = NULL;
		// GetItem
		lvItem.iItem = nIndex;
		lvItem.mask = LVIF_PARAM;
		m_SourceListCtrl.GetItem(&lvItem);

		pItem = (LPAFX_SHELLITEMINFO2)lvItem.lParam;
		if (pItem == NULL)
		continue;

		strFilePath = SHGetFilePath(pItem->pParentFolder, pItem->pidlRel);
		if (strFilePath.IsEmpty())
		continue;

		// Copy
		CString strDestPath = m_strTargetPath + pItem->szFQPath;

		IFileOperation* fileoperation = NULL;
		hr = ::CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_IFileOperation, (void**)&fileoperation);
		if (hr == S_OK && fileoperation)
		{
			if (COPYFILE)
				fileoperation->SetOperationFlags(FOF_SIMPLEPROGRESS | FOF_NOCONFIRMMKDIR);
			else
				fileoperation->SetOperationFlags(FOF_NO_UI);
			fileoperation->SetOwnerWindow(NULL);

			// From
			IShellItem *pItemFrom = NULL;
			IShellItem *pItemTo = NULL;
			BOOL aborted = false;

			// 1.From
			hr = SHCreateItemFromParsingName(strFilePath, NULL, IID_IShellItem, (void**)(&pItemFrom));
			if (hr == S_OK && pItemFrom)
			{
				// 2.To
				hr = SHCreateItemFromParsingName(strDestPath, NULL, IID_IShellItem, (void**)(&pItemTo));

				// Check Folder
				if (hr != S_OK || pItemTo == NULL)	// Folder not exiset
				{
					hr = SHCreateDirectory(NULL, strDestPath);
					if (hr == S_OK)
					{
						hr = SHCreateItemFromParsingName(strDestPath, NULL, IID_IShellItem, (void**)(&pItemTo));
					}
				}

				if (hr == S_OK && pItemTo)
				{
					// 3.Copy
					hr = fileoperation->CopyItem(pItemFrom, pItemTo, NULL, NULL);
					fileoperation->PerformOperations();
					fileoperation->GetAnyOperationsAborted(&aborted);
					if (aborted)
					{
						nResult = COPY_Status_Aborted;
					}
					else
					{
						nFileOK++;
					}

					pItemTo->Release();
				}
				else
				{
					nResult = COPY_Status_Error;
				}

				pItemFrom->Release();
			}
			else
			{
				nResult = COPY_Status_Error;
			}

			fileoperation->Release();
		}
		else
		{
			nResult = COPY_Status_Error;
		}
	}
	::CoUninitialize();
	m_progressDlgExt.Stop();
	m_hThread = NULL;

	CString strMessage = _T("");

	if (nResult == COPY_Status_OK)
	{
// 2015.07.18 Ver1.00[International] Start
//		strMessage.Format(_T("%d のファイルの合計,%d ファイルのコピーの成功."),
//			nCount, nFileOK);
		strMessage.Format(DllLoadIDResource(IDS_MSGInfo_CopyFiles1), nCount, nFileOK);
// 2015.07.18 Ver1.00[International] End	
		
	}
	else
	{
// 2015.07.18 Ver1.00[International] Start
//		strMessage.Format(_T("%d のファイルの合計,%d ファイルのコピーの成功,%dつのファイルのコピーに失敗しました."),
//			nCount, nFileOK, nCount - nFileOK);
		strMessage.Format(DllLoadIDResource(IDS_MSGInfo_CopyFiles2),	nCount, nFileOK, nCount - nFileOK);
// 2015.07.18 Ver1.00[International] End	
	}

// 2015.07.18 Ver1.00[International] Start
//	MessageBox(strMessage, _T(" リリース "));
	MessageBox(strMessage, DllLoadIDResource(IDS_MSGTitle_Release));
// 2015.07.18 Ver1.00[International] End
	

	return 0;
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnCopy
// Return   :   void
// Comment  :   OnCopy
// -----------------------------------------------------------//
void CFileCopyView::OnCopy()
{
	HANDLE m_hThread = CreateThread(NULL, 0, CopyThread_, this, 0, NULL);
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnFileCopy
// Return   :   void
// Comment  :   OnFileCopy
// -----------------------------------------------------------//
void CFileCopyView::OnFileCopy()
{
	if (m_hThread == NULL)
	{
		OnCopy();
	}
}

// -----------------------------------------------------------//
// Function :   CFileCopyView::OnSetting
// Return   :   void
// Comment  :   OnSetting
// -----------------------------------------------------------//
void CFileCopyView::OnSetting()
{
	CSettingDialog setting;
	setting.OnSetTabPage(m_nCurTabPage);
	setting.DoModal();

	OnGetIniSetting();
}

// 2015.07.21 Ver1.00 [TabPage] Start
void CFileCopyView::SwitchToView(int nTabPage, BOOL bFirst/* = TRUE*/)
{
	m_nCurTabPage = nTabPage;
	// 1:.setting.ini
	OnGetIniSetting();
	// 2:FileList
	m_SourceListCtrl.SetRootPath(m_strSourcePath);
	m_SourceListCtrl.DeleteAllItems();
	// 3:Auto Start
	if (m_AutoRefresh)
	{
		OnShow();
	}
}
// 2015.07.21 Ver1.00 [TabPage] End

// 2015.07.18 Ver1.00[International] Start
void	CFileCopyView::OnInitResourceLanguage()
{
	m_SourceListCtrl.OnInitResourceLanguage();
}
// 2015.07.18 Ver1.00[International] End 