#if !defined(AFX_TANBUNEDIT_H__FB37CD84_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_)
#define AFX_TANBUNEDIT_H__FB37CD84_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TanbunEdit.h : ヘッダー ファイル
//
#ifdef SHAR

#include "ListCtrEx.h"
#include "tanbun.h"
#include "ViewEnv.h"
/////////////////////////////////////////////////////////////////////////////
// CTanbunEdit ダイアログ
typedef struct {
	string	strName;
	CTanbun *pTanbun;
}TANBUN_EDIT_DATA;

class CTanbunEdit : public CDialog
{
// コンストラクション
public:
	CTanbunEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTanbunEdit)
	enum { IDD = IDD_TANBUN_EDIT };
	//}}AFX_DATA
	//CListCtrl	m_lstctr;
	CListCtrEx	m_lstctr;

	CImageList m_smallImage;
	CImageList m_stateImage;

	bool m_bChange;
	int			m_nItem;
	TANBUN_EDIT_DATA	*m_pdata;
	CTanbuns *m_ptanbuns;
	string	m_strInputData;
	bool UpdateEnv(bool bSetToEnv, CTanbuns *ptanbuns);
	string GetInputData() {
		return m_strInputData; }
private:
	bool GetSelData(bool bSetEnableWindow=false);
	void ResetListctrEntry();
	void RemoveAll();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CTanbunEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTanbunEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnEdit();
	afx_msg void OnItemchangedLstctr(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SHAR


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TANBUNEDIT_H__FB37CD84_3D7D_11D2_AF10_0060976DB3A9__INCLUDED_)
