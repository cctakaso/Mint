///////////////////////////////////////////////
//ViewEnv.cpp

#include "StdAfx.h"
#include "version.h"
#include "The.h"
#include "ViewEnv.h"
#include "Profile.h"
#include "chara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CKinsokTbl vKinsokTbl;
extern CTheApp theApp;
CViewEnvTbl theViewEnvTbl;
///////////////////////////////////////////////
static void InitialVertFont(LOGFONT* plf);
static void ReadWriteProfileLogFont(int mode, const char *szKey, LOGFONT* plf, const char *szAppendKey=null);

static char	 stcGeneral[] = "General";
static char	 stcbTopMost[] = "TopMost";
static char	 stcRcClient[] = "RcClient";
#ifdef SPEC_PRINT
static char	 stcRcPrintMargin[] ="RcPrintMargin";
static char	 stcHeaderLeft[] ="HeaderLeft";
static char	 stcHeaderCenter[] ="HeaderCenter";
static char	 stcHeaderRight[] ="HeaderRight";
static char	 stcFooterLeft[] ="FooterLeft";
static char	 stcFooterCenter[] ="FooterCenter";
static char	 stcFooterRight[] ="FooterRight";
static char  stcPrintLineNum[] = "PrintLineNum";
static char  stcPrintColor[] = "PrintColor";
static char  stcPrintHeaderLine[] = "PrintHeaderLine";
static char  stcPrintFooterLine[] = "PrintFooterLine";
#endif //SPEC_PRINT

static char	 stcRef[] = "StrRef";

static char  stcPrintFont[] = "PrintFont";

//static char  stcxdspMax[] = "xdspMax";
//static char  stcxdspMax_save[] = "xdspMax_save";
//static char  stcbKinsok[] = "bKinsok";
static char  stciKinLevel[] = "iKinLevel";
static char  stcKinsokFirst[] = "KinsokFirst";
static char  stcKinsokLast[] = "KinsokLast";
static char  stcbCRLFBra[] = "bCRLFBra";
static char  stcbKinFirst[] = "bKinFirst";
static char  stcbKinLast[] = "bKinLast";

static char  stcbWordWrap[] = "bWordWrap";
static char  stcSettings[] = "Settings";
static char  stcTabStops[] = "TabStops";

//static char  stcFont[] = "Font";
static char  stcAppendFontVert[] = "_V";
static char  stcHeight[] = "Height";
static char  stcWeight[] = "Weight";
static char  stcItalic[] = "Italic";
static char  stcUnderline[] = "Underline";
static char  stcPitchAndFamily[] = "PitchAndFamily";
static char  stcCharSet[] = "CharSet";
static char  stcFaceName[] = "FaceName";
static char  stcSystem[] = "System";

static char  stcbFreeCursor[] = "FreeCursor";
static char  stcbLineNum[] = "LineNum";
static char  stcbRowNum[] = "RowNum";
static char  stcbVert[] = "Vertical";
static char	 stcBackFile[] = "BackFile";
static char	 stcBackFolder[] = "BackFolder";
static char	 stcColorText[] = "ColorText";
static char	 stcColorTextBk[] = "ColorTextBk";
static char	 stcColorLineNum[] = "ColorLineNum";
static char	 stcColorEOF[] = "ColorEOF";
static char	 stcColorMeta[] = "ColorMeta";
static char	 stcColorCaret[] = "ColorCaret";
//static char	 stcbEditMenu_atOpen[] = "EditMenuatOpen";
//static char	 stcbGird_atEditMenu[] = "GardAtEditMenu";
static char	 stcbAutoIndent[] = "AutoIndent";
static char	 stcRatioBetweenLine[] = "RatioBetweenLine";


void InitialHorzFont(LOGFONT* plf)
{
   ::GetObject(GetStockObject(OEM_FIXED_FONT), sizeof(LOGFONT), plf);
	//memset(plf, 0, sizeof(LOGFONT));
	//plf->lfCharSet = DEFAULT_CHARSET;
	plf->lfHeight = min(18,plf->lfHeight*0.9);
	strcpy(plf->lfFaceName, "ＭＳ ゴシック"); //ＭＳ Ｐ明朝
	//strcpy(plf->lfFaceName, "Terminal"); //stcSystem
	plf->lfWidth=0; //for scaling font zoom!
	plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
}
static void InitialVertFont(LOGFONT* plf)
{
   ::GetObject(GetStockObject(OEM_FIXED_FONT), sizeof(LOGFONT), plf);
	//memset(plf, 0, sizeof(LOGFONT));
	//plf->lfCharSet = DEFAULT_CHARSET;
	plf->lfHeight = min(18,plf->lfHeight*0.9);
	strcpy(plf->lfFaceName, "ＭＳ 明朝"); //ＭＳ Ｐ明朝
	//strcpy(plf->lfFaceName, "Terminal"); //stcSystem
	plf->lfWidth=0; //for scaling font zoom!
	plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
}
#define APPEND_KEYSZ(a,b)	(IsValidDataSz(b) ? ((string(a)+b).c_str()):a)
static void ReadWriteProfileLogFont(int mode, const char *szKey, LOGFONT* plf, const char *szAppendKey)
{
	if (mode==CProfile::__read) {
		plf->lfHeight = theProfile.ReadInt(szKey, APPEND_KEYSZ(stcHeight,szAppendKey), plf->lfHeight);
		plf->lfWeight = theProfile.ReadInt(szKey, APPEND_KEYSZ(stcWeight,szAppendKey), plf->lfWeight);
		plf->lfItalic = (BYTE)theProfile.ReadInt(szKey, APPEND_KEYSZ(stcItalic,szAppendKey), plf->lfItalic);
		plf->lfUnderline = (BYTE)theProfile.ReadInt(szKey, APPEND_KEYSZ(stcUnderline,szAppendKey), plf->lfUnderline);
		plf->lfPitchAndFamily = (BYTE)theProfile.ReadInt(szKey, APPEND_KEYSZ(stcPitchAndFamily,szAppendKey), plf->lfPitchAndFamily);
		plf->lfCharSet = (BYTE)theProfile.ReadInt(szKey, APPEND_KEYSZ(stcCharSet,szAppendKey), plf->lfCharSet);
		string strFont = theProfile.ReadString(szKey, APPEND_KEYSZ(stcFaceName,szAppendKey), plf->lfFaceName);
		strcpy(plf->lfFaceName, strFont.c_str());
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}else{
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcHeight,szAppendKey), plf->lfHeight);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcHeight,szAppendKey), plf->lfHeight);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcWeight,szAppendKey), plf->lfWeight);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcItalic,szAppendKey), plf->lfItalic);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcUnderline,szAppendKey), plf->lfUnderline);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcPitchAndFamily,szAppendKey), plf->lfPitchAndFamily);
		theProfile.WriteInt(szKey, APPEND_KEYSZ(stcCharSet,szAppendKey), plf->lfCharSet);
		theProfile.WriteString(szKey, APPEND_KEYSZ(stcFaceName,szAppendKey), (LPCTSTR)plf->lfFaceName);
	}
}
///////////////////////////////////////////////
/*static char stcColor[] = "Color"; */
static COLORREF stcppEnvColorInit[][idc_max] = 
{
	//default
	{
	RGB(0,0,0),
	RGB(255,255,255),
	RGB(192,192,192),
	RGB(0,128,128),
	RGB(0,0,128),
	RGB(0,128,0),
	RGB(0,128,0),
	RGB(128,128,0),
	RGB(0,0,255),
	RGB(128,0,128),
	},
};
//表示順
DSP_COLORS dsp_colors[idc_max] = {
	{"テキスト",			false, idc_text},
	{"背景(ｱｸﾃｨﾌﾞ)",		false, idc_textBk},
	{"背景(ｲﾝｱｸﾃｨﾌﾞ)",		false, idc_textBk_UnAct},
	{"行番号",				true, idc_lineNum},
	{"行番号ライン",		true,  idc_lineNum_Line},
	{"タブ記号",			true,  idc_tab},
	{"改行記号",			true,  idc_CRLF},
	{"文末記号(EOF)",		true,  idc_EOF},
	{"ｲﾝﾀｰﾈｯﾄ･ｱﾄﾞﾚｽ(URL)",	true,  idc_Url},
	{"ﾒｰﾙ･ｱﾄﾞﾚｽ",			true,  idc_Mail},
};
#define APPEND_KEYSTR(a,b)	(string(a)+b.c_str()).c_str()
static void InitColorref(ENV_COLORS *penvcolor, const COLORREF *psrc, int max)
{
	for (int i=0; i<max; i++) {
		penvcolor[i].color = *psrc++;
	}
}

/* not use function!
static void ReadWriteProfileStrings(int mode, const char *szKey, string *pstr, int max)
{
	if (mode==CProfile::__read) {
		for (int i=0; i<max; i++) {
			pstr[i]  = theProfile.ReadString(szKey, APPEND_KEYSTR(stcColor,itostring(i)), pstr[i].c_str());
		}
	}else{
		for (int i=0; i<max; i++) {
			theProfile.WriteString(szKey, APPEND_KEYSTR(stcColor,itostring(i)), (LPCTSTR)pstr[i].c_str());
		}
	}
}
*/
///////////////////////////////////////////////
char *stcpKinStrs[] = 
{
	//Level0 - first,last
	"!%,.:;?、。，．｡､",
	"（［｛「『([{｢",
	//Level1 - first,last
	"!%),.:;?]}￠°’”‰′″℃、。々〉》」』】〕゛゜ゝゞ・ヽヾ！％），．：；？］｝｡｣､･ﾞﾟ￠",
	"$([{￡\\‘“〈《「『【〔＄（［｛｢￡￥",
	//Level2 - first,last
	"!%),.:;?]}￠°’”‰′″℃、。々〉》」』】〕ぁぃぅぇぉっゃゅょゎ゛゜ゝゞァィゥェォッャュョヮヵヶ・ーヽヾ！％），．：；？］｝｡｣､･ｧｨｩｪｫｬｭｮｯｰﾞﾟ￠",
	"$([{￡\\‘“〈《「『【〔＄（［｛｢￡￥",
	//"$([\{￡\‘“〈《「『【〔＄（［｛｢￡￥",
};

///////////////////////////////////////////////
///////////////////////////////////////////////
//CViewEnv class
CViewEnv::CViewEnv(const char *szProfile, const char *szExtents,  const char *szProfileDirDir)
{
	m_bDirty =  false;
	Initialize(szProfile, szExtents,  szProfileDirDir, true);
}

CViewEnv::~CViewEnv()
{
	if (m_pkeywords_word)
		delete m_pkeywords_word;
	if (m_pkeywords_unword)
		delete m_pkeywords_unword;
}
CViewEnv *CViewEnv::CopyNew()
{
//	m_profile.ResetArc();
	CViewEnv *pEnv = new CViewEnv(m_profile.GetSzProfileNameOnly(), m_strExtents.data(), 
		GetModuleDirDir(theApp.m_hInstance).data());
	pEnv->ReadWriteProfile(CProfile::__read);
	return pEnv;
}

void CViewEnv::Initialize(const char *szProfile, const char *szExtents, const char *szProfileDirDir, bool bFirst)
{
	if (!bFirst) {
		if (m_pkeywords_word)
			delete m_pkeywords_word;
		if (m_pkeywords_unword)
			delete m_pkeywords_unword;
	}
	
	SetProfile_Extents(szProfile, szExtents, szProfileDirDir);

	m_bRockUpdate = false;
	m_bEditMenu_atOpen = false;
	m_bGird_atEditMenu = false;
	m_countAttach = 0;
	m_xdspMax = 76;
	m_xdspMax_save = 76;
	m_bKinsok = true;
	m_bKinFirst = true;
	m_bKinLast = true;
	m_bCRLFBra = true;
	m_iKinLevel = kin_level0;
	m_strKinFirst = stcpKinStrs[m_iKinLevel*2];
	m_strKinLast = stcpKinStrs[m_iKinLevel*2+1];
	m_bWordWrap = true;
	m_nTabStops = 4;
	m_bFreeCursor = false;
	InitialHorzFont(&m_lf);
	InitialVertFont(&m_lfVert);
	//m_bLineNum = true;//->m_envColors[idc_lineNum].bEnable
	m_bRowNum = false;
	m_bVert = false;
	m_iBackFile = BACKFILE_FOLDER;
	m_strBackFolder = GetModuleDirDir(AfxGetInstanceHandle()) + "back\\";
	m_maxColors = idc_max;
	for (int i=0; i<m_maxColors; i++) {
		m_envColors[i].bEnable = true;
	}
	m_envColors[idc_lineNum_Line].bEnable = false;
	InitColorref(m_envColors, stcppEnvColorInit[0], m_maxColors);
	m_bAutoIndent = false;
	m_ratioBetweenLine = 10;
	m_pkeywords_word = null;
	m_pkeywords_unword = null;
}

bool &CViewEnv::BShowLineNum()
{
	return m_envColors[idc_lineNum].bEnable;
}

void CViewEnv::SetFocusEnv()
{
	vKinsokTbl.ResetKinsok(m_bKinsok, m_bKinFirst, m_strKinFirst,
		m_bKinLast, m_strKinLast);
}

void CViewEnv::SetProfile_Extents(const char *szProfile, const char *szExtents, const char *szProfileDirDir)
{
	if (IsValidDataSz(szExtents))
		m_strExtents = szExtents;
	else
		m_strExtents = "";

	VERIFY(m_profile.SetProfile(szProfile, 
		(IsValidDataSz(szProfileDirDir) ? szProfileDirDir:GetModuleDirDir(theApp.m_hInstance).data()),
		ENVFILE_EXTENT));
}

bool CViewEnv::ReadWriteProfile(int mode, bool bCreate, bool bUpdate_Rock)
{
	assert(mode == CProfile::__read || IsDirty()); //ライトはダーティーの時だけ！
	static string strcheckdoc = SZCHKENVMARK;

	if (mode == CProfile::__write && m_bRockUpdate && !bUpdate_Rock)
		return true; //updateしない

	if (!m_profile.ResetArcMode(mode, bCreate)) {
		return false;
	}
	//if (bCreate && mode==CProfile::__read)
	//	return true; //デフォルトの値をそのまま使う
	try {
	m_profile.ReadWriteString(mode, strcheckdoc);
	if (mode == CProfile::__read && strcheckdoc != SZCHKENVMARK) { //strcmp(strcheckdoc.c_str(), SZCHKENVMARK)==0
		CString cstrMsg;
		cstrMsg.Format("以下の設定ファイルはこのアプリケーションの設定ファイルではない為、" \
			"設定が不完全です。\n%s\n既存の設定ファイルを用意する場合は、" \
			"必ず本アプリケーションを終了した後コピーして下さい。", m_profile.GetSzProfile());
		AfxMessageBox(cstrMsg);
	 return true; //!HANK!
	}

	int iDocVersion = DOCVERSION;
	m_profile.ReadWriteInt(mode, iDocVersion);
	if (mode == CProfile::__read && iDocVersion > DOCVERSION) {
		CString cstrMsg;
		cstrMsg.Format("以下の設定ファイルは本体よりバージョンが新しい為設定が不完全です。\n%s\n既存の設定ファイルを用意する場合は、" \
			"必ず本アプリケーションを終了した後コピーして下さい。", m_profile.GetSzProfile());
		AfxMessageBox(cstrMsg);
		 return true; //!HANK!
	}
	m_profile.ReadWriteBool(mode, m_bRockUpdate);
	m_profile.ReadWriteString(mode, m_strExtents);
	m_profile.ReadWriteBool(mode, m_bEditMenu_atOpen);
	m_profile.ReadWriteBool(mode, m_bGird_atEditMenu);
	m_profile.ReadWriteInt(mode, m_xdspMax);
	if (m_xdspMax<20)
		m_xdspMax = 20;
	else if (m_xdspMax != SHRT_MAX && m_xdspMax>256)
		m_xdspMax = 256;
	m_profile.ReadWriteInt(mode, m_xdspMax_save);
	if (m_xdspMax_save<20)
		m_xdspMax_save = 20;
	else if (m_xdspMax_save>256)
		m_xdspMax_save = 256;
	m_profile.ReadWriteBool(mode, m_bKinsok);
	m_profile.ReadWriteBool(mode, m_bKinFirst);
	m_profile.ReadWriteBool(mode, m_bKinLast);
	m_profile.ReadWriteBool(mode, m_bCRLFBra);
	m_profile.ReadWriteInt(mode, m_iKinLevel);
	m_profile.ReadWriteString(mode, m_strKinFirst);
	m_profile.ReadWriteString(mode, m_strKinLast);
	m_profile.ReadWriteBool(mode, m_bWordWrap);
	m_profile.ReadWriteInt(mode, m_nTabStops);
	m_profile.ReadWriteBool(mode, m_bFreeCursor);
//	m_profile.ReadWriteBool(mode, m_bLineNum);
	m_profile.ReadWriteBool(mode, m_bRowNum);
	m_profile.ReadWriteBool(mode, m_bVert);
	m_profile.ReadWriteInt(mode, m_iBackFile);
	m_profile.ReadWriteString(mode, m_strBackFolder);
	if (mode==CProfile::__read && 
		(m_strBackFolder.size()==0 || (m_strBackFolder.size()==1 && m_strBackFolder[0]=='?'))) {
		m_strBackFolder = GetModuleDirDir(AfxGetInstanceHandle()) + "back\\";
	}
	m_profile.ReadWriteBool(mode, m_bAutoIndent);
#ifdef FREE
	m_bAutoIndent = false;
#endif
	m_profile.ReadWriteInt(mode, m_ratioBetweenLine);
	if (m_iKinLevel<0) {
		m_strKinFirst = "";
		m_strKinLast = "";
	}else if (m_iKinLevel<kin_levelC) {
		m_strKinFirst = stcpKinStrs[m_iKinLevel*2];
		m_strKinLast = stcpKinStrs[m_iKinLevel*2+1];
	}
	m_profile.ReadWriteInt(mode, m_maxColors);
	if (m_maxColors > idc_max)
		m_maxColors = idc_max;
	
	m_profile.ReadWriteBytes(mode, m_envColors, sizeof(m_envColors));
	m_maxColors = idc_max;

	ReadWriteProfileLogFont(mode, &m_lf);
	ReadWriteProfileLogFont(mode, &m_lfVert);


	m_keywords.ReadWriteProfile(mode, m_profile);
#ifdef SHAR
	if (mode==CProfile::__read) {
		m_keywords.Copy_OnlyEnable(m_pkeywords_word, m_pkeywords_unword);
	}
#endif	//#ifdef SHAR
	}catch(CException *e) {
		e->Delete();
		m_profile.ResetArc();
		return false;
	}
	m_profile.ResetArc();
	m_bDirty = false;
	return true;
}

void CViewEnv::ReadWriteProfileStrings(int mode, string *pstr, int max)
{
	for (int i=0; i<max; i++) {
		m_profile.ReadWriteString(mode, pstr[i]);
	}
}
void CViewEnv::ReadWriteProfileLogFont(int mode, LOGFONT* plf)
{
	m_profile.ReadWriteBytes(mode, plf, sizeof(LOGFONT));
}

#ifdef SPEC_PRINT
///////////////////////////////////////////////
//CPrintEnv class
CPrintEnv::CPrintEnv()
{
	m_rcMargin.SetRectEmpty();
	m_rcMargin.bottom = -1;
	m_iLineNum = 0;
	m_bColor = true;
	m_bHeaderLine = false;
	m_bFooterLine = true;
	m_page_max = 0;
	m_page = 0;
	m_bfilestat = false;
}
void CPrintEnv::ReadIniProfile()
{
	//[General]-->
	string str;
	str = theProfile.ReadString(stcGeneral, stcRcPrintMargin, IntArray2PartString((int *)&m_rcMargin, 4).c_str());
	PartString2IntArray(str, (int *)&m_rcMargin, 4);
	m_strHeaderLeft = theProfile.ReadString(stcGeneral, stcHeaderLeft, m_strHeaderLeft.c_str());
	m_strHeaderCenter = theProfile.ReadString(stcGeneral, stcHeaderCenter, m_strHeaderCenter.c_str());
	m_strHeaderRight = theProfile.ReadString(stcGeneral, stcHeaderRight, m_strHeaderRight.c_str());
	m_strFooterLeft = theProfile.ReadString(stcGeneral, stcFooterLeft, m_strFooterLeft.c_str());
	m_strFooterCenter = theProfile.ReadString(stcGeneral, stcFooterCenter, m_strFooterCenter.c_str());
	m_strFooterRight = theProfile.ReadString(stcGeneral, stcFooterRight, m_strFooterRight.c_str());
	if (m_rcMargin.bottom<0) {
		m_rcMargin.SetRect(20, 20, 20, 20);
		m_strFooterCenter = "([ﾍﾟｰｼﾞ]/[総ﾍﾟｰｼﾞ])";
		m_strFooterRight = "[現在日付]";
	}
	m_iLineNum = theProfile.ReadInt(stcGeneral, stcPrintLineNum, m_iLineNum);
#ifdef SHAR
	m_bColor = theProfile.ReadBool(stcGeneral, stcPrintColor, m_bColor);
#else
	m_bColor = false;
#endif
	m_bHeaderLine = theProfile.ReadBool(stcGeneral, stcPrintHeaderLine, m_bHeaderLine);
	m_bFooterLine = theProfile.ReadBool(stcGeneral, stcPrintFooterLine, m_bFooterLine);
}
void CPrintEnv::ReadWriteProfile(int mode)
{
	if (mode==CProfile::__read) {
	}else{
		//[General]-->
		theProfile.WriteString(stcGeneral, stcRcPrintMargin, IntArray2PartString((int *)&m_rcMargin, 4).c_str());
		theProfile.WriteString(stcGeneral, stcHeaderLeft, m_strHeaderLeft.c_str());
		theProfile.WriteString(stcGeneral, stcHeaderCenter, m_strHeaderCenter.c_str());
		theProfile.WriteString(stcGeneral, stcHeaderRight, m_strHeaderRight.c_str());
		theProfile.WriteString(stcGeneral, stcFooterLeft, m_strFooterLeft.c_str());
		theProfile.WriteString(stcGeneral, stcFooterCenter, m_strFooterCenter.c_str());
		theProfile.WriteString(stcGeneral, stcFooterRight, m_strFooterRight.c_str());
		theProfile.WriteInt(stcGeneral, stcPrintLineNum, m_iLineNum);
#ifdef SHAR
		theProfile.WriteBool(stcGeneral, stcPrintColor, m_bColor);
#endif
		theProfile.WriteBool(stcGeneral, stcPrintHeaderLine, m_bHeaderLine);
		theProfile.WriteBool(stcGeneral, stcPrintFooterLine, m_bFooterLine);
		//<----General
	}
}

char *stcPrintKey[] = {
"[ﾍﾟｰｼﾞ]",
"[総ﾍﾟｰｼﾞ]",
"[現在日付]",
"[現在日時]",
"[ﾌｧｲﾙ名]",
"[ﾌｧｲﾙｻｲｽﾞ]",
"[更新日付]",
"[更新日時]",
"[ﾌｫﾙﾀﾞ名]"
};


void CPrintEnv::ResetPrintEnvData(int page_max, const char *szFileName)
{
	ASSERT(page_max>0);
	m_page_max = page_max;
	m_page = 0;
	m_strHeaderL = "";
	m_strHeaderC = "";
	m_strHeaderR = "";
	m_strFooterL = "";
	m_strFooterC = "";
	m_strFooterR = "";
	m_bfilestat = false;
	m_strfilename = szFileName;
	if (IsExistFile(szFileName)) {
		m_bfilestat = GetFileStat(szFileName, m_filestat);
	}
}

bool CPrintEnv::IsExistHeader()
{
	return m_strHeaderLeft!="" || m_strHeaderCenter!="" || m_strHeaderRight!="";
}
bool CPrintEnv::IsExistFooter()
{
	return m_strFooterLeft!="" || m_strFooterCenter!="" || m_strFooterRight!="";
}
void CPrintEnv::PrintHeader(CDC *pDC, LPRECT prc)
{
	if (m_strHeaderLeft!="") {
		ConvKeyStrings(m_strHeaderL, m_strHeaderLeft);
		pDC->TextOut(prc->left, prc->top, m_strHeaderL.c_str());
	}
	if (m_strHeaderCenter!="") {
		ConvKeyStrings(m_strHeaderC, m_strHeaderCenter);
		pDC->DrawText(m_strHeaderC.c_str(), prc, DT_CENTER |DT_NOCLIP | DT_TOP | DT_SINGLELINE);
	}
	if (m_strHeaderRight!="") {
		ConvKeyStrings(m_strHeaderR, m_strHeaderRight);
		pDC->DrawText(m_strHeaderR.c_str(), prc, DT_RIGHT |DT_NOCLIP | DT_TOP | DT_SINGLELINE);
	}
}
void CPrintEnv::PrintFooter(CDC *pDC, LPRECT prc)
{
	if (m_strFooterLeft!="") {
		ConvKeyStrings(m_strFooterL, m_strFooterLeft);
		pDC->TextOut(prc->left, prc->top, m_strFooterL.c_str());
	}
	if (m_strFooterCenter!="") {
		ConvKeyStrings(m_strFooterC, m_strFooterCenter);
		pDC->DrawText(m_strFooterC.c_str(), prc, DT_CENTER |DT_NOCLIP | DT_TOP | DT_SINGLELINE);
	}
	if (m_strFooterRight!="") {
		ConvKeyStrings(m_strFooterR, m_strFooterRight);
		pDC->DrawText(m_strFooterR.c_str(), prc, DT_RIGHT |DT_NOCLIP | DT_TOP | DT_SINGLELINE);
	}
}

bool CPrintEnv::ConvKeyStrings(string &strDes, const string &strSrc)
{
	strDes = "";
	for (int i=0, len = strSrc.size(); len>0; i++, len--) {
		for (int k=0; k<sizeof(stcPrintKey)/sizeof(char*); k++) {
			char *szKey = stcPrintKey[k];
			int lenKey = strlen(szKey);
			if (len>=lenKey && memcmp(szKey, &strSrc[i], lenKey)==0) {
				strDes += ConvKeyStr(k);
				i += lenKey;
				len -= lenKey;
			}
		}
		BYTE ch = strSrc[i];
		strDes += ch;
		if (IsSJIS_1stByte(ch)) {
			strDes += strSrc[++i];
			len--;
		}
	}
	return true;
}
string CPrintEnv::ConvKeyStr(int index)
{
	string str;
	switch(index) {
	case 0://"[ﾍﾟｰｼﾞ]",
		str = itostring(m_page);
		break;
	case 1://"[総ﾍﾟｰｼﾞ]",
		str = itostring(m_page_max);
		break;
	case 2://"[現在日付]",
		str = theViewEnvTbl.m_datetimes.GetTodayString_First();
		break;
	case 3://"[現在日時]",
		str = theViewEnvTbl.m_datetimes.GetTodayString_First();
		str += " ";
		str += theViewEnvTbl.m_datetimes.GetThisTimeString_First();
		break;
	case 4://"[ﾌｧｲﾙ名]",
		if (m_bfilestat) {
			str = GetFileName(m_strfilename);
		}
		break;
	case 5://[ﾌｧｲﾙｻｲｽﾞ]
		if (m_bfilestat) {
//#ifdef WIN32
//			str = 64tostring(m_filestat.st_size);
//#else
			str = itostring(m_filestat.st_size);
//#endif
		}
		break;
	case 6://[更新日付]
		if (m_bfilestat)
			str = theViewEnvTbl.m_datetimes.GetThedayString_First(m_filestat.st_mtime);
		break;
	case 7://[更新日時]
		if (m_bfilestat) {
			str = theViewEnvTbl.m_datetimes.GetThedayString_First(m_filestat.st_mtime);
			str += " ";
			str += theViewEnvTbl.m_datetimes.GetTheTimeString_First(m_filestat.st_mtime);
		}
		break;
	case 8://[ﾌｫﾙﾀﾞ名]
		if (m_bfilestat)
			str = GetFileDir(m_strfilename);
		break;
	}
	return str;
}
#endif //SPEC_PRINT all class

///////////////////////////////////////////////
//CViewEnvTbl class
CViewEnvTbl::CViewEnvTbl()
{
	InitialHorzFont(&m_lfPrint);
	m_bRockUpdate = false;
#ifdef FREE
	m_bEditMenu = false;
#else
	m_bEditMenu = true;
#endif
	m_bFileSelecter = true;
	m_bTopMost = false;
	m_strRef = ">";
	m_bEditMarkView = false;
	m_bEditToolBarView = false;
	m_bFloat_FileSelectBar = false;
	m_bFloat_EditToolBar = true;
	m_rcClient.SetRectEmpty();
	m_rcFileSelectBar.SetRectEmpty();
	m_rcEditToolBar.SetRectEmpty();
}
CViewEnvTbl::~CViewEnvTbl()
{
	for (iterator iter=begin(); iter!=end(); iter++)
		delete (CViewEnv *)(*iter).second;
}
CViewEnv *CViewEnvTbl::WriteOneProfile(const char *szName, bool bAlwaysUpdate)
{
	iterator iter;
	string str = szName;
	if ((iter=find(str))==end())
		return null;
	CViewEnv *pEnv = (*iter).second;
	if (bAlwaysUpdate || pEnv->IsDirty()) {
		pEnv->SetDirty(); //強制的にダーティにする
		if (!pEnv->ReadWriteProfile(CProfile::__write)) {
			CString cstr;
			cstr.Format("ドライブ領域が足りない、またはインストール先フォルダに書き込み権限が" \
				"ない等の理由により、以下の設定ファイルを更新できませんでした。\n%s", pEnv->m_profile.GetSzProfile());
			AfxMessageBox(cstr);
			return null;
		}
	}
//	pEnv->m_profile.ResetArc();
	return pEnv;
}
void CViewEnvTbl::WriteEnvEntryOnly()
{
	string strEntry;
	CViewEnv *pEnv;
	int i=0;
	for (iterator iter=begin(); iter!=end(); iter++) {
		strEntry = (*iter).first;
		if (strEntry.size()==0 || strEntry[0]=='!')
			continue;
		theProfile.WriteString("EnvEntrys",i++, strEntry.data()); //設定名のエントリー
		pEnv = (*iter).second;
		theProfile.WriteString("EnvFiles", strEntry.data(), pEnv->m_profile.GetSzProfileNameOnly()); //設定名のエントリー
	}
	//最後の設定名エントリーの次を空エントリーにしておく
	theProfile.WriteString("EnvEntrys",i, ""); //設定名のエントリー
}
void CViewEnvTbl::ReadIniProfile()
{
	//[General]-->
	string str;
	m_bRockUpdate = theProfile.ReadBool(stcGeneral, "bRockUpdate", m_bRockUpdate);
	m_bEditMenu = theProfile.ReadBool(stcGeneral, "bEditMenu", m_bEditMenu);
	m_bFileSelecter = theProfile.ReadBool(stcGeneral, "bFileSelecter", m_bFileSelecter);
	m_bTopMost = theProfile.ReadBool(stcGeneral, stcbTopMost, m_bTopMost);
	m_strRef = theProfile.ReadString(stcGeneral, stcRef, m_strRef.c_str());
	m_bEditMarkView = theProfile.ReadBool(stcGeneral, "bEditMarkView", m_bEditMarkView);
	m_bEditToolBarView = theProfile.ReadBool(stcGeneral, "bEditToolBarView", m_bEditToolBarView);
	if (m_rcClient.IsRectEmpty()) {
		str = theProfile.ReadString(stcGeneral, stcRcClient, IntArray2PartString((int *)&m_rcClient, 4).c_str());
		PartString2IntArray(str, (int *)&m_rcClient, 4);
	}
	str = theProfile.ReadString(stcGeneral, "rcFileSelectBar", IntArray2PartString((int *)&m_rcFileSelectBar, 4).c_str());
	PartString2IntArray(str, (int *)&m_rcFileSelectBar, 4);
	str = theProfile.ReadString(stcGeneral, "rcEditToolBar", IntArray2PartString((int *)&m_rcEditToolBar, 4).c_str());
	PartString2IntArray(str, (int *)&m_rcEditToolBar, 4);
	m_bFloat_FileSelectBar = theProfile.ReadBool(stcGeneral, "bFloat_FileSelectBar", m_bFloat_FileSelectBar);
	m_bFloat_EditToolBar = theProfile.ReadBool(stcGeneral, "bFloat_EditToolBar", m_bFloat_EditToolBar);
#ifdef SPEC_PRINT
	m_printenv.ReadIniProfile();
#endif //SPEC_PRINT
	//<----General
}
void CViewEnvTbl::ReadWriteProfile(int mode)
{
	string str, strEntry, strFile;
	CViewEnv *pEnv;
	iterator iter;

	if (mode==CProfile::__read) {

		//[Env]
		//0=Normal
		 //設定ファイルからのエントリー読み出し
#ifdef SHAR
		for (int i=0; (strEntry=theProfile.ReadString("EnvEntrys",i)).size()>0; i++) {
			iter=find(strEntry);
			if (iter==end()) {
				if ((strFile=theProfile.ReadString("EnvFiles", strEntry.data())).size()>0)
					pEnv = new CViewEnv(strFile.data());
				else
					break; //このエントリーに番号に相当する設定ファイルの指定がない
			}else
				pEnv = (*iter).second;
			assert(pEnv);
			if (!pEnv->ReadWriteProfile(mode)) {
				CString cstr;
				cstr.Format("以下の設定ファイルが見つからない為設定が不完全です。\n%s\n既存の設定ファイルを用意する場合は、" \
					"必ず本アプリケーションを終了した後コピーして下さい。", pEnv->m_profile.GetSzProfile());
				AfxMessageBox(cstr);
				//pEnv->SetDirty(); //一番最初の書き込み
				continue;
			}
			if (iter==end())
				(*this)[strEntry] = pEnv;
		}
		//少なくともNORMALは必ず有るようにする
		if (size()==0 || find(NORMAL_NAME)==end()) {
			pEnv = new CViewEnv(NORMAL_NAME);
			pEnv->ReadWriteProfile(mode, true);
			pEnv->SetDirty(); //一番最初の書き込み
			(*this)[NORMAL_NAME] = pEnv;
		}
#else
		strEntry = NORMAL_NAME;
		while(true){
			iter=find(strEntry);
			if (iter==end()) {
				if ((strFile=theProfile.ReadString("EnvFiles", strEntry.data())).size()>0)
					pEnv = new CViewEnv(strFile.data());
				else
					break; //このエントリーに番号に相当する設定ファイルの指定がない
			}else
				pEnv = (*iter).second;
			assert(pEnv);
			if (!pEnv->ReadWriteProfile(mode)) {
				CString cstr;
				cstr.Format("以下の設定ファイルが見つからない為設定が不完全です。\n%s\n既存の設定ファイルを用意する場合は、" \
					"必ず本アプリケーションを終了した後コピーして下さい。", pEnv->m_profile.GetSzProfile());
				AfxMessageBox(cstr);
				//pEnv->SetDirty(); //一番最初の書き込み
				continue;
			}
			if (iter==end())
				(*this)[strEntry] = pEnv;
			break; //always break!
		}
		//少なくともNORMALは必ず有るようにする
		if (size()==0 || find(NORMAL_NAME)==end()) {
			pEnv = new CViewEnv(NORMAL_NAME);
			pEnv->ReadWriteProfile(mode, true);
			pEnv->SetDirty(); //一番最初の書き込み
			(*this)[NORMAL_NAME] = pEnv;
		}
#endif
	}else{
		//[General]-->
		if (m_bRockUpdate)
			return;
//		theProfile.WriteBool(stcGeneral, stcbTopMost, m_bTopMost);
		theProfile.WriteBool(stcGeneral, "bEditMenu", m_bEditMenu);
		theProfile.WriteBool(stcGeneral, "bFileSelecter", m_bFileSelecter);
		theProfile.WriteBool(stcGeneral, "bEditMarkView", m_bEditMarkView);
		theProfile.WriteBool(stcGeneral, "bEditToolBarView", m_bEditToolBarView);
		theProfile.WriteString(stcGeneral, stcRcClient, IntArray2PartString((int *)&m_rcClient, 4).c_str());
		theProfile.WriteString(stcGeneral, "rcFileSelectBar", IntArray2PartString((int *)&m_rcFileSelectBar, 4).c_str());
		theProfile.WriteString(stcGeneral, "rcEditToolBar", IntArray2PartString((int *)&m_rcEditToolBar, 4).c_str());
		theProfile.WriteBool(stcGeneral, "bFloat_FileSelectBar", m_bFloat_FileSelectBar);
		theProfile.WriteBool(stcGeneral, "bFloat_EditToolBar", m_bFloat_EditToolBar);
//		theProfile.WriteString(stcGeneral, stcRef, m_strRef.c_str());
#ifdef SPEC_PRINT
		m_printenv.ReadWriteProfile(mode);
#endif //SPEC_PRINT
		//<----General

		//[Env]
		//0=Normal
#ifdef SHAR
		int i=0;
		for (iter=begin(); iter!=end(); iter++) {
			strEntry = (*iter).first;
			if (strEntry.size()==0 || strEntry[0]=='!')
				continue;
			theProfile.WriteString("EnvEntrys",i++, strEntry.data()); //設定名のエントリー
			pEnv = (*iter).second;
			theProfile.WriteString("EnvFiles", strEntry.data(), pEnv->m_profile.GetSzProfileNameOnly()); //設定名のエントリー
			if (pEnv->IsDirty() && !pEnv->ReadWriteProfile(mode)) {
				CString cstr;
				cstr.Format("ドライブ領域が足りない、またはインストール先フォルダに書き込み権限が" \
					"ない等の理由により、以下の設定ファイルを更新できませんでした。\n%s", pEnv->m_profile.GetSzProfile());
				AfxMessageBox(cstr);
			}
		}
		//最後の設定名エントリーの次を空エントリーにしておく
		theProfile.WriteString("EnvEntrys",i, ""); //設定名のエントリー
#else
		for (int i=0; (strEntry=theProfile.ReadString("EnvEntrys",i)).size()>0; i++) {
			if (strEntry==NORMAL_NAME)
				break;
		}
		{
			strEntry = NORMAL_NAME;
			iter=find(strEntry);
			strEntry = (*iter).first;
			//if (strEntry.size()==0 || strEntry[0]=='!')
			//	continue;
			theProfile.WriteString("EnvEntrys",i++, strEntry.data()); //設定名のエントリー
			pEnv = (*iter).second;
			theProfile.WriteString("EnvFiles", strEntry.data(), pEnv->m_profile.GetSzProfileNameOnly()); //設定名のエントリー
			if (pEnv->IsDirty() && !pEnv->ReadWriteProfile(mode)) {
				CString cstr;
				cstr.Format("ドライブ領域が足りない、またはインストール先フォルダに書き込み権限が" \
					"ない等の理由により、以下の設定ファイルを更新できませんでした。\n%s", pEnv->m_profile.GetSzProfile());
				AfxMessageBox(cstr);
			}
		}
#endif
	}

#ifdef SHAR
	//短文登録ﾌｧｲﾙの読み書き-->
	VERIFY(m_tanbuns.ReadWriteProfile(mode));
	//<----短文登録ﾌｧｲﾙの読み書き
#endif
	
	//[PrinterFont]-->
	ReadWriteProfileLogFont(mode, stcPrintFont, &m_lfPrint);
	//<----PrinterFont

	//日付時間のiniへの読み書き------>
	m_datetimes.ReadWriteProfile(mode);
	//<--------日付時間のiniへの読み書き
}

CViewEnv *CViewEnvTbl::GetViewEnv_byExtent(const char *szfilename)
{
	string strExtent = GetFileExtent(szfilename);
	for (iterator iter=begin(); iter!=end(); iter++) {
		string str, strPart;
		str = (*iter).second->m_strExtents;
		while((strPart = GetPartString(str, ' ')).size()>0) {
			if (strExtent == strPart)
				return (*iter).second;
		}
	}
	//NORMALを返す
	return (*this)[NORMAL_NAME];
}

CViewEnv *CViewEnvTbl::GetViewEnv_byEnvIndex(int index)
{
	if (index>=0) {//normal find
		int i= 0;
		for (iterator iter=begin(); iter!=end(); iter++) {
			if ((*iter).first != NORMAL_NAME) {
				if (i==index)
					return (*iter).second;
				i++;
			}
		}
	}
	return (*this)[NORMAL_NAME];
}

int	CViewEnvTbl::GetEnvIndex(CViewEnv *pEnv)
{
	int i= 0;
	for (iterator iter=begin(); iter!=end(); iter++) {
		if ((*iter).first != NORMAL_NAME) {
			if ((*iter).second == pEnv)
				return i;
			i++;
		}else if ((*iter).second == pEnv) {
			return -1;
		}
	}
	assert(false);
	return -2; //error
}

////////////////////////////////////////////////////////////////////////////
// 
bool CFormatChars::EntryLastSell(SELL &sell, OBJOFF &off)
{
	if (off>m_off_first) {
		sell.off = m_off_first;
		sell.len = off - m_off_first;
		if (m_bline) {
			sell.line = ++m_line;
			m_bline = false;
		}else
			sell.line = 0;	//initialize
		return true;
	}
	return false;
}

inline  void CheckSaveChType(int &chTypeSave, int &chType, OBJOFF &off, OBJOFF &offWordWrap) 
{
	bool bWhite = (chType & WHT)>0;
	if ((chType & KANJ)==0 && !bWhite && chTypeSave!=chType  && chTypeSave!=0) {//ホワイトが後に続くと同じとする
														//行頭のオフセットは入れない。
		offWordWrap = off;
	}
	chTypeSave = chType;
}
inline bool IsKinsokFirst(int chType, int chType2, BYTE ch, BYTE ch2)
{
	return IsKinBit(chType) && 
		(IsKinFirstSingle(chType) || IsKinFirstDouble(chType, chType2, ch, ch2)) ;
}
inline bool IsKinsokLast(int chType, int chType2, BYTE ch, BYTE ch2)
{
	return IsKinBit(chType) && 
		(IsKinLastSingle(chType) || IsKinLastDouble(chType, chType2, ch, ch2));
}

bool CFormatChars::EntrySell(BYTE* &pch, BYTE &ch, BYTE &ch2, SELL &sell, OBJOFF &off) 
{
	//1   2   3    4
	//    ^---pch
	//ch  ch2
	//        ^---pch
	//    ch  ch2
	//----------------
	//あ
	//1   2   3    4
	//    ^---pch
	//ch  ch2
	//        ^---pch
	//             ^---pch
	//        ch   ch2
	int chType = GetFullType(ch);
	int chType2 = GetFullType(ch2);
	int chtype_chk;
	BYTE ch2_save = ch2;
#ifdef _DBCS
	if (IsSJIS_1stByte_chType(chType) && IsSJIS_2ndByte_chType(chType2)) {
		m_chTypeWordWrap = KANJ;
		/*
		if (m_bWordWrap) {
			//スピードアップの為、漢字は空白文字以外全部同じタイプとする。
			chtype_chk = KANJ;
			if (ch == wchSpace1st && ch2 == wchSpace2nd) {
				chtype_chk |= WHT;//HACK！
			}
			CheckSaveChType(m_chTypeWordWrap, chtype_chk, off, m_offWordWrap);
			//if (!IsSameChType(m_chTypeWordWrap, chtype_chk)) {
			//	if (m_chTypeWordWrap!=0) //行頭のオフセットは入れない。
			//		m_offWordWrap = off;
			//	m_chTypeWordWrap = chtype_chk;
			//}
		}
		*/
		off++;
		m_keta+=2;
		ch2 = *(++pch);
	}else
#endif
	{
		//if (IsControl_chtype(chType)) {
		if (IsCRLF(ch)) {//if (ch==VK_RETURN) //
			if (IsCRLF(ch2) && ch!=ch2) {
				off++;
				ch2 = *(++pch);
			}
			m_keta_save = m_keta;
			m_keta = NPOS;
		}else{
			if (m_bWordWrap) {
				chtype_chk = AndSbcsChType(chType);
				CheckSaveChType(m_chTypeWordWrap, chtype_chk, off, m_offWordWrap);
				//if (!IsSameChType(m_chTypeWordWrap, chtype_chk)) {
				//	if (m_chTypeWordWrap!=0) //行頭のオフセットは入れない。
				//		m_offWordWrap = off;
				//	m_chTypeWordWrap = chtype_chk;
				//}
			}
			if (ch==VK_TAB)
				m_keta += CalcTabstops(m_keta);
			else
				m_keta++;
		}
	}
	if (m_keta>=m_xdspMax) {
		//ぶら下げをチェック
		//1.改行以外で、
		//2.既に行頭禁則ある場合以外で、
		//3.ぶら下げ改行、または、行頭禁則の場合
		if (m_keta==NPOS || m_bKinBra/*ぶら下げは１文字だけ*/ ||  (!(m_bCRLFBra && m_keta==m_xdspMax && IsCRLF(ch2)) && //行末ぶら下げ改行の場合
			//1   !   3    4
			//    ^---pch
			//ch  ch2
			//        ^---pch
			//    ch  ch2
			//----------------
			//！
			//あ      ！
			//1   2   3    4
			//    ^---pch
			//ch  ch2
			//        ^---pch
			//             ^---pch
			//        ch   ch2
			//(m_bKinBra=((chType2|GetFullType(ch2)) & (KINSOK_FIRST|KINSOK_2ND))==KINSOK_FIRST) //行頭禁則文字の場合
			!(m_bKinBra=IsKinsokFirst(GetFullType(ch2), GetFullType(*(pch+1)), ch2, *(pch+1)))//行頭禁則文字の場合
			) )  {
			sell.off = m_off_first;
			if (m_keta!=NPOS && !m_bKinBra) {
				bool bHami=false;
				if (m_keta>m_xdspMax && !m_bKinBra && !IsKinFirstDouble(chType, chType2, ch,  ch2_save)) {  
					bHami = true;
					//禁則によるぶら下げ以外で
					//この文字は２バイト目がはみ出す為、前の文字までとする
					//あ
					//1   2   3    4
					//    ^---pch
					//ch  ch2
					//        ^---pch
					//        ch2
					//             ^---pch
					//        ch  ch2
					off-=2;
					pch-=2;
					ch2=(*pch);
					m_keta-=2;
					//文末禁則の為に!
				}
				//行末禁則で、２バイト文字目以外？
				if (IsKinsokLast(chType, chType2, ch, ch2_save)) {
					//1   2   3    4
					//    ^---pch
					//ch  ch2
					//    ^---pch
					//    ch  ch2
					//----------------
					//あ
					//1   2   3    4
					//    ^---pch
					//ch  ch2
					//        ^---pch
					//        ch2
					//             ^---pch
					//        ch  ch2
					if ((chType & KINSOK_SINGLE)>0) {
						off--;
						pch--;
						ch2=*(pch);
						m_keta--;
					}else{
						off-=2;
						pch-=2;
						ch2=(*pch);
						m_keta-=2;
					}
				}else
				//ワードラップ
				if (m_offWordWrap>0 && !bHami && (chType & (KANJ|WHT))==0 && (chType2 & (KANJ|WHT))==0) {
					int off_diff = off - m_offWordWrap +1;
					assert(off_diff>=0);
					off -= off_diff;
					pch -= off_diff;
					ch2=*(pch);
					m_keta -= off_diff;
				}
			}
			sell.len = off - m_off_first;
			m_off_first = off;
			
			if (m_bline) {
				sell.line = ++m_line;
				m_bline = false;
			}else
				sell.line = 0;	//initialize
			if (m_keta==NPOS) {
				m_bline=true;
			}else
				m_keta_save = m_keta;
			m_keta = 0;
			m_bKinBra = false;
			m_offWordWrap = 0;
			m_chTypeWordWrap = 0;
			return true;
		}
	}
	return false;
}
void CFormatChars::Initialize(OBJOFF off, OBJOFF line)
{
	m_keta=0;
	m_off_first=off;
	m_off_read=0;
	if (line!=NPOS)
		m_line=line;
	m_bline=true;
	m_keta_save=0;
	m_bKinBra = false;
	m_offWordWrap = 0;
	m_chTypeWordWrap = 0;
}

