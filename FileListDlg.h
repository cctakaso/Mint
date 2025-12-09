#if !defined(AFX_FILELISTDLG_H__EE31E844_D913_11D2_B077_000000000000__INCLUDED_)
#define AFX_FILELISTDLG_H__EE31E844_D913_11D2_B077_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileListDlg.h : ヘッダー ファイル
//
#ifdef SHAR
/////////////////////////////////////////////////////////////////////////////
// CFileListDlg ダイアログ

class CFileListDlg : public CDialog
{
// コンストラクション
public:
	CFileListDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFileListDlg)
	enum { IDD = IDD_FILE_LIST };
	CListBox	m_lst_sort;
	CListBox	m_lst;
	static BOOL	m_bSort;
	//}}AFX_DATA

public:
	void SetFileSelectBar(CFileSelectBar *pBar) {
		m_pBar = pBar; }
	int GetCurSel(void) {
		return m_selWindow; }
private:
	void SetCurSel(CListBox *pList);
	void SetListEntry(CListBox *pList);
	CListBox *GetFrontList(void) {
		return (m_bSort ? &m_lst_sort:&m_lst); }
	CListBox *GetBackList(void) {
		return (m_bSort ? &m_lst:&m_lst_sort); }
	void SwitchList();
	CFileSelectBar *m_pBar;
	int				m_selWindow;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFileListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFileListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLst();
	virtual void OnOK();
	afx_msg void OnChkSort();
	afx_msg void OnDblclkLstWindowsSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FILELISTDLG_H__EE31E844_D913_11D2_B077_000000000000__INCLUDED_)
