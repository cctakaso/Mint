#if !defined(AFX_FORMATPAGE_H__94EE5435_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
#define AFX_FORMATPAGE_H__94EE5435_2460_11D2_AEAE_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FormatPage.h : ヘッダー ファイル
//
#include "ViewEnv.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatPage ダイアログ

class CFormatPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFormatPage)

// コンストラクション
public:
	CFormatPage();
	~CFormatPage();

// ダイアログ データ
	//{{AFX_DATA(CFormatPage)
	enum { IDD = IDD_FORMAT };
	CButton	m_raoColBlank;
	CEdit	m_edtColTab;
	CEdit	m_edtColBlank;
	int		m_optOrient;
	int		m_iColTab;
	int		m_optColBlank;
	int		m_iColBlank;
	BOOL	m_bRowNum;
	BOOL	m_bAutoIndent;
	//}}AFX_DATA

	bool m_bChange;
	bool m_bChange_format;

	int m_optOrient_save;
	int m_iColTab_save;
	BOOL m_bAutoIndent_save;
	int m_optColBlank_save;
	int m_iColBlank_save;
	int m_bRowNum_save;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFormatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
	int IsChangeFormat() {
		return m_bChange_format; }
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFormatPage)
	afx_msg void OnRaoColBlack();
	afx_msg void OnRaoColBlackNo();
	afx_msg void OnChangeEdtColTab();
	afx_msg void OnChangeEdtColBlack();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FORMATPAGE_H__94EE5435_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
