// ColorDlg.cpp : インプリメンテーション ファイル
//
#include "stdafx.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg ダイアログ

CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pEnv = null;
	//Initialize(TRUE);
	
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_LST_COLOR, m_lstColors);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_LBN_DBLCLK(IDC_LST_COLOR, OnDblclkLstColor)
	ON_LBN_SELCHANGE(IDC_LST_COLOR, OnSelchangeLstColor)
	ON_BN_CLICKED(ID_CHANGE_COLOR, OnChangeColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg メッセージ ハンドラ
BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// コントロールのサブクラス化
	//VERIFY(m_lstColors.SubclassDlgItem(IDC_LST_COLOR, this));

	// リストボックスへ追加
	Initialize();
	m_lstColors.SetCurSel(0);
	OnSelchangeLstColor();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CColorDlg::Initialize(BOOL fMemOnly)
{
	assert(m_pEnv);
	// リストボックスへ追加
	if (!fMemOnly)
		m_lstColors.ResetContent();
	CColorSel sel;
	string str;
	str = "1,ﾃｷｽﾄ,";
	str += m_pEnv->m_strColorText;
	sel.Initialize(str, false);
	m_lstColors.AddColorItem(sel, fMemOnly);
	str = "2,背景,";
	str += m_pEnv->m_strColorTextBk;
	sel.Initialize(str, false);
	m_lstColors.AddColorItem(sel, fMemOnly);
}

void CColorDlg::OnOK() 
{
	CColorSel *psel = GetSelData(0);
	if (!psel)
		goto Err;
	COLORREF frColor, bkColor;
	frColor = psel->GetColor();
	m_pEnv->m_strColorText = psel->GetStringColor();

	psel = GetSelData(1);
	if (!psel)
		goto Err;
	bkColor = psel->GetColor();
	m_pEnv->m_strColorTextBk = psel->GetStringColor();
	
	CDialog::OnOK();
	return;
Err:
	Initialize();
	AfxMessageBox(_T("色指定できませんでした。"));
}

void CColorDlg::OnCancel() 
{
	CDialog::OnCancel();
}


void CColorDlg::OnSelchangeLstColor()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (::IsWindow(m_btnOk.m_hWnd))
		m_btnOk.EnableWindow(m_lstColors.GetCurSel()!=LB_ERR);
}

void CColorDlg::OnDblclkLstColor() 
{
	OnChangeColor();
}

void CColorDlg::OnChangeColor() 
{
	// 最後の色を取得
	DWORD color;
	CColorSel *pselData = m_lstColors.GetSelData();
	if (pselData==NULL)
		return;
	color = pselData->GetColor();
	CColorDialog dlg(color);
	//dlg.m_cc.Flags = CC_ENABLEHOOK;
	if (dlg.DoModal()==IDOK) {
		//int sel = m_lstColors.GetCurSel();
		pselData->SetColor(dlg.GetColor());
		m_lstColors.Invalidate();
		//Initialize();
		//if (sel!=LB_ERR)
		//	m_lstColors.SetCurSel(sel);
	}
}
