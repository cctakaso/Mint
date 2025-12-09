#if !defined(AFX_EDITSEL_H__99192E83_91B7_11D1_AD46_0060976DB3A9__INCLUDED_)
#define AFX_EDITSEL_H__99192E83_91B7_11D1_AD46_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditSel.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditSel ウィンドウ

class CEditSel : public CEdit
{
// コンストラクション
public:
	CEditSel();

// アトリビュート
public:
	int m_row;
	int m_col;
	UINT m_nJustify;
	void SetText(int row, int col, LPCSTR sz, UINT nJustify=DT_LEFT);
	CString GetText();
	int GetRow() {
		return m_row; }
	int GetCol() {
		return m_col; }

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditSel)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CEditSel();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditSel)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITSEL_H__99192E83_91B7_11D1_AD46_0060976DB3A9__INCLUDED_)
