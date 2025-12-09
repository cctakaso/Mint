#if !defined(AFX_CONFIGEDIT_H__ACFF7314_3AF0_11D2_AF04_0060976DB3A9__INCLUDED_)
#define AFX_CONFIGEDIT_H__ACFF7314_3AF0_11D2_AF04_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConfigEdit.h : ヘッダー ファイル
#ifdef SHAR

#include "ListCtrEx.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigEdit ダイアログ
typedef struct {
	string	strName;
	CViewEnv *pEnv;
}ENV_EDIT_DATA;

class CConfigEdit : public CDialog
{
// コンストラクション
public:
	CConfigEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConfigEdit)
	enum { IDD = IDD_CONFIG_EDIT };
	//}}AFX_DATA

	//CListCtrl	m_lstctr;
	CListCtrEx	m_lstctr;

	CImageList m_smallImage;
	CImageList m_stateImage;

	bool m_bChange;
	int			m_nItem;
	ENV_EDIT_DATA	*m_pdata;
	CViewEnvTbl *m_pEnvTbl;
	bool UpdateEnv(bool bSetToEnv, CViewEnvTbl *pEnvTbl);
private:
	void NewData(const char *szName_base=null) ;
	bool GetSelData(bool bSetEnableWindow=false);
	void ResetListctrEntry();
	void RemoveAll();
	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CConfigEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfigEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnAddCopy();
	afx_msg void OnBtnDel();
	afx_msg void OnItemchangedLstCtr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnEdit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SHAR
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFIGEDIT_H__ACFF7314_3AF0_11D2_AF04_0060976DB3A9__INCLUDED_)
