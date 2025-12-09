#if !defined(AFX_JUMPDLG_H__4CB230B4_2A8B_11D2_AEC1_000000000000__INCLUDED_)
#define AFX_JUMPDLG_H__4CB230B4_2A8B_11D2_AEC1_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// JumpDlg.h : ヘッダー ファイル
//
class CTheView;
/////////////////////////////////////////////////////////////////////////////
// CJumpDlg ダイアログ
class CJumpDlg : public CDialog
{
// コンストラクション
public:
	CJumpDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CJumpDlg)
	enum { IDD = IDD_JUMP };
	CButton	m_chkRowNum;
	CComboBox	m_cmbJump;
	CString	m_strJump;
	BOOL	m_bRowNum;
	//}}AFX_DATA


// オーバーライド
	bool OnJumpLine(CTheView *pTheView, OBJOFF lineJump, JUMPTYPE JumpType=JUMP_LINE);
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CJumpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL
private:
	CStringArray m_aryStrJump;
	CStringArray m_aryStrRowNumFlg;
	CTheView *m_pTheView;
// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CJumpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeCmbJump();
	virtual void OnOK();
	afx_msg void OnSelchangeCmbJump();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_JUMPDLG_H__4CB230B4_2A8B_11D2_AEC1_000000000000__INCLUDED_)
