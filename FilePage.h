#if !defined(AFX_FILEPAGE_H__94EE5436_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
#define AFX_FILEPAGE_H__94EE5436_2460_11D2_AEAE_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FilePage.h : ヘッダー ファイル
//
#include "ViewEnv.h"

/////////////////////////////////////////////////////////////////////////////
// CFilePage ダイアログ

class CFilePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFilePage)

// コンストラクション
public:
	CFilePage();
	~CFilePage();

// ダイアログ データ
	//{{AFX_DATA(CFilePage)
	enum { IDD = IDD_FILE };
	CEdit	m_edtBackupFolder;
	CButton	m_btnBackupFolder;
	int		m_optBackup;
	CString	m_strBackFolder;
#ifdef EDITMENU_OFLAY
	BOOL	m_bEditMenu_atOpen;
	BOOL	m_bGird_atEditMenu;
#endif
	CString	m_strExtents;
	BOOL	m_bRockUpdate;
	//}}AFX_DATA

	bool	m_bChange;
	bool	m_bRockUpdate_Chnage;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFilePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL
public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
	bool IsNowUpdate() {
		return m_bRockUpdate_Chnage; }
// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFilePage)
	afx_msg void OnBtnBackupFolder();
	afx_msg void OnRaoBackupNo();
	afx_msg void OnRaoBackupTrash();
	afx_msg void OnRaoBackupBak();
	afx_msg void OnRaoBackupFolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkRockupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FILEPAGE_H__94EE5436_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
