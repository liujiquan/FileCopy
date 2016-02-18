
// FileCopy.h : main header file for the FileCopy application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFileCopyApp:
// See FileCopy.cpp for the implementation of this class
//

class CFileCopyApp : public CWinAppEx
{
public:
	CFileCopyApp();
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
// 2015.07.23 Ver1.00 [Modify Style] Start
	UINT  m_nAppLook;
// 2015.07.23 Ver1.00 [Modify Style] End
	BOOL	m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	void OnFileProcsel();
	int GetTabPage(){	
		return m_nTabPage; 
	}
private:
	int m_nTabPage;
};
extern CFileCopyApp theApp;
