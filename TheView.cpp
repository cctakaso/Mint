// TheView.cpp : CTheView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <mbstring.h>
#include <time.h>
#include "The.h"

#include "TheDoc.h"
#include "TheView.h"
#include "MainFrm.h"
#include "action.h"
#include "chara.h"
#include "ConfigSheet.h"
#include "JumpDlg.h"
#include "LineHeader.h"
#include "ConfigEdit.h"
#include "TanbunAddDlg.h"
#include "TanbunEdit.h"
#include "PageDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int svnViewCount = 0;
static int svbCommandFirst = true;
static CPoint svpt_mouse;
//find dialog!
static CFindDlg svdlgFind;
static CJumpDlg svdlgJump;
#ifdef SHAR
static CLineHeaderDlg svdlgLineHeader;
#endif //#ifdef SHAR
static unsigned int svtimeCount=0;
extern CViewEnvTbl theViewEnvTbl;
extern CTheApp theApp;
const static char szNil[] = "";
CViewEdTbl theViewEdTbl;

#define CLOCK_PER_10MSEC (10)

#define DEF_CLOCK_SPUP (200)	//220
#define DEF_CLOCK_SP_FST (20)	//(30)
#define DEF_CLOCK_SP_SND (10) //(11)

#define DEF_CLOCK_SPUP_95 (250) //(80)
#define DEF_CLOCK_SP_FST_95 (33) //(8)
#define DEF_CLOCK_SP_SND_95 (13) //(1)

bool svbSysWinNT = false;
bool svbSysWin95 = false;
static bool svbDontScroll;
/////////////////////////////////////////////////////////////////////////////
// static 関数
static int SStDP_CopyProc_line(const string &str, void *pv)
{
	string strT;
	if (IsValidDataPStr(((SSDP_DATA*)pv)->pstrAdd)) {	// && str.size()>0 && !IsCRLF(str[0])データのある行だけ引用ﾏｰｸ追加->削除
		strT = *((SSDP_DATA*)pv)->pstrAdd;
		strT += str;
	}else {
		strT = str;
		if (IsValidPStr(((SSDP_DATA*)pv)->pstrRemove)) {
			if (IsValidDataPStr(((SSDP_DATA*)pv)->pstrRemove))
				TrimLeftChar(strT, ((SSDP_DATA*)pv)->pstrRemove); //指定文字列だけ削除
			else
				TrimLeft(strT); //左空白だけ削除
		}
	}
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, (BYTE*)strT.data(), strT.size()) ? 0:-1;
}
static int SStDP_CopyProc_block(const BYTE *sz, OBJSIZE len, void *pv)
{
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, sz, len) ? 0:-1;
}
static int SStDP_CopyProc_line_center(const string &str, void *pv)
{
	string strT, strNoSp = str;
	TrimLeft(strNoSp); //左空白だけ削除
	bool bOver; //bTab = strNoSp.find("\t") != string::npos;
	OBJOFF keta = ((SSDP_DATA*)pv)->pHint->String2Keta(strNoSp, bOver);
	OBJOFF keta_space = ((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax != SHRT_MAX ?
		((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax:((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax_save;
	if (keta_space>256)
		keta_space = 76;
//#ifdef _VERIFY
//	CString strMsg;
//	strMsg.Format("keta=%d, \"%s\"", keta, str.c_str());
//	AfxMessageBox(strMsg);
//#endif
	if (/*!bOver &&*/ keta<keta_space && ((keta_space-=keta)/=2)>0) {
		strT.append(keta_space, ' ');
		strT += strNoSp;
	}else
		strT = str;
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, (BYTE*)strT.data(), strT.size()) ? 0:-1;
}
static int SStDP_CopyProc_line_right(const string &str, void *pv)
{
	string strT, strNoSp = str;
	TrimLeft(strNoSp); //左空白だけ削除
	bool bOver; //bTab = strNoSp.find("\t") != string::npos;
	OBJOFF keta = ((SSDP_DATA*)pv)->pHint->String2Keta(strNoSp, bOver);
	OBJOFF keta_space = ((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax != SHRT_MAX ?
		((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax:((SSDP_DATA*)pv)->pHint->m_pFormatChars->m_xdspMax_save;
	if (keta_space>256)
		keta_space = 76;
	if (/*!bOver &&*/ keta<keta_space && (keta_space-=keta)>0) {
		strT.append(keta_space, ' ');
		strT += strNoSp;
	}else
		strT = str;
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, (BYTE*)strT.data(), strT.size()) ? 0:-1;
}
static int SStDP_CopyProc_line_header(const string &str, void *pv)
{
	string strT;
	if (IsValidDataPStr(((SSDP_DATA*)pv)->pstrAdd)) {	//データのある行だけ引用ﾏｰｸ追加
		string strNoSp = str;
		OBJSIZE len_space = strNoSp.size();
		TrimLeft(strNoSp); //左空白だけ削除
		len_space -= strNoSp.size();
		strT.append(len_space, ' ');
		strT = *((SSDP_DATA*)pv)->pstrAdd;
		strT += strNoSp;
		IncLineHeaderNumStr(*((SSDP_DATA*)pv)->pstrAdd); //次の為にインクリメントしておく。ﾃﾝﾎﾟﾗﾘじゃないといけない！
	}else {
		strT = str;
		if (IsValidPStr(((SSDP_DATA*)pv)->pstrRemove)) {
			TrimLeftLineHeader(strT); //
		}
	}
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, (BYTE*)strT.data(), strT.size()) ? 0:-1;
}



/////////////////////////////////////////////////////////////////////////////
// CTheView

IMPLEMENT_DYNCREATE(CTheView, CView)

BEGIN_MESSAGE_MAP(CTheView, CView)
	//{{AFX_MSG_MAP(CTheView)
	ON_MESSAGE(WM_AROWKEY_DOWN, OnMessageArowKeyDown)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_FREE_CUR, OnUpdateFreeCur)
	ON_COMMAND(ID_FREE_CUR, OnFreeCur)
	ON_UPDATE_COMMAND_UI(ID_VERTICAL, OnUpdateVertical)
	ON_COMMAND(ID_VERTICAL, OnVertical)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_WM_KILLFOCUS()
	ON_UPDATE_COMMAND_UI(ID_LINENUM, OnUpdateLinenum)
	ON_UPDATE_COMMAND_UI(ID_LINENUM_LINE, OnUpdateLinenumLine)
	ON_UPDATE_COMMAND_UI(ID_LINENUM_NO, OnUpdateLinenumNo)
	ON_UPDATE_COMMAND_UI(ID_LINENUM_ROW, OnUpdateLinenumRow)
	ON_COMMAND(ID_LINENUM, OnLinenum)
	ON_COMMAND(ID_LINENUM_LINE, OnLinenumLine)
	ON_COMMAND(ID_LINENUM_NO, OnLinenumNo)
	ON_COMMAND(ID_LINENUM_ROW, OnLinenumRow)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateEditFindNext)
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnEditFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_CONFIG, OnUpdateConfig)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_UPDATE_COMMAND_UI(ID_LIMITCOL, OnUpdateLimitcol)
	ON_COMMAND(ID_LIMITCOL, OnLimitcol)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_JUMP, OnUpdateJump)
	ON_COMMAND(ID_JUMP, OnJump)
	ON_UPDATE_COMMAND_UI(ID_EDT_VIEW, OnUpdateEdtView)
	ON_COMMAND(ID_EDT_VIEW, OnEdtView)
	ON_UPDATE_COMMAND_UI(ID_EDTL_REF_COPY, OnUpdateEdtlRefCopy)
	ON_COMMAND(ID_EDTL_REF_COPY, OnEdtlRefCopy)
	ON_UPDATE_COMMAND_UI(ID_EDTL_REF_PAST, OnUpdateEdtlRefPast)
	ON_COMMAND(ID_EDTL_REF_PAST, OnEdtlRefPast)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_APPEND, OnUpdateEditCopyAppend)
	ON_COMMAND(ID_EDIT_COPY_APPEND, OnEditCopyAppend)
	ON_UPDATE_COMMAND_UI(ID_EDTL_LEFT, OnUpdateEdtlLeft)
	ON_COMMAND(ID_EDTL_LEFT, OnEdtlLeft)
	ON_UPDATE_COMMAND_UI(ID_EDTL_CENTER, OnUpdateEdtlCenter)
	ON_COMMAND(ID_EDTL_CENTER, OnEdtlCenter)
	ON_UPDATE_COMMAND_UI(ID_EDTL_RIGHT, OnUpdateEdtlRight)
	ON_COMMAND(ID_EDTL_RIGHT, OnEdtlRight)
	ON_UPDATE_COMMAND_UI(ID_EDTL_REF_RIGHT, OnUpdateEdtlRefRight)
	ON_COMMAND(ID_EDTL_REF_RIGHT, OnEdtlRefRight)
	ON_UPDATE_COMMAND_UI(ID_EDTL_REF_LEFT, OnUpdateEdtlRefLeft)
	ON_COMMAND(ID_EDTL_REF_LEFT, OnEdtlRefLeft)
	ON_UPDATE_COMMAND_UI(ID_EDTL_LINE_HEADER_RIGHT, OnUpdateEdtlLineHeaderRight)
	ON_COMMAND(ID_EDTL_LINE_HEADER_RIGHT, OnEdtlLineHeaderRight)
	ON_UPDATE_COMMAND_UI(ID_EDTL_LINE_HEADER_LEFT, OnUpdateEdtlLineHeaderLeft)
	ON_COMMAND(ID_EDTL_LINE_HEADER_LEFT, OnEdtlLineHeaderLeft)
	ON_UPDATE_COMMAND_UI(ID_EDTL_INDENT_RIGHT, OnUpdateEdtlIndentRight)
	ON_COMMAND(ID_EDTL_INDENT_RIGHT, OnEdtlIndentRight)
	ON_UPDATE_COMMAND_UI(ID_EDTL_INDENT_LEFT, OnUpdateEdtlIndentLeft)
	ON_COMMAND(ID_EDTL_INDENT_LEFT, OnEdtlIndentLeft)
	ON_UPDATE_COMMAND_UI(ID_EDTL_INDENT_AUTO, OnUpdateEdtlIndentAuto)
	ON_COMMAND(ID_EDTL_INDENT_AUTO, OnEdtlIndentAuto)
	ON_UPDATE_COMMAND_UI(ID_REDRAW, OnUpdateRedraw)
	ON_COMMAND(ID_REDRAW, OnRedraw)
	ON_UPDATE_COMMAND_UI(ID_FLP_MARK, OnUpdateFlpMark)
	ON_COMMAND(ID_FLP_MARK, OnFlpMark)
	ON_UPDATE_COMMAND_UI(ID_FLP_MARK_JUMP, OnUpdateFlpMarkJump)
	ON_COMMAND(ID_FLP_MARK_JUMP, OnFlpMarkJump)
	ON_UPDATE_COMMAND_UI(ID_MARK, OnUpdateMark)
	ON_COMMAND(ID_MARK, OnMark)
	ON_UPDATE_COMMAND_UI(ID_MARK_JUMP_PREV, OnUpdateMarkJumpPrev)
	ON_COMMAND(ID_MARK_JUMP_PREV, OnMarkJumpPrev)
	ON_UPDATE_COMMAND_UI(ID_MARK_JUMP_NEXT, OnUpdateMarkJumpNext)
	ON_COMMAND(ID_MARK_JUMP_NEXT, OnMarkJumpNext)
	ON_UPDATE_COMMAND_UI(ID_MARK_REMOVEALL, OnUpdateMarkRemoveall)
	ON_COMMAND(ID_MARK_REMOVEALL, OnMarkRemoveall)
	ON_UPDATE_COMMAND_UI(ID_FIND_BRACKET, OnUpdateFindBracket)
	ON_COMMAND(ID_FIND_BRACKET, OnFindBracket)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_PREV, OnUpdateEditFindPrev)
	ON_COMMAND(ID_EDIT_FIND_PREV, OnEditFindPrev)
	ON_UPDATE_COMMAND_UI(ID_INPUT_MARK, OnUpdateInputMark)
	ON_COMMAND(ID_INPUT_MARK, OnInputMark)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_EDIT, OnUpdateConfigEdit)
	ON_COMMAND(ID_CONFIG_EDIT, OnConfigEdit)
	ON_UPDATE_COMMAND_UI(ID_SELENV_NORMAL, OnUpdateSelenv)
	ON_COMMAND(ID_SELENV_NORMAL, OnSelenvNormal)
	ON_COMMAND(ID_SELENV_START1, OnSelenvStart1)
	ON_COMMAND(ID_SELENV_START2, OnSelenvStart2)
	ON_COMMAND(ID_SELENV_START3, OnSelenvStart3)
	ON_COMMAND(ID_SELENV_START4, OnSelenvStart4)
	ON_COMMAND(ID_SELENV_START5, OnSelenvStart5)
	ON_COMMAND(ID_SELENV_START6, OnSelenvStart6)
	ON_COMMAND(ID_SELENV_START7, OnSelenvStart7)
	ON_COMMAND(ID_SELENV_START8, OnSelenvStart8)
	ON_COMMAND(ID_SELENV_START9, OnSelenvStart9)
	ON_COMMAND(ID_TANBUN_ADD, OnTanbunAdd)
	ON_UPDATE_COMMAND_UI(ID_TANBUN_ADD, OnUpdateTanbunAdd)
	ON_COMMAND(ID_TANBUN_EDIT, OnTanbunEdit)
	ON_UPDATE_COMMAND_UI(ID_TANBUN_EDIT, OnUpdateTanbunEdit)
	ON_COMMAND(ID_EDTL_TIME, OnEdtlTime)
	ON_UPDATE_COMMAND_UI(ID_EDTL_TIME, OnUpdateEdtlTime)
	ON_COMMAND(ID_EDTL_DATE, OnEdtlDate)
	ON_UPDATE_COMMAND_UI(ID_EDTL_DATE, OnUpdateEdtlDate)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
//	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(IDC_FILE_CLOSE, OnFileClose)
//	ON_UPDATE_COMMAND_UI(IDC_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_WINDOW_CLOSE_NO_THIS, OnWindowCloseNoThis)
//	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_NO_THIS, OnUpdateWindowCloseNoThis)
	ON_UPDATE_COMMAND_UI(ID_TAGJUMP, OnUpdateTagjump)
	ON_COMMAND(ID_TAGJUMP, OnTagjump)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START1, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START1, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START2, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START3, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START4, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START5, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START6, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START7, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START8, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START9, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START10, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START11, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START12, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START13, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START14, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START15, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START16, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START17, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START18, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START19, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START20, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START21, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START22, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START23, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START24, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START25, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START26, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START27, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START28, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START29, OnUpdateSelenv)
	ON_UPDATE_COMMAND_UI(ID_SELENV_START30, OnUpdateSelenv)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
#ifdef SPEC_PRINT
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_PAGE_SETUP, OnUpdatePageSetup)
#endif /SPEC_PRINT
    ON_WM_MOUSEWHEEL()
    END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTheView クラスの構築/消滅

CTheView::CTheView()
{
	// TODO: この場所に構築用のコードを追加してください。
	svnViewCount++;
	m_idleCount = 1;
//	m_bAttach = false;
	m_bDirtyView = false;
	m_bInputView = false;

	m_pViewEd = null;
#ifdef SPEC_PRINT
	m_modeDraw = DRAW_DSP;
#endif //SPEC_PRINT
}

CTheView::~CTheView()
{
	svnViewCount--;
	//if (m_pViewEd)
	//	delete m_pViewEd;
}

BOOL CTheView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
			AfxGetApp()->LoadStandardCursor(IDC_IBEAM),
			(HBRUSH)(COLOR_WINDOW+1));
	
	if (cs.lpszClass != NULL)
		return TRUE;
	else
		return FALSE;
	//return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTheView クラスの印刷
void CTheView::OnUpdatePageSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

#ifdef SPEC_PRINT
void CTheView::OnPageSetup() 
{
	CPageDlg dlg;
	dlg.DoModal();
}
#endif

#ifdef SPEC_PRINT
BOOL CTheView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CWaitCursor wait;
	//CSize new_size(850, 1100);  // 8.5" x 11" default
	HDC hDC;
	CDC dc;
	CSize sizePapermm(2100, 2970), sizePaper, sizePrint;  // A4 default
	CRect rcPrintMargin;

	// magic to get printer dialog that would be used if we were printing!
	CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&dlgPrint.m_pd))
	{
		AfxMessageBox("Default Printer ERR!");
		return FALSE;
	}

	hDC = dlgPrint.CreatePrinterDC();
	dc.Attach(hDC);

	if (svbSysWin95) {
		Escape (dc, GETPHYSPAGESIZE, NULL, NULL, (LPPOINT) &sizePaper);
	}else{
		sizePaper.cx = dc.GetDeviceCaps(PHYSICALWIDTH);
		sizePaper.cy = dc.GetDeviceCaps(PHYSICALHEIGHT);
	}
	sizePrint.cx = dc.GetDeviceCaps( HORZRES );
	sizePrint.cy = dc.GetDeviceCaps( VERTRES );

	//mm->inch
	//1 inch -> 25.4mm
	//int MMx10_from_Inchx100(int inch) {
	//return (int)(((unsigned long)inch) * 254 / 100);
	//}
	rcPrintMargin = theViewEnvTbl.m_printenv.m_rcMargin;
	rcPrintMargin.left = MulDiv(rcPrintMargin.left, dc.GetDeviceCaps(LOGPIXELSX)*10, 254);
	rcPrintMargin.top = MulDiv(rcPrintMargin.top, dc.GetDeviceCaps(LOGPIXELSY)*10, 254);
	rcPrintMargin.right = MulDiv(rcPrintMargin.right, dc.GetDeviceCaps(LOGPIXELSX)*10, 254);
	rcPrintMargin.bottom = MulDiv(rcPrintMargin.bottom, dc.GetDeviceCaps(LOGPIXELSY)*10, 254);
	
#ifdef NOTUSE
	MulDivRect(rcPrintMargin, theViewEnvTbl.m_printenv.m_rcMargin, 10, 1);
	LPDEVMODE lpDev = dlgPrint.GetDevMode();
	if (lpDev && (lpDev->dmPaperLength>0 && lpDev->dmPaperWidth)) {
		sizePapermm = CSize(lpDev->dmPaperWidth, lpDev->dmPaperLength);
		ConvertMargin(rcPrintMargin, sizePapermm, *lpDev);
		MulDivRect(rcPrintMargin, rcPrintMargin, sizePaper.cy, sizePapermm.cy);
	}
#endif

#if 0	
	//Escape (hPrnDC, GETPRINTINGOFFSET, NULL, NULL, (LPPOINT) &pt);
	//特に指定しない限り、Visual Basic の移動ステートメント、
	//サイズ変更ステートメント、およびグラフィカル描画ステートメントではすべて、
	//単位として twip が使用されます。
	//1 twip は、プリンタの 1 ポイントの 1/20 で、
	//1,440 twip で 1 インチ、567 twip で 1cm となります。
	//?mm * ?dot/inch /(1440twip/inch)*5670 twip/mm
	//dot/inch * inch * 1440/5670
	int mul=0, dev;
	int mapmode, mapmode_save;
	dc.SaveDC();
#ifdef _DEBUG
	mapmode = dc.GetMapMode();
#endif
	mapmode_save = dc.SetMapMode(mapmode = MM_LOMETRIC);
	mapmode = dc.GetMapMode();
	switch(mapmode) {
	case MM_HIMETRIC:
		mul = 100;
		dev = 1;
		break;
	case MM_LOMETRIC:
		mul = 10;
		dev = 1;
		break;
	case MM_TWIPS:
		mul = 5670;
		dev = 1;
		break;
	case MM_HIENGLISH:
		mul = 56700;
		dev = 144;
		break;
	case MM_LOENGLISH:
		mul = 5670;
		dev = 144;
		break;
	}
	if (mul!=0)
	else if (mapmode==MM_TEXT) {
		MulDivRect(rcPrintMargin, theViewEnvTbl.m_printenv.m_rcMargin, 567, 144);
		rcPrintMargin.left = MulDiv(rcPrintMargin.left, dc.GetDeviceCaps(LOGPIXELSX), 1);
		rcPrintMargin.top = MulDiv(rcPrintMargin.top, dc.GetDeviceCaps(LOGPIXELSY), 1);
		rcPrintMargin.right = MulDiv(rcPrintMargin.right, dc.GetDeviceCaps(LOGPIXELSX), 1);
		rcPrintMargin.bottom = MulDiv(rcPrintMargin.bottom, dc.GetDeviceCaps(LOGPIXELSX), 1);
	}
#endif

	CPoint ptOffset;
	if (svbSysWin95) {
		Escape (dc, GETPRINTINGOFFSET, NULL, NULL, (LPPOINT) &ptOffset);
	}else{
		ptOffset.x = dc.GetDeviceCaps(PHYSICALOFFSETX);
		ptOffset.y = dc.GetDeviceCaps(PHYSICALOFFSETY);
	}

#if 0
	dc.SetMapMode(mapmode_save);
	dc.RestoreDC(-1);
#endif

	rcPrintMargin.left = max(0, rcPrintMargin.left-ptOffset.x);
	rcPrintMargin.top = max(0, rcPrintMargin.top-ptOffset.y);
	rcPrintMargin.right = max(0, rcPrintMargin.right-ptOffset.x);
	rcPrintMargin.bottom = max(0, rcPrintMargin.bottom-ptOffset.y);

	

	// デフォルトの印刷準備
	OBJSIZE row_max = m_pViewEd->GetLineMaxNum(true);
	OBJSIZE row = m_pViewEd->m_cursor.Befor().D.y;//m_pViewEd->VGetScrollPos(SB_VERT);
	//size get
	CCharsPrintAttr chardata;
	chardata.SetPrinter(m_pViewEd, &dc);
	if (theViewEnvTbl.m_printenv.IsExistHeader()) {
		rcPrintMargin.top += chardata.cyLine;
		if (theViewEnvTbl.m_printenv.m_bHeaderLine)
			rcPrintMargin.top += chardata.cyLine/2;
	}
	if (theViewEnvTbl.m_printenv.IsExistFooter()) {
		rcPrintMargin.bottom += chardata.cyLine;
		if (theViewEnvTbl.m_printenv.m_bFooterLine)
			rcPrintMargin.bottom += chardata.cyLine/2;
	}

	sizePrint.cx -= (rcPrintMargin.left + rcPrintMargin.right);
	sizePrint.cy -= (rcPrintMargin.top +  rcPrintMargin.bottom);

//	rcPrintMargin.left -= rcOffset.left;
//	rcPrintMargin.top -= rcOffset.top;
//	rcPrintMargin.right = (sizePaper.cx - rcPrintMargin.right) - rcOffset.left;
//	rcPrintMargin.bottom = (sizePaper.cy - rcPrintMargin.bottom) - rcOffset.top;
	rcPrintMargin.right = (sizePrint.cx + rcPrintMargin.left);
	rcPrintMargin.bottom = (sizePrint.cy + rcPrintMargin.top);


	int row_per_page = max((sizePrint.cy + chardata.cyBeforLine) / chardata.cyLine, 1);
	int pages = row_max/row_per_page + (((row_max%row_per_page)>0) ? 1:0);
	chardata.SetDisplay(m_pViewEd, &dc);
	
	
	pInfo->SetMaxPage(pages);
	pInfo->m_pPD->m_pd.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

	if (pInfo && pInfo->m_bPreview) {
		pInfo->m_nCurPage = (row/row_per_page+1);
		string strT = "総ﾍﾟｰｼﾞ数: ";
		strT += itostring(pages,10);
		((CMainFrame*)AfxGetMainWnd())->SetPaneText((char*)strT.c_str(), 1);
	} else {
		pInfo->m_pPD->m_pd.nFromPage = row/row_per_page+1;
		//pInfo->SetMinPage(row/row_per_page+1);
	}
	//pInfo->m_strPageDesc = 
//	if (pInfo && pInfo->m_bPreview)
//		::ReleaseDC(NULL, dc.Detach());

	if (DoPreparePrinting(pInfo)) {
		CTheDoc* pDoc = (CTheDoc*)GetDocument();
		CRect *prc = new CRect[2];
		*prc = CRect(rcPrintMargin);
		pInfo->m_lpUserData = prc;
		if (pInfo->m_pPD->PrintAll())
			pInfo->m_pPD->m_pd.nFromPage = 1;
		else if (pInfo->m_pPD->PrintSelection()) {
			row = m_pViewEd->m_cursor.Befor().D.y;
			row_max = m_pViewEd->m_cursor.End().D.y;
			if (!m_pViewEd->m_cursor.IsExpand())	//(row==row_max)
				row_max += (row_per_page - 1);
			(prc+1)->top = row;
			(prc+1)->bottom = row_max;
			row_max = row_max - row + 1;
			pages = row_max/row_per_page + (((row_max%row_per_page)>0) ? 1:0);
			pInfo->m_pPD->m_pd.nFromPage = 1;
			pInfo->m_pPD->m_pd.nToPage = pages;
		}
		theViewEnvTbl.m_printenv.ResetPrintEnvData(pages, pDoc->GetPathName());
		return TRUE;
	}
	return FALSE;
}

BOOL CTheView::DoPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	// don't prompt the user if we're doing print preview, printing directly,
	// or printing via IPrint and have been instructed not to ask

	CWinApp* pApp = AfxGetApp();
	if (pInfo->m_bPreview || pInfo->m_bDirect ||
		(pInfo->m_bDocObject && !(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER)))
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// if no printer set then, get default printer DC and create DC without calling
			//   print dialog.
			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			{
				// bring up dialog to alert the user they need to install a printer.
				if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						return FALSE;
			}

			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				// call CreatePrinterDC if DC was not created by above
				if (pInfo->m_pPD->CreatePrinterDC() == NULL)
					return FALSE;
			}
		}

		// set up From and To page range from Min and Max
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	}
	else
	{
		// otherwise, bring up the print dialog and allow user to change things
		// preset From-To range same as Min-Max range
//TODO:Comment out!
//		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();

		if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
			return FALSE;       // do not print

	}

	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;

	pInfo->m_nNumPreviewPages = pApp->m_nNumPreviewPages;
	VERIFY(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	return TRUE;
}

void CTheView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
	m_modeDraw = ((pInfo && pInfo->m_bPreview) ? DRAW_PRV:DRAW_PRN);
}

void CTheView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	// TODO: 印刷後の後処理を追加してください。
	if (pInfo->m_lpUserData)
		delete [] ((CRect*)pInfo->m_lpUserData);
#ifdef SPEC_PRINT
	m_modeDraw = DRAW_DSP;
#endif
}
void CTheView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	m_modeDraw = ((pInfo && pInfo->m_bPreview) ? DRAW_PRV:DRAW_PRN);
	
//	CView::OnPrint(pDC, pInfo);
	if (!m_pViewEd) {
		m_modeDraw = DRAW_DSP;
		return;
	}

	//CTheDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);

	CCharsPrintAttr chardata;
	chardata.SetPrinter(m_pViewEd, pDC);


//	int mapmode, mapmode_save;
#ifdef _DEBUG
//	mapmode = pDC->GetMapMode();
#endif
//	mapmode_save = pDC->SetMapMode(mapmode = MM_LOMETRIC);
//	mapmode = pDC->GetMapMode();



	CRect rc, rcMargin = *((CRect*)pInfo->m_lpUserData);
	theViewEnvTbl.m_printenv.ResetPrintEnvData_Page(pInfo->m_nCurPage);
	//ﾍｯﾀﾞｰ印刷開始
	if (theViewEnvTbl.m_printenv.IsExistHeader()) {
		rc = rcMargin;
		rc.bottom = rc.top;
		rc.top -= ((theViewEnvTbl.m_printenv.m_bHeaderLine ? chardata.cyLine*3/2:chardata.cyLine) - chardata.cyBeforLine);
		theViewEnvTbl.m_printenv.PrintHeader(pDC, rc);
		if (theViewEnvTbl.m_printenv.m_bHeaderLine) {
			rc = rcMargin;
			int y = rc.top - chardata.cyLine / 4;
			SelectObject(pDC->GetSafeHdc(), GetStockObject(BLACK_PEN)); 
			pDC->MoveTo(rc.left, y);
			pDC->LineTo(rc.right, y);
		}
	}else if (theViewEnvTbl.m_printenv.m_bHeaderLine) {
		rc = rcMargin;
		int y = rc.top;
		SelectObject(pDC->GetSafeHdc(), GetStockObject(BLACK_PEN)); 
		pDC->MoveTo(rc.left, y);
		pDC->LineTo(rc.right, y);
	}
	if (theViewEnvTbl.m_printenv.IsExistFooter()) {
		rc = rcMargin;
		rc.top = rc.bottom + ((theViewEnvTbl.m_printenv.m_bFooterLine ? chardata.cyLine/2:0) + chardata.cyBeforLine);
		rc.bottom += (2*chardata.cyLine);
		theViewEnvTbl.m_printenv.PrintFooter(pDC, rc);
		if (theViewEnvTbl.m_printenv.m_bFooterLine) {
			rc = rcMargin;
			int y = rc.bottom + chardata.cyLine / 4;
			SelectObject(pDC->GetSafeHdc(), GetStockObject(BLACK_PEN)); 
			pDC->MoveTo(rc.left, y);
			pDC->LineTo(rc.right, y);
		}
	}else if (theViewEnvTbl.m_printenv.m_bFooterLine) {
		rc = rcMargin;
		int y = rc.bottom;
		SelectObject(pDC->GetSafeHdc(), GetStockObject(BLACK_PEN)); 
		pDC->MoveTo(rc.left, y);
		pDC->LineTo(rc.right, y);
	}
	
	//本文印刷開始
	rc = rcMargin;
	rc.bottom += chardata.cyBeforLine;
	int row_per_page = max(rc.Height() / chardata.cyLine, 1);
	int row;
	if (pInfo->m_pPD->PrintSelection()) {
		CRect rc2 = *(((CRect*)pInfo->m_lpUserData)+1);
		row = rc2.top + ((pInfo->m_nCurPage-1) * row_per_page);
		int rows = rc2.bottom - row;
		if (rows>=row_per_page)
			rc.bottom = rc.top + chardata.cyLine * row_per_page;
		else
			rc.bottom = rc.top + chardata.cyLine * rows;
	}else{
		row = (pInfo->m_nCurPage-1) * row_per_page;
		rc.bottom = rc.top + chardata.cyLine * row_per_page;
	}
	
	m_pViewEd->OnDraw(pDC, 0, row, rc, false, m_modeDraw);

	chardata.SetDisplay(m_pViewEd, pDC);
	m_modeDraw = DRAW_DSP;
}
#endif //SPEC_PRINT
/////////////////////////////////////////////////////////////////////////////
// CTheView クラスの描画

void CTheView::OnDraw(CDC* pDC)
{
    if (!m_pViewEd || !m_pViewEd->IsValidEnv() || !m_pViewEd->m_pWnd)
		return;
#ifdef SPEC_PRINT
	if (m_modeDraw != DRAW_DSP)
		return;
	m_modeDraw = DRAW_DSP;
#endif //SPEC_PRINT
	//CTheDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);

	pDC->SaveDC();
//	SelectPalette(pDC->m_hDC, theApp.m_hSysPal, false);
//	RealizePalette(pDC->m_hDC);

	CRect rcClip;
	pDC->GetClipBox(&rcClip);

	CPoint pt;
	pt.y = m_pViewEd->VGetScrollPos(SB_VERT);//GetScrollPos();
	pt.x = m_pViewEd->VGetScrollPos(SB_HORZ);//GetScrollPos();

#ifdef PAGEVIEW

#endif

	m_pViewEd->OnDraw(pDC, pt.x, pt.y, rcClip);
	pDC->RestoreDC(-1);
}

/////////////////////////////////////////////////////////////////////////////
// CTheView クラスの診断

#ifdef _DEBUG
void CTheView::AssertValid() const
{
	CView::AssertValid();
}

void CTheView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTheDoc* CTheView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTheDoc)));
	return (CTheDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTheView クラスのメッセージ ハンドラ

void CTheView::OnInitialUpdate() 
{
	if (m_pViewEd && m_pViewEd->m_pWnd)
		m_pViewEd->Detach();
	//m_bAttach = false;
	m_pViewEd = theViewEdTbl.GetEntry(GetDocument()->m_pMainObjLay);
	m_pViewEd->Attach(GetDocument()->m_pMainObjLay, this);
/*
	m_pViewEd->GetEnv()->SetFocusEnv();
	GetClientRect(&m_rcClient);
	m_pViewEd->ChangeFonts(this, m_rcClient);
	m_pViewEd->ChangeLineNum();
	m_pViewEd->MoveCoursor(VK_F1); //initial set cursel!
	if (m_pViewEd->IsVert())
		((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_RIGHT);
	else
		((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_TOP);
	m_pViewEd->ChangeColors(this, GetFocus()==this);//new!
*/
	Invalidate();
//	CView::OnInitialUpdate();
	if (m_pViewEd) {
		m_pViewEd->ResetCaret(this);
		SetPaneText_CurPos();
	}

	theIme.ImeWndProc(this, WM_CREATE, 0,0);
//	m_bAttach = true;
}

void CTheView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    if (!m_pViewEd || !m_pViewEd->m_pWnd)
		return;
//	SetClassLong(GetSafeHwnd(), GCL_HCURSOR,
//		(long)(m_pViewEd->IsVert() ? AfxGetApp()->LoadCursor(IDC_CUR_VERT):
//		AfxGetApp()->LoadStandardCursor(IDC_IBEAM))); 
	if (m_pViewEd && lHint == HINT_CLOSE_DOC) {
		m_pViewEd->VSetScrollPos(SB_HORZ, 0);
		m_pViewEd->VSetScrollPos(SB_VERT, 0);
		m_pViewEd->Detach();
//		m_bAttach = false;
		return;
	}else if(lHint == HINT_DIRTY_VIEW){
		Invalidate();
		//行数のチェック
		m_pViewEd->ChangeLineNum();
		//m_bDirtyView = true;
		return ;
	}else if (lHint == HINT_FOCUS_VIEW || lHint == HINT_LOST_FOCUS_VIEW) {
		if ((lHint == HINT_FOCUS_VIEW && pHint==this) || 
			(lHint == HINT_LOST_FOCUS_VIEW && pHint!=this)) {
			m_pViewEd->PushPopScrolls(this, m_rcClient, pHint==this);
			m_pViewEd->ChangeColors(this, pHint==this);
			Invalidate();
			//UpdateWindow();
		}
		return;
	}
	/*
	}else if (lHint == HINT_VERTICAL_VIEW){
		//GetDocument()->UpdateAllViews(this, HINT_VERTICAL_VIEW);
		GetClientRect(&m_rcClient);
		m_pViewEd->SetScrolls(this, m_rcClient, true);
		return;
	}*/

	GetClientRect(&m_rcClient);
	if (m_pViewEd->SetScrolls(this, m_rcClient)) {
		Invalidate();
	}

	m_bDirtyView = true;
}

void CTheView::UpdateOtherView()
{
//	if (m_bDirtyView = bDirty)
	GetDocument()->UpdateAllViews(this, HINT_DIRTY_VIEW);
}


void CTheView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	GetClientRect(&m_rcClient);
	if (m_pViewEd)
		m_pViewEd->SetScrolls(this, m_rcClient);
}

void CTheView::SetClocksPerSec()
{
	if (!svbSysWinNT && !svbSysWin95) {
 		OSVERSIONINFO osvi;

		osvi.dwOSVersionInfoSize = sizeof(osvi);
		if (GetVersionEx(&osvi)) {
			if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) {
				svbSysWin95 = true;
			}else{
				svbSysWinNT = true;
			}
			if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT ) {
				svbSysWinNT = true;
			}
		}
	
#ifdef _DEBUG
//		svbSysWin95 = true;
//		svbSysWinNT = false;
#endif
	}
}
BOOL CTheView::OnIdle(LONG lCount)
{
	TRACE("------CTheView::OnIdle() call %s ----\n",theViewEnvTbl.m_datetimes.GetThisTimeString_First().c_str());
#ifdef SPEC_PRINT
	if (m_modeDraw!=DRAW_DSP || !m_pViewEd || !GetDocument())
		return false;
#else
	if (!m_pViewEd || !GetDocument())
		return false;
#endif //SPEC_PRINT
	
	SetClocksPerSec();

	/*MSG msg;
	TRACE("------MyPeekMessage_Key() call ----\n");
	while (::PeekMessage (&msg, m_hWnd, 0, 0, PM_REMOVE)) {
		if (msg.message==WM_MOUSEMOVE && ::GetCapture()!=m_hWnd || msg.message==WM_TIMER) {
			; //eate!
		}else{
			if (msg.message>=WM_KEYFIRST && msg.message<=WM_KEYLAST) {
				if (msg.message==WM_KEYDOWN) {
					;
					TRACE("msg.wParam=%X\n", msg.wParam);
					OnKeyDown(msg.wParam, 0, 0);
				}
				continue;
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);
			if (msg.message==WM_CLOSE)
				return true;
			break;
		}
	}
	TRACE("<------------\n");
	*/
	if (((CMainFrame*)AfxGetMainWnd())->m_timeCount != svtimeCount) {
		m_idleCount++;
		svtimeCount = ((CMainFrame*)AfxGetMainWnd())->m_timeCount;
		//マウスキャプチャのチェック、View外マウスカーソルによる
		//スクロール
		if (::GetCapture()==m_hWnd && !m_rcClient.PtInRect(svpt_mouse)) {
			OnMouseMove(MK_LBUTTON, svpt_mouse);
			return true;
		}
		//ウェイトカーソルを元に戻す
		if (GetCursor()==AfxGetApp()->LoadStandardCursor(IDC_WAIT))
			m_pViewEd->SetMouseCursor(); 
		//タイトルの設定（変更ありの場合＊を付ける）
		GetDocument()->SetDirtyDocTitle(GetDocument()->m_pMainObjLay->GetActions().IsEnableUndo());
	}
#if Cursor_bug_20140510
	ShowBlinkCursor(false, false);
#endif
	if (m_bInputView && m_idleCount>=1) {
		//ｲﾝﾌﾟｯﾄﾏｰｸの設定
		m_pViewEd->GetHint()->m_bookMarks.SetMark(m_pViewEd, null, bmark_input);
		m_bInputView = false;
	}

	//行数のチェック
	if (m_bDirtyView && m_idleCount>=5) {
		if (m_pViewEd->ChangeLineNum())
			Invalidate();
	}

#ifdef DWAW_UPDATE
	while(m_bDirtyView && m_idleCount>=20) {


		if (MyPeekMessage(m_hWnd)) {
			return true;
		}


		CDC *pDC = GetDC();

		CRect rcClip = m_rcClient;
		CPoint pt;
		pt.y = m_pViewEd->VGetScrollPos(SB_VERT);//GetScrollPos();
		pt.x = m_pViewEd->VGetScrollPos(SB_HORZ);//GetScrollPos();
		int cy = m_pViewEd->GetSizeCell().cy;

		if (m_pViewEd->IsHorz())
			rcClip.bottom = m_rcClient.top;
		else
			rcClip.left = m_rcClient.right;
		while (true){
			if (m_pViewEd->IsHorz()) {
				if ((rcClip.top = rcClip.bottom)>m_rcClient.bottom) {
					m_bDirtyView = false;
					break;
				}
				rcClip.bottom = rcClip.top + cy;
			}else{
				if ((rcClip.right = rcClip.left)<m_rcClient.left) {
					m_bDirtyView = false;
					break;
				}
				rcClip.left = rcClip.right - cy;
			}
			pDC->SaveDC();
			m_pViewEd->OnDraw(pDC, pt.x, pt.y, rcClip, false);
			pDC->RestoreDC(-1);

			if (MyPeekMessage(m_hWnd))
				break;
		}
		ReleaseDC(pDC);

		
		UpdateOtherView();

		break;
	}
#endif //#ifdef DWAW_UPDATE
	
#if Cursor_bug_20140510
	ShowBlinkCursor(true, false);
#endif
	return FALSE;
}

inline int MyPeekMessage_Key(HWND hWnd, UINT nChar) {
	MSG msg;
	while (::PeekMessage (&msg, hWnd, 0, 0, PM_REMOVE)) {
		if (msg.message==WM_MOUSEMOVE && ::GetCapture()!=hWnd || msg.message==WM_TIMER) {
			; //eate!
		}else{
			if (msg.message>=WM_KEYFIRST && msg.message<=WM_KEYLAST) {
				TRACE("msg=%X\n", msg.message);
				if (msg.wParam==nChar) {
#ifdef _DEBUG
//					AfxMessageBox("EATE key!");
#endif
					return 1; //eate!
				}
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);
			return -1;
		}
	}
	return 0;
}

LRESULT CTheView::OnMessageArowKeyDown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
void CTheView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	static bool sbKeyDownCall = false;
	static int scount_rep = 0;
	int iMode;
	clock_t start, first_clock;
	clock_t interval;

	if (sbKeyDownCall)
		return;
	sbKeyDownCall = true;

	switch(nChar) {
		case VK_HOME:
		case VK_END:
		case VK_PRIOR:
		case VK_NEXT:
			scount_rep = 0;
			KeyDown(nChar, nRepCnt, nFlags);
			break;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DELETE:
		case VK_UP:
		case VK_DOWN:
#ifdef BUG_20111204 //リピートで早くするのをやめる。
		//default:
		//	if (svbSysWinNT) {
			{
//#define CLOCK_PER_10MSEC (10)
//#define DEF_CLOCK_SPUP (300)
//#define DEF_CLOCK_SP_FST (30)
//#define DEF_CLOCK_SP_SND (10)
				if (!(nFlags & (1<<14)) && nRepCnt<=1) {
					if (!(nFlags==0 && nRepCnt==0))
						scount_rep = 0;
					KeyDown(nChar, 0, 0);
					break; //no repeat!
				}
				if (svbSysWin95) {
					CCursor cursorOld = m_pViewEd->m_cursor;
					PostMessage(WM_KEYDOWN, nChar, 0);
					KeyDown(nChar, 0, 0);
					if (cursorOld.IsExpand())
						scount_rep = 0;
					else{
						if (scount_rep<10)
							scount_rep++;
						if (scount_rep>=10)
							PostMessage(WM_KEYDOWN, nChar, 0);
					}
					break;
				}
				SetClocksPerSec();
				iMode = 0;
				start = clock();
				interval = (svbSysWin95 ? DEF_CLOCK_SP_FST_95:DEF_CLOCK_SP_FST);
				//int r=0;
				do {
					first_clock= clock();
					KeyDown(nChar, 0, 0);
					if (iMode==0 && clock()>(start + (svbSysWin95 ? DEF_CLOCK_SPUP_95:DEF_CLOCK_SPUP))) {
						interval = (svbSysWin95 ? DEF_CLOCK_SP_SND_95:DEF_CLOCK_SP_SND);
						iMode=1;
					}
					//r=0;
					while(clock() < (first_clock + interval)) {
					//	r = MyPeekMessage_Key(m_hWnd, nChar);
					//	if (r==1) {
					//		break;
					//	}
					}
					
					if (svbSysWinNT) {
						if (GetAsyncKeyState(nChar)>=0)
							break;
					}else{
						//if (r!=1) {
						//	if (GetKeyState(nChar)>=0) {
						//		#ifdef _DEBUG
						//		AfxMessageBox("GetKeyState() break!");
						//		#endif
						//		break;
						//	}
						//}
					}
				}while(true);
		//	}else{
		//		if (!(nFlags & (1<<14))/* && nRepCnt<=1*/)
		//			KeyDown(nChar, 0, 0);
		//		else {
		//			PostMessage(WM_AROWKEY_DOWN, nChar, 0);
		//			PostMessage(WM_AROWKEY_DOWN, nChar, 0);
		//			PostMessage(WM_AROWKEY_DOWN, nChar, 0);
		//		}
			}
		break;
#else
		KeyDown(nChar, 0, 0);
		break;
#endif
	}
	sbKeyDownCall = false;
}


void CTheView::KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	svbCommandFirst = true;

	CCursor cursorOld = m_pViewEd->m_cursor;
	((CMainFrame*)AfxGetMainWnd())->SetPaneText();
	CAction action;
	VK vk;
	if (vk.IsEntryMoveKey(nChar, 
		GetKeyState(VK_CONTROL)<0, GetKeyState(VK_SHIFT)<0, 1))
		action.push_vk(act_move, vk);
	else if (vk.IsDeleteKey(nChar)) {
		OnChar(VK_MBUTTON, 0, 0);
		return;
		//PostMessage(WM_CHAR, /*VK_DELETE*/VK_MBUTTON, 0);
	}
	if (!action.IsEmpty()) {
		//SELL selOld = m_pViewEd->GetBlinkSell();
		int iScroll;
		if (iScroll=m_pViewEd->Action(&action)) {
			//文末へスクロール
			SetPaneText_CurPos();
#ifdef AUTO_SCROLL	
//			DoScroll(m_pViewEd->GetEnv()->m_bVert ? (iScroll==1 ? SB_LINELEFT:(iScroll==-2 ? SB_PAGERIGHT:SB_PAGELEFT)):
//							(iScroll==1 ? SB_LINEDOWN:(iScroll==-2 ? SB_PAGEUP:SB_PAGEDOWN)), 0,
//							m_pViewEd->GetEnv()->m_bVert ? SB_HORZ:SB_VERT);
//			return;
#endif //#ifdef AUTO_SCROLL	
		}

		ScrollForSell(cursorOld, action.m_act, nChar==VK_HOME);
		return;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTheView::OnMouseMove(UINT nFlags, CPoint point) 
{

	if (m_pViewEd==null || m_pViewEd->m_pWnd==null) //takaso2010 bugfix
		return ;

	//キャプチャー中で欄外スクロールの場合は、
	//Idleに言った後でないと動かないようにする
	if (::GetCapture()==m_hWnd && !m_rcClient.PtInRect(svpt_mouse)) {
		if (((CMainFrame*)AfxGetMainWnd())->m_timeCount != svtimeCount) {
			return;
		}
	}

	svpt_mouse = point;
	CPoint pt;
	CCursor cursorOld = m_pViewEd->m_cursor;
	pt.y = m_pViewEd->VGetScrollPos(SB_VERT);//GetScrollPos();
	pt.x = m_pViewEd->VGetScrollPos(SB_HORZ);//GetScrollPos();

	if (m_pViewEd->OnMouseMove(pt.y, pt.x, point, nFlags))
		ScrollForSell(cursorOld, act_move);
	

	//CView::OnMouseMove(nFlags, point);
}

void CTheView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (theIme.IsInComposition()) {
		theIme.ImeOpenClose(m_hWnd, false);//close IME!
		theIme.ImeOpenClose(m_hWnd, true);//open IME!
	}
	CPoint pt;
	CCursor cursorOld = m_pViewEd->m_cursor;
	pt.y = m_pViewEd->VGetScrollPos(SB_VERT);//GetScrollPos();
	pt.x = m_pViewEd->VGetScrollPos(SB_HORZ);//GetScrollPos();

	if (::GetCapture()!=m_hWnd)
		::SetCapture(m_hWnd);

	if (m_pViewEd->OnMouseMove(pt.y, pt.x, point, nFlags, true))
		ScrollForSell(cursorOld, act_move);
	
	//CView::OnLButtonDown(nFlags, point);
}

void CTheView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPoint pt;
	CCursor cursorOld = m_pViewEd->m_cursor;
	pt.y = m_pViewEd->VGetScrollPos(SB_VERT);//GetScrollPos();
	pt.x = m_pViewEd->VGetScrollPos(SB_HORZ);//GetScrollPos();

	//if (::GetCapture()!=m_hWnd)
	//	::SetCapture(m_hWnd);

	if (m_pViewEd->OnMouseDblClk(pt.y, pt.x, point, nFlags))
		ScrollForSell(cursorOld, act_move);
	//CView::OnLButtonDblClk(nFlags, point);
}

void CTheView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (::GetCapture()==m_hWnd)
		::ReleaseCapture();
	
	CView::OnLButtonUp(nFlags, point);
}

void CTheView::OnImeString(const string &str, const string &strAttr, bool bResult, bool bRedraw) 
{
	CLineHint *pHint = m_pViewEd->GetHint();
	bool bSpace=false;

	if (bResult && !GetDocument()->m_pMainObjLay->GetActions().IsBeforInputIme()) {
		if (!(bSpace = str.size()>0))
			return; //LostFucus からの強制ＩＭＥｸﾛｰｽﾞ　未確定文字列がない場合
	}

	//ｲﾝﾃﾞﾝﾄ処理
	if (bSpace && 
		m_pViewEd->m_cursor.IsExpand() && m_pViewEd->m_cursor.Befor().D.x==0 && m_pViewEd->m_cursor.End().D.x==0) {
		string strT = "　"; //全角スペース
		if (GetKeyState(VK_SHIFT)<0) {
			OnEdtTextFormat(null, &strT, (Normal_Proc)SStDP_CopyProc_line);
		}else{
			OnEdtTextFormat(&strT, null, (Normal_Proc)SStDP_CopyProc_line);
		}
		return;
	}

	//短文入力処理
	if (bSpace && GetKeyState(VK_CONTROL)<0) {
		if (DoTanbun())
			return;
	}

	if (m_pViewEd->GetEnv()->m_bFreeCursor)
		pHint->ResetUnFreeCursor(m_pViewEd->m_cursor);

	CCursor cursorOld = m_pViewEd->m_cursor;
//	OBJSIZE rowsOld = pHint->GetRows_ThisLine(cursorOld.GetBlinkCursor().D.y);

	//更新前：カーソル情報取得
	CUR curBef = bRedraw ? cursorOld.Befor():cursorOld.End(); //現在カーソルの最初をデフォルトセット
	OBJOFF row = curBef.D.y;
	if (curBef.D.x == 0 && pHint->GetThis_LineTop(row) < row) {
		//直前行の行末にセット
		curBef.D.y = row-1;
		curBef.D.x = NPOS;
//		pHint->SetCur(curBef, false);
	}
	OBJOFF rowNext = pHint->GetNext_LineTop(cursorOld.End().D.y);//直後行セット


	//メモリ内での更新処理
	CAction *paction = new CAction;
	if (bSpace)
		paction->push_string(act_input, str.c_str(), str.size()); //undoの為に、通常imeじゃなくする
	else
		paction->push_ime_string(str, bResult);
	bool bPush = m_pViewEd->Action(paction)!=0;
	if (bPush)
		GetDocument()->m_pMainObjLay->GetActions().push_action(paction);
	else
		delete paction;

	//OBJSIZE rowsNew = pHint->GetRows_ThisLine(m_pViewEd->GetBlinkCursor().D.y);
	//ScrollForSell(cursorOld, paction->m_act, false, true, rowsNew-rowsOld);
	//更新後：カーソル情報取得
	row = m_pViewEd->m_cursor.End().D.y; //現在カーソルの最初をデフォルトセット
	OBJOFF rowNext_After = pHint->GetNext_LineTop(row);//直後行セット
	CUR curLast;
	curLast.D.y = pHint->GetNext_LineTop(row);//直後行の行頭にセット
	curLast.D.x = 0;
	if (curLast.D.y == pHint->size()) {
		//該当論理行の最後を指す
		curLast.D.y = SIZEMAC(pHint->size());
		curLast.D.x = NPOS;
	}
//	pHint->SetCur(curLast, false); 

	//更新とスクロール
	CCursor cursor_update;
	cursor_update.SetCursor(&curLast, &curBef);
//	cursor_update.SetExpand();
//	cursor_update.SetOrgCursor(curBef);
//	cursor_update.SetBlinkCursor(curLast);
	ScrollForSell(cursorOld, paction->m_act, false, true, cursor_update, rowNext, rowNext_After);
}

bool CTheView::DoTanbun()
{
#ifdef SHAR
	CCursor cursorOld = m_pViewEd->m_cursor;
	string strKey = m_pViewEd->GetCursorWord_2Expand(true); //multiline ng, tail cursor ok
	CTanbunData *pdata = theViewEnvTbl.m_tanbuns.FindData(strKey);
	if (pdata) {
		if (pdata->m_fmt>CTanbunData::FMT_NO) {
			if (m_pViewEd->m_cursor.Befor().D.x > 0) {
				cursorOld = m_pViewEd->m_cursor;
				CAction *paction = new CAction;
				paction->push_char(VK_RETURN); //行の先頭以外でフォーマットする場合は、改行してから
				OnString(paction, cursorOld);
			}
		}
		CAction *paction = new CAction;
		cursorOld = m_pViewEd->m_cursor;
		paction->push_string(act_past, pdata->m_strData.data(), pdata->m_strData.size()); //strlen(psz)は、Win95では改行があるとダメ
		OnString(paction, m_pViewEd->m_cursor);
		if (pdata->m_fmt>CTanbunData::FMT_NO) {
			CUR cur = cursorOld.Befor();
			m_pViewEd->GetHint()->SetCur(cur);
			m_pViewEd->m_cursor.SetExpand(true);
			m_pViewEd->m_cursor.SetOrgCursor(cur);
			if (pdata->m_fmt==CTanbunData::FMT_CENTER)
				OnEdtlCenter();
			else if (pdata->m_fmt==CTanbunData::FMT_RIGHT)
				OnEdtlRight();
		}
		return true;
	}else
		m_pViewEd->m_cursor = cursorOld;
	return false;
#else
	StopFreeMsg();
	return false;
#endif
}

void CTheView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	TRACE("nChar=%d, nRepCnt=%d nFlags=%d\n", nChar, nRepCnt, nFlags);
	bool bCtrl = GetKeyState(VK_CONTROL)<0;
	bool bShift = GetKeyState(VK_SHIFT)<0;
	svbCommandFirst = true;

	//短文入力処理
	if (nChar==VK_SPACE && bCtrl) {
		if (DoTanbun())
			return;
	}

	if (bCtrl) {//改行以外では、コントロールキー押されてる場合は無視
		if (nChar==VK_NL)
			nChar = VK_RETURN;
		else
			return;
	}


	//ｲﾝﾃﾞﾝﾄ処理
	if ((nChar==VK_TAB || nChar==VK_SPACE) && 
		m_pViewEd->m_cursor.IsExpand() && m_pViewEd->m_cursor.Befor().D.x==0 && m_pViewEd->m_cursor.End().D.x==0) {
		string strT;
		strT.append(1,(char)nChar);
		if (bShift) {
			OnEdtTextFormat(null, &strT, (Normal_Proc)SStDP_CopyProc_line);
		}else{
			OnEdtTextFormat(&strT, null, (Normal_Proc)SStDP_CopyProc_line);
		}
		return;
	}

	MSG msg;
	CAction *paction = new CAction;
	
	if (!paction->push_char(nChar)) {
		delete paction;
		return;
	}


	//オートインデント処理 - ｺﾝﾄﾛｰﾙｷｰと共にの場合はｲﾝﾃﾞﾝﾄなし！
	if (nChar==VK_RETURN  && m_pViewEd->GetEnv()->m_bAutoIndent && !bCtrl) {
		string strT = m_pViewEd->GetHint()->GetPrevPartStringOfLine(m_pViewEd->GetBlinkCursor());
		string strNoSp = strT;
		TrimLeft(strNoSp);
		OBJSIZE len = strT.size() - strNoSp.size();
		OBJOFF len_header=0, off_num = NPOS;
		CLineHeader lh;
		lh.Initialize((BYTE*)strNoSp.data());
		len_header = lh.GetLength(); //ScanHitLineHeader((BYTE*)strNoSp.data(), &off_num);
		if (len>0)
			paction->push_string(act_input, strT.data(), len); //paction->push_string(act_input, strT.substr(0, len).c_str(), len);
		if (len_header>0) {
			if (bShift){
				paction->push_char(' ', len_header);
			}else{
				//IncLineHeaderNumStr(strNoSp);
				lh.GetNextLineHeader(strNoSp);
				paction->push_string(act_input, strNoSp.data(), strNoSp.size());
			}
		}
	}else{
		while (/*!IsDelete(nChar) &&*/ ::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message==WM_KEYDOWN) {
				TranslateMessage(&msg);
				paction->push_char(nChar);
				continue;
			}
			if (msg.message==WM_CHAR) {
				paction->push_char(nChar);
				continue;
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	CCursor cursorBef = m_pViewEd->m_cursor;
	//bool bBackCRLF=false;
	if (IsDelete(nChar)) {
		paction->m_act = nChar == VK_BACK ? act_delete_back:act_delete_del;
		if (!m_pViewEd->m_cursor.IsExpand()) {
			if (nChar == VK_BACK) {
				m_pViewEd->MoveCoursor(m_pViewEd->IsVert() ? VK_UP:VK_LEFT, false, true, 1);
				//HACK!!!!
				//if (m_pViewEd->m_cursor.IsExpand()) {
				//	CUR cur = m_pViewEd->m_cursor.GetOrgCursor();
				//	assert(nRepCnt==1);
				//	if (cur.D.x==0)
				//		bBackCRLF = true;
				//	if (bBackCRLF) {
				//	}
				//}
			}else if (nChar == VK_MBUTTON/*VK_DELETE*/)
				m_pViewEd->MoveCoursor(m_pViewEd->IsVert() ? VK_DOWN:VK_RIGHT, false, true, 1);
		}
	}

	OnString(paction, cursorBef);
	//	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CTheView::OnString(CAction *paction,  CCursor &cursorBef) 
{
	CLineHint *pHint = m_pViewEd->GetHint();
	CCursor cursorOld = m_pViewEd->m_cursor;
	
	
	//更新前：カーソル情報取得
	CUR curBef = cursorOld.Befor(); //現在カーソルの最初をデフォルトセット
	OBJOFF row = curBef.D.y;
	if (curBef.D.x == 0 && pHint->GetThis_LineTop(row) < row) {
		//直前行の行末にセット
		curBef.D.y = row-1;
		curBef.D.x = NPOS;
		//pHint->SetCur(curBef, false);
	}
	OBJOFF rowNext = pHint->GetNext_LineTop(cursorOld.End().D.y);//直後行セット

	//改行が含まれた処理か否か？
	bool bStrCRLF;
	if (!IsValidDataPStr(paction->m_pstring)) {
		bStrCRLF = (cursorOld.IsExpand() && cursorOld.GetBlinkCursor().D.y!=cursorOld.GetOrgCursor().D.y);
	//act==act_input || act==act_past
	}else if (paction->m_pstring->size()>80)//あまり大きな文字列で見つけるのは時間がかかるので含まれるとする
		bStrCRLF = true;
	else
#if bug_111201
		bStrCRLF = paction->m_pstring->find(VK_RETURN)!=string::npos;
#else
		bStrCRLF = paction->m_pstring->find(VK_RETURN)!=string::npos || paction->m_pstring->find(VK_NL)!=string::npos;
#endif

	//メモリ内での更新処理
	if (paction->IsEmpty() || m_pViewEd->Action(paction)==0) {
		delete paction;
		return;
	}
	GetDocument()->m_pMainObjLay->GetActions().push_action(paction);

	
	//更新後：カーソル情報取得
	row = m_pViewEd->m_cursor.End().D.y; //現在カーソルの最初をデフォルトセット
	OBJOFF rowNext_After = pHint->GetNext_LineTop(row);//直後行セット
	CUR curLast;
	curLast.D.y = pHint->GetNext_LineTop(row);//直後行の行頭にセット
	curLast.D.x = 0;
	if (curLast.D.y == pHint->size()) {
		//該当論理行の最後を指す
		curLast.D.y = SIZEMAC(pHint->size());
		curLast.D.x = NPOS;
	}
	//pHint->SetCur(curLast, false); 


	//更新とスクロール
	CCursor cursor_update;
	cursor_update.SetCursor(&curLast, &curBef);
	//cursor_update.SetExpand();
	//cursor_update.SetOrgCursor(curBef);
	//cursor_update.SetBlinkCursor(curLast);
	ScrollForSell(cursorOld, paction->m_act, false, true, cursor_update, rowNext, rowNext_After, bStrCRLF);
}

bool CTheView::ScrollForInput(OBJOFF rowNext, OBJOFF rowNext_After, bool bStrCRLF, bool bDelCRLFCheck)
{
	CLineHint *pHint = m_pViewEd->GetHint();
	int linesExpand = rowNext_After - rowNext;
	bool bScroll=false;
	//カーソルに変化領域があれば、リドローする
/*	CCursor cursor_update= m_pViewEd->m_cursor;
	CUR cur = cursor_update.End();
	if (act!=act_delete && !cursor_update.IsExpand() && cursorOld==cursor_update) {
		//選択状態ではなくてカーソルが変わらない場合は、直前行への追い込みぶら下げがあやしい
		cur.D.y = SIZEMAC(cur.D.y);
		cur.D.x = 0;
	}else{
		cur.D.y = pHint->GetNext_LineTop(cur.D.y);
		if (cur.D.y>0)
			cur.D.y --;	//最後の行をセット
		cur.D.x = NPOS;
	}
	cursor_update.SetBlinkCursor(cur);
*/
	//行の増減だけ取得
	if (linesExpand!=0 || bStrCRLF) {
		CRect rcNum, rcClient = m_rcClient;
		bool bVert = m_pViewEd->IsVert();
		int xAmount=0, yAmount=0;
		CSize sizeFont = m_pViewEd->GetSizeCell();
		CPoint pt;

		if (linesExpand<0)
			rowNext--;//更新前の次の行位置
		else if (linesExpand>0)
			rowNext++;

		pt = m_pViewEd->Sell2Point_Win(0, rowNext);

		if (!bVert) {
			yAmount = sizeFont.cy * linesExpand;
			rcClient.top = pt.y - yAmount;
			if (bDelCRLFCheck) {
				if (rcClient.top==(sizeFont.cy*2)) {
					svbDontScroll = true;//1行目へ行く場合は止める。
				}else{
				//	InvalidateRect(null);
				}
			}
		}else{
			linesExpand = -linesExpand;
			xAmount = sizeFont.cy * linesExpand;
			rcClient.right = pt.x - xAmount;
			if (bDelCRLFCheck) {
				if ((rcClient.right + (sizeFont.cy*2)) == m_rcClient.right) {
					svbDontScroll = true;//1行目へ行く場合は止める。
				}else{
				//	InvalidateRect(null);
				}
			}
		}

		rcNum = rcClient;

		if ((bScroll = !svbDontScroll) && linesExpand!=0) {
			ScrollWindow( xAmount, yAmount, &rcClient);
			//選択有り時の複数行更新
			if (!bVert) {// && linesExpand < -1) {
				rcClient.top = rcClient.bottom + yAmount;
				InvalidateRect(&rcClient);
			}else{// if (linesExpand > 1) {
				rcClient.right = rcClient.left + xAmount;
				InvalidateRect(&rcClient);
			}
		}

		//行番号エリアの更新
		if (!bVert) {
			rcNum.right = m_pViewEd->GetViewOrg().x;
			rcNum.top += yAmount;
		}else{
			rcNum.bottom = m_pViewEd->GetViewOrg().x;
			rcNum.right += xAmount;
		}
		InvalidateRect(&rcNum);
		
		m_bDirtyView = true;
	}

	m_bInputView = true;
	//Updateする範囲を取得
//	return cursor_update.MargeCursor(cursorOld.Befor()<=m_pViewEd->m_cursor.Befor() ?
//						cursorOld: m_pViewEd->m_cursor);
	return bScroll;
}
bool CTheView::SetScrollPos_Auto(int nBar, int nPos, int nPosOld, bool bVSwitch)
{
	if (nPos==nPosOld)
		return false;
	CSize sizeFont = /*bVSwitch ? m_pViewEd->GetSizeCell():*/m_pViewEd->VGetSizeCell();
	CRect rc = m_rcClient;
	int xAmount =0, yAmount = 0;
	if (bVSwitch)
		nBar = m_pViewEd->VSwitchBar(nBar);
	if (nBar==SB_HORZ) {
		xAmount = (nPosOld - nPos)*sizeFont.cx;
		if (m_pViewEd->IsVert()) {
			xAmount = (-1 * xAmount);
		}
		if (xAmount<0)
			rc.left = rc.right + xAmount -1;
		else {
			rc.right = rc.left + xAmount;
			//rc.right += rc.right + sizeFont.cx;
		}
	}else{
		yAmount = (nPosOld - nPos)*sizeFont.cy;
		if (yAmount<0) {
			rc.top = rc.bottom + yAmount;
			//rc.top -= rc.bottom % sizeFont.cy;
		}else
			rc.bottom = rc.top + yAmount;
	}
	CRect rcClient = m_rcClient;
	//rcClient.left = 0;
	//rcClient.top = 0;
	ScrollWindow( xAmount, yAmount, &rcClient);
	m_pViewEd->VSetScrollPos_NoSwitch(nBar, nPos);
	//ValidateRect(NULL);
	InvalidateRect(&rc);
	//UpdateWindow();
	return true;
}
void CTheView::ScrollForSell(CCursor cursorOld, ACT act, bool bLineTop, bool bInput, CCursor cursor_update, OBJOFF rowNext, OBJOFF rowNext_After, bool bStrCRLF)
{
	ShowBlinkCursor(false);
	//カーソル行桁表示
//	SetPaneText_CurPos();
#ifdef AUTO_SCROLL	
	bool bScroll2DocEnd = false;
#endif
	bool bUpdate = false;

	svbDontScroll = false;

	CSize sizeFont = m_pViewEd->GetSizeCell();
	CUR_SIZE cursize = m_pViewEd->GetCurSize();


	SCROLLINFO sbh_info;
	m_pViewEd->VGetScrollInfo(SB_HORZ, &sbh_info);
	int nPosH_Old = sbh_info.nPos;
	SCROLLINFO sbv_info;
	m_pViewEd->VGetScrollInfo(SB_VERT, &sbv_info);
	int nPosV_Old = sbv_info.nPos;

	if (bInput) {
		CUR cur = cursor_update.GetOrgCursor();
		ScrollForInput(rowNext, rowNext_After, bStrCRLF, IsActDelete(act) && cur.D.x==0 && cur.D.y>=1);
		if (!IsActDelete(act) ? comp(m_pViewEd->m_cursor, cursorOld)<=0 ||
			abs((long)cursorOld.GetBlinkCursor().D.x - (long)m_pViewEd->m_cursor.GetBlinkCursor().D.x)>2:
			(act==act_delete_del ? comp(m_pViewEd->m_cursor, cursorOld)!=0:
			abs((long)cursorOld.GetBlinkCursor().D.x - (long)m_pViewEd->m_cursor.GetBlinkCursor().D.x)>2
					) ) {
			CCursor cursor;
			cur = cursorOld.GetBlinkCursor();
			if (cur.D.y>0)
				cur.D.y --;
			cursor.SetCursor(&cur);
			cursor_update = cursor_update.MargeCursor(cursor);
		}

		//if (m_bDirtyView)
		if (cursor_update.IsExpand()) {
			m_pViewEd->InvalidateCursor(this, cursor_update, nPosH_Old, nPosV_Old);
			bUpdate = true;
		}
	}

	CPoint pt(0,0);
	pt.x = sbh_info.nPos * sizeFont.cx;//GetScrollPos();
	pt.y = sbv_info.nPos * sizeFont.cy;//GetScrollPos();

	CRect rcClient, rcCursor;
	rcClient = m_pViewEd->VSwitchRect_View(m_rcClient);
	rcCursor = m_pViewEd->VSwitchRect_View(m_pViewEd->GetBlinkCursorRect(true));

	//上下１行づつ空ける
	if (rcCursor.top>=sizeFont.cy)
		rcCursor.top-=sizeFont.cy;
	rcCursor.bottom+=sizeFont.cy;

	bool bScroll=false;
	if (bLineTop || rcCursor.left < pt.x) {
		if (bLineTop || rcCursor.right < rcClient.right)
			pt.x = 0;
		else {
			pt.x -= max((pt.x - rcCursor.left) , max(rcClient.Width()/3,1));
			pt.x = DIVMOD(pt.x, sizeFont.cx);
		}
		//if (pt.x == sbh_info.nPos)
		//	pt.x--;
		if (pt.x<0)
			pt.x = 0;
		if (m_pViewEd->GetBlinkSell().x < sbh_info.nMax && sbh_info.nMax>cursize.cx) {
			sbh_info.nMax= max(m_pViewEd->GetBlinkSell().x + 1,cursize.cx);
			sbh_info.nPos = pt.x;
			//sbh_info.nPage = max(rcClient.Height() / sizeFont.cx, 1);
			sbh_info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
			m_pViewEd->VSetScrollInfo(SB_HORZ, &sbh_info, false);
		}
		bScroll = SetScrollPos_Auto(SB_HORZ, pt.x, nPosH_Old, true) || bScroll;//ScrollToPosition(pt);
	}else
	if (rcCursor.right > (pt.x + rcClient.right)) {
		pt.x += max((rcCursor.right - (pt.x + rcClient.right)), max(rcClient.Width()/3,1));
		pt.x = DIVMOD(pt.x, sizeFont.cx);
		//if (pt.x == sbh_info.nPos)
		//	pt.x++;
		if (sbh_info.nPage+pt.x>=sbh_info.nMax) {
			sbh_info.nMax= sbh_info.nPage + pt.x;
			sbh_info.nPos = pt.x;
			//sbh_info.nPage = max(rcClient.Height() / sizeFont.cx, 1);
			sbh_info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
			m_pViewEd->VSetScrollInfo(SB_HORZ, &sbh_info, false);
		}
		bScroll = SetScrollPos_Auto(SB_HORZ, pt.x, nPosH_Old, true) || bScroll;//ScrollToPosition(pt);
	}else
		pt.x = nPosH_Old;

	if (rcCursor.top < pt.y) {
		pt.y -= max((pt.y - rcCursor.top), 1); //((rcClient.Height())/2);
		pt.y = DIVMOD(pt.y, sizeFont.cy);
		//if (pt.y == sbv_info.nPos)
		//	pt.y--;
		if (pt.y<0)
			pt.y = 0;
		if (m_pViewEd->GetBlinkSell().y < sbv_info.nMax && sbv_info.nMax>cursize.cy) {
#ifdef AUTO_SCROLL
			int nMax = max(m_pViewEd->GetBlinkSell().y + 1,cursize.cy)+max(abs(sbv_info.nPage), 2)/2;
			if (sbv_info.nMax > nMax) {
				sbv_info.nMax= nMax;//max(m_pViewEd->GetBlinkSell().y + 1,cursize.cy);
				sbv_info.nPos = pt.y;
				//sbv_info.nPage = max(rcClient.Height() / sizeFont.cy, 1);
				sbv_info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
				m_pViewEd->VSetScrollInfo(SB_VERT, &sbv_info);
				m_pViewEd->ChangeLineNum();
			}
#endif
			int nMax = max(m_pViewEd->GetBlinkSell().y + 1,cursize.cy);
			if (sbv_info.nMax > nMax) {
				sbv_info.nMax = max(sbv_info.nMax - 1, cursize.cy);
				sbv_info.nPos = pt.y;
				//sbv_info.nPage = max(rcClient.Height() / sizeFont.cy, 1);
				sbv_info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
				m_pViewEd->VSetScrollInfo(SB_VERT, &sbv_info);
				m_pViewEd->ChangeLineNum();
			}
		}
		if (svbDontScroll && ((nPosV_Old - pt.y) == 1))
			m_pViewEd->VSetScrollPos_NoSwitch(m_pViewEd->VSwitchBar(SB_VERT), pt.y);
		else {
			bScroll = SetScrollPos_Auto(SB_VERT, pt.y, nPosV_Old, true) || bScroll;//ScrollToPosition(pt);
		}

	}else
	if (rcCursor.bottom > (pt.y + rcClient.bottom)) {
		pt.y += max((rcCursor.bottom - (pt.y + rcClient.bottom)), 1);//((rcClient.Height())/2);
		pt.y = DIVMOD(pt.y, sizeFont.cy);
		//if (pt.y == sbv_info.nPos)
		//	pt.y++;
		if (sbv_info.nPage + pt.y>=sbv_info.nMax) {
			sbv_info.nMax= sbv_info.nPage + pt.y;
			sbv_info.nPos = pt.y;
			//sbv_info.nPage = max(rcClient.Height() / sizeFont.cy, 1);
			sbv_info.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
#ifdef AUTO_SCROLL	
			if (bInput) {
				sbv_info.nMax += max(abs(sbv_info.nPage), 2)/2;
				bScroll2DocEnd = true;
			}
#endif
			m_pViewEd->VSetScrollInfo(SB_VERT, &sbv_info);
			m_pViewEd->ChangeLineNum();
		}
		bScroll = SetScrollPos_Auto(SB_VERT, pt.y, nPosV_Old, true) || bScroll;//ScrollToPosition(pt);
	}else
		pt.y = nPosV_Old;


//	if (bScroll && bInput)
//		m_bDirtyView = true;

	//カーソルに変化領域があれば、リドローする
	cursor_update =  m_pViewEd->m_cursor.DiffCursor(cursorOld);
	if (cursor_update.IsExpand()) {
		m_pViewEd->InvalidateCursor(this, cursor_update, pt.x, pt.y);
		bUpdate = true;
	}else if (bScroll) {
		bUpdate = true;
	}
	if (bUpdate) {
		UpdateWindow();
	}
#ifdef AUTO_SCROLL	
//	if (bScroll2DocEnd) {
//		//文末へスクロール
//		DoScroll(m_pViewEd->GetEnv()->m_bVert ? SB_PAGELEFT:SB_PAGEDOWN, 0,
//			m_pViewEd->GetEnv()->m_bVert ? SB_HORZ:SB_VERT);
//	}
#endif
	
	ShowBlinkCursor(true);


	if (act!=act_move && act!=act_input_ime && act!=act_copy)
		UpdateOtherView();
}

void CTheView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
//	if (m_pViewEd->m_bAct)
		DoScroll(nSBCode, nPos, SB_HORZ);
//	else
//		CView::OnHScroll(nSBCode, nPos, pScrollBar);

	//CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTheView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
//	if (m_pViewEd->m_bAct)
		DoScroll(nSBCode, nPos, SB_VERT);
//	else
//		CView::OnVScroll(nSBCode, nPos, pScrollBar);

	//CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTheView::DoScroll(UINT nSBCode, UINT nPos, int sb) 
{
	//if (((CMainFrame*)AfxGetMainWnd())->GetActiveView()->m_hWnd!=m_hWnd) //他の分割ビューメッセージだ！
	//	return;

	static int scount_rep = 0;
	static UINT nSBCode_save = 0;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if (nSBCode==SB_ENDSCROLL)
		return;
	nSBCode = m_pViewEd->VSwitchSBCode(nSBCode, sb);
	SCROLLINFO sb_info;
	m_pViewEd->VGetScrollInfo_NoSwitch(sb, &sb_info);
	int pos = sb_info.nPos;
//	TRACE("nSBCode=%d, pos=%d\n",nSBCode, pos);

	if (nSBCode==nSBCode_save) {
		if (scount_rep<10)
			scount_rep++;
	}else {
		scount_rep = 0;
		nSBCode_save = nSBCode;
	}

	switch(nSBCode)	{
	//case SB_THUMBTRACK:
	//	return;
	//	break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		pos = sb_info.nTrackPos;
		break;
	case SB_TOP:
	//case SB_LEFT:
		//pos = sb_info.nMin;
		pos = nPos;
		break;
	case SB_LINEUP:
	//case SB_LINERIGHT:
		if (scount_rep>=10)
			pos -=2;
		else
			pos --;
		break;
	case SB_LINEDOWN:
	//case SB_LINELEFT:
		if (scount_rep>=10)
			pos +=2;
		else
			pos ++;
		break;
	case SB_PAGEUP:
	//case SB_PAGELEFT:
		pos -= max(sb_info.nPage-1, 1);
		break;
	case SB_PAGEDOWN:
	//case SB_PAGERIGHT:
		pos += max(sb_info.nPage-1, 1);
		break;
	case SB_BOTTOM:
	//case SB_RIGHT:
		//pos = sb_info.nMax;
		pos = nPos;
		break;
	default:
//		ValidateRect(NULL);
		return;
		break;
	}
	if (pos<sb_info.nMin)
		pos = sb_info.nMin;
	else if (pos>(sb_info.nMax-sb_info.nPage))
		pos = (sb_info.nMax-sb_info.nPage);
//	else if (pos>sb_info.nMax)
//		pos = sb_info.nMax;

	//TRACE("pos=%d\n",pos);
	
	ValidateRect(NULL);

	if (SetScrollPos_Auto(sb, pos, sb_info.nPos))
		UpdateWindow();

	//((CMainFrame*)AfxGetMainWnd())->
	//	m_wndRulerBar.m_unit.SetDateOrg(GetScrollPos());

	//CPlanView* pView = ((CMainFrame*)AfxGetMainWnd())->m_pPlanView;
	//pView->Invalidate();
}


void CTheView::ShowBlinkCursor(bool bShow, bool bClear_IdleCount)
{
	TRACE("------CTheView::ShowBlinkCursor() call%s ----\n",theViewEnvTbl.m_datetimes.GetThisTimeString_First().c_str());
	if (!theIme.IsNotCompCursor()) {
		//TRACE("Caret %s\n", bShow ?"Show!":"Hid");
		m_pViewEd->DrawBlinkCursor(this, bShow);
	}
	if (bShow && bClear_IdleCount)
		SetPaneText_CurPos();

	if (bClear_IdleCount)
		m_idleCount =0;
}
void CTheView::OnSetFocus(CWnd* pOldWnd) 
{
	TRACE("\nCTheView::OnSetFocus()!");
	GetDocument()->UpdateAllViews(null, HINT_LOST_FOCUS_VIEW, this);
	GetDocument()->UpdateAllViews(null, HINT_FOCUS_VIEW, this);

	if (m_pViewEd) {
		m_pViewEd->ResetCaret(this);
	}
	((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.UpdateTip();

	CView::OnSetFocus(pOldWnd);

}
void CTheView::OnKillFocus(CWnd* pNewWnd) 
{
	TRACE("\nCTheView::OnKillFocus()!");
	if (m_pViewEd) {
		m_pViewEd->ResetCaret(null);
		//m_pViewEd->Detach();
	}

	CView::OnKillFocus(pNewWnd);
}
void CTheView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	TRACE("\nCTheView::OnActivateView(bActivate=%d)!", bActivate);
#if BUG_20111203
	if (bActivate) {
		if (m_pViewEd) {
			Invalidate();
			UpdateWindow();
		}
	}
#endif
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
///////////////////////////////////////////////////////////////////////////////////////
// menue /toolbar interface handling

void CTheView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_pMainObjLay->GetActions().IsEnableUndo());
}

void CTheView::OnEditUndo() 
{
	CAction action(act_undo);
	CCursor cursorOld = m_pViewEd->m_cursor;

	if (m_pViewEd->Action(&action)!=0) {
		return;
	}

	GetDocument()->SetDirtyDocTitle(GetDocument()->m_pMainObjLay->GetActions().IsEnableUndo());

	ScrollForSell(cursorOld);
	Invalidate();
}

void CTheView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_pMainObjLay->GetActions().IsEnableRedo());
}

void CTheView::OnEditRedo() 
{
	CAction action(act_redo);
	CCursor cursorOld = m_pViewEd->m_cursor;

	if (m_pViewEd->Action(&action)!=0) {
		return;
	}
	GetDocument()->SetDirtyDocTitle();

	ScrollForSell(cursorOld);
	Invalidate();
}


LRESULT CTheView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (m_pViewEd && theIme.ImeWndProc(this, message, wParam, lParam))
		return 0;
	return CView::DefWindowProc(message, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////////////
/////////// Edit menu or Text Format menu HANDLING
//////////////////////////////

void CTheView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnEditFind() 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	string str = m_pViewEd->GetCursorWord();
	svdlgFind.OnEditFind(this, str.size()>0 ? str.c_str():"");
}

void CTheView::OnUpdateEditFindPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(svdlgFind.m_strFind.GetLength()>0);
}

void CTheView::OnEditFindPrev() 
{
	svdlgFind.OnEditFindPrev(this);
}

void CTheView::OnUpdateEditFindNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(svdlgFind.m_strFind.GetLength()>0);
}
void CTheView::OnEditFindNext()
{
	svdlgFind.OnEditFindNext(this);
}

void CTheView::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnEditReplace() 
{
	string str = m_pViewEd->GetCursorWord();
	svdlgFind.OnEditReplace(this, str.size()>0 ? str.c_str():"");
}

//////////////////////////////////////////////////////////////////////
// config

void CTheView::OnUpdateConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::UpdateViewEnv(bool bVert_Old, bool bChangeFormat)
{
	//表示に関係する変更項目
	//GetClientRect(&m_rcClient);
	CCursor cursorOld = m_pViewEd->m_cursor;

	AfxGetApp()->DoWaitCursor(-1); 

	//format change?
	if (bChangeFormat) {
		m_pViewEd->GetEnv()->SetFocusEnv();
		m_pViewEd->ReBuildHint();
	}

	//change colors
	GetDocument()->UpdateAllViews(null, HINT_LOST_FOCUS_VIEW, this);
	GetDocument()->UpdateAllViews(null, HINT_FOCUS_VIEW, this);
	//orientation change
	if (bVert_Old != m_pViewEd->GetEnv()->m_bVert) {
		//if (m_pViewEd->IsVert())
		//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_RIGHT);
		//else
		//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_TOP);
		m_pViewEd->GetEnv()->m_bVert = !m_pViewEd->GetEnv()->m_bVert;
		m_pViewEd->SetScrolls(this, m_rcClient, true);
	}else{
		//font change
		m_pViewEd->ChangeFonts(this, m_rcClient);
	}

	//line number
	//m_pViewEd->ChangeLineNum();

	AfxGetApp()->DoWaitCursor(-1); 

	//For update!
	if (bVert_Old != m_pViewEd->GetEnv()->m_bVert) {
		//if (m_pViewEd->IsVert())
		//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_RIGHT);
		//else
		//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_TOP);
//		SetClassLong(GetSafeHwnd(), GCL_HCURSOR,
//			(long)(m_pViewEd->IsVert() ? AfxGetApp()->LoadCursor(IDC_CUR_VERT):
//			AfxGetApp()->LoadStandardCursor(IDC_IBEAM))); 
	}else{
		m_pViewEd->SetScrolls(this, m_rcClient);
		AfxGetMainWnd()->Invalidate();
	}
	
	if (bChangeFormat)
		ScrollForSell(cursorOld);
}

void CTheView::OnConfig() 
{
	CConfigSheet sheet;
	sheet.UpdateEnv(false, m_pViewEd->GetEnv());
	if (sheet.DoModal()==IDOK) {
		bool bVert_Old = m_pViewEd->GetEnv()->m_bVert;
		if (int iChange = sheet.UpdateEnv(true, m_pViewEd->GetEnv())) {
			UpdateViewEnv(bVert_Old, sheet.IsChangeFormat()==TRUE);
		}
	}
}
void CTheView::OnUpdateLimitcol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->GetEnv()->m_xdspMax != SHRT_MAX);
}

void CTheView::OnLimitcol() 
{
	if (!GetDocument()->m_pMainObjLay->IsFormatAll()) {
		AfxMessageBox("ﾛｰﾃﾞｨﾝｸﾞ中は、行折返し設定を変更できません。");
		return;
	}
	CCursor cursorOld = m_pViewEd->m_cursor;
	AfxGetApp()->DoWaitCursor(1); 
	GetClientRect(&m_rcClient);
	if (m_pViewEd->GetEnv()->m_xdspMax == SHRT_MAX)
		m_pViewEd->GetEnv()->m_xdspMax = m_pViewEd->GetEnv()->m_xdspMax_save;
	else
		m_pViewEd->GetEnv()->m_xdspMax = SHRT_MAX;
	m_pViewEd->ReBuildHint();
	AfxGetApp()->DoWaitCursor(-1);
	m_pViewEd->SetScrolls(this, m_rcClient);
	ScrollForSell(cursorOld);
	ShowBlinkCursor(true);
	//	SetPaneText_CurPos();
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnUpdateFreeCur(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->GetEnv()->m_bFreeCursor);
}

void CTheView::OnFreeCur() 
{
	m_pViewEd->GetEnv()->m_bFreeCursor = !m_pViewEd->GetEnv()->m_bFreeCursor;
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnUpdateVertical(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv() && svnViewCount==1);
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->IsVert());
}

void CTheView::OnVertical()
{
	GetClientRect(&m_rcClient);
	m_pViewEd->SetScrolls(this, m_rcClient, true);
	
	//GetDocument()->UpdateAllViews(this, HINT_VERTICAL_VIEW);

	//if (m_pViewEd->IsVert())
	//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_RIGHT);
	//else
	//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_TOP);

	theIme.SetCandidateFont(this);

	//DocToolBar()でのツールバー移動が、Invalidate()を呼んでくれる！
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();

//	SetClassLong(GetSafeHwnd(), GCL_HCURSOR,
//		(long)(m_pViewEd->IsVert() ? AfxGetApp()->LoadCursor(IDC_CUR_VERT):
//		AfxGetApp()->LoadStandardCursor(IDC_IBEAM))); 
}

void CTheView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnZoomIn() 
{
	CViewEnv *pEnv = m_pViewEd->GetEnv();
	
	GetClientRect(&m_rcClient);
	m_pViewEd->ChangeFonts(this, m_rcClient, 1);
	m_pViewEd->ChangeLineNum();
	m_pViewEd->SetScrolls(this, m_rcClient);
	m_pViewEd->ResetCaret(this);
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnZoomOut() 
{
	CViewEnv *pEnv = m_pViewEd->GetEnv();
	
	GetClientRect(&m_rcClient);
	m_pViewEd->ChangeFonts(this, m_rcClient, -1);
	m_pViewEd->ChangeLineNum();
	m_pViewEd->SetScrolls(this, m_rcClient);
	m_pViewEd->ResetCaret(this);
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}


void CTheView::OnUpdateLinenum(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->GetEnv()->BShowLineNum());
}

void CTheView::OnUpdateLinenumLine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->GetEnv()->BShowLineNum() && !m_pViewEd->GetEnv()->m_bRowNum);
}

void CTheView::OnUpdateLinenumNo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(!m_pViewEd->GetEnv()->BShowLineNum());
}

void CTheView::OnUpdateLinenumRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
	if (m_pViewEd->IsValidEnv())
		pCmdUI->SetCheck(m_pViewEd->GetEnv()->BShowLineNum() && m_pViewEd->GetEnv()->m_bRowNum);
}

void CTheView::OnLinenum() 
{
	m_pViewEd->GetEnv()->BShowLineNum() = !m_pViewEd->GetEnv()->BShowLineNum();
	m_pViewEd->ChangeLineNum();
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnLinenumLine() 
{
	m_pViewEd->GetEnv()->BShowLineNum() = true;
	m_pViewEd->GetEnv()->m_bRowNum = false;
	m_pViewEd->ChangeLineNum();
	ShowBlinkCursor(true);
	//SetPaneText_CurPos();
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnLinenumNo() 
{
	m_pViewEd->GetEnv()->BShowLineNum() = false;
	m_pViewEd->ChangeLineNum();
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}

void CTheView::OnLinenumRow() 
{
	m_pViewEd->GetEnv()->BShowLineNum() = true;
	m_pViewEd->GetEnv()->m_bRowNum = true;
	m_pViewEd->ChangeLineNum();
	ShowBlinkCursor(true);
	//SetPaneText_CurPos();
	Invalidate();
	m_pViewEd->GetEnv()->SetDirty();
}


BOOL CTheView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
//	m_pViewEd->m_brushTextBk.UnrealizeObject();
//	pDC->FillRect(m_rcClient, &m_pViewEd->m_brushTextBk);
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
void CTheView::SetPaneText_CurPos()
{
	//カーソル行桁表示
	CUR cur = m_pViewEd->GetBlinkCursor();
	if (m_pViewEd->GetEnv()->m_bRowNum || m_pViewEd->GetEnv()->m_xdspMax == SHRT_MAX)
		((CMainFrame*)AfxGetMainWnd())->SetPaneText_CurPos(cur.D.y+1, cur.D.x+1);
	else {
		assert(m_pViewEd->m_pObjLayTbl);
		SELL sell = m_pViewEd->GetHint()->at(m_pViewEd->GetHint()->GetThis_LineTop(cur.D.y));
		assert(cur.L.off-sell.off>=0);
		((CMainFrame*)AfxGetMainWnd())->SetPaneText_CurPos(sell.line, cur.L.off-sell.off+1);
	}
}

void CTheView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!theIme.IsInComposition()) {
		CMenu popMenu;
		popMenu.LoadMenu(IDR_POPMENU_EDIT);
		AfxGetMainWnd()->ClientToScreen(&point);
		popMenu.EnableMenuItem(ID_EDIT_UNDO, GetDocument()->m_pMainObjLay->GetActions().IsEnableUndo() ? MF_ENABLED:MF_GRAYED);
		popMenu.EnableMenuItem(ID_EDIT_REDO, GetDocument()->m_pMainObjLay->GetActions().IsEnableRedo() ? MF_ENABLED:MF_GRAYED);
		popMenu.EnableMenuItem(ID_EDIT_CUT, m_pViewEd->m_cursor.IsExpand() ? MF_ENABLED:MF_GRAYED);
		popMenu.EnableMenuItem(ID_EDIT_COPY, m_pViewEd->m_cursor.IsExpand() ? MF_ENABLED:MF_GRAYED);
		popMenu.EnableMenuItem(ID_EDIT_PASTE, IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED:MF_GRAYED);
		popMenu.EnableMenuItem(IDC_FILE_CLOSE, (MF_ENABLED));
		popMenu.EnableMenuItem(ID_WINDOW_CLOSE_NO_THIS, ((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.GetWindowCount()>1 ? MF_ENABLED : MF_GRAYED);
		::TrackPopupMenu(popMenu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN, point.x, point.y, 0, m_hWnd, null);//AfxGetMainWnd());
		CView::OnRButtonDown(nFlags, point);
	}
}

void CTheView::ShowFSBarMenu(UINT nFlags, CPoint point)
{
	if (!theIme.IsInComposition()) {
		CMenu popMenu;
		popMenu.LoadMenu(IDR_FSBARMENU_EDIT);
		AfxGetMainWnd()->ClientToScreen(&point);
		popMenu.EnableMenuItem(ID_FILE_SAVE, (GetDocument()->IsModified() &&
			GetDocument()->m_pMainObjLay && GetDocument()->m_pMainObjLay->IsFormatAll()) ? MF_ENABLED : MF_GRAYED);
		popMenu.EnableMenuItem(IDC_FILE_CLOSE, MF_ENABLED);
		popMenu.EnableMenuItem(ID_WINDOW_CLOSE_NO_THIS, ((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.GetWindowCount()>1 ? MF_ENABLED : MF_GRAYED);
		::TrackPopupMenu(popMenu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN, point.x, point.y, 0, m_hWnd, null);//AfxGetMainWnd());
	}
}


void CTheView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnEditSelectAll() 
{
/*	CUR cur;
	m_pViewEd->m_cursor.SetExpand();
	m_pViewEd->GetHint()->SetCur(cur);
	m_pViewEd->m_cursor.SetOrgCursor(cur);
	cur.D.y=NPOS;
	cur.D.x=NPOS;
	m_pViewEd->GetHint()->SetCur(cur);
	m_pViewEd->m_cursor.SetBlinkCursor(cur);
*/
	CUR cur0, curNPOS;
	m_pViewEd->GetHint()->SetCur(cur0);
	curNPOS.D.y=NPOS;
	curNPOS.D.x=NPOS;
	m_pViewEd->GetHint()->SetCur(curNPOS);
	m_pViewEd->m_cursor.SetCursor(&curNPOS, &cur0);
	m_pViewEd->m_cursor.ResetExpand_IfCuremp();
	Invalidate();
}

void CTheView::OnJumpLine(OBJOFF lineJump, JUMPTYPE JumpType)
{
	assert(m_pViewEd->IsValidEnv());
	svdlgJump.OnJumpLine(this, lineJump, JumpType);
}

void CTheView::OnUpdateJump(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnJump()
{
	assert(m_pViewEd->IsValidEnv());
	svdlgJump.OnJumpLine(this, 0, m_pViewEd->GetEnv()->m_bRowNum ||
		m_pViewEd->GetEnv()->m_xdspMax == SHRT_MAX  ?  JUMP_ROW:JUMP_LINE);//takaso2010 bugix true/false->JUMPTYPE
}


void CTheView::OnUpdateEdtView(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(theViewEnvTbl.m_bEditMarkView);
}

void CTheView::OnEdtView() 
{
	theViewEnvTbl.m_bEditMarkView = !theViewEnvTbl.m_bEditMarkView;
	m_pViewEd->ResetDrawFlgs();
	Invalidate();	
}



void CTheView::OnEditCopy() 
{
	OnEditCopy_Hand();
}


void CTheView::OnEditCopyAppend() 
{
	OnEditCopy_Hand(null, true);
}



void CTheView::OnEditCopy_Hand(string *pstrAdd, bool bAppend) 
{
	if (OpenClipboard()){
		HGLOBAL hMem;
		SSDP_DATA data;
		memset(&data, 0, sizeof(data));
		if (bAppend && (hMem = GetClipboardData (CF_TEXT))) {
			BYTE *psz;
			if (psz = (BYTE*)GlobalLock (hMem)) {
				OBJOFF len = GlobalSize(hMem);
				len = ((BYTE*)memchr(psz, 0, len)) - psz;//BUGFIX:Win95では、GlobalSize(hTextData);は当てにならない
				data.hData = AppendSz2GData(0, data.lenData, psz, len);
				GlobalUnlock (hMem);
			}
		}
		if (::EmptyClipboard()) {
			CAction action(act_copy);
			data.pstrAdd = pstrAdd;
			HGLOBAL hTextData = (HGLOBAL) m_pViewEd->Action(&action, 
				IsValidDataPStr(pstrAdd) ? (Normal_Proc)SStDP_CopyProc_line:(Normal_Proc)SStDP_CopyProc_block,
				&data);

			if (hTextData == NULL ||
				#ifndef _UNICODE
				::SetClipboardData(CF_TEXT, hTextData) == NULL)
				#else
				::SetClipboardData(CF_UNICODETEXT, hTextData) == NULL)
				#endif
			{
				TRACE("Failed to set clipboard data ErrorCode=%d\n", GetLastError());
				TRACE("[%d]>%s\n", ::GlobalSize(hTextData),(char*)hTextData);
				GlobalFree(hTextData);
				::CloseClipboard();
				return;
			}
		}
		CloseClipboard();
	}

/*
  m_nIDClipFormat = RegisterClipboardFormat(_T("DaoView"));
	if (IsClipboardFormatAvailable (CF_UNICODETEXT) ||
		IsClipboardFormatAvailable (CF_OEMTEXT) ||
		IsClipboardFormatAvailable (CF_TEXT))
		EnableMenuItem (GetMenu (hwnd),MID_PASTESOURCE,	MF_ENABLED);
	else
		EnableMenuItem (GetMenu (hwnd),MID_PASTESOURCE,	MF_GRAYED);
	CloseClipboard ();
	fmtLink = RegisterClipboardFormat(TEXT("Link"));
*/
}
void CTheView::OnEditPaste() 
{
	OnEditPaste_Hand();
}
void CTheView::OnEditPaste_Hand(const char *szMark) 
{
	if (OpenClipboard ()) {
		HGLOBAL hMem;
		if (hMem = GetClipboardData (CF_TEXT)) {
			const char *psz;
			if (psz = (char*)GlobalLock (hMem)) {
				OBJOFF len = GlobalSize(hMem);
				string str;
				if (IsValidDataSz(szMark)) {
					Trans_LineHead_block(str, psz, len, true, szMark);
					psz = str.data();
					len = str.size();
				}else{
				//if (len>=0x8000 && len>2 && psz[len-1]=='\0' && psz[len-2]!='\0')
				//	len--;
				//else
					len = ((char*)memchr(psz, 0, len)) - psz;
				}
				CAction *paction = new CAction;
				paction->push_string(act_past, psz, len); //strlen(psz)は、Win95では改行があるとダメ
				GlobalUnlock (hMem);
				OnString(paction, m_pViewEd->m_cursor);
			}
		}
		CloseClipboard ();
	}
}

void CTheView::OnEditCut() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnEditCopy_Hand(null, false); //GetKeyState(VK_SHIFT)<0
	CAction *paction = new CAction;
	paction->push_char(VK_MBUTTON/*VK_DELETE*/);
	CCursor cursorBef = m_pViewEd->m_cursor;
	OnString(paction, cursorBef);
}

void CTheView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->m_cursor.IsExpand() && !theIme.IsInComposition());
	//m_pViewEd->IsValidEnv()
}

void CTheView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	OnUpdateEditCopy(pCmdUI);
}

void CTheView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
}

void CTheView::OnUpdateEdtlRefCopy(CCmdUI* pCmdUI) 
{
	OnUpdateEditCopy(pCmdUI);
}
void CTheView::OnUpdateEdtlRefPast(CCmdUI* pCmdUI) 
{
	OnUpdateEditPaste(pCmdUI);
}
void CTheView::OnUpdateEditCopyAppend(CCmdUI* pCmdUI) 
{
	OnUpdateEditCopy(pCmdUI);
}
bool CTheView::OnEdtTextFormat(string *pstrAdd, const string *pstrRemove, Normal_Proc pFunc)
{
	SSDP_DATA data;
	memset(&data, 0, sizeof(data));
	data.pstrAdd = pstrAdd;
	data.pstrRemove = pstrRemove;
	data.pHint = m_pViewEd->GetHint();
	bool bExpand = m_pViewEd->m_cursor.IsExpand();
	CCursor cursorOld;
	if (!bExpand)
		cursorOld = m_pViewEd->m_cursor;

	m_pViewEd->SetExpandLineCursor();
	//if (!m_pViewEd->m_cursor.IsExpand())
	//	return false;
	if (bExpand)
		cursorOld = m_pViewEd->m_cursor;

	CAction action(act_copy);
	HGLOBAL hTextData = (HGLOBAL) m_pViewEd->Action(&action, pFunc, &data);

	bool bChange = false;
	const char *psz = null;
	OBJOFF len = 0;
	if (psz = (char*)GlobalLock (hTextData)) {
		//len = GlobalSize(hTextData);
		len = data.lenData;	//BUGFIX:Win95では、GlobalSize(hTextData);は当てにならない
	}else{
		psz = szNil;
		len = 1;
	}
	assert(len>0);

	CAction *paction = new CAction;
	paction->push_string(act_past, psz, len-1); //strlen(psz)は、Win95では改行があるとダメ
	if (psz != szNil)
		GlobalUnlock (hTextData);
	OnString(paction, m_pViewEd->m_cursor);
	bChange = true;
	if (bExpand) {
		m_pViewEd->SetExpandLineCursor(&cursorOld);
		Invalidate();
	}else{
		CUR cur = cursorOld.Befor();
		if (pFunc == (Normal_Proc)SStDP_CopyProc_line_center ||
			pFunc == (Normal_Proc)SStDP_CopyProc_line_right) {
			//行末へカーソルセット
			cur.D.x = NPOS;
		}else{
			if (pstrAdd)
				cur.D.x = cursorOld.GetBlinkCursor().D.x + pstrAdd->size();
			else
				cur.D.x = 0;
		}
		m_pViewEd->GetHint()->SetCur(cur);
//		m_pViewEd->m_cursor.SetExpand(false);
//		m_pViewEd->m_cursor.SetBlinkCursor(cur);
		m_pViewEd->m_cursor.SetCursor(&cur);

	}

	return bChange;
}

//////////REF
void CTheView::OnEdtlRefCopy() 
{
	OnEditCopy_Hand(&theViewEnvTbl.m_strRef, GetKeyState(VK_SHIFT)<0);
}
void CTheView::OnEdtlRefPast() 
{
	OnEditPaste_Hand(theViewEnvTbl.m_strRef.c_str());
}

void CTheView::OnUpdateEdtlLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CTheView::OnEdtlLeft() 
{
#ifdef SHAR
	string strDumy;
	OnEdtTextFormat(null, &strDumy, (Normal_Proc)SStDP_CopyProc_line); //半角SPACE 全角SPACE タブ
#else
	StopFreeMsg();
#endif
}


void CTheView::OnUpdateEdtlCenter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv()/* && m_pViewEd->GetEnv()->m_xdspMax != SHRT_MAX*/);
}


void CTheView::OnEdtlCenter() 
{
#ifdef SHAR
	string strDumy;
	OnEdtTextFormat(null, &strDumy, (Normal_Proc)SStDP_CopyProc_line_center); //半角SPACE 全角SPACE タブ
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlRight(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlCenter(pCmdUI);
}


void CTheView::OnEdtlRight() 
{
#ifdef SHAR
	string strDumy;
	OnEdtTextFormat(null, &strDumy, (Normal_Proc)SStDP_CopyProc_line_right); //半角SPACE 全角SPACE タブ
#else
	StopFreeMsg();
#endif
}


void CTheView::OnUpdateEdtlRefRight(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	//OnUpdateEditCopy(pCmdUI);
}

void CTheView::OnEdtlRefRight() 
{
	if (theIme.IsInComposition())
		return;
#ifdef SHAR
	string strRemove = "";
	OnEdtTextFormat(&theViewEnvTbl.m_strRef, &strRemove, (Normal_Proc)SStDP_CopyProc_line);
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlRefLeft(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlRefRight(pCmdUI);
}

void CTheView::OnEdtlRefLeft() 
{
#ifdef SHAR
	string strRemove = ">＞";
	OnEdtTextFormat(null, &strRemove, (Normal_Proc)SStDP_CopyProc_line);
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlLineHeaderRight(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlRefRight(pCmdUI);
}


void CTheView::OnEdtlLineHeaderRight() 
{
#ifdef SHAR
//static CLineHeaderDlg svdlgLineHeader;
	if (GetKeyState(VK_SHIFT)<0 || svdlgLineHeader.m_bOpenDlg) {
		if(svdlgLineHeader.DoModal()!=IDOK)
			return;
	}
	string str = svdlgLineHeader.m_strLineHeader;
	OnEdtTextFormat(&str, null, (Normal_Proc)SStDP_CopyProc_line_header);
	//選択状態でなければ以降、自動ｲﾝﾃﾞﾝﾄﾓｰﾄﾞとする
	if (!m_pViewEd->m_cursor.IsExpand()) {
		m_pViewEd->GetEnv()->m_bAutoIndent = true;
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlLineHeaderLeft(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlRefRight(pCmdUI);
}

void CTheView::OnEdtlLineHeaderLeft() 
{
#ifdef SHAR
	string str = "";
	OnEdtTextFormat(null, &str, (Normal_Proc)SStDP_CopyProc_line_header);
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlIndentRight(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlRefRight(pCmdUI);
}


void CTheView::OnEdtlIndentRight() 
{
#ifdef SHAR
	string str = "\t";
	OnEdtTextFormat(&str, null, (Normal_Proc)SStDP_CopyProc_line);
	//選択状態でなければ以降、自動ｲﾝﾃﾞﾝﾄﾓｰﾄﾞとする
	if (!m_pViewEd->m_cursor.IsExpand()) {
		m_pViewEd->GetEnv()->m_bAutoIndent = true;
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlIndentLeft(CCmdUI* pCmdUI) 
{
	OnUpdateEdtlRefRight(pCmdUI);
}

void CTheView::OnEdtlIndentLeft() 
{
#ifdef SHAR
	string str = "\t";
	OnEdtTextFormat(null, &str, (Normal_Proc)SStDP_CopyProc_line);
#else
	StopFreeMsg();
#endif

}

void CTheView::OnUpdateEdtlIndentAuto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_pViewEd->GetEnv()->m_bAutoIndent);
}

void CTheView::OnEdtlIndentAuto() 
{
#ifdef SHAR
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_pViewEd->GetEnv()->m_bAutoIndent = !m_pViewEd->GetEnv()->m_bAutoIndent;
	m_pViewEd->GetEnv()->SetDirty();
#else
	StopFreeMsg();
#endif
}


void CTheView::OnUpdateRedraw(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CTheView::OnRedraw() 
{
	Invalidate();
}

void CTheView::OnUpdateFlpMark(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnFlpMark() 
{
#ifdef SHAR
	m_pViewEd->GetHint()->m_bookMarks.SetMark(m_pViewEd, null, bmark_flp /*takaso2010 bugfix true-> */);
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateFlpMarkJump(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnFlpMarkJump() 
{
#ifdef SHAR
	CCursor cursorOld = m_pViewEd->m_cursor;
	if (m_pViewEd->GetHint()->m_bookMarks.JumpUnnormalMark(m_pViewEd, bmark_flp)) {
		ScrollForSell(cursorOld);
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateInputMark(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnInputMark() 
{
#ifdef SHAR
	CCursor cursorOld = m_pViewEd->m_cursor;
	if (m_pViewEd->GetHint()->m_bookMarks.JumpUnnormalMark(m_pViewEd, bmark_input)) {
		ScrollForSell(cursorOld);
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateMark(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnMark() 
{
	((CMainFrame*)AfxGetMainWnd())->
		SetPaneText( m_pViewEd->GetHint()->m_bookMarks.SetMark(m_pViewEd) ?
		"現在行ﾏｰｸされました":"現在行ﾏｰｸ解除されました");
}

void CTheView::OnUpdateMarkJumpPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv() && !m_pViewEd->GetHint()->m_bookMarks.IsEmpty_normal());
}

void CTheView::OnMarkJumpPrev() 
{
	CCursor cursorOld = m_pViewEd->m_cursor;
	if (m_pViewEd->GetHint()->m_bookMarks.JumpPrev(m_pViewEd)) {
		ScrollForSell(cursorOld);
	}
}

void CTheView::OnUpdateMarkJumpNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv() && !m_pViewEd->GetHint()->m_bookMarks.IsEmpty_normal());
}

void CTheView::OnMarkJumpNext() 
{
	CCursor cursorOld = m_pViewEd->m_cursor;
	if (m_pViewEd->GetHint()->m_bookMarks.JumpNext(m_pViewEd)) {
		ScrollForSell(cursorOld);
	}
}

void CTheView::OnUpdateMarkRemoveall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv() && !m_pViewEd->GetHint()->m_bookMarks.IsEmpty_normal());
}

void CTheView::OnMarkRemoveall() 
{
	m_pViewEd->GetHint()->m_bookMarks.RemoveAll_normal(); //flip以外全部
}

void CTheView::OnUpdateFindBracket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}
#ifdef SHAR
typedef struct {
	bool	bToEnd;
	bool	bCLang;
	int		iLevel;
	string	strFind;
	string	strReverse;
}find_blacket_data;

static int FindBlacketProc(const string &str, void *pv, OBJOFF &_off_find)
{
	// ( ( )
	//}
	//
	find_blacket_data *pdata = (find_blacket_data*)pv;

	//C言語コメント位置を設定
	OBJOFF off_comment = NPOS;
	if (pdata->bCLang) {
		off_comment = findSz(str, stcCLangComment);
		if (off_comment!=string::npos) {
			if (off_comment == findSz(str, stchttp) + abs(stchttp-stcCLangComment))
				off_comment = NPOS;
		}
	}
	//ブラケット位置バッファの作成
	vector<OBJOFF> vec_find, vec_reverse;
	OBJOFF off_find=0, off_reverse=0;
    for (; off_find!=string::npos || off_reverse!=string::npos;) {
		if (off_find!=string::npos) {
			off_find = findStr(str, pdata->strFind, off_find);
			if (off_find!=string::npos) {
				if (off_find<off_comment) {
					vec_find.push_back(off_find);
					off_find++;
				}else{
					off_find=string::npos;
				}
			}
		}
		if (off_reverse!=string::npos) {
			off_reverse = findStr(str, pdata->strReverse, off_reverse);
			if (off_reverse!=string::npos) {
				if (off_reverse<off_comment) {
					vec_reverse.push_back(off_reverse);
					off_reverse++;
				}else{
					off_reverse=string::npos;
				}
			}
		}
	}
	if (!pdata->bToEnd) {
		reverse(vec_find.begin(), vec_find.end());
		reverse(vec_reverse.begin(), vec_reverse.end());
	}

	if (vec_find.size()>0 || vec_reverse.size()>0) {
		off_find=0;
		off_reverse=0;
		for (int ifind = 0,	ireverse = 0; off_find!=NPOS || off_reverse!=NPOS;) {
			if (ifind<vec_find.size()) {
				off_find = vec_find[ifind];
			}else
				off_find = NPOS;
			if (ireverse<vec_reverse.size()) {
				off_reverse = vec_reverse[ireverse];
			}else
				off_reverse = NPOS;
			if (off_find!=NPOS && (pdata->bToEnd ? off_find<off_reverse:
				(off_find>off_reverse || off_reverse==NPOS))) {
				pdata->iLevel--;
				if (pdata->iLevel==0) {
					_off_find = off_find;
					return pdata->strFind.size();	//find size retrun!
				}
				ifind++;
			}else if (off_reverse!=NPOS && (pdata->bToEnd ? off_reverse<off_find:
				(off_reverse>off_find || off_find==NPOS))) {
				pdata->iLevel++;
				ireverse++;
			}
		}
	}
	return 0;
}
#endif//SHAR

void CTheView::OnFindBracket() 
{
#ifdef SHAR
	bool	bFindRight;
	find_blacket_data data;
	CUR cur = m_pViewEd->m_cursor.GetBlinkCursor();
	string strLeft, strRight;
	CCursor cursorOld = m_pViewEd->m_cursor;

	//AfxGetApp()->DoWaitCursor(1); 

	m_pViewEd->GetHint()->GetCurLeftRightChStrLine(cur, strLeft, strRight);

	//まずｶｰｿﾙ右文字から
	string *pstr;
	int off_bracket = string::npos;
	int i;
	for (i=0, pstr=&strRight; i<2 && data.strFind.size()==0; i++, pstr=&strLeft) {
		if (pstr->size()==1) {
			if ((off_bracket=GetIndexSbcs_bracket_left((BYTE*)pstr->data()))!=string::npos) {
				data.strFind = GetStrSbcs_bracket_right(off_bracket);
				bFindRight = true;
			}else
			if ((off_bracket=GetIndexSbcs_bracket_right((BYTE*)pstr->data()))!=string::npos) {
				data.strFind = GetStrSbcs_bracket_left(off_bracket);
				bFindRight = false;
			}else
				continue;
		}else
		if (pstr->size()==2) {
			if ((off_bracket=GetIndexDbcs_bracket_left((BYTE*)pstr->data()))!=string::npos) {
				data.strFind = GetStrDbcs_bracket_right(off_bracket);
				bFindRight = true;
			}else
			if ((off_bracket=GetIndexDbcs_bracket_right((BYTE*)pstr->data()))!=string::npos) {
				data.strFind = GetStrDbcs_bracket_left(off_bracket);
				bFindRight = false;
			}else
				continue;
		}else
			continue;
		data.strReverse = *pstr;
		break;
	}

	if (data.strFind.size()>0) {
		assert(i==0 || i==1);
		CCursor cursor;
		cursor.SetExpand();
		CUR cur = m_pViewEd->m_cursor.GetBlinkCursor(), curOrg;
		if (bFindRight) {	//文末方向へ')'右ブラケットを検索
			if (i==0) { //^(xxxxx) ｶｰｿﾙを一つ後ろへ
				m_pViewEd->GetHint()->NextCur(cur, false);
			}
			curOrg.D.y = NPOS;
			curOrg.D.x = NPOS;
		}else{				//文頭方向へ'('左ブラケットを検索
			if (i==1) { //(xxxxx)^ ｶｰｿﾙを一つ前へ
				m_pViewEd->GetHint()->PrevCur(cur, false);
			}
			curOrg.D.y = 0;
			curOrg.D.x = 0;
		}
		m_pViewEd->GetHint()->SetCur(curOrg);
		cursor.SetOrgCursor(curOrg);
		cursor.SetBlinkCursor(cur);
		data.bToEnd = bFindRight;
		data.bCLang = true;
		data.iLevel = 1;
		if (bFindRight ? m_pViewEd->GetHint()->FindData_ToEnd(cursor, FindBlacketProc, &data, false):
			m_pViewEd->GetHint()->FindData_ToTop(cursor, FindBlacketProc, &data, false)) {
			//見つけたのだ！
			assert(cursor.IsValid());
			m_pViewEd->m_cursor = cursor;
			//ShowBlinkCursor(true);
			ScrollForSell(cursorOld);
		}
	}
	//AfxGetApp()->DoWaitCursor(-1); 
#else
	StopFreeMsg();
#endif
}



void CTheView::OnUpdateConfigEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_pMainObjLay->IsFormatAll());
}


void CTheView::OnConfigEdit() 
{
#ifdef SHAR
	CConfigEdit dlg;
	dlg.UpdateEnv(false, &theViewEnvTbl);
	if (dlg.DoModal()==IDOK) {
		if (dlg.UpdateEnv(true, &theViewEnvTbl)) {
			((CMainFrame *)AfxGetMainWnd())->UpdateMenuConf(); //メニューツールバーの設定
		}
	}
#else
	StopFreeMsg();
#endif
}

bool CTheView::UpdateSelEnv(int envindex) 
{
#ifdef SHAR
	bool bVert_Old = m_pViewEd->GetEnv()->m_bVert;
	if (envindex==-1) { //ID_SELENV_NORMAL) {
		m_pViewEd->m_pObjLayTbl->Attach_Env(theViewEnvTbl.GetViewEnv_byEnvIndex(-1/*NORMAL*/));
	}else {//if (wParam>=ID_SELENV_START && wParam<=ID_SELENV_START30) {
		m_pViewEd->m_pObjLayTbl->Attach_Env(theViewEnvTbl.GetViewEnv_byEnvIndex(envindex));
	}
	UpdateViewEnv(bVert_Old, true);
	return true;
#else
	StopFreeMsg();
	return true;
#endif
}

void CTheView::OnUpdateSelenv(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck((theViewEnvTbl.GetEnvIndex(m_pViewEd->GetEnv())+ID_SELENV_START1)
		== pCmdUI->m_nID);
}

void CTheView::OnSelenvNormal() 
{
	UpdateSelEnv(-1);
}
void CTheView::OnSelenvStart1() 
{
	UpdateSelEnv(0);
}
void CTheView::OnSelenvStart2() 
{
	UpdateSelEnv(1);
}
void CTheView::OnSelenvStart3() 
{
	UpdateSelEnv(2);
}
void CTheView::OnSelenvStart4() 
{
	UpdateSelEnv(3);
}
void CTheView::OnSelenvStart5() 
{
	UpdateSelEnv(4);
}
void CTheView::OnSelenvStart6() 
{
	UpdateSelEnv(5);
}
void CTheView::OnSelenvStart7() 
{
	UpdateSelEnv(6);
}
void CTheView::OnSelenvStart8() 
{
	UpdateSelEnv(7);
}
void CTheView::OnSelenvStart9() 
{
	UpdateSelEnv(8);
}

void CTheView::OnTanbunAdd() 
{
#ifdef SHAR
	CTanbunAddDlg dlg;
	string strKey, strData = m_pViewEd->GetCursorWord(true); //multiline ok!
	int fmt;
	dlg.UpdateEnv(false, &theViewEnvTbl.m_tanbuns,
		CTanbunAddDlg::mode_new, strKey, strData, fmt);
	if (dlg.DoModal()==IDOK) {
		dlg.UpdateEnv(true, &theViewEnvTbl.m_tanbuns,
				0, strKey, strData, fmt);	//multimap 実際処理はここ!
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateTanbunAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void CTheView::OnTanbunEdit() 
{
#ifdef SHAR
	CTanbunEdit dlg;
	//string strKey, strData;
	//int fmt;
	dlg.UpdateEnv(false, &theViewEnvTbl.m_tanbuns);
	if (dlg.DoModal()==IDOK) {
		//dlg.UpdateEnv(true, &theViewEnvTbl.m_tanbuns,
		//		0, strKey, strData, fmt);
		string str = dlg.GetInputData();
		CAction *paction = new CAction;
		paction->push_string(act_past, str.data(), str.size()); //strlen(psz)は、Win95では改行があるとダメ
		OnString(paction, m_pViewEd->m_cursor);
	}
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateTanbunEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void CTheView::OnEdtlTime() 
{
#ifdef SHAR
	bool bPrev = GetKeyState(VK_SHIFT)<0;
	string strData = svbCommandFirst ? theViewEnvTbl.m_datetimes.GetThisTimeString_First():
							theViewEnvTbl.m_datetimes.GetThisTimeString_Next(!bPrev);
	CCursor cursorOld = m_pViewEd->m_cursor;
	CAction *paction = new CAction;
	paction->push_string(act_past, strData.data(), strData.size()); //strlen(psz)は、Win95では改行があるとダメ
	OnString(paction, m_pViewEd->m_cursor);
	CUR cur = cursorOld.Befor();
	m_pViewEd->GetHint()->SetCur(cur);
	m_pViewEd->m_cursor.SetExpand(true);
	m_pViewEd->m_cursor.SetOrgCursor(cur);
	svbCommandFirst = false;
	Invalidate();
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlTime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void CTheView::OnEdtlDate() 
{
#ifdef SHAR
	bool bPrev = GetKeyState(VK_SHIFT)<0;
	string strData = svbCommandFirst ? theViewEnvTbl.m_datetimes.GetTodayString_First():
							theViewEnvTbl.m_datetimes.GetTodayString_Next(!bPrev);

	CCursor cursorOld = m_pViewEd->m_cursor;
	CAction *paction = new CAction;
	paction->push_string(act_past, strData.data(), strData.size()); //strlen(psz)は、Win95では改行があるとダメ
	OnString(paction, m_pViewEd->m_cursor);
	CUR cur = cursorOld.Befor();
	m_pViewEd->GetHint()->SetCur(cur);
	m_pViewEd->m_cursor.SetExpand(true);
	m_pViewEd->m_cursor.SetOrgCursor(cur);
	svbCommandFirst = false;
	Invalidate();
#else
	StopFreeMsg();
#endif
}

void CTheView::OnUpdateEdtlDate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void CTheView::OnFileSave()
{
	((CTheDoc*)GetDocument())->FileSave();
}
void CTheView::OnFileClose()
{
	((CMainFrame*)AfxGetMainWnd())->OnFileClose();
}
void CTheView::OnWindowCloseNoThis()
{
	((CMainFrame*)AfxGetMainWnd())->OnWindowCloseNoThis();
}


/*
void CTheView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
((CTheDoc*)GetDocument())->UpdateFileSave(pCmdUI);
}
void CTheView::OnUpdateFileClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CTheView::OnUpdateWindowCloseNoThis(CCmdUI* pCmdUI) 
{
	((CMainFrame*)AfxGetMainWnd())->OnUpdateWindowCloseNoThis(pCmdUI);
}

*/

void CTheView::OnUpdateTagjump(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewEd->IsValidEnv());
}

void CTheView::OnTagjump() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
}



BOOL CTheView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

    if (m_pViewEd->IsVert()){
        DoScroll(zDelta<0 ? SB_LINELEFT:SB_LINERIGHT, 0, SB_HORZ);
    }else{
        DoScroll(zDelta<0 ? SB_LINEDOWN:SB_LINEUP, 0, SB_VERT);
    }
    return CView::OnMouseWheel(nFlags, zDelta, pt);
}
