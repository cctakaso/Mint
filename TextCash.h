#ifndef __TEXTCASH_H__
#define __TEXTCASH_H__

#include "ObjCash.h"
#include "sell.h"
#include "ViewEnv.h"

class CViewEd;
class SELLS;
class CLineHint;
class CTextCashTbl;
class CViewEd;

class CTextCash	 :protected string
{//必ずスペースを' 'を付加してある、場合によってCopyの為にNLだけの場合は、CRLFにする
	friend class CTextCashTbl;
public:
	CTextCash() {m_off=NPOS;};
	~CTextCash() {};
protected:
	bool IsHit(OBJOFF off) {
		return off>=m_off && off<(m_off+m_size);	}
	void Initialize(OBJOFF off) {
		m_off = off;
		m_size = 0;
		erase(begin(), end()); }
	OBJSIZE Read(BYTE *&pBuff, OBJOFF off) {
		pBuff = GetDataPtr(off);
		return SIZEMAC(size()) - (off-m_off); }
	BYTE* GetDataPtr(OBJOFF off=0) {
		assert(IsHit(off));
		return ((BYTE*)data())+(off-m_off); }
	void SetSize() { //現在の
		m_size = size();}
private:
	OBJOFF	m_off;
	OBJSIZE m_size;		//データとしてはCRLFだが、付加したLF分はヒットしないようにする
	bool IsEnable() {
		return size()>0; }
};

class CByteCash
{
	friend class CTextCashTbl;
	friend class CViewEd;
	friend class CLineHint;
public:
	CByteCash();
//	CByteCash(unsigned long seekpos);

	~CByteCash();
protected:
	OBJOFF	m_off;
	OBJSIZE	m_size;
	time_t		m_time;
	int			m_count_lock;
	BYTE		*m_pdata;
	bool		m_bABLS_BEF;
protected:
	int Lock(){
		return ++m_count_lock;	}
	int Unlock() {
		assert(m_count_lock>0);
		time(&m_time);
		return --m_count_lock; }
	int GetLockCount() {
		return  m_count_lock; }

	bool IsEnable() {
		return m_pdata!=null; }
	void Unload() {
		assert(IsEnable());
		delete m_pdata;
		m_pdata = null;
		m_count_lock = 0; }
	bool Load(CObjCashFile *pObjCashFile, bool bNoSeek=false);
	bool IsValidOffset(OBJOFF off) {
		return off<m_size; }
	OBJSIZE Read(BYTE *&pBuff, OBJOFF off) {
		pBuff = GetDataPtr(off);
		return m_size - off; }
	BYTE *GetDataPtr(OBJOFF off) {
		assert(IsValidOffset(off) && IsEnable());
		return m_bABLS_BEF ? m_pdata+off+ABLS_BEF:m_pdata+off; }
	void CopyData(BYTE *pdata, OBJOFF size);
	void replace(OBJOFF src_first, OBJOFF src_last, BYTE *pbytes, OBJSIZE len);
};

class CPosCash
{
	friend class CTextCashTbl;
	friend class CLineHint;
public:
	CPosCash();
	~CPosCash();
private:
	CByteCash	*m_pbytes;
	OBJOFF		m_off;
	OBJSIZE		m_size;
	OBJOFF		*m_pdata;
	bool IsEnable() {
		return m_pdata!=null; }
	bool IsValidOffset(OBJOFF off) {
		assert(IsEnable());
		return off>=m_off && off < (m_off + m_size); }
	bool IsHit_Enable(CByteCash *pbytes, OBJOFF off) {
		return IsEnable() && pbytes==m_pbytes && IsValidOffset(off); }
	OBJOFF IsLastOffsetOfRow(OBJOFF off);
#ifdef _DEBUG
	void Dump();
#endif
protected:
	bool IsHit(CByteCash *pbytes, OBJOFF off) {
		return IsHit_Enable(pbytes, off) && *GetDataPtr(m_off+m_size-1)>0; }
	void Initialize(CByteCash *pbytes, OBJOFF off, OBJSIZE size);
	void Reset();
	OBJOFF *GetDataPtr(OBJOFF off) {
		assert(IsValidOffset(off) && IsEnable());
		return m_pdata + (off-m_off); }
	OBJOFF FindOffset_Of_Keta(OBJOFF off_first, OBJOFF &keta, int iPrevfit, bool &bNextRow, bool bFree);
};

typedef deque<CByteCash *>	TextCashTbl;
typedef TextCashTbl::size_type TEXTBLOCK_IDX;

class CTextCashTbl	:public CObjCashFile
{
	friend class SELLS;
	friend class CLineHint;
	friend class CViewEd;
	friend class CScanDataBlock;
	friend class CTheApp;
public:
	CTextCashTbl();
	~CTextCashTbl();
protected:
	OBJSIZE Read(BYTE *&pBuff, OBJOFF off_first);
	OBJSIZE ReadLineCash(BYTE *&pBuff, OBJOFF off_first, CByteCash *pbytes=null);
	CPosCash *ReadLineCash_With_PosCash(BYTE *&pBuf, OBJOFF off_first, CByteCash *pbytes, OBJSIZE len, CFormatChars *pFormatChars);
	OBJSIZE ReadBlockCash(BYTE *&pBuff, OBJOFF off_first, CByteCash *pbytes=null);
	int Lock(TEXTBLOCK_IDX idx);
	int Unlock(TEXTBLOCK_IDX idx);
	CByteCash *NewElement(const char *sz, OBJSIZE len);

	//same as CObjLayTbl/CTextCashTbl member
	bool FormatObj(bool bfirst, CLineHint *pHint, bool bfastReturn=true);
	void ResetCash() {
		m_posLine.Reset(); }
private:
	BYTE		*m_pchBuf;
	OBJOFF  m_iEateNextChar;

	TextCashTbl elements;
	CTextCash	m_textLine;
	CPosCash	m_posLine;
	CFormatChars *m_pFormatChars_Loader;
#ifdef _VERIFY
	long m_iCall;
#endif

	bool FormatObj_Loader(bool bfirst, CLineHint *pHint, bool bfastReturn=true);
	void InitCashFiles(OBJOFF off_last);


	//for elements
	#define size_type	TEXTBLOCK_IDX
	const size_type size() const{
		return elements.size(); }
	void push_back(CByteCash *pbytes) {
		assert(pbytes);
		elements.push_back(pbytes); }
	CByteCash* at(size_type pos) {
		return elements.at(pos); }
	CByteCash* operator[](size_type pos) {
		return at(pos); }
	const CByteCash* at(size_type pos) const{
		return elements.at(pos); }
	const CByteCash* operator[](size_type pos) const{
		return at(pos); }
	#undef size_type
};

#endif //__TEXTCASH_H__
