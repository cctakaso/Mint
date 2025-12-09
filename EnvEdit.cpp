// EnvEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "ViewEnv.h"
#include "EnvEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

extern CTheApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEnvEdit ダイアログ


CEnvEdit::CEnvEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvEdit)
	m_strName = _T("");
	m_strFileName = _T("");
	m_strExtents = _T("");
	//}}AFX_DATA_INIT
	m_pEnvTbl = null;
}


void CEnvEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvEdit)
	DDX_Text(pDX, IDC_EDT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDT_FILENAME, m_strFileName);
	DDX_CBString(pDX, IDC_CMB_EXTENTS, m_strExtents);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvEdit, CDialog)
	//{{AFX_MSG_MAP(CEnvEdit)
	ON_BN_CLICKED(IDC_BTN_REF, OnBtnRef)
	ON_EN_UPDATE(IDC_EDT_NAME, OnUpdateEdtName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvEdit メッセージ ハンドラ
bool CEnvEdit::UpdateEnv(bool bSetToEnv, CViewEnvTbl *pEnvTbl, int mode, string &strName, string &strFile, string &strExtents, const char *szName_base)
{
	assert(pEnvTbl);
	if (bSetToEnv) {
		if (m_bChange) {
			strName = m_strName.GetBuffer(0);
			strFile = m_strFileName.GetBuffer(0);
			strExtents = m_strExtents.GetBuffer(0);
		}
	}else{
		m_bChange = false;
		m_mode = mode;
		m_strName = strName.data();
		m_strName_base = szName_base;
		m_strFileName = strFile.data();
		m_strExtents = strExtents.data();
		m_strName_save = m_strName;
		m_strFileName_save = m_strFileName;
		m_strExtents_save = m_strExtents;
		m_pEnvTbl = pEnvTbl;
	}
	return m_bChange;
}

void CEnvEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	UpdateData(TRUE);

	if (m_strName.GetLength()==0){
		AfxMessageBox("設定名称を入力して下さい。");
		return;
	}
	if (m_strFileName.GetLength()==0){
		AfxMessageBox("ﾌｧｲﾙ名を入力して下さい。");
		return;
	}

	CViewEnvTbl::iterator iter;
	bool bExistFile;
	string strT;
	CViewEnv *pEnv;
	CViewEnv *pEnv_new;

	//拡張子だけ更新か？
	if(m_mode==mode_update && m_strName == m_strName_save &&
		m_strFileName == m_strFileName_save && m_strExtents != m_strExtents_save) {
		strT = m_strName;
		if ((iter=m_pEnvTbl->find(strT))!=m_pEnvTbl->end() && (*iter).second) {
			(*iter).second->m_strExtents = m_strExtents;
			goto OK;
		}
	}
	//設定ﾌｧｲﾙの既存チェック
	if (!CheckFile(m_strFileName, bExistFile)) {
		return;
	}
	
	if (bExistFile) {
		//設定名称の既存チェック
		if (!CheckName(m_strName))
			return;
		//メモリ上の設定
		//既存の設定ファイルを読みメモリへロード
		pEnv_new = new CViewEnv(m_strFileName, m_strExtents, 
						GetModuleDirDir(theApp.m_hInstance).data());
		pEnv_new->ReadWriteProfile(CProfile::__read);
		pEnv_new->SetProfile_Extents(m_strFileName, m_strExtents); //m_strExtentsが設定ファイルから読み込まれ上書きされたので
																	//指定されたものに戻す
		if (m_mode==mode_update) {
			strT = m_strName_save.GetBuffer(0);
			if ((iter=m_pEnvTbl->find(strT))!=m_pEnvTbl->end()) {
				//元の名前のエントリー削除
				m_pEnvTbl->erase(iter);
			}
		}
		strT = m_strName.GetBuffer(0);
		m_pEnvTbl->insert(CViewEnvTbl::value_type(strT, pEnv_new));
	}else if (m_mode==mode_new) {
		//新しい設定を作成する---->
		//設定名称の既存チェック
		if (!CheckName(m_strName))
			return;
		//ベースになるﾌｧｲﾙの更新
		if (!bExistFile && (pEnv=m_pEnvTbl->WriteOneProfile(m_strName_base, false))==null) {
			AfxMessageBox("基になる設定を最新状態に更新できませんでした。");
			return;
		}
		//ベースからコピーする
		string strFile = MakeProfileName(m_strFileName);
		if(!bExistFile && CopyFile(pEnv->m_profile.GetSzProfile(),
			strFile.c_str(), FALSE/*no check exist file*/)==0) {
			AfxMessageBox("基になる設定から新しい設定をコピーできませんでした。");
			return;
		}
		//メモリ上の設定
		//新しく設定を作って追加する
		CViewEnv *pEnv_new = pEnv->CopyNew();
		pEnv_new->SetProfile_Extents(m_strFileName, m_strExtents);
		strT = m_strName.GetBuffer(0);
		m_pEnvTbl->insert(CViewEnvTbl::value_type(strT, pEnv_new));
	}else if(m_mode==mode_update) {
		m_bChange = m_strName != m_strName_save ||
					m_strFileName != m_strFileName_save ||
					m_strExtents != m_strExtents_save;
		if (!m_bChange)
			return;
		m_bChange = false;

		if (m_strFileName != m_strFileName_save) {
			//ベースになるﾌｧｲﾙの更新
			if ((pEnv_new=m_pEnvTbl->WriteOneProfile(m_strName_save, false))==null) {
				AfxMessageBox("基になる設定を最新状態に更新できませんでした。");
				return;
			}
			//ファイルのリネーム
			string strFile_src = MakeProfileName(m_strFileName_save);
			string strFile_des = MakeProfileName(m_strFileName);
			if (!bExistFile && !MoveFileEx(strFile_src.c_str(), strFile_des.c_str(),
				MOVEFILE_COPY_ALLOWED)) {
				AfxMessageBox("設定ﾌｧｲﾙ名を更新できませんでした。");
				return;
			}
			//メモリ上の設定
			strT = m_strName_save.GetBuffer(0);
			if ((iter=m_pEnvTbl->find(strT))!=m_pEnvTbl->end()) {
				//元の名前のエントリー削除
				m_pEnvTbl->erase(iter);
			}
		}
		//メモリ上の設定

		//新たな名前で更新
		pEnv_new->SetProfile_Extents(m_strFileName, m_strExtents);
		if (m_strName != m_strName_save) {
			strT = m_strName.GetBuffer(0);
			m_pEnvTbl->insert(CViewEnvTbl::value_type(strT, pEnv_new));
		}
	}
OK:
	m_bChange = true;

	//他のﾐﾝﾄの為、新しい設定を保存しておく
	if ((pEnv_new=m_pEnvTbl->WriteOneProfile(m_strName))==null) {
		AfxMessageBox("該当設定を最新状態に更新できませんでした。");
		return;
	}

	CDialog::OnOK();
}
bool CEnvEdit::CheckName(const char* szName)
{
	CViewEnvTbl::iterator iter;
	string str = szName;
	if ((iter=m_pEnvTbl->find(str))!=m_pEnvTbl->end()) {
		//既存で同一名がある場合
		AfxMessageBox("既にこの設定名称は使われています。別の名前を指定して下さい。");
		GetDlgItem(IDC_EDT_NAME)->SetFocus();
		return false;
	}
	return true;
}
bool CEnvEdit::CheckFile(const char * szFile, bool &bExist)
{
	string strFileTemp = MakeProfileName(szFile);
	if (bExist=IsExistFile(strFileTemp.data())) {
//		if (AfxMessageBox("既にﾌｧｲﾙが存在します、このﾌｧｲﾙを設定ﾌｧｲﾙとして使用しますか？\n" \
//			"・この設定ﾌｧｲﾙをｱﾄﾞｲﾝする場合は (OK) ﾎﾞﾀﾝを押して下さい\n"\
//			"・既に設定ﾌｧｲﾙとして使用されて場合は二重使用できません", MB_OKCANCEL)==IDCANCEL) {
//			GetDlgItem(IDC_EDT_FILENAME)->SetFocus();
//			return false;
//		}
	}
	return true;
}
string CEnvEdit::MakeProfileName(const char *szFileName)
{
	string strFile = GetModuleDirDir(theApp.m_hInstance).data();
	strFile += szFileName;
	strFile += '.';
	strFile += ENVFILE_EXTENT;
	return strFile;
}

void CEnvEdit::OnBtnRef() 
{
	UpdateData(TRUE);
	string strFile = GetModuleDirDir(theApp.m_hInstance).data();
	strFile += "*.";
	strFile += ENVFILE_EXTENT;
	CFileDialog dlg(TRUE, _T("*.mnt"), strFile.c_str(), //NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("設定ﾌｧｲﾙ (*.mnt)|*.mnt||"));


	try{
	if (dlg.DoModal() == IDOK)	{
		m_strFileName = dlg.GetFileTitle(); 
		m_strName = m_strFileName;	//初期値として、名前も揃えておく
		CViewEnv *pEnv_new;
		pEnv_new = new CViewEnv(m_strFileName, m_strExtents, 
						GetModuleDirDir(theApp.m_hInstance).data());
		pEnv_new->ReadWriteProfile(CProfile::__read);
		m_strExtents = pEnv_new->m_strExtents.data();//初期値として、拡張子を入れる
		delete pEnv_new;
		UpdateData(FALSE);
	}
	}catch(CException *e) {
		e->ReportError();
		e->Delete();
		DWORD dw=CommDlgExtendedError();
		if (dw!=0) {
			char str[256];
			sprintf(str,"%0x",dw);
			AfxMessageBox(str);
		}
	}
}

void CEnvEdit::OnUpdateEdtName() 
{
	UpdateData(TRUE);
	m_strFileName = Trans2ValidFileName(m_strName).c_str();

	UpdateData(FALSE);
}
#endif //#ifdef SHAR
