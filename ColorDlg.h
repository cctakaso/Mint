// ColorDlg.h : ヘッダー ファイル
//
#ifndef __COLORDLG_H__
#define __COLORDLG_H__
#include "resource.h"
#include "colorlist.h"
#include "ViewEnv.h"
/////////////////////////////////////////////////////////////////////////////
// CColorDlg ダイアログ
class CColorDlg : public CDialog
{
// コンストラクション
public:
	CColorDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CColorSel *GetSelData(int iSel) {
		return m_lstColors.GetSelData(iSel); };
	void Initialize(CViewEnv *pEnv) {
			m_pEnv = pEnv; }
protected:
	CViewEnv *m_pEnv;
	// ダイアログ データ
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLORS };
	CButton	m_btnOk;
	CColorListBox	m_lstColors;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void Initialize(BOOL fMemOnly=FALSE);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CColorDlg)
	afx_msg void OnDblclkLstColor();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstColor();
	afx_msg void OnChangeColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifndef __COLORDLG_H__
