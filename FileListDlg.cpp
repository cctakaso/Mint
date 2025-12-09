// FileListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "TheDoc.h"
#include "FileSelectBar.h"
#include "FileListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CFileListDlg ダイアログ

BOOL CFileListDlg::m_bSort=FALSE;
CFileListDlg::CFileListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileListDlg)
	//}}AFX_DATA_INIT
	m_pBar = NULL;
	m_selWindow = -1;
}


void CFileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileListDlg)
	DDX_Control(pDX, IDC_LST_WINDOWS_SORT, m_lst_sort);
	DDX_Control(pDX, IDC_LST_WINDOWS, m_lst);
	DDX_Check(pDX, IDC_CHK_SORT, m_bSort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileListDlg, CDialog)
	//{{AFX_MSG_MAP(CFileListDlg)
	ON_LBN_DBLCLK(IDC_LST_WINDOWS, OnDblclkLst)
	ON_BN_CLICKED(IDC_CHK_SORT, OnChkSort)
	ON_LBN_DBLCLK(IDC_LST_WINDOWS_SORT, OnDblclkLstWindowsSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileListDlg メッセージ ハンドラ

BOOL CFileListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pBar);

	SetListEntry(GetFrontList());
	SetListEntry(GetBackList());
	SwitchList();
	UpdateData(FALSE);	//set sort check!	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CFileListDlg::OnDblclkLst() 
{
	OnOK();
}

void CFileListDlg::OnDblclkLstWindowsSort() 
{
	OnOK();
}

void CFileListDlg::OnOK() 
{
	SetCurSel(GetFrontList());
	CDialog::OnOK();
}


void CFileListDlg::OnChkSort() 
{
	UpdateData(TRUE);
	SwitchList();
}
void CFileListDlg::SwitchList()
{
	int sel = GetBackList()->GetCurSel();
	if (sel!=LB_ERR) {
		CString str;
		GetBackList()->GetText(sel, str);
		GetFrontList()->SelectString(-1, str);
	}
	GetFrontList()->ShowWindow(SW_SHOW);
	GetFrontList()->SetFocus();
	GetBackList()->ShowWindow(SW_HIDE);
}
void CFileListDlg::SetCurSel(CListBox *pList)
{
	m_selWindow = pList->GetCurSel();
	if (m_selWindow!=LB_ERR)
		m_selWindow = pList->GetItemData(m_selWindow);
	else
		m_selWindow = -1;
}

void CFileListDlg::SetListEntry(CListBox *pList)
{
//	LONG val = GetWindowLong(pList->m_hWnd, GWL_STYLE);
//	SetWindowLong(pList->m_hWnd, GWL_STYLE, m_bSort ? (val | LBS_SORT):(val ^ LBS_SORT));

	pList->ResetContent();

	string filename;
	int iMax = m_pBar->GetWindowCount();
	if (iMax>0) {
		int focus = m_pBar->GetCurSel();
		for (int i = 0; i<iMax; i++) {
			filename = m_pBar->GetFileName(i);
			int sel=pList->AddString(filename.data());
			if (sel!=LB_ERR) {
				pList->SetItemData(sel, (DWORD)i);
				if (i==focus)
					pList->SetCurSel(sel);
			}
		}
	}
}
#endif //SHAR

