// FileSelectBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "FileSelectBar.h"
#include "MainFrm.h"
#include "DocTempl.h"
#include "TheDoc.h"
#include "TheView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTheApp theApp;
static bool svbEnableMode=true;
/////////////////////////////////////////////////////////////////////////////
// CTabCtrEx
IMPLEMENT_DYNCREATE(CTabCtrlEx, CTabCtrl)
BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
//TODO:	ON_WM_NCHITTEST()
//エラー	1	error C2440: 'static_cast' : 'UINT (__thiscall CTabCtrlEx::* )(CPoint)' から 'LRESULT (__thiscall CWnd::* )(CPoint)' に変換できません。	c:\projects\mint\fileselectbar.cpp	25	

    ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CTabCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	((CFileSelectBar*)GetParent())->HitTab4Tip(point);
	CTabCtrl::OnMouseMove(nFlags, point);
}
UINT CTabCtrlEx::OnNcHitTest( CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);
	((CFileSelectBar*)GetParent())->HitTab4Tip(pt);
	return CTabCtrl::OnNcHitTest(point);
}
void CTabCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	SendMessage(WM_LBUTTONDOWN, nFlags, point.x | (point.y<<16));
	((CTheView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView())->ShowFSBarMenu(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CFileSelectBar ダイアログ
IMPLEMENT_DYNCREATE(CFileSelectBar, CDialogBar)

CFileSelectBar::CFileSelectBar(): CDialogBar()
{
	m_ptip = null;
}
CFileSelectBar::~CFileSelectBar()
{
	if (m_ptip) {
		m_ptip->DestroyWindow();
		delete m_ptip;
	}
}

void CFileSelectBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSelectBar)
	DDX_Control(pDX, IDC_TAB_SELECT, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileSelectBar, CDialogBar)
	//{{AFX_MSG_MAP(CFileSelectBar)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )

/////////////////////////////////////////////////////////////////////////////
// CFileSelectBar メッセージ ハンドラ
BOOL CFileSelectBar::Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID )
{
	BOOL bRtn = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID );
	if (bRtn) {
//wp もともと
//		CRect rc(0,0,0,0);
//		m_tab.Create(TCS_SINGLELINE | TCS_TABS | TCS_RAGGEDRIGHT | HDS_HORZ | CCS_TOP | WS_CHILD | WS_VISIBLE,
//						rc, this, 1);
//		m_tab.Attach(GetDlgItem(IDC_TAB_SELECT)->m_hWnd);
//		EnableToolTips(TRUE);

		UpdateData(FALSE);
		if (m_ptip = new CToolTipCtrl) {
			m_ptip->Create(this);
			m_tab.SetToolTips(m_ptip);
			m_ptip->AddTool(&m_tab, "");
		}
/*typedef struct _TC_ITEM {
    UINT mask;         
    UINT lpReserved1;  // 予約されているので使いません。
    UINT lpReserved2;  // 予約されているので使いません。
    LPSTR pszText;     
    int cchTextMax;    
    int iImage;        
    LPARAM lParam;     
} TC_ITEM;
*/
		/*
		CRect rc(0,0,DCX_MUGEN,DCX_DOWN_BMP);
		m_header.Create(HDS_BUTTONS | HDS_HORZ | CCS_TOP | WS_CHILD | WS_VISIBLE,
						rc, this, 1);//nID
		m_header.AddItem("てすとです.txt");
		m_header.AddItem("test2.txt");
		m_sizeDefault = CSize(DCX_MUGEN, DCX_DOWN_BMP);
		*/
	}
	return TRUE;
}
/*
int CFileSelectBar::OnToolHitTest( CPoint point, TOOLINFO* pTI )
{
	m_ptMouse = point;
	if (pTI->uFlags & TTF_IDISHWND) {
		m_htip = (HANDLE)pTI->uId;
	}
	return 1;
}
BOOL CFileSelectBar::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
			//m_tab.ScreenToClient(&m_ptMouse);
			TC_HITTESTINFO hitTestInfo;
			hitTestInfo.pt = m_ptMouse;
			int sel = m_tab.HitTest(&hitTestInfo);
			if (sel>=0) {
				CObjLayTbl *pLay = m_lays[sel];
				pTTT->lpszText = (char*)pLay->GetPathName().data();//MAKEINTRESOURCE(nID);
				pTTT->hinst = null;
				//SetToolRect(this, UINT nIDTool, LPCRECT lpRect );
				return(TRUE);
			}
        }
    }
    return(FALSE);
}
*/
void CFileSelectBar::AddTab(const char *szName, CObjLayTbl *pLay)
{
	assert(szName);
	assert(pLay);
	svbEnableMode= false;

	TC_ITEM tc;
	int sel = m_tab.GetItemCount();
	memset(&tc, 0, sizeof(tc));
	tc.mask = TCIF_TEXT ;
	tc.pszText = (char*)szName;
	m_tab.InsertItem(sel, &tc);
	m_tab.SetCurSel(sel);
	m_lays.push_back(pLay);

	//m_ptip->AddTool(&m_tab, pLay->GetPathName().data(), NULL, sel);
	svbEnableMode= true;
}

void CFileSelectBar::SetDocTitle(const char *szName)
{
	int sel = GetCurSel();
	TC_ITEM tc;
	memset(&tc, 0, sizeof(tc));
	tc.mask = TCIF_TEXT ;
	tc.pszText = (char*)szName;
	m_tab.SetItem(sel, &tc);
}

/*
CSize CFileSelectBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize size;
	size.cx = (bStretch && bHorz ? 32767 : 0);
	size.cy = (bStretch && !bHorz ? 32767 : 0);
	return size;
}
*/
/*
void CFileSelectBar::OnUpdateCmdUI(class CFrameWnd *,int )
{

}
*/

void CFileSelectBar::OnSize(UINT nType, int cx, int cy) 
{
	if (svbEnableMode) {
		CDialogBar::OnSize(nType, cx, cy);
		UpdateSize();
	}
}
void CFileSelectBar::UpdateSize()
{
	if (svbEnableMode && IsWindow(m_tab.m_hWnd)) {
		svbEnableMode= false;
		CRect rc, rcClient;

		if (!IsFloating()) {
			GetWindowRect(&rc);
			GetParent()->GetWindowRect(&rcClient);
			//rc.right = rcClient.right;
			GetParent()->ScreenToClient(&rc);
			rc.left = rc.left<0 ? 0:rc.left;
			rc.right = GetSystemMetrics(SM_CXSCREEN);
			MoveWindow(&rc);
		}

		
	//	m_tab.GetWindowRect(&rc);
		GetClientRect(&rcClient);
		rc.left = 3;
		rc.right = rcClient.right - 3;
		rc.top = 4;
		rc.bottom = rcClient.bottom - 1;
		m_tab.MoveWindow(&rc);
		svbEnableMode= true;
	}
}

void CFileSelectBar::OnDestroy() 
{
	//if (IsWindow(m_tab.m_hWnd)) {
	//	m_tab.Detach();
	//}

	CDialogBar::OnDestroy();
}

CSize CFileSelectBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	UpdateSize();
	if (bStretch) // if not docked stretch to fit
		return CSize(bHorz ? 32767 : m_sizeDefault.cx,
			bHorz ? m_sizeDefault.cy : 32767);
	else
		return m_sizeDefault;
}


BOOL CFileSelectBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ((int)LOWORD(wParam)==IDC_TAB_SELECT &&
		((LPNMHDR)lParam)->code == TCN_SELCHANGE ) {
		int sel = GetCurSel();
		((CTheDocTemplate*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument()->GetDocTemplate())->SelectDocumentFile(m_lays[sel]);
	}
	return CDialogBar::OnNotify(wParam, lParam, pResult);
}
void CFileSelectBar::CloseTabs(int ctype, int sel)
{
	svbEnableMode= false;
	switch(ctype) {
	case CLOSE_ALL:
		m_tab.DeleteAllItems();
		m_lays.clear();
		break;
	case CLOSE_THIS:
		m_tab.DeleteItem(sel);
		m_lays.erase(m_lays.begin()+sel);
		//GetToolCount( )
		//m_ptip->DelTool(&m_tab, pLay->GetPathName().data(), NULL, sel);
		break;
	case CLOSE_NOT_THIS:
		{
		CObjLayTbl *pLay = m_lays[sel];
		m_tab.DeleteAllItems();
		m_lays.clear();
		AddTab(((CMainFrame*)AfxGetMainWnd())->GetActiveDocument()->GetTitle(), pLay);
		}
		break;
	}
	svbEnableMode= true;
}

bool CFileSelectBar::CloseDocs(int ctype, CMainFrame* pFrame, string *pathname)
{
	//ctype == CLOSE_ALL: 全てクローズして新規を１つ追加
	//ctype == CLOSE_THIS: 現在表示中のものをクローズして１つ前のを表示
	//ctype == CLOSE_NOT_THIS: 現在表示中のものを残して他を全てクローズ
	int sel = GetCurSel();
	switch(ctype) {
	case CLOSE_ALL:
		if (CanCloseFrame(pFrame)) {
			theObjTbl.CloseLayouts(ctype);
			CloseTabs(ctype);
			((CTheView*)pFrame->GetActiveView())->m_pViewEd->Detach(true);
			theApp.OnFileNew();
			return true;
		}
		break;
	case CLOSE_THIS:
		if (CanCloseDoc(sel, pFrame, pathname)) {
			theObjTbl.CloseLayouts(ctype, m_lays[sel]);
			CloseTabs(ctype, sel);
			((CTheView*)pFrame->GetActiveView())->m_pViewEd->Detach(true);
			if (sel==0) {
				if (m_lays.size()==0) {
					theApp.OnFileNew();
				} else {
					SelectDoc(0);
				}
			}else{
				SelectDoc(sel-1);
			}
			return true;
		}
		break;
	case CLOSE_NOT_THIS:
		if (CanCloseFrame(pFrame, sel)) {
			if (SelectDoc(sel)) {
				theApp.CWinApp::OnIdle(0);
				//pFrame->m_wndToolBar.UpdateWindow();
				//pFrame->m_wndToolEditBar.UpdateWindow();
			}
			theObjTbl.CloseLayouts(ctype, m_lays[sel]);
			CloseTabs(ctype, sel);
			return true;
		}
		break;
	}
	return false;
}

bool CFileSelectBar::CanCloseFrame(CMainFrame* pFrame, int sel_no)
{
	for (int sel=0; sel<m_lays.size(); sel++) {
		if (sel_no!=sel) {
			//this is dirty document!
			//Change view.
			if (!CanCloseDoc(sel, pFrame))
				return false;
		}
	}
	return true;//can close frame!
}

bool CFileSelectBar::CanCloseDoc(int sel, CMainFrame* pFrame, string *pathname)
{
	CDocument *pDocument = pFrame->GetActiveDocument();
	if (m_lays[sel]->GetActions().IsEnableUndo()) {
		//this is dirty document!
		//Change view.
		if (SelectDoc(sel)) {
			theApp.CWinApp::OnIdle(0);
			//pFrame->m_wndToolBar.UpdateWindow();
			//pFrame->m_wndToolEditBar.UpdateWindow();
		}

		if (pathname) {
			CString cstr;
			*pathname = ((CTheDoc*)pDocument)->GetPathName();
			cstr.Format(IDS_TRASH_MODIFYFILE, pathname->c_str());
			if (AfxMessageBox(cstr, MB_OKCANCEL)==IDCANCEL) {
				return false;
			}
		}else{
			assert(pDocument);
			CString str, str_back = pDocument->GetTitle();
			str = str_back;
			((CTheDoc*)pDocument)->SetDirtyPahtName(str, false);
			if (str.GetLength()>5 && str.Left(5)=="無題 ") {
				CString strT = "無題";
				strT += str.Mid(5);
				str = strT;
			}
			if (str.Find(".")<0) {
				str += ".txt";
			}
			((CTheDoc*)pDocument)->SetTitleStringOnly(str);
			if (!pDocument->CanCloseFrame(pFrame)) {
				((CTheDoc*)pDocument)->SetTitleStringOnly(str_back);
				return false; //cancel button click!
			}
		}
	}
	if (pathname)
		*pathname = ((CTheDoc*)pDocument)->GetPathName();
	return true;//can close frame!
}

bool CFileSelectBar::SelectDoc(int sel)
{
	if (GetCurSel()!=sel && sel<m_lays.size()) {
		m_tab.SetCurSel(sel);
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		CDocument *pDocument = pFrame->GetActiveDocument();
		assert(pDocument);
		((CTheDocTemplate*)pDocument->GetDocTemplate())->SelectDocumentFile(m_lays[sel]);
		return true;
	}
	return false;
}

void CFileSelectBar::SetNext(int inc)
{
	assert(GetWindowCount()>1);
	int sel = GetCurSel();
	if (inc>0) {
		if (++sel>=GetWindowCount())
			sel = 0;
	}else{
		if (sel==0)
			sel = GetWindowCount()-1;
		else
			sel--;
	}
	SelectDoc(sel);
}
string CFileSelectBar::GetFileName(int sel)
{
	string str;
	assert(sel<GetWindowCount());
	assert(sel<m_tab.GetItemCount());
	if (m_lays[sel]->m_filename.size()==0)
		return GetTitle(sel);
	else {
		string str = m_lays[sel]->m_filename.data();
		if (m_lays[sel]->GetActions().IsEnableUndo())
			str += " *";
		return str;
	}
}
string CFileSelectBar::GetTitle(int sel)
{
	assert(sel<GetWindowCount());
	assert(sel<m_tab.GetItemCount());
	string str;
	TC_ITEM tc;
	char sz[512];
	memset(&tc, 0, sizeof(tc));
	tc.mask = TCIF_TEXT ;
	tc.pszText = (char*)sz;
	tc.cchTextMax = sizeof(sz);
	m_tab.GetItem(sel, &tc);
	str = sz;
	return str;
}
int CFileSelectBar::GetCurSel()
{
	return m_tab.GetCurSel();
}

bool CFileSelectBar::SelectDoc(const char *filename)
{
	assert(IsValidDataSz(filename));
#ifdef WIN32
	string str1, str2;
	str1 = filename;
	strupr((char*)str1.data());
#endif
    int sel;
	for (sel=0; sel<m_lays.size(); sel++) {
#ifdef WIN32
		str2 = m_lays[sel]->m_filename.data();
		strupr((char*)str2.data());
		if (str1 == str2) {
#else
		if (m_lays[sel]->m_filename == filename) {
#endif
			SelectDoc(sel);
			return true;
		}
	}
	//ファイル未保存の場合は、タイトルを検索
	for (sel=0; sel<m_lays.size(); sel++) {
		if (GetTitle(sel) == filename) {
			SelectDoc(sel);
		}
	}
	return false;
}
void CFileSelectBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	HitTab4Tip(point);
	CDialogBar::OnMouseMove(nFlags, point);
}

void CFileSelectBar::UpdateTip() 
{
	if (!m_ptip)
		return;
	m_ptip->DestroyWindow();
	delete m_ptip;
	if (m_ptip = new CToolTipCtrl) {
		m_ptip->Create(this);
		m_tab.SetToolTips(m_ptip);
		m_ptip->AddTool(&m_tab, "");
		m_strTip = "";
	}
}
void CFileSelectBar::HitTab4Tip(CPoint point) 
{
	if (!m_ptip)
		return;
	TC_HITTESTINFO hitTestInfo;
	hitTestInfo.pt = point;
	int sel = m_tab.HitTest(&hitTestInfo);
	if (sel>=0) {
		CObjLayTbl *pLay = m_lays[sel];
		//takaso2008
		string str; 
		//const char *pSzTip;
		if (pLay->GetPathName()=="")
			str = pLay->m_strTitle;
		else
			str = pLay->GetPathName();
		//const char *pSzTip = pLay->GetPathName()=="" ? pLay->m_strTitle.c_str():pLay->GetPathName().c_str();
		//if (m_strTip != pSzTip) {
		//	m_strTip = pSzTip;
		//	m_ptip->UpdateTipText((char*)m_strTip.data(), &m_tab);
			m_ptip->UpdateTipText((char*)str.data(), &m_tab);
			//m_ptip->DelTool(&m_tab, 0);
			//m_ptip->AddTool(&m_tab, (char*)m_strTip.data(), NULL, 0);
		//}
	}
}

BOOL CFileSelectBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	return CDialogBar::OnSetCursor(pWnd, nHitTest, message);
}

