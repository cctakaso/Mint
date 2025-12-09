#ifndef __OBJCASH_H__
#define __OBJCASH_H__

#include "sell.h"
#include "hint.h"
class CObjTbl;
class CViewEd;
class CViewEnv;
class CLineHint;
class CByteCash;

enum OCF_TYPE {
	OCF_NO,
	OCF_LAYOUT,
	OCF_TEXT,
};
enum {
	CLOSE_ALL,
	CLOSE_THIS,
	CLOSE_NOT_THIS,
};

//Append block size
#define ABLS_BEF	(256)
#define ABLS_AFT	(sizeof(int))
#define ABLS		(ABLS_BEF+ABLS_AFT)
class CObjCashFile
{
	friend class CObjTbl;
	friend class CViewEd;
	friend class CObjLay;
	friend class CWinApplication;
	friend class CObjLayTbl;
	friend class CByteCash;
public:
	CObjCashFile();
	virtual ~CObjCashFile();
protected:
	bool OnIdle(LONG lCount) {
//		if (!m_bFormatAll) {
			return FormatObj(false, null);
//		}
		return false;
	}

	virtual bool FormatObj(bool bfirst, CLineHint *pHint, bool bfastReturn=true) = 0;
	virtual CViewEnv *Attach_Env(CViewEnv *pViewEnv);
	virtual void ResetCash() = 0;

	bool IsValidEnv() {
		return m_pViewEnv!=null; }

	bool IsFormatAll() const {
		return m_bFormatAll; }


	CViewEnv *GetEnv() {return m_pViewEnv;};
	const int AttachCount(int i=0) {
		return (m_countAttach +=i); }

	bool bAbleWrite() {
		return (m_nMode & ios::out)!=0; }
	bool DoBackupFile(string &strNewFile, bool bRename, const char *szOldFile=null);
	bool ResetFile(const char *sz=null, bool bTempFile=true);
	bool SetFile(const char *szfilename,
		int nMode=(ios::in | ios::out | ios::binary), int nProt = 0/*filebuf::sh_read*/);
	bool IsOpenFile() {
		return (IsValidDataSz(m_filename.c_str()) && m_file.is_open()); }
	void DeleteTempFile();

	void InitCashSizes();
	OBJSIZE InitCashRead(BYTE *&pCashBuffer);
	OBJSIZE InitBlockRead(BYTE *&pCashBuffer);
	OBJSIZE ReadCash(BYTE *pCashBuffer, OBJOFF off=NPOS);
	OBJSIZE ReadBlock(BYTE *pCashBuffer, OBJOFF off=NPOS);
private:
	OBJSIZE InitBufferRead(BYTE *&pCashBuffer, OBJSIZE size);
	OBJSIZE ReadBuffer(BYTE *pCashBuffer, OBJOFF off, OBJSIZE size);
protected:
	bool		m_bFormatAll;
	CViewEnv*	m_pViewEnv;
	int			m_countAttach;
	int		m_nMode;
	int		m_nProt;
	fstream m_file;
	string	m_filename;
	OBJSIZE	m_size;
	time_t	m_time;
	BYTE*	m_pCashBuffer;
	int		m_numBlock;
	OBJSIZE	m_sizeBlock;
	OBJSIZE	m_sizeCash;
	OCF_TYPE	m_ocf;
	bool	m_bNowAccessFile;
	//bool	m_bFirstSave;//takaso2010 move to CObjLayTbl
	bool	m_bTempFile;
};


class CObjTbl :protected deque<CObjCashFile *>
{
	friend class CWinDocument;
	friend class CObjCashFile;
	friend class CTextCashTbl;
	friend class CObjLayTbl;
	friend class CByteCash;
	friend class CFileSelectBar;
	friend class CTheDocTemplate;
	friend class CDLLArray;
public:
	CObjTbl();
	~CObjTbl();

	DWORD CheckLowMem() {
		GlobalMemoryStatus(&m_mem_stat);
		return m_mem_stat.dwAvailPhys;}
	bool InitialDocTran(bool frest=false);

protected:
	void InitCashSizes(OBJSIZE sizefile, OBJSIZE &sizeCash, OBJSIZE &sizeBlock);
	CObjLayTbl* NewMainFile(const char *szfilename=null,  
		int nMode=(ios::in | ios::out | ios::binary), int nProt = 0/*filebuf::sh_read*/, LPSTR knjcode=null);
	void CloseBeforFiles(size_type idxMax);
	void CloseLayouts(int ctype, CObjLayTbl *pLay=null);
private:
	MEMORYSTATUS m_mem_stat;
	void DeleteAll();
	CDLLArray *m_parypDll;	//takaso2010 Dllオブジェクト
	//takaso2010
	bool DocTran(const char *szSrcname,  const char *szDesname, string &srcknjcode, LPSTR desknjcode);
};


/////////////////////////////////
//Global object
extern CObjTbl theObjTbl;

#endif //__OBJCASH_H__
