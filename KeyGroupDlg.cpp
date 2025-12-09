// KeyGroupDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "KeyGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CKeyGroupDlg ダイアログ


CKeyGroupDlg::CKeyGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyGroupDlg)
	m_strName = _T("");
	m_strKeywords = _T("");
	//}}AFX_DATA_INIT
}


void CKeyGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyGroupDlg)
	DDX_Text(pDX, IDC_EDT_GROUP_NAME, m_strName);
	DDX_Text(pDX, IDC_EDT_KEYWORDS, m_strKeywords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyGroupDlg メッセージ ハンドラ

BOOL CKeyGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
#endif //#ifdef SHAR
