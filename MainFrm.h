// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__86A4F108_0A00_11D2_AA08_444553540000__INCLUDED_)
#define AFX_MAINFRM_H__86A4F108_0A00_11D2_AA08_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "ObjLay.h"
#include "FileSelectBar.h"

/*
#ifndef SINGLE_TEMPLATE
class CSplitters: public vector<CSplitterWnd *>//deque<SELL>
{
public:
	CSplitters(){};
	~CSplitters(){ 
		for (iterator iter=begin(); iter!=end(); iter++) {
			delete (*iter);
		}
	};

};
#endif
*/
class CMainFrame : public CFrameWnd
{
	friend class CTheApp;
	friend class CTheDoc;
	friend class CTheDocTemplate;
	friend class CTheView;
	friend class CFileSelectBar;
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// アトリビュート
protected:
//#ifdef SINGLE_TEMPLATE
	CSplitterWnd	m_wndSplitter;
//#else
//	CSplitters	m_Splitters;
//#endif

	CFileSelectBar  m_wndFileSelectBar;

	UINT m_uiTimer;
private:
public:
	unsigned int m_timeCount;
	
// オペレーション
public:
	void SetPaneText(char *sz=NULL, int indicator=0);
	void SetPaneText_CurPos(int row, int col);
	void SetPaneText_Loading(OBJSIZE off, OBJSIZE len);
	void SetPaneText_Layout(OBJSIZE off, OBJSIZE len);
	void DockToolBar(UINT nDockBarID);
	void DockControlBarOf(CControlBar* Bar, UINT nDockBarID, CControlBar* LeftOf);
	CObjLayTbl*	GetActiveObjLayTbl();
	void UpdateMenuConf(bool bUpdateAll=false);
	bool DoClose();
private:
	void ResetSelEnvMenu(CObjLayTbl* pMainObjLay, CMenu *pmenu=null);
	void ResetWindowMenu(CObjLayTbl* pMainObjLay, CMenu *pmenu=null);
#ifdef EDITMENU_OFLAY
	void OnEditMenu_NoChange() {
		if (GetActiveObjLayTbl()) {
			GetActiveObjLayTbl()->ChangeEditMenu();//dumy change
			OnEditMenu(); 
		}	}
#endif
	bool UpdateSelWindow(int sel);
	void SetWindowTopMost();
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();
	virtual BOOL LoadFrame(UINT nIDResource,
				DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
				CWnd* pParentWnd = NULL,
				CCreateContext* pContext = NULL);
	LRESULT OnMessageCheckOpenDoc(WPARAM wParam, LPARAM lParam);
	LRESULT OnMessageSetTopWindow(WPARAM wParam, LPARAM lParam);

	string &getShiteiKnjcode (){
		return m_knjcode_shitei;
	}
	void setShiteiKnjcode (LPSTR knjcode){
		m_knjcode_shitei = knjcode;
	}
//	virtual void InitialUpdateFrame( CDocument* pDoc, BOOL bMakeVisible);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndLongToolBar;
	CToolBar    m_wndEditToolBar;

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnUpdateTopmost(CCmdUI* pCmdUI);
	afx_msg void OnTopmost();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEdtl();
	afx_msg void OnUpdateEditMenu(CCmdUI* pCmdUI);
	afx_msg void OnEditMenu();
	afx_msg void OnUpdateEdtl(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateHelpIndex(CCmdUI* pCmdUI);
	afx_msg void OnHelpIndex();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnWindowCloseNoThis();
	afx_msg void OnUpdateWindowCloseNoThis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowNext(CCmdUI* pCmdUI);
	afx_msg void OnWindowNext();
	afx_msg void OnWindowPrev();
	afx_msg void OnUpdateWindowPrev(CCmdUI* pCmdUI);
	afx_msg void OnWindowCloseAll();
	afx_msg void OnUpdateWindowCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelWindow(CCmdUI* pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnWindow1();
	afx_msg void OnWindow2();
	afx_msg void OnWindow3();
	afx_msg void OnWindow4();
	afx_msg void OnWindow5();
	afx_msg void OnWindow6();
	afx_msg void OnWindow7();
	afx_msg void OnWindow8();
	afx_msg void OnWindow9();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnUpdateViewFileSelecter(CCmdUI* pCmdUI);
	afx_msg void OnViewFileSelecter();
	afx_msg void OnUpdateExecApp(CCmdUI* pCmdUI);
	afx_msg void OnExecApp();
	afx_msg void OnUpdateMoveTop(CCmdUI* pCmdUI);
	afx_msg void OnMoveTop();
	afx_msg void OnUpdateMoveTopShit(CCmdUI* pCmdUI);
	afx_msg void OnMoveTopShit();
	afx_msg void OnUpdateWindowOthers(CCmdUI* pCmdUI);
	afx_msg void OnWindowOthers();
	afx_msg void OnFileOpenRetry();
	afx_msg void OnUpdateFileOpenRetry(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenSjis();
	afx_msg void OnUpdateFileOpenSjis(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenJis();
	afx_msg void OnUpdateFileOpenJis(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenEuc();
	afx_msg void OnUpdateFileOpenEuc(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenUtf8();
	afx_msg void OnUpdateFileOpenUtf8(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void FileOpenRetry(LPSTR knjcode=null);
	string m_knjcode_shitei;
public:
	afx_msg void OnFileNewJis();
	afx_msg void OnUpdateFileNewJis(CCmdUI *pCmdUI);
	afx_msg void OnFileNewEuc();
	afx_msg void OnUpdateFileNewEuc(CCmdUI *pCmdUI);
	afx_msg void OnFileNewUtf8();
	afx_msg void OnUpdateFileNewUtf8(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__86A4F108_0A00_11D2_AA08_444553540000__INCLUDED_)
