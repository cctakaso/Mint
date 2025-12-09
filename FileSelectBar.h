#if !defined(AFX_FILESELECTBAR_H__83AA1AB4_3CD1_11D2_AF0C_000000000000__INCLUDED_)
#define AFX_FILESELECTBAR_H__83AA1AB4_3CD1_11D2_AF0C_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileSelectBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFileSelectBar ダイアログ
//#include "HeaderCtrlEx.h"
#include "ObjLay.h"
typedef vector<CObjLayTbl *> CObjLays;

class CTabCtrlEx : public CTabCtrl
{
	DECLARE_DYNCREATE(CTabCtrlEx)
public:
	CTabCtrlEx() {};
	virtual ~CTabCtrlEx() {};
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTabCtrlEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest( CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
};
class CFileSelectBar : public CDialogBar
{
	DECLARE_DYNCREATE(CFileSelectBar)
// コンストラクション
public:
	CFileSelectBar();   // 標準のコンストラクタ
	~CFileSelectBar();

// ダイアログ データ
	//{{AFX_DATA(CFileSelectBar)
	CTabCtrlEx m_tab;
	//}}AFX_DATA

	//CHeaderCtrlEx m_header;
	BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );
	void AddTab(const char *szName, CObjLayTbl *pLay);
	void CloseTabs(int ctype, int sel=-1);
	void SetDocTitle(const char *szName);
	bool SelectDoc(const char *filename);
	bool SelectDoc(int sel);

	bool CloseDocs(int ctype, CMainFrame* pFrame, string *pathname=0);
	bool CanCloseFrame(CMainFrame* pFrame, int sel_no=-1);
	bool CanCloseDoc(int sel, CMainFrame* pFrame, string *pathname=0);
	int	 GetWindowCount() {
		return m_lays.size(); }
	void SetNext(int inc=1);
	string GetFileName(int sel);
	string GetTitle(int sel);
	int GetCurSel();
	void UpdateSize();
	void UpdateTip();
	void HitTab4Tip(CPoint point);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	//{{AFX_VIRTUAL(CFileSelectBar)
	public:
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL
	//virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI );
private:
	CObjLays	m_lays;
	//HANDLE m_htip;
	//CPoint	m_ptMouse;
	CToolTipCtrl	*m_ptip;
	string m_strTip;

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFileSelectBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	//afx_msg BOOL CFileSelectBar::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FILESELECTBAR_H__83AA1AB4_3CD1_11D2_AF0C_000000000000__INCLUDED_)
