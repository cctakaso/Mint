#include "stdafx.h"
#include "OBJIDL.h"
#include "shlobj.h"

#include "util.h"
#include "chara.h"
#include "UserPath.h"
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
//#include "Colorlist.h"
bool vbEscHit=false;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std ;

/*
int WINAPI PalEntriesOnDevice(HDC hDC)
{
    int nColors;  // number of colors

    // Find out the number of colors on this device.
    
    nColors = (1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));

    ASSERT(nColors);
    return nColors;
}
HPALETTE WINAPI GetSystemPalette(void)
{
#define PALVERSION   0x300
    HDC hDC;                // handle to a DC
    static HPALETTE hPal = NULL;   // handle to a palette
    HANDLE hLogPal;         // handle to a logical palette
    LPLOGPALETTE lpLogPal;  // pointer to a logical palette
    int nColors;            // number of colors

    // Find out how many palette entries we want.

    hDC = GetDC(NULL);

    if (!hDC)
        return NULL;

    nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

    // Allocate room for the palette and lock it.

    hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors *
            sizeof(PALETTEENTRY));

    // if we didn't get a logical palette, return NULL

    if (!hLogPal)
        return NULL;

    // get a pointer to the logical palette

    lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

    // set some important fields

    lpLogPal->palVersion = PALVERSION;
    lpLogPal->palNumEntries = nColors;

    // Copy the current system palette into our logical palette

    GetSystemPaletteEntries(hDC, 0, nColors,
            (LPPALETTEENTRY)(lpLogPal->palPalEntry));

    // Go ahead and create the palette.  Once it's created,
    // we no longer need the LOGPALETTE, so free it.    

    hPal = CreatePalette(lpLogPal);

    // clean up

    GlobalUnlock(hLogPal);
    GlobalFree(hLogPal);
    ReleaseDC(NULL, hDC);

    return hPal;
}
*/
int roundleast(int n)
{
	int mod = n%10;
	n -= mod;
	if (mod >= 5)
		n += 10;
	else if (mod <= -5)
		n -= 10;
	return n;
}

void RoundRect(LPRECT r1)
{
	r1->left = roundleast(r1->left);
	r1->right = roundleast(r1->right);
	r1->top = roundleast(r1->top);
	r1->bottom = roundleast(r1->bottom);
}

void MulDivRect(LPRECT r1, const LPRECT r2, const int num, const int div)
{
	r1->left = MulDiv(r2->left, num, div);
	r1->top = MulDiv(r2->top, num, div);
	r1->right = MulDiv(r2->right, num, div);
	r1->bottom = MulDiv(r2->bottom, num, div);
}


#ifdef FREE
void StopFreeMsg()
{
	AfxMessageBox("ごめんなさい、この機能はシェアウェア版の機能です。フリーウェア版では実行できません。\n"\
		"by og. (t.ogisawa@webinter.net - http://hp.vector.co.jp/authors/VA052798)");

}
#endif

void GetMailAddressBlock(BYTE* &pch_first, BYTE *pch_point, BYTE *&pch_last)
{
	GetTextBlockFirst(pch_first, pch_point, IsMailAddressChar);
	GetTextBlockLast(pch_point+1, pch_last, IsMailAddressChar); //+1 '@'の次をスキャン
}
void GetUrlAddressLast(BYTE *pch, BYTE *&pch_last)
{
	GetTextBlockLast(pch, pch_last, IsUrlAddressChar);
}
void GetTextBlockFirst(BYTE* &pch, BYTE* pch_last, IsChar_Proc pProc)
{
	BYTE *pch_find = null;
	while(pch < pch_last) {
		BYTE ch=*pch;
		if (IsSJIS_1stByte(ch)) {
			pch_find = null;
//			pch += 2;
			pch ++;
		}else if ((*pProc)(ch)) {
			if (pch_find==null)
				pch_find = pch;
		}else{
			pch_find = null;
		}
		pch ++;
	}
	if (pch_find)
		pch = pch_find;
}
void GetTextBlockLast(BYTE *pch, BYTE *&pch_last, IsChar_Proc pProc)
{
	while(pch < pch_last) {
		BYTE ch=*pch;
		if (IsSJIS_1stByte(ch))
			break;
		else if (!(*pProc)(ch))
			break;
		pch ++;
	}
	pch_last = pch;
}

bool IsMailAddressChar(BYTE ch)
{
	const static char stcMailAddressOk[] = ".~_-%";
	return (GetFullType(ch) &  (ALP|NUM)) || strchr(stcMailAddressOk, ch)!=0;
}
bool IsUrlAddressChar(BYTE ch)
{
	const static char stcUrlAddressOk[] = ".~_@-%/=+#$&\\`{}[]?|";
	return (GetFullType(ch) &  (ALP|NUM)) || strchr(stcUrlAddressOk, ch)!=0;
}
bool IsDBCSTrailByte(const char *base, const char *p )
{
	int lbc = 0;	// lead byte count

	assert(base <= p);

	while ( p > base ) {
	if ( !IsSJIS_1stByte(*(--p)) )
		break;
	lbc++;
	}
	return (lbc & 1);
}
OBJOFF findStr(const string &str, const string &str_find, const OBJOFF off_first)
{
	return findSz(str, str_find.data(), off_first);
}
OBJOFF findSz(const string &str, const char *sz_find, const OBJOFF off_first)
{
	OBJOFF off = str.find(sz_find, off_first);
	if (off != string::npos && !IsDBCSTrailByte(str.data()+off_first, str.data()+off))
		return off;
	return string::npos;
}
/*
COLORREF Sz2Colorref(const char *sz)
{
	CColorSel colorsel;
	colorsel.Initialize(sz);
	return colorsel.GetColor();
}

COLORREF Str2Colorref(const string &str) 
{
	return Sz2Colorref(str.c_str());
}
*/	
/*
bool FAppendCpyStr2GData(HGLOBAL &hData, OBJSIZE &lenData, const string &str)
{
	if (str.size()>0) {
		HGLOBAL hDataNew = AppendSz2GData(hData, lenData, str.c_str(), str.size()+1); //+1 is for terminat null.
		if (hDataNew==null)
			return false;
		hData = hDataNew;
	}
	return true;
}
*/
int ScanHitCharOfString(const BYTE *sz, void *pv)
{
	assert(sz);
	assert(pv);
	char szT[3];
	memset(szT, 0, sizeof(szT));
	szT[0] = *sz;
	if (IsSJIS_1stByte(*sz))
		szT[1] = *(sz+1);
	OBJOFF off_find = ((string *)pv)->find(szT);
	if (off_find!=string::npos) {
		if (!IsDBCSTrailByte(((string *)pv)->data(), ((string *)pv)->data()+off_find)) {
			return szT[1]!=0 ? 2:1;
		}
	}
	return 0;
}
bool TrimLeftChar(string &str, const string *pstrRemove)
{
	return Trim(str, true, false, false, false, ScanHitCharOfString, (void*)pstrRemove);
}
bool IncLineHeaderNumStr(string &str)
{
	CLineHeader lh;
	lh.Initialize((BYTE*)str.data());
	return lh.GetNextLineHeader(str);
}

int ScanHitLineHeader(const BYTE *sz, void *pv)
{
	CLineHeader lh;
	lh.Initialize(sz);
	return lh.GetLength();
}

bool TrimLeftLineHeader(string &str)
{
	return Trim(str, true, false, false, false, ScanHitLineHeader, null);
}

bool Trim(string &str, bool bDbcsWhite)
{
	return Trim(str, true, true, true, bDbcsWhite);
}
bool TrimLeft(string &str, bool bDbcsWhite)
{
	return Trim(str, true, false, true, bDbcsWhite);
}
bool TrimRight(string &str, bool bDbcsWhite)
{
	return Trim(str, false, true, true, bDbcsWhite);
}

bool Trim(string &str, bool bLeft, bool bRightWhite, bool bWhiteDo, bool bDbcsWhite, ScanHitChar pProc, void *pv)
{
	bool bChange = false;
	if (str.size()>0) {
		OBJSIZE len_right=0;
		OBJOFF left_first=NPOS, left_last=NPOS;
		BYTE *pch = (BYTE*)str.data(), *pch_max = pch + str.size();
		BYTE *pch_white_head = pch;
		BYTE ch, ch2;
		int chtype, chtype2;
		int len_delete;
		chtype = GetChType(ch=*pch++);
		bool bKanj, bWhite=false, bHit=false;
		while(/*pch<pch_max*/ch) {
			ch2 = *pch++;
			chtype2= GetChType(ch2);
			if (bKanj=(chtype & SJ1 && chtype2 & SJ2)) {
				if (pProc)
					bHit =  (len_delete = (*pProc)(pch-2, pv)) >0;
				if (pProc || (bWhiteDo && bDbcsWhite))
					bWhite = bHit ? false:(ch == wchSpace1st && ch2 == wchSpace2nd);
			}else{
				if (pProc)
					bHit = (len_delete = (*pProc)(pch-2, pv)) >0;
				if (pProc || bWhiteDo)
					bWhite = bHit ? false:IsWhite_chtype(chtype);
			}
			if (bLeft && left_last==NPOS) {
				if ( ((bWhiteDo && pProc) && !(bWhite || bHit)) ||
					((bWhiteDo && !pProc) && !bWhite)) {
					left_last = pch - ((BYTE*)str.data()) -2;
					if (!bRightWhite)
						break;
				}else if (!bWhiteDo && pProc) {
					if (bHit) {
						if (left_first==NPOS) {
							left_first = pch - (BYTE*)str.data() -2;//削除start位置を覚えておく
							left_last = left_first + len_delete;
							if (!bRightWhite)
								break;
						}
					}else if (!bWhite) {
						return false;
					}
				}
			}
			if (bKanj) {
				ch = *pch++;
				chtype = GetChType(ch);
			}else{
				ch = ch2;
				chtype = chtype2;
			}
			if (bRightWhite && !bWhite) {
				pch_white_head = pch -1;
			}
		}
		if (bRightWhite) {
			if ((len_right = pch_max - pch_white_head)>0) {
				OBJOFF len = str.size();
				str.erase(len-len_right, len_right);
				//str.resize(len-len_right);
				if (str.size()==0)
					return true;
				bChange = true;
			}
		}
		if (bLeft /*&& left_last != NPOS*/) {
			if (left_last == NPOS)
				left_last = str.size();
			if (left_first==NPOS)
				left_first = 0;
			if (left_last>left_first) {
				//OBJOFF len = str.size();
				str.erase(left_first, left_last - left_first);
				//str.resize(len-(left_last - left_first));
				bChange = true;
			}
		}
	}
	return bChange;
}
///////////////////////////////////////////////////////////////////////////
// 行頭文字の挿入削除処理 (">")参照マークなど
// 複数行版
bool Trans_LineHead_block(string &str, const char *sz, const OBJSIZE len, bool bAppend, const char *szMark, const char *szMark2, const char *szMark3)
{
	assert(IsValidDataSz(sz));
	assert(len>0);

	bool brtn = false;
	const char *pszLine = sz, *pszMax = sz + len;
	OBJSIZE lenLine=0;
	for (const char *psz = sz; psz<pszMax && *psz!='\0'; psz++) {//ついでに'\0'のチェックも行う
		if (IsCRLF(*psz)) {
			if (lenLine>0) {
				str += Trans_LineHead_line(pszLine, lenLine, bAppend, szMark, szMark2, szMark3);
				lenLine = 0;
				brtn = true;
			}
			str += *psz;		//CRLFはそのまま追加
			pszLine = psz+1;	//次のライン先頭ポインタの用意
		}else
			lenLine++;
	}
	if (lenLine>0) {
		str += Trans_LineHead_line(pszLine, lenLine, bAppend, szMark, szMark2, szMark3);
		lenLine = 0;
		brtn = true;
	}
	return brtn;
}
///////////////////////////////////////////////////////////////////////////
// 行頭文字の挿入削除処理
// 単一行版
string Trans_LineHead_line(const char *sz, const OBJSIZE len, bool bAppend, const char *szMark, const char *szMark2, const char *szMark3)
{
	string str;
	
	if (bAppend) {
		assert(IsValidDataSz(szMark));
		if (len>0 && !IsCRLF(sz[0])) {
			str = szMark;
		}
		str.append(sz, len);
	}else{
		str.append(sz, len);
		if (!RemoveHeadStr(str, szMark)) {
			if (!RemoveHeadStr(str, szMark2))
				RemoveHeadStr(str, szMark3);
		}
	}
	return str;
}
string Trans_LineHead_string(const string &_str, bool bAppend, const char *szMark, const char *szMark2, const char *szMark3)
{
	string str;
	
	if (bAppend) {
		assert(IsValidDataSz(szMark));
		if (_str.size()>0 && !IsCRLF(_str[0])) {
			str = szMark;
		}
		str  += _str;
	}else{
		str = _str;
		if (!RemoveHeadStr(str, szMark)) {
			if (!RemoveHeadStr(str, szMark2))
				RemoveHeadStr(str, szMark3);
		}
	}
	return str;
}
bool RemoveHeadStr(string &str, const char *szMark)
{
	if (IsValidDataSz(szMark)) {
		int len = strlen(szMark);
		string strT;
		strT.append(str.data(),len);	//str.substr(0,len)
		if (str.size()>=len && strT==szMark) {
			//OBJOFF lenall = str.size();
			str.erase(0, len);
			//str.resize(lenall-len);
			return true;
		}
	}
	return false;
}

bool FAppendCpySz2GData(HGLOBAL &hData, OBJSIZE &lenData, const BYTE *sz, OBJSIZE len)
{
	if (len>0) {
		HGLOBAL hDataNew = AppendSz2GData(hData, lenData, sz, len); //+1 is for terminat null.
		if (hDataNew==null)
			return false;
		hData = hDataNew;
	}
	return true;
}

HGLOBAL AppendSz2GData(HGLOBAL hData, OBJSIZE &lenData, const BYTE *sz, OBJSIZE len)
{
	HGLOBAL hDataNew;
	//WIN95-::GlobalSize(hData) is not work just 16bit size!
	OBJOFF lenOld = (hData!=null ? lenData-1:0); //-1 最後の\0の位置からコピーする

	BYTE *p;
	if (hData)
		//hDataNew = ::GlobalReAlloc(hData, lenData=lenOld+len, GMEM_MOVEABLE | GMEM_ZEROINIT );
		hDataNew = ::GlobalReAlloc(hData, lenData+=len, GMEM_MOVEABLE /*| GMEM_ZEROINIT*/);
	else
		hDataNew = ::GlobalAlloc(GMEM_DDESHARE  /* | GMEM_MOVEABLE  | GMEM_ZEROINIT*/, lenData=len+1);

	if (hDataNew == null) {
		assert(false);
		return null;
	}
	p = (BYTE*)::GlobalLock(hDataNew);
	if (p==null) {
		assert(false);
		GlobalFree(hDataNew);
		return null;
	}
	memcpy((BYTE*)hDataNew+lenOld, sz, len);
	BYTE *p_max = &p[lenData-1];
	while((p=(BYTE*)memchr(p, 0, p_max-p))!=null && p<p_max) {
		*p++ = 0x20;
	}
	*p_max = '\0';
#ifdef _DEBUG
	assert(memchr((BYTE*)hDataNew, 0, lenData) == p_max);
#endif
	::GlobalUnlock(hDataNew);
	return hDataNew;
}

/*
HGLOBAL AppendSz2GData(HGLOBAL hData, const char *sz, OBJSIZE len)
{

#ifdef _DEBUG
	#define __REALLOC(p,nSize)	_realloc_dbg(p, nSize,_NORMAL_BLOCK, NULL, 0)
	#define __FREE(p)		_free_dbg(p, _NORMAL_BLOCK)
#else
	#define __REALLOC(p, nSize)	realloc(p, nSize)
	#define __FREE(p)		free(p)
#endif

	HGLOBAL hDataNew;
	OBJOFF lenOld = (hData!=null ? _msize(hData)-1:0); //-1 最後の\0の位置からコピーする

	hDataNew = __REALLOC(hData, len + lenOld);

	if (hDataNew == null)
		return null;
	memcpy((BYTE*)hDataNew+lenOld, sz, len);
	return hDataNew;
}
*/
void _assert(char *exp, char *szFile, int line) {
	char sz[512];
	sprintf(sz, "このエラーが現れた場合は、下の情報をメモしてメールお願いします。\n" \
		"%s [%d] line assert!", szFile, line);
#ifdef _DEBUG
	TRACE(sz);
	TRACE("\n");
	_asm {
		int 3
	};
#endif
	MessageBox(NULL, sz, "Mint Report", 0);
}

/*
unsigned long fileLegnth(int handle) const;
{
	unsigned long dwLen, dwCur;

	// Seek is a non const operation
	dwCur = pFile->Seek(0L, current);
	dwLen = pFile->SeekToEnd();
	VERIFY(dwCur == (DWORD)pFile->Seek(dwCur, begin));

	return dwLen;
}
*/
////////////////////////////////
////// public function
const static char stcInvalidFileNameChar[] = "\\/:.:*?\"<>|";
string Trans2ValidFileName(const char *sz)
{
	string str = sz;
	BYTE ch;
	for (int i=0; ch=(BYTE)str[i]; i++) {
		if (IsSJIS_1stByte(ch))
			i++;
		else if (strchr(stcInvalidFileNameChar, (char)ch)!=0) {
			str[i] = '_';
		}
	}
	return str;
}
string GetPartStringOfPath(string &strSrc)
{
	bool bDq=false;
	string str,strT;

	try
	{
	while(true)
		{
		//trim befor space.
		long i;
        for(i=0; i<strSrc.length() && *(strSrc.c_str()+i) == ' '; i++);
		if (i<strSrc.length() && *(strSrc.c_str()+i) == '\"') {
			bDq = true;
			i++;
		}
		//strT = strSrc.substr(i);//Mid(i);
		strT = "";
		strT.append(strSrc.data()+i);//Mid(i);
		strSrc = strT;

		int count = strSrc.find(bDq ? '\"':' ');
		if (count==-1)
			{
			str = strSrc;
			strSrc="";
			break;
			}
		else if (count==0) 
			{
			//strT = strSrc.substr(1); //Mid(1);
			strT = "";
			strT.append(strSrc.data()+1); //Mid(1);
			strSrc = strT;
			continue;
			}
		else
			{
			//str = strSrc.substr(0, count); //Left(count);
			str = "";
			str.append(strSrc.data(), count);
			//strT = strSrc.substr(count+1); //Mid(count+1);
			strT = "";
			strT.append(strSrc.data()+count+1);
			strSrc = strT;
			break;
			}
		}

#ifdef _MFC_VER
	}catch (exception* e) {
#else
	}catch (exception &e) {
#endif
		TraceException(e);
#ifdef _MFC_VER
		e->Delete();
#endif
		assert(false);
	}
	return str;
}

LPMALLOC pMalloc;    /* Gets the Shell's default allocator */
bool TrashFile(const char *szFileName, HWND hWnd)
{
	if (SHGetMalloc(&pMalloc) != NOERROR)
		return false;

 	SHFILEOPSTRUCT data;
		
	//親Windowをﾃﾞｽｸﾄｯﾌﾟに
	data.hwnd = hWnd;
	//'「削除」の指定
	data.wFunc = FO_DELETE;
	char *psz = new char[strlen(szFileName)+2];
	strcpy(psz, szFileName);
	psz[strlen(szFileName)+1] = '\0';
	data.pFrom = psz;
	//後で「元に戻」せる & 「ﾀﾞｲｱﾛｸﾞを非表示」
	data.fFlags = FOF_ALLOWUNDO | FOF_SILENT;

	//実行
	bool b = (SHFileOperation(&data)==0);
	
	delete [] psz;
	pMalloc->Release();

	return b;
}
bool IsExistFile_slowway(const char *szFileName)
{
   int fh;
   if( (fh = _open( szFileName, _O_RDONLY )) == -1 )
	   return false;
   _close( fh );
   return true;

}

string itostring(int i, int base)
{
	char sz[256];
	itoa(i, sz, base);
	return string(sz);
}

string ultostring(unsigned long l, int base)
{
	char sz[256];
	ultoa(l, sz, base);
	return string(sz);
}
/*
string 64tostring(__int64 dl, int base)
{
	char sz[256];
	_i64toa(dl, sz, base);
	return string(sz);
}
*/
bool GetFileStat(const char *szFileName, struct _stat &filestat)
{
//#ifdef WIN32
//   if (_stati64( szFileName, &filestat ) != 0)
//	   return false;
//#else
   if (_stat( szFileName, &filestat ) != 0)
	   return false;
//#endif
   return true;
}

bool IsExistFile(const char *szFileName)
{
	//DWORD dw = GetFileAttributes(szFileName);
	//TRACE("%s attrib=0x%dh\n", szFileName, dw);
   //return dw!=0xFFFFFFFF && !(dw & FILE_ATTRIBUTE_DIRECTORY);
   //HACK!
	//ファイルがなくともエラーにならない。
	//NT4.0では、0x32で返るので、FILE_ATTRIBUTE_DIRECTORYが立ってない場合
	//を条件に加える。
	//#define FILE_ATTRIBUTE_READONLY         0x00000001  
	//#define FILE_ATTRIBUTE_HIDDEN           0x00000002  
	//#define FILE_ATTRIBUTE_SYSTEM           0x00000004  
	//#define FILE_ATTRIBUTE_DIRECTORY        0x00000010  
	//#define FILE_ATTRIBUTE_ARCHIVE          0x00000020  
	//#define FILE_ATTRIBUTE_NORMAL           0x00000080  
	//#define FILE_ATTRIBUTE_TEMPORARY        0x00000100  
	//#define FILE_ATTRIBUTE_COMPRESSED       0x00000800  
	//#define FILE_ATTRIBUTE_OFFLINE          0x00001000  

   struct _stat filestat;
	if (!GetFileStat(szFileName, filestat))
		return false;

	//   if( result != 0 )
	//	perror( "ファイルの情報を取得できませんでした" );
	//else   {
	//		/* 統計情報の一部を表示 */
	//	printf( "ファイルの大きさ : %ld\n", filestat.st_size );
	//	printf( "ドライブ         : %c:\n", filestat.st_dev + 'A' );
	//	printf( "更新時刻         : %s", ctime( &filestat.st_atime ) );
	//}
   if (filestat.st_size>0)
	   return true;
   return IsExistFile_slowway(szFileName);
   //return result==0 && filestat.st_size>0*/; //Size==0でもファイルはある
}

string GetFileDir(const string &strSrc)
{
	//Win95 コンパイラエラー VC\INCLUDE\xstring(40) : fatal error C1001: 内部ｺﾝﾊﾟｲﾗ ｴﾗｰ
	//(ｺﾝﾊﾟｲﾗ ﾌｧｲﾙ 'msc1.cpp', 行番号 1188) change substr()->append();
	//string str = GetFileName(strSrc);
	//return strSrc.substr(0, strSrc.length()-str.length());
	string str;
	str.append(strSrc.data(), strSrc.length()-GetFileName(strSrc).length() -1);
	return str;
}
string GetFileDirDir(const string &strSrc)
{
	//Win95 コンパイラエラー VC\INCLUDE\xstring(40) : fatal error C1001: 内部ｺﾝﾊﾟｲﾗ ｴﾗｰ
	//(ｺﾝﾊﾟｲﾗ ﾌｧｲﾙ 'msc1.cpp', 行番号 1188) change substr()->append();
	//string str = GetFileName(strSrc);
	//return strSrc.substr(0, strSrc.length()-str.length());
	string str;
	str.append(strSrc.data(), strSrc.length()-GetFileName(strSrc).length());
	return str;
}
string GetFileExtent(const string &strSrc)
{
	string str;
	string::size_type c=0;
	string::size_type  cext = strSrc.find_last_of(".");
	
	if (cext==string::npos) {
		//str = strSrc;
	}else {
		c = cext;
		if (++c<strSrc.size()) {
			//str = strSrc.substr(c);
			str.append(strSrc.data()+c);
		}
	}
	_strupr((char*)str.data());
	return str;

}
string GetFileName(const string &strSrc)
{
	string str, strT;
	string::size_type c=0;
	string::size_type  cdir;
	string::size_type  cdrv = strSrc.find_last_of(SZ_DRV);

	strT = strSrc;
	while ((cdir=strT.find_last_of(SZ_DIR))!=string::npos &&
		IsDBCSTrailByte(strT.data(), strT.data()+cdir)) {
		if (cdir>1)
			strT.resize(cdir-1);
		else {
			cdir =  string::npos;
		}
	}

	if (cdir==string::npos && cdrv==string::npos) {
		str = strSrc;
		return str;
	}
	if (cdir!=string::npos)
		c = cdir;
	if (cdrv!=string::npos && cdrv>c)
		c = cdrv;

	if (++c<strSrc.size()) {
		//str = strSrc.substr(c);
		str.append(strSrc.data()+c);
	}
	return str;
}

//ストリングの次の部分を取得し、
//取得した部分を取り除いたストリングを作成しておく
//This codes is slow way , but this code is not so many called at this function.
string GetPartString(string &strSrc, char ch)
{
	string str,strT;
	try	{
		while(true) {
			int count = strSrc.find(ch);
			if (count==-1) {
				str = strSrc;
				strSrc="";
				break;
			} else if (count==0)  {
				//strT = strSrc.substr(1);//Mid(1);
				strT = "";
				strT.append(strSrc.data()+1);
				strSrc = strT;
				continue;
			}else {
				//str = strSrc.substr(0, count);//Left(count);			
				str = "";
				str.append(strSrc.data(), count);
				//strT = strSrc.substr(count+1);//Mid(count+1);
				strT = "";
				strT.append(strSrc.data()+count+1);
				strSrc = strT;
				break;
			}
		}
#ifdef _MFC_VER
	}catch (exception* e) {
#else
	}catch (exception &e) {
#endif
		TraceException(e);
#ifdef _MFC_VER
		e->Delete();
#endif
		assert(false);
	}
	return str;
}

void PartString2IntArray(string &strSrc, int *pInt, const int max, char ch)
{
	assert(pInt && max>0);
	string str;
	for (int i=0; i<max; i++) {
		str = GetPartString(strSrc, ch);
		*pInt++ = atoi(str.c_str());
	}
}
string IntArray2PartString(int *pInt, const int max, char ch)
{
	string strDes;
	assert(pInt && max>0);
	for (int i=0; i<max; i++) {
		if (i>0)
			strDes += ch;
		strDes += itostring(*pInt++);
	}
	return strDes;
}
int	FindStringArray(const CStringArray &aryStr, int first, const char *sz)
{
	if (first==-1)
		first = 0;
	for (int i = first; i<aryStr.GetSize(); i++) {
		if (aryStr[i] == sz)
			return i;
	}
	return -1; //not find
}

bool FCreateDirectory(const char * lpszPath)
{
	string strPath;
	bool bCreate=false;
	BYTE ch, chBef;
	while(true) {
		ch=*lpszPath++;
		if ((ch==CH_DIR && chBef!=CH_DRV) || (ch=='\0' && chBef!=CH_DIR)) {
			if (strPath.size()>0 && strPath!=SZ_NET_DIR) {
				#ifdef WINVER
					bCreate = (CreateDirectory(strPath.c_str(), NULL/*LPSECURITY_ATTRIBUTES lpSecurityAttributes*/)==TRUE);
				#else
					bCreate = (mkdir(strPath.c_str())==0);
				#endif
			}
		}
		if (ch=='\0')
			break;
		strPath += ch;
		if ((IsSJIS_1stByte(ch)) && *lpszPath) //漢字リードバイトチェック(つまりANSIまたはシフトJISのファイル名だとしている。)
			strPath += *lpszPath++;
		else
			chBef = ch;
	}
	return bCreate;
}
string GetModuleDirDir(HINSTANCE hInst)
{

/*	string str;
	char sz[1024];

	if (GetModuleFileName(hInst, sz, sizeof(sz))>0) {
		str = sz;
		str = GetFileDirDir(str);
	}
*/
	string str = UserPath::myDocuments().c_str();
	str +='\\';
#ifndef BUG_20121025_MYDOCUMENT_PLUS_MINT
	str += "Mint Editor\\";
#endif
	return str;
}
string GetCurrentPath(void)
{
	string str;
#ifdef _MSC_VER
	char *pszBuf = (char*)getcwd(NULL, 0);
	str = pszBuf;
	free(pszBuf);
	return str;
#else
	unsigned long dwRet1=__MAX_PATH,dwRet2;
	char *pszBuf=new char[__MAX_PATH];

	while(true) {
		dwRet2 = getcwd((char *)pszBuf, dwRet1);
		if (dwRet2 >= dwRet1)
			{
			delete [] pszBuf;
			dwRet1=dwRet2*2;
			pszBuf=new char[dwRet1];
			}
		else
			break;
	}
	str = pszBuf;
	delete [] pszBuf;
	if (dwRet2!=0 && str.size()>0)
		return str;
	str="";
	return str;
#endif
}

void ParseOption::Set(const char *sz)
{
	assert(sz && *sz!='\0');
	st = sz;
	bOption = (st.size()>0 && (st[0]=='-' || st[0]=='/'));

	int pos = st.find(':');
	
	stOption = "";
	if (bOption && st.length()>=2) {
		if (pos == string::npos) {
			//stOption = st.substr(1);
			stOption.append(st.data()+1);
		}else{
			//stOption = st.substr(1, pos - 1);
			stOption.append(st.data()+1, pos-1);
		}
	}
	
	stCol = "";
	if (pos != string::npos){
		//stCol = st.substr(pos);
		stCol.append(st.data()+pos);
	}
}

void ParseOptions::Set(const char *szOptions)
{
    DEQUE_OPTIONS::iterator pi;
	ParseOption opt;
	string stPart;
	string stOptions = szOptions;
	while((stPart = GetPartString(stOptions, ' ')).size()>0) {
		opt.Set(stPart.c_str());
		options.push_back(opt);
	}
}


void RemoveLastBackSlash(string *pcstr)
{
	int length = length=pcstr->length();
	string str;
	str.append(pcstr->data()+length-1, 1);

	if (length>0 && pcstr->substr(length-1, 1)=="\\" && !(length==3 && pcstr->substr(1,1)==":")) {
		*pcstr = pcstr->substr(0, length-1);
	}
}

#ifdef NOTUSE
BOOL ParseOptions(int argc, char* argv[])
{
	int nIndex;
	for (nIndex = 1; nIndex < argc; nIndex++)
	{
		// an option or a URL?

		if (*argv[nIndex] == '-' || *argv[nIndex] == '/')
		{
			if (argv[nIndex][1] == 'D' || argv[nIndex][1] == 'd')
				dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'L' || argv[nIndex][1] == 'l')
				dwAccessType = LOCAL_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'G' || argv[nIndex][1] == 'g')
				dwAccessType = GATEWAY_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'S' || argv[nIndex][1] == 's')
				bStripMode = true;
			else if (argv[nIndex][1] == 'P' || argv[nIndex][1] == 'p')
				bProgressMode = true;
			else if (argv[nIndex][1] == 'F' || argv[nIndex][1] == 'f')
				dwHttpRequestFlags |= INTERNET_FLAG_RELOAD;
			else
			{
				cerr << _T("Error: unrecognized option: ") << argv[nIndex] << endl;
				return false;
			}
		}
		else
		{
		// can't have too many URLs

		if (pszURL != NULL)
		{
			cerr << _T("Error: can only specify one URL!") << endl;
			return false;
		}
		else
			pszURL = argv[nIndex];
		}
	}

	return true;
}

#endif //#ifdef NOTUSE
