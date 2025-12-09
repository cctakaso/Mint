// TanbunEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "TanbunEdit.h"
#include "tanbunAddDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CTanbunEdit ダイアログ


CTanbunEdit::CTanbunEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CTanbunEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTanbunEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_bChange = false;
	m_ptanbuns = null;
	m_pdata = null;
	m_nItem = -1;
}


void CTanbunEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTanbunEdit)
	DDX_Control(pDX, IDC_LSTCTR, m_lstctr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTanbunEdit, CDialog)
	//{{AFX_MSG_MAP(CTanbunEdit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTCTR, OnItemchangedLstctr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTanbunEdit メッセージ ハンドラ

BOOL CTanbunEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	#define NMASK	(LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM)
	m_smallImage.Create(IDB_ENV_SMALL, 16, 1, RGB(255, 255, 255));
	//m_stateImage.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_lstctr.LockUpdate(TRUE);
	VERIFY(m_lstctr.SetSelType(LVS_REPORT, st_full/*st_grid*/, FALSE) && m_lstctr.SetImageLists(&m_smallImage, NULL, NULL));
	m_lstctr.AddColumn(_T("　　短文ｷｰﾜｰﾄﾞ　　"),0,LVCFMT_LEFT, FALSE, NMASK,-1);
	m_lstctr.AddColumn(_T("　　　　短文内容　　　　"),1, LVCFMT_LEFT, TRUE, NMASK,-1);
	ResetListctrEntry();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
void CTanbunEdit::ResetListctrEntry()
{
	m_lstctr.LockUpdate(TRUE);
	
	RemoveAll();
	if (m_ptanbuns->size()>0) {
		int i=0;
		for (CTanbuns::iterator iter=m_ptanbuns->begin(); iter!=m_ptanbuns->end(); iter++) {
			string strName = (*iter).first;
			TANBUN_EDIT_DATA *pdata = new TANBUN_EDIT_DATA;
			pdata->strName = strName;
			pdata->pTanbun = (*iter).second;
			m_lstctr.AddItem(i, 0, pdata->strName.data(), TRUE, 1, -1, pdata);
			m_lstctr.AddItem(i++, 1, pdata->pTanbun->m_pdata->m_strData.data());
		}
		m_lstctr.SetItemState(0, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);
	}
	m_lstctr.LockUpdate(FALSE);
	m_lstctr.SetFocus();
	GetSelData(true);
}
void CTanbunEdit::RemoveAll() 
{
	int i = m_lstctr.GetItemCount();
	while(i--) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(i); //pNMListView->iItem
		if (p) {
			if (p->pdata)
				delete (TANBUN_EDIT_DATA *)(p->pdata);
			delete p;
		}
	}
	m_lstctr.DeleteAllItems();
}

void CTanbunEdit::OnDestroy() 
{
	RemoveAll();
	
	CDialog::OnDestroy();
}

bool CTanbunEdit::UpdateEnv(bool bSetToEnv, CTanbuns *ptanbuns)
{
	assert(ptanbuns);
	if (bSetToEnv) {
		;
	}else{
		m_bChange = false;
		m_ptanbuns = ptanbuns;
		m_strInputData = "";
	}
	return m_bChange;
}


void CTanbunEdit::OnBtnAdd() 
{
	CTanbunAddDlg dlg;
	string strKey, strData;
	int fmt;
	dlg.UpdateEnv(false, m_ptanbuns,
		CTanbunAddDlg::mode_new, strKey, strData, fmt);
	if (dlg.DoModal()==IDOK) {
		dlg.UpdateEnv(true, m_ptanbuns,
				0, strKey, strData, fmt);
		m_ptanbuns->ReadWriteProfile(CProfile::__write);
		ResetListctrEntry();
		m_bChange = true;
	}
}

void CTanbunEdit::OnBtnEdit() 
{
	if (GetSelData()) {
		CTanbunAddDlg dlg;
		string strKey = m_pdata->strName,
			strData = m_pdata->pTanbun->m_pdata->m_strData;
		int fmt = m_pdata->pTanbun->m_pdata->m_fmt;
		dlg.UpdateEnv(false, m_ptanbuns,
			CTanbunAddDlg::mode_update, strKey, strData, fmt, m_nItem);
		if (dlg.DoModal()==IDOK) {
			dlg.UpdateEnv(true, m_ptanbuns,
					0, strKey, strData, fmt);
			m_ptanbuns->ReadWriteProfile(CProfile::__write);
			ResetListctrEntry();
			m_bChange = true;
		}
	}
}

void CTanbunEdit::OnBtnDel() 
{
	if (GetSelData()) {
		if (!m_ptanbuns->RemoveEntry(m_nItem)) {
			return; //failer
		}
		m_ptanbuns->ReadWriteProfile(CProfile::__write);

		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem); //pNMListView->iItem
		if (p) {
			if (p->pdata)
				delete (TANBUN_EDIT_DATA *)(p->pdata);
			delete p;
		}

		m_lstctr.DeleteItem(m_nItem);
		if (m_lstctr.GetItemCount()>0) {
			if (m_nItem>0)
				m_nItem--;
			m_lstctr.SetFocus();
			m_lstctr.SetItemState(m_nItem, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);
		}
		GetSelData();
		m_bChange = true;
	}

}


void CTanbunEdit::OnOK() //data input!
{
	if (GetSelData()) {
		m_strInputData = m_pdata->pTanbun->m_pdata->m_strData;
		CDialog::OnOK();
	}
}

void CTanbunEdit::OnItemchangedLstctr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (m_lstctr.IsWindowVisible()) {
		//if (phdn->uNewState==0) {
		//	m_pdata = null;
		//	*pResult = 0;
		//	return;
		//}
		GetSelData();
	}
	*pResult = 0;
}

bool CTanbunEdit::GetSelData(bool bSetEnableWindow)
{
	bool bEnable = m_pdata!=null;
	m_nItem = m_lstctr.GetNextItem(-1, LVNI_SELECTED);
	if (m_nItem>=0) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem); //pNMListView->iItem
		if (p && p->pdata) {
			//dialog へｾｯﾄ
			m_pdata = (TANBUN_EDIT_DATA *)p->pdata;
		}
	}else
		m_pdata = null;
	if (bSetEnableWindow || bEnable != (m_pdata!=null)) {
		bEnable = m_pdata!=null;
		GetDlgItem(IDOK)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_EDIT)->EnableWindow(bEnable);
	}
	return m_pdata!=null;
}


#endif //#ifdef SHAR

