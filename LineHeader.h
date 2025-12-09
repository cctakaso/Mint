#if !defined(AFX_LINEHEADER_H__2710D334_318E_11D2_AED6_000000000000__INCLUDED_)
#define AFX_LINEHEADER_H__2710D334_318E_11D2_AED6_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LineHeader.h : ヘッダー ファイル
//
#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CLineHeaderDlg ダイアログ

class CLineHeaderDlg : public CDialog
{
// コンストラクション
public:
	CLineHeaderDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CLineHeaderDlg)
	enum { IDD = IDD_LINE_HEADER };
	CComboBox	m_cmbLineHeader;
	CString	m_strLineHeader;
	BOOL	m_bOpenDlg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CLineHeaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CLineHeaderDlg)
	virtual void OnOK();
	afx_msg void OnDropdownCmbLineHeader();
	afx_msg void OnChkOpendlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SHAR


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_LINEHEADER_H__2710D334_318E_11D2_AED6_000000000000__INCLUDED_)
