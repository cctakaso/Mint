// TheView.h : CTheView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TheVIEW_H__86A4F10C_0A00_11D2_AA08_444553540000__INCLUDED_)
#define AFX_TheVIEW_H__86A4F10C_0A00_11D2_AA08_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "The.h"
#include "ViewEd.h"
#include "FindDlg.h"

//UpdateAllView hint!
#define HINT_CLOSE_DOC	(1)
#define HINT_DIRTY_VIEW	(2)
#define HINT_FOCUS_VIEW	(3)
#define HINT_LOST_FOCUS_VIEW	(4)

class CFlpMark
{
public:
	CFlpMark() {};
	~CFlpMark() {};
public:
	void SetMark(const CViewEd *pviewed) {
		m_cursor = pviewed->m_cursor;
	}
	bool JumpMark(CViewEd *pviewed) {
		if (!m_cursor.IsEmpty()) {
			CCursor	cursor = m_cursor;
			m_cursor = pviewed->m_cursor;
			pviewed->SetMCursor(cursor);
			return true;
		}
		m_cursor = pviewed->m_cursor;
		return false;
	}
private:
	CCursor	m_cursor;
};



//#define HINT_VERTICAL_VIEW (4)
class CTheView : public CView
{
	friend class CTheApp;
	friend class CFindDlg;
	friend class CJumpDlg;
	friend class CMainFrame;
	friend class CIme;
	friend class CTabCtrlEx;
protected: // シリアライズ機能のみから作成します。
	CTheView();
	DECLARE_DYNCREATE(CTheView)

// アトリビュート
public:
	CTheDoc* GetDocument();
	CViewEd		*m_pViewEd;
	//CViewEd		m_ViewEd;
	//bool	m_bAttach;
	bool	m_bDirtyView;
	bool	m_bInputView;
protected:
	CRect	m_rcClient;
//	CSize	m_sizeFont;
	int		m_idleCount;
	CFlpMark m_flpMark;
//	CFindDlg m_dlgFind;-->move to global
#ifdef SPEC_PRINT
	DRAW_MODE	m_modeDraw;
#endif //SPEC_PRINT

private:
	void UpdateOtherView();
	void OnEditCopy_Hand(string *pstrAdd=null, bool bAppend=false);
	void OnEditPaste_Hand(const char *szMark=null);
	bool OnEdtTextFormat(string *pstrAdd, const string *pstrRemove, Normal_Proc pFunc);
	void UpdateViewEnv(bool bVert_Old, bool bChangeFormat);
	bool DoTanbun();
	BOOL DoPreparePrinting(CPrintInfo* pInfo);
// オペレーション
public:
	BOOL OnIdle(LONG lCount);
	void OnImeString(const string &str, const string &strAttr, bool bResult=false, bool bRedraw=true);
	void OnJumpLine(OBJOFF lineJump, JUMPTYPE JumpType=JUMP_LINE);
	bool UpdateSelEnv(int envindex);
	void SetClocksPerSec();
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTheView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#ifdef SPEC_PRINT
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
#endif //#ifdef SPEC_PRINT
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTheView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DoScroll(UINT nSBCode, UINT nPos, int sb);
	void ScrollForSell(CCursor cursorOld, ACT act=act_input, bool bLineTop=false, bool bInput=false, CCursor cursor_update=CCursor(), OBJOFF rowNext=0, OBJOFF rowNext_After=0, bool bStrCRLF=false);
	bool SetScrollPos_Auto(int nBar, int nPos, int nPosOld, bool bVSwitch=false);
	void ShowBlinkCursor(bool bShow, bool bClear_IdleCount=true);
	void OnString(CAction *paction, CCursor &cursorBef);
	void SetPaneText_CurPos();
	void KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
// 生成されたメッセージ マップ関数
protected:
	void ShowFSBarMenu(UINT nFlags, CPoint point);
	bool ScrollForInput(OBJOFF rowNext, OBJOFF rowNext_After, bool bStrCRLF, bool bDelCRLFCheck);
	//{{AFX_MSG(CTheView)
	afx_msg LRESULT OnMessageArowKeyDown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateFreeCur(CCmdUI* pCmdUI);
	afx_msg void OnFreeCur();
	afx_msg void OnUpdateVertical(CCmdUI* pCmdUI);
	afx_msg void OnVertical();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnUpdateLinenum(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinenumLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinenumNo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinenumRow(CCmdUI* pCmdUI);
	afx_msg void OnLinenum();
	afx_msg void OnLinenumLine();
	afx_msg void OnLinenumNo();
	afx_msg void OnLinenumRow();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFindNext(CCmdUI* pCmdUI);
	afx_msg void OnEditFindNext();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateConfig(CCmdUI* pCmdUI);
	afx_msg void OnConfig();
	afx_msg void OnUpdateLimitcol(CCmdUI* pCmdUI);
	afx_msg void OnLimitcol();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateJump(CCmdUI* pCmdUI);
	afx_msg void OnJump();
	afx_msg void OnUpdateEdtView(CCmdUI* pCmdUI);
	afx_msg void OnEdtView();
	afx_msg void OnUpdateEdtlRefCopy(CCmdUI* pCmdUI);
	afx_msg void OnEdtlRefCopy();
	afx_msg void OnUpdateEdtlRefPast(CCmdUI* pCmdUI);
	afx_msg void OnEdtlRefPast();
	afx_msg void OnUpdateEditCopyAppend(CCmdUI* pCmdUI);
	afx_msg void OnEditCopyAppend();
	afx_msg void OnUpdateEdtlLeft(CCmdUI* pCmdUI);
	afx_msg void OnEdtlLeft();
	afx_msg void OnUpdateEdtlCenter(CCmdUI* pCmdUI);
	afx_msg void OnEdtlCenter();
	afx_msg void OnUpdateEdtlRight(CCmdUI* pCmdUI);
	afx_msg void OnEdtlRight();
	afx_msg void OnUpdateEdtlRefRight(CCmdUI* pCmdUI);
	afx_msg void OnEdtlRefRight();
	afx_msg void OnUpdateEdtlRefLeft(CCmdUI* pCmdUI);
	afx_msg void OnEdtlRefLeft();
	afx_msg void OnUpdateEdtlLineHeaderRight(CCmdUI* pCmdUI);
	afx_msg void OnEdtlLineHeaderRight();
	afx_msg void OnUpdateEdtlLineHeaderLeft(CCmdUI* pCmdUI);
	afx_msg void OnEdtlLineHeaderLeft();
	afx_msg void OnUpdateEdtlIndentRight(CCmdUI* pCmdUI);
	afx_msg void OnEdtlIndentRight();
	afx_msg void OnUpdateEdtlIndentLeft(CCmdUI* pCmdUI);
	afx_msg void OnEdtlIndentLeft();
	afx_msg void OnUpdateEdtlIndentAuto(CCmdUI* pCmdUI);
	afx_msg void OnEdtlIndentAuto();
	afx_msg void OnUpdateRedraw(CCmdUI* pCmdUI);
	afx_msg void OnRedraw();
	afx_msg void OnUpdateFlpMark(CCmdUI* pCmdUI);
	afx_msg void OnFlpMark();
	afx_msg void OnUpdateFlpMarkJump(CCmdUI* pCmdUI);
	afx_msg void OnFlpMarkJump();
	afx_msg void OnUpdateMark(CCmdUI* pCmdUI);
	afx_msg void OnMark();
	afx_msg void OnUpdateMarkJumpPrev(CCmdUI* pCmdUI);
	afx_msg void OnMarkJumpPrev();
	afx_msg void OnUpdateMarkJumpNext(CCmdUI* pCmdUI);
	afx_msg void OnMarkJumpNext();
	afx_msg void OnUpdateMarkRemoveall(CCmdUI* pCmdUI);
	afx_msg void OnMarkRemoveall();
	afx_msg void OnUpdateFindBracket(CCmdUI* pCmdUI);
	afx_msg void OnFindBracket();
	afx_msg void OnUpdateEditFindPrev(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrev();
	afx_msg void OnUpdateInputMark(CCmdUI* pCmdUI);
	afx_msg void OnInputMark();
	afx_msg void OnUpdateConfigEdit(CCmdUI* pCmdUI);
	afx_msg void OnConfigEdit();
	afx_msg void OnUpdateSelenv(CCmdUI* pCmdUI);
	afx_msg void OnSelenvNormal();
	afx_msg void OnSelenvStart1();
	afx_msg void OnSelenvStart2();
	afx_msg void OnSelenvStart3();
	afx_msg void OnSelenvStart4();
	afx_msg void OnSelenvStart5();
	afx_msg void OnSelenvStart6();
	afx_msg void OnSelenvStart7();
	afx_msg void OnSelenvStart8();
	afx_msg void OnSelenvStart9();
	afx_msg void OnTanbunAdd();
	afx_msg void OnUpdateTanbunAdd(CCmdUI* pCmdUI);
	afx_msg void OnTanbunEdit();
	afx_msg void OnUpdateTanbunEdit(CCmdUI* pCmdUI);
	afx_msg void OnEdtlTime();
	afx_msg void OnUpdateEdtlTime(CCmdUI* pCmdUI);
	afx_msg void OnEdtlDate();
	afx_msg void OnUpdateEdtlDate(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
//	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
//	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnWindowCloseNoThis();
	afx_msg void OnUpdateWindowCloseNoThis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTagjump(CCmdUI* pCmdUI);
	afx_msg void OnTagjump();
	afx_msg void OnUpdatePageSetup(CCmdUI* pCmdUI);
#ifdef SPEC_PRINT
	afx_msg void OnPageSetup();
#endif //#ifdef SPEC_PRINT
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // TheView.cpp ファイルがデバッグ環境の時使用されます。
inline CTheDoc* CTheView::GetDocument()
   { return (CTheDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TheVIEW_H__86A4F10C_0A00_11D2_AA08_444553540000__INCLUDED_)
