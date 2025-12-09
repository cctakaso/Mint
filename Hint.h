#ifndef __HINT_H__
#define __HINT_H__

#include "sell.h"
#include "action.h"

class CViewEnv;
class CFormatChars;
class CActionLgc;
class CAction;

enum {
	SETSEL_SET,
//	SETSEL_ROWEND,
	SETSEL_PREVCHAR,
	SETSEL_NEXTCHAR,
};

//カーソルを与えてその中の文字列を１行単位でコールする
class CLineHint;

typedef struct {
	HGLOBAL hData;
	OBJSIZE lenData;
	string *pstrAdd;
	const string *pstrRemove;
	const CLineHint *pHint;
}SSDP_DATA;
typedef struct {
	string strFind;
	bool bCase;
	BYTE	chUpper;
	BYTE	chLower;
}find_data;

typedef int (*FindDataLineProc)(const string &str, void *pv, OBJOFF &off_find);
typedef int (*ScanDataLineProc)(const string &, void *pv);
typedef int (*ScanDataBlockProc)(const BYTE *pbytes, OBJSIZE len, void *pv);

/////////////////////////////////////////////////////////////////
//BookMark
enum BMARK{
	bmark_normal,
	bmark_flp,
	bmark_input,
};
class CBlockMarks;
class CBookMark
{
	friend class CBlookMarks;
public:
	CBookMark() {
		m_line = 0; m_row = 0; m_col = 0; m_bmark=bmark_normal;}
	CBookMark(CViewEd *pviewed, BMARK bmark=bmark_normal) {
		SetMark(pviewed, bmark); }
	~CBookMark() {};
public:
	void SetMark(CViewEd *pviewed, BMARK bmark=bmark_normal);
	bool JumpMark(CViewEd *pviewed);
	//bool JumpFlpMark(CViewEd *pviewed);
	OBJOFF m_line;
	OBJOFF m_row;
	OBJOFF m_col;
	BMARK  m_bmark;
	CBookMark &operator =(const CBookMark &mark);
protected:
	friend int comp(const CBookMark &mark1, const CBookMark &mark2);;
};
int	comp(const CBookMark &mark1, const CBookMark &mark2);
inline bool operator ==(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)==0; }
inline bool operator !=(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)!=0; }
inline bool operator >(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)>0; }
inline bool operator >=(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)>=0; }
inline bool operator <(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)<0; }
inline bool operator <=(const CBookMark &mark1, const CBookMark &mark2) {
	return comp(mark1, mark2)<=0; }
//erace()コール後、Begin()がかならずあるようになるため、
//最初からFlpMarkは入れておく事にする。
class CBookMarks :public vector<CBookMark>
{
public:
	CBookMarks();
	~CBookMarks() {};
public:
	bool SetMark(CViewEd *pviewed, CBookMark *pMark=null,  BMARK bmark=bmark_normal);
	void RecalcPosition(OBJOFF line_first, OBJOFF line_num_del, OBJOFF line_num_ins, 
					OBJOFF row_first, OBJOFF row_num_del, OBJOFF row_num_ins);
	bool JumpPrev(CViewEd *pviewed);
	bool JumpNext(CViewEd *pviewed);
	bool JumpUnnormalMark(CViewEd *pviewed, BMARK bmark);
	CBookMark RemoveUnnormal(BMARK bmark);
	void RemoveAll_normal();
	bool IsEmpty_normal(bool bAll=false) {
		return bAll ? size()==0:size()<=2; }
};

/////////////////////////////////////////////////////////////////
// CScanDataBlock
class CScanDataBlock
{
public:
	CScanDataBlock();
	~CScanDataBlock();
	
	void Initialize(CUR _cur, CLineHint *pHint, const CUR *pcur_last=null);
	OBJSIZE ReadBuffer();
	BYTE *GetBuffer() {
		return m_pch; }
//	OBJOFF GetOffSetOfScanStart();
	OBJSIZE GetReadSize() {
		return m_len; }
	CUR GetCur(OBJOFF off);
protected:
	bool m_bLoop;
	CUR m_cur;
	CUR m_cur_prv;
	const CUR *m_pcur_last;
	BYTE *m_pch;
	OBJSIZE m_len;
	CLineHint *m_pHint;
	OBJSIZE m_sizeHint;
};

/////////////////////////////////////////////////////////////////
// CLineHint
class CLineHint :public SELLS
{
	friend int SStDP_CopyProc_line_center(const string &str, void *pv);
	friend int SStDP_CopyProc_line_right(const string &str, void *pv);
public:
	CLineHint();
	~CLineHint();
public:
	void Initialize(CViewEnv *pViewEnv);
	
	//cursor transfer.
	CUR GetCur_0();
	CUR GetCur_NPOS();
	void SetCur(CUR &cur, bool bFree=false, int iPrevfit=SETSEL_SET, bool bExpand = false);
	void NextCur(CUR &cur, bool bFree=false);
	void PrevCur(CUR &cur, bool bFree=false);
	bool SetCur_4Lgc(CUR &cur, OBJOFF row=0);
	OBJOFF String2Keta(const string &str, bool &bOver) const;

	//input fuctions
	bool DoActionLgc(CActionBlock &actBlock_del, CActionBlock &actBlock_ins,
							CCursor &cursor_befor, CCursor &cursor_after, CTextCashTbl *pEditTbl);
	bool SetActionLgc(CActionBlock &actBlock_del, CActionBlock &actBlock_ins,
							CCursor &cursor_befor, CCursor &cursor_after, const string *pstring, CTextCashTbl *pEditTbl);
	//size
	DSP_Y size_mac() {
		return size()==0 ? 0:size()-1; }
	//copy
	HGLOBAL GetCursorData(const CCursor &_cursor, Normal_Proc pProc, SSDP_DATA *pData);
	//HGLOBAL GetCursorData(const CCursor &cursor, const string *pstring, HGLOBAL hData=null, OBJSIZE lenData=0);
	
	//scan
	int ScanDataString(CCursor &cursor, ScanDataLineProc pProc, void *pv);
	int ScanDataBlock(CUR _cur, ScanDataBlockProc pProc, void *pv, const CUR *pcur_last=null);
	OBJSIZE GetNext_BlockTop(CUR &cur, const OBJSIZE &lenCheck);

	//find
	bool FindData(CCursor &cursor, string strFind, bool bCase, bool bToDocEnd);
	bool FindData_ToTop(CCursor &cursor, FindDataLineProc pProc, void *pv, bool bExpand=true);
	bool FindData_ToEnd(CCursor &cursor, FindDataLineProc pProc, void *pv, bool bExpand=true);

	//double click
	bool FindDiffChType(CUR &cur, int& _type, bool bEnableSpace, bool bTalCurOk=false, bool bFoward=true);

	//for free cursor
	void ResetUnFreeCursor(CCursor &cursor);

	//format
	void ReBuildDsp2DspSells(CViewEnv *pViewEnv, CLineHint &sellsDest);

	//mark jump
	CBookMarks	m_bookMarks;

	//get
	void GetCurLeftRightChStrLine(CUR &cur, string &strLeft, string &strRight);
protected:
	CFormatChars *m_pFormatChars;
	SELL &at_mac(DSP_Y ydsp) {
		return ydsp<size() ? at(ydsp):at(size_mac()); }

	void BuildLine2DspSells(const SELLS::iterator iter_begin, const SELLS::iterator iter_end, SELLS &sellsDsp);
	void BuildLine2DspSell(SELL sellLgc, SELLS &sellsDsp, OBJOFF line=NPOS);
	SELL BuildString2LineSells(SELLS &sells, const string *pstr, CTextCashTbl *pEditTbl, OBJOFF off_cur);
};

#endif //__HINT_H__
