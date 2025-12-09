// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "The.h"
#include "TheDoc.h"
#include "TheView.h"
#include "ViewEd.h"

#include "MainFrm.h"
#include "ViewEnv.h"
#include "FileListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CViewEnvTbl theViewEnvTbl;
extern bool vIsExist_prev_The;
extern CTheApp theApp;
extern bool svbAppOnlyOne;
extern CMyCmdLine vscmdInfo;

/////////////////////////////////////////////////////////////////////////////
// サーポートＵＲＬ
static char szSupportUrl[] = "http://hp.vector.co.jp/authors/VA052798/mint";
void SuportUrlBrowse(HWND hwnd) 
{
	HINSTANCE hInst;
	hInst = ShellExecute(hwnd, "open",
			szSupportUrl, NULL, NULL, SW_SHOWNORMAL);
	if ((UINT)hInst<=31) {
		if (::WinExec(szSupportUrl, SW_SHOWNORMAL)<=31) {
			if ((UINT)(hInst = ShellExecute(hwnd, NULL/*"open"*/,
				szSupportUrl, NULL, NULL, SW_SHOW))<=31)
				;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_TOPMOST, OnUpdateTopmost)
	ON_COMMAND(ID_TOPMOST, OnTopmost)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_EDTL, OnEdtl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MENU, OnUpdateEditMenu)
	ON_COMMAND(ID_EDIT_MENU, OnEditMenu)
	ON_UPDATE_COMMAND_UI(ID_EDTL, OnUpdateEdtl)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_HELP_INDEX, OnUpdateHelpIndex)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_UPDATE_COMMAND_UI(IDC_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(IDC_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEXT, OnUpdateWindowNext)
	ON_COMMAND(ID_WINDOW_NEXT, OnWindowNext)
	ON_COMMAND(ID_WINDOW_PREV, OnWindowPrev)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_PREV, OnUpdateWindowPrev)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL, OnUpdateWindowCloseAll)
	ON_COMMAND(ID_WINDOW_CLOSE_NO_THIS, OnWindowCloseNoThis)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_NO_THIS, OnUpdateWindowCloseNoThis)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_1, OnUpdateSelWindow)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_WINDOW_1, OnWindow1)
	ON_COMMAND(ID_WINDOW_2, OnWindow2)
	ON_COMMAND(ID_WINDOW_3, OnWindow3)
	ON_COMMAND(ID_WINDOW_4, OnWindow4)
	ON_COMMAND(ID_WINDOW_5, OnWindow5)
	ON_COMMAND(ID_WINDOW_6, OnWindow6)
	ON_COMMAND(ID_WINDOW_7, OnWindow7)
	ON_COMMAND(ID_WINDOW_8, OnWindow8)
	ON_COMMAND(ID_WINDOW_9, OnWindow9)
	ON_WM_GETMINMAXINFO()
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILE_SELECTER, OnUpdateViewFileSelecter)
	ON_COMMAND(ID_VIEW_FILE_SELECTER, OnViewFileSelecter)
	ON_UPDATE_COMMAND_UI(ID_EXEC_APP, OnUpdateExecApp)
	ON_COMMAND(ID_EXEC_APP, OnExecApp)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_2, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_3, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_4, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_5, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_6, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_7, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_8, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_9, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_10, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_11, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_12, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_13, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_14, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_15, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_16, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_17, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_18, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_19, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_20, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_21, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_22, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_23, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_24, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_25, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_26, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_27, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_28, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_29, OnUpdateSelWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_30, OnUpdateSelWindow)
	ON_MESSAGE(WM_CHECK_OPENDOC, OnMessageCheckOpenDoc)
	ON_MESSAGE(WM_SET_TOPWINDOW, OnMessageSetTopWindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_OTHERS, OnUpdateWindowOthers)
	ON_COMMAND(ID_WINDOW_OTHERS, OnWindowOthers)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_OPEN_RETRY, &CMainFrame::OnFileOpenRetry)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_RETRY, &CMainFrame::OnUpdateFileOpenRetry)
	ON_COMMAND(ID_FILE_OPEN_SJIS, &CMainFrame::OnFileOpenSjis)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_SJIS, &CMainFrame::OnUpdateFileOpenSjis)
	ON_COMMAND(ID_FILE_OPEN_JIS, &CMainFrame::OnFileOpenJis)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_JIS, &CMainFrame::OnUpdateFileOpenJis)
	ON_COMMAND(ID_FILE_OPEN_EUC, &CMainFrame::OnFileOpenEuc)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_EUC, &CMainFrame::OnUpdateFileOpenEuc)
	ON_COMMAND(ID_FILE_OPEN_UTF8, &CMainFrame::OnFileOpenUtf8)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_UTF8, &CMainFrame::OnUpdateFileOpenUtf8)
	ON_COMMAND(ID_FILE_NEW_JIS, &CMainFrame::OnFileNewJis)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_JIS, &CMainFrame::OnUpdateFileNewJis)
	ON_COMMAND(ID_FILE_NEW_EUC, &CMainFrame::OnFileNewEuc)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_EUC, &CMainFrame::OnUpdateFileNewEuc)
	ON_COMMAND(ID_FILE_NEW_UTF8, &CMainFrame::OnFileNewUtf8)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_UTF8, &CMainFrame::OnUpdateFileNewUtf8)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_ROW_CAL,
	ID_INDICATOR_FONT_PT,
	ID_KNJCODE,
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


void CMainFrame::SetPaneText_CurPos(int row, int col)
{
	string strT = itostring(row,10);
	string strRow = string(strT.size()>4 ? 0:4-strT.size(), ' ') + strT;
	strT = itostring(col,10);
	string strCol = string(strT.size()>3 ? 0:3-strT.size(), ' ') + strT;
	string str = strRow + " 行 " + strCol + " 桁";
	m_wndStatusBar.SetPaneText(1, str.c_str());
}
void CMainFrame::SetPaneText_Loading(OBJSIZE off, OBJSIZE len)
{
	string str;
	if (off>=len)
		str = "100% ﾛｰﾃﾞｨﾝｸﾞ終了";
	else
		str = " " + itostring(off*100/len,10) + "% ﾛｰﾃﾞｨﾝｸﾞ中";
	m_wndStatusBar.SetPaneText(0, str.c_str());
}

void CMainFrame::SetPaneText_Layout(OBJSIZE off, OBJSIZE len)
{
	string str;
	if (off>=len)
		str = "100% ﾚｲｱｳﾄ終了";
	else
		str = " " + itostring(off*100/len,10) + "% ﾚｲｱｳﾄ 中";
	m_wndStatusBar.SetPaneText(0, str.c_str());
}

void CMainFrame::SetPaneText(char *sz/*=NULL*/, int indicator/*=0*/)
{
	if (sz)
		m_wndStatusBar.SetPaneText(indicator, sz);
	else
		{
		m_wndStatusBar.SetPaneText(0, "");
	//	m_wndStatusBar.SetPaneText(1, "");
		}
	m_wndStatusBar.UpdateWindow();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	if (!m_wndLongToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0, 0, 0, 0), AFX_IDW_RESIZE_BAR+1) ||
		!m_wndLongToolBar.LoadToolBar(IDR_MAINFRAME_EDIT))
	{
		TRACE0("Failed to create tool-view bar\n");
		return -1;      // 作成に失敗
	}

	if (!m_wndEditToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0, 0, 0, 0), AFX_IDW_RESIZE_BAR+2) ||
		!m_wndEditToolBar.LoadToolBar(IDR_EDT_TOOL))
	{
		TRACE0("Failed to create tool-edit bar\n");
		return -1;      // 作成に失敗
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}
	//if (!m_wndFileSelectBar.Create(this, IDD_FILE_SELECT_BAR,
	//	WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
	//		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_FILE_SELECT_BAR))

	if (!m_wndFileSelectBar.Create(this, IDD_FILE_SELECT_BAR,
		WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_FILE_SELECT_BAR))
	{
		TRACE0("Failed to create DlgBar\n");
		return -1;      // fail to create
	}


	// TODO: もしツール チップスが必要ない場合、ここを削除してください。
//	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

//	m_wndToolBar.SetBarStyle(m_wndEditToolBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: ツール バーをドッキング可能にしない場合は以下の３行を削除
	//       してください。
	m_wndToolBar.SetWindowText(_T(""));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLongToolBar.SetWindowText(_T(""));
	m_wndLongToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndEditToolBar.SetWindowText(_T("整形ﾂｰﾙ"));
	m_wndEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileSelectBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndLongToolBar);
	DockControlBar(&m_wndEditToolBar);
	DockControlBar(&m_wndFileSelectBar);

	//OnEdtl();
	ShowControlBar(&m_wndToolBar, !theViewEnvTbl.m_bEditMenu, FALSE);
	ShowControlBar(&m_wndLongToolBar, theViewEnvTbl.m_bEditMenu, FALSE);
	if (theViewEnvTbl.m_bEditMenu) {
		theViewEnvTbl.m_bEditMenu = false;
		OnEditMenu();
		theViewEnvTbl.m_bEditMenu = true;
	}

	
	ShowControlBar(&m_wndEditToolBar, theViewEnvTbl.m_bEditToolBarView, FALSE);
	if (theViewEnvTbl.m_bEditToolBarView) {
		CRect rc = theViewEnvTbl.m_rcEditToolBar;
		if (theViewEnvTbl.m_bFloat_EditToolBar) {
			CPoint pt;
			pt.x = rc.left;
			pt.y = rc.top;
			FloatControlBar(&m_wndEditToolBar, pt, CBRS_ALIGN_TOP );
			//m_wndEditToolBar.MoveWindow(rc);
		}else if (!rc.IsRectEmpty()){
			if (rc.left>10) {
				//rc.left--;
				//m_wndEditToolBar.MoveWindow(rc);
				//DockControlBar(&m_wndEditToolBar, CBRS_ALIGN_TOP, rc);
				DockControlBarOf(&m_wndEditToolBar, AFX_IDW_DOCKBAR_LEFT,
					theViewEnvTbl.m_bEditMenu ? &m_wndLongToolBar:&m_wndToolBar);
			}
		}
	}
	
	if (theViewEnvTbl.m_bFileSelecter) {
		CRect rc = theViewEnvTbl.m_rcFileSelectBar;
		if (theViewEnvTbl.m_bFloat_FileSelectBar) {
			CPoint pt;
			pt.x = rc.left;
			pt.y = rc.top;
			FloatControlBar(&m_wndFileSelectBar, pt, CBRS_ALIGN_TOP );
			//m_wndFileSelectBar.MoveWindow(rc);
		}else if (!rc.IsRectEmpty()){
			if (rc.left>10) {
				//rc.top -- ;
				DockControlBarOf(&m_wndFileSelectBar, AFX_IDW_DOCKBAR_LEFT,
					theViewEnvTbl.m_rcEditToolBar.top==rc.top ? &m_wndEditToolBar:
					(theViewEnvTbl.m_bEditMenu ? &m_wndLongToolBar:&m_wndToolBar));
			}
		}
	}
	ShowControlBar(&m_wndFileSelectBar, theViewEnvTbl.m_bFileSelecter, FALSE);
//	LoadBarState(_T("General"));

	//カーソル用にタイマを設定します
	for (m_uiTimer = 16; SetTimer(m_uiTimer, 100, NULL) == 0 && --m_uiTimer>0; );
	m_timeCount = 0;
	return 0;
}
void CMainFrame::DockToolBar(UINT nDockBarID) 
{

	//	DockControlBar(&m_wndEditToolBar, nDockBarID); 
	CRect rc, rcWin;
	GetWindowRect(&rcWin);
	if (nDockBarID==AFX_IDW_DOCKBAR_RIGHT) {
		if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_RIGHT)
			return;
		rc = CRect(rcWin.right, rcWin.top, rcWin.right, rcWin.bottom);
	}else{
		if (m_wndToolBar.GetBarStyle() & CBRS_ALIGN_TOP)
			return;
		rc = CRect(rcWin.left, rcWin.top, rcWin.right, rcWin.top);
	}
	DockControlBar(m_wndToolBar.IsWindowVisible() ? &m_wndToolBar:&m_wndLongToolBar, nDockBarID, &rc); 
	DockControlBar(m_wndToolBar.IsWindowVisible() ? &m_wndLongToolBar:&m_wndToolBar, nDockBarID, &rc); 
}
void CMainFrame::DockControlBarOf(CControlBar* Bar, UINT nDockBarID, CControlBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	switch(nDockBarID) {
	case AFX_IDW_DOCKBAR_TOP:
		rect.OffsetRect(0,-1);
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		rect.OffsetRect(0,1);
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		rect.OffsetRect(1,0);
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		rect.OffsetRect(-1,0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
//#ifdef SINGLE_TEMPLATE
	return m_wndSplitter.Create( this,
		2, 2,                 // TODO: 行と列の数を調整してください。
		CSize( 10, 10 ),      // TODO: 最小の区画サイズを調整してください。
		pContext );
	return TRUE;
/*#else
	CSplitterWnd *pwnd = new CSplitterWnd;
	if (pwnd->Create( this,
	2, 2,                 // TODO: 行と列の数を調整してください。
		CSize( 10, 10 ),      // TODO: 最小の区画サイズを調整してください。
		pContext )) {
		m_Splitters.push_back(pwnd);
		//SetActiveView((CView*)pwnd->GetPane(0,0));
		return TRUE;
	}
	return FALSE;
#endif
	*/
}
/*
void CMainFrame::InitialUpdateFrame( CDocument* pDoc, BOOL bMakeVisible)
{
#define WM_INITIALUPDATE    0x0364 
	// if the frame does not have an active view, set to first pane
	CView* pView = NULL;
	if (GetActiveView() == NULL)
	{
		CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pView = (CView*)pWnd;
			SetActiveView(pView, FALSE);
		}
	}

	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		if (GetActiveView() != NULL)
			GetActiveView()->OnInitialUpdate();
		else
			SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

		// give view a chance to save the focus (CFormView needs this)
//		if (pView != NULL)
//			pView->OnActivateFrame(WA_INACTIVE, this);

		// finally, activate the frame
		// (send the default show command unless the main desktop window)
		int nCmdShow = -1;      // default
		CWinApp* pApp = AfxGetApp();
		if (this == pApp->m_pMainWnd)
		{
			nCmdShow = pApp->m_nCmdShow; // use the parameter from WinMain
			pApp->m_nCmdShow = -1; // set to default after first time
		}
		ActivateFrame(nCmdShow);
		if (pView != NULL)
				pView->SetFocus();
//pView->OnActivateView(TRUE, pView, pView);
	}

	// update frame counts and frame title (may already have been visible)
	if (pDoc != NULL)
		pDoc->UpdateFrameCounts();
	OnUpdateFrameTitle(TRUE);
}
*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//       修正してください。
	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	m_timeCount++;
	CFrameWnd::OnTimer(nIDEvent);
//	MyPeekMessage(m_hWnd);

}

void CMainFrame::OnClose() 
{
//	CFrameWnd::OnClose();
//	SaveBarState("General");
	CRect rc;
	if (theViewEnvTbl.m_bEditToolBarView) {
		m_wndEditToolBar.GetWindowRect(&rc);
		theViewEnvTbl.m_bFloat_EditToolBar = m_wndEditToolBar.IsFloating()==TRUE;
		if (!theViewEnvTbl.m_bFloat_EditToolBar)
			ScreenToClient(&rc);
		theViewEnvTbl.m_rcEditToolBar.left = rc.left;
		theViewEnvTbl.m_rcEditToolBar.right = rc.right;
		theViewEnvTbl.m_rcEditToolBar.top = rc.top;
		theViewEnvTbl.m_rcEditToolBar.bottom = rc.bottom;
//			GetWindowRect(&rc);
//			GetParent()->GetWindowRect(&rcClient);
//			rc.right = rcClient.right;
//			GetParent()->ScreenToClient(&rc);
//			MoveWindow(&rc);
		//}
	}
	if (theViewEnvTbl.m_bFileSelecter) {
		m_wndFileSelectBar.GetWindowRect(&rc);
		theViewEnvTbl.m_bFloat_FileSelectBar = m_wndFileSelectBar.IsFloating()==TRUE;
		if (!theViewEnvTbl.m_bFloat_FileSelectBar)
			ScreenToClient(&rc);
		theViewEnvTbl.m_rcFileSelectBar.left = rc.left;
		theViewEnvTbl.m_rcFileSelectBar.right = rc.right;
		theViewEnvTbl.m_rcFileSelectBar.top = rc.top;
		theViewEnvTbl.m_rcFileSelectBar.bottom = rc.bottom;
	}
	if (!AfxGetApp()->m_pMainWnd->IsIconic() && !AfxGetApp()->m_pMainWnd->IsZoomed())
		{
		AfxGetApp()->m_pMainWnd->GetWindowRect(&rc);
		theViewEnvTbl.m_rcClient.left = rc.left;
		theViewEnvTbl.m_rcClient.right = rc.right;
		theViewEnvTbl.m_rcClient.top = rc.top;
		theViewEnvTbl.m_rcClient.bottom = rc.bottom;
		}
	if (DoClose()) {
		//close!
	//	if (m_pMainObjLay && !m_pMainObjLay->IsFormatAll()) {
	//		AfxMessageBox("遅延ﾛｰﾃﾞｨﾝｸﾞ中です。");
	//		return ;
	//	}
	}
}
bool CMainFrame::DoClose() 
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return false;

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	//if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	if (pDocument != NULL && !m_wndFileSelectBar.CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return false;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
			return false;     // don't close it

		//timer kill & flag on
		if (m_uiTimer>0)
			KillTimer(m_uiTimer);
		theApp.m_bDestroy = true;

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return false;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return true;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return false;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// then destroy the window
	DestroyWindow();
	return true;
}


void CMainFrame::OnUpdateTopmost(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(theViewEnvTbl.m_bTopMost);
}

void CMainFrame::OnTopmost() 
{
	::SetWindowPos(GetSafeHwnd(),
		(theViewEnvTbl.m_bTopMost = !theViewEnvTbl.m_bTopMost) ?
		 HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0,
		 SWP_NOMOVE | SWP_NOSIZE);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	TRACE("\nCMainFrame::OnActivate()!");
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
#if BUG_20111203
	if (nState==WA_ACTIVE || nState==WA_CLICKACTIVE) {
		Invalidate();
		//CView *pView = GetActiveView();
		//if (pView)
		//	pView->Invalidate();
	}
#endif
}

#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children from afxprv.h
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{

	if (AfxGetApp()->m_pMainWnd->GetSafeHwnd())
		return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle,
				pParentWnd, pContext);
	// only do this once
	//ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	CRect rc;
//	if (!vIsExist_prev_The) { //一番最初のｳｨﾝﾄﾞｳだけ
		if (theViewEnvTbl.m_rcClient.IsRectEmpty())
			::SetRect(&rc, CW_USEDEFAULT,CW_USEDEFAULT,0,0);	// set default size
		else
			rc = theViewEnvTbl.m_rcClient;
//	}else
//		::SetRect(&rc, CW_USEDEFAULT,CW_USEDEFAULT,0,0);	// set default size

	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  strTitle, dwDefaultStyle /*^ WS_VISIBLE*/, rc,
	  //pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
	  pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
		return FALSE;   // will self destruct on failure normally

	LoadAccelTable(MAKEINTRESOURCE(nIDResource));

	if (pContext == NULL)   // send initial update
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE);

	return TRUE;
}

void CMainFrame::OnUpdateEdtl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(theViewEnvTbl.m_bEditToolBarView=m_wndEditToolBar.IsWindowVisible()==TRUE);
}

void CMainFrame::OnEdtl() 
{
#ifdef SHAR
	//DockControlBar(&m_wndEditToolBar,AFX_IDW_DOCKBAR_BOTTOM);
	theViewEnvTbl.m_bEditToolBarView = !theViewEnvTbl.m_bEditToolBarView;
	CRect rect;
	CPoint pt;
	GetWindowRect(&rect);
	pt.x = (rect.left + rect.right)/2;
	pt.y = (rect.top + rect.bottom)/2;
	FloatControlBar(&m_wndEditToolBar, pt, CBRS_ALIGN_TOP );
	ShowControlBar(&m_wndEditToolBar, theViewEnvTbl.m_bEditToolBarView, FALSE);
#else
	StopFreeMsg();
#endif
}

void CMainFrame::UpdateMenuConf(bool bUpdateAll)
{
#ifdef SHAR
	CObjLayTbl* pMainObjLay;
	if (pMainObjLay=GetActiveObjLayTbl()) {
		if (bUpdateAll || (IsWindowVisible() && theViewEnvTbl.m_bEditMenu ? 
			GetMenu()->GetMenuItemCount()!=8:GetMenu()->GetMenuItemCount()!=6)) {
			theViewEnvTbl.m_bEditMenu = !theViewEnvTbl.m_bEditMenu; //dumy change!
			OnEditMenu();
		}else{
			ResetSelEnvMenu(pMainObjLay);
			ResetWindowMenu(pMainObjLay);
		}
	}
#else //!SHAR
	StopFreeMsg();
#endif
}
void CMainFrame::ResetSelEnvMenu(CObjLayTbl* pMainObjLay, CMenu *pmenu)
{
#ifdef SHAR
	bool bRedraw=false;
	if (pmenu==null) {
		pmenu = GetMenu();
		bRedraw = true;
	}
	int i;
	CViewEnvTbl::iterator iter;
	CMenu *pmenuSub = pmenu->GetSubMenu((theViewEnvTbl.m_bEditMenu ? 6:4));
	for(i=ID_SELENV_START1; pmenuSub->DeleteMenu(i,  MF_BYCOMMAND); i++);
	for (i = 0, iter=theViewEnvTbl.begin(); i<=(ID_SELENV_START30-ID_SELENV_START1) && iter!=theViewEnvTbl.end(); iter++, i++) {
		string strName = (*iter).first;
		if (strName!=NORMAL_NAME) {
			if (i<9) {
				char ch = '1'+i;
				strName += "(&";
				strName += ch;
				strName += ')';
				strName += "\tAlt+";
				strName += ch;
			}
			pmenuSub->AppendMenu(MF_STRING | MF_ENABLED, ID_SELENV_START1+i, strName.data());
		}
	}
	if (bRedraw)
		DrawMenuBar();
#else //SHAR
	StopFreeMsg();
#endif
}
void CMainFrame::ResetWindowMenu(CObjLayTbl* pMainObjLay, CMenu *pmenu)
{
//#ifdef SHAR
	if (!theViewEnvTbl.m_bEditMenu)
		return ;
	bool bRedraw=false;
	if (pmenu==null) {
		pmenu = GetMenu();
		bRedraw = true;
	}
	int i;
	CMenu *pmenuSub = pmenu->GetSubMenu((theViewEnvTbl.m_bEditMenu ? 5:4));
	for(i=ID_WINDOW_1; pmenuSub->DeleteMenu(i,  MF_BYCOMMAND); i++);
	for (i = 0; i<9 && i<m_wndFileSelectBar.GetWindowCount(); i++) {
		string strName = m_wndFileSelectBar.GetTitle(i);
		string strT = strName;
		char ch = '1'+i;
		strName = " &";
		strName += ch;
		strName += ' ';
		strName += strT;
		if (i==8 && m_wndFileSelectBar.GetWindowCount()>=9)
			strName += " .....";
		strName += "\tCtrl+";
		strName += ch;
		pmenuSub->InsertMenu(ID_WINDOW_OTHERS, MF_BYCOMMAND | MF_STRING | MF_ENABLED | 
			(m_wndFileSelectBar.GetCurSel()==i ? MF_CHECKED:MF_UNCHECKED),
			ID_WINDOW_1+i, strName.data());
	}
		
	if (bRedraw)
		DrawMenuBar();
//#else
//	StopFreeMsg();
//#endif
}
void CMainFrame::OnUpdateEditMenu(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(theViewEnvTbl.m_bEditMenu);
/*
	CObjLayTbl* pMainObjLay;
	if (pMainObjLay=GetActiveObjLayTbl()) {
		pCmdUI->SetCheck(theViewEnvTbl.m_bEditMenu);
		CMenu *pMenu = GetMenu();
		if (pMenu) {
			int count = pMenu->GetMenuItemCount();
			if ((IsWindowVisible() && theViewEnvTbl.m_bEditMenu ? count!=9:count!=7)) {
				theViewEnvTbl.m_bEditMenu = !theViewEnvTbl.m_bEditMenu;
				OnEditMenu();
				//ShowControlBar(&m_wndToolBar, TRUE, TRUE);
				//ShowControlBar(&m_wndLongToolBar, FALSE, TRUE);
			}
		}
	}
*/
}

void CMainFrame::OnEditMenu() 
{
#ifdef SHAR
	CObjLayTbl* pMainObjLay;
	if (pMainObjLay=GetActiveObjLayTbl()) {
		CMenu menu;
		CMenu *pmenuOld = GetMenu();
		theViewEnvTbl.m_bEditMenu = !theViewEnvTbl.m_bEditMenu;
		menu.LoadMenu(theViewEnvTbl.m_bEditMenu ? IDR_MAINFRAME_EDIT:IDR_MAINFRAME);
		ResetSelEnvMenu(pMainObjLay, &menu);
		ResetWindowMenu(pMainObjLay, &menu);
		if (SetMenu(&menu)) {
			if (m_wndToolBar.IsWindowVisible() || m_wndLongToolBar.IsWindowVisible()) {
				CToolBar *pToolShow, *pToolHide;
				pToolHide = theViewEnvTbl.m_bEditMenu ? &m_wndToolBar:&m_wndLongToolBar;
				pToolShow = theViewEnvTbl.m_bEditMenu ? &m_wndLongToolBar:&m_wndToolBar;
				ShowControlBar(pToolHide, FALSE, FALSE);
				ShowControlBar(pToolShow, TRUE, FALSE);
				CRect rc;
				m_wndEditToolBar.GetWindowRect(rc);
				ScreenToClient(&rc);
				if (rc.left>10) {
						DockControlBarOf(&m_wndEditToolBar, AFX_IDW_DOCKBAR_LEFT,
							theViewEnvTbl.m_bEditMenu ? &m_wndLongToolBar:&m_wndToolBar);
				}
			}
			//DockControlBar((CControlBar*)pToolShow, ((CTheView*)GetActiveView())->m_pViewEd->IsHorz() ?
			//		AFX_IDW_DOCKBAR_TOP:AFX_IDW_DOCKBAR_RIGHT);
			if (pmenuOld->m_hMenu)
				pmenuOld->DestroyMenu();
			menu.Detach();
		}
	}
#else//SHAR
	StopFreeMsg();
#endif
}
CObjLayTbl*	CMainFrame::GetActiveObjLayTbl()
{
	if (GetActiveDocument())
		return ((CTheDoc*)GetActiveDocument())->m_pMainObjLay;
	return null;
}



BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (wParam==ID_SELENV_NORMAL || wParam>=ID_SELENV_START1 && wParam<=ID_SELENV_START30) {
		((CTheView*)GetActiveView())->UpdateSelEnv(wParam-ID_SELENV_START1);
		return 0;
	}else if (wParam>=ID_WINDOW_1 && wParam<=ID_WINDOW_30) {
		UpdateSelWindow(wParam-ID_WINDOW_1);
		return 0;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CFrameWnd::RecalcLayout(bNotify);
}


void CMainFrame::OnUpdateHelpIndex(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHelpIndex() 
{
#ifdef HTML_HELP
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strFile = ((CFdiverApp *)AfxGetApp())->PGetDefaltPath();
	strFile += "\\mint_man.html";
	HINSTANCE hInst = ShellExecute(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), "open",
		strFile, NULL, NULL, SW_SHOWNORMAL);
	if ((UINT)hInst<=31) {
		if (::WinExec(strFile, SW_SHOWNORMAL)<=31) {
			CString str;
			str.LoadString(IDS_MSG_SHOW_MAN);
			str += "\n[";
			str += strFile;
			str += "]";
			AfxMessageBox(str.GetBuffer(0), MB_OK);
		}
	}
#else
	SuportUrlBrowse(AfxGetApp()->m_pMainWnd->GetSafeHwnd());
	//WinHelp(0, HELP_FINDER);
#endif
}


void CMainFrame::OnFileClose() 
{
	m_wndFileSelectBar.CloseDocs(CLOSE_THIS, this);
}
void CMainFrame::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnWindowNext() 
{
	m_wndFileSelectBar.SetNext();
}
void CMainFrame::OnUpdateWindowNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndFileSelectBar.GetWindowCount()>1);
}

void CMainFrame::OnWindowPrev() 
{
	m_wndFileSelectBar.SetNext(-1);
}
void CMainFrame::OnUpdateWindowPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndFileSelectBar.GetWindowCount()>1);
}

void CMainFrame::OnWindowCloseAll() 
{
	m_wndFileSelectBar.CloseDocs(CLOSE_ALL, this);
}
void CMainFrame::OnUpdateWindowCloseAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnWindowCloseNoThis() 
{
	m_wndFileSelectBar.CloseDocs(CLOSE_NOT_THIS, this);
}
void CMainFrame::OnUpdateWindowCloseNoThis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndFileSelectBar.GetWindowCount()>1);
}

bool CMainFrame::UpdateSelWindow(int sel) 
{
	m_wndFileSelectBar.SelectDoc(sel);
	return true;
}

void CMainFrame::OnUpdateSelWindow(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(GetDocument()->m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck((m_wndFileSelectBar.GetCurSel()+ID_WINDOW_1) == pCmdUI->m_nID);
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	CObjLayTbl* pMainObjLay;
	if (pMainObjLay=GetActiveObjLayTbl()) {
		if (theViewEnvTbl.m_bEditMenu && nIndex== 5)
			ResetWindowMenu(pMainObjLay/*, pPopupMenu*/);
	}	
}

void CMainFrame::OnWindow1() 
{
	UpdateSelWindow(0);
}

void CMainFrame::OnWindow2() 
{
	UpdateSelWindow(1);
}

void CMainFrame::OnWindow3() 
{
	UpdateSelWindow(2);
}

void CMainFrame::OnWindow4() 
{
	UpdateSelWindow(3);
}

void CMainFrame::OnWindow5() 
{
	UpdateSelWindow(4);
}

void CMainFrame::OnWindow6() 
{
	UpdateSelWindow(5);
}

void CMainFrame::OnWindow7() 
{
	UpdateSelWindow(6);
}

void CMainFrame::OnWindow8() 
{
	UpdateSelWindow(7);
}

void CMainFrame::OnWindow9() 
{
	UpdateSelWindow(8);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_wndFileSelectBar.UpdateSize();	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnUpdateViewFileSelecter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(theViewEnvTbl.m_bFileSelecter=m_wndFileSelectBar.IsWindowVisible()==TRUE);
}

void CMainFrame::OnViewFileSelecter() 
{
	//FloatControlBar(&m_wndEditToolBar, pt, CBRS_ALIGN_TOP );
	theViewEnvTbl.m_bFileSelecter = !theViewEnvTbl.m_bFileSelecter;
	ShowControlBar(&m_wndFileSelectBar, theViewEnvTbl.m_bFileSelecter, FALSE);
}


LRESULT CMainFrame::OnMessageCheckOpenDoc(WPARAM wParam, LPARAM lParam)
{
	CString str = AfxGetApp()->GetProfileString("General", "CheckDoc", "");
	CString strFileName;

	bool bAppOnlyOne = wParam!=0;

	if (bAppOnlyOne)
		SetWindowTopMost();
	
	if (!str.IsEmpty()) {
		CString strT;
		if (str[0]=='"')
			strT = str.Mid(1);
		int pos = strT.Find('"');
		if (pos>=0) {
			strFileName = strT.Left(pos);
			string stOptions, stPart;
			stOptions = strT.Mid(pos+1);
			while((stPart = GetPartString(stOptions, ' ')).size()>0) {
				vscmdInfo.ParseParam(stPart.c_str(), TRUE, FALSE);
			}
		}
	}
	bool bSelect=false;
	if (!strFileName.IsEmpty()) {
		if (m_wndFileSelectBar.SelectDoc(strFileName)) {
			bSelect =  true;
			if (!bAppOnlyOne)
				SetWindowTopMost();
		}
		if (bAppOnlyOne)
			theApp.OpenDocumentFile(strFileName);
	}
	if (bAppOnlyOne || bSelect) {
		CWnd *pWnd = GetActiveView();
		if (pWnd) {
			pWnd->SetActiveWindow();
		}else{
			pWnd = this;
		}
		pWnd->SetForegroundWindow();//SetFocus();全然効かない
		return true;
	}
	return false;
}
void CMainFrame::SetWindowTopMost()
{
	::SetWindowPos(GetSafeHwnd(),
		 HWND_TOPMOST,0,0,0,0,
		 SWP_NOMOVE | SWP_NOSIZE);
	if (!theViewEnvTbl.m_bTopMost) {
		::SetWindowPos(GetSafeHwnd(),
			 HWND_NOTOPMOST,0,0,0,0,
			 SWP_NOMOVE | SWP_NOSIZE);
	}
}


LRESULT CMainFrame::OnMessageSetTopWindow(WPARAM, LPARAM lParam)
{
	return true;
}

void CMainFrame::OnUpdateExecApp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}


void CMainFrame::OnExecApp() 
{
#if BUG_20121018_KIDOUFUKA
	str += GetModuleDirDir(theApp.m_hInstance).data();
	str += "mint.exe\" /N";
#else
	char vz[1024];
	string str = "\"";

	GetModuleFileName(theApp.m_hInstance, (char *)vz, sizeof(vz));

	str += vz;
	str += "\" /N";
#endif

	CRect rc;
	GetWindowRect(&rc);
	OffsetRect(&rc, GetSystemMetrics(SM_CYCAPTION), GetSystemMetrics(SM_CYCAPTION));
	if (rc.left>GetSystemMetrics(SM_CXSCREEN)/2 || rc.top>GetSystemMetrics(SM_CYSCREEN)/2) {
		OffsetRect(&rc, -rc.left, -rc.top);
	}
	str += " /RC:";
	str += IntArray2PartString((int *)&rc, 4).c_str();

	if (::WinExec(str.data(), SW_SHOWNORMAL)<32)
		{
	//	CString cstr;
	//	cstr.LoadString(IDS_ACTEDITER_ERR);
	//	AfxMessageBox(cstr.GetBuffer(0), MB_ICONSTOP | MB_OK);
		}
}



void CMainFrame::OnUpdateWindowOthers(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void CMainFrame::OnWindowOthers() 
{
#ifdef SHAR
	CFileListDlg dlg;
	int iMax = m_wndFileSelectBar.GetWindowCount();
	if (iMax>0) {
		dlg.SetFileSelectBar(&m_wndFileSelectBar);
		if (dlg.DoModal()==IDOK) {
			int sel = dlg.GetCurSel();
			if (sel>=0)
				m_wndFileSelectBar.SelectDoc(sel);
		}
	}
#endif
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	CFrameWnd::OnSetPreviewMode(bPreview, pState);
//	if (!bPreview && GetActiveView()) {
//		((CTheView*)GetActiveView())->SetPaneText_CurPos();
//	}
}
void CMainFrame::FileOpenRetry(LPSTR knjcode)
{
	string pathname;
	if (m_wndFileSelectBar.CloseDocs(CLOSE_THIS, this, &pathname)) {
		if (pathname.length()!=0) {
			m_knjcode_shitei = knjcode != null ? knjcode:"";
			theApp.OpenDocumentFile(pathname.c_str());
			m_knjcode_shitei.clear();
		}
	}
}

void CMainFrame::OnFileOpenRetry()
{
	FileOpenRetry();
}

void CMainFrame::OnUpdateFileOpenRetry(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnFileOpenSjis()
{
	FileOpenRetry("S");
}

void CMainFrame::OnUpdateFileOpenSjis(CCmdUI *pCmdUI)
{
	CObjLayTbl* pMainObjLay = GetActiveObjLayTbl();
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pMainObjLay &&
		(pMainObjLay->getKnjcode()=="S" || pMainObjLay->getKnjcode().empty()));
}

void CMainFrame::OnFileOpenJis()
{
	FileOpenRetry("J");
}

void CMainFrame::OnUpdateFileOpenJis(CCmdUI *pCmdUI)
{
	CObjLayTbl* pMainObjLay = GetActiveObjLayTbl();
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pMainObjLay && pMainObjLay->getKnjcode()=="J");
}

void CMainFrame::OnFileOpenEuc()
{
	FileOpenRetry("E");
}

void CMainFrame::OnUpdateFileOpenEuc(CCmdUI *pCmdUI)
{
	CObjLayTbl* pMainObjLay = GetActiveObjLayTbl();
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pMainObjLay && pMainObjLay->getKnjcode()=="E");
}

void CMainFrame::OnFileOpenUtf8()
{
	FileOpenRetry("U");
}

void CMainFrame::OnUpdateFileOpenUtf8(CCmdUI *pCmdUI)
{
	CObjLayTbl* pMainObjLay = GetActiveObjLayTbl();
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pMainObjLay && pMainObjLay->getKnjcode()=="U");
}

void CMainFrame::OnFileNewJis()
{
	m_knjcode_shitei = "J";
	theApp.OnFileNew();
	m_knjcode_shitei.clear();
}

void CMainFrame::OnUpdateFileNewJis(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnFileNewEuc()
{
	m_knjcode_shitei = "E";
	theApp.OnFileNew();
	m_knjcode_shitei.clear();
}

void CMainFrame::OnUpdateFileNewEuc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnFileNewUtf8()
{
	m_knjcode_shitei = "U";
	theApp.OnFileNew();
	m_knjcode_shitei.clear();
}

void CMainFrame::OnUpdateFileNewUtf8(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
