// The.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "The.h"

#include "MainFrm.h"
#include "TheDoc.h"
#include "TheView.h"
#include "Profile.h"
#include "ViewEnv.h"
#include "crejit.h"
#include "DocTempl.h"
#include "TextCash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CTheApp theApp;
bool vIsExist_prev_The = false;
bool svbAppOnlyOne = false;
typedef struct {
	bool bSend;
	LRESULT bActiveOtherApp;
}ENUMWND_DATA;
/////////////////////////////////////////////////////////////////////////////
// CTheApp
BEGIN_MESSAGE_MAP(CTheApp, CWinApp)
	//{{AFX_MSG_MAP(CTheApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(IDR_CREJIT, OnUpdateCrejit)
	ON_COMMAND(IDR_CREJIT, OnCrejit)
	ON_UPDATE_COMMAND_UI(ID_APP_ONLYONE, OnUpdateAppOnlyone)
	ON_COMMAND(ID_APP_ONLYONE, OnAppOnlyone)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW, &CTheApp::OnFileNew)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTheApp クラスの構築

CTheApp::CTheApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CTheApp オブジェクト


/////////////////////////////////////////////////////////////////////////////
// Pase CommandLine

void CMyCmdLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast ) 
{
#ifdef _DEBUG_0
	CString str;
	str.Format("ParseParam()\n lpszParam=%s", lpszParam);
	AfxMessageBox(str);
	TRACE(str);
#endif
	if (bFlag) {
		CString str=lpszParam;
		str.MakeUpper();
		if (!str.IsEmpty() && str[0]=='J') {
			m_iJump = ((str.GetLength()>=2 && str[1]=='B') ? JUMP_BYTE:JUMP_LINE);
			m_numJump = _atoi64(str.GetBuffer(0)+(m_iJump==JUMP_LINE ? 1:2));
			m_strFlgs +=  " ";
			m_strFlgs += str;
		}else if (!str.IsEmpty() && str[0]=='N') {
			m_bNew = true;
			m_strFlgs +=  " ";
			m_strFlgs += str;
		}else if (str.GetLength()>3 && str.Left(3)=="RC:"){
			string str_rc = str.Mid(3);
			PartString2IntArray(str_rc, (int *)&theViewEnvTbl.m_rcClient, 4);
		}
	}
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast );
}
string CMyCmdLine::GetFullPathName(const char *szShort) 
{
	SHFILEINFO sfi;
	string strLong;
	string strT, strT2;
	string strShortName;
	strT = szShort;
	strShortName = GetFileName(strT);
	//AfxMessageBox(szShort);
	if (strShortName.size()<=12 &&
		SHGetFileInfo(szShort, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME)!=0) {
		strT = szShort;
		strT2 = sfi.szDisplayName;
		strLong = GetFileDirDir(strT);
		strLong += strT2;
		if (GetFileExtent(strT2).size()==0 &&
			(strT=GetFileExtent(GetFileName(strT))).size()>0) {
			strLong += ".";
			strLong += strT;
		}
		//AfxMessageBox(GetFileExtent(strT2).c_str());
		//AfxMessageBox(strT.c_str());
	}
	return strLong;
}
void CMyCmdLine::ConvertFullPathName()
{
	if (!m_strFileName_back.IsEmpty() && m_strFileName != m_strFileName_back)
		m_strFileName = m_strFileName_back;
	if (!m_strFileName.IsEmpty()) {
		//ショートファイル名をロングへ
		bool bDblQ =  (m_strFileName.GetLength()>1 &&
				m_strFileName[0]=='"' && m_strFileName[m_strFileName.GetLength()-1]=='"');
		if (bDblQ) {
			CString str = m_strFileName;
			m_strFileName = str.Mid(1, m_strFileName.GetLength()-2);
		}
		string strLong = GetFullPathName(m_strFileName);
		if (strLong.size()>0) {
#ifdef _DEBUG_0
			CString str;
			str.Format("ConvertFullPathName()\n Org=%s\n->%s",m_strFileName, strLong.data());
			AfxMessageBox(str);
#endif
			m_strFileName = strLong.data();
		}
		{
		CString str = m_strFileName;
		m_strFileName = "\"";
		m_strFileName += str;
		m_strFileName += "\"";
		}
	}
}


void CTheApp::MyParseCommandLine(CMyCmdLine &rCmdInfo)
{
	if (__argc>1) {
		for (int i = 1; i < __argc; i++){
			LPCTSTR pszParam = __targv[i];
			BOOL bLast = ((i + 1) == __argc);
			if (pszParam[0] == '-' || pszParam[0] == '/') {
				break;
			}else{
				if (!rCmdInfo.m_strFileName_back.IsEmpty())
					rCmdInfo.m_strFileName_back += " ";
				rCmdInfo.m_strFileName_back += pszParam;
			}
		}
	}
	ParseCommandLine(rCmdInfo);
}

CMyCmdLine vscmdInfo;

/////////////////////////////////////////////////////////////////////////////
// CTheApp クラスの初期化
BOOL CALLBACK MyEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	ENUMWND_DATA *pdata = (ENUMWND_DATA*)lParam;

	char sz[256];
	if (GetWindowText(hwnd,sz, sizeof(sz))>0) {
		CString str = sz;
		if (str.Find("Mint")>=0){
			vIsExist_prev_The = true;
			if (pdata->bSend) {
				//同一ファイルを開こうとしているか否かのチェック
				//ShellExecute(hwnd, "open", vscmdInfo.m_strFileName, NULL, NULL, SW_SHOW);
				if (pdata->bActiveOtherApp = SendMessage(hwnd, WM_CHECK_OPENDOC,
					(vscmdInfo.m_bNew ? 0:(svbAppOnlyOne ? 1:0)), 0)) {
					//以下のコードはいずれも効果無し
					//PostMessage(hwnd, WM_SET_TOPWINDOW, 0, 0);
					//::SetFocus(hwnd);
					/*CAboutDlg aboutDlg;
					aboutDlg.Create(IDD_ABOUTBOX);
					aboutDlg.ShowWindow(SW_MINIMIZE|SW_SHOW);
					aboutDlg.DestroyWindow();*/
					return false;
				}

			}
			else
				return false;
		}
	}
	return true;
}

void CTheApp::OnUpdateAppOnlyone(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(svbAppOnlyOne);
}

void CTheApp::OnAppOnlyone() 
{
	svbAppOnlyOne = !svbAppOnlyOne;
	//スイッチされたらすぐに書き込むようにする
	AfxGetApp()->WriteProfileInt("General", "AppOnlyOne", svbAppOnlyOne ? 1:0);
}


BOOL CTheApp::InitInstance()
{
	//takaso2008
//	setlocale(LC_CTYPE, "");
    setlocale(LC_ALL, "Japanese_Japan.932");



	AfxEnableControlContainer();

	m_bDestroy = false;
	//if( !AfxOleInit() )             //  ＯＬＥの初期化 
	//	return FALSE;
	
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	//  したければ以下の特定の初期化ルーチンの中から不必要なもの
	//  を削除してください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	// 設定が保存される下のレジストリ キーを変更します。
	// 会社名または所属など、適切な文字列に
	// 変更してください。
	SetRegistryKey(_T("Mint Editor"));
	
#ifndef BUG_20121025_MYDOCUMENT_PLUS_MINT
	FCreateDirectory(GetModuleDirDir(m_hInstance).c_str());
#endif
	theProfile.SetProfile(PRIVATE_PROFILE_NAME, GetModuleDirDir(m_hInstance).c_str());
	theViewEnvTbl.ReadWriteProfile(CProfile::__read);

	LoadStdProfileSettings(10);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	svbAppOnlyOne = AfxGetApp()->GetProfileInt("General", "AppOnlyOne", 1)==1;

	
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	MyParseCommandLine(vscmdInfo);
	vscmdInfo.ConvertFullPathName(); //FullPathに変換する
									//WINBUG:?D&Dでファイルオープン＆ミント起動うまくいかない

	//if (vscmdInfo.m_strFileName.GetLength()))
	string  str = vscmdInfo.m_strFileName;
	str += vscmdInfo.m_strFlgs;
	AfxGetApp()->WriteProfileString("General", "CheckDoc", str.data());
	
	ENUMWND_DATA data;
	data.bSend = svbAppOnlyOne || !vscmdInfo.m_strFileName.IsEmpty();
	data.bActiveOtherApp = false;

	EnumWindows(MyEnumWindowsProc, (LPARAM)&data);
	if (data.bActiveOtherApp) {
		return false;
	}

	if ((vscmdInfo.m_nShellCommand == CCommandLineInfo::FileNew || 
		vscmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing) &&
		 m_splash.Create(NULL))
		{
		m_splash.ShowWindow(SW_SHOW);
		m_splash.UpdateWindow();
		m_splash.SetTimer(1, 500, NULL);
		m_dwSplashTime = ::GetCurrentTime();
		}

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	// クレジットダイアログの表示
#ifdef SHAR_CREJIT
	{
	CString cstr;
	CCrejit crejitDlg;
	switch(crejitDlg.FInitial())
		{
		case CRJ_ER_TIMERREQ:
			AfxMessageBox("タイマが設定できません。タイマを使用する他のアプリケーションを終了するなどしてから、再起動して下さい。", MB_ICONSTOP | MB_OK);
			return FALSE;
		case CRJ_ER_PASSWORD:
#ifdef PLEASE_TEMP_ID
			if (crejitDlg.DoModal()!=IDOK || crejitDlg.GetTmpID()->IsEmpty())
#else
			if (crejitDlg.DoModal()!=IDOK)
#endif
				return FALSE;
			break;
		//CRJ_ER_OK:
		default:
			break;
		}
	}
#endif //SHAR

	theViewEnvTbl.ReadIniProfile();

	theObjTbl.InitialDocTran();

	//CSingleDocTemplate* pDocTemplate;
	//pDocTemplate = new CSingleDocTemplate(
	CTheDocTemplate* pDocTemplate;
	pDocTemplate = new CTheDocTemplate( //new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTheDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CTheView));
	AddDocTemplate(pDocTemplate);



	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
//	MyParseCommandLine(vscmdInfo);

	
//	vscmdInfo.ConvertFullPathName(); //FullPathに変換する
									//WINBUG:?D&Dでファイルオープン＆ミント起動うまくいかない

//	EnumWindows(MyEnumWindowsProc, 0);


	
	// コマンドラインでディスパッチ コマンドを指定します。
#ifdef _DEBUG
/*{
	CString cstr;
	for (int i = 1; i < __argc; i++)
	{
		LPCTSTR pszParam = __targv[i];
		cstr.Format("commad line = %s",pszParam);
		AfxMessageBox(cstr);
	}
	switch (vscmdInfo.m_nShellCommand)
	{
	case CCommandLineInfo::FileNew:
		//if (! (AfxGetApp()->OnCmdMsg(ID_FILE_NEW, 0, NULL, NULL)))
		//	OnFileNew();
		//if (m_pMainWnd == NULL)
		//	bResult = FALSE;
		break;

		// If we've been asked to open a file, call OpenDocumentFile()

	case CCommandLineInfo::FileOpen:
		//if (!OpenDocumentFile(vscmdInfo.m_strFileName))
		//	bResult = FALSE;
		cstr.Format("ShellCommand=FileOpen\n vscmdInfo.m_strFileName=%s",
					vscmdInfo.m_strFileName);
		break;

		// If the user wanted to print, hide our main window and
		// fire a message to ourselves to start the printing

	case CCommandLineInfo::FilePrintTo:
	case CCommandLineInfo::FilePrint:
		//m_nCmdShow = SW_HIDE;
		//ASSERT(m_pCmdInfo == NULL);
		//OpenDocumentFile(vscmdInfo.m_strFileName);
		//m_pCmdInfo = &vscmdInfo;
		//m_pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);
		//m_pCmdInfo = NULL;
		//bResult = FALSE;
		break;

		// If we're doing DDE, hide ourselves

	case CCommandLineInfo::FileDDE:
		cstr.Format("ShellCommand=FileDDE\n m_nCmdShow=%s\n vscmdInfo.m_strFileName=%s",
					m_nCmdShow,vscmdInfo.m_strFileName);
		//m_pCmdInfo = (CCommandLineInfo*)m_nCmdShow;
		//m_nCmdShow = SW_HIDE;
		break;

	// If we've been asked to unregister, unregister and then terminate
	case CCommandLineInfo::AppUnregister:
		break;
	}
	AfxMessageBox(cstr);
}
*/
#endif
	if (!ProcessShellCommand(vscmdInfo))
		return FALSE;

#ifdef SHAR
	((CMainFrame *)m_pMainWnd)->UpdateMenuConf(); //メニューツールバーの設定
#endif //#ifdef SHAR

	// enable file manager drag/drop and DDE Execute open
	m_pMainWnd->DragAcceptFiles();
	EnableShellOpen();

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	ASSERT(m_pMainWnd);
	SetWindowPos(m_pMainWnd->GetSafeHwnd(),
		theViewEnvTbl.m_bTopMost ? HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	//m_hSysPal = GetSystemPalette();

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

void CTheApp::DoBrowse(const char *sz)
{
	HINSTANCE hInst;
	hInst = ShellExecute(m_pMainWnd->GetSafeHwnd(), "open",
			sz, NULL, NULL, SW_SHOWNORMAL);
	if ((UINT)hInst<=31) {
		if (::WinExec(sz, SW_SHOWNORMAL)<=31) {
			if ((UINT)(hInst = ShellExecute(m_pMainWnd->GetSafeHwnd(), NULL/*"open"*/,
				sz, NULL, NULL, SW_SHOW))<=31)
				;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strInfo = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDT_INFO, m_strInfo);
	DDX_Text(pDX, IDC_STC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BTN_Browse, OnBTNBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CTheApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTheApp コマンド

BOOL CTheApp::OnIdle(LONG lCount) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	BOOL bMore = CWinApp::OnIdle(lCount);
	BOOL bsplash = false;

	if (m_bDestroy)
		return bMore;

	// then do our work
	if (m_splash.m_hWnd != NULL)
	{
		if (::GetCurrentTime() - m_dwSplashTime > 3000)
		{
			// timeout expired, destroy the splash window
			m_splash.DestroyWindow();

			m_pMainWnd->UpdateWindow();

	//		if (m_fEnableShowMenuPal && 
	//			!((CMainFrame *)m_pMainWnd)->m_fShowMenuPal)
	//			((CMainFrame *)m_pMainWnd)->DoMenuPal();


			// NOTE: don't set bResult to FALSE,
			//  CWinApp::OnIdle may have returned TRUE

		}
		else
		{
			// check again later...
			bMore = TRUE;
			bsplash = TRUE;
		}
	}


	CTheDoc* pDoc = (CTheDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	if (pDoc) {
		CTheView* pView = (CTheView*)((CMainFrame*)m_pMainWnd)->GetActiveView();
		if (pDoc->m_pMainObjLay && pDoc->m_pMainObjLay->OnIdle(lCount)) {
			if (m_bDestroy) {
				return bMore;
			}
			pDoc->UpdateAllViews(NULL);
			//pDoc->m_pMainObjLay->m_pMainObj->size()/pDoc->m_pMainObjLay->m_pMainObj->m_numBlock * 100
		}else if (m_bDestroy) {
			return bMore;
		}
		if (vscmdInfo.m_iJump != JUMP_NO) {
			bool bJump = false;
			if (pDoc->m_pMainObjLay->IsFormatAll())
				bJump = true;
			else if (vscmdInfo.m_iJump==JUMP_LINE && pDoc->m_pMainObjLay->GetCurSize().cy>=vscmdInfo.m_numJump) {
				bJump = true;
			}else if (vscmdInfo.m_iJump==JUMP_BYTE) {
				OBJOFF off = ((CTextCashTbl*)pDoc->m_pMainObjLay->m_pMainObj)->m_pFormatChars_Loader->Pop_ReadOffset();
				((CTextCashTbl*)pDoc->m_pMainObjLay->m_pMainObj)->m_pFormatChars_Loader->Push_ReadOffset(off);
				if (off>vscmdInfo.m_numJump)
					bJump = true;
			}
			if (bJump) {
				pView->OnJumpLine(vscmdInfo.m_numJump, vscmdInfo.m_iJump);
				vscmdInfo.m_iJump = JUMP_NO;
			}
		}
		if (pView && pView->OnIdle(lCount))
			return 1;
		if (pDoc->m_pMainObjLay && pDoc->m_pMainObjLay->IsFormatAll())
			return bMore;
	}

	if (bsplash)
		return bMore;
	return 1;
}


BOOL CTheApp::PreTranslateMessage(MSG* pMsg)
{
	BOOL bResult = CWinApp::PreTranslateMessage(pMsg);

	if (m_splash.m_hWnd != NULL &&
		(pMsg->message == WM_KEYDOWN ||
		 pMsg->message == WM_SYSKEYDOWN ||
		 pMsg->message == WM_LBUTTONDOWN ||
		 pMsg->message == WM_RBUTTONDOWN ||
		 pMsg->message == WM_MBUTTONDOWN ||
		 pMsg->message == WM_NCLBUTTONDOWN ||
		 pMsg->message == WM_NCRBUTTONDOWN ||
		 pMsg->message == WM_NCMBUTTONDOWN))
	{
		m_splash.DestroyWindow();
		m_pMainWnd->UpdateWindow();
	}

	return bResult;
}

int CTheApp::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	theViewEnvTbl.ReadWriteProfile(CProfile::__write);
	//if (m_hSysPal)
	//	DeleteObject(m_hSysPal);	
	return CWinApp::ExitInstance();
}


void CTheApp::OnUpdateCrejit(CCmdUI* pCmdUI) 
{
#ifdef SHAR
	pCmdUI->Enable(true);
#else
	pCmdUI->Enable(false);
#endif
}

void CTheApp::OnCrejit() 
{
#ifdef SHAR
	CCrejit crejitDlg;
	crejitDlg.DoModal();
#endif
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_strVersion.LoadString(IDS_VERSION);
	m_strInfo = "taka.ogi@gmail.com\r\n"\
				"http://hp.vector.co.jp/authors/VA052798"; 
	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


BOOL CTheApp::OnDDECommand(LPTSTR lpszCommand) 
{
	return CWinApp::OnDDECommand(lpszCommand);
}


static char szSupportUrl[] = "http://hp.vector.co.jp/authors/VA052798"; 
static char szSupportMail[] = "mailto:taka.ogi@gmail.com"; 
void CAboutDlg::OnBTNBrowse() 
{
	theApp.DoBrowse(szSupportUrl);
}


void CTheApp::OnFileNew()
{
	CWinApp::OnFileNew();
}
