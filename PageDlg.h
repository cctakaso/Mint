#if !defined(AFX_PAGEDLG_H__01AB7984_EAFE_11D2_B096_0060976DB3A9__INCLUDED_)
#define AFX_PAGEDLG_H__01AB7984_EAFE_11D2_B096_0060976DB3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPageDlg ダイアログ
#ifdef SPEC_PRINT
bool ConvertMargin(CRect &rc_MM10, const CSize &sizePaperSize_MM10, DEVMODE devmode);
class CPageDlg : public CDialog
{
// コンストラクション
public:
	CPageDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPageDlg)
	enum { IDD = IDD_PAGE_SETUP_DIALOG };
	int		m_iMarginTop;
	int		m_iMarginRight;
	int		m_iMarginLeft;
	int		m_iMarginBottom;
	CString	m_strHeaderLeft;
	CString	m_strHeaderRight;
	CString	m_strHeaderCenter;
	CString	m_strFooterRight;
	CString	m_strFooterLeft;
	CString	m_strFooterCenter;
	BOOL	m_bColor;
	BOOL	m_bLineNum;
	BOOL	m_bHeaderLine;
	BOOL	m_bFooterLine;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPageSetup();
	virtual void OnOK();
	afx_msg void OnBtnPrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SPEC_PRINT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PAGEDLG_H__01AB7984_EAFE_11D2_B096_0060976DB3A9__INCLUDED_)
