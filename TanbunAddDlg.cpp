// TanbunAddDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "TanbunAddDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

/////////////////////////////////////////////////////////////////////////////
// CTanbunAddDlg ダイアログ


CTanbunAddDlg::CTanbunAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTanbunAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTanbunAddDlg)
	m_strKey = _T("");
	m_strData = _T("");
	m_fmt = -1;
	m_bLayout = FALSE;
	//}}AFX_DATA_INIT
}


void CTanbunAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTanbunAddDlg)
	DDX_Text(pDX, IDC_EDT_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDT_TANBUN, m_strData);
	DDX_Radio(pDX, IDC_RDO_LEFT, m_fmt);
	DDX_Check(pDX, IDC_CHK_LAYOUT, m_bLayout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTanbunAddDlg, CDialog)
	//{{AFX_MSG_MAP(CTanbunAddDlg)
	ON_BN_CLICKED(IDC_CHK_LAYOUT, OnChkLayout)
	ON_BN_CLICKED(IDC_BTN_HOWTO, OnBtnHowto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTanbunAddDlg メッセージ ハンドラ

BOOL CTanbunAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	GetDlgItem(IDC_RDO_LEFT)->EnableWindow(m_bLayout);
	GetDlgItem(IDC_RDO_CENTER)->EnableWindow(m_bLayout);
	GetDlgItem(IDC_RDO_RIGHT)->EnableWindow(m_bLayout);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

bool CTanbunAddDlg::UpdateEnv(bool bSetToEnv, CTanbuns *pTanbuns, int mode, string &strKey, string &strData, int &fmt, const int index)
{
	assert(pTanbuns);
	if (bSetToEnv) {
		m_bChange = m_strKey != m_strKey_save || m_bChange;
		m_bChange = m_strData != m_strData_save || m_bChange;
		m_bChange = m_fmt != m_fmt_save || m_bChange;
		if (m_bChange) {
			strKey = m_strKey;
			strData = m_strData;
			fmt = m_fmt;
			if (m_mode==mode_new) {
				CTanbunData *pdata = new CTanbunData(m_strData, m_fmt);
				m_pTanbuns->AddNewEntry(m_strKey, pdata);
			}else{// if(mode==mode_update){
				assert(m_mode==mode_update);
				assert(m_index>=0);
				m_bChange = m_pTanbuns->UpdateEntry(m_index, m_strKey, m_strData, m_fmt);
			}
		}
	}else{
		m_bChange = false;
		m_mode = mode;
		if (mode==mode_new) {
			m_index = -1;
			m_strKey = "";
			m_bLayout = false;
			m_fmt = -1;
		}else{
			assert(index>=0);
			m_index  = index;
			m_strKey = strKey.data();
			if (fmt==CTanbunData::FMT_NO) {
				m_bLayout = false;
			}else{
				m_bLayout = true;
			}
			m_fmt = fmt;
		}
		m_strData = strData.data();
		m_strKey_save = m_strKey;
		m_strData_save = m_strData;
		m_fmt_save = m_fmt;
		m_pTanbuns = pTanbuns;
	}
	return m_bChange;
}

void CTanbunAddDlg::OnOK() 
{
	UpdateData(TRUE);

	if (m_strKey.IsEmpty() || m_strData.IsEmpty()) {
		AfxMessageBox("短文ｷｰﾜｰﾄﾞ及び、短文内容を入力して下さい。");
		if (m_strKey.IsEmpty())
			GetDlgItem(IDC_EDT_KEY)->SetFocus();
		else
			GetDlgItem(IDC_EDT_TANBUN)->SetFocus();
		return;
	}
	CDialog::OnOK();
}


void CTanbunAddDlg::OnChkLayout() 
{
	UpdateData(TRUE);
	m_fmt = (m_bLayout ? 0:-1);
	UpdateData(FALSE);
	GetDlgItem(IDC_RDO_LEFT)->EnableWindow(m_bLayout);
	GetDlgItem(IDC_RDO_CENTER)->EnableWindow(m_bLayout);
	GetDlgItem(IDC_RDO_RIGHT)->EnableWindow(m_bLayout);
}

void CTanbunAddDlg::OnBtnHowto() 
{
	AfxMessageBox("本文編集中に短文ｷｰﾜｰﾄﾞを入力後、[Ctrl + ｽﾍﾟｰｽｷｰ] を押すと登録内容に変換されます。\n"\
		"※短文ｷｰﾜｰﾄﾞは単語単位で設定及び入力する必要があります。\n"\
		"　もっとも一般的な方法は、短文ｷｰﾜｰﾄﾞを半角ローマ字で設定しておいて、\n"\
		"本文編集時に短文ｷｰﾜｰﾄﾞ前後を単語として認識される様に入力し\n"\
		"[Ctrl + ｽﾍﾟｰｽｷｰ]を押して変換する方法です。\n"\
		"尚、単語の切れ目は、全角／半角／ひらがな／カタカナ／漢字／空白文字などにあります。");
//	AfxMessageBox("短文ｷｰﾜｰﾄﾞを入力後、[Ctrl + ｽﾍﾟｰｽｷｰ] を押すと、\n"\
//		"登録内容に変換されます。\n"\
//		"同じｷｰﾜｰﾄﾞに複数の登録内容を別途登録する事も可能です。");
}

#endif //#ifdef SHAR
