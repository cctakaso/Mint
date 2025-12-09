#if !defined(__LISTCTREX_INCLUDED__)
#define __LISTCTREX_INCLUDED__
// ListCtrEx.h : interface of the CListCtrEx class
//
// This class provedes a full row selection mode for the report
// mode list view control.
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
#include <afxcmn.h>
//#include "EditSel.h"
//#include "cell.h"

enum eSelType {
	st_first=-1,
	st_normal,	//first item selection
	st_full,
	st_grid,
	st_end
};

#define INT2COLORREF(p)		(p&0xffffff)

typedef struct {
	COLORREF color;
	CStringArray aryStr;
	void *pdata;
}COLOR_ITEM_DATA;

class CListCtrEx : public CListCtrl
{
	DECLARE_DYNCREATE(CListCtrEx)

// Construction
public:
	CListCtrEx();
//	friend class CEditSel;
	void Clear();
// Attributes
protected:
	eSelType m_selType;
	BOOL	m_selEdit;
	int		m_selRow;
	BOOL	m_selIn;
	int		m_selCol;
	int		m_selColMac;
	BOOL	m_bRGBColor;
	BOOL	m_bMultiCheck;
	BOOL	m_bDirty;
//	CEditSel	m_edit;
//	CCellTable	m_cellTbl;
	int		m_maxColumn;
	BOOL	m_bLockUpdate;
public:
	CString GetItemText(int nItem=-1, int nColumn=-1);
	BOOL SetItemText(int nItem, int nColumn, LPCSTR sz);
	BOOL IsDirty() {
		return m_bDirty; }
// Operations
public:
	BOOL SetImageLists(CImageList* pImageListSmall=NULL, CImageList* pImageListNormal=NULL, CImageList* pImageListState=NULL);
	BOOL AddColumn(LPCTSTR strItem,int nItem, int nFmt = LVCFMT_LEFT, BOOL bEdit=FALSE,
		int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM, int nSubItem = -1);
	BOOL AddItem(int nItem, int nSubItem, LPCTSTR strItem, BOOL bEdit=FALSE, int nImageIndex = -1, int nState=-1, void *pdata=null);
	BOOL SetItemData( int nItem, DWORD dwData );
	DWORD GetItemData( int nItem);

	BOOL SetViewType(DWORD dwViewType);
	DWORD GetViewType();
	////
	BOOL SetSelType(DWORD type, eSelType selType=st_normal, BOOL bEdit=FALSE, BOOL bColor=FALSE, BOOL bMultiCheck=FALSE);
	eSelType GetSelType() {
		return m_selType; }
	void LockUpdate(BOOL bLockUpdate);

protected:
	CRect GetSelTextRect() {
		return GetItemTextRect(m_selRow, m_selCol); }
	CRect GetSelRect() {
		return GetItemRect(m_selRow, m_selCol); }
	CRect GetItemTextRect(int rowPt, int colPt);
	CRect GetItemRect(int rowPt, int colPt);
//	void SetEdit(BOOL b);
	UINT GetItemJustify(int row, int col);
// Overrides
	enum {
		hit_no,
		hit_state,
		hit_icon,
		hit_label,
	};
	int GetHitPoint(CPoint point, int &nHitItem) ;

protected:
#ifdef _DEBUG
	void AssertValid() const;
	void Dump(CDumpContext& dc) const;
#endif //_DEBUG
	CListCtrl& GetListCtrl() const;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrEx)
	public:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL
//	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)=0;
//	virtual void DoPropExchange(CPropExchange* pPX)=0;
//	virtual void OnResetState()=0;

// Implementation
public:
	virtual ~CListCtrEx();

protected:
	BOOL m_bClientWidthSel;
	BOOL m_bStateIcons;
	int m_nCheckedItem;
	HICON m_hSmallIcon;
	HICON m_hLargeIcon;

	void CheckItem(int nNewCheckedItem);
	void ColorRGBItem(int nNewCheckedItem, bool bInc=true);
	/////
	static LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	void RepaintSelectedItems();
	void SetSelOfSelection(CDC *&pDC, CRect &rcLabel);
// Implementation - client area width
	int m_cxClient;

// Implementation - state icon width
	int m_cxStateImageOffset;
	afx_msg LRESULT OnSetImageList(WPARAM wParam, LPARAM lParam);

// Implementation - list view colors
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;
	COLORREF m_clrGray;
	COLORREF m_clrHighlight;

	CBrush m_brTextBk;
	CBrush m_brBkgnd;
	CBrush m_brGray;
	CBrush m_brHighlight;
	afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);

// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
inline BOOL CListCtrEx::SetItemData( int nItem, DWORD dwData ){
	return GetListCtrl().SetItemData(nItem, dwData); }
inline DWORD CListCtrEx::GetItemData( int nItem){
	return GetListCtrl().GetItemData(nItem); }
inline CListCtrl& CListCtrEx::GetListCtrl() const
	{ return *(CListCtrl*)this; }

/////////////////////////////////////////////////////////////////////////////
#endif //__LISTCTREX_INCLUDED__
