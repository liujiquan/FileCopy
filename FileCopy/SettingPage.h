//*-----------------------------------------------------------------------------*/
//* Copyright(C) 2015, liujiquan Company All rights reserved. )
//* FileName :   DialogSetting.h
//* Author   :   liujiquan
//* DateTime :   7/9/2015
//* Version  :   1.0
//* Comment  :   
//*-----------------------------------------------------------------------------*/
#ifndef __SETTINGPAGE_H_2459134951B94A1895E650CFD52F4215__
#define __SETTINGPAGE_H_2459134951B94A1895E650CFD52F4215__
#pragma once

#include "afxwin.h"
#include "Util.h"
#include "ListBoxExt.h"
//////////////////////////////////////////////////////////////////////
// CSettingPage dialog
class CSettingPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingPage)
public:
	CSettingPage(CWnd* pParent = NULL);
	CSettingPage(int nTabPage, CWnd* pParent = NULL);
	virtual ~CSettingPage();
// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING_PAGE };
protected:
	HICON	m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpenSource();
	afx_msg void OnBnClickedOpenTarget();
	afx_msg void OnBnClickedBtnAddfilter();
	afx_msg void OnBnClickedBtnDelfilter();
	afx_msg void OnBnClickedBtnDefaultFilter();
private:
	int				m_nTabPage;			// Page Index

	CString			m_strSourcePath;		// 開発環境環境
	CString			m_strTargetPath;		// 評価環境	
	BOOL			m_AutoRefresh;			// プログラムの起動時の自動検索
	CListBoxExt		m_Filterlist;			// フィルタ規則
	CStringArray	m_strDefaultfilterArr;	// default フィルタ規則
public:
	void OnSetTabPage(int nTabPage){ m_nTabPage = nTabPage; }
	void OnAplly();
};
#endif//__SETTINGPAGE_H_2459134951B94A1895E650CFD52F4215__