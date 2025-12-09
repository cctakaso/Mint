#ifndef __VIEWENV_H__
#define __VIEWENV_H__
////////////////////////////////////////////
#include "sell.h"
#include "profile.h"
#include "keyword.h"
#include "tanbun.h"
#include "datetime.h"
/*
class CViewEnvTbl;
class CViewEd;
class CObjCashFile;
class CByteCash;
class CLineHint;
class CTextCashTbl;
class CDispPage;
class CFilePage;
class CFormatPage;
class CConfigSheet;
*/
#define CALC_TABSTOPS(keta,stops)	(stops - keta%stops)
class CFormatChars
{
	friend class CLineHint;
	friend class CTextCashTbl;
	friend int SStDP_CopyProc_line_center(const string &str, void *pv);
	friend int SStDP_CopyProc_line_right(const string &str, void *pv);
	friend class CTheApp;
public:
	CFormatChars(int xdspMax, int xdspMax_save, bool bKinsok,
		bool bCRLFBra, bool bKinFirst, bool bKinLast, bool bWordWrap, int nTabStops) {
		Initialize(0,0);
		m_xdspMax=xdspMax; m_xdspMax_save=xdspMax_save; m_bKinsok=bKinsok;
		m_bCRLFBra=bCRLFBra, m_bKinFirst=bKinFirst, m_bKinLast=bKinLast;
		m_bWordWrap=bWordWrap; m_nTabStops=nTabStops; }
	~CFormatChars() {};
protected:
	bool EntrySell(BYTE* &pch, BYTE &ch, BYTE &ch2, SELL &sell, OBJOFF &off);
	bool EntryLastSell(SELL &sell, OBJOFF &off);
	void Push_ReadOffset(OBJOFF off) {
		m_off_read=off; }
	OBJOFF Pop_ReadOffset() {
		return m_off_read; }
	void Initialize(OBJOFF off_first, OBJOFF line);
	OBJOFF GetKeta() {
		return m_keta; }
	OBJOFF GetSaveKeta() {
		return m_keta_save; }
private:
	OBJOFF m_keta;
	OBJOFF m_keta_save;
	OBJOFF m_off_first;
	OBJOFF m_off_read;
	bool m_bKinBra;
	OBJOFF m_line;
	bool   m_bline;
	int	m_xdspMax;
	int	m_xdspMax_save;
	bool m_bKinsok;
	bool	m_bCRLFBra;
	bool	m_bKinFirst;
	bool	m_bKinLast;
	bool m_bWordWrap;
	OBJOFF	 m_offWordWrap;
	int	 m_chTypeWordWrap;
	int m_nTabStops;
	OBJOFF	CalcTabstops(OBJOFF keta) {
		assert(m_nTabStops>0);
		return CALC_TABSTOPS(keta,m_nTabStops); }
};
typedef struct {
	char *title;
	bool bstate;
	int index;
}DSP_COLORS;
typedef struct {
	COLORREF color;
	bool	bEnable;
}ENV_COLORS;
//データ格納順
enum {
	idc_text,
	idc_textBk,
	idc_textBk_UnAct,
	idc_lineNum,
	idc_lineNum_Line,
	idc_tab,
	idc_CRLF,
	idc_EOF,
	idc_Url,	//http://に続くsingle bytes Alpha,num,symbol
	idc_Mail,	//a@b  single bytes Alpha, num, '.'
	//idc_Caret,
	idc_max
};

enum {
};

enum {
	kin_level0,
	kin_level1,
	kin_level2,
	kin_levelC,
};
extern char *stcpColorTitles[];
void InitialHorzFont(LOGFONT* plf);

class CCharsPrintAttr;
class CViewEnv
{
	friend class CViewEnvTbl;
	friend class CViewEd;
	friend class CObjCashFile;
	friend class CTextCashTbl;
	friend class CLineHint;
	friend class CObjLayTbl;
	friend class CWinView;
	friend class CDispPage;
	friend class CFilePage;
	friend class CFormatPage;
	friend class CFormatCharPage;
	friend class CConfigSheet;
	friend class CEnvEdit;
	friend class CConfigEdit;
	friend class CCharsPrintAttr;
private:
	CViewEnv() {};
public:
	CViewEnv(const char *szProfile, const char *szExtents=null, const char *szProfileDirDir=null);
	~CViewEnv();
protected:
	const int AttachCount(int i=0) {
		return (m_countAttach +=i); }
	int GetAttachCount() {
		return m_countAttach; }
	bool ReadWriteProfile(int mode, bool bCreate=false, bool bUpdate_Rock=false);
	//format support functions.
	CFormatChars *NewFormatChars() {
		return new CFormatChars(m_xdspMax, m_xdspMax_save, m_bKinsok,
			m_bCRLFBra, m_bKinFirst, m_bKinLast, m_bWordWrap, m_nTabStops); }
	void SetFocusEnv();
	void SetProfile_Extents(const char *szProfile, const char *szExtents, const char *szProfileDirDir=null);
	void ResetArc();
	void Initialize(const char *szProfile, const char *szExtents=null,
		const char *szProfileDirDir=null, bool bFirst=false);
	CViewEnv *CopyNew();
	bool &BShowLineNum();
	bool IsDirty() {
		return m_bDirty; }
	void SetDirty() {
		m_bDirty = true; }
private:
	void ReadWriteProfileStrings(int mode, string *pstr, int max);
	void ReadWriteProfileLogFont(int mode, LOGFONT* plf);
	int	m_countAttach;
	CProfile m_profile;
	int m_maxColors;
	bool m_bDirty;
public:
	enum {
		BACKFILE_NO,		//バックアップファイルを作成しない
		BACKFILE_BAK,		//同一フォルダーに"*.BAK"ファイル作成
		BACKFILE_FOLDER,	//所定のホルダに作成
		BACKFILE_TRASH,		//ごみ箱へファイル作成
	};

	bool	m_bRockUpdate;
	int	m_iBackFile;
	string m_strBackFolder;

	string m_strExtents;
	bool	m_bEditMenu_atOpen;
	bool	m_bGird_atEditMenu;
	int	m_xdspMax; //SHRT_MAX == 折り返さない
	int	m_xdspMax_save; //SHRT_MAX のときに、桁数を保存する為。
	bool m_bWordWrap;
	bool m_bKinsok;
	bool	m_bCRLFBra;
	bool	m_bKinFirst;
	bool	m_bKinLast;
	int	m_iKinLevel;
	string m_strKinFirst;
	string m_strKinLast;

	int m_nTabStops;
	bool m_bFreeCursor;
	LOGFONT m_lf;
	LOGFONT m_lfVert;
	bool m_bVert;

//	bool	m_bLineNum;
	bool	m_bRowNum;
	ENV_COLORS m_envColors[idc_max];
	bool	m_bAutoIndent;
	int		m_ratioBetweenLine;	
//	bool	m_bOpenURL;
//	bool	m_bOpenMail;
//	bool	m_bDblClkURL;
	//format support functions.
	CKeywords	m_keywords;
	CKeywords	*m_pkeywords_word;	//Enableのものだけ
	CKeywords	*m_pkeywords_unword;	//Enableのものだけ
};

#ifdef SPEC_PRINT
class CPrintEnv
{
public:
	CPrintEnv();
	~CPrintEnv() {};
public:
	void ReadIniProfile();
	void ReadWriteProfile(int mode);
	bool IsExistHeader();
	bool IsExistFooter();
	void PrintHeader(CDC *pDC, LPRECT prc);
	void PrintFooter(CDC *pDC, LPRECT prc);
	void ResetPrintEnvData(int page_max, const char *szFileName);
	void ResetPrintEnvData_Page(int page) {
		m_page = page; }
public:
	CRect	m_rcMargin;
	string	m_strHeaderLeft;
	string	m_strHeaderRight;
	string	m_strHeaderCenter;
	string	m_strFooterRight;
	string	m_strFooterLeft;
	string	m_strFooterCenter;
	int		m_iLineNum;
	bool	m_bColor;
	bool	m_bHeaderLine;
	bool	m_bFooterLine;
private:
	bool ConvKeyStrings(string &szDes, const string &szKeys);
	string ConvKeyStr(int index);
	string	m_strHeaderL;
	string	m_strHeaderR;
	string	m_strHeaderC;
	string	m_strFooterR;
	string	m_strFooterL;
	string	m_strFooterC;
	int		m_page;
	int		m_page_max;
	string	m_strfilename;
	struct _stat m_filestat;
	bool	m_bfilestat;
};
#endif

class CObjTbl;
class CViewEnvTbl:protected map<string, CViewEnv *>
{
	friend class CWinApplication;
	friend class CObjTbl;
	friend class CConfigEdit;
	friend class CEnvEdit;
	friend class CMainFrame;
	friend class CTheView;
public:
	CViewEnvTbl();
	~CViewEnvTbl();

	bool	m_bRockUpdate;
	bool	m_bEditMenu;
	bool	m_bFileSelecter;
	bool	m_bTopMost;
	string	m_strRef;
	bool	m_bEditMarkView;
	bool	m_bEditToolBarView;
	CRect	m_rcClient;
	CRect	m_rcFileSelectBar;
	CRect	m_rcEditToolBar;
	bool	m_bFloat_FileSelectBar;
	bool	m_bFloat_EditToolBar;
#ifdef SHAR
	CTanbuns	m_tanbuns;
#endif
#ifdef SPEC_PRINT
	CPrintEnv	m_printenv;
#endif //SPEC_PRINT
	CDateTimes	m_datetimes;
protected:
	void ReadIniProfile();
	void ReadWriteProfile(int mode);
	CViewEnv *GetViewEnv_byExtent(const char *szfilename=null);
	CViewEnv *GetViewEnv_byEnvIndex(int index);
	int		GetEnvIndex(CViewEnv *pEnv);
	CViewEnv *WriteOneProfile(const char *szName, bool bAlwaysUpdate = true);
	void WriteEnvEntryOnly();
private:
	LOGFONT m_lfPrint;
};
extern CViewEnvTbl theViewEnvTbl;
/////////////////////////////////////////////////

#endif //__VIEWENV_H__