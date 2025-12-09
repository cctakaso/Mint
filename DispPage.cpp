// DispPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "DispPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern DSP_COLORS dsp_colors[];
/////////////////////////////////////////////////////////////////////////////
// CDispPage プロパティ ページ

IMPLEMENT_DYNCREATE(CDispPage, CPropertyPage)

CDispPage::CDispPage() : CPropertyPage(CDispPage::IDD)
{
	//{{AFX_DATA_INIT(CDispPage)
	m_strFontHorz = _T("");
	m_strFontVert = _T("");
	m_ratioBetweenLine = 0;
	//}}AFX_DATA_INIT
}

CDispPage::~CDispPage()
{
}

void CDispPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispPage)
	DDX_Control(pDX, IDC_LISTCTR_COLORS, m_lstctr);
	DDX_Control(pDX, IDC_BTN_FONT_VERT, m_btnFontVert);
	DDX_Control(pDX, IDC_BTN_FONT_HORZ, m_btnFontHorz);
	DDX_Text(pDX, IDC_EDT_FONT_HOR, m_strFontHorz);
	DDX_Text(pDX, IDC_EDT_FONT_VERT, m_strFontVert);
	DDX_Text(pDX, IDC_EDT_RATIO_BETWEEN_LINE, m_ratioBetweenLine);
	DDV_MinMaxInt(pDX, m_ratioBetweenLine, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDispPage)
	ON_BN_CLICKED(IDC_BTN_FONT_HORZ, OnBtnFontHorz)
	ON_BN_CLICKED(IDC_BTN_FONT_VERT, OnBtnFontVert)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispPage メッセージ ハンドラ
BOOL CDispPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Initialize();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

bool CDispPage::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	if (bSetToEnv) {
		m_bChange = (pEnv->m_ratioBetweenLine != m_ratioBetweenLine) || m_bChange;
		if (m_bChange) {
//			UpdateData(TRUE);
			memcpy(pEnv->m_envColors, m_envColors, sizeof(m_envColors));
			pEnv->m_lf = m_lf;
			pEnv->m_lfVert = m_lfVert;
			pEnv->m_ratioBetweenLine = m_ratioBetweenLine;
		}
	}else{
		m_bChange = false;
		memcpy(m_envColors, pEnv->m_envColors, sizeof(m_envColors));
		m_lf = pEnv->m_lf;
		m_strFontHorz = FontChange(m_lf, false);
		m_lfVert = pEnv->m_lfVert;
		m_strFontVert = FontChange(m_lfVert, false);
		m_ratioBetweenLine = pEnv->m_ratioBetweenLine;
	}
	return m_bChange;
}

void CDispPage::Initialize(BOOL fMemOnly)
{
	// リストボックスへ追加
	#define NMASK	(LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM)
	m_smallImage.Create(IDB_ENV_SMALL, 16, 1, RGB(255, 255, 255));
	m_stateImage.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
	m_lstctr.LockUpdate(TRUE);
	VERIFY(m_lstctr.SetSelType(LVS_REPORT, st_full/*st_grid*/, FALSE, TRUE, TRUE) && m_lstctr.SetImageLists(&m_smallImage, NULL, &m_stateImage));
	m_lstctr.AddColumn(_T("           項目            "),0,LVCFMT_LEFT, FALSE, NMASK,-1);

	for (int i=0; i<idc_max; i++) {
		m_lstctr.AddItem(i, 0, dsp_colors[i].title, TRUE, INT2COLORREF(m_envColors[dsp_colors[i].index].color), dsp_colors[i].bstate ? m_envColors[dsp_colors[i].index].bEnable:-1);
	}
	m_lstctr.LockUpdate(FALSE);
}




void CDispPage::OnBtnFontHorz() 
{
	m_strFontHorz = FontChange(m_lf, true);
	UpdateData(FALSE);
}

void CDispPage::OnBtnFontVert() 
{
	m_strFontVert = FontChange(m_lfVert, true);
	UpdateData(FALSE);
}

CString CDispPage::FontChange(LOGFONT &lf, bool bDoChange)
{
	LOGFONT lf_new = lf;
	CString str; 
	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));

	// now bring up the dialog since we know the printer DC
	CFontDialog dlg(&lf_new, CF_SCREENFONTS, &dcScreen);
	if (bDoChange && dlg.DoModal() == IDOK) {
		// map the resulting logfont back to printer metrics.
		lf = lf_new;
		lf.lfWidth = 0;
		//lf.lfWeight = 400;
		lf.lfHeight = abs(lf.lfHeight);
		m_bChange = true;
	}
	str = dlg.GetFaceName();
	CString str_t;
	int pt10 = ::MulDiv(abs(lf.lfHeight), 720, dcScreen.GetDeviceCaps(LOGPIXELSY));
	str_t.Format(" %d.%d ﾎﾟｲﾝﾄ", pt10/10, pt10%10>=5 ? 5:0);
	str += str_t;
	::ReleaseDC(NULL, dcScreen.Detach());
	return str;
}



/*
void CDispPage::OnItemchangedListctrColors(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (m_lstctr.IsWindowVisible()) {
		int index = dsp_colors[pNMListView->iItem].index;
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(pNMListView->iItem);
		if (p && m_envColors[index].color != p->color) {
			m_envColors[index].color = p->color;
			m_bChange = true;
		}
		UINT val = m_lstctr.GetItemState(pNMListView->iItem, LVIS_STATEIMAGEMASK);
		if (val==INDEXTOSTATEIMAGEMASK(1) || val==INDEXTOSTATEIMAGEMASK(2)) {
			bool bEnable = val==INDEXTOSTATEIMAGEMASK(2);
			if (m_envColors[index].bEnable != bEnable) {
				m_envColors[index].bEnable = bEnable;
				m_bChange = true;
			}
		}
	}
	*pResult = 0;
}
*/
void CDispPage::OnDestroy() 
{
	//リストから再構築する
	for (int nItem=0; nItem<m_lstctr.GetItemCount(); nItem++) {
		COLOR_ITEM_DATA *p = (COLOR_ITEM_DATA *)m_lstctr.GetItemData(nItem);
		assert(p);
		int index = dsp_colors[nItem].index;
		m_envColors[index].color = p->color;

		UINT val = m_lstctr.GetItemState(nItem, LVIS_STATEIMAGEMASK);
		if (val==INDEXTOSTATEIMAGEMASK(1) || val==INDEXTOSTATEIMAGEMASK(2)) {
			bool bEnable = val==INDEXTOSTATEIMAGEMASK(2);
			m_envColors[index].bEnable = bEnable;
		}
		delete p;
	}
	m_bChange = m_bChange || m_lstctr.IsDirty();	

	CPropertyPage::OnDestroy();
	
}
