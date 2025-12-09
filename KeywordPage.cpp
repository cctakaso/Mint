// KeywordPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "KeywordPage.h"
#include "KeyGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage プロパティ ページ
static char svch_keywordtail = 'A';

IMPLEMENT_DYNCREATE(CKeywordPage, CPropertyPage)

CKeywordPage::CKeywordPage() : CPropertyPage(CKeywordPage::IDD)
{
	//{{AFX_DATA_INIT(CKeywordPage)
	m_bEndRow = FALSE;
	m_bGroup = FALSE;
	m_bCase = FALSE;
	m_bUnderline = FALSE;
	m_bWord = FALSE;
	//}}AFX_DATA_INIT
	m_bChange = false;
	m_pkeywords = null;
	m_nItem = -1;
	m_pconf = null;
}

CKeywordPage::~CKeywordPage()
{
	if (m_pkeywords) {
		delete m_pkeywords;
		m_pkeywords = null;
	}
}

void CKeywordPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeywordPage)
	DDX_Control(pDX, IDC_BTN_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Check(pDX, IDC_CHK_ENDROW, m_bEndRow);
	DDX_Check(pDX, IDC_CHK_GROUP, m_bGroup);
	DDX_Check(pDX, IDC_CHK_CASE, m_bCase);
	DDX_Check(pDX, IDC_CHK_UNDERLINE, m_bUnderline);
	DDX_Control(pDX, IDC_LISTCTR_COLORS, m_lstctr);
	DDX_Check(pDX, IDC_CHK_WORD, m_bWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeywordPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKeywordPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	ON_BN_CLICKED(IDC_CHK_GROUP, OnChkGroup)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LISTCTR_COLORS, OnEndlabeleditListctrColors)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTR_COLORS, OnItemchangedListctrColors)
	ON_BN_CLICKED(IDC_CHK_ENDROW, OnChkEndrow)
	ON_BN_CLICKED(IDC_CHK_CASE, OnChkMeta)
	ON_BN_CLICKED(IDC_CHK_UNDERLINE, OnChkUnderline)
	ON_BN_CLICKED(IDC_CHK_WORD, OnChkWord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage メッセージ ハンドラ
BOOL CKeywordPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	svch_keywordtail = 'A';

	// TODO: この位置に初期化の補足処理を追加してください
	#define NMASK	(LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM)
	m_smallImage.Create(IDB_ENV_SMALL, 16, 1, RGB(255, 255, 255));
	m_stateImage.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_lstctr.LockUpdate(TRUE);
	VERIFY(m_lstctr.SetSelType(LVS_REPORT, st_full/*st_grid*/, TRUE, TRUE, TRUE) && m_lstctr.SetImageLists(&m_smallImage, NULL, &m_stateImage));
	m_lstctr.AddColumn(_T("   ｷｰﾜｰﾄﾞ(/ｸﾞﾙｰﾌﾟ名称)   "),0,LVCFMT_LEFT, FALSE, NMASK,-1);
	m_lstctr.AddColumn(_T("  (ｸﾞﾙｰﾌﾟｷｰﾜｰﾄﾞ)  "),1, LVCFMT_LEFT, TRUE, NMASK,-1);

	int i=0;
	for (CKeywords::iterator iter=m_pkeywords->begin(); iter!=m_pkeywords->end(); iter++, i++) {
		CKeyword *pkeyword = (*iter).second;
		CKeywordConf *pconf = new CKeywordConf;
		*pconf = *pkeyword->m_pconf;
		m_lstctr.AddItem(0, 0, (*iter).first.c_str(), TRUE, 
			pkeyword->m_pconf->m_color, pkeyword->m_pconf->m_bEnable, pconf);
		m_lstctr.AddItem(0, 1, pkeyword->m_strGroup.c_str());
	}
	m_lstctr.LockUpdate(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

bool CKeywordPage::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	if (bSetToEnv) {
		//m_bChange = (pEnv->m_ratioBetweenLine != m_ratioBetweenLine) || m_bChange;
		if (m_bChange) {
			pEnv->m_keywords.Copy(m_pkeywords, m_bChange);
			pEnv->m_keywords.Copy_OnlyEnable(pEnv->m_pkeywords_word, pEnv->m_pkeywords_unword);
		}
		delete m_pkeywords;
		m_pkeywords = null;
	}else{
		m_bChange = false;
		m_pkeywords = pEnv->m_keywords.CopyNew();
		assert(m_pkeywords);
	}
	return m_bChange;
}

void CKeywordPage::OnBtnAdd() 
{
	int nItem = m_lstctr.GetItemCount();
	string strName;
	LV_FINDINFO lvf;
	lvf.flags = LVFI_STRING;
	do {
		strName = "新しいｷｰﾜｰﾄﾞ";
		strName += svch_keywordtail;
		lvf.psz = strName.data();
	}while (m_lstctr.FindItem(&lvf, -1)>=0 && ++svch_keywordtail<='Z');
	CKeywordConf *pconf = new CKeywordConf;
	m_lstctr.AddItem(nItem, 0, strName.data(), TRUE, RGB(0,0,0), TRUE, pconf);
	m_lstctr.AddItem(nItem, 1, "");
	m_lstctr.SetFocus();
	m_lstctr.SetItemState(nItem, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);
	m_lstctr.EditLabel(nItem);
	m_pconf = pconf;
	m_bChange = true;
}

void CKeywordPage::OnBtnDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (GetSelData()) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem);
		if (p) {
			if (p->pdata)
				delete ((CKeywordConf*)p->pdata);
			delete p;
		}
		m_lstctr.DeleteItem(m_nItem);
		if (m_lstctr.GetItemCount()>0) {
			if (m_nItem>0)
				m_nItem--;
			m_lstctr.SetFocus();
			m_lstctr.SetItemState(m_nItem, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);
		}
		m_bChange = true;
	}
}

void CKeywordPage::OnBtnUpdate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if (GetSelData()) {
		if (!m_bGroup) {
			m_lstctr.SetFocus();
			m_lstctr.SetItemState(m_nItem, LVNI_SELECTED | LVIS_FOCUSED , LVIF_STATE);
			m_lstctr.EditLabel(m_nItem);
		}else{
			//ｸﾞﾙｰﾌﾟダイアログ表示
			OnChkGroup();
		}
	}
}



void CKeywordPage::OnEndlabeleditListctrColors(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (GetSelData()) {
		CEdit *pEdit = m_lstctr.GetEditControl();
		if (pEdit) {
			CString str;
			pEdit->GetWindowText(str);
			LV_FINDINFO lvf;
			lvf.flags = LVFI_STRING;
			lvf.psz = str.GetBuffer(0);
			int nItemFind;
			if ((nItemFind=m_lstctr.FindItem(&lvf, -1))>=0 && (nItemFind == m_nItem && 
				m_lstctr.FindItem(&lvf, m_nItem+1)>=0)) {
				AfxMessageBox("同名のｷｰﾜｰﾄﾞが既にあります。別のｷｰﾜｰﾄﾞを指定して下さい");
			}else
				m_lstctr.SetItemText(m_nItem, 0, str);
		}
		m_bChange = true;
	}
	*pResult = 0;
}

void CKeywordPage::OnItemchangedListctrColors(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (m_lstctr.IsWindowVisible()) {
		if (pNMListView->uNewState==0) {
			m_pconf = null;
			*pResult = 0;
			return;
		}
		bool bEnable = m_pconf!=null;
		if (GetSelData()) {
			m_bEndRow = m_pconf->m_keycpos == keycpos_rowend;
			m_bGroup = m_lstctr.GetItemText(pNMListView->iItem, 1).GetLength()>0;
			m_bWord = m_pconf->m_bWord;
			m_bCase = m_pconf->m_bCase;
			m_bUnderline = m_pconf->m_bUnderline;
			//m_keytype = -1;
			UpdateData(FALSE);
		}else
			m_pconf = null;
		if (bEnable != (m_pconf!=null)) {
			bEnable = !bEnable;
			GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(bEnable);
			GetDlgItem(IDC_BTN_DEL)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_ENDROW)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_GROUP)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_WORD)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_CASE)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_UNDERLINE)->EnableWindow(bEnable);
		}
	}
	*pResult = 0;
}

void CKeywordPage::OnDestroy() 
{
	
	m_pkeywords->Delete();

	//リストから再構築する
	for (int nItem=0; nItem<m_lstctr.GetItemCount(); nItem++) {
		string strName = m_lstctr.GetItemText(nItem, 0);
		string strKeywords = m_lstctr.GetItemText(nItem, 1);
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(nItem);
		assert(p && p->pdata);
		CKeywordConf *pconf = new CKeywordConf;
		if (p->pdata) {
			*pconf = *(CKeywordConf*)p->pdata;
			pconf->m_color = p->color;
			delete (CKeywordConf*)p->pdata;
			delete p;
		}else
			pconf->m_color = p->color;
		UINT val = m_lstctr.GetItemState(nItem, LVIS_STATEIMAGEMASK);
		if (val==INDEXTOSTATEIMAGEMASK(1) || val==INDEXTOSTATEIMAGEMASK(2)) {
			bool bEnable = val==INDEXTOSTATEIMAGEMASK(2);
			pconf->m_bEnable = bEnable;
		}
		CKeyword *pKeyword = new CKeyword(strKeywords.c_str(), pconf);
		(*m_pkeywords)[strName] = pKeyword;
	}
	m_bChange = m_bChange || m_lstctr.IsDirty();	
	
	CPropertyPage::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CKeywordPage::OnChkGroup() 
{
	UpdateData(TRUE);
	if (GetSelData()) {
		//ｸﾞﾙｰﾌﾟダイアログ表示
		CKeyGroupDlg dlg;
		dlg.m_strName = m_lstctr.GetItemText(m_nItem, 0);
		dlg.m_strKeywords = m_lstctr.GetItemText(m_nItem, 1);
#ifdef _DEBUG
			int len = dlg.m_strKeywords.GetLength();
#endif
		int rtn = dlg.DoModal();
		if (rtn==IDOK) {
			m_lstctr.SetItemText(m_nItem, 0, dlg.m_strName);
#ifdef _DEBUG
			int len = dlg.m_strKeywords.GetLength();
#endif
			m_lstctr.SetItemText(m_nItem, 1, dlg.m_strKeywords);
		}
	}
	m_bChange = true;
}

void CKeywordPage::OnChkEndrow() 
{
	UpdateData(TRUE);
	if (GetSelData()) {
		m_pconf->m_keycpos = m_bEndRow ? keycpos_rowend:keycpos_no;
		m_pconf->m_bWord = BOOL2bool(m_bWord);
		m_pconf->m_bCase = BOOL2bool(m_bCase);
		m_pconf->m_bUnderline = BOOL2bool(m_bUnderline);
		m_bChange = true;
	}
}

void CKeywordPage::OnChkMeta() 
{
	OnChkEndrow();
	if (GetSelData()) {
		CString strKeywords = m_lstctr.GetItemText(m_nItem, 1);
		if (m_bCase) {
			strKeywords.MakeLower();
		}else{
			strKeywords.MakeUpper();
		}
		m_lstctr.SetItemText(m_nItem, 1, strKeywords);
	}
}

void CKeywordPage::OnChkUnderline() 
{
	OnChkEndrow();
}

void CKeywordPage::OnChkWord() 
{
	OnChkEndrow();
}
bool CKeywordPage::GetSelData()
{
	m_nItem = m_lstctr.GetNextItem(-1, LVNI_SELECTED);
	if (m_nItem>=0) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(m_nItem); //pNMListView->iItem
		if (p && p->pdata) {
			//dialog へｾｯﾄ
			m_pconf = (CKeywordConf *)p->pdata;
			return true;
		}
	}
	m_pconf = null;
	return false;
}
#endif //#ifdef SHAR
