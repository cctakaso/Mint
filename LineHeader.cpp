// LineHeader.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "LineHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CLineHeaderDlg ダイアログ


CLineHeaderDlg::CLineHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineHeaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineHeaderDlg)
	m_strLineHeader = _T("１．");
	m_bOpenDlg = TRUE;
	//}}AFX_DATA_INIT
}


void CLineHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineHeaderDlg)
	DDX_Control(pDX, IDC_CMB_LINE_HEADER, m_cmbLineHeader);
	DDX_CBString(pDX, IDC_CMB_LINE_HEADER, m_strLineHeader);
	DDX_Check(pDX, IDC_CHK_OPENDLG, m_bOpenDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineHeaderDlg, CDialog)
	//{{AFX_MSG_MAP(CLineHeaderDlg)
	ON_CBN_DROPDOWN(IDC_CMB_LINE_HEADER, OnDropdownCmbLineHeader)
	ON_BN_CLICKED(IDC_CHK_OPENDLG, OnChkOpendlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineHeaderDlg メッセージ ハンドラ

void CLineHeaderDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	CDialog::OnOK();
}

void CLineHeaderDlg::OnDropdownCmbLineHeader() 
{
	UpdateData(TRUE);
	m_cmbLineHeader.SelectString(-1, m_strLineHeader);
}

void CLineHeaderDlg::OnChkOpendlg() 
{
}
#endif //#ifdef SHAR
