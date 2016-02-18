#include "stdafx.h"
#include "FileCopy.h"
#include "DlgProcSelect.h"
#include "afxdialogex.h"
#include "Util.h"

// CDlgProcSelect 
// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
extern CString g_TabPageName[TABPAGECOUNT];
IMPLEMENT_DYNAMIC(CDlgProcSelect, CDialogEx)
CDlgProcSelect::CDlgProcSelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProcSelect::IDD, pParent)
{
	m_nTabPage = -1;
}

CDlgProcSelect::~CDlgProcSelect()
{
}

void CDlgProcSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_RootList);
}

BEGIN_MESSAGE_MAP(CDlgProcSelect, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgProcSelect::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST1, &CDlgProcSelect::OnLbnDblclkList1)
END_MESSAGE_MAP()
// CDlgProcSelect
BOOL CDlgProcSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(int i = 0; i < TABPAGECOUNT; i++)
	{
		m_RootList.AddString(g_TabPageName[i]);
	}
	if (m_RootList.GetCount())
	{
		m_RootList.SetCurSel(0);
	}
// 2015.07.18 Ver1.00[International] Start
	OnInitResourceLanguage();
// 2015.07.18 Ver1.00[International] End

	return TRUE;
}

// 2015.07.18 Ver1.00[International] Start
void	CDlgProcSelect::OnInitResourceLanguage()
{
	// OK
	GetDlgItem(IDOK)->SetWindowText(DllLoadIDResource(IDS_BTN_Sel));
	// Close
	GetDlgItem(IDCANCEL)->SetWindowText(DllLoadIDResource(IDS_BTN_Close));
	// Select
	GetDlgItem(IDC_STATIC)->SetWindowText(DllLoadIDResource(IDS_Menu_File_Sel));
	// Title
	SetWindowText(DllLoadIDResource(IDS_Menu_File_Sel));
}
// 2015.07.18 Ver1.00[International] End

void CDlgProcSelect::OnBnClickedOk()
{
	m_nTabPage = m_RootList.GetCurSel();

	CDialogEx::OnOK();
}


void CDlgProcSelect::OnLbnDblclkList1()
{
	m_nTabPage = m_RootList.GetCurSel();

	CDialogEx::OnOK();
}
