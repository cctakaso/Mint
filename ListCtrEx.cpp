// ListVwEx.cpp : implementation of the CListCtrEx class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ListCtrEx.h"
//#include "Cell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrEx

IMPLEMENT_DYNCREATE(CListCtrEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrEx)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(LVM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(LVM_SETTEXTCOLOR, OnSetTextColor)
	ON_MESSAGE(LVM_SETTEXTBKCOLOR, OnSetTextBkColor)
	ON_MESSAGE(LVM_SETBKCOLOR, OnSetBkColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrEx construction/destruction

CListCtrEx::CListCtrEx()
{
	m_bLockUpdate = FALSE;
	m_maxColumn = 0;
	m_bDirty = FALSE;
	Clear();
}

CListCtrEx::~CListCtrEx()
{
}


void CListCtrEx::Clear()
{
	if (::IsWindow(GetSafeHwnd()) && !m_bLockUpdate)
		LockWindowUpdate();

	m_bStateIcons = TRUE;
	m_nCheckedItem = -1;
	m_hSmallIcon = NULL;
	m_hLargeIcon = NULL;
	
	m_selType = st_normal;
	m_selEdit = FALSE;
	m_selIn = FALSE;
	m_selCol = 0;
	m_selColMac = -1;
	m_selRow = -1;
	m_bRGBColor = FALSE;
	m_bMultiCheck = FALSE;

	m_bClientWidthSel = TRUE;

	m_cxClient = 0;
	m_cxStateImageOffset = 0;

	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);

	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_brTextBk.Detach();
	m_brTextBk.CreateSolidBrush(m_clrTextBk);
	VERIFY(UnrealizeObject(m_brTextBk));

	m_clrBkgnd = RGB(128,128,128);//::GetSysColor(COLOR_WINDOW);
	m_brBkgnd.Detach();
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	VERIFY(UnrealizeObject(m_brBkgnd));

	m_clrGray = RGB(192,192,192);
	m_brGray.Detach();
	m_brGray.CreateSolidBrush(m_clrGray);
	VERIFY(UnrealizeObject(m_brGray));

	m_clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
	m_brHighlight.Detach();
	m_brHighlight.CreateSolidBrush(m_clrHighlight);
	VERIFY(UnrealizeObject(m_brHighlight));

	
	if (::IsWindow(GetSafeHwnd())) {
		DeleteAllItems();
		while(m_maxColumn>0){
			DeleteColumn(--m_maxColumn);
		}
	}
	m_maxColumn = 0;
//	m_cellTbl.RemoveAll();

	if (::IsWindow(GetSafeHwnd()) && !m_bLockUpdate)
		UnlockWindowUpdate();
}

void CListCtrEx::LockUpdate(BOOL bLockUpdate)
{
	if (m_bLockUpdate = bLockUpdate) {
		if (::IsWindow(GetSafeHwnd()))
			LockWindowUpdate();
	} else {
		if (::IsWindow(GetSafeHwnd())) {
			UnlockWindowUpdate();
			Invalidate();
			//UpdateWindow(); this is bug
		}
	}
}

/*
BOOL CListCtrEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// default is report view and full row selection
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT  | LVS_SHOWSELALWAYS ;
	if (m_selType != st_normal)
		cs.style |= LVS_OWNERDRAWFIXED;

	return(CListCtrl::PreCreateWindow(cs));
}
*/
BOOL CListCtrEx::SetSelType(DWORD type, eSelType selType, BOOL bEdit, BOOL bRGBColor, BOOL bMultiCheck)
{
	if (GetViewType()!=type) {
		if (!SetViewType(type))
			return FALSE;
	}

	if (selType<=st_first || selType>=st_end)
		return FALSE;

	// no painting during change
	if (!m_bLockUpdate)
		LockWindowUpdate();

	BOOL bRet = TRUE;

	if (bEdit) {
	//	if (bEdit) { //cellをEdit使わない
		if (!(GetStyle() & LVS_EDITLABELS))
			bRet = ModifyStyle(0L, LVS_EDITLABELS);
	}else{
		if (GetStyle() & LVS_EDITLABELS)
			bRet = ModifyStyle(LVS_EDITLABELS, 0);
	}
	bRet = ModifyStyle(0L, LVS_REPORT | LVS_SHOWSELALWAYS);

	if (selType != st_normal) {
		if (!(GetStyle() & LVS_OWNERDRAWFIXED))
			bRet = ModifyStyle(0L, LVS_OWNERDRAWFIXED);
	}else{
		if (GetStyle() & LVS_OWNERDRAWFIXED)
			bRet = ModifyStyle(LVS_OWNERDRAWFIXED, 0L);
	}

	// repaint window if we are not changing view type
	if (bRet && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		Invalidate();

	if (bRet) {
		m_selType = selType;
		m_selCol = 0;
		m_selEdit = bEdit;
		//m_selEdit = FALSE;
		m_bRGBColor = bRGBColor;
		m_bMultiCheck = bMultiCheck;
	}

	// repaint changes
	if (!m_bLockUpdate)
		UnlockWindowUpdate();

	return(bRet);
}


/////////////////////////////////////////////////////////////////////////////
// CListCtrEx drawing

// offsets for first and other columns
#define OFFSET_FIRST	2
#define OFFSET_OTHER	6
inline void CListCtrEx::SetSelOfSelection(CDC *&pDC, CRect &rcLabel)
{
	pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->SetBkColor(m_clrHighlight);
	pDC->FillRect(rcLabel, &m_brHighlight);
	//pDC->RestoreDC(-1);
	//pDC->SaveDC();
};
void CListCtrEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	UINT uiFlags = ILD_TRANSPARENT;
	CImageList* pImageList;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	//COLORREF clrTextSave, clrBkSave;
	COLORREF clrImage = GetSysColor(COLOR_WINDOW);//m_clrBkgnd;
	static _TCHAR szBuff[MAX_PATH];
	LPCTSTR pszText;

//	AfxTrace("DrawItem(%d)\n", nItem);

	pDC->SaveDC();
// get item data

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	BOOL bSelected = (bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

// set colors if item is selected
	if (bSelected)
		m_selRow = nItem;

	CRect rcAllLabels;
	CListCtrl::GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

	CRect rcLabel;
	CListCtrl::GetItemRect(nItem, rcLabel, LVIR_LABEL);

	rcAllLabels.left = rcLabel.left;
	if (m_bClientWidthSel && rcAllLabels.right<m_cxClient)
		rcAllLabels.right = m_cxClient;

	if (bSelected && m_selType==st_full) {
		SetSelOfSelection(pDC, rcItem/*rcAllLabels*/);
	}else{
		if (m_selType==st_full) {
			pDC->FillRect(rcItem, &m_brTextBk);
		} else {
			CRect rc = rcItem;
			rc.left = rcAllLabels.left;
			pDC->FillRect(rc, &m_brTextBk);
		}
	}

// set color and mask for the icon

	if (lvi.state & LVIS_CUT)
	{
		clrImage = GetSysColor(COLOR_WINDOW);//m_clrBkgnd;
		uiFlags |= ILD_BLEND50;
	}
	else if (bSelected && m_selType==st_full)
	{
		clrImage = m_clrHighlight;
		uiFlags |= ILD_BLEND50;
	}

// draw state icon

	UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
	if (nStateImageMask)
	{
		int nImage = (nStateImageMask>>12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
		}
	}

// draw normal and overlay icon

	CRect rcIcon;
	CListCtrl::GetItemRect(nItem, rcIcon, LVIR_ICON);

	if (m_bRGBColor) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(nItem);
		CBrush br(p->color);
		rcIcon.top++;
		rcIcon.bottom--;
		pDC->FillRect(&rcIcon, &br);
	}else{
		pImageList = GetImageList(LVSIL_SMALL);
		if (pImageList)
		{
			UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
			if (rcItem.left<rcItem.right-1)
			{
				ImageList_DrawEx(pImageList->m_hImageList, lvi.iImage,
						pDC->m_hDC,rcIcon.left,rcIcon.top, m_cxStateImageOffset/*16*/, 16,
						GetSysColor(COLOR_WINDOW)/*m_clrBkgnd*/,
						clrImage, uiFlags | nOvlImageMask);
			}
		}
	}

// draw item label

	CListCtrl::GetItemRect(nItem, rcItem, LVIR_LABEL);
#ifdef BUG
	rcItem.right -= m_cxStateImageOffset;
#endif
	pszText = MakeShortString(pDC, szBuff,
				rcItem.right-rcItem.left, 2*OFFSET_FIRST);

	rcLabel = rcItem;
	rcLabel.left += OFFSET_FIRST;
	rcLabel.right -= OFFSET_FIRST;

	BOOL bEdit = m_selType!=st_grid /*|| m_cellTbl.GetAt(nItem, 0).m_bEdit*/;
	if (!bEdit && m_selEdit) {
		CRect rc = GetItemRect(nItem, 0);
		pDC->FillRect(rc, &m_brGray);
	}
	if (bSelected && m_selType==st_grid && m_selCol==0 && !m_selIn) {
		if (bEdit)
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		else
			pDC->SetTextColor(m_clrGray);
		pDC->SetBkColor(m_clrHighlight);
		pDC->FillRect(rcLabel, &m_brHighlight);
		pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
		pDC->RestoreDC(-1);
		pDC->SaveDC();
	}
	else
	pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
				   
	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		bEdit = m_selType!=st_grid /*|| m_cellTbl.GetAt(nItem, nColumn).m_bEdit*/;
		if (!bEdit && m_selEdit) {
			pDC->FillRect(rcItem, &m_brGray);
		}
		int nRetLen = CListCtrl::GetItemText(nItem, nColumn,
						szBuff, sizeof(szBuff));
		if (nRetLen == 0)
			continue;

		pszText = MakeShortString(pDC, szBuff,
			rcItem.right - rcItem.left, 2*OFFSET_OTHER);

		UINT nJustify = DT_LEFT;

		if(pszText == szBuff)
		{
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify = DT_CENTER;
				break;
			default:
				break;
			}
		}

		rcLabel = rcItem;
		rcLabel.left += OFFSET_OTHER;
		rcLabel.right -= OFFSET_OTHER;

		if (bSelected && m_selType==st_grid && m_selCol==nColumn && !m_selIn) {
			CRect rc=rcLabel;
			rc.left -= OFFSET_OTHER;
			rc.right += OFFSET_OTHER;
			if (bEdit)
				pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			else
				pDC->SetTextColor(m_clrGray);
			pDC->SetBkColor(m_clrHighlight);
			pDC->FillRect(rc, &m_brHighlight);
			pDC->DrawText(pszText, -1, rcLabel,
				nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
			pDC->RestoreDC(-1);
			pDC->SaveDC();
		}
		else
		pDC->DrawText(pszText, -1, rcLabel,
			nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	}

// draw focus rectangle if item has focus

	if (lvi.state & LVIS_FOCUSED && bFocus)
		pDC->DrawFocusRect(rcAllLabels);

// set original colors if item was selected
	pDC->RestoreDC(-1);
	//if (bSelected && m_selType==st_full)
	//{
    //    pDC->SetTextColor(clrTextSave);
	//	pDC->SetBkColor(clrBkSave);
	//}
}

LPCTSTR CListCtrEx::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static string svstrShort;
	string str;
	//lstrcpy(svstrShort,lpszLong);
	str = lpszLong;

	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;
	int iDbcs = -10;
	int i;
    for(i = 0; i<str.size(); i++)
	{
		if((pDC->GetTextExtent(str.data(), i).cx + nOffset + nAddLen)
			>= nColumnLen)
		{
			if (--i==iDbcs)
				i--;
			break;
		}
		if (IsSJIS_1stByte(str[i])) {
			i++;
			iDbcs = i;
		}
	}

	if (i<=0)
		svstrShort = "";
	else{
		//svstrShort = str.substr(0,i);
		svstrShort = "";
		svstrShort.append(str.data(), i);
	}

	svstrShort += szThreeDots;
	//svstrShort.append(szThreeDots);
	return(svstrShort.data());
}

void CListCtrEx::RepaintSelectedItems()
{
	CRect rcItem, rcLabel;

	if (m_bLockUpdate)
		return;
// invalidate focused item so it can repaint properly

	int nItem = GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
		CListCtrl::GetItemRect(nItem, rcItem, LVIR_BOUNDS);
		CListCtrl::GetItemRect(nItem, rcLabel, LVIR_LABEL);
		rcItem.left = rcLabel.left;

		InvalidateRect(rcItem, FALSE);
	}

// if selected items should not be preserved, invalidate them

	if(!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = GetNextItem(-1, LVNI_SELECTED);
			nItem != -1; nItem = GetNextItem(nItem, LVNI_SELECTED))
		{
			CListCtrl::GetItemRect(nItem, rcItem, LVIR_BOUNDS);
			CListCtrl::GetItemRect(nItem, rcLabel, LVIR_LABEL);
			rcItem.left = rcLabel.left;

			InvalidateRect(rcItem, FALSE);
		}
	}

// update changes 

	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrEx diagnostics

#ifdef _DEBUG

void CListCtrEx::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CListCtrEx::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);

	dc << "m_selType = " << (int)m_selType;
	dc << "\n";
	dc << "m_cxStateImageOffset = " << m_cxStateImageOffset;
	dc << "\n";
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListCtrEx message handlers

LRESULT CListCtrEx::OnSetImageList(WPARAM wParam, LPARAM lParam)
{
	if( (int) wParam == LVSIL_STATE)
	{
		int cx, cy;

		if(::ImageList_GetIconSize((HIMAGELIST)lParam, &cx, &cy))
			m_cxStateImageOffset = cx;
		else
			m_cxStateImageOffset = 0;
	}

	return(Default());
}

LRESULT CListCtrEx::OnSetTextColor(WPARAM wParam, LPARAM lParam)
{
	m_clrText = (COLORREF)lParam;
	return(Default());
}

LRESULT CListCtrEx::OnSetTextBkColor(WPARAM wParam, LPARAM lParam)
{
	m_clrTextBk = (COLORREF)lParam;
	m_brTextBk.Detach();
	m_brTextBk.CreateSolidBrush(m_clrTextBk);
	VERIFY(UnrealizeObject(m_brTextBk));
	return(Default());
}

LRESULT CListCtrEx::OnSetBkColor(WPARAM wParam, LPARAM lParam)
{
	m_clrBkgnd = (COLORREF)lParam;
	m_brBkgnd.Detach();
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	VERIFY(UnrealizeObject(m_brBkgnd));
	return(Default());
}

void CListCtrEx::OnSize(UINT nType, int cx, int cy) 
{
	m_cxClient = cx;
	CListCtrl::OnSize(nType, cx, cy);
}

void CListCtrEx::OnPaint() 
{
	// in full row select mode, we need to extend the clipping region
	// so we can paint a selection all the way to the right
	if (m_bClientWidthSel &&
		(GetStyle() & LVS_TYPEMASK) == LVS_REPORT &&
		m_selType!=st_normal)
	{
		CRect rcAllLabels;
		CListCtrl::GetItemRect(0, rcAllLabels, LVIR_BOUNDS);

		if(rcAllLabels.right < m_cxClient)
		{
			// need to call BeginPaint (in CPaintDC c-tor)
			// to get correct clipping rect
			CPaintDC dc(this);

			CRect rcClip;
			dc.GetClipBox(rcClip);

			rcClip.left = min(rcAllLabels.right-1, rcClip.left);
			rcClip.right = m_cxClient;

			InvalidateRect(rcClip, FALSE);
			// EndPaint will be called in CPaintDC d-tor
		}
	}

	CListCtrl::OnPaint();
}

void CListCtrEx::OnSetFocus(CWnd* pOldWnd) 
{
	//AfxTrace("SetFocus()\n");
	CListCtrl::OnSetFocus(pOldWnd);

//	SetEdit(FALSE);

	// check if we are getting focus from label edit box
	if(pOldWnd!=NULL && pOldWnd->GetParent()==this)
		return;

	// repaint items that should change appearance
	if(m_selType != st_normal && (GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
		RepaintSelectedItems();
}

void CListCtrEx::OnKillFocus(CWnd* pNewWnd) 
{
	//AfxTrace("KillFocus()\n");
	CListCtrl::OnKillFocus(pNewWnd);

	// check if we are losing focus to label edit box
	if(pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if(m_selType != st_normal && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}
/////////////////////////////////////////////////////////////////////////////
// CListCtrEx helpers

BOOL CListCtrEx::SetImageLists(CImageList* pImageListSmall, CImageList* pImageListNormal, CImageList* pImageListState)
{
	if (pImageListSmall)
		SetImageList(pImageListSmall, LVSIL_SMALL);
	if (pImageListNormal)
		SetImageList(pImageListNormal, LVSIL_NORMAL);
	if (pImageListState)
		SetImageList(pImageListState, LVSIL_STATE);

	return TRUE;
}

BOOL CListCtrEx::AddColumn(LPCTSTR strItem,int nItem,int nFmt, int bEdit, int nMask,int nSubItem)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = GetStringWidth(lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	if (InsertColumn(nItem,&lvc)!=-1) {
		m_maxColumn ++;
		if(nMask & LVCF_SUBITEM)
			m_selColMac = lvc.iSubItem;
		//CCell cell(bEdit);
		//if (!m_cellTbl.SetAtExpand(0,nItem,cell))
		//	return FALSE;
		return TRUE;
	}else
		return FALSE;
}


BOOL CListCtrEx::AddItem(int nItem,int nSubItem,LPCTSTR strItem, int bEdit, int nImageIndex, int nState, void *pdata)
{
	BOOL bRtn;
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1 || m_bRGBColor){
		lvItem.mask |= LVIF_IMAGE;
		if (m_bRGBColor)
			lvItem.iImage = 0;
		else
			lvItem.iImage = nImageIndex;
	}
	if (nState != -1) {
		lvItem.mask |= LVIF_STATE;
		lvItem.state = nState==0 ? INDEXTOSTATEIMAGEMASK(1):INDEXTOSTATEIMAGEMASK(2);
	}
	if(nSubItem == 0) {
		if (InsertItem(&lvItem)==-1)
			return FALSE;
	}
	bRtn = SetItem(&lvItem);
	if (bRtn && nSubItem == 0 /*&& m_bRGBColor*/) {
		COLOR_ITEM_DATA *p = new COLOR_ITEM_DATA;
		p->color = INT2COLORREF(nImageIndex);
		p->pdata = pdata;
		SetItemData(nItem, (unsigned long)p);
	}
	//for COLOR_ITEM_DATA.aryStr
	SetItemText(nItem, nSubItem, strItem);
		//CCell cell(FALSE); //CCell cell(bEdit);
		//if (!m_cellTbl.SetAtExpand(nItem, nSubItem, cell))
		//	return FALSE;
	return bRtn;
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrEx diagnostics


/////////////////////////////////////////////////////////////////////////////
// CListCtrEx helpers

BOOL CListCtrEx::SetViewType(DWORD dwViewType)
{
	return(ModifyStyle(LVS_TYPEMASK,dwViewType & LVS_TYPEMASK));
}

DWORD CListCtrEx::GetViewType()
{
	return(GetStyle() & LVS_TYPEMASK);
}

void CListCtrEx::CheckItem(int nItem)
{
// reset if there is checked item
	if (m_bMultiCheck) {
		UINT val = GetItemState(nItem,
										LVIS_STATEIMAGEMASK);
		if (val==INDEXTOSTATEIMAGEMASK(1) || val==INDEXTOSTATEIMAGEMASK(2)) {
			SetItemState(nItem,
				val==INDEXTOSTATEIMAGEMASK(1) ? INDEXTOSTATEIMAGEMASK(2):INDEXTOSTATEIMAGEMASK(1)
				, LVIS_STATEIMAGEMASK);
		}
	}
	else if (m_nCheckedItem != -1)
	{
		SetItemState(m_nCheckedItem,
			INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
		::DestroyIcon(m_hSmallIcon);
		SetItemState(m_nCheckedItem,
				INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
		m_nCheckedItem = nItem;
	}
	m_bDirty = TRUE;
}

COLORREF IncColorref(COLORREF color, bool bInc, int mod=2)
{
	COLORREF svColors[] = {
	RGB(0,0,0),
	RGB(0,0,128),
	RGB(0,0,255),
	RGB(128,0,128),
	RGB(255,0,255),
	RGB(0,128,0),
	RGB(0,255,0),
	RGB(0,128,128),
	RGB(0,255,255),
	RGB(128,0,0),
	RGB(255,0,0),
	RGB(128,128,0),
	RGB(255,255,0),
	RGB(128,128,128),
	RGB(192,192,192),
	RGB(255,255,255),
	};
	int ent=sizeof(svColors)/sizeof(COLORREF);
	int i;
    for(i=0; i<ent; i++) {
		if (color==svColors[i]) {
			if (bInc)
				i++;
			else {
				if (i==0)
					i=ent-1;
				else
					i--;
			}
			break;
		}
	}
	return svColors[i%ent];
	/*
	if (bInc && color==RGB(255,255,255))
		return RGB(0,0,0);
	else if (!bInc && color==RGB(0,0,0))
		return RGB(255,255,255);

	BYTE R = GetRValue(color);
	BYTE G = GetGValue(color);
	BYTE B = GetBValue(color);

	int inc = 2550/(mod-1);
	inc = inc/10 + (inc%10>=5 ? 1:0);

	if (bInc) {
		if (++R==0) {
			if (++G==0) {
				if (++B==0)
					++R;
			}
		}
		bool bR=R==0, bG=G==0, bB=B==0;
		R = (R/inc + (R%inc >0 ? 1:0))*inc;
		G = (G/inc + (G%inc >0 ? 1:0))*inc;
		B = (B/inc + (B%inc >0 ? 1:0))*inc;
		if (R==0 && !bR)
			R--;
		if (G==0 && !bG)
			G--;
		if (B==0 && !bB)
			B--;
	}else{
		if (--R==255) {
			if (--G==255) {
				if (--B==255)
					--R;
			}
		}
		bool bR=R==255, bG=G==255, bB=B==255;
		R = (R/inc)*inc;
		G = (G/inc)*inc;
		B = (B/inc)*inc;
		if (bR)
			R=255;
		if (bG)
			G=255;
		if (bB)
			B=255;
	}
//	TRACE("RGB=(%d,%d,%d)\n",R,G,B);
	return RGB(R,G,B);
*/
}

void CListCtrEx::ColorRGBItem(int nItem, bool bInc)
{
	COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(nItem);
	if (p) {
		p->color = IncColorref(p->color, bInc);
		VERIFY(SetItemData(nItem, (unsigned long)p));
		RedrawItems(nItem,nItem);
		m_bDirty = TRUE;
	}
}

void CListCtrEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//選択状態の更新
	RedrawItems(m_selRow,m_selRow);
	if (GetItemCount()>0) {
		CRect rc;
		//CListCtrl::GetItemRect(0, rc, LVIR_LABEL);
		int width = 0;
		for (int col=0; col<m_maxColumn; col++){
			width += GetColumnWidth(col);
			if (point.x<=width) {
				m_selCol = col;
				RedrawItems(m_selRow,m_selRow);
				break;
			}
		}
	}
	
	//状態チェックの更新
	int nHitItem;
	int hit = GetHitPoint(point, nHitItem);
	if (hit == hit_state)
		CheckItem(nHitItem);
	else if (hit == hit_icon && m_bRGBColor)
		ColorRGBItem(nHitItem, !(GetKeyState(VK_SHIFT)<0));
	else	
		CListCtrl::OnLButtonDown(nFlags, point);


//	CListCtrl::OnLButtonDown(nFlags, point);
}

int CListCtrEx::GetHitPoint(CPoint point, int &nHitItem) 
{
	UINT uFlags = 0;
	nHitItem = HitTest(point, &uFlags);

	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	int hit = hit_no;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect, rcLabel;
		CListCtrl::GetItemRect(nHitItem, rect, LVIR_ICON);
		CListCtrl::GetItemRect(nHitItem, rcLabel, LVIR_LABEL);
		// check if hit was on a state icon
		if (m_bStateIcons && point.x < rect.left)
			hit = hit_state;
		else if (point.x < rect.right)
			hit = hit_icon;
		else if (point.x < rcLabel.right)
			hit = hit_label;
	}
	else if (uFlags & LVHT_ONITEMSTATEICON)
		hit = hit_state;
	else if (uFlags & LVHT_ONITEMICON)
		hit = hit_icon;
	else if (uFlags & LVHT_ONITEMLABEL)
		hit = hit_label;
	return hit;
}
void CListCtrEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//	SetEdit(!m_selIn);
	//状態チェックの更新
	int nHitItem;
	int hit = GetHitPoint(point, nHitItem);
	if ((/*hit == hit_icon ||*/ hit == hit_label) && m_bRGBColor) {
		//ColorRGBItem(nHitItem, (GetKeyState(VK_SHIFT)<0)); //ここに来る直前にLButtonDown()で変更されたものを基に戻す！
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(nHitItem);
		CColorDialog dlg(p->color);
		if (dlg.DoModal()==IDOK) {
			p->color = dlg.GetColor();
			VERIFY(SetItemData(nHitItem, (unsigned long)p));
			RedrawItems(nHitItem,nHitItem);
			m_bDirty = TRUE;
		}
	}else	
		CListCtrl::OnLButtonDblClk(nFlags, point);
}
/*
void CListCtrEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch (nChar) {
	case VK_LEFT:
		if (m_selCol>0) {
			m_selCol--;
		}
		break;
	case VK_RIGHT:
		if (m_selCol<m_selColMac)
			m_selCol++;
		break;
	case VK_SPACE:
	case VK_RETURN:
		SetEdit(!m_selIn);
		break;
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CListCtrEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CListCtrl::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;
	if (m_edit.Create(WS_CHILD, CRect(0,0,0,0), this, 1)) {
		m_edit.SetFont(GetFont(), FALSE);
		LockUpdate(m_bLockUpdate);
		return TRUE;
	}else
		return FALSE;
}
*/
CRect CListCtrEx::GetItemTextRect(int rowPt, int colPt)
{
	CRect rc = GetItemRect(rowPt, colPt);
	if (colPt>0) {
		rc.left += OFFSET_OTHER;
		rc.right -= OFFSET_OTHER;
	}
	return rc;
}
CRect CListCtrEx::GetItemRect(int rowPt, int colPt)
{
	//ScreenToClient(&point);
	if (GetItemCount()<=0 || rowPt<0)
		return CRect(0,0,0,0);

	CRect rc;
	if (colPt==0) {
		CListCtrl::GetItemRect(rowPt, rc, LVIR_LABEL);//LVIR_BOUNDS
		//rc.bottom = pos + GetColumnWidth(0);
	}else{
		CListCtrl::GetItemRect(rowPt, rc, LVIR_BOUNDS);
		int width, pos;
		pos = rc.left;
		for (int col=0; col<m_maxColumn; col++){
			width = GetColumnWidth(col);
			if (col == colPt) {
				rc.left = pos;
				rc.right = pos + width;
				break;
			}
			pos += width;
		}
	}
	return rc;
}
/*
void CListCtrEx::SetEdit(BOOL b)
{
	if (m_selRow<0 || m_selCol<0)
		return;
	if (m_selEdit && m_cellTbl.GetAt(m_selRow, m_selCol).m_bEdit) {
		CString str;
		if (m_selIn=b) {
			m_edit.ShowWindow(SW_HIDE);
			CRect rect;
			rect = GetSelTextRect();
			rect.top++; rect.bottom--;
			ASSERT(!rect.IsRectNull());
			m_edit.MoveWindow(rect);
			str = GetItemText();
			m_edit.SetText(m_selRow, m_selCol, str, GetItemJustify(m_selRow, m_selCol));
			m_edit.ShowWindow(SW_SHOW);
			m_edit.SetFocus();
		}else if (m_edit.IsWindowVisible()) {
			SetItemText(m_edit.GetRow(), m_edit.GetCol(), m_edit.GetText());
			m_edit.ShowWindow(SW_HIDE);
			SetFocus();
#ifdef update_msg_to_parent
			((CMDListCtrl *)GetParent())->FireCellChange(m_edit.GetRow(), m_edit.GetCol());
#endif
		}
	}
}
*/
CString CListCtrEx::GetItemText(int nItem, int nColumn)
{
	CListCtrl& ListCtrl = GetListCtrl();
	static _TCHAR szBuff[MAX_PATH];
	if (nItem<0)
		nItem = m_selRow;
	if (nColumn<0)
		nColumn = m_selCol;

	COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(nItem);
	if (p) {
		return p->aryStr[nColumn];
	}
	if (CListCtrl::GetItemText(nItem, nColumn, szBuff, sizeof(szBuff))>0)
		return CString(szBuff);
	else
		return CString();
}
BOOL CListCtrEx::SetItemText(int nItem, int nColumn, LPCSTR sz)
{
	CListCtrl& ListCtrl = GetListCtrl();
	if (nItem<0)
		nItem = m_selRow;
	if (nColumn<0)
		nColumn = m_selCol;

#ifdef _DEBUG
	int len = strlen(sz);
#endif
	BOOL rtn =  CListCtrl::SetItemText(nItem, nColumn, sz);
	if (rtn) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(nItem);
		if (p) {
			p->aryStr.SetAtGrow(nColumn, sz);
			return TRUE;
		}
	}
	return rtn;
}
/*
void CListCtrEx::ScrollSel()
{
	CListCtrl& ListCtrl = GetListCtrl();

	CRect rectView = GetViewRect();
	CRect rect = GetSelRect();
	if (rect.left<rectView.left) {

	} else if (rect.right>rectView.right) {
	}
}
*/

UINT CListCtrEx::GetItemJustify(int row, int col)
{
	CListCtrl& ListCtrl = GetListCtrl();
	UINT nJustify = DT_LEFT;
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
				   
	if (GetColumn(col, &lvc)) {
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}
	}
	return nJustify;
}

BOOL CListCtrEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(rect, &m_brBkgnd);
	return TRUE;
	//return CListCtrl::OnEraseBkgnd(pDC);
}

/*
void CListCtrEx::OnClose() 
{
	for (int i=0; i<GetItemCount(); i++) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA*)GetItemData(i);
		if (p) {
			//if (p->pdata)
			//	delete p->pdata;
			delete p;
		}
	}
	CListCtrl::OnClose();
}
*/