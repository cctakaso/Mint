///////////////////////////////////////////////
//ObjLay.cpp

#include "StdAfx.h"
#include "ObjLay.h"
#include "ViewEnv.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////
//CObjLayTbl class
CObjLayTbl::CObjLayTbl()
{
#ifdef EDITMENU_OFLAY
	m_bEditMenu = true;
#endif
	m_ocf = OCF_LAYOUT;
	m_pMainObj = null;
	m_pEditObj = null;
	m_bFirstSave = true;
	m_phint = null;
}
CObjLayTbl::~CObjLayTbl()
{
	if (!m_bFirstSave) {
		//テンポラリファイルを削除しておく
		m_pMainObj->DeleteTempFile();
	}
	if (m_phint)
		delete m_phint;
}
CViewEnv *CObjLayTbl::Attach_Env(CViewEnv *pViewEnv)
{
	CViewEnv *pOld = CObjCashFile::Attach_Env(pViewEnv);
	if (!m_phint)
		m_phint = new CLineHint;
	//EditMenuを設定
	//m_bEditMenu = m_pMainObj->IsOpenFile() ? pViewEnv->m_bEditMenu_atOpen:false;
#ifdef EDITMENU_OFLAY
	m_bEditMenu = pViewEnv->m_bEditMenu_atOpen;
#endif
	m_phint->Initialize(pViewEnv);
	if (m_pMainObj)
		m_pMainObj->Attach_Env(pViewEnv);
	if (m_pEditObj)	
		m_pEditObj->Attach_Env(pViewEnv);
	return pOld;
}
void CObjLayTbl::RecalcCurSize()
{
	m_cursize = CUR_SIZE(GetEnv()->m_xdspMax, m_phint->size());
	ResetCash();
}

bool CObjLayTbl::FormatObj(bool bfirst, CLineHint *pHint, bool bfastReturn)
{
	if (!pHint)
		pHint = m_phint;
	bool bChange = false;
	if (bfirst || !m_bFormatAll) {
		if (bfirst || !m_pMainObj->IsFormatAll()) {
			if (m_pMainObj->IsOpenFile()){
				bChange = m_pMainObj->FormatObj(bfirst, pHint, bfastReturn);
				m_bFormatAll = m_pMainObj->IsFormatAll();
			}else{
				bChange = m_pEditObj->FormatObj(bfirst, pHint, bfastReturn);
				m_bFormatAll = m_pEditObj->IsFormatAll();
			}
			if (bChange) {
				RecalcCurSize(); //CUR_SIZE(m_pViewEnv->m_xdspMax, pHint->size());
			}
		}
	}
	return bChange;
}

int SelealizeData(const BYTE *pbytes, OBJOFF len, void *pv)
{
	try{
		((CArchive *)pv)->Write(pbytes, len);
		return 0;
	}
	catch (CFileException *e) {
		e->Delete();
		return -1;
	}
	return 0;
}


int CObjLayTbl::SaveData(CArchive& ar)
{
	assert(ar.IsStoring());
	
	//上書き保存の場合
	//bool bResetFile = ar.GetFile()->GetFilePath()==m_pMainObj->m_filename.c_str();

	string strBackFile;
	string strOldFile = (LPCSTR)ar.GetFile()->GetFilePath();

	if (IsExistFile(strOldFile.c_str())) {
		string strSrc = strOldFile.c_str();
		switch (GetEnv()->m_iBackFile) {
		case CViewEnv::BACKFILE_FOLDER:
			strBackFile = GetEnv()->m_strBackFolder;
			strBackFile += GetFileName(strSrc);
		case CViewEnv::BACKFILE_BAK:
			if (strBackFile.size()==0) {
				string strExtent = GetFileExtent(strSrc);
				int length = strSrc.size()-strExtent.size();
				//Win95 コンパイラエラー VC\INCLUDE\xstring(40) : fatal error C1001: 内部ｺﾝﾊﾟｲﾗ ｴﾗｰ
				//(ｺﾝﾊﾟｲﾗ ﾌｧｲﾙ 'msc1.cpp', 行番号 1188) change substr()->append();
				//strBackFile = strSrc.substr(0, length);
				strBackFile.append(strSrc.c_str(), length);
				if (strExtent.size()==0 && strBackFile[strBackFile.size()-1]!='.')
					strBackFile += ".";
				strBackFile += "BAK";
			}
			if (!(m_pMainObj->DoBackupFile(strBackFile, 
					(bool)(GetEnv()->m_iBackFile==CViewEnv::BACKFILE_BAK), strOldFile.c_str()))) {
				if (GetEnv()->m_iBackFile==CViewEnv::BACKFILE_NO)
					return -2;	//通常の保存エラーを表示する
				return -1; //バックアップ作成エラー
			}
			break;
		case CViewEnv::BACKFILE_TRASH:
			if (!TrashFile(strSrc.data(), AfxGetMainWnd()->GetSafeHwnd()))
				return -1; //バックアップ作成エラー
			break;
		//case CViewEnv::BACKFILE_NO:
		default:
			break;
		}
	}

	if (m_bFirstSave && m_pMainObj->m_filename.size()>0) {
		//テンポラリファイルにオリジナルを移しておく。
		char sz[512];
		if (GetTempPath(sizeof(sz), sz)==0){
			if (GetEnv()->m_iBackFile==CViewEnv::BACKFILE_NO)
				return -2;	//通常の保存エラーを表示する
			return -1; //バックアップ作成エラー
		}
		string strPathName(sz);
		GetTempFileName (strPathName.c_str(), "MIT", 0, sz) ;	// ﾃﾝﾎﾟﾗﾘ･ﾌｧｲﾙを作成するﾄﾞﾗｲﾌﾞ名を取得し,
		strBackFile = sz;
		if (!m_pMainObj->DoBackupFile(strBackFile, false)) {
			if (GetEnv()->m_iBackFile==CViewEnv::BACKFILE_NO)
				return -2;	//通常の保存エラーを表示する
			return -1; //バックアップ作成エラー
		}
		if (!m_pMainObj->ResetFile(strBackFile.c_str()))
			return -1;
		m_bFirstSave = false;
	}


	string knjcode;
	if (AfxGetMainWnd()) {
		knjcode = ((CMainFrame*)AfxGetMainWnd())->getShiteiKnjcode();
	}
	if (knjcode.length()==0) {
		knjcode = m_knjcode;
	}


	//takaso2010
	if (knjcode.length()!=0) {
		string strsave;
		char szDesname[512];
		if (GetTempPath(sizeof(szDesname), szDesname) > 0){
			CUR cur;

			//create directory
			FCreateDirectory(szDesname);

			GetTempFileName (szDesname, "MIT", 0, szDesname) ;	// ﾃﾝﾎﾟﾗﾘ･ﾌｧｲﾙを作成するﾄﾞﾗｲﾌﾞ名を取得し,

			DeleteFile(szDesname);

			CFile myFile(szDesname, CFile::modeCreate | CFile::modeWrite);
			CArchive myar(&myFile, CArchive::store);
			//CDocument mydoc;

			//myar.m_pDocument = &mydoc;

			if (m_phint->ScanDataBlock(cur, SelealizeData, &myar)>=0) {
				try{
					myar.Flush();
				}catch(CFileException *e) {
					e->Delete();
					return -2;//通常の保存エラーを表示する
				}
				//成功した！
				//現在のファイル名をコピー

				strsave = ar.GetFile()->GetFilePath();
				try{
				//myar.Close();
				myar.GetFile()->Close();
				//myar.Abort();

				//ar.Close();
				//ar.Abort();
				ar.GetFile()->Close();
				//ReleaseFile(pFile, FALSE);
				}catch(CFileException *e) {
					e->Delete();
//					return -2;//通常の保存エラーを表示する
				}
			}else{
				return -2;//通常の保存エラーを表示する
			}
		}else{
			return -2;//通常の保存エラーを表示する
		}
		string srcknjcode="S";

		bool f = theObjTbl.DocTran(szDesname, (LPCSTR)strsave.c_str(), srcknjcode, (LPSTR)knjcode.c_str());
				
		DeleteFile(szDesname);

		ar.GetFile()->Open(strsave.c_str(), CFile::shareDenyWrite | CFile::modeReadWrite | CFile::typeBinary );

		if (f) {
			//m_filename = (LPCSTR)ar.GetFile()->GetFilePath();
			m_filename = strsave;
			setKnjcode(knjcode.c_str()); 
			return 0;
		}
	}else{
		CUR cur;

		if (m_phint->ScanDataBlock(cur, SelealizeData, &ar)>=0) {
			try{
				ar.Flush();
			}catch(CFileException *e) {
				e->Delete();
				return -2;//通常の保存エラーを表示する
			}
			//成功した！
			//現在のファイル名をコピー
			m_filename = (LPCSTR)ar.GetFile()->GetFilePath();
			return 0;
		}
	}



	return -2;//通常の保存エラーを表示する
}
