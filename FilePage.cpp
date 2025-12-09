// FilePage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "FilePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilePage プロパティ ページ

IMPLEMENT_DYNCREATE(CFilePage, CPropertyPage)

CFilePage::CFilePage() : CPropertyPage(CFilePage::IDD)
{
	//{{AFX_DATA_INIT(CFilePage)
	m_optBackup = -1;
	m_strBackFolder = _T("");
#ifdef EDITMENU_OFLAY
	m_bEditMenu_atOpen = FALSE;
	m_bGird_atEditMenu = FALSE;
#endif
	m_strExtents = _T("");
	m_bRockUpdate = FALSE;
	//}}AFX_DATA_INIT
	m_bRockUpdate_Chnage = false;
}

CFilePage::~CFilePage()
{
}

void CFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilePage)
	DDX_Control(pDX, IDC_EDT_BACKUP_FOLDER, m_edtBackupFolder);
	DDX_Control(pDX, IDC_BTN_BACKUP_FOLDER, m_btnBackupFolder);
	DDX_Radio(pDX, IDC_RAO_BACKUP_NO, m_optBackup);
	DDX_Text(pDX, IDC_EDT_BACKUP_FOLDER, m_strBackFolder);
#ifdef EDITMENU_OFLAY
	DDX_Check(pDX, IDC_CHK_EDIT_MENU_ATOPEN, m_bEditMenu_atOpen);
	DDX_Check(pDX, IDC_CHK_GIRD_ATEDITMENU, m_bGird_atEditMenu);
#endif
	DDX_CBString(pDX, IDC_CMB_EXTENTS, m_strExtents);
	DDX_Check(pDX, IDC_CHK_ROCKUPDATE, m_bRockUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilePage, CPropertyPage)
	//{{AFX_MSG_MAP(CFilePage)
	ON_BN_CLICKED(IDC_BTN_BACKUP_FOLDER, OnBtnBackupFolder)
	ON_BN_CLICKED(IDC_RAO_BACKUP_NO, OnRaoBackupNo)
	ON_BN_CLICKED(IDC_RAO_BACKUP_BAK, OnRaoBackupBak)
	ON_BN_CLICKED(IDC_RAO_BACKUP_TRASH, OnRaoBackupTrash)
	ON_BN_CLICKED(IDC_RAO_BACKUP_FOLDER, OnRaoBackupFolder)
	ON_BN_CLICKED(IDC_CHK_ROCKUPDATE, OnChkRockupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilePage メッセージ ハンドラ
bool CFilePage::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	if (bSetToEnv) {
		SetBool2(m_bChange, (CString2string(m_strExtents) != pEnv->m_strExtents));
#ifdef EDITMENU_OFLAY
		SetBool2(m_bChange, (BOOL2bool(m_bEditMenu_atOpen) != pEnv->m_bEditMenu_atOpen));
		SetBool2(m_bChange, (BOOL2bool(m_bGird_atEditMenu) != pEnv->m_bGird_atEditMenu));
#endif
		SetBool2(m_bChange, (m_optBackup != pEnv->m_iBackFile));
		SetBool2(m_bChange, ((CString2string(m_strBackFolder)+"\\") != pEnv->m_strBackFolder));
		SetBool2(m_bChange, (BOOL2bool(m_bRockUpdate) != pEnv->m_bRockUpdate));
		m_bRockUpdate_Chnage = m_bRockUpdate && (!pEnv->m_bRockUpdate);
		if (m_bChange) {
			pEnv->m_strExtents = m_strExtents.GetBuffer(0);
#ifdef EDITMENU_OFLAY
			pEnv->m_bEditMenu_atOpen = BOOL2bool(m_bEditMenu_atOpen);
			pEnv->m_bGird_atEditMenu = BOOL2bool(m_bGird_atEditMenu);
#endif
			pEnv->m_iBackFile = m_optBackup;
			if (m_strBackFolder.IsEmpty()) {
				pEnv->m_strBackFolder = GetModuleDirDir(AfxGetInstanceHandle()) + "back\\";
			}else{
				pEnv->m_strBackFolder = m_strBackFolder;
				if (m_strBackFolder[0]!='?') //? is specal character
					pEnv->m_strBackFolder += "\\";
			}
			pEnv->m_bRockUpdate = BOOL2bool(m_bRockUpdate);
		}
	}else{
		m_bChange = false;
		m_bRockUpdate_Chnage = false;
		m_strExtents = pEnv->m_strExtents.c_str();
#ifdef EDITMENU_OFLAY
		m_bEditMenu_atOpen = bool2BOOL(pEnv->m_bEditMenu_atOpen);
		m_bGird_atEditMenu = bool2BOOL(pEnv->m_bGird_atEditMenu);
#endif
		m_optBackup = pEnv->m_iBackFile;
		m_strBackFolder = pEnv->m_strBackFolder.c_str();
		m_bRockUpdate = pEnv->m_bRockUpdate;
		if (!m_strBackFolder.IsEmpty())
			m_strBackFolder.GetBufferSetLength(m_strBackFolder.GetLength()-1);
	}
	return m_bChange;
}

void CFilePage::OnBtnBackupFolder() 
{
	CFileDialog dlg(TRUE, _T("*"), m_strBackFolder, //NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
		{
		m_bChange = true;
		m_strBackFolder = dlg.GetPathName(); 
		}
}

void CFilePage::OnRaoBackupNo() 
{
	UpdateData(TRUE);
	m_edtBackupFolder.EnableWindow(m_optBackup==CViewEnv::BACKFILE_FOLDER);
}

void CFilePage::OnRaoBackupBak() 
{
	OnRaoBackupNo();
}

void CFilePage::OnRaoBackupTrash() 
{
	OnRaoBackupNo();
}

void CFilePage::OnRaoBackupFolder() 
{
	OnRaoBackupNo();
}

BOOL CFilePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_edtBackupFolder.EnableWindow(m_optBackup==CViewEnv::BACKFILE_FOLDER);

#ifdef FREE
	GetDlgItem(IDC_CHK_EDIT_MENU_ATOPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_EXTENTS)->EnableWindow(FALSE);
#endif

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CFilePage::OnChkRockupdate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}
