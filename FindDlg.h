#if !defined(AFX_FINDDLG_H__56AF8C84_2225_11D2_AEA0_0060976DB3A9__INCLUDED_)
#define AFX_FINDDLG_H__56AF8C84_2225_11D2_AEA0_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FindDlg.h : ヘッダー ファイル
//
#include "sell.h"

class CTheView;
/////////////////////////////////////////////////////////////////////////////
// CFindDlg ダイアログ

class CFindDlg : public CDialog
{
// コンストラクション
public:
	CFindDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFindDlg)
	enum { IDD = IDD_FIND };
	CButton	m_btnDoUp;
	CButton	m_btnReplaceAll;
	CButton	m_btnReplace;
	CComboBox	m_cmbReplace;
	CButton	m_btnDoDown;
	CComboBox	m_cmbFind;
	BOOL	m_bCase;
	CString	m_strFind;
	CString	m_strReplace;
	//}}AFX_DATA


	void OnEditFind(CTheView *pTheView, const char *sz=null);
	void OnEditFindPrev(CTheView *pTheView);
	void OnEditFindNext(CTheView *pTheView);
	void OnEditReplace(CTheView *pTheView, const char *sz=null);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

private:
	CStringArray m_aryStrFind;
	CStringArray m_aryStrReplace;
	bool		m_bReplace;
	CTheView	*m_pTheView;
	bool		m_bDointReplaceAll;
	bool		m_bAbleReplace;
	CUR			m_cur_last;

// インプリメンテーション
	bool DoFind(bool bToDocEnd);
	bool DoFindNext_Between(CUR cur_first, CUR cur_last, bool bToDocEnd);
	bool DoFindNext(CUR *pcur_last, bool bToDocEnd);

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFindDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoDown();
	afx_msg void OnDoReplace();
	afx_msg void OnDoReplaceAll();
	virtual void OnCancel();
	afx_msg void OnDoUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDDLG_H__56AF8C84_2225_11D2_AEA0_0060976DB3A9__INCLUDED_)
