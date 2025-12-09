// FindDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "FindDlg.h"
#include "TheView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDlg ダイアログ


CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDlg)
	m_bCase = FALSE;
	m_strFind = _T("");
	m_strReplace = _T("");
	//}}AFX_DATA_INIT
	m_bReplace = false;
	m_pTheView = null;
}


void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Control(pDX, IDC_DO_UP, m_btnDoUp);
	DDX_Control(pDX, IDC_DO_REPLACE_ALL, m_btnReplaceAll);
	DDX_Control(pDX, IDC_DO_REPLACE, m_btnReplace);
	DDX_Control(pDX, IDC_CMB_REPLACE, m_cmbReplace);
	DDX_Control(pDX, IDC_DO_DOWN, m_btnDoDown);
	DDX_Control(pDX, IDC_CMB_FIND, m_cmbFind);
	DDX_Check(pDX, IDC_CHK_CASE, m_bCase);
	DDX_CBString(pDX, IDC_CMB_FIND, m_strFind);
	DDX_CBString(pDX, IDC_CMB_REPLACE, m_strReplace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	ON_BN_CLICKED(IDC_DO_DOWN, OnDoDown)
	ON_BN_CLICKED(IDC_DO_REPLACE, OnDoReplace)
	ON_BN_CLICKED(IDC_DO_REPLACE_ALL, OnDoReplaceAll)
	ON_BN_CLICKED(IDC_DO_UP, OnDoUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg メッセージ ハンドラ

BOOL CFindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	for (int i=0; i<m_aryStrFind.GetSize(); i++ )
		m_cmbFind.AddString(m_aryStrFind.GetAt(i));
	if (m_bReplace) {
		SetWindowText("置換");
		m_btnDoUp.ShowWindow(FALSE);
		m_cur_last = m_pTheView->m_pViewEd->m_cursor.Befor();//新規検索
		m_bAbleReplace = false;
		m_bDointReplaceAll = false;
		for (int i=0; i<m_aryStrReplace.GetSize(); i++ )
			m_cmbReplace.AddString(m_aryStrReplace.GetAt(i));
	}else {
		SetWindowText("検索");
		CRect rc, rcDlg;
		GetDlgItem(IDC_STC_REPLACE)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMB_REPLACE)->ShowWindow(FALSE);
		GetDlgItem(IDC_DO_REPLACE)->ShowWindow(FALSE);
		GetDlgItem(IDC_DO_REPLACE_ALL)->ShowWindow(FALSE);
		
		GetDlgItem(IDCANCEL)->GetWindowRect(&rc);
		GetWindowRect(&rcDlg);
		SetWindowPos(null, 0, 0, rcDlg.Width(), rc.top - rcDlg.top, SWP_NOACTIVATE |SWP_NOREDRAW |SWP_NOMOVE |SWP_NOZORDER);

		GetDlgItem(IDC_CMB_REPLACE)->GetWindowRect(&rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_CHK_CASE)->SetWindowPos(null, rc.left, rc.top, 0,0,SWP_NOACTIVATE |SWP_NOREDRAW |SWP_NOSIZE |SWP_NOZORDER);

		//m_btnDoDown.GetWindowRect(&rc);
		//ScreenToClient(rc);
		//m_btnDoUp.SetWindowPos(null, rc.left, rc.top, 0,0,SWP_NOACTIVATE |SWP_NOREDRAW |SWP_NOSIZE |SWP_NOZORDER);

		//m_btnReplace.GetWindowRect(&rc);
		//ScreenToClient(rc);
		//m_btnDoDown.SetWindowPos(null, rc.left, rc.top, 0,0,SWP_NOACTIVATE |SWP_NOREDRAW |SWP_NOSIZE |SWP_NOZORDER);

		GetDlgItem(IDC_DO_REPLACE_ALL)->GetWindowRect(&rc);
		ScreenToClient(rc);
		GetDlgItem(IDCANCEL)->SetWindowPos(null, rc.left, rc.top, 0,0,SWP_NOACTIVATE |SWP_NOREDRAW |SWP_NOSIZE |SWP_NOZORDER);
		//GetDlgItem(IDC_DO_REPLACE_ALL)->GetWindowRect(&rc);
		//ScreenToClient(rc);
		}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
void CFindDlg::OnDoUp() 
{
	DoFind(false);
}
void CFindDlg::OnDoDown() 
{
	DoFind(true);
}
bool CFindDlg::DoFind(bool bToDocEnd) 
{
	UpdateData(TRUE);
	if (!m_strFind.IsEmpty()) {
		int index;
		if ((index=m_cmbFind.FindString(-1, m_strFind))!=CB_ERR) {
			m_aryStrFind.RemoveAt(index);
			m_cmbFind.DeleteString(index);
		}
		m_aryStrFind.InsertAt(0, m_strFind);
		m_cmbFind.InsertString(0, m_strFind);
		m_cmbFind.SetCurSel(0);

		if (!m_bReplace) {
			AfxGetApp()->DoWaitCursor(1); 
			if (!DoFindNext(null, bToDocEnd)) {
				if (!vbEscHit) {
					CString msg;
					msg.Format("\"%s\"が見つかりません。",m_strFind);
					AfxMessageBox(msg);
				}
			}else
				EndDialog(TRUE);
			AfxGetApp()->DoWaitCursor(-1); 
			return false;
		}
		
		//replace!
		m_pTheView->ShowBlinkCursor(false);
#if BUG_20121019_FIND
		if (index!=0) {
			m_cur_last = m_pTheView->m_pViewEd->m_cursor.Befor();//新規検索
		}
#else
		if (index!=0) {
			m_cur_last = bToDocEnd ? m_pTheView->m_pViewEd->m_cursor.End():
				m_pTheView->m_pViewEd->m_cursor.Befor();//新規検索
		}
#endif
		AfxGetApp()->DoWaitCursor(1); 
		if (!(m_bAbleReplace = DoFindNext(&m_cur_last, bToDocEnd))) {
			m_pTheView->ShowBlinkCursor(true);
			if (!m_bDointReplaceAll && !vbEscHit)
				AfxMessageBox("全ての検索は終わりました。");
#if BUG_20121019_FIND
			m_cur_last = m_pTheView->m_pViewEd->m_cursor.Befor();//新規検索
#else
			m_cur_last = bToDocEnd ? m_pTheView->m_pViewEd->m_cursor.End():
				m_pTheView->m_pViewEd->m_cursor.Befor();//新規検索
#endif
		}
		AfxGetApp()->DoWaitCursor(-1); 
		m_pTheView->ShowBlinkCursor(false);
		return m_bAbleReplace;
	}
	return false;
}

void CFindDlg::OnDoReplace() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	if (!m_bDointReplaceAll) {
		int index;
		if ((index=m_cmbReplace.FindString(-1, m_strReplace))!=CB_ERR) {
			m_aryStrReplace.RemoveAt(index);
			m_cmbReplace.DeleteString(index);
		}
		m_aryStrReplace.InsertAt(0, m_strReplace);
		m_cmbReplace.InsertString(0, m_strReplace);
		m_cmbReplace.SetCurSel(0);
	}
	if (m_bAbleReplace) {
		assert(m_pTheView->m_pViewEd->m_cursor.IsExpand());
		CAction *paction = new CAction;
		paction->push_string(act_past, m_strReplace, m_strReplace.GetLength()); //strlen(psz)は、Win95では改行があるとダメ
		m_pTheView->OnString(paction, m_pTheView->m_pViewEd->m_cursor);
	}
	OnDoDown();
}

void CFindDlg::OnDoReplaceAll() 
{
	AfxGetApp()->DoWaitCursor(1); 
	int index;
	UpdateData(TRUE);
	if ((index=m_cmbReplace.FindString(-1, m_strReplace))!=CB_ERR) {
		m_aryStrReplace.RemoveAt(index);
		m_cmbReplace.DeleteString(index);
	}
	m_aryStrReplace.InsertAt(0, m_strReplace);
	m_cmbReplace.InsertString(0, m_strReplace);
	m_cmbReplace.SetCurSel(0);

	m_bDointReplaceAll = true;
	int count_replace = 0;
	do {
		if (m_bAbleReplace)
			count_replace++;
		OnDoReplace();
	}while(m_bAbleReplace && !vbEscHit);

	//m_pTheView->ShowBlinkCursor(true);
	if (!vbEscHit) {
		CString msg;
		msg.Format("全部で %d 個、置換しました。", count_replace);
		AfxMessageBox(msg);
	}
	m_bDointReplaceAll = false;
	AfxGetApp()->DoWaitCursor(-1); 
	m_pTheView->ShowBlinkCursor(false);
}

void CFindDlg::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	m_bReplace = false;	
	CDialog::OnCancel();
}

void CFindDlg::OnEditFind(CTheView *pTheView, const char *sz)
{
	m_bReplace = false;
	m_pTheView = pTheView;
	if (IsValidDataSz(sz))
		m_strFind = sz;

	DoModal();
}
void CFindDlg::OnEditFindPrev(CTheView *pTheView)
{
	m_bReplace = false;
	m_pTheView = pTheView;
	DoFindNext(null, false);
}
void CFindDlg::OnEditFindNext(CTheView *pTheView)
{
	m_bReplace = false;
	m_pTheView = pTheView;
	DoFindNext(null, true);
}
void CFindDlg::OnEditReplace(CTheView *pTheView, const char *sz)
{
	m_bReplace = true;
	m_pTheView = pTheView;
	if (IsValidDataSz(sz))
		m_strFind = sz;
	DoModal();
}
bool CFindDlg::DoFindNext(CUR *pcur_last, bool bToDocEnd)
{
#if BUG_20121019_FIND
	CUR cur_first, cur_last, cur_0, cur_npos;
	bool bAll;
	cur_first = bToDocEnd ? m_pTheView->m_pViewEd->m_cursor.Befor():
					m_pTheView->m_pViewEd->m_cursor.End();
#else
	CUR cur_first, cur_last, cur_0, cur_npos, cur_end;
	bool bAll;
	if (bToDocEnd) {
		cur_first = m_pTheView->m_pViewEd->m_cursor.Befor();
		cur_end = m_pTheView->m_pViewEd->m_cursor.End();
	}else{
		cur_first = m_pTheView->m_pViewEd->m_cursor.End();
		cur_end = m_pTheView->m_pViewEd->m_cursor.Befor();
	}
#endif
	
	if (m_pTheView->m_pViewEd->m_cursor.IsExpand()) {
		if (bToDocEnd)
			m_pTheView->m_pViewEd->GetHint()->NextCur(cur_first);
		else
			m_pTheView->m_pViewEd->GetHint()->PrevCur(cur_first);
	}
	
	cur_0 = m_pTheView->m_pViewEd->GetHint()->GetCur_0();
	cur_npos = m_pTheView->m_pViewEd->GetHint()->GetCur_NPOS();

	if (bToDocEnd) {
		cur_last = cur_npos;
		bAll = cur_first == cur_0;
	}else{
		cur_last = cur_0;
		bAll = cur_first == cur_npos;
	}

	bool bFind;
	vbEscHit = false;//ESC flg off!
	while (!vbEscHit && !(bFind=DoFindNext_Between(cur_first, cur_last, bToDocEnd))&& !bAll ) {
		//もう一度文頭から検索する
		if (pcur_last) {
			cur_last = *pcur_last;
			*pcur_last = cur_0;
		}else{
#if BUG_20121019_FIND
			cur_last = cur_first;
#else
			cur_last = cur_end;
#endif
		}
		m_pTheView->m_pViewEd->GetHint()->SetCur(cur_last);
		cur_first = bToDocEnd ? m_pTheView->m_pViewEd->GetHint()->GetCur_0():
								m_pTheView->m_pViewEd->GetHint()->GetCur_NPOS();
		bAll = true;
	}
	return bFind;
}
bool CFindDlg::DoFindNext_Between(CUR cur_first, CUR cur_last, bool bToDocEnd)
{
	CUR cur;
	CCursor cursorOld = m_pTheView->m_pViewEd->m_cursor;
	CCursor cursor = cursorOld;
	//cursor.SetExpand(false);
	//cursor.SetBlinkCursor(cur_first);
	//cursor.SetExpand();
	//cursor.SetBlinkCursor(cur_last);
	cursor.SetCursor(&cur_first, &cur_last);
	m_pTheView->ShowBlinkCursor(false);
	if (m_pTheView->m_pViewEd->GetHint()->FindData(cursor, string(m_strFind), m_bCase==TRUE, bToDocEnd)) {
		assert(cursor.IsValid());
		m_pTheView->m_pViewEd->m_cursor = cursor;
		m_pTheView->ShowBlinkCursor(true);
		m_pTheView->ScrollForSell(cursorOld);
		return true;
	}else
		m_pTheView->ShowBlinkCursor(true);
	return false;
}

