#ifndef __CHARA_H__
#define __CHARA_H__

extern const char stchttp[];
extern const char stcCLangComment[];

//#define         VK_RETURN      0x0d
#define     VK_NL      0x0a
#define     ESC			0x1b
#define     SP			0x20
#define     DEL			0x7f
#define		VK_EOF		(0x1A)
#define		wchSpace	0x8140
#define LowByte(w)			((BYTE)(w) & 0x00ff)
#define HighByte(w)			(((unsigned short)(w) >> 8) & 0x00ff)
#define		wchSpace1st	HighByte(wchSpace)
#define		wchSpace2nd	LowByte(wchSpace)

//////ANSI->
//#define ANS			1
#define CTR			1			//\0, CR, LF, EOF
#define WHT			2			//SPACE, TAB
#define NUM			4			//1,2,3...
//#define ALP			8			//A,B,C...
#define ALP			NUM			//A,B,C...  //±ÙÌ§ÍÞ¯Ä‚Í”Žš‚Æ“¯‚¶ˆµ‚¢‚É‚·‚é
#define SYM		    0x10		//!"#$...
////////////////////////////
#define HANKATA     0x20        /* "half-width" KATAKANA            */
#define SJ1			0x40		/* The first byte of shift-JIS KANJI    */
#define SJ2			0x80		/* The second byte of shift-JIS KANJI   */
////////////////////////////
#define KANJ		0x40		/* Š¿Žš‚Ìê‡‚ÍA‚±‚Ìƒtƒ‰ƒO‚¾‚¯ON    */
#define HIRAGANA	0x80		/* ‚Ð‚ç‚ª‚È  */
////////////////////////////
#define KINSOK_FIRST	0x0100
#define KINSOK_LAST		0x0200
#define KINSOK_SINGLE	0x0400
#define KINSOK_1ST		0x0800
#define KINSOK_2ND		0x1000
////////////////////////////
//#define ASJ2		(ANS | SJ2)
#define SYJ2		(SYM | SJ2)
#define ALJ2		(ALP | SJ2)
#define SJ12        (SJ1 | SJ2)
#define SJ2H        (SJ2 | HANKATA)
#define ALPSYM		(ALP | SYM)
#define ALPSYMNUM		(ALP | SYM | NUM)
#define CTRWHTSYM	(CTR | WHT | SYM)
extern unsigned short vchTypeTbl[];
#define GetChType(ch)		((vchTypeTbl[0xff & (ch)]) & 0xff)
#define GetFullType(ch)		vchTypeTbl[0xff & (ch)]

//takaso2008
void widen(const std::string &src, std::wstring &dest);
//BYTE GetChType(BYTE ch);
#define AndSbcsChType(chType)	(chType & 0x3F)
#define IsValidCode(p)	(p!=0)
#define IsSbcWordKeyword_first(chtype_bef, chtype)	((AndSbcsChType(chtype_bef & chtype))==0)
#define IsDbcWordKeyword_first(chtype_bef)	((chtype_bef & SJ1)==0)
#define IsDbcSpace(ch, ch2)	(ch==wchSpace1st && ch2==wchSpace2nd)
string PickupSbcsWord(const BYTE *sz);
string PickupDbcsWord(const BYTE *sz);
 
//((((BYTE)ch)>=0x81 && ((BYTE)ch)<=0x9f) || (((BYTE)ch)>=0xe0 && ((BYTE)ch)<=0xef))
#define IsSJIS_1stByte(ch)	(GetFullType(ch) & SJ1)
#define IsSJIS_1stByte_chType(chType)	(chType & SJ1)
//((((BYTE)ch)>=0x40 && ((BYTE)ch)<=0x7e) || (((BYTE)ch)>=0x80 && ((BYTE)ch)<=0xfc))
#define IsSJIS_2ndByte(ch)	(GetFullType(ch) & SJ2)
#define IsSJIS_2ndByte_chType(chType)	(chType & SJ2)
//#define szEOF	"[EOF]"
#define IsCRLF(p)	(p==VK_RETURN || p==VK_NL)
#define IsDelete(p)	(p==VK_BACK || p==VK_MBUTTON/*VK_DELETE*/)
#define IsControl_chtype(chType) ((chType & CTR)>0)
#define IsWhite_chtype(chType)	((chType & WHT)>0)
#define IsWhite(p)	IsWhite_chtype(GetFullType(ch))

//typedef unsigned short      unsigned short;
//#define MAKEWORD(a, b)      ((unsigned short)(((BYTE)(a)) | ((unsigned short)((BYTE)(b))) << 8))
//#define	MakeWord(a, b)	((unsigned short)((b<<8) | a))
#define MakeWord(bLo, bHi)	((unsigned short)(((BYTE)(bLo)) | ((unsigned short)((BYTE)(bHi))) << 8))
#define INV_MAKEWORD(a, b)      ((unsigned short)(((BYTE)(b)) | ((unsigned short)((BYTE)(a))) << 8))

int GetDbcsChType(BYTE ch, BYTE ch2);

//////////////////////////////////////////////////////////////////
//for kinsok!
#define ClearKinBit(chType)		(chType & 0xe0ff)
#define GetKinBit(chType)		(chType & 0x1f00)
#define IsKinBit(chType)		(GetKinBit(chType)>0)
class CKinsokDblTbl_One
{
public:
	CKinsokDblTbl_One() {
		m_pdata =null; }
	~CKinsokDblTbl_One() {
		clear(); }
public:
	void Initialize(int entry) {
		assert(entry>=0);
		clear();
		if (entry>0) {
			m_pdata = new unsigned short[entry+1];
			memset(m_pdata,0,sizeof(unsigned short)*(entry+1));
		}
	}
	void push(BYTE ch, BYTE ch2) {
		assert(m_pdata);
		m_pdata[m_useMax++] = INV_MAKEWORD(ch,ch2);
	}
	bool Find(BYTE ch, BYTE ch2) {
		unsigned short wch = INV_MAKEWORD(ch,ch2);
		for(int i=0; i<m_useMax; i++) {
			if (wch==m_pdata[i])
				return true;
		}
		return false;
	}
private:
	void clear() {
		if (m_pdata)
			delete [] m_pdata;
		m_pdata = null;
		m_useMax = 0; }
	unsigned short *m_pdata;
	int m_useMax;
};
class CKinsokTbl
{
public:
	CKinsokTbl() {};
	~CKinsokTbl() {};
	void ResetKinsok(bool bKinsok, bool bKinFirst, const string &strKinFirst, bool bKinLast, const string &strKinLast);
	bool FindKinFirst(BYTE ch, BYTE ch2) {
		return m_first.Find(ch, ch2); }
	bool FindKinLast(BYTE ch, BYTE ch2) {
		return m_last.Find(ch, ch2); }
private:
	CKinsokDblTbl_One m_first;
	CKinsokDblTbl_One m_last;
	void ResetKinsok(const int bitKin, const string &strKin, CKinsokDblTbl_One &m_tbl);
};

extern CKinsokTbl vKinsokTbl;

inline bool IsKinSingle(int chType, int kin_bit)
{
	return (chType & (kin_bit|KINSOK_SINGLE))==(kin_bit|KINSOK_SINGLE);
}
inline bool IsKinDouble(int chType, int chType2, int kin_bit)
{
	return 	(chType & (kin_bit|KINSOK_1ST))==(kin_bit|KINSOK_1ST) && (chType2 & (kin_bit|KINSOK_2ND))==(kin_bit|KINSOK_2ND);
}
inline bool IsKinFirstSingle(int chType)
{
	return IsKinSingle(chType, KINSOK_FIRST);
}
inline bool IsKinLastSingle(int chType)
{
	return IsKinSingle(chType, KINSOK_LAST);
}
inline bool IsKinFirstDouble(int chType, int chType2, BYTE ch, BYTE ch2)
{
	return IsKinDouble(chType, chType2, KINSOK_FIRST) && vKinsokTbl.FindKinFirst(ch, ch2);
}
inline bool IsKinLastDouble(int chType, int chType2, BYTE ch, BYTE ch2)
{
	return IsKinDouble(chType, chType2, KINSOK_LAST) && vKinsokTbl.FindKinLast(ch, ch2);
}

bool IsDbcKatakana(unsigned short wch);

//////////////////////////////////////////////////////////////////
//Bracket handling
extern const string stcSbcs_bracket_left;
extern const string stcSbcs_bracket_right;
extern const string stcDbcs_bracket_left;
extern const string stcDbcs_bracket_right;

string GetStrSbcs_bracket_left(int off);
int GetIndexSbcs_bracket_left(const BYTE *sz);
int iSbcs_bracket_left(const BYTE ch);
string GetStrSbcs_bracket_right(int off);
int iSbcs_bracket_right(const BYTE ch);
int GetIndexSbcs_bracket_right(const BYTE *sz);

int GetIndexDbcs_bracket_left(const BYTE *sz);
int GetIndexDbcs_bracket_right(const BYTE *sz);
string GetStrDbcs_bracket_left(int off);
string GetStrDbcs_bracket_right(int off);

inline string GetStrSbcs_bracket_left(int off) {
	assert(off<stcSbcs_bracket_left.size());
	string str;
	str.append(1, stcSbcs_bracket_left[off]);
	return str;
}
inline int GetIndexSbcs_bracket_left(const BYTE *sz) {
	assert(sz);
	return iSbcs_bracket_left(*sz);
}
inline int iSbcs_bracket_left(const BYTE ch) {
	return (OBJOFF)stcSbcs_bracket_left.find((char)ch);
}
inline string GetStrSbcs_bracket_right(int off) {
	assert(off<stcSbcs_bracket_right.size());
	string str;
	str.append(1, stcSbcs_bracket_right[off]);
	return str;
}
inline int iSbcs_bracket_right(const BYTE ch) {
	return (OBJOFF)stcSbcs_bracket_right.find((char)ch);
}
inline int GetIndexSbcs_bracket_right(const BYTE *sz) {
	assert(sz);
	return iSbcs_bracket_right(*sz);
}

//////////////////////////////////////////////////////////////////
//Line Header handling
class CLineHeader
{
public:
	CLineHeader();
	virtual ~CLineHeader() {};
	
	enum  {
		LHT_NO=0,
		LHT_MARK=0x01,
		LHT_NUM=0x02,
		LHT_ALPHA=0x03,
		LHT_KATA=0x04,
		LHT_BRACKET_LEFT=0x05,
		LHT_BRACKET_RIGHT=0x06,
		LHT_OTHERS=0x07,
		LHT_SPECIAL=0x08,
		LHT_SPACE=0x09,
		LHT_DBC_MARK=0x11,
		LHT_DBC_NUM=0x12,
		LHT_DBC_ALPHA=0x13,
		LHT_DBC_KATA=0x14,
		LHT_DBC_BRACKET_LEFT=0x15,
		LHT_DBC_BRACKET_RIGHT=0x16,
		LHT_DBC_OTHERS=0x17,
		LHT_DBC_SPECIAL=0x18,
		LHT_DBC_SPACE=0x19,
	};
	int	m_lht;
private:
	string m_strLeft;
	string m_strLh;
	string m_strRight;

public:
	bool Initialize(const BYTE *sz);
	bool GetNextLineHeader(string &str);
	int GetLength();
	int OffDbcs_BitLht(int lht) { return lht & 0xef; }
	int IsDbcs_BitLht(int lht) { return lht & 0x10; }
protected:
	bool SetNextLineHeader();

	int GetSbcsLineHeaderType(const BYTE *sz);
	bool IsSbcMark(BYTE ch) {
		return (ch=='*' || ch=='¥'); }
	bool IsSbcBracketLeft(BYTE ch) {
		return (ch=='.' || iSbcs_bracket_left(ch)!=string::npos);}
	bool IsSbcBracketRight(BYTE ch) {
		return (ch=='.' ||  iSbcs_bracket_right(ch)!=string::npos); }
	
	int GetDbcsLineHeaderType(const BYTE *sz);
	bool IsDbcsMark(const BYTE *sz);
	bool IsDbcsBracketLeft(const BYTE *sz);
	bool IsDbcsBracketRight(const BYTE *sz);

	int GetLineHeaderType(const BYTE *sz);
};

#endif // __CHARA_H__
