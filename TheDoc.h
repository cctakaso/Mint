// TheDoc.h : CTheDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TheDOC_H__86A4F10A_0A00_11D2_AA08_444553540000__INCLUDED_)
#define AFX_TheDOC_H__86A4F10A_0A00_11D2_AA08_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "ObjCash.h"

class CMainFrame;
class CTheDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CTheDoc();
	DECLARE_DYNCREATE(CTheDoc)

// アトリビュート
public:
	CObjLayTbl*		m_pMainObjLay;	//pointer of main object layout.

// オペレーション
public:
	void SetDirtyDocTitle(bool bDirty=true);
	void SelectDocument(CObjLayTbl *pLay);
	void SetTitleStringOnly(const char *sz) {
		m_strTitle = sz; }
	void SetDirtyPahtName(CString &str, bool bDirty);
	void FileSave() {
		OnFileSave();
	}
	void UpdateFileSave(CCmdUI* pCmdUI) {
		OnUpdateFileSave(pCmdUI);
	}

	//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTheDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, CMainFrame* pMainFrame, BOOL bAddToMRU = TRUE);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTheDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTheDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSaveAsSjis();
	afx_msg void OnUpdateFileSaveAsSjis(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAsJis();
	afx_msg void OnUpdateFileSaveAsJis(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAsEuc();
	afx_msg void OnUpdateFileSaveAsEuc(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAsUtf8();
	afx_msg void OnUpdateFileSaveAsUtf8(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TheDOC_H__86A4F10A_0A00_11D2_AA08_444553540000__INCLUDED_)
