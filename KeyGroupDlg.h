#if !defined(AFX_KEYGROUPDLG_H__8FCDA884_397D_11D2_AEFF_0060976DB3A9__INCLUDED_)
#define AFX_KEYGROUPDLG_H__8FCDA884_397D_11D2_AEFF_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KeyGroupDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CKeyGroupDlg ダイアログ
#ifdef SHAR

class CKeyGroupDlg : public CDialog
{
// コンストラクション
public:
	CKeyGroupDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CKeyGroupDlg)
	enum { IDD = IDD_KEYWORD_GROUP };
	CString	m_strName;
	CString	m_strKeywords;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CKeyGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKeyGroupDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SHAR

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_KEYGROUPDLG_H__8FCDA884_397D_11D2_AEFF_0060976DB3A9__INCLUDED_)
