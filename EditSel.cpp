// EditSel.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "mdlist.h"
#include "EditSel.h"
#include "ListCtrEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSel

CEditSel::CEditSel()
{
	m_row = -1;
	m_col = -1;
	m_nJustify = DT_LEFT;
}

CEditSel::~CEditSel()
{
}


BEGIN_MESSAGE_MAP(CEditSel, CEdit)
	//{{AFX_MSG_MAP(CEditSel)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSel メッセージ ハンドラ
void CEditSel::SetText(int row, int col, LPCSTR sz, UINT nJustify)
{
	m_row = row;
	m_col = col;
	m_nJustify = nJustify;
	SetWindowText(sz);
	SetSel(0,-1);
}

CString CEditSel::GetText()
{
	CString str;
	GetWindowText(str);
	return str;
}

BOOL CEditSel::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	cs.style |= ES_WANTRETURN | ES_MULTILINE;

	return CEdit::PreCreateWindow(cs);
}


void CEditSel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch (nChar) {
	case VK_RETURN:
		((CListCtrEx *)GetParent())->OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	case VK_TAB:
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditSel::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case VK_BACK:
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		break;
	default:
		if (m_nJustify==DT_RIGHT)
			return;
		break;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
