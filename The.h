// The.h : The アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_The_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_)
#define AFX_The_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル
#include "Splash.h"

class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CTheApp:
// このクラスの動作の定義に関しては The.cpp ファイルを参照してください。
//
#define WM_CHECK_OPENDOC	(WM_USER+10)
#define WM_SET_TOPWINDOW	(WM_USER+11)
#define WM_AROWKEY_DOWN		(WM_USER+12)

enum JUMPTYPE{
	JUMP_NO,
	JUMP_LINE,
	JUMP_ROW,
	JUMP_BYTE,
};

class CAboutDlg : public CDialog
{

public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVersion;
	CString m_strInfo;
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CMyCmdLine :public CCommandLineInfo
{
public:
	CMyCmdLine() { m_iJump=JUMP_NO; m_bNew = false;}
	virtual ~CMyCmdLine() {};
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
	
	CString m_strFileName_back;
	/*typedef struct _SHFILEINFO { // shfi
		HICON hIcon;
		int   iIcon; 
		DWORD dwAttributes;
		char  szDisplayName[MAX_PATH];
		char  szTypeName[80];
		} SHFILEINFO;  
	*/
	string GetFullPathName(const char *szShort);
	void ConvertFullPathName();
public:
	JUMPTYPE m_iJump;
	OBJOFF m_numJump;
	string m_strFlgs;
	bool m_bNew;
};

class CTheApp : public CWinApp
{
	friend CMainFrame;
	friend class CFileSelectBar;
public:
	CTheApp();
public:
	//HPALETTE m_hSysPal;
	bool m_bDestroy;
	void DoBrowse(const char *sz);

	// オーバーライド
private:
	void MyParseCommandLine(CMyCmdLine &rCmdInfo);

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTheApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CTheApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateCrejit(CCmdUI* pCmdUI);
	afx_msg void OnCrejit();
	afx_msg void OnUpdateAppOnlyone(CCmdUI* pCmdUI);
	afx_msg void OnAppOnlyone();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DWORD m_dwSplashTime;
	CSplash m_splash;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_The_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_)
