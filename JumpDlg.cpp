// JumpDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "JumpDlg.h"
#include "TheView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJumpDlg ダイアログ


CJumpDlg::CJumpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJumpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJumpDlg)
	m_strJump = _T("");
	m_bRowNum = FALSE;
	//}}AFX_DATA_INIT
}


void CJumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJumpDlg)
	DDX_Control(pDX, IDC_CHK_ROWNUM, m_chkRowNum);
	DDX_Control(pDX, IDC_CMB_JUMP, m_cmbJump);
	DDX_CBString(pDX, IDC_CMB_JUMP, m_strJump);
	DDX_Check(pDX, IDC_CHK_ROWNUM, m_bRowNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJumpDlg, CDialog)
	//{{AFX_MSG_MAP(CJumpDlg)
	ON_CBN_EDITCHANGE(IDC_CMB_JUMP, OnEditchangeCmbJump)
	ON_CBN_SELCHANGE(IDC_CMB_JUMP, OnSelchangeCmbJump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJumpDlg メッセージ ハンドラ

BOOL CJumpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	for (int i=0; i<m_aryStrJump.GetSize(); i++ )
		m_cmbJump.AddString(m_aryStrJump.GetAt(i));
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CJumpDlg::OnEditchangeCmbJump() 
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(!m_strJump.IsEmpty());
}

void CJumpDlg::OnOK() 
{
	UpdateData(TRUE);
	if (!m_strJump.IsEmpty()) {
		OBJOFF lineJump = _atoi64(m_strJump.GetBuffer(0));
		assert(m_pTheView);
		if (_atoi64(m_strJump.GetBuffer(0))<=0 || !OnJumpLine(m_pTheView, lineJump, m_bRowNum==TRUE ? JUMP_ROW:JUMP_LINE)) {
			UpdateData(FALSE);
			AfxMessageBox("指定行が見つかりません。");
			return;
		}else
			UpdateData(FALSE);
	}
	CDialog::OnOK();
}

bool CJumpDlg::OnJumpLine(CTheView *pTheView, OBJOFF lineJump, JUMPTYPE JumpType)
{
	assert(pTheView);
	m_pTheView = pTheView;
	
	if (lineJump==0) {
		return DoModal()==IDOK;
	}
	CCursor cursorOld = pTheView->m_pViewEd->m_cursor;
	if (pTheView->m_pViewEd->JumpLine(lineJump, JumpType)) {
		m_pTheView->ScrollForSell(cursorOld);
		//見つかった場合のみエントリー
		m_strJump = ultostring(lineJump).c_str();
		int index;
		if ((index = FindStringArray(m_aryStrJump, -1, m_strJump))>=0) {
			m_aryStrJump.RemoveAt(index);
			m_aryStrRowNumFlg.RemoveAt(index);
			//m_cmbJump.DeleteString(index);
		}
		if (JumpType == JUMP_LINE || JumpType == JUMP_ROW) {
			m_aryStrJump.InsertAt(0, m_strJump);
			m_aryStrRowNumFlg.InsertAt(0, (JumpType == JUMP_ROW ? "1":"0"));
		}
		//m_cmbJump.InsertString(0, m_strJump);
		return true;
	}else
		m_strJump = ultostring(lineJump).c_str();

	return false;
}

void CJumpDlg::OnSelchangeCmbJump() 
{
	int index;
	if ((index = m_cmbJump.GetCurSel())>=0) {
		m_chkRowNum.SetCheck(m_aryStrRowNumFlg.GetAt(index)=="1" ? 1:0);
	}
}

