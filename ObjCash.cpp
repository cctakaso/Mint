///////////////////////////////////////////////
//ObjCash.cpp

#include "StdAfx.h"
#include "ObjCash.h"
#include "ObjLay.h"
#include "TextCash.h"
#include "ViewEnv.h"
#include "chara.h"
#include "docTran.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
///////////////////////////////////////////////
//Global object
CObjTbl theObjTbl;

///////////////////////////////////////////////
//CObjCashFile class
CObjCashFile::CObjCashFile()
{
	m_bFormatAll = false;
	m_pViewEnv = null;
	m_countAttach = 0;

	m_nMode = 0;
	m_nProt = 0;
	m_size = 0;
	m_time = 0;
	m_pCashBuffer = null;
	m_sizeCash = 0;
	m_sizeBlock = 0;
	m_numBlock = 0;
	m_ocf = OCF_NO;
	m_bNowAccessFile = false;
	m_bTempFile = false;
}
CObjCashFile::~CObjCashFile()
{
	if (m_pCashBuffer)
		delete [] m_pCashBuffer;
}
void CObjCashFile::DeleteTempFile()
{
	if (m_bTempFile && m_file.is_open()) {
		m_file.clear();
		m_file.close();
		DeleteFile(m_filename.c_str());
	}
}

bool CObjCashFile::DoBackupFile(string &strNewFile,  bool bRename, const char *szOldFile)
{
	bool bRtn=false;
	//Win95 コンパイラエラー VC\INCLUDE\xstring(40) : fatal error C1001: 内部ｺﾝﾊﾟｲﾗ ｴﾗｰ
	//(ｺﾝﾊﾟｲﾗ ﾌｧｲﾙ 'msc1.cpp', 行番号 1188) change substr()->append();
	//string strPath = strNewFile.substr(0, strNewFile.size()-GetFileName(strNewFile).size());
	string strPath;
	strPath.append(strNewFile.c_str(), strNewFile.size()-GetFileName(strNewFile).size());
	if (!bRename)
		FCreateDirectory(strPath.c_str());
	try{
		//if (bRename) {
		DeleteFile(strNewFile.c_str());
		//}
		//if (m_file.is_open()) {
		//	m_file.clear();
		//	m_file.close();
		//}
		if(CopyFile(szOldFile ? szOldFile:m_filename.c_str(), strNewFile.c_str(), TRUE/*check exist file*/)==0) {
		//if (MoveFile(m_filename.c_str(), strNewFile.c_str())==0)
			//failer function!
#ifdef _DEBUG
			if (GetLastError()==ERROR_FILE_EXISTS)
				TRACE("File exists\n");
			else
				TRACE("%hd:\n", GetLastError());
#endif
		}else{
#define MY_FILE_ATTRIBUTE_SPECIAL (FILE_ATTRIBUTE_READONLY)		// | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)
			DWORD dw = GetFileAttributes(strNewFile.c_str());
			if (dw!=0xFFFFFFFF) {
				bRtn = true;
				if (dw & MY_FILE_ATTRIBUTE_SPECIAL) {
					if (!SetFileAttributes(strNewFile.c_str(), dw & ~MY_FILE_ATTRIBUTE_SPECIAL))
						bRtn = false;
				}
			}
		}
		return bRtn; //takaso 2014.05.18
	}
	catch(CFileException *e) {
		e->Delete();
	}
//	ResetFile();
	return bRtn;
}
bool CObjCashFile::ResetFile(const char *sz, bool bTempFile)
{
	if (m_file.is_open()) {
		m_file.clear();
		m_file.close();
	}
	if (IsValidDataSz(sz)) {
		m_bTempFile = bTempFile;
		return SetFile(sz, m_nMode, m_nProt);
	}
	return SetFile(null, m_nMode, m_nProt);
}
bool CObjCashFile::SetFile(const char *sz, int nMode, int nProt)
{
	if (IsValidDataSz(sz))
		m_filename = sz;
	m_nMode = nMode;
	m_nProt = nProt;
	m_file.clear();
	//takaso2008
	if (sz!=null){
		string str(sz);
		wstring wstr;
		widen(str,wstr);
		m_file.open( (wchar_t *)wstr.c_str(), nMode/*, nProt*/);
	}else{
		m_file.open( sz, nMode/*, nProt*/);
	}
	////const wchar_t *wsz = (const wchar_t *)sz;
	//wchar_t *wsz;
	//mbtowc( wsz, sz, MB_CUR_MAX );
	//m_file.open( wsz, nMode/*, nProt*/);
	//(m_bIO ? ios::in | ios::out : ios::in)  | ios::trunc, filebuf::sh_none);
	if (!m_file.is_open()) {
		if (nMode & ios::out) {
			nMode &= ~ios::out;
			m_file.open( sz, nMode/*, nProt*/);
			if (!m_file.is_open()) {
				AfxMessageBox("File open error!");
				return false;
			}
		}else{
			AfxMessageBox("File open error!");
			return false;
		}
	}
#ifdef WINVER
//	m_size =  _filelengthi64(m_file.fd()) + 1; //+1 is for VK_EOF
	m_file.seekg( 0, ios_base::end);
	m_size = m_file.tellg();
	m_size++;
	m_file.seekg( 0, ios_base::beg);
#else
	assert(false);
	m_size = filelength(m_file.fd());
#endif
	assert(m_size!=NPOS);
	return true;
}
CViewEnv *CObjCashFile::Attach_Env(CViewEnv *pViewEnv)
{
	CViewEnv *pOld = m_pViewEnv;
	if (pOld)
		pOld->AttachCount(-1);
	m_pViewEnv = pViewEnv;
	pViewEnv->AttachCount(1);
	return pOld;
}

void CObjCashFile::InitCashSizes()
{
	theObjTbl.InitCashSizes(m_size, m_sizeCash, m_sizeBlock);
	m_numBlock = m_size/m_sizeBlock;
}
OBJSIZE CObjCashFile::InitCashRead(BYTE *&pCashBuffer)
{
	return InitBufferRead(pCashBuffer, m_sizeCash);
}
OBJSIZE CObjCashFile::InitBlockRead(BYTE *&pCashBuffer)
{
	return InitBufferRead(pCashBuffer, m_sizeBlock);
}

OBJSIZE CObjCashFile::InitBufferRead(BYTE *&pCashBuffer, OBJSIZE size)
{
	assert(size>0);
	if (pCashBuffer)
		delete [] pCashBuffer;
	pCashBuffer = new BYTE[size + ABLS];
	memset(pCashBuffer, 0, size + ABLS);
	return size;
}

OBJSIZE CObjCashFile::ReadCash(BYTE *pCashBuffer, OBJOFF off)
{
	return ReadBuffer(pCashBuffer, off, m_sizeCash);
}
OBJSIZE CObjCashFile::ReadBlock(BYTE *pCashBuffer, OBJOFF off)
{
	return ReadBuffer(pCashBuffer, off, m_sizeBlock);
}

OBJSIZE CObjCashFile::ReadBuffer(BYTE *pCashBuffer, OBJOFF off, OBJSIZE size)
{
	OBJSIZE size_rtn;
#ifdef takaso2010
	CByteCash *spbyteCash = new CByteCash; 
#endif

	assert(!m_bNowAccessFile);
	while(m_bNowAccessFile);
	m_bNowAccessFile = true;
	m_file.clear();

	// 00000000000000|bbbbbbbbbbbbbb|rrrrrrrrrrr|aaaaa0000
	// ^pCashBuffer   ^pBef          +ABLS_BEF
	//                ^off_bef       ^off
	//                   (size_bef)  (size)       (ABLS_AFT)
	//
	// 012345678901234567890123456789012345678901234567890123456789
	// example:size=10, ABLS_BEF=3, ABLS_AFT=2
	// 

	// 1th off=0, off_bef=0, size_bef=0, m_file.gcount()=7, size_rtn=7-0+1=8 size_rtn+size_bef=8
	// 0123456[EOF]


	// 1st off=0 ,size=10
	// 0---------0--0-|
	// 2nd off=10, off_bef=10-3=7, size_bef=3, size_rtn=10
	//        ---0---------0-|
	// 3rd off=20, off_bef=20-3=17, size_bef=3, size_rtn=10
	//                  ---0---------0-|
	// 4th off=30, off_bef=30-3=27, size_bef=3, size_rtn=10
	//                            ---0---------0-|
	// 5th off=40, off_bef=40-3=37, size_bef=3, m_file.gcount()=8, size_rtn=8-3+1=6
	//                                      ---0----[EOF

	// 4th off=30, off_bef=30-3=27, size_bef=3, m_file.gcount()=9, size_rtn=9-3+1=7
	//                            ---0123456[EOF]

	// 4th off=30, off_bef=30-3=27, size_bef=3, size_rtn=10
	//                            ---0---------0\0
	// 5th off=40, off_bef=40-3=37, size_bef=3, m_file.gcount()=4, size_rtn=4-3+1=2
	//                                      ---0[EOF



	// 4th off=30, off_bef=30-3=27, size_bef=3, size_rtn=10
	//                            ---0---------\0
	// 5th off=40, off_bef=40-3=37, size_bef=3, m_file.gcount()=3, size_rtn=3-3+1=1
	//                                      ---[EOF


	OBJOFF off_bef = off;
	if (off_bef < ABLS_BEF)
		off_bef = 0;
	else if (off_bef != NPOS)
		off_bef -= ABLS_BEF;
	OBJSIZE size_bef = off - off_bef;
	BYTE *pBefBuff = pCashBuffer + ABLS_BEF - size_bef;

#ifdef takaso2010


#endif
	// takaso 2014.1/17 std::
	//if (off != NPOS)
	//if (off != NPOS && m_file.seekg(off_bef, ios::beg) == false)
	if (off != NPOS) {
		m_file.seekg(off_bef, ios::beg);
		//goto error;
	}
	m_file.seekg(off_bef, ios::beg);
	if (m_file.rdstate()!=ios::goodbit)
		goto error;
	m_file.read((char*)pBefBuff, size_bef + size + ABLS_AFT);
	if (m_file.gcount() >= (size_bef + size) ) {
		size_rtn =  size;
		if (m_file.gcount() < (size_bef + size + ABLS_AFT))
			pBefBuff[m_file.gcount()] = '\0';
	}else{
		//最後のブロック
		if (off < m_size /*&& m_file.gcount()>0 && pCashBuffer[m_file.gcount()-1]!=VK_EOF*/) {
			pBefBuff[m_file.gcount()] = VK_EOF;
			//takaso2010 this is bug!
			//size_rtn =  m_file.gcount() - size_bef + 1;
			pBefBuff[m_file.gcount() + 1] = '\0';
			size_rtn = m_file.gcount()- size_bef +1;
		}else{
			size_rtn =  0; //m_file.gcount() - size_bef;
			pBefBuff[0] = '\0';
		}

#if 0 //this is bug
		if (off < m_size /*&& m_file.gcount()>0 && pCashBuffer[m_file.gcount()-1]!=VK_EOF*/) {
			pBefBuff[m_file.gcount()] = VK_EOF;
			size_rtn =  m_file.gcount() - size_bef + 1;
		}else
			size_rtn =  0; //m_file.gcount() - size_bef;
		pBefBuff[size_bef + size_rtn] = '\0';
#endif 
	}
	m_bNowAccessFile = false;
	return size_rtn;
error:
	assert(false);
	m_bNowAccessFile = false;
	return NPOS;
}

///////////////////////////////////////////////
//CObjCashTbl class
CObjTbl::CObjTbl()
{
	m_parypDll = null;
}
CObjTbl::~CObjTbl()
{
	DeleteAll();
	if (m_parypDll)
		delete m_parypDll;
}
void CObjTbl::DeleteAll()
{
	for (size_type i=0; i<size(); i++)
		delete at(i);
	clear();//erase(begin(), end());
}
void CObjTbl::CloseBeforFiles(size_type idxMax)
{
	assert(size()>idxMax);
	for (size_type i=0; i<idxMax; i++)
		delete at(i);
	erase(begin(), begin()+idxMax);
}

void CObjTbl::CloseLayouts(int ctype, CObjLayTbl *pLay)
{
	//ctype == CLOSE_ALL: 全てクローズして新規を１つ追加
	//ctype == CLOSE_THIS: 現在表示中のものをクローズして１つ前のを表示
	//ctype == CLOSE_NOT_THIS: 現在表示中のものを残して他を全てクローズ
	iterator iter;
	switch(ctype) {
	case CLOSE_ALL:
		DeleteAll();
		break;
	case CLOSE_THIS:
		iter = find(begin(), end(), (CObjCashFile*)pLay);
		if (iter<end()) {
			delete (*iter);
			erase(iter);
		}
		iter = find(begin(), end(), pLay->m_pMainObj);
		if (iter<end()) {
			delete (*iter);
			erase(iter);
		}
		iter = find(begin(), end(), pLay->m_pEditObj);
		if (iter<end()) {
			delete (*iter);
			erase(iter);
		}
		break;
	case CLOSE_NOT_THIS:
		for (iter=begin(); iter<end(); iter++) {
			if ((*iter)!=pLay && 
				(*iter)!=pLay->m_pMainObj && 
				(*iter)!=pLay->m_pEditObj)
			delete (*iter);
		}
		clear();
		push_back(pLay);
		push_back(pLay->m_pMainObj);
		push_back(pLay->m_pEditObj);
		break;
	}
}

CObjLayTbl* CObjTbl::NewMainFile(const char *szfilename, int nMode, int nProt, LPSTR knjcode)
{
	string srcKnjcode, strknjfilename;
	bool isFilename = IsValidDataSz(szfilename);

	size_type sizeOld = size();
	CObjLayTbl *pObjLayTbl = null;
	CTextCashTbl *pTextCashTbl = null;
	CTextCashTbl *pTextCashTbl_Edit = null;
	//テキストキャッシュテーブルの生成
	if (!(pTextCashTbl = new CTextCashTbl))
		goto Err;
	if (!(pTextCashTbl_Edit = new CTextCashTbl))
		goto Err;

	//ファイル名指定があれば、ファイルをオープン
	if (isFilename) {
		//テンポラリファイルにオリジナルを移しておく。
		char szDesname[512];
		if (GetTempPath(sizeof(szDesname), szDesname)==0){
			goto Err;
		}
		if (knjcode)
			srcKnjcode = knjcode; //漢字コードの指定オープン
		//create directory
		FCreateDirectory(szDesname);
		GetTempFileName (szDesname, "MIT", 0, szDesname) ;	// ﾃﾝﾎﾟﾗﾘ･ﾌｧｲﾙを作成するﾄﾞﾗｲﾌﾞ名を取得し,
		DeleteFile(szDesname);
		bool ftran = DocTran(szfilename, szDesname, srcKnjcode, "S");
		if (!pTextCashTbl->SetFile(ftran ?
			szDesname : szfilename, nMode, nProt))
			goto Err;
	}

	//オブジェクトレイアウトテーブルの生成
	if (!(pObjLayTbl = new CObjLayTbl))
		goto Err;
	
	//ファイル名指定があれば、ファイルをオープン
	if (isFilename) {
		if (srcKnjcode.length()>0) {
			pObjLayTbl->setIsFirstSave(false); //漢字変換したテンポラリファイルが既にある。
			pObjLayTbl->setKnjcode(srcKnjcode.c_str()); //オリジナルファイルの漢字コードを設定
		}
	}else{
		if (AfxGetMainWnd()) {
			srcKnjcode = ((CMainFrame*)AfxGetMainWnd())->getShiteiKnjcode();
			if (srcKnjcode.length()>0) {
				pObjLayTbl->setKnjcode(srcKnjcode.c_str()); 
			}
		}
	}

	// オブジェクトレイアウトに情報を設定
	pObjLayTbl->Initialize(pTextCashTbl, pTextCashTbl_Edit, szfilename);

	pObjLayTbl->Attach_Env(theViewEnvTbl.GetViewEnv_byExtent(szfilename));
//	pTextCashTbl->Attach_Env(pObjLayTbl->GetEnv());
//	pTextCashTbl_Edit->Attach_Env(pObjLayTbl->GetEnv());

	//オブジェクトレイアウトテーブルの登録
	push_back(pObjLayTbl);
	push_back(pTextCashTbl);
	push_back(pTextCashTbl_Edit);

	//ファイル名指定がなくとも、最初のフォーマットを実行
	//if (IsValidDataSz(szfilename)) {
		pObjLayTbl->FormatObj(true, null);	
	//}
	//旨くいった場合は、オブジェクトレイアウトテーブルへのポインタを
	//返却する。
	return pObjLayTbl;
Err:
	if (pTextCashTbl)
		delete pTextCashTbl;
	if (pTextCashTbl_Edit)
		delete pTextCashTbl_Edit;
	return null;
}

bool CObjTbl::InitialDocTran(bool frest)
{
	//dll reset initailize
	if (frest && m_parypDll) {
		delete m_parypDll;
		m_parypDll = null;
	}

	if (m_parypDll==null) {
		m_parypDll = new CDLLArray;
		if (!m_parypDll->FInitialize()) {
			delete m_parypDll;
			m_parypDll = null;
			return false;
		}
	}
	return true;
}

bool CObjTbl::DocTran(const char *szSrcname, const char *szDesname, string &srcknjcode, LPSTR szdesknjcode)
{
	bool fchkknjcode = srcknjcode.length()==0;/*チェックしてSJISでロード!*/
	assert(szdesknjcode!=null);
	string desknjcode = szdesknjcode;
	transform(desknjcode.begin(), desknjcode.end(), desknjcode.begin(), ToLower());

	if (!fchkknjcode) { //指定してSJISでロード
		transform(srcknjcode.begin(), srcknjcode.end(), srcknjcode.begin(), ToUpper());
		//if (srcknjcode=="S" && desknjcode=="s")
		//	goto Err; //SJIS指定の場合、変換は必要ない
	}

	//dll reset initailize
	if (!InitialDocTran()) {
		return false;
	}


	// get kanji type
	CDLLDoc *pdll = (CDLLDoc *)m_parypDll->PDll(EXT_KFT);
	if (!pdll)
		return false;

	if (fchkknjcode) {
		if (pdll->DoOpenTran(szSrcname)) {
			srcknjcode = pdll->DoGetType();
			pdll->DoCloseTran();
			if (srcknjcode.length()==0) {
				//sjis or ascii
				goto Err; //変換は必要ない
			}
		}else{
			return false;
		}
	}



	if (pdll->DoTrans(szSrcname, szDesname,
		srcknjcode.c_str(), desknjcode.c_str() )!=UNKANJI_OK) {
		goto Err;
	}

	return true;
Err:
	if (fchkknjcode)
		srcknjcode.clear();
	return false;
}

void CObjTbl::InitCashSizes(OBJSIZE sizefile, OBJSIZE &sizeCash, OBJSIZE &sizeBlock)
{
	GlobalMemoryStatus(&m_mem_stat);
	/*
	typedef struct _MEMORYSTATUS { // mst 
    DWORD dwLength;        // sizeof(MEMORYSTATUS) 
    DWORD dwMemoryLoad;    // percent of memory in use 
    DWORD dwTotalPhys;     // bytes of physical memory 
    DWORD dwAvailPhys;     // free physical memory bytes 
    DWORD dwTotalPageFile; // bytes of paging file 
    DWORD dwAvailPageFile; // free bytes of paging file 
    DWORD dwTotalVirtual;  // user bytes of address space 
    DWORD dwAvailVirtual;  // free user bytes } MEMORYSTATUS, *LPMEMORYSTATUS;  
	}
	*/
	//sizefile/sizeBlock * sizeof(SELL) + sizeBlock*10 + sizeCash = m_mem_stat.dwAvailPhys/2
	//連立方程式をとくべき！

	OBJSIZE ps = m_mem_stat.dwTotalPhys/(sizefile == 0 ? 1:sizefile);
	ps = (ps==0 ? 1:(ps>64 ? 64:ps));
	sizeBlock = sizefile/(64/ps);

	sizeCash = m_mem_stat.dwAvailPhys/8;
	if (sizeCash > sizefile){
		if (sizefile < m_mem_stat.dwTotalPhys/256)
			sizeCash = m_mem_stat.dwTotalPhys/256;
		else
			sizeCash = sizefile;
	}else if (sizeCash < sizefile/50) {
		//memoryきつい
		sizeCash = m_mem_stat.dwTotalPhys/256;
		sizeBlock = sizeCash;
	}

	if (sizeBlock>sizeCash)
		sizeBlock = sizeCash;
	assert(sizeCash>0);
	assert(sizeBlock>0);
	TRACE("sizefile =0x%X\n", sizefile);
	TRACE("sizeCash =0x%X\n", sizeCash);
	TRACE("sizeBlock=0x%X\n", sizeBlock);
}

