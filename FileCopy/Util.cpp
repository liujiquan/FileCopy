//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   Util.cpp
//* Author   :   liujiquan
//* DateTime :   7/3/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Util.h"
// 2015.07.18 Ver1.00[International] Start
#include "resource.h"
// 2015.07.18 Ver1.00[International] End

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/* Global Paramater                                                     */
/************************************************************************/
COLORREF g_BKgroupColor[2][5] = {
	{RGB(63, 49, 81), RGB(96, 73, 123), RGB(178, 161, 199), RGB(204, 192, 218), RGB(229, 224, 236)},
	{RGB(29, 27, 17), RGB(74, 69, 42), RGB(148, 139, 84), RGB(197, 190, 151), RGB(221, 217, 195)}
};

COLORREF g_TextgroupColor[2][5] = {
	{RGB(255, 255, 255), RGB(255, 255, 255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0)},
	{RGB(255, 255, 255), RGB(255, 255, 255), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0)}
};

CString g_TabPageName[TABPAGECOUNT] = {
	_T("CyberXeed") , _T("e-clocking"), _T("individual"), _T("s-clocking")
};
// 2015.07.18 Ver1.00[International] Start
int g_language = 0;
// 2015.07.18 Ver1.00[International] End
/************************************************************************/
/* Setting.ini                                                          */
/************************************************************************/
// -----------------------------------------------------------//
// Function :   CString		GetSourceRootString
// Return   :   
// Comment  :   
// -----------------------------------------------------------//
CString	GetSourceRootString(int nTabPage)
{
	TCHAR szBuffer[MAX_PATH * 10] = { 0 };
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_SOURCE_ROOT, _T(""), szBuffer, MAX_PATH * 10, INIFILE);

	return CString(szBuffer);
}

// -----------------------------------------------------------//
// Function :   CString		GetTargetRootString
// Return   :   
// Comment  :   
// -----------------------------------------------------------//
CString	GetTargetRootString(int nTabPage)
{
	TCHAR szBuffer[MAX_PATH * 10] = { 0 };
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_TARGET_ROOT, _T(""), szBuffer, MAX_PATH * 10, INIFILE);

	return CString(szBuffer);
}

// -----------------------------------------------------------//
// Function :   CString		GetSourceRootString
// Return   :   
// Comment  :   
// -----------------------------------------------------------//
void	SetSourceRootString(int nTabPage, CString strSourcePath)
{
	WritePrivateProfileString(g_TabPageName[nTabPage], KEY_SOURCE_ROOT, strSourcePath, INIFILE);
}

// -----------------------------------------------------------//
// Function :   CString		GetTargetRootString
// Return   :   
// Comment  :   
// -----------------------------------------------------------//
void	SetTargetRootString(int nTabPage, CString strTargetPath)
{
	WritePrivateProfileString(g_TabPageName[nTabPage], KEY_TARGET_ROOT, strTargetPath, INIFILE);
}

// -----------------------------------------------------------//
// Function :   GetDiffDay
// Return   :   
// Comment  :   GetDiffDay
// -----------------------------------------------------------//
int	GetDiffDay(int nTabPage)
{
	return GetPrivateProfileInt(g_TabPageName[nTabPage], KEY_DIFFDAY, 0, INIFILE);
}

// -----------------------------------------------------------//
// Function :   SetDiffDay
// Param    :   int nDiffDay
// Return   :   
// Comment  :   SetDiffDay
// -----------------------------------------------------------//
BOOL	SetDiffDay(int nTabPage, int nDiffDay)
{
	return WritePrivateProfileString(g_TabPageName[nTabPage], KEY_DIFFDAY, NumberToString(nDiffDay), INIFILE);
}

// -----------------------------------------------------------//
// Function :   GetFilter
// Return   :   
// Comment  :   GetFilter
// -----------------------------------------------------------//
CString	GetFilter(int nTabPage)
{
	TCHAR szBuffer[MAX_PATH * 10] = { 0 };
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_FILTER, _T(""), szBuffer, MAX_PATH * 10, INIFILE);

	return CString(szBuffer);
}

// -----------------------------------------------------------//
// Function :   GetFilter
// param	   :	   CStringArray& strFilterArr
// Return   :   CStringArray
// Comment  :   GetFilter
// -----------------------------------------------------------//
void	GetFilter(int nTabPage, CStringArray& strFilterArr)
{
	CString	strFilter = _T("");
	CString	strTemp = _T("");
	int nIndex = 0;
	int nOldIndex = 0;
	TCHAR szBuffer[MAX_PATH * 10] = {0};

	strFilterArr.RemoveAll();
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_FILTER, _T(""), szBuffer, MAX_PATH * 10, INIFILE);
	strFilter = CString(szBuffer);
	nIndex = strFilter.Find(SPLIT_CHAR, nOldIndex);

	while (nIndex != -1)
	{
		strTemp = strFilter.Mid(nOldIndex, nIndex - nOldIndex).Trim();
		strFilterArr.Add(strTemp);

		nOldIndex = nIndex + 1;
		nIndex = strFilter.Find(_T(","), nOldIndex);
	}
	strTemp = strFilter.Mid(nOldIndex, strFilter.GetLength() - nOldIndex).Trim();
	strFilterArr.Add(strTemp);
}

// -----------------------------------------------------------//
// Function :   GetFilter
// param	:	CStringArray& strFilterArr
// Return   :   CStringArray
// Comment  :   GetFilter
// -----------------------------------------------------------//
void	GetDefaultFilter(int nTabPage, CStringArray& strFilterArr)
{
	CString	strFilter = _T("");
	CString	strTemp = _T("");
	int nIndex = 0;
	int nOldIndex = 0;
	TCHAR szBuffer[MAX_PATH * 10] = {0};

	strFilterArr.RemoveAll();
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_DEFAULT_FILTER, DEFAULT_FILTER, szBuffer, MAX_PATH * 10, INIFILE);
	strFilter = CString(szBuffer);
	nIndex = strFilter.Find(SPLIT_CHAR, nOldIndex);

	while (nIndex != -1)
	{
		strTemp = strFilter.Mid(nOldIndex, nIndex - nOldIndex).Trim();
		strFilterArr.Add(strTemp);

		nOldIndex = nIndex + 1;
		nIndex = strFilter.Find(_T(","), nOldIndex);
	}
	strTemp = strFilter.Mid(nOldIndex, strFilter.GetLength() - nOldIndex).Trim();
	strFilterArr.Add(strTemp);
}

// -----------------------------------------------------------//
// Function :   SetFilter
// param	   :	   CString strFilter
// Return   :   
// Comment  :   SetFilter
// -----------------------------------------------------------//
BOOL	SetFilter(int nTabPage, CString strFilter)
{
	return WritePrivateProfileString(g_TabPageName[nTabPage], KEY_FILTER, strFilter, INIFILE);
}

// -----------------------------------------------------------//
// Function :   GetAutoRefresh
// param    :  
// Return   :   bool
// Comment  :   GetAutoRefresh
// -----------------------------------------------------------//
BOOL GetAutoRefresh(int nTabPage)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	GetPrivateProfileString(g_TabPageName[nTabPage], KEY_AUTOREFRESH, DEFAULT_AUTOFRESH, szBuffer, MAX_PATH, INIFILE);

	CString strBuffer(szBuffer);
	strBuffer.Trim();
	strBuffer.MakeUpper();

	return strBuffer == _T("TRUE");
}

BOOL SetAutoRefresh(int nTabPage, BOOL bRefresh)
{
	CString strBuffer = bRefresh ? _T("TRUE") : _T("FALSE");

	return WritePrivateProfileString(g_TabPageName[nTabPage], KEY_AUTOREFRESH, strBuffer, INIFILE);
}

// 2015.07.18 Ver1.00[International] Start
BOOL	SetLanguage(CString strLanguage)
{
	return WritePrivateProfileString(SEC_LANGUAGE, KEY_LANGUAGE, strLanguage, INIFILE);
}

int GetLanguage()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	int nLanguage = 0;

	GetPrivateProfileString(SEC_LANGUAGE, KEY_LANGUAGE, DEFAULT_LANGUAGE, szBuffer, MAX_PATH, INIFILE);
	CString strBuffer(szBuffer);
	strBuffer.Trim();
	if (strBuffer == DllLoadIDResource(IDS_Language_en))
	{
		nLanguage = 0;
	}
	else if (strBuffer == DllLoadIDResource(IDS_Language_cn))
	{
		nLanguage = 1;
	}
	else if (strBuffer == DllLoadIDResource(IDS_Language_jp))
	{
		nLanguage = 2;
	}

	return nLanguage;
}
// 2015.07.18 Ver1.00[International] End
/************************************************************************/
/* Shell							                                    */
/************************************************************************/
// -----------------------------------------------------------//
// Function :   SHGetFileDetail
// Param    :   LPSHELLFOLDER2 pFolder
//              LPITEMIDLIST pIDList
//              UINT iColumn
// Return   :   CString
// Comment  :   
// -----------------------------------------------------------//
CString SHGetFileDetail(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList, UINT iColumn)
{
	if (pFolder == NULL || pIDList == NULL)
		return _T("");

	STRRET retFile = {0};								// filename(strref)
	TCHAR szBuffer[MAX_PATH] = { 0 };				// filename(real)
	SHELLDETAILS details;
	HRESULT hr = S_FALSE;

	hr = pFolder->GetDetailsOf(pIDList, iColumn, &details);
	if (hr != S_OK)
	{
		return _T("");
	}
	StrRetToBuf(&details.str, pIDList, szBuffer, MAX_PATH);

	if (iColumn == DETAILS_MODIFIED)
	{
		CString strFilePath = SHGetFilePath(pFolder, pIDList);
		CFileStatus fs;
		if (CFile::GetStatus(strFilePath, fs))
		{
			COleDateTime dateFile(fs.m_mtime.GetTime());
			CString str = _T("");

			str.Format(_T("%04d/%02d/%02d %02d:%02d"), dateFile.GetYear(), dateFile.GetMonth(), dateFile.GetDay(), dateFile.GetHour(), dateFile.GetMinute());

			return str;
		}
		else
		{
			return _T("");
		}
	}
	else if (iColumn == DETAILS_SIZE)
	{
		CString strFilePath = SHGetFilePath(pFolder, pIDList);
		CFileStatus fs;
		if (CFile::GetStatus(strFilePath, fs))
		{
			CString str = _T("");
			if ( (fs.m_attribute & (CFile::directory | CFile ::volume) ) == 0 )
			{
				OnFormatFileSize(fs.m_size, str);
			}

			return str;
		}
		else
		{
			return _T("");
		}
	}
	else
	{
		return CString(szBuffer);
	}
}

// -----------------------------------------------------------//
// Function :   OnFormatFileSize
// Param    :   __int64 lFileSize
//              CString& str
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void OnFormatFileSize(__int64 lFileSize, CString& str)
{
	str.Empty();

	if (lFileSize < 1024)
	{
		str.Format(_T("%d"), lFileSize);
		str += _T(" B");
	}
	else
	{
		lFileSize = lFileSize / 1024 + 1;
		str.Format(_T("%I64d"), lFileSize);

		// Convert number to the system format:
		TCHAR szNumOut [256];
		GetNumberFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, str, NULL, szNumOut, 255);

		str = szNumOut;

		// Truncate trailing fractal digits:
		TCHAR szDec [10];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDec, 10);

		int nDecLen = lstrlen(szDec);
		if (nDecLen > 0)
		{
			for (int i = str.GetLength() - nDecLen - 1; i >= 0; i--)
			{
				if (str.Mid(i, nDecLen) == szDec)
				{
					str = str.Left(i);
					break;
				}
			}
		}
		str += _T(" KB");
	}
}

// -----------------------------------------------------------//
// Function :   SHGetFilePath
// Param    :   LPSHELLFOLDER2 pFolder
//              LPITEMIDLIST pIDList
// Return   :   CString
// Comment  :   
// -----------------------------------------------------------//
CString SHGetFilePath(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList)
{
	if (pFolder == NULL || pIDList == NULL)
		return _T("");

	STRRET retFile;								// filename(strref)
	TCHAR szBuffer[MAX_PATH] = { 0 };				// filename(real)

	// 3.3-filename(strref)
	HRESULT hr = pFolder->GetDisplayNameOf(pIDList, SHGDN_FORPARSING, &retFile);
	// 3.4-filename(real)
	StrRetToBuf(&retFile, pIDList, szBuffer, MAX_PATH);

	return CString(szBuffer);
}

// -----------------------------------------------------------//
// Function :   SHGetPidlFolderFromPath
// Param    :   CString strPath
//              LPSHELLFOLDER& pFolder
//              LPITEMIDLIST& pIDList
// Return   :   HRESULT
// Comment  :   
// -----------------------------------------------------------//
HRESULT SHGetPidlFolderFromPath(CString strPath, LPSHELLFOLDER& pFolder, LPITEMIDLIST& pIDList)
{
	if (strPath.IsEmpty())
		return S_FALSE;

	IShellFolder* pDesktopShellFolder = NULL;		// Desktop(IShellFolder)

	// 1.1-Desktop(IShellFolder)
	HRESULT hr = SHGetDesktopFolder(&pDesktopShellFolder);

	// 2
	// 2.1-RootPath(IDList)
	hr = pDesktopShellFolder->ParseDisplayName(::GetActiveWindow(),
		NULL,
		strPath.GetBuffer(0),
		NULL,
		&pIDList,
		NULL);
	// 2.2-RootPath(IShellFolder2)
	hr = pDesktopShellFolder->BindToObject(pIDList, NULL, IID_IShellFolder, (void**)&pFolder);

	if (pDesktopShellFolder)	// Desktop(IShellFolder)
		pDesktopShellFolder->Release();

	return hr;
}

// -----------------------------------------------------------//
// Function :   SHGetPidlFolderFromPath2
// Param    :   CString strPath
//              LPSHELLFOLDER2& pFolder
//              LPITEMIDLIST& pIDList
// Return   :   HRESULT
// Comment  :   
// -----------------------------------------------------------//
HRESULT SHGetPidlFolderFromPath2(CString strPath, LPSHELLFOLDER2& pFolder, LPITEMIDLIST& pIDList)
{
	if (strPath.IsEmpty())
		return S_FALSE;

	IShellFolder* pDesktopShellFolder = NULL;		// Desktop(IShellFolder)
	IShellFolder2* pDesktopShellFolder2 = NULL;		// Desktop(IShellFolder2)

	// 1.1-Desktop(IShellFolder)
	HRESULT hr = SHGetDesktopFolder(&pDesktopShellFolder);
	// 1.2-Desktop(IShellFolder2)
	hr = pDesktopShellFolder->QueryInterface(IID_IShellFolder2, (void**)&pDesktopShellFolder2);

	// 2
	// 2.1-RootPath(IDList)
	hr = pDesktopShellFolder2->ParseDisplayName(::GetActiveWindow(),
		NULL,
		strPath.GetBuffer(0),
		NULL,
		&pIDList,
		NULL);
	// 2.2-RootPath(IShellFolder2)
	hr = pDesktopShellFolder2->BindToObject(pIDList, NULL, IID_IShellFolder2, (void**)&pFolder);

	// 3.Release
	if (pDesktopShellFolder2)	// Desktop(IShellFolder2)
		pDesktopShellFolder2->Release();

	if (pDesktopShellFolder)	// Desktop(IShellFolder)
		pDesktopShellFolder->Release();

	return hr;
}

// -----------------------------------------------------------//
// Function :   SHGetIconIndex
// Param    :   LPITEMIDLIST pIDList
// Return   :   int
// Comment  :   
// -----------------------------------------------------------//
int SHGetIconIndex(LPITEMIDLIST pIDList)
{
	SHFILEINFO sfi;
	int iIcon = -1;
	if (SHGetFileInfo((LPCTSTR)pIDList, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
	{
		iIcon = sfi.iIcon;
	}

	return iIcon;
}

// -----------------------------------------------------------//
// Function :   SHGetFileDate
// Param    :   LPSHELLFOLDER2 pFolder
//              LPITEMIDLIST pIDList
// Return   :   CTime
// Comment  :   Get Diff Day
// -----------------------------------------------------------//
CTime SHGetFileDate(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList)
{
	CString str1 = SHGetFilePath(pFolder, pIDList);
	if (str1.IsEmpty())
	{
		return NULL;
	}

	CFileStatus fs;
	if (CFile::GetStatus(str1, fs) == FALSE)
	{
		return NULL;
	}

	return fs.m_mtime;
}

// -----------------------------------------------------------//
// Function :   SHGetDiffDays
// Param    :   LPSHELLFOLDER2 pFolder
//              LPITEMIDLIST pIDList
// Return   :   long
// Comment  :   Get Diff Day
// -----------------------------------------------------------//
long SHGetDiffDays(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList)
{
	CTime tmNow_ = CTime::GetCurrentTime();
	CTime tmNow(tmNow_.GetYear(), tmNow_.GetMonth(), tmNow_.GetDay(), 23, 59, 59);
	CTime tmFile = SHGetFileDate(pFolder, pIDList);
	CTimeSpan timespan;
	long ldays = 0;

	if (tmFile == NULL || tmNow == NULL)
		return -1;

	CString str1 = tmNow.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString str2 = tmFile.Format(_T("%Y-%m-%d %H:%M:%S"));
	timespan = tmNow - tmFile;
	ldays = timespan.GetDays(); 

	return ldays;
}

// -----------------------------------------------------------//
// Function :   SHPathToPidlEx
// Param    :   LPTSTR szPath
//              LPSHELLFOLDER pParentFolder
//              LPITEMIDLIST* ppidl
//              LPSHELLFOLDER* ppFolder
// Return   :   HRESULT
// Comment  :   
// -----------------------------------------------------------//
HRESULT SHPathToPidlEx(LPTSTR szPath, LPSHELLFOLDER pParentFolder, LPITEMIDLIST* ppidl, LPSHELLFOLDER* ppFolder)
{
	LPSHELLFOLDER pDesktopFolder = NULL;
	ULONG nCharsParsed = 0;
	BOOL bDesktop = FALSE;
	HRESULT hr = S_OK;

	// 1.ParentFolder
	if (pParentFolder == NULL)
	{
		SHGetDesktopFolder(&pDesktopFolder);
		if (pDesktopFolder == NULL)
			return S_FALSE;

		bDesktop = TRUE;
	}

	// 2.PATH -> PIDL
	hr = pDesktopFolder->ParseDisplayName(NULL, NULL, szPath, &nCharsParsed, ppidl, NULL);

	// 3.PIDL -> ShellFolder
	if (bDesktop)
		hr = pDesktopFolder->BindToObject(*ppidl, NULL, IID_IShellFolder, (void**)ppFolder);
	else
		hr = pParentFolder->BindToObject(*ppidl, NULL, IID_IShellFolder, (void**)ppFolder);

	if (ppFolder == NULL)
		hr = S_FALSE;

	// 4.Release DesktopFolder
	if (bDesktop)
		pDesktopFolder->Release();

	return hr;
}

// -----------------------------------------------------------//
// Function :   SHPidlToShellFolder
// Param    :   LPSHELLFOLDER pParentFolder
//              LPITEMIDLIST* ppidl
//              LPSHELLFOLDER* ppFolder
// Return   :   HRESULT
// Comment  :   
// -----------------------------------------------------------//
HRESULT SHPidlToShellFolder(LPSHELLFOLDER pParentFolder, LPITEMIDLIST* ppidl, LPSHELLFOLDER* ppFolder)
{
	LPSHELLFOLDER pDesktopFolder = NULL;
	BOOL bDesktop = FALSE;
	HRESULT hr = S_OK;

	// 1.ParentFolder
	if (pParentFolder == NULL)
	{
		SHGetDesktopFolder(&pDesktopFolder);
		if (pDesktopFolder == NULL)
			return S_FALSE;

		bDesktop = TRUE;
	}

	// 2.PIDL -> ShellFolder
	if (bDesktop)
		hr = pDesktopFolder->BindToObject(*ppidl, NULL, IID_IShellFolder, (void**)ppFolder);
	else
		hr = pParentFolder->BindToObject(*ppidl, NULL, IID_IShellFolder, (void**)ppFolder);

	if (ppFolder == NULL)
		hr = S_FALSE;

	// 3.Release DesktopFolder
	if (bDesktop)
		pDesktopFolder->Release();

	return hr;
}

// -----------------------------------------------------------//
// Function :   SHSelectFolder
// Param    :   CString& strFolderPath
// Return   :   BOOL
// Comment  :   select folder
// -----------------------------------------------------------//
BOOL SHSelectFolder(HWND hwnd, CString& strFolderPath)			
{
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = hwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("Select Folder..");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI | BIF_SHAREABLE | BIF_BROWSEINCLUDEURLS | BIF_UAHINT | BIF_NONEWFOLDERBUTTON;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		return FALSE;
	}

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		strFolderPath = CString(pszPath);

		return TRUE;
	}

	return FALSE;
}

// -----------------------------------------------------------//
// Function :   SHSelectFolder2
// Param    :   CString& strFolderPath
// Return   :   BOOL
// Comment  :   select folder
// -----------------------------------------------------------//
BOOL SHSelectFolder2(HWND hwnd, CString& strFolderPath)
{
	::CoInitialize(NULL);

	IFileDialog* pIFileDialog = NULL;
	BOOL bSuccess = FALSE;
	DWORD dwFlags = 0;

	HRESULT	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
		IID_IFileDialog, (void**)&pIFileDialog);
	if (hr == S_OK && pIFileDialog)
	{
		hr = pIFileDialog->GetOptions(&dwFlags);
		hr = pIFileDialog->SetOptions(dwFlags | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_SHAREAWARE);
		hr = pIFileDialog->SetTitle(_T("Select one folder"));
		hr = pIFileDialog->SetOkButtonLabel(_T("OK"));

		hr = pIFileDialog->Show(hwnd);

		if (hr == S_OK)
		{
			IShellItem* pItem = NULL;
			LPWSTR filePath = NULL;

			hr = pIFileDialog->GetFolder(&pItem);
			if (hr == S_OK && pItem)
			{
				pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filePath);
				strFolderPath = CString(filePath);
				bSuccess = TRUE;
			}
		}

		pIFileDialog->Release();
	}

	::CoUninitialize();

	return bSuccess;
}

// -----------------------------------------------------------//
// Function :   SHSelectFolder3
// Param    :   CString& strFolderPath
// Return   :   BOOL
// Comment  :   select folder
// -----------------------------------------------------------//
const GUID FAR CLSID_FileOpenDialog2 = 
{ 
	0x61744fc7, 0x85b5, 0x4791, 
	0xa9, 0xb0, 0x27, 0x22, 
	0x76, 0x30, 0x9b, 0x13 
};
//DEFINE_GUID(CLSID_FileOpenDialog2,
//	0x61744fc7, 0x85b5, 0x4791, 0xa9, 0xb0, 0x27, 0x22, 0x76,
//	0x30, 0x9b, 0x13);
BOOL SHSelectFolder3(HWND hwnd, CString& strFolderPath)
{
	::CoInitialize(NULL);

	IFileDialog*	pIFileDialog = NULL;
	IFileDialog2*	 pIFileDialog2 = NULL;
	BOOL bSuccess = FALSE;
	DWORD dwFlags = 0;

	HRESULT	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
		IID_IFileDialog, (void**)&pIFileDialog);

	if (hr == S_OK && pIFileDialog)
	{
		hr = pIFileDialog->QueryInterface(IID_IFileDialog2, (void**)&pIFileDialog2);  
//		hr = CoCreateInstance(CLSID_FileOpenDialog2, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog2, (void**)&pIFileDialog2);
		if (hr == S_OK && pIFileDialog2)
		{
			hr = pIFileDialog2->GetOptions(&dwFlags);
			hr = pIFileDialog2->SetOptions(dwFlags | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_SHAREAWARE);
			hr = pIFileDialog2->SetTitle(_T("Select one folder"));
			hr = pIFileDialog2->SetOkButtonLabel(_T("OK"));
			hr = pIFileDialog2->SetCancelButtonLabel(_T("Cancel"));

			hr = pIFileDialog2->Show(hwnd);

			if (hr == S_OK)
			{
				IShellItem* pItem = NULL;
				LPWSTR filePath = NULL;

				hr = pIFileDialog2->GetFolder(&pItem);
				if (hr == S_OK && pItem)
				{
					pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filePath);
					strFolderPath = CString(filePath);
					bSuccess = TRUE;
				}
			}

			pIFileDialog2->Release();
		}

		pIFileDialog->Release();
	}
	

	::CoUninitialize();

	return bSuccess;
}

////////////////////////////////////////////////////////////////////////////
// Function
CString NumberToString(int number)
{
	CString str = _T("");

	str.Format(_T("%d"), number);
	return str;
}

FileInfo g_FileInfo[] = {
//				Type				Name			Info			
	FileInfo(	_T("*.h"),		_T("*.h"),	DllLoadIDResource(IDS_FileType_h)),    		
	FileInfo(	_T("*.cpp"),		_T("cpp"),	DllLoadIDResource(IDS_FileType_cpp)),    	
	FileInfo(	_T("*.idl"),		_T("idl"),	DllLoadIDResource(IDS_FileType_idl)),    	
	FileInfo(	_T("*.def"),		_T("def"),	DllLoadIDResource(IDS_FileType_def)),    	
	FileInfo(	_T("*.sql"),		_T("sql"),	DllLoadIDResource(IDS_FileType_sql)),    	
	FileInfo(	_T("*.rc"),		_T("rc"),	DllLoadIDResource(IDS_FileType_rc)),    	
	FileInfo(	_T("*.rc2"),		_T("rc2"),	DllLoadIDResource(IDS_FileType_rc2)),    	
	FileInfo(	_T("*.rgs"),		_T("rgs"),	DllLoadIDResource(IDS_FileType_rgs)),    	
	FileInfo(	_T("*.bmp"),		_T("bmp"),	DllLoadIDResource(IDS_FileType_bmp)), 		
	FileInfo(	_T("*.png"),		_T("png"),	DllLoadIDResource(IDS_FileType_png)),		
	FileInfo(	_T("*.gif"),		_T("gif"),	DllLoadIDResource(IDS_FileType_gif)),     	
	FileInfo(	_T("*.jpg"),		_T("jpg"),	DllLoadIDResource(IDS_FileType_jpg)),    	
	FileInfo(	_T("*.cur"),		_T("cur"),	DllLoadIDResource(IDS_FileType_cur)),     	
	FileInfo(	_T("*.ico"),		_T("ico"),	DllLoadIDResource(IDS_FileType_ico)),     	
	FileInfo(	_T("*.html"),		_T("html"),	DllLoadIDResource(IDS_FileType_html)),    	
	FileInfo(	_T("*.xml"),		_T("xml"),	DllLoadIDResource(IDS_FileType_xml)),     	
	FileInfo(	_T("*.css"),		_T("css"),	DllLoadIDResource(IDS_FileType_css)),     	
	FileInfo(	_T("*.js"),		_T("js"),	DllLoadIDResource(IDS_FileType_js)),      	
	FileInfo(	_T("*.sln"),		_T("sln"),	DllLoadIDResource(IDS_FileType_sln)),     	
	FileInfo(	_T("*.vcproj"),	_T("vcproj"),	DllLoadIDResource(IDS_FileType_vcproj)), 	
	FileInfo(	_T("*.asp"),		_T("asp"),	DllLoadIDResource(IDS_FileType_asp)),     	
	FileInfo(	_T("*.inc"),		_T("inc"),	DllLoadIDResource(IDS_FileType_inc)),     	
	FileInfo(	_T("*.dll"),		_T("dll"),	DllLoadIDResource(IDS_FileType_dll)),     	
	FileInfo(	_T("*.exe"),		_T("exe"),	DllLoadIDResource(IDS_FileType_exe)),     	
	FileInfo(	_T("*.dsw"),		_T("dsw"),	DllLoadIDResource(IDS_FileType_dsw)),  	
	FileInfo(	_T("*.dsp"),		_T("dsp"),	DllLoadIDResource(IDS_FileType_dsp)),  	
	FileInfo(	_T("*.bat"),		_T("bat"),	DllLoadIDResource(IDS_FileType_bat)),  	
	FileInfo(	_T("*.*"),		_T("*"),		DllLoadIDResource(IDS_FileType_all )),  	
};

CString GetFileTypeInfo(CString strFileType)
{
	strFileType = strFileType.Trim();		// trim
	strFileType = strFileType.MakeLower();	// lower

	int nSize = sizeof(g_FileInfo) / sizeof(FileInfo);
	// 0-default
	for(int i = 0; i < nSize; i++)											
	{
		if (strFileType == CString(g_FileInfo[i].szType))
		{
			return CString(g_FileInfo[i].szInfo);
		}
	}

	// 1-*.XXX -> XXX ファイル(.XXX)
	if (strFileType.GetLength() > 2 && strFileType.Left(2) == TYPEHEADER)	
	{
		CString strName =  strFileType.Right(strFileType.GetLength() - 2);

		return strName + DllLoadIDResource(IDS_FileType_ext) + _T("(.") + strName + _T(")");
	}

	return _T("");
}

CString GetFileType(CString strFileInfo)
{
	int nSize = sizeof(g_FileInfo) / sizeof(FileInfo);
	// 0-default
	for(int i = 0; i < nSize; i++)											
	{
		if (strFileInfo == CString(g_FileInfo[i].szInfo))
		{
			return CString(g_FileInfo[i].szType);
		}
	}

	// 1- XXX ファイル(.XXX) - > *.XXX
	TCHAR szName1[MAX_PATH] = {0};
	TCHAR szName2[MAX_PATH] = {0};
	// szName1:"11" szName2:"11)"
//	if (::_stscanf_s(strFileInfo.GetBuffer(0), _T("%s ファイル(.%s)"), szName1, MAX_PATH,  szName2, MAX_PATH) > 0)	
	if (::_stscanf_s(strFileInfo.GetBuffer(0), (_T("%s") + DllLoadIDResource(IDS_FileType_ext) + _T("%s")), szName1, MAX_PATH,  szName2, MAX_PATH) > 0)
	{
		CString strName1 = _T("(.") + CString(szName1) + _T(")");
		if ( strName1 == CString(szName2))
		{
			return TYPEHEADER + CString(szName1);
		}
	}

	return _T("");
}

CRect GetCenterRect(CRect rect, int nWid, int nHei)
{
	int nWidth = rect.Width();
	int nHeith = rect.Height();

	if (nWidth < nWid && nHeith < nHei)
		return CRect(rect);

	if (nWidth > nWid)
	{
		rect.left = rect.left + (nWidth - nWid) / 2;
		rect.right = rect.right - (nWidth - nWid) / 2;
	}

	if (nHeith > nHei)
	{
		rect.top = rect.top + (nHeith - nHei) / 2;
		rect.bottom = rect.bottom - (nHeith - nHei) / 2;
	}

	return CRect(rect);
}

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
CString GetCurrentTimeString()
{
	return CTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S"));
}
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
// 2015.07.18 Ver1.00[International] Start
CString DllLoadIDResource(UINT nID)
{
	TCHAR szBuffer[MAX_PATH * 10] = {0};

//	::LoadString(::AfxGetResourceHandle(), nID, szBuffer, MAX_PATH * 10);
	::LoadString(NULL, nID, szBuffer, MAX_PATH * 10);
	return CString(szBuffer);
}
/*
CString DllLoadIDResource(UINT nID, int nType)
{
	TCHAR szBuffer[MAX_PATH * 10] = {0};

	switch(nType)
	{
	case 0:		// En
//		::LoadString(::AfxGetResourceHandle(), nID, szBuffer, MAX_PATH * 10);	break;
		::LoadString(NULL, nID, szBuffer, MAX_PATH * 10);	break;
	case 1:		// Ch
//		::LoadString(::AfxGetResourceHandle(), nID + 1, szBuffer, MAX_PATH * 10);	break;
		::LoadString(NULL, nID + 1, szBuffer, MAX_PATH * 10);	break;
	case 2:		// JP
//		::LoadString(::AfxGetResourceHandle(), nID + 2, szBuffer, MAX_PATH * 10);	break;
		::LoadString(NULL, nID + 2, szBuffer, MAX_PATH * 10);	break;
	}
	return CString(szBuffer);
}*/
// 2015.07.18 Ver1.00[International] End