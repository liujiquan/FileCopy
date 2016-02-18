#include "stdafx.h"
#include "FileCopy.h"
#include "ListBoxExt.h"
#include "Util.h"

// CListBoxExt
IMPLEMENT_DYNAMIC(CListBoxExt, CListBox)
CListBoxExt::CListBoxExt()
{
	m_bInsert = FALSE;
	m_InsertIndex = -1;
	m_bCreate = FALSE;

	GdiplusStartupInput gidplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gidplusStartupInput, NULL);
}
CListBoxExt::~CListBoxExt()
{
	GdiplusShutdown(m_gdiplusToken);
}
BEGIN_MESSAGE_MAP(CListBoxExt, CListBox)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(InsertEditCtlID, &CListBoxExt::OnEditKillFocus)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()
// CListBoxExt message handlers

void CListBoxExt::OnInitEdit()
{
	if (m_bCreate == FALSE)
	{
		m_InsertEdit.CreateEx(
			0,
			_T("EDIT"), NULL, 
			WS_CHILD|WS_VISIBLE|WS_BORDER, 
			CRect(0, 0, 0, 0), 
			this, 
			InsertEditCtlID);
		m_InsertEdit.SetFont(GetFont());
		m_InsertEdit.ShowWindow(SW_HIDE);
		m_bCreate = TRUE;
	}
}

int CListBoxExt::AddString(LPCTSTR lpszName, LPCTSTR lpszItem, BOOL bEdit/* = FALSE*/)
{
	// 1-AddString
	int nIndex = CListBox::AddString(lpszItem);
	if (nIndex == -1)
		return -1;
	// 2-malloc
	ListBoxData* data = (ListBoxData*)malloc(sizeof(ListBoxData));
	if (data == NULL)
	{
		DeleteString(nIndex);
		return -1;
	}
	memset(data, 0x00, sizeof(ListBoxData));

	// 3-Init
	data->bEdit = bEdit;
	memcpy(data->szName, lpszName, _tcslen(lpszName) * sizeof(TCHAR));
	memcpy(data->szData, lpszItem, _tcslen(lpszItem) * sizeof(TCHAR));
	// 4-SetItemDataPtr
	SetItemDataPtr(nIndex, data);
// 2015.07.24 Ver1.00 [HScrollBar] Start
	// 5-HScrollBar
	RefushHorizontalScrollBar();
// 2015.07.24 Ver1.00 [HScrollBar] End

	return nIndex;
}

int CListBoxExt::DeleteString(UINT nIndex)
{
	// 1-Free
	void* data = GetItemDataPtr(nIndex);
	free(data);
	data = NULL;
	SetItemDataPtr(nIndex, NULL);

	// 2-DeleteString
	int index = CListBox::DeleteString(nIndex);
	if (index == -1)
		return -1;
	
	// 3-Set
	if (nIndex < GetCount())
		SetCurSel(nIndex);
// 2015.07.24 Ver1.00 [HScrollBar] Start
	// 4-HScrollBar
	RefushHorizontalScrollBar();
// 2015.07.24 Ver1.00 [HScrollBar] End

	return index;
}

int CListBoxExt::InsertString(int index, LPCTSTR lpszName, LPCTSTR lpszItem, BOOL bEdit/* = FALSE*/)
{
	// 1-InsertString
	int nIndex = CListBox::InsertString(index, lpszItem);
	if (nIndex == -1)
		return -1;
	// 2-malloc
	ListBoxData* data = (ListBoxData*)malloc(sizeof(ListBoxData));
	if (data == NULL)
	{
		DeleteString(nIndex);
		return -1;
	}
	memset(data, 0x00, sizeof(ListBoxData));

	// 3-Init
	data->bEdit = bEdit;
	memcpy(data->szName, lpszName, _tcslen(lpszName) * sizeof(TCHAR));
	memcpy(data->szData, lpszItem, _tcslen(lpszItem) * sizeof(TCHAR));
	// 4-SetItemDataPtr
	SetItemDataPtr(nIndex, data);

// 2015.07.24 Ver1.00 [HScrollBar] Start
	// 5-HScrollBar
	RefushHorizontalScrollBar();
// 2015.07.24 Ver1.00 [HScrollBar] End

	return nIndex;
}

void CListBoxExt::Release()
{
	for(int nIndex = GetCount()- 1; nIndex >= 0; nIndex--)
	{
		void* data = GetItemDataPtr(nIndex);
		if (data)
		{
			free(data);
			data = NULL;
		}
	}
}

long CListBoxExt::OnAppendString()
{
	if (m_bInsert)
		return 1;
	// 1.AddString
	m_InsertIndex = InsertString(0, _T(""), _T(""), TRUE);
	if (m_InsertIndex == -1)
		return -1;
	m_bInsert = TRUE;

	return 0;
}

CString	CListBoxExt::GetLBText(int nIndex)
{
	CString strText;
	ListBoxData* data = (ListBoxData*)GetItemDataPtr(nIndex);
	if (data)
	{
		return CString(data->szData);
	}

	return strText;
}

int CListBoxExt::SetText(int index, CString strText, BOOL bEdit/* = FALSE*/)
{
	if(index == -1)
		return -1;

	ListBoxData* data = (ListBoxData*)GetItemDataPtr(index);
	if(data == NULL)
		return -1;

	{
		data->bEdit = bEdit;
		memset(data->szData, 0x00, MAX_PATH);
		memcpy(data->szName, strText.GetBuffer(0), strText.GetLength() * sizeof(TCHAR));
		CString strInfo = GetFileTypeInfo(TYPEHEADER + strText);
		memcpy(data->szData, strInfo.GetBuffer(0), strInfo.GetLength() * sizeof(TCHAR));
	}
// 2015.07.24 Ver1.00 [HScrollBar] Start
	// 5-HScrollBar
	RefushHorizontalScrollBar();
// 2015.07.24 Ver1.00 [HScrollBar] End

	return index;
}

void CListBoxExt::OnEditKillFocus()
{
	if (m_InsertIndex == -1 || m_bInsert == FALSE)
		return;

	int nIndex = GetCurSel();

	if (m_bInsert && nIndex != m_InsertIndex)	// Click Other
	{
		CString strText;
		m_InsertEdit.GetWindowText(strText);
		m_InsertEdit.ShowWindow(SW_HIDE);
		if (strText.IsEmpty())
		{
			DeleteString(m_InsertIndex);
		}
		else
		{
			SetText(m_InsertIndex, strText, FALSE);
		}
		
		m_bInsert = FALSE;
		m_InsertIndex = -1;
		Invalidate();
	}
}

// -----------------------------------------------------------//
// Function :   CListBoxExt::DrawItem
// Param    :   LPDRAWITEMSTRUCT lpDrawItemStruct
// Return   :   void
// Comment  :   DrawItem
// -----------------------------------------------------------//
void CListBoxExt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (GetCount() <= lpDrawItemStruct->itemID)
	{
		return;
	}
	ASSERT(lpDrawItemStruct->CtlType & ODT_LISTBOX);
	OnInitEdit();

	CRect rectItem(lpDrawItemStruct->rcItem);
	CRect rectItemText(rectItem);
	rectItemText.InflateRect(0, -2, 0, -2);

	COLORREF oldTextColor = ::GetTextColor(lpDrawItemStruct->hDC);

	Gdiplus::Graphics graphics(lpDrawItemStruct->hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	// Fill Back
	if ((lpDrawItemStruct->itemAction | ODA_SELECT)	&& 	(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		Gdiplus::SolidBrush brush(ConvertColor(::GetSysColor(COLOR_HIGHLIGHT)));
		graphics.FillRectangle(&brush, ConvertRect(rectItemText));
		SetTextColor(lpDrawItemStruct->hDC, ::GetSysColor(COLOR_HIGHLIGHTTEXT));	// selected
	}
	else
	{
		Gdiplus::SolidBrush brush(ConvertColor(::GetBkColor(lpDrawItemStruct->hDC)));
		graphics.FillRectangle(&brush, ConvertRect(rectItem));
	}

	// Draw Text
	// Data
	ListBoxData* data = (ListBoxData*)GetItemDataPtr(lpDrawItemStruct->itemID);
	if (data == NULL/* || data->nIndex != lpDrawItemStruct->itemID */)
	{
		return;
	}

	Gdiplus::Pen pen(LINECOLOR, LINESIZE);
	Gdiplus::Font strfont(lpDrawItemStruct->hDC);
	Gdiplus::StringFormat stringformat;
	Gdiplus::SolidBrush stringbrush( ConvertColor(::GetTextColor(lpDrawItemStruct->hDC) ));
	stringformat.SetAlignment(StringAlignmentNear);
	stringformat.SetFormatFlags(StringFormatFlagsNoWrap);

	if (data->bEdit)
	{
		m_InsertEdit.SetWindowTextW(data->szData);
		//GetScrollInfo()
//		m_InsertEdit.MoveWindow(rectItemText, FALSE);
		m_InsertEdit.SetWindowPos(NULL, 
			rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), FALSE);
		m_InsertEdit.ShowWindow(SW_SHOW);
		m_InsertEdit.SetFocus();
	}
	else
	{
//		if (lpDrawItemStruct->itemData)
//			graphics.DrawString((LPCTSTR)lpDrawItemStruct->itemData,-1, &strfont,ConvertRect(rectItem),	&stringformat,&stringbrush);
//		graphics.DrawString(data->szData, -1, &strfont, ConvertRect(rectItemText), &stringformat, &stringbrush);
		Gdiplus::RectF nameRectF;
		Gdiplus::RectF dataRectF;
		ConvertRect(rectItemText, &nameRectF, &dataRectF);
		
		CString strName(data->szName);
		if (strName.GetLength() >= 2 && strName.Left(2) == TYPEHEADER)
		{
			strName = strName.Right(strName.GetLength() - 2);
		}
		
		graphics.DrawString(strName, -1, &strfont, nameRectF, &stringformat, &stringbrush);
		graphics.DrawString(data->szData, -1, &strfont, dataRectF, &stringformat, &stringbrush);
	
		graphics.DrawLine(&pen, Point(rectItemText.left, rectItemText.bottom), Point(rectItemText.right, rectItemText.bottom));
		graphics.DrawLine(&pen, Point(nameRectF.X + nameRectF.Width, rectItemText.top), Point(nameRectF.X + nameRectF.Width, rectItemText.bottom));
	}

	SetTextColor(lpDrawItemStruct->hDC, oldTextColor);
}

BOOL CListBoxExt::PreTranslateMessage(MSG* pMsg)
{
	HACCEL haccel = LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_LISTBOX));
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (haccel && TranslateAccelerator(m_hWnd, haccel, pMsg))
		{
			if (pMsg->wParam == VK_DELETE)			// delete
			{
				int nIndex = GetCurSel();
				if (nIndex != -1)
				{
					DeleteString(nIndex);
					if (nIndex  < GetCount())
					{
						SetCurSel(nIndex);
					}
				}
			}
			else if (pMsg->wParam == VK_RETURN)		// enter
			{
				OnEditKillFocus();

				return TRUE;
			}
		}
	}

	return CListBox::PreTranslateMessage(pMsg);
}


void CListBoxExt::OnDestroy()
{
	Release();
	CListBox::OnDestroy();
}

// 2015.07.24 Ver1.00 [HScrollBar] Start
// -----------------------------------------------------------//
// Function :   CListBoxExt::RefushHorizontalScrollBar
// Param    :   void
// Return   :   void
// Comment  :   
// -----------------------------------------------------------//
void CListBoxExt::RefushHorizontalScrollBar()
{
	CDC *pDC = this->GetDC();
	if ( NULL == pDC )
	{
		return;
	}

	int nCount = this->GetCount();
	if ( nCount < 1 )
	{
		this->SetHorizontalExtent( 0 );
		return;
	}

	int nMaxExtent = 0;
	CRect rect0;
	for ( int i = 0; i < nCount; ++i )
	{
		// width0 + width1
		ListBoxData* data = (ListBoxData*)GetItemDataPtr(i);
		if (data)
		{
			CSize &cs = pDC->GetTextExtent( CString(data->szData) );
			if ( cs.cx > nMaxExtent )
			{
				nMaxExtent = cs.cx + NAME_WID;
			}
		}
	}

	nMaxExtent += ADDWIDTH;		// Add

	this->SetHorizontalExtent( nMaxExtent );

	 ReleaseDC(pDC);
}

void CListBoxExt::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Invalidate();
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
}
// 2015.07.24 Ver1.00 [HScrollBar] End

Gdiplus::Color ConvertColor(COLORREF RGBcolor)
{
	Gdiplus::Color color;
	color.SetFromCOLORREF(RGBcolor);
	return color;
}

Gdiplus::RectF ConvertRect(RECT rect)
{
	Gdiplus::RectF rectF(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	return  rectF;
}

Gdiplus::RectF ConvertRect(CRect rect)
{
	Gdiplus::RectF rectF(rect.left, rect.top, rect.Width(), rect.Height());

	return  rectF;
}

void ConvertRect(CRect rect, Gdiplus::RectF* nameRectF, Gdiplus::RectF* dataRectF)
{
	if (nameRectF == NULL || dataRectF == NULL)
		return;

	nameRectF->X = rect.left;
	nameRectF->Y = rect.top;
// 2015.07.24 Ver1.00 [HScrollBar] Start
//	nameRectF->Width = (rect.Width() - LINESIZE) * NAME_WID;
	nameRectF->Width = NAME_WID;
// 2015.07.24 Ver1.00 [HScrollBar] End
	nameRectF->Height = rect.Height();

// 2015.07.24 Ver1.00 [HScrollBar] Start
//	dataRectF->X = rect.left + nameRectF->Width + LINESIZE;
	dataRectF->X = rect.left + NAME_WID + LINESIZE;
// 2015.07.24 Ver1.00 [HScrollBar] End
	dataRectF->Y = rect.top;
// 2015.07.24 Ver1.00 [HScrollBar] Start
//	dataRectF->Width = (rect.Width() - LINESIZE) * DATA_WID;
	dataRectF->Width = DATA_WID(rect.Width());
// 2015.07.24 Ver1.00 [HScrollBar] End
	dataRectF->Height = rect.Height();
}