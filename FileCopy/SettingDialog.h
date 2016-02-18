//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   DialogSetting.h
//* Author   :   liujiquan
//* DateTime :   7/21/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __SETTINGDIALOG_H_2459134951B94A1895E650CFD52F4215__
#define __SETTINGDIALOG_H_2459134951B94A1895E650CFD52F4215__
#pragma once

#include "SettingPage.h"
class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)
public:
	CSettingDialog(CWnd* pParent = NULL);
	CSettingDialog(int nTabPage, CWnd* pParent = NULL);   
	virtual ~CSettingDialog();
	enum { IDD = IDD_DIALOG_SETTING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
private:
	HICON	m_hIcon;
	int			m_nTabPage;				// Page Index
	CSettingPage	m_pPage[TABPAGECOUNT];		// All Page
	CTabCtrl		m_TabCtrl;
// 2015.07.18 Ver1.00[International] Start
	CComboBox	m_LanaguageCmb;
	int			m_nLastLanaguage;
// 2015.07.18 Ver1.00[International] End
public:
	void OnSetTabPage(int nTabPage){ m_nTabPage = nTabPage; }
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};

#endif//__SETTINGDIALOG_H_2459134951B94A1895E650CFD52F4215__
