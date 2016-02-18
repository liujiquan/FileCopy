#pragma once
// CListBoxExt
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#define InsertEditCtlID	 WM_USER + 1
// 2015.07.24 Ver1.00 [HScrollBar] Start
//#define NAME_WID	1 / 4
//#define DATA_WID	3 / 4
#define NAME_WID	80
#define DATA_WID(width)	(width - LINESIZE - NAME_WID)
#define ADDWIDTH	20
// 2015.07.24 Ver1.00 [HScrollBar] End
#define LINESIZE	1.0
#define LINECOLOR	Color(255, 230, 230, 230)
typedef struct ListBoxData{
	TCHAR	szName[MAX_PATH];		// name
	TCHAR	szData[MAX_PATH];		// text
	BOOL	bEdit;					// CEdit
	ListBoxData()
	{
		memset(szName, 0x00, MAX_PATH);
		memset(szData, 0x00, MAX_PATH);
		bEdit = FALSE;
	}
}ListBoxData;
class CListBoxExt : public CListBox
{
	DECLARE_DYNAMIC(CListBoxExt)
public:
	CListBoxExt();
	virtual ~CListBoxExt();
private:
	BOOL		m_bInsert;
	int			m_InsertIndex;
	BOOL		m_bCreate;
	CEdit		m_InsertEdit;

	ULONG_PTR	m_gdiplusToken;
private:
	void		OnInitEdit();
public:
	long		OnAppendString();
	int			SetText(int index, CString strText, BOOL bEdit = FALSE);
	CString		GetLBText(int nIndex);
protected:
	DECLARE_MESSAGE_MAP()
public:
	void	Release();
	int		AddString(LPCTSTR lpszName, LPCTSTR lpszItem, BOOL bEdit = FALSE);
	int		DeleteString(UINT nIndex);
	int		InsertString(int nIndex, LPCTSTR lpszName, LPCTSTR lpszItem, BOOL bEdit = FALSE);
	void	OnEditKillFocus();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);			// DrawItem
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 2015.07.24 Ver1.00 [HScrollBar] Start
	void	RefushHorizontalScrollBar();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
// 2015.07.24 Ver1.00 [HScrollBar] End
};

Gdiplus::Color ConvertColor(COLORREF color);
Gdiplus::RectF ConvertRect(RECT rect);
Gdiplus::RectF ConvertRect(CRect rect);
void ConvertRect(CRect rect, Gdiplus::RectF* nameRectF, Gdiplus::RectF* dataRectF);