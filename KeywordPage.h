#if !defined(AFX_KEYWORDPAGE_H__DAB90CD4_38D4_11D2_AEFC_000000000000__INCLUDED_)
#define AFX_KEYWORDPAGE_H__DAB90CD4_38D4_11D2_AEFC_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KeywordPage.h : ヘッダー ファイル
//
#ifdef SHAR

#include "keyword.h"
#include "ListCtrEx.h"
#include "ViewEnv.h"

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage ダイアログ

class CKeywordPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CKeywordPage)

// コンストラクション
public:
	CKeywordPage();
	~CKeywordPage();

// ダイアログ データ
	//{{AFX_DATA(CKeywordPage)
	enum { IDD = IDD_KEYWORD };
	CButton	m_btnUpdate;
	CButton	m_btnDel;
	CButton	m_btnAdd;
	BOOL	m_bEndRow;
	BOOL	m_bGroup;
	BOOL	m_bCase;
	BOOL	m_bUnderline;
	BOOL	m_bWord;
	//}}AFX_DATA

	//CListCtrl	m_lstctr;
	CListCtrEx	m_lstctr;
	CImageList m_smallImage;
	CImageList m_stateImage;

	bool m_bChange;
	CKeywords *m_pkeywords;
	CKeywordConf *m_pconf;
	bool GetSelData();
	int m_nItem;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CKeywordPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
private:
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKeywordPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnUpdate();
	afx_msg void OnChkGroup();
	afx_msg void OnEndlabeleditListctrColors(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListctrColors(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkEndrow();
	afx_msg void OnChkMeta();
	afx_msg void OnChkUnderline();
	afx_msg void OnChkWord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif //#ifdef SHAR

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_KEYWORDPAGE_H__DAB90CD4_38D4_11D2_AEFC_000000000000__INCLUDED_)
