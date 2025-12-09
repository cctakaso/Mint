#if !defined(AFX_ENVEDIT_H__1EEE87F4_3BB1_11D2_AF07_000000000000__INCLUDED_)
#define AFX_ENVEDIT_H__1EEE87F4_3BB1_11D2_AF07_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EnvEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEnvEdit ダイアログ
#ifdef SHAR

class CEnvEdit : public CDialog
{
// コンストラクション
public:
	CEnvEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEnvEdit)
	enum { IDD = IDD_ENV_EDIT };
	CString	m_strName;
	CString	m_strFileName;
	CString	m_strExtents;
	//}}AFX_DATA

	enum {
		mode_new,
		mode_update,
		//mode_del,
	};
	int		m_mode;
	bool	m_bChange;
	CString	m_strName_save;
	CString	m_strFileName_save;
	CString	m_strExtents_save;
	CString m_strName_base;

	CViewEnvTbl *m_pEnvTbl;

	bool UpdateEnv(bool bSetToEnv, CViewEnvTbl *pEnvTbl, int mode, string &strName, string &strFile, string &strExtents, const char *szName_base=null);
private:
	bool CheckName(const char* szName);
	bool CheckFile(const char * szFile, bool &bExist);
	string MakeProfileName(const char *szFileName);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEnvEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEnvEdit)
	virtual void OnOK();
	afx_msg void OnBtnRef();
	afx_msg void OnUpdateEdtName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef SHAR

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ENVEDIT_H__1EEE87F4_3BB1_11D2_AF07_000000000000__INCLUDED_)
