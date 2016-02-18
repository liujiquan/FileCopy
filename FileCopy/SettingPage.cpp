//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   DialogSetting.cpp
//* Author   :   liujiquan
//* DateTime :   7/9/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "SettingPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
// CSettingPage dialog
IMPLEMENT_DYNAMIC(CSettingPage, CDialogEx)

CSettingPage::CSettingPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingPage::IDD, pParent)
{
	m_strSourcePath = _T("");		// 開発環境
	m_strTargetPath = _T("");		// 評価環境
	m_AutoRefresh = FALSE;			// プログラムの起動時の自動検索
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTabPage = -1;
}

CSettingPage::CSettingPage(int nTabPage, CWnd* pParent/* = NULL*/)
	: CDialogEx(CSettingPage::IDD, pParent)
{
	m_strSourcePath = _T("");		// 開発環境
	m_strTargetPath = _T("");		// 評価環境
	m_AutoRefresh = FALSE;			// プログラムの起動時の自動検索
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTabPage = nTabPage;
}

CSettingPage::~CSettingPage()
{
}

void CSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURCEPATH, m_strSourcePath);
	DDX_Text(pDX, IDC_EDIT_TARGETPATH, m_strTargetPath);
	DDX_Check(pDX, IDC_CHECK_AUDOREFRESH, m_AutoRefresh);
	DDX_Control(pDX, IDC_LIST_Filter, m_Filterlist);
}

BEGIN_MESSAGE_MAP(CSettingPage, CDialogEx)
	ON_BN_CLICKED(ID_OPEN_SOURCE, &CSettingPage::OnBnClickedOpenSource)
	ON_BN_CLICKED(ID_OPEN_TARGET, &CSettingPage::OnBnClickedOpenTarget)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_AddFilter, &CSettingPage::OnBnClickedBtnAddfilter)
	ON_BN_CLICKED(IDC_BTN_DelFilter, &CSettingPage::OnBnClickedBtnDelfilter)
	ON_BN_CLICKED(IDC_BTN_DefaultFilter, &CSettingPage::OnBnClickedBtnDefaultFilter)
END_MESSAGE_MAP()
// CSettingPage message handlers
BOOL CSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_nTabPage == -1)
		return FALSE;
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	// 1-開発環境
	m_strSourcePath = GetSourceRootString(m_nTabPage);
	// 2-評価環境
	m_strTargetPath = GetTargetRootString(m_nTabPage);
	// 3-フィルタ規則
	CStringArray strFilterArr;
	GetFilter(m_nTabPage, strFilterArr);
	for(int i = 0; i < strFilterArr.GetSize(); i++)
	{
		CString strType = strFilterArr.GetAt(i);
		CString strInfo = GetFileTypeInfo(strType);
		if (!strType.IsEmpty() && !strInfo.IsEmpty())
		{
			m_Filterlist.AddString(strType, strInfo);
		}
	}
	// 4-default フィルタ規則
	GetDefaultFilter(m_nTabPage, m_strDefaultfilterArr);
	// 5-プログラムの起動時の自動検索
	m_AutoRefresh = GetAutoRefresh(m_nTabPage);

	UpdateData(FALSE);

	return TRUE; 
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedOpenSource
// Return   :   void
// Comment  :   開発環境
// -----------------------------------------------------------//
void CSettingPage::OnBnClickedOpenSource()
{
	if (SHSelectFolder3(m_hWnd, m_strSourcePath))
	{
		UpdateData(FALSE);
	}
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedOpenTarget
// Return   :   void
// Comment  :   評価環境
// -----------------------------------------------------------//
void CSettingPage::OnBnClickedOpenTarget()
{
	if (SHSelectFolder3(m_hWnd, m_strTargetPath))
	{
		UpdateData(FALSE);
	}
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedOk
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CSettingPage::OnAplly()
{
	UpdateData(TRUE);

	// 1-開発環境
	SetSourceRootString(m_nTabPage, m_strSourcePath);
	// 2-評価環境
	SetTargetRootString(m_nTabPage, m_strTargetPath);
	// 3-フィルタ規則
	CString strFilter = _T("");
	int nCount = m_Filterlist.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		strFilter = strFilter + GetFileType(m_Filterlist.GetLBText(i));
		if (i != nCount - 1)
			strFilter = strFilter + SPLIT_CHAR;
	}
	SetFilter(m_nTabPage, strFilter);
	// 4-プログラムの起動時の自動検索
	SetAutoRefresh(m_nTabPage, m_AutoRefresh);
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedBtnAddfilter
// Return   :   void
// Comment  :   add
// -----------------------------------------------------------//
void CSettingPage::OnBnClickedBtnAddfilter()
{
	m_Filterlist.OnAppendString();
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedBtnDelfilter
// Return   :   void
// Comment  :   delete
// -----------------------------------------------------------//
void CSettingPage::OnBnClickedBtnDelfilter()
{
	if (m_Filterlist.GetCurSel() != -1)
	{
		m_Filterlist.DeleteString(m_Filterlist.GetCurSel());
	}
}

// -----------------------------------------------------------//
// Function :   CSettingPage::OnBnClickedBtnDefaultFilter
// Return   :   void
// Comment  :   default filter
// -----------------------------------------------------------//
void CSettingPage::OnBnClickedBtnDefaultFilter()
{
	// 1-clear
	int nCount = m_Filterlist.GetCount();
	for(int i = nCount - 1; i >= 0; i--)
	{
		m_Filterlist.DeleteString(i);
	}
	// 2-reset default
	for(int i = 0; i < m_strDefaultfilterArr.GetSize(); i++)
	{
		CString strType = m_strDefaultfilterArr.GetAt(i);
		CString strInfo = GetFileTypeInfo(strType);
		if (!strType.IsEmpty() && !strInfo.IsEmpty())
		{
			m_Filterlist.AddString(strType, strInfo);
		}
	}
}
