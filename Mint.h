// Mint.h : Mint アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_Mint_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_)
#define AFX_Mint_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMintApp:
// このクラスの動作の定義に関しては Mint.cpp ファイルを参照してください。
//
class CMyCmdLine :public CCommandLineInfo
{
public:
	CMyCmdLine() { m_bJump=false; }
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
	bool m_bJump;
	OBJOFF m_lineJump;
};

class CMintApp : public CWinApp
{
public:
	CMintApp();
public:
	//HPALETTE m_hSysPal;
// オーバーライド
private:
	void MyParseCommandLine(CMyCmdLine &rCmdInfo);

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMintApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMintApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateCrejit(CCmdUI* pCmdUI);
	afx_msg void OnCrejit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_Mint_H__86A4F104_0A00_11D2_AA08_444553540000__INCLUDED_)
