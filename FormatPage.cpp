// FormatPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "FormatPage.h"
#include "TheDoc.h"
#include "MainFrm.h"
#include "ObjLay.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatPage プロパティ ページ

IMPLEMENT_DYNCREATE(CFormatPage, CPropertyPage)

CFormatPage::CFormatPage() : CPropertyPage(CFormatPage::IDD)
{
	//{{AFX_DATA_INIT(CFormatPage)
	m_optOrient = -1;
	m_iColTab = 0;
	m_optColBlank = -1;
	m_iColBlank = 0;
	m_bRowNum = FALSE;
	m_bAutoIndent = FALSE;
	//}}AFX_DATA_INIT
}

CFormatPage::~CFormatPage()
{
}

void CFormatPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatPage)
	DDX_Control(pDX, IDC_RAO_COL_BLACK_NO, m_raoColBlank);
	DDX_Control(pDX, IDC_EDT_COL_TAB, m_edtColTab);
	DDX_Control(pDX, IDC_EDT_COL_BLACK, m_edtColBlank);
	DDX_Radio(pDX, IDC_RAO_HORZ, m_optOrient);
	DDX_Text(pDX, IDC_EDT_COL_TAB, m_iColTab);
	DDV_MinMaxInt(pDX, m_iColTab, 2, 8);
	DDX_Radio(pDX, IDC_RAO_COL_BLACK_NO, m_optColBlank);
	DDX_Text(pDX, IDC_EDT_COL_BLACK, m_iColBlank);
	DDV_MinMaxInt(pDX, m_iColBlank, 20, 256);
	DDX_Check(pDX, IDC_CHK_LINENUM_ROW, m_bRowNum);
	DDX_Check(pDX, IDC_CHK_AUTOINDENT, m_bAutoIndent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFormatPage)
	ON_BN_CLICKED(IDC_RAO_COL_BLACK, OnRaoColBlack)
	ON_BN_CLICKED(IDC_RAO_COL_BLACK_NO, OnRaoColBlackNo)
	ON_EN_CHANGE(IDC_EDT_COL_TAB, OnChangeEdtColTab)
	ON_EN_CHANGE(IDC_EDT_COL_BLACK, OnChangeEdtColBlack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatPage メッセージ ハンドラ
bool CFormatPage::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	if (bSetToEnv) {
		//書き換えに時間のかかる重要な項目なので、厳密に変更チェックする
		m_bChange = (m_optOrient_save != m_optOrient) || m_bChange;
		m_bChange_format = (m_iColTab_save != m_iColTab) || m_bChange_format;
		m_bChange_format = (m_optColBlank_save != m_optColBlank) || m_bChange_format;
		m_bChange_format = (m_optColBlank == 1 && m_iColBlank_save != m_iColBlank) || m_bChange_format;
		m_bChange = (m_bRowNum_save != m_bRowNum) || m_bChange;
		m_bChange = m_bChange_format || m_bChange;
		m_bChange = (m_bAutoIndent_save != m_bAutoIndent) || m_bChange;
		if (m_bChange) {
			//UpdateData(TRUE);
			pEnv->m_bVert = (m_optOrient == 1);
			pEnv->m_nTabStops = m_iColTab;
			pEnv->m_xdspMax = m_optColBlank==0 ? SHRT_MAX:m_iColBlank;
			pEnv->m_xdspMax_save = m_iColBlank;
			pEnv->m_bRowNum = BOOL2bool(m_bRowNum);
			pEnv->m_bAutoIndent = BOOL2bool(m_bAutoIndent);
		}
		//表示＆ﾌｫｰﾏｯﾄに関係ないので、アップデートチェックはなし
	}else{
		m_bChange = false;
		m_bChange_format = false;

		m_optOrient = pEnv->m_bVert ? 1:0;
		m_iColTab = pEnv->m_nTabStops;
		m_bAutoIndent = pEnv->m_bAutoIndent;
		m_optColBlank = (pEnv->m_xdspMax == SHRT_MAX) ? 0:1;
		if (m_optColBlank==0)
			m_iColBlank = pEnv->m_xdspMax_save;
		else
			m_iColBlank = pEnv->m_xdspMax;
		m_bRowNum = bool2BOOL(pEnv->m_bRowNum);
		
		//Save values
		//書き換えに時間のかかる重要な項目なので、厳密に変更チェックする
		m_optOrient_save = m_optOrient;
		m_iColTab_save = m_iColTab;
		m_bAutoIndent_save = m_bAutoIndent;
		m_optColBlank_save = m_optColBlank;
		m_bRowNum_save = m_bRowNum;
		m_iColBlank_save = m_iColBlank;

	}
	return m_bChange;

}


void CFormatPage::OnRaoColBlack() 
{
	

	UpdateData(TRUE);
	m_edtColBlank.EnableWindow(m_optColBlank==1);
}

void CFormatPage::OnRaoColBlackNo() 
{
	OnRaoColBlack();
}

void CFormatPage::OnChangeEdtColTab() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CPropertyPage::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CFormatPage::OnChangeEdtColBlack() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CPropertyPage::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

BOOL CFormatPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//ﾌｫｰﾏｯﾄ中であればﾃﾞｨｽｲﾈｰﾌﾞﾙにする
	bool bEnable = ((CTheDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())->m_pMainObjLay->IsFormatAll();;
	m_raoColBlank.EnableWindow(bEnable);
	m_edtColBlank.EnableWindow(bEnable);
	m_edtColTab.EnableWindow(bEnable);
	GetDlgItem(IDC_RAO_COL_BLACK)->EnableWindow(bEnable);
#ifdef FREE
	GetDlgItem(IDC_CHK_AUTOINDENT)->EnableWindow(FALSE);
#endif
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
