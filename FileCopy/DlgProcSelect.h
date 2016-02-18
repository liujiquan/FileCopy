#pragma once

// CDlgProcSelect
class CDlgProcSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProcSelect)
public:
	CDlgProcSelect(CWnd* pParent = NULL);
	virtual ~CDlgProcSelect();
	enum { IDD = IDD_DIALOG_PROCSELECT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
private:
	int m_nTabPage;
public:
	int GetTabPage(){	return m_nTabPage;	}
	virtual BOOL OnInitDialog();
	CListBox m_RootList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkList1();
// 2015.07.18 Ver1.00[International] Start
	void			OnInitResourceLanguage();
// 2015.07.18 Ver1.00[International] End
};
