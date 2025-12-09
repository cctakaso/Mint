#if !defined(AFX_TANBUNADDDLG_H__FB37CD85_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_)
#define AFX_TANBUNADDDLG_H__FB37CD85_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TanbunAddDlg.h : ヘッダー ファイル
//
#ifdef SHAR


#include "tanbun.h"
/////////////////////////////////////////////////////////////////////////////
// CTanbunAddDlg ダイアログ

class CTanbunAddDlg : public CDialog
{
// コンストラクション
public:
	CTanbunAddDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTanbunAddDlg)
	enum { IDD = IDD_TANBUN_ADD };
	CString	m_strKey;
	CString	m_strData;
	int		m_fmt;
	BOOL	m_bLayout;
	//}}AFX_DATA
	enum {
		mode_new,
		mode_update,
	};
	CString	m_strKey_save;
	CString	m_strData_save;
	int		m_fmt_save;
	bool	m_bChange;
	int		m_mode;
	int		m_index;
	CTanbuns *m_pTanbuns;
	bool UpdateEnv(bool bSetToEnv, CTanbuns *pTanbuns,
		int mode, string &strKey, string &strData, int &fmt, const int index=-1);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CTanbunAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTanbunAddDlg)
	afx_msg void OnChkLayout();
	afx_msg void OnBtnHowto();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef SHAR


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TANBUNADDDLG_H__FB37CD85_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_)
