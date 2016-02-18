//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   DialogSetting.cpp
//* Author   :   liujiquan
//* DateTime :   7/21/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "SettingDialog.h"
#include "afxdialogex.h"
// 2015.07.18 Ver1.00[International] Start
#include "MainFrm.h"
#include "FileCopyView.h"
// 2015.07.18 Ver1.00[International] End

// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
extern CString g_TabPageName[TABPAGECOUNT];
IMPLEMENT_DYNAMIC(CSettingDialog, CDialogEx)
CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/) : CDialogEx(CSettingDialog::IDD, pParent)
{
	m_nTabPage = -1;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSettingDialog::CSettingDialog(int nTabPage, CWnd* pParent /*=NULL*/) : CDialogEx(CSettingDialog::IDD, pParent)
{
	m_nTabPage = nTabPage;
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_TabCtrl);
// 2015.07.18 Ver1.00[International] Start
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_LanaguageCmb);
// 2015.07.18 Ver1.00[International] End	
}
BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CSettingDialog::OnTcnSelchangeTabPage)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()
// CSettingDialog 
BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_nTabPage == -1)
		return FALSE;
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CRect rectTab;
	m_TabCtrl.GetClientRect(&rectTab); 
	rectTab.top += 25;
	rectTab.bottom -= 3;
	rectTab.left += 3;
	rectTab.right -= 3;

	for(int index = 0; index < TABPAGECOUNT; index++)
	{
		m_pPage[index].OnSetTabPage(index);
		m_pPage[index].Create(IDD_DIALOG_SETTING_PAGE, GetDlgItem(IDC_TAB_PAGE));

		m_TabCtrl.InsertItem(index, g_TabPageName[index]);
		m_pPage[index].MoveWindow(&rectTab);
	}
	m_TabCtrl.SetCurSel(m_nTabPage);
	m_pPage[m_nTabPage].ShowWindow(TRUE);

// 2015.07.18 Ver1.00[International] Start
	// 5-language
	m_LanaguageCmb.AddString(DllLoadIDResource(IDS_Language_en));
	m_LanaguageCmb.AddString(DllLoadIDResource(IDS_Language_cn));
	m_LanaguageCmb.AddString(DllLoadIDResource(IDS_Language_jp));
	m_LanaguageCmb.SetCurSel(g_language);
	// 6-OnInitResourceLanguage
	m_nLastLanaguage = g_language;
// 2015.07.18 Ver1.00[International] End

	return TRUE;
}

void CSettingDialog::OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCursel = m_TabCtrl.GetCurSel();
	for(int index = 0; index < TABPAGECOUNT; index++)
	{
		m_pPage[index].ShowWindow(nCursel == index);
	}

	*pResult = 0;
}

void CSettingDialog::OnBnClickedOk()
{
	for(int index = 0; index < TABPAGECOUNT; index++)
	{
		m_pPage[index].OnAplly();
	}
// 2015.07.18 Ver1.00[International] Start
	// 5-language
	CString strLanguage = _T("");
	int nlanguage = m_LanaguageCmb.GetCurSel();
	if (nlanguage != m_nLastLanaguage)
	{
		m_nLastLanaguage = nlanguage;
		m_LanaguageCmb.GetLBText(nlanguage, strLanguage);
		SetLanguage(strLanguage);
//		MessageBox(DllLoadIDResource(IDS_MSGLanguage), DllLoadIDResource(IDS_Title));
		// Switch Language
		switch(nlanguage)
		{
		case 0:
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));	break;
		case 1:
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));	break;
		case 2:
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_DEFAULT));	break;
		default:
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));	break;
		}
		// MainFrame
		CMainFrame* mainframe = (CMainFrame*)AfxGetMainWnd();
		if (mainframe)
		{
			mainframe->OnInitResourceLanguage();
			// View
			CFileCopyView* view = (CFileCopyView*)mainframe->GetActiveView();
			if (view)
			{
				view->OnInitResourceLanguage();
			}
		}

		// Lanaguage
		g_language = m_nLastLanaguage;
	}
	
// 2015.07.18 Ver1.00[International] End
	CDialogEx::OnOK();
}
