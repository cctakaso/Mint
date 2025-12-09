/*    PortTool v2.2     DOCTRAN.CPP          */

#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <exception>
#include <io.h>
#include <iostream>
#include <fstream>
#include <locale.h>

#include "chara.h"
#include "util.h"
#include "mainfrm.h"
#include "doctran.h"
#include "CDxdoc2txt.h"

using namespace std ;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef takaso2010
#endif /* takaso2010 */

//extern CFindOption *vpFindOption;
//extern CFindList *vpFindList;		// 検索結果ﾘｽﾄへのﾎﾟｲﾝﾀ

UINT WM_ARCEXTRACT_MSG=NULL;

char *szDllFileName[] = {
	"UNKANJ.DLL",
	};

char *szArcFuncLst[][14] = {
//	"UNKANJ32.DLL",
	{"ccGetVersion",
	"ccSetBackGroundMode",
	"ccSetCursorMode",
	"ccCommand",
	"ccOpen",
	"ccRead",
	"ccClose",
	NULL,
	NULL,
	"ccSetOwnerWindow",
	NULL, //"ccClearOwnerWindow",
	"ccGetRunning",
	"ccGetType",
	"ccIsEOF"
	},
};
char *szArcCmdLst[] = {
	"t -",
	};

int iSupportVersion[] = {
	UNKANJ_VERSION,
	};

/*************************************************************************
*************************************************************************/
void CDLL::FreeLib()
{
	if (m_hDll) {
		FreeLibrary (m_hDll);
	}
	m_hDll = NULL;
}

bool CDLL::FInitialize(EXT_TYPE ext/*=EXT_NON*/, bool fActive/*=TRUE*/)
{
	// ｄｌｌタイプセット
	if (ext != EXT_NON)
		m_ext =ext;
	assert(m_ext!=EXT_NON);



	// ＡｒｃＤＬＬのリ・ロード
	if (!fActive)
		{
//		if (m_hDll)
//			{
//			FreeLibrary (m_hDll);
//			m_hDll = NULL;
//			}
		return FALSE;	//アクティブでなくする
		}

	if (!m_hDll) {
		if ((m_hDll = LoadLibrary (szDllFileName[m_ext]))==NULL ){
#ifdef WIN32
#ifdef _DEBUG
			DWORD dw = GetLastError();
#endif
#endif
			return FALSE;
		}
	}
	return TRUE;
}

bool CDLL::FActiveUpdate()
{
	if (!FActive())
		return FInitialize();
	return TRUE;
}

//ソースファイルをセットする
bool CDLL::FSetSrcFile(const char *szSrcFile)
{
	if (!FActive())
		return FALSE;
	assert(m_ext!=EXT_NON && m_hDll!=NULL);
	assert(szSrcFile);
//	m_dFileSrc = szSrcFile;
//	if (!IsExistFile(m_dFileSrc.c_str()))	//存在するか？
//		return FALSE;
	return TRUE;
}

/*************************************************************************
*************************************************************************/
CDLLDoc::CDLLDoc()
{
	m_handle=NULL;
}
CDLLDoc::~CDLLDoc()
{
	if (m_handle) {
		DoCloseTran();
	}
}
bool CDLLDoc::FInitialize(EXT_TYPE ext/*=EXT_NON*/, bool fActive/*=TRUE*/)
{
	if (CDLL::FInitialize(ext, fActive) == FALSE)
		return FALSE;

	if (WM_ARCEXTRACT_MSG==NULL)
		{
		//WM_ARCEXTRACT_MSGの登録
		WM_ARCEXTRACT_MSG=::RegisterWindowMessage(WM_ARCEXTRACT_TXT);
		if (WM_ARCEXTRACT_MSG==0)
			return FALSE;
		}
	// チェックバージョン＆サポートファンクション
	GetVersion = (DGetVersion)GetProcAddress (m_hDll, szArcFuncLst[m_ext][0]);
	SetBackGroundMode = (DSetBackGroundMode)GetProcAddress (m_hDll, szArcFuncLst[m_ext][1]);
	ArcCmd = (DCommand)GetProcAddress (m_hDll, szArcFuncLst[m_ext][3]);
	SetOwnerWindow	= (DSetOwnerWindow)GetProcAddress(m_hDll, szArcFuncLst[m_ext][9]);
	OpenTran	= (DOpen)GetProcAddress(m_hDll, szArcFuncLst[m_ext][4]);
	ReadTran	= (DRead)GetProcAddress(m_hDll, szArcFuncLst[m_ext][5]);
	CloseTran	= (DClose)GetProcAddress(m_hDll, szArcFuncLst[m_ext][6]);
	//ClearOwnerWindow = (DClearOwnerWindow)GetProcAddress(m_hDll, szArcFuncLst[m_ext][10]);
	GetRunning	= (DGetRunning)GetProcAddress(m_hDll, szArcFuncLst[m_ext][11]);
	GetType = (DGetType)GetProcAddress(m_hDll, szArcFuncLst[m_ext][12]);
	IsEOFTran = (DIsEOF)GetProcAddress(m_hDll, szArcFuncLst[m_ext][13]);

	if (!(GetVersion && ArcCmd && OpenTran && ReadTran && CloseTran &&
		SetBackGroundMode && GetRunning && SetOwnerWindow &&
		GetType && IsEOFTran))
		goto FREE_LIB;
	if ((GetVersion)()<iSupportVersion[m_ext])
		goto FREE_LIB;
	// 解凍されたパス付きファイル名のイニシャルエンプティー
//	(SetOwnerWindow)(AfxGetMainWnd()->GetSafeHwnd());
	return TRUE;
FREE_LIB:
	FreeLib();
	return FALSE;
}

int CDLLDoc::DoTrans(const char *szSrcFilename, const char *szDesFilename,
		const char *szSrcCode, const char *szDesCode)
{
	int iRtn;
	char sz[256];

	if (!FActive())
		return -1;	//NG!!!
	

	//コマンドラインの作成
	string cstrCmd;
	cstrCmd = szArcCmdLst[m_ext];
	cstrCmd += szSrcCode;
	cstrCmd += SP;
	cstrCmd += '-';
	cstrCmd += szDesCode;

	cstrCmd += SP;

	// ソースファイル
	cstrCmd += '\"';
	cstrCmd += szSrcFilename;
	cstrCmd += '\"';
	cstrCmd += SP;
	// ディスティネーションファイルの指定
	cstrCmd += '\"';
	cstrCmd += szDesFilename;
	cstrCmd += '\"';

	VERIFY((SetBackGroundMode)(FALSE));//タスクを占有

	TRACE(cstrCmd.c_str());
	TRACE("\r\n");
	iRtn = (ArcCmd)(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), cstrCmd.c_str(), sz, sizeof(sz));

	if (iRtn==0)
		{
		return iRtn;
		}
	return -1;
}


bool CDLLDoc::DoOpenTran(const char *szSrcFile)
{
	char sz[256];
	int rtn;


	if (!FActive())
		return false;	//NG!!!

	if (m_handle)
		DoCloseTran();

	//ステータスバーの表示
//	string cstr;
//	string cstrMsg;
//	cstrMsg = m_dFileSrc.GetName().c_str();
//	cstrMsg.MakeLower();
//	cstrMsg += SP;
//	cstr.LoadString(IDS_KFT_TRANS);
//	cstrMsg += cstr;
//	cstrMsg += "...";

//	((CMainFrame *)AfxGetMainWnd())->SetPaneText(cstrMsg.c_str());

	//コマンドラインの作成
	string cstrCmd;
	cstrCmd = szArcCmdLst[m_ext];
	cstrCmd += 's';

	cstrCmd += SP;
	// アーカイブファイル
	cstrCmd += '\"';
	cstrCmd += szSrcFile;
	cstrCmd += '\"';

	// 解凍されたパス付きファイル名のイニシャルエンプティー
//	(ClearOwnerWindow)();
//	if (!(SetOwnerWindow)(AfxGetMainWnd()->GetSafeHwnd()))
//		goto ERR;
	VERIFY((SetBackGroundMode)(TRUE));//タスクを占有しない
//	if (!WaitEndRunning())
//		return -1;			//User Abort!
	TRACE(cstrCmd.c_str());
	TRACE("\r\n");
	m_handle = (OpenTran)(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), cstrCmd.c_str(), sz, sizeof(sz), &rtn);
	if (m_handle!=NULL)
		return TRUE;
	return FALSE;
}

unsigned int CDLLDoc::DoReadTran(void *lpBuf, UINT nCount, int *pRtn)
{
	if (!FActive())
		return -1;	//NG!!!
	assert(m_handle);
	return (ReadTran)(m_handle, lpBuf, nCount, pRtn);
}

void CDLLDoc::DoCloseTran()
{

	if (!FActive())
		return;	//NG!!!
	assert(m_handle);
	(CloseTran)(m_handle);
	m_handle = NULL;
}

string CDLLDoc::DoGetType()
{
	string str;
	if (!FActive())
		return str;	//NG!!!

	assert(m_handle);
	int type =  (GetType)(m_handle);
	switch(type) {
		case kt_jis:
			str = "J";
			break;
		case kt_euc:
			str = "E";
			break;
		case kt_utf8:
			str = "U";
			break;
		default:
			break; //shift-JIS, ANSIは、変換不要
	}
	return str;
}

bool CDLLDoc::DoIsEOFTran()
{
	if (!FActive())
		return false;	//NG!!!
	assert(m_handle);

	return (bool)(IsEOFTran)(m_handle);
}
/*************************************************************************
*************************************************************************/

/*************************************************************************
*************************************************************************/

CDLLArray::~CDLLArray()
{
	for (int i=0; i<GetSize(); i++)
		delete ((CDLL *)GetAt(i));
}

bool CDLLArray::FInitialize()
{
	// アーカイブＤＬＬクラスの作成
	CDLL *pDll;

	for (int i=(int)EXT_MIN; i<EXT_MAX; i++)
		{
		if (i>=GetSize())
			{
			pDll = new CDLLDoc;
			if (!pDll)
				return FALSE;
			Add(pDll);
			if (pDll)
				pDll->FInitialize((EXT_TYPE)i);
			}
		}
	return TRUE;
}


CDLL *CDLLArray::PDll(EXT_TYPE ext)
{
	if ((int)ext>=GetSize())
		return NULL;
	return (CDLL *)GetAt((int)ext);
}

bool CDLLArray::FActiveAndCheck(EXT_TYPE ext)
{
	if ((int)ext>=GetSize())
		return FALSE;
	return ((CDLL *)GetAt((int)ext))->FActive();
}

