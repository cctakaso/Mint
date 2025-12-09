#include "stdafx.h"	//this is WINVER & _MFC_VER
#include "chara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char stchttp[] = "http://";
const char stcCLangComment[] = "//";
/*
 * Character classifier for KANJI.
 */
unsigned short  vchTypeTbl[0x100] = {
//   0,	 1,	 2,	 3,	 4,	 5,	 6,	 7,
//   8,	 9,	 A,	 B,	 C,	 D,	 E,	 E,
   /*000 0x00ÇANSÇ∆ÇµÇƒÇ®Ç≠*/
   CTR,   000,   000,   000,   000,   000,   000,   000,	/* 00		   */
   000,   WHT,   CTR,   000,   000,   CTR,   000,   000,	/* 08		   */
   000,   000,   000,   000,   000,   000,   000,   000,	/* 10		   */
   000,   000,   CTR,   000,   000,   000,   000,   000,	/* 18		   */
   WHT,   SYM,   SYM,   ALP,   SYM,   SYM,   SYM,   SYM,	/* 20  !"#$%&'  */	//#Ç±ŸÃßÕﬁØƒÇ∆ìØÇ∂àµÇ¢Ç…Ç∑ÇÈ for C/C++
   SYM,   SYM,   SYM,   SYM,   SYM,   SYM,   SYM,   SYM,	/* 28 ()*,+-./  */
   NUM,   NUM,   NUM,   NUM,   NUM,   NUM,   NUM,   NUM,	/* 30 01234567  */
   NUM,   NUM,   SYM,   SYM,   SYM,   SYM,   SYM,   SYM,	/* 38 89:;<=>?  */
   SYJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 40 @ABCDEFG  */
   ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 48 HIJKLMNO  */
   ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 50 PQRSTUVW  */
   ALJ2,  ALJ2,  ALJ2,  SYJ2,  SYJ2,  SYJ2,  SYJ2,  ALJ2,   /* 58 XYZ[\]^_  */ //_Ç±ŸÃßÕﬁØƒÇ∆ìØÇ∂àµÇ¢Ç…Ç∑ÇÈ for C/C++
   SYJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 60 `abcdefg  */
   ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 68 hijklmno  */
   ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,  ALJ2,   /* 70 pqrstuvw  */
   ALJ2,  ALJ2,  ALJ2,  SYJ2,  SYJ2,  SYJ2,  SYJ2,  SYM,	/* 78 xyz{|}~   */
   SJ2,   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   80 .. 87   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   88 .. 8F   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   90 .. 97   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   98 .. 9F   */
   SJ2,   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   A0 .. A7   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   A8 .. AF   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   B0 .. B7   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   B8 .. BF   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   C0 .. C7   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   C8 .. CF   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   D0 .. D7   */
   SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,  SJ2H,   /*   D8 .. DF   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   E0 .. E7   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   E8 .. EF   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  SJ12,   /*   F0 .. F7   */
   SJ12,  SJ12,  SJ12,  SJ12,  SJ12,  000,   000,   000,	/*   F8 .. FF   */
};
#define _JPN

#ifdef _JPN
/* Japanese specific character definitions */
//#define wchSpace				0x8140->move to header!
#define wchSpaceStr				0x4081	// for search logic!
#define wchPlus					0x817b
#define wchBoten				0x815b
#define wchMinus				0x817c
#define wchPercent				0x8193
#define wchNum0					0x824f
#define wchNum9					0x8258
#define wchUppA					0x8260
#define wchUppZ					0x8279
#define wchLowA					0x8281
#define wchLowZ					0x829a
#define wchKanjiFirst			0x889f
#define wchHiraganaFirst		0x829f
#define wchHiraganaLast			0x82f1
#define wchKatakanaFirst		0x8340
#define wchKatakanaLast			0x8396
#define chKatakanaFirst			0xa1
#define chKatakanaLast			0xdf
#define chPureKatakanaFirst		0xa6
#define chPureKatakanaLast		0xdd
#define chDakuten				0xde
#define wchDakuten				0x814a
#define chHandakuten			0xdf
#define wchHandakuten			0x814b

/* used in the conversion */
#define chConvSbcsFirst			0x20
#define chConvSbcsLast			0xdf
#define wchConvDbcsFirst		0x8140
#define wchConvDbcsLast			0x8396

/* substitutes for illegal characters, returned from FetchCp */
#define chBadSNG				'*'
#define chBadDB1				0x81
#define chBadDB2				0x96

/* used in the sentence selection */
#define	chJpnPeriod				0xa1
#define	wchPeriodEqv			0x8142
#define	wchPeriod				0x8144
#define	wchQuestion				0x8148
#define	wchExclamation			0x8149

//#define wchSpace1st				HighByte(wchSpace) //->move to header!
//#define wchSpace2nd				LowByte(wchSpace)
#endif //_JPN

#ifdef _KOREA
/* Korea specific character definitions */
#define wchSpace					0xa1a1
#define wchPlus					0xa3ab
#define wchMinus					0xa3ad
#define wchPercent				0xa3a5
#define wchNum0					0xa3b0
#define wchNum9					0xa3b9
#define wchUppA					0xa3c1
#define wchUppZ					0xa3da
#define wchLowA					0xa3e1
#define wchLowZ					0xa3fa

/* substitutes for illegal characters, returned from FetchCp */
#define chBadSNG				'*'
#define chBadDB1				0xa3		// DB asterisk 1st byte
#define chBadDB2				0xaa		// DB asterisk 2nd byte

#define chBeginSB				0x01
#define chEndSB					0x7f
#define chBeginDB				0xa1
#define chEndDB					0xfe
#define wTildeDBl				0xa1ad		// double byte code of '~'
#define chSpaceSB				0x20
#define chTild					0x7e

/* used in the sentence selection */
#define	wchPeriod				0xa3ae
#define	wchQuestion				0xa3bf
#define	wchExclamation			0xa3a1

#define	wchBgnNumD				0xa3b0
#define	wchEndNumD				0xa3b9
#define	wchBgnAlph1D			0xa3c1
#define	wchEndAlph1D			0xa3da
#define	wchBgnAlph2D			0xa3e1
#define	wchEndAlph2D			0xa3fa
#define	wchBgnHangeul			0xb0a1
#define	wchEndHangeul			0xc8fe
#define	wchBgnHanja				0xcaa1
#define	wchEndHanja				0xfdfe
#define	wchBgnJaso				0xa4a1
#define	wchEndJaso				0xa4d3
#define	wchBgnPunct1D			0xa3a1
#define	wchEndPunct1D			0xa3af
#define	wchBgnPunct2D			0xa3ba
#define	wchEndPunct2D			0xa3c0
#define	wchBgnPunct3D			0xa3db
#define	wchEndPunct3D			0xa3e0
#define	wchBgnPunct4D			0xa3fb
#define	wchEndPunct4D			0xa3fe
#define	wchBgnPunct5D			0xa1b2
#define	wchEndPunct5D			0xa1bd


#define wchSpace1st				HighByte(wchSpace)
#define wchSpace2nd				LowByte(wchSpace)

#endif //_KOREA

#ifdef _TAIWAN
/* Taiwan specific character definitions */

#define wchSpace				0xa140
#define wchPlus					0xa1cf
#define wchMinus				0xa1d0
#define wchPercent				0xa1c1
#define wchNum0					0xa2af
#define wchNum9					0xa2b8

#define wchUppA					0xa2cf
#define wchUppZ					0xa2e8
#define wchUppV					0xa2e4
#define wchUppW					0xa2e5

#define wchLowA					0xa2e9
#define wchLowV					0xa2fe
#define wchLowW					0xa340		// note a gap at 0xa2ff
#define wchLowZ					0xa343

#define chConvSbcsFirst			0x20
#define chConvSbcsLast			0xfe

/* substitutes for illegal characters, returned from FetchCp */
#define chBadSNG				'*'
#define chBadDB1				0xa1		// DB asterisk 1st byte
#define chBadDB2				0xaf		// DB asterisk 2nd byte

/* used in the sentence selection */
#define	wchPeriodEqv			0xa143
#define	wchPeriod				0xa144
#define	wchQuestion				0xa148
#define	wchExclamation			0xa149

#define wchSpace1st				HighByte(wchSpace)
#define wchSpace2nd				LowByte(wchSpace)

#endif //_TAIWAN

#if defined(_DEBUG) || defined(_VERIFY)


bool IsSbc ( WORD wch ) 
{
		wch = LowByte ( wch ) ;
		return !IsSJIS_1stByte((BYTE)wch);
}
bool IsValidDbc ( WORD wch )
{
	BYTE bHi = (BYTE) HighByte ( wch ) ;
	BYTE bLo = (BYTE) LowByte  ( wch ) ;

	if ( bHi )
		return ( IsSJIS_1stByte ( bHi ) && IsSJIS_2ndByte ( bLo ) ) ;
	else
		return ( IsSbc ( bLo ) ) ;
}
#endif //defined(_DEBUG) || defined(_VERIFY)

//takaso2008
void widen(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

bool IsUpper(WORD wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (WORD)(wch - wchUppA) <= (wchUppZ - wchUppA) || (WORD)(wch - 'A') <= ('Z' - 'A');
#endif
#ifdef _TAIWAN
	return (WORD)(wch - wchUppA) <= (wchUppZ - wchUppA) || (WORD)(wch - 'A') <= ('Z' - 'A');
#endif
#ifdef _KOREA
	return ((WORD)(wch - wchUppA) <= (wchUppZ - wchUppA) || (WORD)(wch - 'A') <= ('Z' - 'A'));
#endif
}

bool IsDbcUpper(WORD wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (WORD)(wch - wchUppA) <= (wchUppZ - wchUppA);
#endif
#ifdef _TAIWAN
	return (WORD)(wch - wchUppA) <= (wchUppZ - wchUppA);
#endif
#ifdef _KOREA 
		return ((WORD)(wch - wchUppA) <= (wchUppZ - wchUppA));
#endif
}

bool IsLower(WORD wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (WORD)(wch - wchLowA) <= (wchLowZ - wchLowA) || (WORD)(wch - 'a') <= ('z' - 'a');
#endif
#ifdef _TAIWAN
	return  (wchLowA <= wch && wch  <= wchLowV)
		||(wchLowW <= wch && wch  <= wchLowZ) ||	('a' <= wch && wch  <= 'z');
#endif
#ifdef _KOREA
	return ((WORD)(wch - wchLowA) <= (wchLowZ - wchLowA) || (WORD)(wch - 'a') <= ('z' - 'a'));
#endif
}

bool IsDbcLower(WORD wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (WORD)(wch - wchLowA) <= (wchLowZ - wchLowA);
#endif
#ifdef _TAIWAN
	return  (wchLowA <= wch && wch <= wchLowV) || (wchLowW <= wch && wch <= wchLowZ);
#endif
#ifdef _KOREA
	return ((WORD)(wch - wchLowA) <= (wchLowZ - wchLowA));
#endif
}

bool IsDigit(unsigned short wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0) || (unsigned short)(wch - '0') <= ('9' - '0');
#endif
#ifdef _TAIWAN
	return (unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0) || (unsigned short)(wch - '0') <= ('9' - '0');
#endif
#ifdef _KOREA
	return ((unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0) || (unsigned short)(wch - '0') <= ('9' - '0'));
#endif
}

bool IsDbcDigit(unsigned short wch)
{
	assert(IsValidDbc(wch));
#ifdef _JPN
	return (unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0);
#endif
#ifdef _TAIWAN
	return (unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0);
#endif
#ifdef _KOREA
	return ((unsigned short)(wch - wchNum0) <= (wchNum9 - wchNum0));
#endif
}

char ToSbcDigit(unsigned short wch)
{
	assert(IsDbcDigit(wch));
	return wch-wchNum0 + '0';
}
string ToSbcsDigit(const unsigned short *psz)
{
	string str;
	while(IsSJIS_1stByte(*psz)) {
		WORD wh = INV_MAKEWORD(*((BYTE*)psz), *((BYTE*)psz+1));
		str += ToSbcDigit(wh);
		psz++;
	}
	return str;
}
unsigned short ToDbcDigit(char ch)
{
	assert(isdigit(ch));
	return ch-'0' + wchNum0;
}

string ToDbcsDigit(const char *psz)
{
	string str;
	while(*psz) {
		unsigned short w = ToDbcDigit(*psz++);
		str += HighByte(w);
		str += LowByte(w);
	}
	return str;
}
unsigned short *itow(int i, unsigned short *psz, int base)
{
	itoa(i, (char*)psz, base);
	string str = ToDbcsDigit((char*)psz);
	strcpy((char*)psz, str.data());
	return psz;
}

int wtoi(const unsigned short *psz)
{
	string str = ToSbcsDigit(psz);
	return atoi(str.data());
}

bool IsAlpha(unsigned short wch)
{
	return IsUpper(wch) || IsLower(wch);
}
bool IsDbcAlpha(unsigned short wch)
{
	return IsDbcUpper(wch) || IsDbcLower(wch);
}
bool IsAlphaNum(unsigned short wch)
{
	return IsAlpha(wch) || IsDigit(wch);
}
bool IsDbcAlphaNum(unsigned short wch)
{
	return IsDbcAlpha(wch) || IsDbcDigit(wch);
}


#ifdef _JPN
bool IsKanji(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return (wch >= wchKanjiFirst);
}
bool IsHiragana(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return (unsigned short)(wch - wchHiraganaFirst)	<= (wchHiraganaLast - wchHiraganaFirst);
}
bool IsDbcKatakana(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return (unsigned short)(wch - wchKatakanaFirst) <= (wchKatakanaLast - wchKatakanaFirst) || (wch == wchBoten);
}
bool IsSbcKatakana(unsigned short wch)
{
	wch = LowByte(wch);
	return (unsigned short)(wch - chKatakanaFirst) <= (chKatakanaLast - chKatakanaFirst);
}
/*bool IsSbcKatakana(unsigned short wch)
{
	wch = LowByte(wch);
	return (unsigned short)(wch - chPureKatakanaFirst) <= (chPureKatakanaLast - chPureKatakanaFirst);
}
*/
bool IsKatakana(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return (!HighByte(wch) && IsSbcKatakana(wch)) || IsDbcKatakana(wch);
}
bool IsSbcKatakanaSymbol(unsigned short wch)
{
	return IsSbcKatakana(wch) && !IsSbcKatakana(wch);
}
bool IsDakuten(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return wch == chDakuten;
		//return (wch == chDakuten || wch == wchDakuten);
}
bool IsHandakuten(unsigned short wch)
{
	assert(IsValidDbc(wch));
	return wch == chHandakuten;
		//return (wch == chHandakuten || wch == wchHandakuten);
}
#endif /* _JPN */
bool IsSpecialCode(unsigned short wch)
{
#ifdef _JPN
	return (wch < 0x20 || (!IsSJIS_1stByte(LowByte(wch)) && !IsSbcKatakana(wch)));
#else
	return (wch < 0x20 || !IsSJIS_1stByte(LowByte(wch)));
#endif
}

int GetDbcsChType(BYTE ch, BYTE ch2)
{
	unsigned short wch = INV_MAKEWORD(ch, ch2);//MakeWord(ch, ch2);
	int chtype=0;
	if (IsKanji(wch))
		chtype = KANJ;
	else if (IsHiragana(wch))
		chtype = HIRAGANA;
	else if (IsDbcKatakana(wch))
		chtype = HANKATA;
	else if (wch == wchSpace)
		chtype = WHT;
	else if (IsDbcDigit(wch))
		chtype = NUM;
	else if (IsDbcAlphaNum(wch))
		chtype = ALP;
	else
		chtype = SYM;
	return chtype |= KANJ;
}
//////////////////////////////////////////////////////////////////////////////////////
CKinsokTbl vKinsokTbl;

void CKinsokTbl::ResetKinsok(const int bitKin, const string &strKin, CKinsokDblTbl_One &m_tbl)
{
	m_tbl.Initialize(strKin.size());
	for (int i=0; i<strKin.size(); i++) {
		BYTE ch = strKin[i], ch2;
		int chType = GetFullType(ch) | bitKin;
		if (IsSJIS_1stByte(ch)) {
			vchTypeTbl[ch] = chType | KINSOK_1ST;
			assert(i+1<strKin.size());
			ch2 = strKin[++i];
			chType = GetFullType(ch2) | bitKin;
			assert(IsSJIS_2ndByte(ch2));
			vchTypeTbl[ch2] = chType | KINSOK_2ND;
			m_tbl.push(ch, ch2);
		}else{
			vchTypeTbl[ch] = chType | KINSOK_SINGLE;
		}
	}
}

void CKinsokTbl::ResetKinsok(bool bKinsok, bool bKinFirst, const string &strKinFirst, bool bKinLast, const string &strKinLast)
{
#ifdef _DEBUG
//	BYTE ch_dbg = 'a';
//	TRACE("'%c'=0x%x\n", ch_dbg, GetFullType(ch_dbg));
#endif
	for (int i=0; i<0x100; i++) {
		vchTypeTbl[i] = ClearKinBit(GetFullType(i));
	}
#ifdef _DEBUG
//	TRACE("'%c'=0x%x\n", ch_dbg, GetFullType(ch_dbg));
#endif
	if (!bKinsok)
		return;
	if (bKinFirst)
		ResetKinsok(KINSOK_FIRST, strKinFirst, m_first);
#ifdef _DEBUG
//	TRACE("'%c'=0x%x\n", ch_dbg, GetFullType(ch_dbg));
#endif
	if (bKinLast)
		ResetKinsok(KINSOK_LAST, strKinLast, m_last);
#ifdef _DEBUG
//	TRACE("'%c'=0x%x\n", ch_dbg, GetFullType(ch_dbg));
#endif
}

string PickupSbcsWord(const BYTE *sz)
{
	string str;
	const BYTE *pch = sz;
	int chtype = AndSbcsChType(GetChType(*pch));
	while(*(++pch)) {
		if ((chtype & GetChType(*pch))==0) {
			str.append((char*)sz,pch-sz);
			break;
		}
	}
	return str;
}

string PickupDbcsWord(const BYTE *sz)
{
	string str;
	const BYTE *pch = sz;
	int chtype = GetChType(*pch);
	while(*(++pch)) {
		if ((chtype & GetChType(*pch))==0 || (*pch==wchSpace1st && *(pch+1)==wchSpace2nd)) {
			str.append((char*)sz,pch-sz);
			break;
		}
	}
	return str;
}

//#define GetType(ch)		vchTypeTbl[0xff & (ch)]
/*BYTE GetType(BYTE ch)
{
	return vchTypeTbl[0xff & (ch)];
}
*/
//////////////////////////////////////////////////////////////////
//Bracket handling
string GetStrDbcs_bracket_left(int off)
{
	assert(off<stcDbcs_bracket_left.size());
	string str;
	str.append(stcDbcs_bracket_left.data() + off*2, 2);
	return str;
}
int GetIndexDbcs_bracket_left(const BYTE *sz)
{
	OBJOFF off = stcDbcs_bracket_left.find((char*)sz, 0, 2);
	if (off != string::npos) {
		if (off%2 == 0)
			off /=2;
		else
			off = string::npos;
	}
	return  off;
}
string GetStrDbcs_bracket_right(int off)
{
	assert(off<stcDbcs_bracket_right.size());
	string str;
	str.append(stcDbcs_bracket_right.data() + off*2, 2);
	return str;
}
int GetIndexDbcs_bracket_right(const BYTE *sz)
{
	OBJOFF off = stcDbcs_bracket_right.find((char*)sz, 0, 2);
	if (off != string::npos) {
		if (off%2 == 0)
			off /=2;
		else
			off = string::npos;
	}
	return  off;
}
//////////////////////////////////////////////////////////////////
//Line Header handling
const static string stcSbcs_bracket_left =  "([<{¢";
const static string stcSbcs_bracket_right = ")]>}£";
const static string stcDbcs_bracket_left = "ÅiÅkÅmÅoÅqÅsÅyÅÉÅ·ÅgÅwÅuÅeáÄ";
const static string stcDbcs_bracket_right = "ÅjÅlÅnÅpÅrÅtÅzÅÑÅ‚ÅhÅxÅvÅfáÅ";
CLineHeader::CLineHeader()
{
	m_lht = LHT_NO;
}
bool CLineHeader::Initialize(const BYTE *sz)
{
	assert(sz);
	m_lht = LHT_NO;

	bool bLoop = true;
	bool bNextCheck_forDotNum = false;

	int lht;
	const BYTE *psz = sz;
	const BYTE *pszLh;
	while(*psz && bLoop) {
		if (IsSJIS_1stByte(*psz)) {
			lht = GetDbcsLineHeaderType(psz);
		}else{
			lht = GetSbcsLineHeaderType(psz);
			if (bNextCheck_forDotNum && OffDbcs_BitLht(lht)!=LHT_NUM) {
				lht = LHT_BRACKET_RIGHT;
				break;
			}
		}

		switch(OffDbcs_BitLht(lht)) {
		case LHT_NO:
			bLoop = false;
			break;
		case LHT_BRACKET_LEFT:
			m_strLh = "";
			break;
		case LHT_SPACE:
		case LHT_BRACKET_RIGHT:
			if (m_strRight=="") {
				m_strRight.append((char*)psz, 0, IsDbcs_BitLht(lht) ? 2:1);
				if (!IsDbcs_BitLht(lht) && *psz=='.' && OffDbcs_BitLht(m_lht)==LHT_NUM) {
					bNextCheck_forDotNum = true;
				}else
					bLoop = false;
			}
			break;
		//case LHT_MARK:
		//case LHT_NUM:
		//case LHT_ALPHA:
		//case LHT_KATA:
		//case LHT_OTHERS:
		//case LHT_SPECIAL:
		default:
			if (m_lht!=LHT_NO && !(OffDbcs_BitLht(m_lht)==LHT_NUM && OffDbcs_BitLht(lht)==LHT_NUM)) {
				m_lht = LHT_NO;
				return false;
			}
			if (OffDbcs_BitLht(lht)!=LHT_NUM || m_strRight!="" || lht!=m_lht) {
				m_strLh = "";
				m_strRight = "";
			}
			if (m_strLh == "") {
				pszLh = psz;
				m_lht = lht;
			}
			m_strLh.append((char*)psz, 0, IsDbcs_BitLht(lht) ? 2:1);
			m_lht = lht;
			break;
		}
		psz += IsDbcs_BitLht(lht) ? 2:1;
	}
	if (OffDbcs_BitLht(lht) == LHT_BRACKET_RIGHT && m_strLh != "" && m_strRight != "") {
		m_strLeft.append((char*)sz, 0, (int)(pszLh-sz));
		return true;
	}else if (OffDbcs_BitLht(m_lht) == LHT_MARK){
		return true;
	}else if (OffDbcs_BitLht(m_lht) == LHT_SPECIAL) {	//á@Å|áS	8740-8753
		return true;
	}
	m_lht = LHT_NO;
	return false;
}

bool CLineHeader::GetNextLineHeader(string &str)
{
	if (m_lht == LHT_NO)
		return false;
	switch(OffDbcs_BitLht(m_lht)) {
	case LHT_MARK:
		str = m_strLeft;
		str.append(m_strLh);
		str.append(m_strRight);
		return false;	//same now.
	//case LHT_NUM:
	//case LHT_ALPHA:
	//case LHT_KATA:
	//case LHT_OTHERS:
	//case LHT_SPECIAL:
	default:
		str = m_strLeft;
		if (SetNextLineHeader()) {
			str.append(m_strLh);
			str.append(m_strRight);
		}
		break;
	}
	return true;
}
int CLineHeader::GetLength()
{
	return (m_lht==LHT_NO ? 0:m_strLeft.size()+m_strLh.size()+m_strRight.size());
}
bool CLineHeader::SetNextLineHeader()
{
	//static const string strDbcsOthersLast = "áSá]˙IÑ`ÑëÉ∂É÷";
	static const string strLowerKatakana = "É@ÉBÉBÉDÉFÉHÉKÉMÉMÉOÉQÉSÉUÉWÉYÉ[É]É_ÉaÉbÉbÉdÉdÉfÉhÉoÉpÉrÉsÉuÉvÉxÉyÉ{É|ÉÉÉÖÉáÉéÉêÉëÉíÉîÉïÉñÅEÅE";
	bool brtn = false;
	switch(OffDbcs_BitLht(m_lht)) {
	case LHT_NUM:
		{
		int len = m_strLh.size();
		int i= (!IsDbcs_BitLht(m_lht) ? atoi(m_strLh.data()):
							wtoi((unsigned short*)m_strLh.data())) +1;
		char *psz = new char[len+3];
		int len_0 = len - strlen(!IsDbcs_BitLht(m_lht) ? itoa(i, psz, 10):
							(char*)itow(i, (unsigned short *)psz, 10));
		if (len_0>0) {
			string str = m_strLh;
			m_strLh = "";
			m_strLh.append(str.data(), 0, len_0);
		}else{
			m_strLh = "";
		}
		m_strLh += psz;
		delete [] psz;
		brtn = true;
		}
		break;
	case LHT_ALPHA:
	case LHT_KATA:
	case LHT_OTHERS:
	case LHT_SPECIAL:
		//skip strLowerKatakana
		{
		int len = m_strLh.size();
		BYTE szT[3];
		memset(szT, 0, sizeof(szT));
		szT[0] = m_strLh[IsDbcs_BitLht(m_lht) ? len-2:len-1];
		if (IsDbcs_BitLht(m_lht)) {
			szT[1] = m_strLh[len-1] + 1;
			while(OffDbcs_BitLht(m_lht)==LHT_KATA) {
				if (strLowerKatakana.find((char*)szT) == string::npos) {
					break;
				}
				szT[1]++;
			}
		}else
			szT[len-1]++;
		if (GetLineHeaderType(szT)==OffDbcs_BitLht(m_lht)) {
			m_strLh = (char*)szT;
			brtn = true;
		}
		}
		break;
	default:
		assert(false);
		break;
	}
	return brtn;
}

int CLineHeader::GetSbcsLineHeaderType(const BYTE *sz)
{
	BYTE ch = *sz;
	if (isdigit(ch))
		return LHT_NUM;
	else if (isalpha(ch))
		return LHT_ALPHA;
	else if (IsSbcKatakana(ch))
		return LHT_KATA;
	else if (IsSbcMark(ch))
		return LHT_MARK;
	else if (IsSbcBracketRight(ch))
		return LHT_BRACKET_RIGHT;
	else if (IsSbcBracketLeft(ch))
		return LHT_BRACKET_LEFT;
	else if(ch==0x20)
		return LHT_SPACE;
	return LHT_NO;
}

bool CLineHeader::IsDbcsMark(const BYTE *sz)
{
	static string stcDbcsLineHeader = "ÅñÅùÅEÅ¶ÅúÅ°ÅüÅöÅõÅ†ÅûÅô";
	OBJOFF off = stcDbcsLineHeader.find((char*)sz, 0, 2);
	return  off != string::npos && off%2==0;
}

bool CLineHeader::IsDbcsBracketLeft(const BYTE *sz)
{
	//ÅiÅjÅkÅlÅmÅnÅoÅpÅqÅrÅsÅtÅyÅzÅÉÅÑÅ·Å‚
	static string stcDbcsLineHeader_bracket_left = "ÅiÅkÅmÅoÅqÅsÅyÅÉÅ·";//remove Å|
	OBJOFF off = stcDbcsLineHeader_bracket_left.find((char*)sz, 0, 2);
	return  off != string::npos && off%2==0;
}
bool CLineHeader::IsDbcsBracketRight(const BYTE *sz)
{
	//ÅDÅiÅjÅkÅlÅmÅnÅoÅpÅqÅrÅsÅtÅyÅzÅÉÅÑÅ·Å‚
	static string stcDbcsLineHeader_bracket_right = "ÅDÅjÅlÅnÅpÅrÅtÅzÅÑÅ‚";//remove Å|
	OBJOFF off = stcDbcsLineHeader_bracket_right.find((char*)sz, 0, 2);
	return  off != string::npos && off%2==0;
}

int CLineHeader::GetDbcsLineHeaderType(const BYTE *sz)
{
	WORD wh = INV_MAKEWORD(*sz, *(sz+1));
	if (IsDbcDigit(wh))
		return LHT_DBC_NUM;
	else if (IsDbcAlpha(wh))
		return LHT_DBC_ALPHA;
	else if (IsDbcKatakana(wh))
		return LHT_DBC_KATA;
	else if (IsDbcsMark(sz))
		return LHT_DBC_MARK;
	else if (IsDbcsBracketRight(sz))
		return LHT_DBC_BRACKET_RIGHT;
	else if (IsDbcsBracketLeft(sz))
		return LHT_DBC_BRACKET_LEFT;
	else if((0x8754 <=wh && wh <= 0x875D) ||	//áTÅ|á]	FA40-FA49
		(0xFA40 <=wh && wh <= 0xFA49) ||	//˙@Å|˙I	FA40-FA49
		//(0x8341 <=wh && wh <= 0x8393) ||	//ÉAÅ|Éì	8341-8393
		(0x8440 <=wh && wh <= 0x8460) ||	//Ñ@-Ñ`		8440-8460
		(0x8470 <=wh && wh <= 0x8491) ||	//Ñp-Ñë		8470-8491
		(0x839F <=wh && wh <= 0x83B6) ||	//Éü-É∂		839F-83B6
		(0x83BF <=wh && wh <= 0x83D6))		//Éø-É÷		83BF-83D6
		return LHT_DBC_OTHERS;
	else if(0x8740 <=wh && wh <= 0x8753)	//á@Å|áS	8740-8753
		return LHT_DBC_SPECIAL;
	else if (wh==wchSpace)
		return LHT_DBC_SPACE;
	return LHT_NO;
}


int CLineHeader::GetLineHeaderType(const BYTE *sz)
{
	if (IsSJIS_1stByte(*sz)) {
		return OffDbcs_BitLht(GetDbcsLineHeaderType(sz));
	}else{
		return GetSbcsLineHeaderType(sz);
	}
}



