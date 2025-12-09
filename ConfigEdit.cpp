// ConfigEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "ViewEnv.h"
#include "ConfigEdit.h"
#include "EnvEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

extern CTheApp theApp;
extern CViewEnvTbl theViewEnvTbl;
/////////////////////////////////////////////////////////////////////////////
// CConfigEdit ダイアログ
CConfigEdit::CConfigEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigEdit)
	//}}AFX_DATA_INIT
	m_bChange = false;
	m_pEnvTbl = null;
	m_pdata = null;
	m_nItem = -1;
}


void CConfigEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigEdit)
	DDX_Control(pDX, IDC_LSTCTR, m_lstctr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigEdit, CDialog)
	//{{AFX_MSG_MAP(CConfigEdit)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_ADD_COPY, OnBtnAddCopy)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTCTR, OnItemchangedLstCtr)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigEdit メッセージ ハンドラ

BOOL CConfigEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	#define NMASK	(LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM)
	m_smallImage.Create(IDB_ENV_SMALL, 16, 1, RGB(255, 255, 255));
	//m_stateImage.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_lstctr.LockUpdate(TRUE);
	VERIFY(m_lstctr.SetSelType(LVS_REPORT, st_full/*st_grid*/, FALSE) && m_lstctr.SetImageLists(&m_smallImage, NULL, NULL));
	m_lstctr.AddColumn(_T("       設定        "),0,LVCFMT_LEFT, FALSE, NMASK,-1);
	m_lstctr.AddColumn(_T("関連付けファイル種類"),1, LVCFMT_LEFT, TRUE, NMASK,-1);
	m_lstctr.LockUpdate(FALSE);

	ResetListctrEntry();
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
void CConfigEdit::ResetListctrEntry()
{
	m_lstctr.LockUpdate(TRUE);
	
	RemoveAll();

	CViewEnv *pEnv = theViewEnvTbl.GetViewEnv_byEnvIndex(-1);
	if (pEnv) {
		ENV_EDIT_DATA *pdata = new ENV_EDIT_DATA;
		pdata->strName = NORMAL_NAME;
		pdata->pEnv = pEnv;
		m_lstctr.AddItem(0, 0, pdata->strName.data(), TRUE, 0, -1, pdata);
		m_lstctr.AddItem(0, 1, pdata->pEnv->m_strExtents.data());
	}
	int i=1;
	for (CViewEnvTbl::iterator iter=m_pEnvTbl->begin(); iter!=m_pEnvTbl->end(); iter++) {
		string strName = (*iter).first;
		if (strName!=NORMAL_NAME) {
			ENV_EDIT_DATA *pdata = new ENV_EDIT_DATA;
			pdata->strName = strName;
			pdata->pEnv = (*iter).second;
			m_lstctr.AddItem(i, 0, pdata->strName.data(), TRUE, 0, -1, pdata);
			m_lstctr.AddItem(i++, 1, pdata->pEnv->m_strExtents.data());
		}
	}
	m_lstctr.SetItemState(0, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);

	m_lstctr.LockUpdate(FALSE);
	m_lstctr.SetFocus();
	GetSelData(true);
}

void CConfigEdit::RemoveAll() 
{
	int i = m_lstctr.GetItemCount();
	while(i--) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(i); //pNMListView->iItem
		if (p) {
			if (p->pdata)
				delete (ENV_EDIT_DATA *)(p->pdata);
			delete p;
		}
	}
	m_lstctr.DeleteAllItems();
}

void CConfigEdit::OnDestroy() 
{
	RemoveAll();	
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
}
bool CConfigEdit::UpdateEnv(bool bSetToEnv, CViewEnvTbl *pEnvTbl)
{
	assert(pEnvTbl);
	if (bSetToEnv) {
		;
	}else{
		m_bChange = false;
		m_pEnvTbl = pEnvTbl;
	}
	return m_bChange;
}

void CConfigEdit::OnBtnAdd() 
{
	NewData(NORMAL_NAME);
}
void CConfigEdit::NewData(const char *szName_base) 
{
	CEnvEdit dlg;
	string strName, strFile, strExtents;
	strName = "新しい設定";
	strFile = "新しい設定";
	strExtents = "TXT";
	dlg.UpdateEnv(false, m_pEnvTbl, CEnvEdit::mode_new, strName, strFile, strExtents, szName_base);
	if (dlg.DoModal()==IDOK) {
		if (dlg.UpdateEnv(true, m_pEnvTbl, 0, strName, strFile, strExtents)) {
			theViewEnvTbl.WriteEnvEntryOnly();
			ResetListctrEntry();
			m_bChange = true;
		}
	}
}

void CConfigEdit::OnBtnAddCopy() 
{
	if (GetSelData()) {
		NewData(m_pdata->strName.data());
	}
}

void CConfigEdit::OnBtnEdit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (GetSelData()) {
		CEnvEdit dlg;
		string strName, strFile, strExtents;
		strName = m_pdata->strName;
		strFile = m_pdata->pEnv->m_profile.GetSzProfileNameOnly();
		strExtents = m_pdata->pEnv->m_strExtents;
		dlg.UpdateEnv(false, m_pEnvTbl, CEnvEdit::mode_update, strName, strFile, strExtents);
		if (dlg.DoModal()==IDOK) {
			if (dlg.UpdateEnv(true, m_pEnvTbl, 0, strName, strFile, strExtents)) {
				theViewEnvTbl.WriteEnvEntryOnly();
				ResetListctrEntry();
				m_bChange = true;
			}
		}
	}
}

void CConfigEdit::OnBtnDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (GetSelData()) {
		if (m_pdata->strName==NORMAL_NAME) {
			CString str;
			str.Format("%sは、削除する事ができません。",NORMAL_NAME);
			AfxMessageBox(str);
			return;
		}
		CViewEnvTbl::iterator iter;
		if ((iter=m_pEnvTbl->find(m_pdata->strName))!=m_pEnvTbl->end()) {
			CViewEnv *pEnv = (*iter).second;
			if (pEnv->GetAttachCount()>0) {
				AfxMessageBox("現在使用中の設定は削除できません");
				return;
			}
			pEnv->m_profile.ResetArc();
			if (AfxMessageBox("実際の設定ファイル(*.mnt)ごと削除しますか？", MB_YESNO)==IDYES) {
				if (!DeleteFile(pEnv->m_profile.GetSzProfile())) {
					AfxMessageBox("他のプロセスが使用している等により削除できません");
					return;
				}
			}
			delete pEnv;
			m_pEnvTbl->erase(iter); //ｴﾝﾄﾘｰの削除
			theViewEnvTbl.WriteEnvEntryOnly();
		}
		
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem); //pNMListView->iItem
		if (p) {
			if (p->pdata)
				delete (ENV_EDIT_DATA *)(p->pdata);
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


void CConfigEdit::OnItemchangedLstCtr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	if (m_lstctr.IsWindowVisible()) {
		//if (phdn->uNewState==0) {
		//	m_pdata = null;
		//	*pResult = 0;
		//	return;
		//}
		bool bEnable = m_pdata!=null;
		GetSelData();
		if (bEnable != (m_pdata!=null)) {
			bEnable = !bEnable;
			GetDlgItem(IDC_BTN_DEL)->EnableWindow(bEnable);
			GetDlgItem(IDC_BTN_EDIT)->EnableWindow(bEnable);
		}
	}
	*pResult = 0;
}

bool CConfigEdit::GetSelData(bool bSetEnableWindow)
{
	bool bEnable = m_pdata!=null;
	m_nItem = m_lstctr.GetNextItem(-1, LVNI_SELECTED);
	if (m_nItem>=0) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem); //pNMListView->iItem
		if (p && p->pdata) {
			//dialog へｾｯﾄ
			m_pdata = (ENV_EDIT_DATA *)p->pdata;
			return true;
		}
	}
	if (bSetEnableWindow || bEnable != (m_pdata!=null)) {
		bEnable = m_pdata!=null;
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_EDIT)->EnableWindow(bEnable);
	}
	m_pdata = null;
	return false;
}

#endif //#ifdef SHAR