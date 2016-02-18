//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   Util.h
//* Author   :   liujiquan
//* DateTime :   7/3/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __UTIL_H_2459134951B94A1895E650CFD52F4215__
#define __UTIL_H_2459134951B94A1895E650CFD52F4215__
#pragma once

/************************************************************************/
/* Setting.ini                                                          */
/************************************************************************/
// Define
#define		MAX_WID				1000
#define		MAX_HEI				800
#define		COPYFILE				false
#define		INIFILE				_T(".\\Setting.ini")
#define		MAX_ROOT_SIZE			10

#define		TABPAGECOUNT			4
#define		TABPAGE0				g_TabPageName[0]
#define		TABPAGE1				g_TabPageName[1]
#define		TABPAGE2				g_TabPageName[2]
#define		TABPAGE3				g_TabPageName[3]

#define		KEY_SOURCE_ROOT			_T("SourceRoot")
#define		KEY_TARGET_ROOT			_T("TargetRoot")
#define		KEY_DIFFDAY				_T("DiffDay")
#define		KEY_FILTER				_T("Filter")
#define		KEY_DEFAULT_FILTER		_T("DefaultFilter")
#define		SPLIT_CHAR				_T(",")
#define		DEFAULT_FILTER			_T("*.h,*.cpp,*.idl,*.def,*.sql,*.rc,*.rc2,*.sln,*.vcproj,*.ico,*.bmp,*.jpg,*.asp,*.png,*.gif,*.inc,*.dll,*.exe,*.js,*.css,*.html,*.dsp,*.bat")
#define		KEY_AUTOREFRESH			_T("AutoRefresh")
#define		DEFAULT_AUTOFRESH			_T("false")
// 2015.07.18 Ver1.00[International] Start
#define		SEC_LANGUAGE				_T("Language")
#define		KEY_LANGUAGE				_T("Language")
#define		DEFAULT_LANGUAGE			_T("English")
// 2015.07.18 Ver1.00[International] End
// Diff Days
#define COMBOX_ID				WM_USER+201
#define DIFFOneDay			1
//#define DIFFThreeDay			3
#define DIFFOneWeek			7
#define DIFFOneMonth			30
//#define DIFFOneYear			360
#define DIFFAll				-1

#define DIFFOneDay_TextColor		g_TextgroupColor[0][1]
#define DIFFOneWeek_TextColor		g_TextgroupColor[0][2]
#define DIFFOneMonth_TextColor		g_TextgroupColor[0][3]
#define DIFFAll_TextColor			g_TextgroupColor[0][4]

#define DIFFOneDay_BkColor			g_BKgroupColor[0][1]
#define DIFFOneWeek_BkColor		g_BKgroupColor[0][2]
#define DIFFOneMonth_BkColor		g_BKgroupColor[0][3]
#define DIFFALL_BkColor			g_BKgroupColor[0][4]
// Function
CString	GetSourceRootString(int nTabPage);												// Setting.ini
void		SetSourceRootString(int nTabPage, CString strSourceRoot);						// Setting.ini
CString	GetTargetRootString(int nTabPage);												// Setting.ini
void		SetTargetRootString(int nTabPage, CString strTargetRoot);						// Setting.ini

int		GetDiffDay(int nTabPage);
BOOL		SetDiffDay(int nTabPage, int nDiffDay);
CString	GetFilter(int nTabPage);
void		GetDefaultFilter(int nTabPage, CStringArray& strFilterArr);
void		GetFilter(int nTabPage, CStringArray& strFilterArr);
BOOL		SetFilter(int nTabPage, CString strFilter);

BOOL		GetAutoRefresh(int nTabPage);
BOOL		SetAutoRefresh(int nTabPage, BOOL bRefresh);
// 2015.07.18 Ver1.00[International] Start
BOOL		SetLanguage(CString strLanguage);
int		GetLanguage();
// 2015.07.18 Ver1.00[International] End
/************************************************************************/
/* Shell																*/
/************************************************************************/
// Define
#define DETAILS_NAME		0
#define DETAILS_SIZE		1
#define DETAILS_TYPE		2
#define DETAILS_MODIFIED	3

// Enum/Struct
typedef IShellFolder2 *LPSHELLFOLDER2;
typedef struct _AFX_SHELLITEMINFO2
{
	LPSHELLFOLDER2	pParentFolder;
	LPITEMIDLIST		pidlFQ;
	LPITEMIDLIST		pidlRel;
	TCHAR			szFQPath[MAX_PATH];	// \\XXXX\\XX\X
	int				nDiffday;

	_AFX_SHELLITEMINFO2()
	{
		pParentFolder = NULL;
		pidlFQ = NULL;
		pidlRel = NULL;
		nDiffday = 0;
		memset(szFQPath, 0x00, MAX_PATH * sizeof(TCHAR));
	}
}AFX_SHELLITEMINFO2, FAR *LPAFX_SHELLITEMINFO2;

// Function
HRESULT SHPathToPidlEx(LPTSTR szPath, LPSHELLFOLDER pParentFolder, LPITEMIDLIST* ppidl, LPSHELLFOLDER* ppFolder);
HRESULT SHPidlToShellFolder(LPSHELLFOLDER pParentFolder, LPITEMIDLIST* ppidl, LPSHELLFOLDER* ppFolder);

CString SHGetFileDetail(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList, UINT iColumn);				// Get File Details
CString SHGetFilePath(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList);							// Get File Full Path
HRESULT SHGetPidlFolderFromPath(CString strPath, LPSHELLFOLDER& pFolder, LPITEMIDLIST& pIDList);
HRESULT SHGetPidlFolderFromPath2(CString strPath, LPSHELLFOLDER2& pFolder, LPITEMIDLIST& pIDList);
int SHGetIconIndex(LPITEMIDLIST pIDList);													// Get File Icon
CTime SHGetFileDate(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList);								// Get File Date
long SHGetDiffDays(LPSHELLFOLDER2 pFolder, LPITEMIDLIST pIDList);								// Get Diff Day

BOOL SHSelectFolder(HWND hwnd, CString& strFolderPath);			// select folder
BOOL SHSelectFolder2(HWND hwnd, CString& strFolderPath);			// select folder2
BOOL SHSelectFolder3(HWND hwnd, CString& strFolderPath);			// select folder2
////////////////////////////////////////////////////////////////////////////
// Function
CString NumberToString(int number);
CString GetFileTypeInfo(CString strFileType);
CString GetFileType(CString strFileInfo);
CRect GetCenterRect(CRect rect, int nWid, int nHei);
void OnFormatFileSize(__int64 lFileSize, CString& str);
// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
CString GetCurrentTimeString();
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
// 2015.07.18 Ver1.00[International] Start
CString DllLoadIDResource(UINT nID);
//CString DllLoadIDResource(UINT nID, int nType);
// 2015.07.18 Ver1.00[International] End

/************************************************************************/
/* Type/FileName/ShowName												*/
/************************************************************************/
#define TYPEHEADER	_T("*.")
// 2015.07.24 Ver1.00 [File Filter] Start
#define TYPEALL		_T("*.*")
// 2015.07.24 Ver1.00 [File Filter] End

typedef struct FileInfo{
	TCHAR	szType[MAX_PATH];		// *.cpp
	TCHAR	szName[MAX_PATH];		// cpp
	TCHAR	szInfo[MAX_PATH];		// C++ Source(.cpp)
	FileInfo(){
		memset(this, 0x00, sizeof(FileInfo));
	}
	FileInfo(LPCTSTR pszType, LPCTSTR pszName, LPCTSTR pszInfo){
		memset(this, 0x00, sizeof(FileInfo));
		memcpy_s(szType, MAX_PATH, pszType, _tcslen(pszType) * sizeof (TCHAR));
		memcpy_s(szName, MAX_PATH, pszName, _tcslen(pszName) * sizeof (TCHAR));
		memcpy_s(szInfo, MAX_PATH, pszInfo, _tcslen(pszInfo) * sizeof (TCHAR));
	}
}FileInfo, *PFileInfo;

// 2015.07.24 Ver1.00 [CMFCStatusBar] Start
#define STATUS_SELFILE_INDEX		5
#define STATUS_MAXFILE_INDEX		STATUS_SELFILE_INDEX + 1
#define STATUS_TIME_INDEX			STATUS_MAXFILE_INDEX + 1
#define TIMER_EVENT				1
#define WM_STATUS					WM_USER + 0x130
// 2015.07.24 Ver1.00 [CMFCStatusBar] End
// 2015.07.18 Ver1.00[International] Start
#define MEMNU_FILE_INDEX			0						// file
#define MEMNU_SETTING_INDEX		MEMNU_FILE_INDEX + 2		// Setting
#define MEMNU_OutLook_INDEX		MEMNU_SETTING_INDEX + 2	// application look
#define MEMNU_Help_INDEX			MEMNU_OutLook_INDEX + 2	// help

#define TOOLBAR_EXIT_INDEX			0						// exit
#define TOOLBAR_DIFFDAY_INDEX		TOOLBAR_EXIT_INDEX+2		// diffday
#define TOOLBAR_REFRESH_INDEX		TOOLBAR_DIFFDAY_INDEX+1	// refresh
#define TOOLBAR_RELEASE_INDEX		TOOLBAR_REFRESH_INDEX+2	// release
#define TOOLBAR_SETTING_INDEX		TOOLBAR_RELEASE_INDEX+2	// setting
#define TOOLBAR_ABOUT_INDEX		TOOLBAR_SETTING_INDEX+2	// about

// 2015.07.18 Ver1.00[International] End
#endif//__UTIL_H_2459134951B94A1895E650CFD52F4215__
