#if !defined(AFX_FormatCharPage_H__CEA43354_2B30_11D2_AEC4_0060976DB3A9__INCLUDED_)
#define AFX_FormatCharPage_H__CEA43354_2B30_11D2_AEC4_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FormatCharPage.h : ヘッダー ファイル
//
#include "ViewEnv.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatCharPage ダイアログ

class CFormatCharPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFormatCharPage)
// コンストラクション
public:
	CFormatCharPage();
	~CFormatCharPage();

// ダイアログ データ
	//{{AFX_DATA(CFormatCharPage)
	enum { IDD = IDD_FORMAT_CHAR };
	CButton	m_btnKinLast;
	CButton	m_btnKinFirst;
	CButton	m_optKinLevel;
	CEdit	m_edtKinLast;
	CEdit	m_edtKinFirst;
	CButton	m_chkKinsoku;
	BOOL	m_bKinsok;
	CString	m_strKinFirst;
	CString	m_strKinLast;
	int		m_iKinLevel;
	BOOL	m_bWordWrap;
	BOOL	m_bKinFirst;
	BOOL	m_bKinLast;
	BOOL	m_bCRLFBra;
	//}}AFX_DATA

	bool m_bChange;
	bool m_bChange_format;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFormatCharPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
private:
	void CFormatCharPage::KinsokEnableWindow(bool bEnable);

public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
	int IsChangeFormat() {
		return m_bChange_format; }

protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFormatCharPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkKinsok();
	afx_msg void OnRdoKinLevel1();
	afx_msg void OnChangeEdtKinFirst();
	afx_msg void OnChangeEdtKinLast();
	afx_msg void OnRdoKinLevel2();
	afx_msg void OnRdoKinCust();
	afx_msg void OnChkKinFirst();
	afx_msg void OnChkKinLast();
	afx_msg void OnRdoKinLevel0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FormatCharPage_H__CEA43354_2B30_11D2_AEC4_0060976DB3A9__INCLUDED_)
