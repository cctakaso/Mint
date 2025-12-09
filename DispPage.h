#if !defined(AFX_DISPPAGE_H__94EE5434_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
#define AFX_DISPPAGE_H__94EE5434_2460_11D2_AEAE_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DispPage.h : ヘッダー ファイル
//
#include "ViewEnv.h"
#include "ListCtrEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDispPage ダイアログ

class CDispPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDispPage)

// コンストラクション
public:
	CDispPage();
	~CDispPage();

// ダイアログ データ
	//{{AFX_DATA(CDispPage)
	enum { IDD = IDD_DISP };
	CButton	m_btnFontVert;
	CButton	m_btnFontHorz;
	CString	m_strFontHorz;
	CString	m_strFontVert;
	int		m_ratioBetweenLine;
	//}}AFX_DATA

//	CListCtr	m_lstColors;
	CListCtrEx	m_lstctr;

	CImageList m_smallImage;
	CImageList m_stateImage;
	ENV_COLORS m_envColors[idc_max];

	bool m_bChange;
	LOGFONT m_lf;
	LOGFONT m_lfVert;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDispPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
private:
	CString FontChange(LOGFONT &lf, bool bDoChange);
	void Initialize(BOOL fMemOnly=FALSE);
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDispPage)
	afx_msg void OnBtnFontHorz();
	afx_msg void OnBtnFontVert();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DISPPAGE_H__94EE5434_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
