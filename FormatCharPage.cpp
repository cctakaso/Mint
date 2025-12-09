// FormatCharPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "FormatCharPage.h"
#include "TheDoc.h"
#include "MainFrm.h"
#include "ObjLay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern char *stcpKinStrs[];
/////////////////////////////////////////////////////////////////////////////
// CFormatCharPage ダイアログ

IMPLEMENT_DYNCREATE(CFormatCharPage, CPropertyPage)

CFormatCharPage::CFormatCharPage() : CPropertyPage(CFormatCharPage::IDD)
{
	//{{AFX_DATA_INIT(CFormatCharPage)
	m_bKinsok = FALSE;
	m_strKinFirst = _T("");
	m_strKinLast = _T("");
	m_iKinLevel = -1;
	m_bWordWrap = FALSE;
	m_bKinFirst = FALSE;
	m_bKinLast = FALSE;
	m_bCRLFBra = FALSE;
	//}}AFX_DATA_INIT
}

CFormatCharPage::~CFormatCharPage()
{
}
void CFormatCharPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatCharPage)
	DDX_Control(pDX, IDC_CHK_KIN_LAST, m_btnKinLast);
	DDX_Control(pDX, IDC_CHK_KIN_FIRST, m_btnKinFirst);
	DDX_Control(pDX, IDC_RDO_KIN_LEVEL0, m_optKinLevel);
	DDX_Control(pDX, IDC_EDT_KIN_LAST, m_edtKinLast);
	DDX_Control(pDX, IDC_EDT_KIN_FIRST, m_edtKinFirst);
	DDX_Control(pDX, IDC_CHK_KINSOK, m_chkKinsoku);
	DDX_Check(pDX, IDC_CHK_KINSOK, m_bKinsok);
	DDX_Text(pDX, IDC_EDT_KIN_FIRST, m_strKinFirst);
	DDX_Text(pDX, IDC_EDT_KIN_LAST, m_strKinLast);
	DDX_Radio(pDX, IDC_RDO_KIN_LEVEL0, m_iKinLevel);
	DDX_Check(pDX, IDC_CHK_WORDWRAP, m_bWordWrap);
	DDX_Check(pDX, IDC_CHK_KIN_FIRST, m_bKinFirst);
	DDX_Check(pDX, IDC_CHK_KIN_LAST, m_bKinLast);
	DDX_Check(pDX, IDC_CHK_CRLF_BRASAGE, m_bCRLFBra);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatCharPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFormatCharPage)
	ON_BN_CLICKED(IDC_CHK_KINSOK, OnChkKinsok)
	ON_BN_CLICKED(IDC_RDO_KIN_LEVEL1, OnRdoKinLevel1)
	ON_EN_CHANGE(IDC_EDT_KIN_FIRST, OnChangeEdtKinFirst)
	ON_EN_CHANGE(IDC_EDT_KIN_LAST, OnChangeEdtKinLast)
	ON_BN_CLICKED(IDC_RDO_KIN_LEVEL2, OnRdoKinLevel2)
	ON_BN_CLICKED(IDC_RDO_KIN_CUST, OnRdoKinCust)
	ON_BN_CLICKED(IDC_CHK_KIN_FIRST, OnChkKinFirst)
	ON_BN_CLICKED(IDC_CHK_KIN_LAST, OnChkKinLast)
	ON_BN_CLICKED(IDC_RDO_KIN_LEVEL0, OnRdoKinLevel0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatCharPage メッセージ ハンドラ

BOOL CFormatCharPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	OnChkKinsok();

	bool bEnable = ((CTheDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())->m_pMainObjLay->IsFormatAll();;
	if (!bEnable) {
		KinsokEnableWindow(false);
		GetDlgItem(IDC_CHK_KINSOK)->EnableWindow(false);
		GetDlgItem(IDC_CHK_CRLF_BRASAGE)->EnableWindow(false);
		GetDlgItem(IDC_CHK_WORDWRAP)->EnableWindow(false);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
bool CFormatCharPage::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	if (bSetToEnv) {
		//書き換えに時間のかかる重要な項目なので、厳密に変更チェックする
		SetBool2(m_bChange, (BOOL2bool(m_bWordWrap) != pEnv->m_bWordWrap));
		SetBool2(m_bChange, (BOOL2bool(m_bCRLFBra) != pEnv->m_bCRLFBra));
		SetBool2(m_bChange, (BOOL2bool(m_bKinsok) != pEnv->m_bKinsok));
		SetBool2(m_bChange, (m_iKinLevel != pEnv->m_iKinLevel && m_bKinsok));
		SetBool2(m_bChange, (BOOL2bool(m_bKinFirst) != pEnv->m_bKinFirst));
		SetBool2(m_bChange, (CString2string(m_strKinFirst) != pEnv->m_strKinFirst && m_bKinsok));
		SetBool2(m_bChange, (BOOL2bool(m_bKinLast) != pEnv->m_bKinLast));
		SetBool2(m_bChange, (CString2string(m_strKinLast) != pEnv->m_strKinLast && m_bKinsok));
		m_bChange_format = m_bChange;
		if (m_bChange) {
			//UpdateData(TRUE);
			pEnv->m_bWordWrap = BOOL2bool(m_bWordWrap);
			pEnv->m_bCRLFBra = BOOL2bool(m_bCRLFBra);
			pEnv->m_bKinsok = BOOL2bool(m_bKinsok);
			pEnv->m_iKinLevel = m_iKinLevel;
			pEnv->m_bKinFirst = BOOL2bool(m_bKinFirst);
			pEnv->m_strKinFirst = CString2string(m_strKinFirst);
			pEnv->m_bKinLast = BOOL2bool(m_bKinLast);
			pEnv->m_strKinLast = CString2string(m_strKinLast);
		}
	}else{
		m_bChange = false;
		m_bChange_format = false;

		m_bWordWrap = bool2BOOL(pEnv->m_bWordWrap);
		m_bCRLFBra = bool2BOOL(pEnv->m_bCRLFBra);
		m_bKinsok = bool2BOOL(pEnv->m_bKinsok);
		m_iKinLevel = pEnv->m_iKinLevel;
		m_bKinFirst = bool2BOOL(pEnv->m_bKinFirst);
		m_strKinFirst = string2CString(pEnv->m_strKinFirst);
		m_bKinLast = bool2BOOL(pEnv->m_bKinLast);
		m_strKinLast = string2CString(pEnv->m_strKinLast);
	}
	return m_bChange;

}

void CFormatCharPage::OnChkKinsok() 
{
	UpdateData(TRUE);
	KinsokEnableWindow(m_bKinsok==TRUE);
	UpdateData(FALSE);
}

void CFormatCharPage::KinsokEnableWindow(bool bEnable) 
{
	m_optKinLevel.EnableWindow(bEnable);
	m_btnKinFirst.EnableWindow(bEnable);
	m_edtKinFirst.EnableWindow(bEnable && m_bKinFirst);
	m_btnKinLast.EnableWindow(bEnable);
	m_edtKinLast.EnableWindow(bEnable && m_bKinLast);

	GetDlgItem(IDC_RDO_KIN_LEVEL1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_KIN_LEVEL2)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_KIN_CUST)->EnableWindow(bEnable);
}

void CFormatCharPage::OnChangeEdtKinFirst() 
{
	UpdateData(TRUE);
	m_iKinLevel = kin_levelC;
	UpdateData(FALSE);
}

void CFormatCharPage::OnChangeEdtKinLast() 
{
	OnChangeEdtKinFirst();
}

void CFormatCharPage::OnRdoKinLevel0() 
{
	UpdateData(TRUE);
	if (m_iKinLevel>=0 && m_iKinLevel<kin_levelC) {
		m_strKinFirst = stcpKinStrs[m_iKinLevel*2];
		m_strKinLast = stcpKinStrs[m_iKinLevel*2+1];
	}
	UpdateData(FALSE);
}
void CFormatCharPage::OnRdoKinLevel1() 
{
	OnRdoKinLevel0();
}

void CFormatCharPage::OnRdoKinLevel2() 
{
	OnRdoKinLevel0();
}

void CFormatCharPage::OnRdoKinCust() 
{
	OnRdoKinLevel0();
}

void CFormatCharPage::OnChkKinFirst() 
{
	UpdateData(TRUE);
	m_edtKinFirst.EnableWindow(m_bKinFirst);
}

void CFormatCharPage::OnChkKinLast() 
{
	UpdateData(TRUE);
	m_edtKinLast.EnableWindow(m_bKinLast);
}

