//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   MFCShellCListCtrlExt.cpp
//* Author   :   liujiquan
//* DateTime :   7/3/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "FileCopy.h"
#include "ListCtrlExt.h"
#include "FileCopyView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2015.07.18 Ver1.00[International] Start
extern int g_language;
// 2015.07.18 Ver1.00[International] End
extern COLORREF g_BKgroupColor[2][5];
extern COLORREF g_TextgroupColor[2][5];
// CListCtrlExt
IMPLEMENT_DYNAMIC(CListCtrlExt, CListCtrl)
CListCtrlExt::CListCtrlExt()
{
	m_strRootPath = _T("");
	m_nDiffDay = 0;
	n_bCreate = FALSE;
	m_hThread = NULL;
}

CListCtrlExt::~CListCtrlExt()
{
}

BEGIN_MESSAGE_MAP(CListCtrlExt, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
void CListCtrlExt::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	if (pHeaderCtrl)
	{
		m_HeaderCtrl.SubclassWindow(pHeaderCtrl->GetSafeHwnd());
		
		m_HeaderCtrl.SetFont((HFONT)GetFont()->GetSafeHandle());
	}
}

DWORD CListCtrlExt::SetExtendedStyle(_In_ DWORD dwNewStyle)
{
	if (dwNewStyle & LVS_EX_CHECKBOXES)
	{
		HDITEM item = { 0 };

		item.mask = HDI_LPARAM;
		m_HeaderCtrl.GetItem(COLUMN_CHECKBOX, &item);
		HEADDITEM* param = (HEADDITEM*)item.lParam;
		if (param)
		{
			param->bCheckBox = true;
		}
	}

	return CListCtrl::SetExtendedStyle(dwNewStyle);
}

int CListCtrlExt::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
	if (nCol == -1 || pColumn == NULL)
		return -1;

	int nRet =  CListCtrl::InsertColumn(nCol, pColumn);

	// HEADDITEM
	HEADDITEM* pParam = (PHEADDITEM)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(HEADDITEM));
	pParam->nCol = nCol;
	pParam->fmt = pColumn->fmt;
	memcpy(pParam->szText, pColumn->pszText, _tcslen(pColumn->pszText) > MAX_PATH ? MAX_PATH * sizeof(TCHAR) : _tcslen(pColumn->pszText)* sizeof(TCHAR));
	
	// HDITEM
	HDITEM item = { 0 };
	item.mask = HDI_LPARAM;
	item.lParam = (LPARAM)pParam;
	m_HeaderCtrl.SetItem(nCol, &item);

	return nRet;
}

int CListCtrlExt::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat/* = LVCFMT_LEFT*/, 
	int nWidth/* = -1*/, int nSubItem/* = -1*/)
{
	if (nCol == -1 || lpszColumnHeading == NULL)
		return -1;

	int nRet = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);

	// HEADDITEM
	HEADDITEM* pParam = (PHEADDITEM)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(HEADDITEM));
	pParam->nCol = nCol;
	pParam->fmt = nFormat;
	memcpy(pParam->szText, lpszColumnHeading, _tcslen(lpszColumnHeading) > MAX_PATH ? MAX_PATH * sizeof(TCHAR) : _tcslen(lpszColumnHeading)* sizeof(TCHAR));

	// HDITEM
	HDITEM item = { 0 };
	item.mask = HDI_LPARAM;
	item.lParam = (LPARAM)pParam;
	m_HeaderCtrl.SetItem(nCol, &item);

	return nRet;
}

BOOL CListCtrlExt::DeleteColumn(int nCol)
{
	HDITEM item = { 0 };
	item.mask = HDI_LPARAM;
	m_HeaderCtrl.GetItem(nCol, &item);
	if (item.lParam)
	{
//		GlobalFree((HGLOBAL)item.lParam);
		item.lParam = NULL;
	}
	
	return CListCtrl::DeleteColumn(nCol);
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::OnSetColumns
// Return   :   void
// Comment  :   OnSetColumns
// -----------------------------------------------------------//
void CListCtrlExt::OnSetColumns()
{
	// Delete all columns
	int nColumnCount = GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		DeleteColumn(0);
	}

	CRect rect;
	GetClientRect(&rect);

// 2015.07.18 Ver1.00[International] Start
//	InsertColumn(COLUMN_CHECKBOX, COLUMN_NAME_CHECKBOX, LVCFMT_LEFT, rect.Width() * 1 / 20);
//	InsertColumn(COLUMN_NAME, COLUMN_NAME_NAME, LVCFMT_LEFT, rect.Width() * 14 / 20);
//	InsertColumn(COLUMN_SIZE, COLUMN_NAME_SIZE, LVCFMT_RIGHT, rect.Width() * 2 / 20);
//	InsertColumn(COLUMN_MODIFIED, COLUMN_NAME_MODIFIED, LVCFMT_RIGHT, rect.Width() * 3 / 20);
	InsertColumn(COLUMN_CHECKBOX, COLUMN_NAME_CHECKBOX, LVCFMT_LEFT, rect.Width() * 1 / 20);
	InsertColumn(COLUMN_NAME, DllLoadIDResource(IDS_Listctrl_Name), LVCFMT_LEFT, rect.Width() * 14 / 20);
	InsertColumn(COLUMN_SIZE, DllLoadIDResource(IDS_Listctrl_Size), LVCFMT_RIGHT, rect.Width() * 2 / 20);
	InsertColumn(COLUMN_MODIFIED, DllLoadIDResource(IDS_Listctrl_Date), LVCFMT_RIGHT, rect.Width() * 3 / 20);
// 2015.07.18 Ver1.00[International] End
	
	if (n_bCreate == FALSE)
	{
		// Progress Dialog
		DWORD dwStyle = WS_VISIBLE | WS_THICKFRAME;
		m_progressDlgExt.Create(IDD_DIALOG_PROGRESS, this);
		n_bCreate = TRUE;
	}

	SetHeaderOwnerDraw();
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetRootPath
// Param    :   CString strRootPath
// Return   :   void
// Comment  :   SetRootPath
// -----------------------------------------------------------//
void CListCtrlExt::SetRootPath(CString strRootPath)
{
	m_strRootPath = strRootPath;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetDiffDay
// Param    :   int diffDay
// Return   :   void
// Comment  :   SetDiffDay
// -----------------------------------------------------------//
void CListCtrlExt::SetDiffDay(int diffDay)
{
	m_nDiffDay = diffDay;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetFilter
// Param    :   CStringArray& filterArr
// Return   :   void
// Comment  :   SetFilter
// -----------------------------------------------------------//
void CListCtrlExt::SetFilter(CStringArray& filterArr)
{
	m_FilterArr.RemoveAll();
	m_FilterArr.Append(filterArr);
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::Sort
// Param    :   int iColumn
//			   BOOL bAscending
// Return   :   void
// Comment  :   SetFilter
// -----------------------------------------------------------//
void CListCtrlExt::Sort(int iColumn, BOOL bAscending)
{
	m_nSortColumn = iColumn;
	m_bSortAscending = bAscending;

	SortItems(OnCompareItems, reinterpret_cast<DWORD>(this));
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::BoolAllCheck
// Return   :   void
// Comment  :   BoolAllCheck
// -----------------------------------------------------------//
BOOL CListCtrlExt::BoolAllCheck()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetCheck(i) == FALSE)
			return FALSE;
	}

	return TRUE;
}
// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetAllCheck
// Return   :   void
// Comment  :   SetAllCheck
// -----------------------------------------------------------//
void CListCtrlExt::SetAllCheck()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		SetCheck(i, TRUE);
	}
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetAllUnCheck
// Return   :   void
// Comment  :   SetAllUnCheck
// -----------------------------------------------------------//
void CListCtrlExt::SetAllUnCheck()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		SetCheck(i, FALSE);
	}
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::GetCheckCount
// Return   :   long
// Comment  :   GetCheckCount
// -----------------------------------------------------------//
long CListCtrlExt::GetCheckCount()
{
	int nCount = 0;
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetCheck(i))
		{
			nCount++;
		}
	}

	return nCount;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::DisplayFiles
// Return   :   void
// Comment  :   DisplayFiles
// -----------------------------------------------------------//
void CListCtrlExt::DisplayFiles()
{
	if (n_bCreate == FALSE || m_hThread)
		return;
	HDITEM item = { 0 };
	item.mask = HDI_LPARAM;
	m_HeaderCtrl.GetItem(COLUMN_CHECKBOX, &item);
	HEADDITEM* headitem = (PHEADDITEM)item.lParam;
	if (headitem && headitem->bCheckBox)
	{
		headitem->bCheckStatus = false;
	}

	m_hThread = CreateThread(NULL, 0, DisplayFilesThread_, this, 0, NULL);
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::DisplayFilesThread_
// Param    :   LPVOID lparam
// Return   :   DWORD WINAPI
// Comment  :   OnShow
// -----------------------------------------------------------//
DWORD WINAPI CListCtrlExt::DisplayFilesThread_(LPVOID lparam)
{
	CListCtrlExt* _this = (CListCtrlExt*)lparam;
	if (_this)
	{
		return _this->DisplayFilesThread();
	}
	
	return 0;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::DisplayFilesThread
// Return   :   void
// Comment  :   OnShow
// -----------------------------------------------------------//
UINT CListCtrlExt::DisplayFilesThread()
{
	DeleteAllItems();

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
	if (m_strRootPath.IsEmpty())
	{
		m_hThread = NULL;
		return 0;
	}
	m_progressDlgExt.Start();
	HRESULT hr = S_OK;
	// 0: 
	::CoInitialize(NULL);
	// 1
	IShellFolder* pDesktopShellFolder = NULL;		// Desktop(IShellFolder)
	IShellFolder2* pDesktopShellFolder2 = NULL;		// Desktop(IShellFolder2)

	// 1.1-Desktop(IShellFolder)
	hr = SHGetDesktopFolder(&pDesktopShellFolder);
	// 1.2-Desktop(IShellFolder2)
	hr = pDesktopShellFolder->QueryInterface(IID_IShellFolder2, (void**)&pDesktopShellFolder2);
	
	// 2
	IShellFolder2* pRootShellFolder2 = NULL;			// RootPath(IShellFolder2)
	LPITEMIDLIST pRootItemIDList = NULL;			// RootPath(IDList)
	// 2.1-RootPath(IDList)
	hr = pDesktopShellFolder2->ParseDisplayName(::GetActiveWindow(),
		NULL,
		m_strRootPath.GetBuffer(0),
		NULL,
		&pRootItemIDList,
		NULL);
	// 2.2-RootPath(IShellFolder2)
	hr = pDesktopShellFolder2->BindToObject(pRootItemIDList, NULL, IID_IShellFolder2, (void**)&pRootShellFolder2);
	
	// 3-EnumSubObjects
	hr = EnumSubObjects(pRootShellFolder2, pRootItemIDList);
	
	if (pDesktopShellFolder2)	// Desktop(IShellFolder2)
		pDesktopShellFolder2->Release();
	
	if (pDesktopShellFolder)	// Desktop(IShellFolder)
		pDesktopShellFolder->Release();

	Sort(COLUMN_MODIFIED, FALSE);
	// 5
	::CoUninitialize();
	
	m_progressDlgExt.Stop();

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
	OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End

	m_hThread = NULL;

	return 0;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::EnumSubObjects
// Param    :   LPSHELLFOLDER2 pFolder
//              LPITEMIDLIST pIDList
// Return   :   HRESULT
// Comment  :   EnumSubObjects
// -----------------------------------------------------------//
HRESULT CListCtrlExt::EnumSubObjects(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList)
{
	if (pFolder == NULL || pIDList == NULL)
		return S_FALSE;

	HRESULT hr = S_OK;							// return
	LPENUMIDLIST pEnumIDList = NULL;			// RootPath(EnumIDList)
	LPITEMIDLIST pFileIDList = NULL;			// File(IDList)
	ULONG dwFetched = 0;						// Fetched
	STRRET retFile = {0};						// filename(strref)
	TCHAR szBuffer[MAX_PATH] = { 0 };			// filename(real)
	LPAFX_SHELLITEMINFO2 pItem = NULL;
	CString strFilePath;
	int i = 0;
	CString strFileName = _T("");
	CString strExt = _T("");

	// 3.1-RootPath(EnumIDList)
	hr = pFolder->EnumObjects(::GetActiveWindow(),
		SHCONTF_NONFOLDERS | SHCONTF_FOLDERS,
		&pEnumIDList);
	// 3.2-File(IDList)
	hr = pEnumIDList->Next(1, &pFileIDList, &dwFetched);
	while (hr == S_OK && dwFetched)
	{
		DWORD dwAttr = SFGAO_DISPLAYATTRMASK;
		pFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pFileIDList, &dwAttr);
		
		if ((dwAttr & SFGAO_FOLDER) && dwAttr != 0x60000145)	// 0x60000145  .zip文件
		{
			LPSHELLFOLDER2 pSubFolder = NULL;

			hr = pFolder->BindToObject(pFileIDList, NULL, IID_IShellFolder2, (void**)&pSubFolder);
			EnumSubObjects(pSubFolder, pFileIDList);
			goto NEXT;
		}
//////////////////////////////////////////////////////////////////////////////////////
		// Filter1-ファイル最新日
		long diffDays = SHGetDiffDays(pFolder, pFileIDList);	// now - fileday
		if (FilterDiffDay(diffDays) == FALSE)
		{
			goto NEXT;
		}

		// Filter2-ﾌｧｲﾙ種類
		strFileName = SHGetFileDetail(pFolder, pFileIDList, DETAILS_NAME);
		if (FilterFileType(strFileName) == FALSE)
		{
			goto NEXT;
		}
//////////////////////////////////////////////////////////////////////////////////////
		LVITEM lvItem;
		ZeroMemory(&lvItem, sizeof(lvItem));

		pFolder->AddRef(); 
		// ShellItemInfo
		pItem = (LPAFX_SHELLITEMINFO2)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO2));
		pItem->pidlRel = pFileIDList;
		pItem->pidlFQ = pFileIDList;
//		pItem->pidlFQ = afxShellManager->ConcatenateItem(pIDList, pFileIDList);
		pItem->pParentFolder = pFolder;
		pItem->nDiffday = diffDays;
		// filepath
		strFilePath = SHGetFilePath(pFolder, pFileIDList);
		strFilePath = strFilePath.Right(strFilePath.GetLength() - m_strRootPath.GetLength());
		strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
		memcpy(pItem->szFQPath, strFilePath.GetBuffer(0), strFilePath.GetLength() * sizeof(TCHAR));

		// LvItem
		lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvItem.lParam = (LPARAM)pItem;
		lvItem.pszText = _T("");
			
		lvItem.iImage = SHGetIconIndex(pItem->pidlFQ);

		if (dwAttr & SFGAO_SHARE)
		{
			lvItem.mask |= LVIF_STATE;
			lvItem.stateMask |= LVIS_OVERLAYMASK;
			lvItem.state |= INDEXTOOVERLAYMASK(1); 
		}
		if (dwAttr & SFGAO_GHOSTED)
		{
			lvItem.mask |= LVIF_STATE;
			lvItem.stateMask |= LVIS_CUT;
			lvItem.state |= LVIS_CUT;
		}
		int iItem = InsertItem(&lvItem);
		if (iItem >= 0)
		{
//			SetItemText(iItem, COLUMN_NAME, SHGetFileDetail(pFolder, pFileIDList, DETAILS_NAME));
			SetItemText(iItem, COLUMN_NAME, SHGetFilePath(pFolder, pFileIDList));
			SetItemText(iItem, COLUMN_SIZE, SHGetFileDetail(pFolder, pFileIDList, DETAILS_SIZE));
			SetItemText(iItem, COLUMN_MODIFIED, SHGetFileDetail(pFolder, pFileIDList, DETAILS_MODIFIED));
		}
//////////////////////////////////////////////////////////////////////////////////////
NEXT:	// Next
		dwFetched = 0;
		hr = pEnumIDList->Next(1, &pFileIDList, &dwFetched);
	}

	// Release
	if (pEnumIDList)			// RootPath(EnumIDList)
		pEnumIDList->Release();
	if (pFolder)
		pFolder->Release();

	return S_OK;
}

// 2015.07.24 Ver1.00 [File Filter] Start
BOOL CListCtrlExt::FilterDiffDay(long nDiffDay)
{
	if (DIFFAll == m_nDiffDay)
		return TRUE;

	if (m_nDiffDay > nDiffDay && nDiffDay >= 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CListCtrlExt::FilterFileType(CString strFileName)
{
	if (strFileName.IsEmpty())
	{
		return FALSE;
	}

	CString strFileExt = strFileName.Right(strFileName.GetLength() - strFileName.Find(_T(".")) - 1);
	
	if (strFileExt.IsEmpty())
	{
		return FALSE;
	}

	CString strFilter = _T("");		// *.cpp
	CString strHeader = _T("");		// *.
	CString strExt	= _T("");		// .cpp
	int nHeaderLen = CString(TYPEHEADER).GetLength();

	for (int i = 0; i < m_FilterArr.GetSize(); i++)
	{
		// 0-Check
		strFilter = m_FilterArr.GetAt(i);
		if (strFilter.GetLength() <= nHeaderLen)
			continue; 
		if (strFilter.Left(nHeaderLen) != TYPEHEADER)
			continue;
		// 1-*.*
		if (strFilter == TYPEALL)
		{
			return TRUE;
		}
		// 2-cpp : cpp
		strExt = strFilter.Right(strFilter.GetLength() - nHeaderLen);
		if (strExt == strFileExt)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}
// 2015.07.24 Ver1.00 [File Filter] End

// -----------------------------------------------------------//
// Function :   CListCtrlExt::OnCompareItems
// Param    :   LPARAM lParam1
//              LPARAM lParam2
//              int iColumn
// Return   :   int  
// Comment  :   // Sort-Compare function
// -----------------------------------------------------------//
int CALLBACK  CListCtrlExt::OnCompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData)
{
	CListCtrlExt* _this = reinterpret_cast<CListCtrlExt*>(lParamData);
	ASSERT(_this->IsKindOf(RUNTIME_CLASS(CListCtrl)));
	ASSERT_VALID(_this);

	LPAFX_SHELLITEMINFO2 pItem1 = (LPAFX_SHELLITEMINFO2)lParam1;
	LPAFX_SHELLITEMINFO2 pItem2 = (LPAFX_SHELLITEMINFO2)lParam2;

	ENSURE(pItem1 != NULL);
	ENSURE(pItem2 != NULL);

	SHFILEINFO sfi1;
	SHFILEINFO sfi2;

	TCHAR szPath1[MAX_PATH] = {0};
	TCHAR szPath2[MAX_PATH] = { 0 };

	CFileStatus fs1;
	CFileStatus fs2;

	int nRes = 0;

	switch (_this->m_nSortColumn)
	{
	case COLUMN_NAME:
		{
			HRESULT hr = pItem1->pParentFolder->CompareIDs(0, pItem1->pidlRel, pItem2->pidlRel);
			if (FAILED(hr))
			{
				return 0;
			}
			nRes = (short)SCODE_CODE(GetScode(hr));
		}
		break;
	case COLUMN_SIZE:
		{
			// File1
			CString str1 = SHGetFilePath(pItem1->pParentFolder, pItem1->pidlRel);
			if (str1.IsEmpty() || CFile::GetStatus(str1, fs1) == FALSE)
			{
				nRes = -1; break;
			}
			// File2
			CString str2 = SHGetFilePath(pItem2->pParentFolder, pItem2->pidlRel);
			if (str2.IsEmpty() || CFile::GetStatus(str2, fs2) == FALSE)
			{
				nRes = 1; break;
			}
			// Compare
			if (fs1.m_attribute & CFile::directory)
			{
				nRes = -1;
			}
			else if (fs2.m_attribute & CFile::directory)
			{
				nRes = 1;
			}
			else
			{
				nRes = fs1.m_size < fs2.m_size ? -1 : fs1.m_size > fs2.m_size ? 1 : 0;
			}
		}
		break;
	case COLUMN_MODIFIED:
		{
			// File1
			CString str1 = SHGetFilePath(pItem1->pParentFolder, pItem1->pidlRel);
			if (str1.IsEmpty())
			{
				nRes = 0; break;
			}
			if (CFile::GetStatus(str1, fs1) == FALSE)
			{
				nRes = 0; break;
			}
			// File2
			CString str2 = SHGetFilePath(pItem2->pParentFolder, pItem2->pidlRel);
			if (str2.IsEmpty())
			{
				nRes = 0; break;
			}
			if (CFile::GetStatus(str2, fs2) == FALSE)
			{
				nRes = 0; break;
			}
			nRes = fs1.m_mtime < fs2.m_mtime ? -1 : fs1.m_mtime > fs2.m_mtime ? 1 : 0;
		}
		break;
	}

	return _this->m_bSortAscending ? nRes : -nRes;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::DrawItem
// Param    :   LPDRAWITEMSTRUCT lpDrawItemStruct
// Return   :   void
// Comment  :   DrawItem
// -----------------------------------------------------------//
void CListCtrlExt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType & ODT_LISTBOX);
	CString str;

	CDC cdc;
	cdc.Attach(lpDrawItemStruct->hDC);

	COLORREF oldTextColor = cdc.GetTextColor();
	COLORREF oldBkColor = cdc.GetBkColor();

	// Draw back
	LVITEM lvItem = { 0 };
	lvItem.mask = LVIF_PARAM | LVIF_IMAGE;
	lvItem.iItem = lpDrawItemStruct->itemID;
	GetItem(&lvItem);
	LPAFX_SHELLITEMINFO2 pItem = (LPAFX_SHELLITEMINFO2)lvItem.lParam;

	if ((lpDrawItemStruct->itemAction | ODA_SELECT)	&& 
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS)
			)
		{
			cdc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));	// selected
			cdc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			cdc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));	// selected
			cdc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		}
	}
	else
	{
		if (pItem && pItem->nDiffday <= DIFFOneDay)
		{
			cdc.SetTextColor(DIFFOneDay_TextColor);		// selected
			cdc.SetBkColor(DIFFOneDay_BkColor);
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, DIFFOneDay_BkColor);
		}
		else if (pItem && pItem->nDiffday <= DIFFOneWeek)
		{
			cdc.SetTextColor(DIFFOneWeek_TextColor);	// selected
			cdc.SetBkColor(DIFFOneWeek_BkColor);
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, DIFFOneWeek_BkColor);
		}
		else if (pItem && pItem->nDiffday <= DIFFOneMonth)
		{
			cdc.SetTextColor(DIFFOneMonth_TextColor);	// selected
			cdc.SetBkColor(DIFFOneMonth_BkColor);
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, DIFFOneMonth_BkColor);
		}
		else
		{
			cdc.SetTextColor(DIFFAll_TextColor);		// selected
			cdc.SetBkColor(DIFFALL_BkColor);
			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, DIFFALL_BkColor);
//			cdc.FillSolidRect(&lpDrawItemStruct->rcItem, oldBkColor);
		}
	}
	// Draw Text
	CRect rcColumn(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;
	rcColumn.right = rcColumn.left;

	for (int nCol = 0; nCol < COLUMN_MASK; nCol++)
	{
		rcColumn.left = rcColumn.right;
		rcColumn.right = rcColumn.left + GetColumnWidth(nCol);

		if (nCol == COLUMN_CHECKBOX)
		{
			if (GetCheck(nItem))
				cdc.DrawFrameControl(::GetCenterRect(rcColumn, CHECKBOX_SIZE, CHECKBOX_SIZE), DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
			else
				cdc.DrawFrameControl(::GetCenterRect(rcColumn, CHECKBOX_SIZE, CHECKBOX_SIZE), DFC_BUTTON, DFCS_BUTTONCHECK);
		}
		else
		{
			CString strText = GetItemText(nItem, nCol);
			rcColumn.left += COLUMN_NAME_OFFSET;
			if (nCol == COLUMN_NAME)
			{
				cdc.DrawText(strText, strText.GetLength(), &rcColumn, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				cdc.DrawText(strText, strText.GetLength(), &rcColumn, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}

	cdc.SetTextColor(oldTextColor);
	cdc.SetBkColor(oldBkColor);
	cdc.Detach();
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::PreTranslateMessage
// Param    :   MSG* pMsg
// Return   :   BOOL
// Comment  :   PreTranslateMessage
// -----------------------------------------------------------//
#define VK_A	0x41
#define VK_C	0x41 + 'C' - 'A'
#define VK_X	0x41 + 'X' - 'A'
#define VK_Z	0x41 + 'Z' - 'A'
#define VK_V	0x41 + 'V' - 'A'
BOOL CListCtrlExt::PreTranslateMessage(MSG* pMsg)
{
	HACCEL haccel = LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_COMBOX));
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (haccel && TranslateAccelerator(m_hWnd, haccel, pMsg))
		{
			if (pMsg->wParam == VK_A)		// Copy
			{
				for (int i = GetItemCount() - 1; i >= 0; i--)
				{
					SetItemState(i, LVNI_SELECTED, LVNI_ALL | LVNI_SELECTED);
				}
			}
		}
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::OnColumnClick
// Param    :   NMHDR* pNMHDR
//              LRESULT* pResult
// Return   :   void
// Comment  :   OnColumnClick
// -----------------------------------------------------------//
void CListCtrlExt::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;
	if (iColumn == COLUMN_CHECKBOX)
	{
		HDITEM item = { 0 };
		item.mask = HDI_LPARAM;
		m_HeaderCtrl.GetItem(iColumn, &item);
		HEADDITEM* headitem = (PHEADDITEM)item.lParam;
		if (headitem && headitem->bCheckBox)
		{
			if (headitem->bCheckStatus)
			{
				headitem->bCheckStatus = false;
				SetAllUnCheck();
			}
			else
			{

				headitem->bCheckStatus = true;
				SetAllCheck();
			}
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
			OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
		}
		return;
	}

	Sort(iColumn, iColumn == m_nSortColumn ? !m_bSortAscending : TRUE);

	*pResult = 0;
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::SetCheckExt
// Param    :   int nIndex
// Return   :   void
// Comment  :   OnColumnClick
// -----------------------------------------------------------//
void CListCtrlExt::SetCheckExt(int nItem)
{
	if (nItem == -1)
		return;
	SetCheck(nItem, GetCheck(nItem) ? FALSE : TRUE);

	HDITEM item = { 0 };
	item.mask = HDI_LPARAM;
	m_HeaderCtrl.GetItem(COLUMN_CHECKBOX, &item);
	HEADDITEM* headitem = (PHEADDITEM)item.lParam;
	if (headitem && headitem->bCheckBox)
	{
		BOOL bAllCheck = BoolAllCheck();
		if ( headitem->bCheckStatus != bAllCheck)
		{
			headitem->bCheckStatus = bAllCheck;
			m_HeaderCtrl.Invalidate();
		}
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
		OnSetStatus();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
	}
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::OnLButtonDown
// Param    :   UINT nFlags
//              CPoint point
// Return   :   void
// Comment  :   OnLButtonDown
// -----------------------------------------------------------//
void CListCtrlExt::OnLButtonDown(UINT nFlags, CPoint point)
{
//	int nItem = HitTest(point);
	LVHITTESTINFO hti;
	hti.pt = point;
//	int nRes = (int) ::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&hti);
	int nRes = (int) ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)&hti);

	if (nRes >= 0 && hti.iItem != -1 && hti.iSubItem == COLUMN_CHECKBOX)
	{
		SetCheckExt(hti.iItem);
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}

// -----------------------------------------------------------//
// Function :   CListCtrlExt::OnLButtonDblClk
// Param    :   UINT nFlags
//              CPoint point
// Return   :   void
// Comment  :   OnLButtonDblClk
// -----------------------------------------------------------//
void CListCtrlExt::OnLButtonDblClk(UINT nFlags, CPoint point)
{
//	int nItem = HitTest(point);
	LVHITTESTINFO hti;
	hti.pt = point;
	int nRes = (int) ::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&hti);
	if (nRes >= 0 && hti.iItem != -1 && hti.iSubItem == COLUMN_CHECKBOX)
	{
		SetCheckExt(hti.iItem);
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CListCtrlExt::SetHeaderOwnerDraw(BOOL bOwnerDraw/* = TRUE */)
{
	HDITEM item;
	item.mask = HDI_FORMAT;
	int nCount = m_HeaderCtrl.GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		m_HeaderCtrl.GetItem(i, &item);
		item.fmt |= HDF_OWNERDRAW;
		m_HeaderCtrl.SetItem(i, &item);
	}
}

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
void CListCtrlExt::OnSetStatus()
{
	if (AfxGetApp() == NULL || AfxGetApp()->m_pMainWnd == NULL)
		return;

	CMainFrame* mainframe = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

	int nMaxfile = GetItemCount();
	int nSelFile = GetCheckCount();

	::SendMessage(mainframe->GetSafeHwnd(), WM_STATUS, nSelFile, nMaxfile);
}
// 2015.07.24 Ver1.00 [CMFCStatusBar] End

// 2015.07.18 Ver1.00[International] Start
void	CListCtrlExt::OnInitResourceLanguage()
{
	CString strValue = _T("");
	for (int i = COLUMN_NAME; i < COLUMN_MASK; i++)
	{
		HDITEM item = { 0 };
		item.mask = HDI_LPARAM;
		m_HeaderCtrl.GetItem(i, &item);
		HEADDITEM* headitem = (PHEADDITEM)item.lParam;
		if (headitem)
		{
			switch(i)
			{
			case COLUMN_NAME:
				strValue = DllLoadIDResource(IDS_Listctrl_Name);	break;
			case COLUMN_SIZE:
				strValue = DllLoadIDResource(IDS_Listctrl_Size);	break;
			case COLUMN_MODIFIED:
				strValue = DllLoadIDResource(IDS_Listctrl_Date);	break;
			}
			memset(headitem->szText, 0x00, MAX_PATH * sizeof(TCHAR));
			memcpy(headitem->szText, strValue, strValue.GetLength() > MAX_PATH ? MAX_PATH * sizeof(TCHAR) : strValue.GetLength() * sizeof(TCHAR));
		}
	}
	m_HeaderCtrl.Invalidate();

	// Statusbar
	OnSetStatus();
}
// 2015.07.18 Ver1.00[International] End 