///////////////////////////////////////////////
//ViewEd.cpp

#include "StdAfx.h"
#include "version.h"
#include "the.h"
#include "ViewEd.h"
#include "ViewEnv.h"
#include "TextCash.h"
//#include "Colorlist.h"
#include "chara.h"
#include "MainFrm.h"
#include "keyword.h"
#include "TheView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCaret svCaret;
//extern CCaret svCaret;

///////////////////////////////////////////////
//CCaret class
CCaret::CCaret()
{
	Initialize();
}
CCaret::~CCaret()
{
	if (IsValid() && m_bShow)
		ResetCaret(null, false);
}
void CCaret::Initialize(CWnd *pWnd, const CSize *psize)
{
	if (pWnd)
		m_pWnd = pWnd;
	else
		m_pWnd = null;
	m_bShow = false;
	m_pt = CPoint(INT_MIN,INT_MIN);
	if (psize)
		m_size = *psize;
	else
		m_size = CSize(0,0);
}
void CCaret::MyDestroyCaret()
{
	if (IsValid()) {
		HideCaret();
		DestroyCaret();
		Initialize();
	}
}
void CCaret::ShowCaret()
{
	assert(IsValid());
#if Cursor_bug_20140510
	if (!m_bShow) {
#endif
		m_pWnd->ShowCaret();
		//takaso 2014.1/20 for win8
		//SetCaretBlinkTime(1000);
#if Cursor_bug_20140510
	}
#endif
	m_bShow = true;
}
void CCaret::HideCaret()
{
	if (IsValid()) {
		if (m_bShow && IsWindow(m_pWnd->m_hWnd))//IsWindow終了時のウィンドウチェック
			m_pWnd->HideCaret();
		m_bShow = false;
	}
}
void CCaret::ResetCaret(CWnd *pWnd, const CRect *prc, COLORREF colorref)
{
	//if pWnd==NULL then destroy caret,
	//else destroy and create caret.
	MyDestroyCaret();
	if (pWnd) {
		CSize size(prc->Width(), prc->Height());
		if (colorref == RGB(0,0,0)) {
			CreateCaret( pWnd->m_hWnd, NULL, size.cx, size.cy);
		}else{
			//HPALETTE hpal;
			//hPal = CreatePalette((LOGPALETTE *)&LogPal);
			//hDC = GetDC(hwnd); 
			//hOldPal = SelectPalette(hDC, hPal, FALSE);
			//RealizePalette(hDC);  
			//hbmp = CreateDIBitmap(hDC, (LPBITMAPINFOHEADER)&DIBHdr, CBM_CREATEDIB, &dib, (LPBITMAPINFO)&DIBHdr, DIB_PAL_COLORS); 
			//CreateCaret(hwnd, hbmp, 1, 12);
			//ReleaseDC(hwnd, hDC);
			assert(false);
			CreateCaret(pWnd->m_hWnd, NULL, size.cx, size.cy);
		}
		//takaso 2014.1/20 for win8
		SetCaretBlinkTime(1000);

		Initialize(pWnd, &size);
		m_pWnd->SetCaretPos(CPoint(prc->left, prc->top));
		ShowCaret();
	}
}
void CCaret::MoveCaret(const CPoint pt)
{
	if (IsValid()) {
		if (m_pt != pt || !m_bShow) {
			HideCaret();
			m_pWnd->SetCaretPos(CPoint(pt.x, pt.y));
			m_pt = pt;
			ShowCaret();
		}
	}
}

#ifdef SPEC_PRINT
///////////////////////////////////////////////
//CCharsPrintAttr class
CCharsPrintAttr::CCharsPrintAttr()
{
	pOld = null;
//	CSize		sizeFont;
//	int			cyLine;
//	int			cyBeforLine;
//	int			cxhalf_font;
//	int			cyhalf_font;
//	int			width_linenum;
	SetPrinterDefault(false);
}
void CCharsPrintAttr::SetPrinter(CViewEd *pViewEd, CDC *pDCPrint)
{
	ASSERT(pOld==null);
	pOld = new CCharsPrintAttr;
	pDCPrint->SaveDC();
	Update(true, pViewEd);
	pOld->Update(true, pViewEd);

	pViewEd->ChoosePrintFont(pDCPrint);
	pViewEd->ResetCharSizes(pDCPrint, *this);
	SetPrinterDefault(theViewEnvTbl.m_printenv.m_bColor);
	Update(false, pViewEd);
	pViewEd->ChangeLineNum();
	Update(true, pViewEd);
}
#define SumColor(c)		(GetRValue(c) + GetGValue(c) + GetBValue(c))
#define DarkColor(c)	(RGB(GetRValue(c)/2, GetGValue(c)/2, GetBValue(c)/2))

void CCharsPrintAttr::SetPrinterDefault(bool bColor)
{
	modeDraw = DRAW_PRN;
	bViewTab = false;
	bViewCRLF = false;
	bViewEOF = false;
	bVert = false;
	bLineNum = (theViewEnvTbl.m_printenv.m_iLineNum!=0);

	colorTextBk = RGB(255,255,255);
	colorText_Select = RGB(255,255,255);
	colorTextBk_Select = RGB(0,0,0);
	colorEOF = RGB(0,0,0);
	if (!bColor) {
		colorText = RGB(0,0,0);
		colorLineNum = RGB(0,0,0);
		colorText_Mail = RGB(0,0,0);
		colorText_Url = RGB(0,0,0);
	}else{
		if (SumColor(colorText)>384)
			colorText = RGB(0,0,0);
		if (SumColor(colorLineNum)>512)
			colorLineNum = DarkColor(colorLineNum);
		if (SumColor(colorText_Mail)>512)
			colorText_Mail = DarkColor(colorText_Mail);
		if (SumColor(colorText_Url)>512)
			colorText_Url = DarkColor(colorText_Url);
	}
}
void CCharsPrintAttr::SetDisplay(CViewEd *pViewEd, CDC *pDCPrint)
{
	ASSERT(pOld);
	pOld->Update(false, pViewEd);
	delete  pOld;
	pOld=null;
	pDCPrint->RestoreDC(-1);
}
void CCharsPrintAttr::Update(bool bGet, CViewEd *pViewEd)
{
	UpdateSizeOnly(bGet, pViewEd);
	if (bGet) {
		modeDraw = pViewEd->m_modeDraw;
		width_linenum = pViewEd->m_width_linenum;
		bViewTab = pViewEd->m_bViewTab;
		bViewCRLF = pViewEd->m_bViewCRLF;
		bViewEOF = pViewEd->m_bViewEOF;
		bVert = pViewEd->GetEnv()->m_bVert;
		bLineNum = pViewEd->GetEnv()->BShowLineNum();
		colorText = pViewEd->m_colorText;
		colorLineNum = pViewEd->m_colorLineNum;
		colorEOF = pViewEd->m_colorEOF;
		colorTextBk = pViewEd->m_colorTextBk;
		colorText_Select = pViewEd->m_colorText_Select;
		colorTextBk_Select = pViewEd->m_colorTextBk_Select;
		colorText_Mail = pViewEd->m_colorText_Mail;
		colorText_Url = pViewEd->m_colorText_Url;
	}else{
		pViewEd->m_modeDraw = modeDraw;
		pViewEd->m_width_linenum = width_linenum;
		pViewEd->m_bViewTab = bViewTab;
		pViewEd->m_bViewCRLF = bViewCRLF;
		pViewEd->m_bViewEOF = bViewEOF;
		pViewEd->GetEnv()->m_bVert = bVert;
		pViewEd->GetEnv()->BShowLineNum() = bLineNum;
		pViewEd->m_colorText = colorText;
		pViewEd->m_colorLineNum = colorLineNum;
		pViewEd->m_colorEOF = colorEOF;
		pViewEd->m_colorTextBk = colorTextBk;
		pViewEd->m_colorText_Select = colorText_Select;
		pViewEd->m_colorTextBk_Select = colorTextBk_Select;
		pViewEd->m_colorText_Mail = colorText_Mail;
		pViewEd->m_colorText_Url = colorText_Url;
	}
}
#endif
////////////////////////////////////////////////////////
//CCharsFontAttr
CCharsFontAttr::CCharsFontAttr()
{
}
void CCharsFontAttr::UpdateSizeOnly(bool bGet, CViewEd *pViewEd)
{
	if (bGet) {
		sizeFont = pViewEd->m_sizeFont;
		cyLine = pViewEd->m_cyLine;
		cyBeforLine = pViewEd->m_cyBeforLine;
		cyhalf_font = pViewEd->m_cyhalf_font;
		cxhalf_font = pViewEd->m_cxhalf_font;
	}else{
		pViewEd->m_sizeFont = sizeFont;
		pViewEd->m_cyLine = cyLine;
		pViewEd->m_cyBeforLine = cyBeforLine;
		pViewEd->m_cyhalf_font = cyhalf_font;
		pViewEd->m_cxhalf_font = cxhalf_font;
	}
}

///////////////////////////////////////////////
//CViewEd class
CViewEd::CViewEd()
{
	m_pWnd = null;
	m_bAct = true;
	m_posScrollH = 0;
	m_posScrollV = 0;

	m_pObjLayTbl = null;
	m_cydsp_page = 0;
	m_sizeFont.cx = 0;
	m_sizeFont.cy = 0;
	m_cyLine = 0;
	m_cyBeforLine = 0;
	m_cxhalf_font = 0;
	m_cyhalf_font = 0;
	m_width_linenum = 0;

	m_colorText = RGB(0,0,0);
	m_colorLineNum = RGB(255,255,0);
	m_colorEOF = RGB(255,255,0);
	m_colorTextBk = GetSysColor(COLOR_WINDOW);
	m_colorText_Select = RGB(255,255,255);//::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_colorTextBk_Select = RGB(255,255,0);
	m_colorText_Mail = RGB(0,0,0);
	m_colorText_Url = RGB(0,0,0);
	m_penTab.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_penCRLF.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_penLineNumLine.CreatePen(PS_SOLID, 1,RGB(0,255,0));
	m_brushTextBk.CreateSolidBrush(m_colorTextBk);

	m_bIbeam = false;
	m_leftClip_save = INT_MAX;

	m_bViewTab = false;
	m_bViewCRLF = false;
	m_bViewEOF = false;
	m_bViewMail = false;
	m_bViewURL = false;

#ifdef SPEC_PRINT
	m_modeDraw = DRAW_DSP;
#endif
}

CViewEd::~CViewEd()
{
	m_penTab.DeleteObject();
	m_penCRLF.DeleteObject();
	m_penLineNumLine.DeleteObject();
	m_brushTextBk.DeleteObject();
	if (m_font.GetSafeHandle())
		m_font.DeleteObject();
}
void CViewEd::PushPopScrolls(CWnd *pWnd, CRect rcClient, bool bPop)
{
	if (m_pObjLayTbl && GetEnv()) {
		if (m_bAct) { //以前アクトの場合
			m_posScrollH = -1;//VGetScrollPos(SB_HORZ);
			m_posScrollV = -1;//VGetScrollPos(SB_VERT);
		}
		if (bPop) {
			SetScrolls(pWnd, rcClient, false, m_posScrollH, m_posScrollV);
		}
	}
}
CObjLayTbl *CViewEd::Detach(bool bCloseDoc)
{
	if (!bCloseDoc && m_pObjLayTbl && GetEnv()) {
		m_posScrollH = VGetScrollPos_NoSwitch(SB_HORZ);
		m_posScrollV = VGetScrollPos_NoSwitch(SB_VERT);
		//VSetScrollPos(SB_HORZ, 0);
		//VSetScrollPos(SB_VERT, 0);
	}

	CObjLayTbl *pObjLayTbl = m_pObjLayTbl;
//2007 復活->2008取り消し
//	m_pObjLayTbl = null;
//	m_cursor.Initialize();
//	pObjLayTbl->GetActions().Initialize();

    m_pWnd = null;
	return pObjLayTbl;
}

//ObjLayTblポインタの格納、
CObjLayTbl *CViewEd::Attach(CObjLayTbl *pObjLayTbl, CWnd *pWnd)
{
	CObjLayTbl* pOld = m_pObjLayTbl;
	if (pOld)
		pOld->AttachCount(-1);
	assert(pObjLayTbl);
	m_pObjLayTbl = pObjLayTbl;
	pObjLayTbl->AttachCount(1);

//	if (m_sizeFont.cy>0) {
//		CUR_SIZE cursize = GetCurSize();
//		if (cursize.cy>0)
//			MoveCoursor(VK_F1); //initial set cursel!
//	}
	m_pWnd = pWnd;

	GetEnv()->SetFocusEnv();
	CRect rcClient;
	m_pWnd->GetClientRect(&rcClient);
	ChangeFonts(m_pWnd, rcClient);
	ChangeLineNum();
	MoveCoursor(VK_F1); //initial set cursel!
	//if (IsVert())
	//	((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_RIGHT);
	//else
		((CMainFrame*)AfxGetMainWnd())->DockToolBar(AFX_IDW_DOCKBAR_TOP);
	//ChangeColors(m_pWnd, m_pWnd->GetActiveWindow()==m_pWnd);//new!
	ChangeColors(m_pWnd, m_pWnd->GetFocus()==null ||
		!m_pWnd->GetFocus()->IsKindOf(RUNTIME_CLASS(CTheView)) ||
		m_pWnd->GetFocus()==m_pWnd);//new!

	SetScrolls(m_pWnd, rcClient, false, m_posScrollH, m_posScrollV);
	return pOld;
}
void CViewEd::ChangeColors(CWnd *pWnd,  bool bAct, bool bSetClassOnly)
{
	m_bAct = bAct;
//#define INV256(p)		(255-p)
//#define COLOR_INVERT(c)	RGB(INV256(GetGValue(c)), INV256(GetBValue(c)), INV256(GetRValue(c)))
//#define COLOR_INVERT(c)	RGB(INV256(GetRValue(c)), INV256(GetGValue(c)), INV256(GetBValue(c)))
#define BKC2TC(c)		((GetRValue(c)+GetGValue(c)+GetBValue(c))<=384 ? RGB(255,255,255):RGB(0,0,0))
	if (!IsValidEnv())
		return;
	if (!bSetClassOnly) {
		m_colorTextBk = GetEnv()->m_envColors[bAct ? idc_textBk:idc_textBk_UnAct].color;
		m_colorText = bAct ? GetEnv()->m_envColors[idc_text].color:BKC2TC(m_colorTextBk);
		m_colorTextBk_Select = m_colorText==RGB(255,255,255) ? RGB(192,192,192):m_colorText;
		m_colorText_Select = m_colorTextBk;
		m_colorLineNum = GetEnv()->m_envColors[idc_lineNum].color;
		if (m_brushTextBk.GetSafeHandle())
			m_brushTextBk.DeleteObject();
		m_brushTextBk.CreateSolidBrush(m_colorTextBk);
		m_brushTextBk.UnrealizeObject();
		if (m_penTab.GetSafeHandle())
			m_penTab.DeleteObject();
		if (m_penCRLF.GetSafeHandle())
			m_penCRLF.DeleteObject();
		m_penTab.CreatePen(PS_SOLID, 1, GetEnv()->m_envColors[idc_tab].color);
		m_penCRLF.CreatePen(PS_SOLID, 1, GetEnv()->m_envColors[idc_CRLF].color);
		if (m_penLineNumLine.GetSafeHandle())
			m_penLineNumLine.DeleteObject();
		m_penLineNumLine.CreatePen(PS_SOLID, 1, GetEnv()->m_envColors[idc_lineNum_Line].color);
		m_colorEOF = GetEnv()->m_envColors[idc_EOF].color;
		m_colorText_Mail = GetEnv()->m_envColors[idc_Mail].color;
		m_colorText_Url = GetEnv()->m_envColors[idc_Url].color;

		ResetDrawFlgs();
	}
	m_pkeywords_word = GetEnv()->m_pkeywords_word;
	m_pkeywords_unword = GetEnv()->m_pkeywords_unword;
	//SetClassLong(pWnd->GetSafeHwnd(), GCL_HBRBACKGROUND, (long)m_brushTextBk.GetSafeHandle());
}
//フォントの設定、ObjLayTblのイニシャライズ
void CViewEd::ChangeFonts(CWnd *pWnd, CRect rcClient, int iZoom)
{
	VCopyWnd(pWnd);
	
	assert(IsValidEnv());
	SetFont(pWnd, iZoom);
}

bool CViewEd::ChangeLineNum()
{
	int width_save = m_width_linenum;
	if (GetEnv()->BShowLineNum()) {
		if (GetCurSize().cy==0)
			m_width_linenum = 2*m_sizeFont.cx + m_cxhalf_font;
		else {
			string str;
			str = ultostring((unsigned long)GetLineMaxNum(GetEnv()->m_bRowNum),10);
			m_width_linenum = (str.length()+1) * m_sizeFont.cx + m_cxhalf_font;
		}
	}else{
		m_width_linenum = m_cxhalf_font;
	}
	if (width_save!=m_width_linenum)
		m_pWnd->Invalidate();
	return (width_save!=m_width_linenum);
}

OBJSIZE CViewEd::GetLineMaxNum(bool brow)
{
	OBJSIZE row;
	if (brow) {
		row = GetCurSize().cy;
	}else{
		for (row=GetCurSize().cy-1; row>=0 && (*GetHint())[row].line==0; row--);
		row = (*GetHint())[row].line;
	}
	return row;
}

void CViewEd::SetFont(CWnd *pWnd, int iZoom)
{
	if (m_font.GetSafeHandle())
		m_font.DeleteObject();

	LOGFONT lf;
	if (IsVert()) {
		lf = GetEnv()->m_lfVert;
		lf.lfFaceName[0] = '@';
		strncpy(lf.lfFaceName+1, GetEnv()->m_lfVert.lfFaceName, LF_FACESIZE-1 );
		//lf.lfEscapement = 2700;
		//lf.lfOrientation = 2700;
		//true type font only!
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
	}else
		lf = GetEnv()->m_lf;

	if (iZoom!=0) {
		//lf.lfWidth=0; //for scaling font zoom!->move to CViewEnv
		if (iZoom>0) //zoom in
			lf.lfHeight += max(lf.lfHeight/10,1);
		else {
			if (lf.lfHeight>5)
				lf.lfHeight -= max(lf.lfHeight/10,1);
		}
		//if (IsVert())
		//	GetEnv()->m_lfVert.lfHeight = lf.lfHeight;
		//else
		//	GetEnv()->m_lf.lfHeight = lf.lfHeight;
		//縦横両方に適用する
		GetEnv()->m_lfVert.lfHeight = lf.lfHeight;
		GetEnv()->m_lf.lfHeight = lf.lfHeight;
	}

	lf.lfHeight = - abs(lf.lfHeight);
	if (m_font.CreateFontIndirect(&lf)) {
		//pWnd->SetFont(&m_font);
		CFont *pfontOld;
		CDC *pDC = pWnd->GetDC();
		//if (IsVert())
		//	pDC->SetMapperFlags(0);
		pfontOld = (CFont *)pDC->SelectObject(m_font);
		
		//size get
		CCharsFontAttr chardata;
		ResetCharSizes(pDC, chardata);
		chardata.UpdateSizeOnly(false, this);

		CString str;
		m_font.GetObject(sizeof(lf), &lf);
		int pt10 = ::MulDiv(abs(lf.lfHeight), 720, pDC->GetDeviceCaps(LOGPIXELSY));
		str.Format(" %d.%d pt", pt10/10, pt10%10>=5 ? 5:0);
		((CMainFrame*)AfxGetMainWnd())->SetPaneText(str.GetBuffer(0), 2);
		pWnd->ReleaseDC(pDC);
	}
	SetClassLong(pWnd->GetSafeHwnd(), GCL_HCURSOR,
		(long)(IsVert() ? AfxGetApp()->LoadCursor(IDC_CUR_VERT):
		AfxGetApp()->LoadStandardCursor(IDC_IBEAM))); 
}


void CViewEd::ResetCharSizes(CDC *pDC, CCharsFontAttr &chardata)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	chardata.sizeFont = pDC->GetTextExtent("W", 1);
	CSize sizeFont_DBCS = pDC->GetTextExtent("あ", 2);
	chardata.sizeFont.cx = min(chardata.sizeFont.cx, sizeFont_DBCS.cx/2);
	//chardata.sizeFont.cy++;	//for ime underline.
	switch (tm.tmPitchAndFamily & 0x0f) {
	case TMPF_FIXED_PITCH:
		break;
	case TMPF_VECTOR:
	case TMPF_TRUETYPE:
		chardata.sizeFont.cx = (tm.tmAveCharWidth + tm.tmMaxCharWidth)/2;
		break;
	case TMPF_DEVICE:
		chardata.sizeFont.cx = (tm.tmAveCharWidth + tm.tmMaxCharWidth)/2;
		break;
	}
	chardata.cyLine = (int)((unsigned int)chardata.sizeFont.cy * (100+GetEnv()->m_ratioBetweenLine)/100);
	chardata.cyLine += (chardata.cyLine - chardata.sizeFont.cy)%2; //行間を前と後ろ同じドット数にする
	chardata.cyBeforLine = (chardata.cyLine - chardata.sizeFont.cy)/2;
	chardata.cyhalf_font = chardata.sizeFont.cy/2;
	chardata.cxhalf_font = min(chardata.cyhalf_font, chardata.sizeFont.cx/3);
}

bool CViewEd::SetScrolls(CWnd *pWnd, CRect rcClient, bool bVSwitch, int posH, int posV)
{
	
	if (m_sizeFont.cx==0 || m_sizeFont.cy==0)
		return false;

	m_rcClient = rcClient;

	CUR_SIZE cursize = GetCurSize();

	SCROLLINFO sb_info_v;
	SCROLLINFO sb_info_h;
	VGetScrollInfo(SB_VERT, &sb_info_v);
	VGetScrollInfo(SB_HORZ, &sb_info_h);
	if (posV>=0) {
		sb_info_v.nPos = IsHorz() ? posV:posH;
	}
	if (posH>=0) {
		sb_info_h.nPos = IsHorz() ? posH:posV;
	}

	if (bVSwitch) {
		GetEnv()->m_bVert = !GetEnv()->m_bVert;
		ChangeFonts(pWnd, rcClient);
		ResetCaret(pWnd);
	}
	//ページアップダウンの行数セット
	rcClient = VSwitchRect_View(rcClient);
	//

	//vert scroll-bar
	sb_info_v.nMin = 0;
	CWnd *pWndSbv=VGetScrollBarCtrl(SB_VERT);
	sb_info_v.nMax = max(cursize.cy,
		max(rcClient.Height() / m_cyLine, 1));
		//	(pWndSbv && IsWindow(pWndSbv->GetSafeHwnd()) && pWndSbv->IsWindowEnabled()) ?
		//	sb_info_v.nPos + 1:0);
	sb_info_v.nPage = min(max(rcClient.Height() / m_cyLine, 1), sb_info_v.nMax);
	sb_info_v.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	VSetScrollInfo(SB_VERT, &sb_info_v);

	m_cydsp_page = sb_info_v.nPage;

	
	//horz scroll-bar
	sb_info_h.nMin = 0;
	CWnd *pWndSbh=VGetScrollBarCtrl(SB_HORZ); 
	sb_info_h.nMax = max(cursize.cx,
		max(rcClient.Width() / m_sizeFont.cx, 1));
	//		(pWndSbh && IsWindow(pWndSbh->GetSafeHwnd()) && pWndSbh->IsWindowEnabled()) ?
	//		sb_info_h.nPos + 1:0);
	sb_info_h.nPage = min(max((rcClient.Width()) / m_sizeFont.cx, 1), sb_info_h.nMax);
	sb_info_h.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	VSetScrollInfo(SB_HORZ, &sb_info_h);

	//change linenum check!
	return ChangeLineNum();
}
void CViewEd::ReBuildHint()
{
	CLineHint *pHintNew = new CLineHint;
	GetHint()->ReBuildDsp2DspSells(GetEnv(), *pHintNew);
	pHintNew->m_bookMarks = GetHint()->m_bookMarks; //ﾌﾞｯｸﾏｰｸｺﾋﾟｰ
	delete GetHint();
	m_pObjLayTbl->m_phint = pHintNew;

	m_pObjLayTbl->RecalcCurSize();

	CUR cur = m_cursor.GetBlinkCursor();
	GetHint()->SetCur_4Lgc(cur);
	m_cursor.SetCursor(&cur);
//	m_cursor.SetExpand(false);
//	m_cursor.SetBlinkCursor(cur);
}

void CViewEd::SetExpandLineCursor(CCursor *pCursor)
{
	bool bFirst;
	if (bFirst = (pCursor==null)) {
		pCursor = &m_cursor;
	}
	CUR curBef = pCursor->Befor(),
		curEnd = pCursor->End();
	bool bExpand = pCursor->IsExpand();
	bool bBlinkBef = curBef == pCursor->GetBlinkCursor();
	OBJOFF row;
	curBef.D.y = GetHint()->GetThis_LineTop(curBef.D.y);
	curBef.D.x = 0;
	GetHint()->SetCur(curBef, false);
	if (bFirst && bExpand && curEnd.D.x == 0 && curEnd.D.y>0 &&
		GetHint()->GetThis_LineTop(curEnd.D.y)==curEnd.D.y) {
		curEnd.D.y--;
	}
	row = GetHint()->GetNext_LineTop(curEnd.D.y);
	curEnd.D.y = row-1;
	curEnd.D.x = NPOS;
	GetHint()->SetCur(curEnd, false, bFirst ? SETSEL_SET:SETSEL_NEXTCHAR);

	m_cursor.SetCursor(bBlinkBef ? &curBef:&curEnd, !bBlinkBef ? &curBef:&curEnd);
//	m_cursor.SetExpand();
//	m_cursor.SetBlinkCursor(bBlinkBef ? curBef:curEnd);
//	m_cursor.SetOrgCursor(!bBlinkBef ? curBef:curEnd);
	m_cursor.ResetExpand_IfCuremp();
}
string CViewEd::GetCursorWord_2Expand(bool bTalCurOk)
{
	CLineHint *pHint = GetHint();
	CCursor cursor = m_cursor;
	string str;
	if (!cursor.IsExpand()) {
		GetWordCursor(cursor, false, bTalCurOk);
		if (!cursor.IsExpand())
			return str;
		m_cursor = cursor; //実際にカーソルを拡張する
	}else if (pHint->IsMultiLinesExpand(cursor))
		return str;
	return pHint->GetPartStringOfLine(cursor);
}
string CViewEd::GetCursorWord(bool bMultiLinesOk)
{
	CLineHint *pHint = GetHint();
	CCursor cursor = m_cursor;
	string str;
	if (theIme.IsInComposition())
		return str;
	if (!cursor.IsExpand()) {
		GetWordCursor(cursor, false);
		if (!cursor.IsExpand())
			return str;
	} else if (pHint->IsMultiLinesExpand(cursor)) {
		if (bMultiLinesOk)
			return GetCursorMultiLineWord();
		return str;
	}	
	return pHint->GetPartStringOfLine(cursor);
}

static int SStDP_CopyProc_block(const BYTE *sz, OBJSIZE len, void *pv)
{
	return FAppendCpySz2GData(((SSDP_DATA*)pv)->hData, ((SSDP_DATA*)pv)->lenData, sz, len) ? 0:-1;
}

string CViewEd::GetCursorMultiLineWord()
{
	string str;
	SSDP_DATA data;
	memset(&data, 0, sizeof(data));
	CAction action(act_copy);
	HGLOBAL hTextData = (HGLOBAL)Action(&action, 
		(Normal_Proc)SStDP_CopyProc_block, &data);
	const char *psz = null;
	OBJOFF len = 0;
	if (psz = (char*)GlobalLock (hTextData)) {
		//len = GlobalSize(hTextData);
		len = data.lenData;	//BUGFIX:Win95では、GlobalSize(hTextData);は当てにならない
		str.append(psz, len);
	}
	return str;
}

void CViewEd::GetWordCursor(CCursor &cursor, bool bEnableSpace, bool bTalCurOk)
{
	CLineHint *pHint = GetHint();
	assert(!cursor.IsExpand());

	CUR cur = m_cursor.GetBlinkCursor();
	m_pObjLayTbl->GetActions().ResetMargeFlg();
	//------>カーソル選択範囲の拡張
	//step1:文末方向への拡張
	CUR cur_first = cur;
	int type = 0;
	//if (type&WHT && !bEnableSpace)
	//	return;
	if (!pHint->FindDiffChType(cur, type, bEnableSpace, bTalCurOk)) {	//stop!
		m_cursor.SetExpand(false);
		return;
	}
//	cursor.SetExpand();
//	cursor.SetBlinkCursor(cur);
	//step1:文頭方向への拡張
	pHint->FindDiffChType(cur_first, type, bEnableSpace, bTalCurOk, false);
//	cursor.SetOrgCursor(cur_first);
	cursor.SetCursor(&cur, &cur_first);
	m_cursor.ResetExpand_IfCuremp();
}

bool CViewEd::OnMouseDblClk(int yPos, int xPos, CPoint point, UINT nFlags)
{
	CLineHint *pHint = GetHint();
	OBJOFF row, col;
	CPoint ptDot;
	CUR cur;
/*
MK_CONTROL Ctrl キーが押されている場合にセットします。
MK_LBUTTON マウスの左ボタンが押されている場合にセットします。
MK_MBUTTON マウスの中央ボタンが押されている場合にセットします。
MK_RBUTTON マウスの右ボタンが押されている場合にセットします。
MK_SHIFT Shift */

	point = VSwitchPoint_View(point);
	if (-yPos>point.y/m_sizeFont.cy)
		row = 0;
	else {
		row = yPos + point.y/m_cyLine;
		if (point.y<0 && row>0)
			row--;
	}
	row = row>pHint->size_mac() ? pHint->size_mac():row;

	if ((point.x+xPos * m_sizeFont.cx) > GetViewOrg().x) {
		col = (point.x+xPos * m_sizeFont.cx - GetViewOrg().x)/m_sizeFont.cx;
	}else{
		return false;//文字列範囲外
	}

	m_cursor.MoveSell(row, col, (nFlags & MK_CONTROL)>0, false);//false=shiftを無視する
	cur = m_cursor.GetBlinkCursor();
	cur.D.y = row;
	cur.D.x = col;
	pHint->SetCur(cur, GetEnv()->m_bFreeCursor, SETSEL_SET, true);

	GetWordCursor(m_cursor, true);
	return true;
}

void CViewEd::SetMCursor(const CCursor &cursor)
{
	assert(GetHint());
	m_cursor = cursor;
	CUR cur =  m_cursor.GetBlinkCursor();
	GetHint()->SetCur(cur);
	m_cursor.SetBlinkCursor(cur);
	if (m_cursor.IsExpand()){
		cur =  m_cursor.GetOrgCursor();
		GetHint()->SetCur(cur);
		m_cursor.SetOrgCursor(cur);
	}
	m_cursor.ResetExpand_IfCuremp();
}

bool CViewEd::JumpLine(OBJOFF lineJump, JUMPTYPE JumpType, OBJOFF col)
{
	bool bfind = false;
	AfxGetApp()->DoWaitCursor(1); 
	SELL sell;
	CUR cur;
	switch(JumpType) {
	case JUMP_LINE:
		if ((cur.D.y=GetHint()->FindLine(lineJump, sell))!=NPOS) {
			cur.L = sell;
			cur.D.x = col;
			bfind = true;
		}
		break;
	case JUMP_ROW:
		cur.D.y = SIZEMAC(lineJump);
		cur.D.x = col;
		GetHint()->SetCur(cur);
		bfind = true;
		break;
	case JUMP_BYTE:
		if ((cur.D.y=GetHint()->FindByte(lineJump, sell))!=NPOS) {
			cur.L = sell;
			cur.D.x = col;
			bfind = true;
		}
		break;
	}

	if (bfind) {
//		m_cursor.SetExpand(false);
//		m_cursor.SetBlinkCursor(cur);
		//takaso2010 FlpMark set!
		CBookMark mark(this, bmark_flp /*takaso2010 bugfix true-> */); //現在位置を記憶
		if (mark.m_row != cur.D.y || mark.m_col != cur.D.x)
			GetHint()->m_bookMarks.SetMark(this, null, bmark_flp); //Janpする前に新しい位置を登録
		m_cursor.SetCursor(&cur);
	}
	AfxGetApp()->DoWaitCursor(-1); 
	return bfind;
}

bool CViewEd::OnMouseMove(int yPos, int xPos, CPoint point, UINT nFlags, bool bDown, bool bUp)
{
	CLineHint *pHint = GetHint();
	OBJOFF row, col;
	CPoint ptDot;
	bool bShift;
	CUR cur;
/*
MK_CONTROL Ctrl キーが押されている場合にセットします。
MK_LBUTTON マウスの左ボタンが押されている場合にセットします。
MK_MBUTTON マウスの中央ボタンが押されている場合にセットします。
MK_RBUTTON マウスの右ボタンが押されている場合にセットします。
MK_SHIFT Shift */

	point = VSwitchPoint_View(point);
	if (-yPos>point.y/m_cyLine)
		row = 0;
	else {
		row = yPos + point.y/m_cyLine;
		if (point.y<0 && row>0)
			row--;
	}
	row = row>pHint->size_mac() ? pHint->size_mac():row;

	if ((point.x+xPos * m_sizeFont.cx) > GetViewOrg().x) {
		col = (point.x+xPos * m_sizeFont.cx - GetViewOrg().x)/m_sizeFont.cx;
	//	HCURSOR hcurOld = GetCursor();
	//	HCURSOR hcur = AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
	//	if (hcurOld != hcur)
	//		SetCursor(hcur);
		m_bIbeam = true;
	}else{
		col = NPOS;
		HCURSOR hcur = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		if (GetCursor() != hcur)
			SetCursor(hcur);
		m_bIbeam = false;
	}


	if (!bDown && !bUp) {
		//マウス移動
		if (::GetCapture()!=m_pWnd->m_hWnd)
			return false;
		bShift = (nFlags & MK_LBUTTON)>0;
		if (!bShift)
			return false;
	}else{
		bShift = (nFlags & MK_SHIFT)>0;
		if (col==NPOS) {//行選択
			if (!bShift) {
				//選択された行頭のカーソル位置を選択開始位置として設定
				cur.D.y = row;
				cur.D.x = 0;
				pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
				m_cursor.Initialize();//選択状態であっても一度解除する
				m_cursor.SetBlinkCursor(cur);
				bShift = true;
			}
		}
	}

	//文頭方向へ複数行頭選択する場合は、文頭にブリンクカーソルがある
	//超むずかしかった！
	if (col==NPOS && bShift) {
		cur = m_cursor.GetOrgCursor();
		CUR curBef = m_cursor.Befor();
		if (row+1==cur.D.y && cur.D.x==0 && row>=curBef.D.y) {
			cur.D.y=row;
			cur.D.x=0;
			m_cursor.SetOrgCursor(cur);
		}else if (row<cur.D.y) {
			m_cursor.SetOrgCursor(m_cursor.End());
			col=0;
		}
	}

	m_cursor.MoveSell(row, col, (nFlags & MK_CONTROL)>0, bShift);
	cur = m_cursor.GetBlinkCursor();
	if (col==NPOS) {
		row ++;
		col = 0;
	}
	cur.D.y = row;
	cur.D.x = col;
	pHint->SetCur(cur, GetEnv()->m_bFreeCursor, SETSEL_SET, true);
	m_cursor.SetBlinkCursor(cur);
	m_cursor.ResetExpand_IfCuremp();
	m_pObjLayTbl->GetActions().ResetMargeFlg();
	return true;
}
CPoint CViewEd::Sell2Point_Win(OBJOFF x, OBJOFF y, bool bLast)
{
	return Sell2Point_Win(x, y, VGetScrollPos(SB_HORZ), VGetScrollPos(SB_VERT), bLast);

}
CPoint CViewEd::Sell2Point_Win(OBJOFF x, OBJOFF y, int xPos, int yPos, bool bLast)
{
	CPoint pt(0,0);
	if (x==NPOS)
		x = max(m_rcClient.Width(), m_rcClient.Height());
	else{
		x += GetViewOrg().x/m_sizeFont.cx;
		if (x<xPos) {
			x = 0;
		}else {
			x = (x - xPos) * m_sizeFont.cx;
			if (bLast) {
				x += (m_sizeFont.cx);
			}
		}
	}
	if (y==NPOS)
		y = max(m_rcClient.Width(), m_rcClient.Height());
	else{
		y += GetViewOrg().y/m_cyLine;
		if (y<yPos) {
			y = 0;
		}else {
			y = (y - yPos) * m_cyLine;
			if (bLast) {
				y += (m_cyLine);
			}
		}
	}
	
	pt = VSwitchPoint_Win(CPoint(x,y));

	if (pt.x< 0)
		pt.x = 0;
	else if (pt.x > m_rcClient.Width())
		pt.x = m_rcClient.Width();
	
	if (pt.y< 0)
		pt.y = 0;
	else if (pt.y > m_rcClient.Height())
		pt.y = m_rcClient.Height();

	return pt;
}
CRect CViewEd::Cursor2Rect_OneLine(const SELL &sell_first, const SELL &sell_last, const int xPos, const int yPos)
{
	CRect rc;
	CPoint pt_first, pt_last;
	pt_first = Sell2Point_Win(sell_first.x, sell_first.y, xPos, yPos);
	pt_last  = Sell2Point_Win(sell_last.x,  sell_last.y,  xPos, yPos, true);
	rc.top = pt_first.y;
	rc.bottom = pt_last.y;
	if (IsHorz()) {
		rc.left = pt_first.x;
		if (sell_first.x==0) //行番号領域も
			rc.left = 0;
		rc.right = pt_last.x;
	}else{
		rc.right = pt_first.x;
		if (sell_first.x==0) //行番号領域も
			rc.top = 0;
		rc.left = pt_last.x;
	}
	return rc;
}

void CViewEd::InvalidateCursor(CWnd *pWnd, CCursor cursor, int xPos, int yPos)
{
	CRect rc;
	assert(cursor.IsExpand());
	SELL sell_first=cursor.Befor().D;
	SELL sell_last =cursor.End().D;
	OBJOFF row = m_cursor.GetBlinkCursor().D.y;

	//TRACE("--------------->>---------------\n");
	//TRACE("(%d,%d)---(%d,%d)\n",sell_first.y, sell_first.x, sell_last.y, sell_last.x);
	CPoint pt_first, pt_last;
	if (sell_first.y == sell_last.y) {//1行のみ更新
		rc = Cursor2Rect_OneLine(sell_first, sell_last, xPos, yPos);
		InvalidateRectLine(pWnd, &rc);
		//TRACE("(%d,%d,%d,%d)\n",rc.left, rc.right, rc.top, rc.bottom);
	}else{
		SELL sell_f, sell_l;
		sell_l = SELL(sell_first.y, NPOS);
		rc = Cursor2Rect_OneLine(sell_first, sell_l, xPos, yPos);
		InvalidateRectLine(pWnd, &rc);
		//pWnd->InvalidateRect(&rc);
		//TRACE("(%d,%d,%d,%d)\n",rc.left, rc.right, rc.top, rc.bottom);
		if (sell_last.y - sell_first.y>1) {//中間の複数行
			sell_f = SELL(sell_first.y+1, 0);
			sell_l = SELL(sell_last.y-1, NPOS);
			rc = Cursor2Rect_OneLine(sell_f, sell_l, xPos, yPos);
			InvalidateRectLine(pWnd, &rc);
			//pWnd->InvalidateRect(&rc);
			//TRACE("(%d,%d,%d,%d)\n",rc.left, rc.right, rc.top, rc.bottom);
		}
		if (sell_last.x > 0) {
			sell_f = SELL(sell_last.y, 0);
			sell_l = SELL(sell_last.y, NPOS);
			rc = Cursor2Rect_OneLine(sell_f, sell_last, xPos, yPos);
			InvalidateRectLine(pWnd, &rc);
			//pWnd->InvalidateRect(&rc);
			//TRACE("(%d,%d,%d,%d)\n",rc.left, rc.right, rc.top, rc.bottom);
		}
	}
	//TRACE("---------------<<---------------\n");
	return;
}

void CViewEd::InvalidateRectLine(CWnd *pWnd, CRect *prc)
{
	assert(pWnd);
	if (prc==null) {
		pWnd->InvalidateRect(null);
		m_leftClip_save = INT_MAX;
	}else{
		//prc->leftをセーブしておく
		CRect rc = *prc;
		if (IsHorz()) {
			if (m_leftClip_save > rc.left)
				m_leftClip_save = rc.left;
			rc.left = rc.left>GetViewOrg().x ? GetViewOrg().x:0;
			//rc.right = m_rcClient.right;
		}else{
			if (m_leftClip_save > rc.top)
				m_leftClip_save = rc.top;
			rc.top = rc.top>GetViewOrg().x ? GetViewOrg().x:0;
			//rc.bottom = m_rcClient.bottom;
		}
		pWnd->InvalidateRect(&rc);
		m_rcClip_save = rc;
	}
}

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}

void CViewEd::ChoosePrintFont(CDC *pdcPrint)
{
	CWaitCursor wait;
	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));

	CFont font;
	LOGFONT lf;
	if (IsVert()) {
		lf = GetEnv()->m_lfVert;
		lf.lfFaceName[0] = '@';
		strncpy(lf.lfFaceName+1, GetEnv()->m_lfVert.lfFaceName, LF_FACESIZE-1 );
		//lf.lfEscapement = 2700;
		//lf.lfOrientation = 2700;
		//true type font only!
		//lf.lfEscapement = 900;
		lf.lfOrientation = 900;
	}else
		lf = GetEnv()->m_lf;

	// need to map initial logfont to screen metrics.
	::ScaleLogFont(&lf, dcScreen, *pdcPrint);

	//SetPrinterFont(NULL);
	//m_fontPrint.DeleteObject();
	if (font.CreateFontIndirect(&lf)) {
		(CFont *)SelectObject(pdcPrint->m_hDC, font);
		(CFont *)SelectObject(pdcPrint->m_hAttribDC, font);
		// notify container that content has changed
		//GetDocument()->UpdateAllItems(NULL);
	}
	::ReleaseDC(NULL, dcScreen.Detach());
}

#ifdef SPEC_PRINT
bool CViewEd::OnDraw(CDC *pDC, int xPos, int yPos, CRect rcClip, bool bEraseBack, DRAW_MODE modeDraw)
#else
bool CViewEd::OnDraw(CDC *pDC, int xPos, int yPos, CRect rcClip, bool bEraseBack)
#endif //SPEC_PRINT
{
	OBJOFF row, rowMac;
	CPoint ptDot;

	assert(m_sizeFont.cx>0 && m_sizeFont.cy>0);

#ifdef SPEC_PRINT
	if (modeDraw == DRAW_DSP) {
#endif
		if (!m_bAct) {
			xPos = m_posScrollH;
			yPos = m_posScrollV;
		}
#ifdef SPEC_PRINT
	}
#endif

	VCopyDC(pDC);

	if (bEraseBack) {
		CRect rc = rcClip;
		if (m_leftClip_save<INT_MAX && rc==m_rcClip_save) {
			if (IsHorz()) {
				rc.left = m_leftClip_save;
				rc.right = m_rcClient.right;
			}else{
				rc.top = m_leftClip_save;
				rc.bottom = m_rcClient.bottom;
			}
			m_leftClip_save = INT_MAX;
		}
		pDC->FillRect(&rc, &m_brushTextBk);
	}
	#ifdef _DEBUG
	//	TRACE("rcClip(%d,%d,%d,%d)-->\n",rcClip.left, rcClip.top,rcClip.right,rcClip.bottom);
	#endif
	rcClip = VSwitchRect_View(rcClip);

#ifdef SPEC_PRINT
	row = yPos + ((modeDraw == DRAW_DSP) ? rcClip.top/m_cyLine:0);
#else
	row = yPos + rcClip.top/m_cyLine;
#endif

#ifdef SPEC_PRINT
	if (modeDraw == DRAW_DSP)
		rowMac = row + DIVMOD(rcClip.bottom - rcClip.top, m_cyLine);//yPos + DIVMOD(rcClip.bottom - rcClip.top, m_cyLine);
	else
		rowMac = yPos + max((rcClip.bottom - rcClip.top)/m_cyLine -1, 0);
#else
		rowMac = row + DIVMOD(rcClip.bottom - rcClip.top, m_cyLine);//yPos + DIVMOD(rcClip.bottom - rcClip.top, m_cyLine);
#endif
	ptDot.x = - xPos * m_sizeFont.cx;
	ptDot.y = (row - yPos) * m_cyLine;

	CPoint ptView;
#ifdef SPEC_PRINT
	if (modeDraw == DRAW_DSP && IsVert())	{
#else
	if (IsVert())	{
#endif
		//CPoint pt;
		//CSize size;
		//pt = pDC->GetWindowOrg(); 
		//size = pDC->GetWindowExt();
		pDC->SetMapMode(MM_ANISOTROPIC); //
		//CRect rcClient = rcClip;
		
		ptView = GetViewOrg();
		pDC->SetWindowOrg(0,0); //ptView.x
		pDC->SetWindowExt(1, 1);
		pDC->SetViewportOrg(m_rcClient.Width()+ptView.y, ptView.x); //ptView.x
		pDC->SetViewportExt(-1, 1);
	}else
	{
		ptView = pDC->GetViewportOrg();
#ifdef SPEC_PRINT
		if (modeDraw != DRAW_DSP)
			ptView.Offset(rcClip.left, rcClip.top);
#endif
		ptView.Offset(GetViewOrg());
		pDC->SetViewportOrg(ptView);
	}

#ifdef SPEC_PRINT
	if (modeDraw == DRAW_DSP)
		pDC->SelectObject(m_font);
#else
	pDC->SelectObject(m_font);
#endif

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colorText);

#ifdef _DEBUG
//	pDC->SelectObject(&m_penRed);
//	pDC->Rectangle(&rcClip);
//	TRACE("rcClip(%d,%d,%d,%d)\n",rcClip.left, rcClip.top,rcClip.right,rcClip.bottom);
#endif


	for (; row<=rowMac; row++, ptDot.y+=m_cyLine) {
		//CRect rcDraw(rcClip.left, ptDot.y, rcClip.right, ptDot.y+m_cyLine);
		//VFillRect(&rcDraw, &m_brushTextBk);
		if (!OnDraw(row, ptDot.y, ptDot.x, rcClip.left, rcClip.right))
			break;
	}

	if (row==0) {
		if(GetEnv()->BShowLineNum()) {
			DrawLineNum(row+1, ptDot.y, ptDot.x);
		}
		return false;
	}

	return true;
}


void CViewEd::DrawLineNum(OBJOFF num, int ydot, int xdot)
{
	assert(GetEnv()->BShowLineNum());
	int xdotLeft = xdot - GetViewOrg().x;
	int keta;

	bool bLineNumLine = GetEnv()->m_envColors[idc_lineNum_Line].bEnable;
	
	if (bLineNumLine) {
		CPen *ppenOld = null;
#ifdef SPEC_PRINT
		if (m_modeDraw == DRAW_DSP || theViewEnvTbl.m_printenv.m_bColor)
#endif
			ppenOld = m_pDC->SelectObject(&m_penLineNumLine);
		VMoveTo(xdot-(m_cxhalf_font*3/2), ydot);
		VLineTo(xdot-(m_cxhalf_font*3/2), ydot+m_cyLine);
		if (ppenOld)
			m_pDC->SelectObject(ppenOld);
	}
	
	if (num<=0)
		return;

	int ydot_text = ydot + m_cyBeforLine;
	string str = ultostring((unsigned long)num,10);
	COLORREF color = m_pDC->GetTextColor();
	m_pDC->SetTextColor(m_colorLineNum);
	if (!bLineNumLine)
		VTextOut(xdot-m_sizeFont.cx, ydot_text, (char *)":", 1);
	for (xdot-=(2*m_sizeFont.cx), keta=str.length()-1; xdot>=xdotLeft && keta>=0; xdot-=m_sizeFont.cx, keta--) {
		//sz[keta+1] = '\0';
		VTextOut(xdot, ydot_text, (char *)&str[keta], 1);
	}
	m_pDC->SetTextColor(color);
}
void CViewEd::ResetDrawFlgs() 
{
	if (theViewEnvTbl.m_bEditMarkView) {
		m_bViewTab = GetEnv()->m_envColors[idc_tab].bEnable;
		m_bViewCRLF = GetEnv()->m_envColors[idc_CRLF].bEnable;
		m_bViewEOF = GetEnv()->m_envColors[idc_EOF].bEnable;
	}else{
		m_bViewTab = false;
		m_bViewCRLF = false;
		m_bViewEOF = false;
	}
	m_bViewMail = GetEnv()->m_envColors[idc_Mail].bEnable;
	m_bViewURL = GetEnv()->m_envColors[idc_Url].bEnable;
}
bool CViewEd::OnDraw( OBJOFF row, int ydot, int xdot, int left, int right)
{
	assert(IsValidEnv());
	if (row>=GetHint()->size())
		return false;

	//TRACE("row=%d xdot=%d\n",row, xdot);

	SELL sellBefor, sellEnd;
	sellBefor = m_cursor.Befor().D;
	sellEnd = m_cursor.End().D;
	BYTE *pch, *pch_cash;
	OBJSIZE keta, len, len_off;
	int xdot_first = xdot;
	SELL selLgc = (*GetHint())[row];
	len_off = selLgc.ptbl->ReadLineCash(pch_cash, selLgc.off, selLgc.pbytes);
	len_off = selLgc.len;
	pch = pch_cash;
	IME_STRING_ATTR isa = isa_no;
	int xdot_isa=xdot;
	int ydot_text = ydot + m_cyBeforLine;
	if(GetEnv()->BShowLineNum()) {
		if (GetEnv()->m_bRowNum)
			DrawLineNum(row+1, ydot, xdot);
		else
			DrawLineNum(selLgc.line, ydot, xdot);
	}
#ifdef _DBCS
	bool bCRLF = false;
	BYTE ch, ch2, sz4[4];
	sz4[2]=0;
	OBJOFF off;
	bool bExpand_draw = false;
	int xdot_start_expand = -1;
	int leftNormal=0;

	//for keyword/mail/html temp auto vars.
	int chtype = WHT;	//initiale set
	bool bCheckKeyword = true;
	bool bDbcsSpace = false;
	BYTE *pchT_first, *pchT_last;
	int lenT, xdot_new;
	CKeywordConf *pconf;


	for(off=selLgc.off, ch=*pch, keta=0;
		off<selLgc.off + selLgc.len && xdot<right;
		off++, xdot += len * m_sizeFont.cx, keta+=len) {
		sz4[0]=ch;

		if ((xdot>=leftNormal || leftNormal==0) && isa==isa_no) {
			m_pDC->SetTextColor((bExpand_draw ?
									 m_colorText_Select:m_colorText));
			//TRACE("leftNormal=%d, xdot=%d isa=%d\n", leftNormal, xdot, isa);
			leftNormal = 0;//INT_MAX;
		}
#ifdef SPEC_PRINT
		if (m_modeDraw == DRAW_DSP && IsExpandCursorArea(row, keta, sellBefor, sellEnd) != bExpand_draw) {
#else
		if (IsExpandCursorArea(row, keta, sellBefor, sellEnd) != bExpand_draw) {
#endif
			bExpand_draw = !bExpand_draw;
			if (!theIme.IsInComposition()) {
				if (bExpand_draw) {
					xdot_start_expand = xdot;
					m_pDC->SaveDC();
					m_pDC->SetBkMode(OPAQUE);
					m_pDC->SetTextColor(m_colorText_Select) ; //::GetSysColor(COLOR_HIGHLIGHTTEXT)
					m_pDC->SetBkColor(m_colorTextBk_Select) ;
				}else{
					//must be befor RestoreDC()
					if (m_cyBeforLine>0) {
						CRect rect(xdot_start_expand, ydot, xdot, ydot + m_cyBeforLine);
						VFillSolidRect(rect, m_colorTextBk_Select);
						rect.OffsetRect(0, m_sizeFont.cy + m_cyBeforLine);
						VFillSolidRect(rect, m_colorTextBk_Select);
						xdot_start_expand = -1;
					}
					m_pDC->RestoreDC(-1);
				}
			}
		}
		if (bExpand_draw && theIme.IsInComposition()) {
			IME_STRING_ATTR isa_new = theIme.GetImeStringAttr(GetHint()->GetByteOffsetInCursor(m_cursor.Befor(), row, off));
			if (isa!=isa_new) {
				DrawISA(xdot, ydot, xdot_isa, isa, isa_new);
				isa = isa_new;
				xdot_isa = xdot;
			}
		}else if (isa!=isa_no) {
			DrawISA(xdot, ydot, xdot_isa, isa);
			isa = isa_no;
			xdot_isa = xdot;
		}

	#ifdef _DBCS
		ch2 = *(++pch);

		if (IsSJIS_1stByte(ch) && IsSJIS_2ndByte(ch2)) {
			if (leftNormal == 0 && !bExpand_draw) {
				bCheckKeyword = m_pkeywords_word && (
					IsDbcWordKeyword_first(chtype) || 
					(bDbcsSpace && !IsDbcSpace(ch, ch2)) );

				//keyword step1:ワード単位のｷｰﾜｰﾄﾞチェック
				if (bCheckKeyword) {
					CKeywords::iterator iter = m_pkeywords_word->FindWordKeyword(pch-1);
					if (iter!=m_pkeywords_word->end()) {
						pconf = (*iter).second->m_pconf;
						pchT_first = pch-1;
						//lenT = (pconf->m_keycpos == keycpos_rowend ? (pch_cash + len_off - pchT_first):(*iter).first.size());
						lenT = (pconf->m_keycpos == keycpos_rowend ? INT_MAX:(*iter).first.size());
						DrawTextBlock(xdot, ydot_text, pchT_first, lenT,
							0, pconf->m_color, leftNormal, pconf->m_bUnderline);
						goto KEYWORD_CHK_END_DBCS;
					}
				}
				//keyword step1:非ワード単位のｷｰﾜｰﾄﾞチェック
				if (m_pkeywords_unword) {
					CKeywords::iterator iter = m_pkeywords_unword->FindUnwordKeyword(pch-1);
					if (iter!=m_pkeywords_unword->end()) {
						pconf = (*iter).second->m_pconf;
						pchT_first = pch-1;
						//lenT = (pconf->m_keycpos == keycpos_rowend ? (pch_cash + len_off - pchT_first):(*iter).first.size());
						lenT = (pconf->m_keycpos == keycpos_rowend ? INT_MAX:(*iter).first.size());
						DrawTextBlock(xdot, ydot_text, pchT_first, lenT,
							0, pconf->m_color, leftNormal, pconf->m_bUnderline);
						goto KEYWORD_CHK_END_DBCS;
					}
				}
KEYWORD_CHK_END_DBCS:
				;
			}
			chtype=GetChType(ch);

			len = 2;
			sz4[1]=ch2;
			//if (IsVert()) {
			//	CRect rc(xdot, ydot, xdot+2*m_sizeFont.cx, ydot+m_sizeFont.cy);
			//	m_pDC->Rectangle(&rc);
				//m_pDC->ExtTextOut(xdot, ydot, 0 , &rc, (char*)sz4, 2, null); //ETO_OPAQUE
			//	m_pDC->TextOut(xdot, ydot, sz4);
			//}else
			bDbcsSpace = IsDbcSpace(ch, ch2);
			VTextOut(xdot, ydot_text, (char*)sz4, 2);
			off++;
			ch2 = *(++pch);
		}else
	#endif
		{
			if (IsCRLF(ch)) {
				DrawCRLF(xdot, ydot, m_bViewCRLF);
				//len = 1;
				break;
			}else if (ch==VK_TAB) {
				len=CALC_TABSTOPS(keta, GetEnv()->m_nTabStops);
				DrawTAB(xdot, ydot, len, m_bViewTab);
			}else if (ch==VK_EOF && row+1==GetCurSize().cy) {
				if (m_bViewEOF)
					DrawEOF(xdot, ydot);
				break;
			}else {
				//Signle byte char!
				//
				if (leftNormal == 0 && !bExpand_draw) {
					//keyword step1:ワード単位のｷｰﾜｰﾄﾞチェック
					bCheckKeyword = m_pkeywords_word && IsSbcWordKeyword_first(chtype, GetChType(ch));
					if (bCheckKeyword) {
						CKeywords::iterator iter = m_pkeywords_word->FindWordKeyword(pch-1);
						if (iter!=m_pkeywords_word->end()) {
							pconf = (*iter).second->m_pconf;
							pchT_first = pch-1;
							//lenT = (pconf->m_keycpos == keycpos_rowend ? (pch_cash + len_off - pchT_first):(*iter).first.size());
							lenT = (pconf->m_keycpos == keycpos_rowend ? INT_MAX:(*iter).first.size());
							DrawTextBlock(xdot, ydot_text, pchT_first, lenT,
								0, pconf->m_color, leftNormal, pconf->m_bUnderline);
							goto KEYWORD_CHK_END;
						}
					}
					//keyword step1:非ワード単位のｷｰﾜｰﾄﾞチェック
					if (m_pkeywords_unword) {
						CKeywords::iterator iter = m_pkeywords_unword->FindUnwordKeyword(pch-1);
						if (iter!=m_pkeywords_unword->end()) {
							pconf = (*iter).second->m_pconf;
							pchT_first = pch-1;
							//lenT = (pconf->m_keycpos == keycpos_rowend ? (pch_cash + len_off - pchT_first):(*iter).first.size());
							lenT = (pconf->m_keycpos == keycpos_rowend ? INT_MAX:(*iter).first.size());
							DrawTextBlock(xdot, ydot_text, pchT_first, lenT,
								0, pconf->m_color, leftNormal, pconf->m_bUnderline);
							goto KEYWORD_CHK_END;
						}
					}
					if (m_bViewMail && ch2 == '@' && GetFullType(ch) | (ALP|NUM)) {
						//a(<-ch) @(<-ch2,<-pch) b cd ｱｲｳ
						//d(<-ch)  (<-ch2,<-pch) ｱ ｲｳ
						if (IsMailAddressChar(*(pch+1))) {
							//書き直す
							pchT_first = pch_cash;
							pchT_last = pch_cash + len_off;
							GetMailAddressBlock(pchT_first, pch, pchT_last);
							//a<- pchT_first ' '<-pchT\last
							lenT = pchT_last-pchT_first; //5
							assert(lenT>0);
							xdot_new = xdot-((pch-pchT_first-1)*m_sizeFont.cx);
							DrawTextBlock(xdot_new,
								ydot_text, pchT_first, lenT, pch-pchT_first-1, m_colorText_Mail, leftNormal);
							goto KEYWORD_CHK_END;
						}
					} else if (m_bViewURL && (ch=='H' || ch=='h')) {
						//h(<-ch) t(<-ch2,<-pch) t p://abc ｱｲｳ
						//c(<-ch)  (<-ch2,<-pch) ｱ ｲｳ
						pchT_first = pch-1;
						pchT_last = pch_cash + len_off;
						if (strnicmp((char*)pchT_first, stchttp, strlen(stchttp))==0) {
							GetUrlAddressLast(pchT_first+strlen(stchttp), pchT_last);
							//h<- pchT_first ' '<-pchT\last
							lenT = pchT_last-pchT_first; //10
							assert(lenT>0);
							DrawTextBlock(xdot, ydot_text, pchT_first, lenT, 0, m_colorText_Url, leftNormal);
						}
					}
KEYWORD_CHK_END:
					;
				}
				VTextOut(xdot, ydot_text, (char*)sz4, 1);
				len = 1;
			}
			chtype=GetChType(ch);
		}
		ch = ch2;
		}
#else
	string str;
	str.append(pch, selLgc.len);
	m_pDC->TextOut(0, ydot_text, str.c_str());
#endif

	if (isa!=isa_no) {
		DrawISA(xdot, ydot, xdot_isa, isa);
	}
	//must be befor RestoreDC()
	if (xdot_start_expand>=0) {
		CRect rect(xdot_start_expand, ydot, xdot, ydot + m_cyBeforLine);
		VFillSolidRect(rect, m_colorTextBk_Select);
		rect.OffsetRect(0, m_sizeFont.cy + m_cyBeforLine);
		VFillSolidRect(rect, m_colorTextBk_Select);
	}

	if (m_pDC->GetTextColor()==m_colorText_Select)//(bExpand_draw || isa==isa_invert)
		m_pDC->RestoreDC(-1);
	m_pDC->SetTextColor(m_colorText);
	m_pDC->SetBkColor(m_colorTextBk);
	return true;
}
void CViewEd::DrawISA(int xdot, int ydot, int xdot_first, IME_STRING_ATTR isa, IME_STRING_ATTR isa_new)
{
	ydot += (m_cyBeforLine + m_sizeFont.cy-1);

	if (isa_new==isa_invert) {
		m_pDC->SaveDC();
		m_pDC->SetBkMode(OPAQUE);
		m_pDC->SetTextColor(m_colorText_Select) ; //::GetSysColor(COLOR_HIGHLIGHTTEXT)
		m_pDC->SetBkColor(m_colorTextBk_Select) ;
		theIme.SetCandidatePos(VSwitchPoint_Win(CPoint(xdot + m_width_linenum, ydot)));
	}else if (isa == isa_invert) {
		m_pDC->RestoreDC(-1);
		//m_pDC->SetBkMode(TRANSPARENT);
		//m_pDC->SetTextColor(m_colorText) ; //::GetSysColor(COLOR_HIGHLIGHTTEXT)
		//m_pDC->SetBkColor(m_colorTextBk) ;
	}

	if (isa == isa_dot || isa == isa_line) {
		CPen *ppen_old;
		CPen pen;
		pen.CreatePen(isa == isa_dot ? PS_DOT:PS_SOLID, 1, m_colorText);
		VMoveTo(xdot_first, ydot);
		ppen_old = (CPen*)m_pDC->SelectObject(&pen);
		VLineTo(xdot, ydot);
		m_pDC->SelectObject(ppen_old);
		pen.DeleteObject();
	}
}

void CViewEd::DrawCRLF(int xdot, int ydot, bool bEnableView)
{
	
	if (m_pDC->GetBkColor()==m_colorTextBk_Select) {
		CRect rect(xdot, ydot, xdot+GetSizeCell().cx/*+3*/, ydot + m_cyLine);
		VFillSolidRect(rect, m_colorTextBk_Select);
	}

	if (bEnableView) {
		ydot += m_cyBeforLine;
		CPen *ppenOld;
		ppenOld = m_pDC->SelectObject(&m_penCRLF);
		VMoveTo(xdot+=(m_cxhalf_font+3), ydot+=m_cyhalf_font/2);
		VLineTo(xdot, ydot+=m_cyhalf_font);
		VLineTo(xdot-m_cxhalf_font, ydot-m_cxhalf_font);
		VMoveTo(xdot, ydot);
		VLineTo(xdot+m_cxhalf_font, ydot-m_cxhalf_font);
		m_pDC->SelectObject(ppenOld);
	}
}
void CViewEd::DrawEOF(int xdot, int ydot)
{
	ydot += m_cyBeforLine;
	m_pDC->SetTextColor(m_colorEOF);
	VTextOut(xdot, ydot, szEOF, strlen(szEOF));
}

void CViewEd::DrawTextBlock(int xdot, int ydot, BYTE *sz, int len, int len_befor, COLORREF color, int& leftNormal, bool bUnderline)
{
	BYTE sz2[2];
	sz2[1]=0;
	
	int xdotT = xdot;
	int cx = GetSizeCell().cx;
#ifdef SPEC_PRINT
	if (m_modeDraw == DRAW_DSP)
		m_pDC->SetTextColor(color);
	else if (theViewEnvTbl.m_printenv.m_bColor)
		m_pDC->SetTextColor(SumColor(color)>512 ? DarkColor(color):color);
	else
		m_pDC->SetTextColor(RGB(0,0,0));
#else
	m_pDC->SetTextColor(color);
#endif
	for (int i=0; i<len_befor; i++, xdotT+=cx) {
		*sz2 = *sz++;
		VTextOut(xdotT, ydot, (char*)sz2, 1);
	}
	
	ydot += m_sizeFont.cy;
/*	if (leftNormal==INT_MAX)
		leftNormal = 0;
	CRect rc = CRect(leftNormal, ydot, xdot, ydot+1);
	VFillRect(&rc, &m_brushTextBk);
*/
	if (bUnderline) {
		CPen *ppenOld, pen;
		pen.CreatePen(PS_SOLID, 1, color);
		ppenOld = m_pDC->SelectObject(&pen);
		VMoveTo(xdot, ydot);
		xdot = len==INT_MAX ? INT_MAX:(xdot+(cx*len));
		VLineTo(xdot, ydot);
		m_pDC->SelectObject(ppenOld);
	}else{
		xdot = len==INT_MAX ? INT_MAX:(xdot+(cx*len));
	}
	leftNormal = xdot;

//	m_drawBlockSet.insert();
}

void CViewEd::DrawTAB(int xdot, int ydot, int lenTab, bool bEnableView)
{
	if (m_pDC->GetBkColor()==m_colorTextBk_Select) {
		CRect rect(xdot, ydot, xdot+m_sizeFont.cx*lenTab, ydot + m_cyLine);
		VFillSolidRect(rect, m_colorTextBk_Select);
	}
	
	if (bEnableView) {
		ydot += m_cyBeforLine;
		CPen *ppenOld;
		ppenOld = m_pDC->SelectObject(&m_penTab);
		xdot += (m_sizeFont.cx*(lenTab-1))/2;
		VMoveTo(xdot, ydot+=m_cyhalf_font);
		VLineTo(xdot+=GetSizeCell().cx, ydot);
		VLineTo(xdot-m_cxhalf_font, ydot-m_cxhalf_font);
		VMoveTo(xdot, ydot);
		VLineTo(xdot-m_cxhalf_font, ydot+m_cxhalf_font);
		m_pDC->SelectObject(ppenOld);
	}
}


void CViewEd::ResetCaret(CWnd *pWnd)
{
	//if pWnd==NULL then destroy caret,
	//else destroy and create caret.
    if (pWnd==null || m_pWnd==null)
        return;
    CRect rc = GetBlinkCursorRect_Win();
	svCaret.ResetCaret(pWnd, &rc);
}

void CViewEd::DrawBlinkCursor(CWnd *pWnd, bool bDraw)
{
	//VCopyDC(pDC);
	//VInvertRect(GetBlinkCursorRect_Win());
	if (bDraw) {
		CRect rc = GetBlinkCursorRect_Win();
		svCaret.MoveCaret(CPoint(rc.left, rc.top));

		CDC *pDC = pWnd->GetDC();
		if (pDC!=null) {
			VCopyDC(pDC);
//takaso 2012.07.24 Cursol Line Draw remove.
//			CRect rcLine;
//			rcLine = CRect(0, rc.bottom-1, 0x1000/*INT_MAX*/, rc.bottom);
//			VInvertRect(rcLine); 


//m_pDC->InvertRect(IsHorz() ? lpRect:(LPCRECT)rc);
		}
		//CDC *pDC = pWnd->GetDC();
		//pDC->FillSolidRect(&rcLine, RGB(255,255,0)); //takaso2010


		//takaso2010
/*		CRect rcLine;
		rcLine = IsHorz() ? CRect(0, rc.bottom, INT_MAX, rc.bottom):CRect(rc.left, 0, rc.left+1, INT_MAX);
		CDC *pDC = pWnd->GetDC();
		pDC->FillSolidRect(&rcLine, RGB(255,255,0)); //takaso2010
		
		if(GetEnv()->BShowLineNum()) {
			CPen *ppenOld;
			CDC *pDC = pWnd->GetDC();
			ppenOld = pDC->SelectObject(&m_penLineNumLine);
			//VMoveTo((m_cxhalf_font*3/2), rc.bottom);
			IsHorz() ? pDC->MoveTo((m_cxhalf_font*3/2), rc.bottom):pDC->MoveTo(rc.left,(m_cxhalf_font*3/2));
			//VLineTo(INT_MAX, rc.bottom);
			IsHorz() ? pDC->LineTo(INT_MAX, rc.bottom):pDC->LineTo(rc.left, INT_MAX);
			if (ppenOld)
				pDC->SelectObject(ppenOld);
		}
*/

//		CPen *ppenOld, pen;
//		pen.CreatePen(PS_SOLID, 1, color);
//		ppenOld = m_pDC->SelectObject(&pen);
//		VMoveTo(0, rc.bottom);
//		VLineTo(INT_MAX, rc.bottom);
//		m_pDC->SelectObject(ppenOld);

#ifndef BUG_20121028_MEMLEAK
		pWnd->ReleaseDC(pDC);
#endif

	}else
		svCaret.HideCaret();

/*	if (bDraw)
		pDC->FillSolidRect(&rc, RGB(255,255,0));
	else
		pDC->FillRect(&rc, &m_brushTextBk);
*/
}

CRect CViewEd::GetBlinkCursorRect_Win()
{
	CRect rcCursor = GetBlinkCursorRect();
	CPoint pt(0,0);
	pt.x = VGetScrollPos(SB_HORZ) * GetSizeCell().cx;
	pt.y = VGetScrollPos(SB_VERT) * GetSizeCell().cy;
	pt = VSwitchPoint(pt);
	rcCursor.OffsetRect(-pt.x, -pt.y);
	return rcCursor;
}
CRect CViewEd::GetBlinkCursorRect(bool bLine)
{
	CRect rcCursor;
	CUR cur;
	if (theIme.IsCompCursor()) {
		cur = theIme.GetBlinkCursor();
	}else{
		cur = GetBlinkCursor();
	}
	rcCursor.top = cur.D.y * GetSizeCell().cy + (bLine ? 0:m_cyBeforLine);
	rcCursor.left= cur.D.x * GetSizeCell().cx;
	rcCursor.bottom = rcCursor.top + (bLine ? m_cyLine:GetSizeFont().cy);
	rcCursor.right = rcCursor.left + min(max((GetSizeCell().cx*10/35), 1), 3);
	if (IsVert()) {
		rcCursor = VSwitchRect_Win(rcCursor);
		rcCursor.OffsetRect(VSwitchPoint(GetViewOrg()));
	}else
		rcCursor.OffsetRect(GetViewOrg());

	return rcCursor;
}

CSize CViewEd::GetExpandCursorLgcSize(OBJOFF row, bool &bHit)
{
	assert(m_cursor.IsExpand());
	CSize size(0,0);

	SELL sellBefor, sellEnd;
	sellBefor = m_cursor.Befor().D;
	sellEnd = m_cursor.End().D;
	if (bHit=(sellBefor.y <= row && row <= sellEnd.y)) {
		if (sellBefor.y == row)
			size.cx = sellBefor.x;
		if (sellEnd.y == row) {
			if (sellEnd.x==0)
				bHit = false;
			else
				size.cy = sellEnd.x;
		}
	}
	return size;
}
bool CViewEd::ActionUndoRedo(CActionBlock &actBlock_befor, CActionBlock &actBlock_after)
{
	CLineHint *pHint = GetHint();
	if (pHint->DoActionLgc(actBlock_befor, actBlock_after,
		actBlock_befor.m_cursor, actBlock_after.m_cursor,
		(CTextCashTbl *)m_pObjLayTbl->m_pEditObj)) {
		//CUR cur = actBlock_after.m_cursor.GetBlinkCursor();
		//pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		//actBlock_after.m_cursor.SetBlinkCursor(cur);
		//レイアウトサイズの再計算
		m_pObjLayTbl->RecalcCurSize();
		//カーソルの設定
		m_cursor = actBlock_after.m_cursor;
//		m_colTarget = m_cursor.GetBlinkCursor().D.x;

		return true;
	}
	return false;
}

int CViewEd::Action(CAction *paction, Normal_Proc pProc, void *pData)
{
	assert(IsValidEnv());
	CActionLgc *pactLgc;
	int rtn=0;

	switch(paction->m_act) {
	case act_undo:
		if (!m_pObjLayTbl->GetActions().IsEnableUndo())
			return -1;
		pactLgc = m_pObjLayTbl->GetActions().pop_undo()->m_pactLgc;
		ActionUndoRedo(pactLgc->m_actBlock[CActionLgc::act_block_after],
			pactLgc->m_actBlock[CActionLgc::act_block_befor]);
		break;
	case act_redo:
		if (!m_pObjLayTbl->GetActions().IsEnableRedo())
			return -1;
		pactLgc = m_pObjLayTbl->GetActions().pop_redo()->m_pactLgc;
		ActionUndoRedo(pactLgc->m_actBlock[CActionLgc::act_block_befor],
			pactLgc->m_actBlock[CActionLgc::act_block_after]);
		break;
	case act_delete_del:
	case act_delete_back:
	case act_input:
	case act_input_ime:
	case act_input_ime_result:
	case act_past:
		rtn = InputString(paction, paction->m_act);		
		break;
	case act_move:
		//if (GetEnv()->m_bFreeCursor || cursize.cy>0) {
		rtn = MoveCoursor(paction->m_vk.vk(), paction->m_vk.bControl(), paction->m_vk.bShift(), paction->m_vk.count());	
		m_pObjLayTbl->GetActions().ResetMargeFlg();
		break;
	case act_copy:
		rtn = (int)GetHint()->GetCursorData(m_cursor, pProc, (SSDP_DATA*)pData);
		break;
	default:
		assert(false);
		break;
	}

	return rtn;
}

UINT CViewEd::VSwitchKey(UINT nChar)
{
	if (IsVert()) {
		switch(nChar) {
		case VK_LEFT:
			nChar = VK_DOWN;
			break;
		case VK_RIGHT:
			nChar = VK_UP;
			break;
		case VK_UP:
			nChar = VK_LEFT;
			break;
		case VK_DOWN:
			nChar = VK_RIGHT;
			break;
		//case VK_HOME:
		//case VK_END:
		//case VK_PRIOR:
		//case VK_NEXT:
		default:
			break;
		}
	}
	return nChar;
}

int CViewEd::MoveCoursor(UINT nChar, const  bool bControl, const bool bShift, const int count)
{
	assert(IsValidEnv());
	CLineHint *pHint = GetHint();
	CCursor coursorOld = m_cursor;

	bool bExpand_Old = m_cursor.IsExpand();
	m_cursor.MoveSell(nChar, bControl, bShift);
	int iScroll = 0;
	bool bSetTarget = false;//bExpand_Old && !m_cursor.IsExpand();
	CUR cur = m_cursor.GetBlinkCursor();
	nChar = VSwitchKey(nChar);
	for (int i=0; i<count; i++) {
	switch(nChar) {
	case VK_HOME:
		if (bControl) {		// ｺﾝﾄﾛｰﾙｷｰも押されている？
			cur.Initialize();
		}else{
			cur.D.x = 0;
		}
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		bSetTarget = true;
		break;
	case VK_END:
		if (bControl) {		// ｺﾝﾄﾛｰﾙｷｰも押されている？
			cur.D.y = NPOS;
		}
		cur.D.x = NPOS;
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		bSetTarget = true;
		break;
	case VK_PRIOR:
		cur.D.y = cur.D.y>m_cydsp_page ? (cur.D.y - max(m_cydsp_page-1, 1)) : 0;
		cur.D.x = m_cursor.GetTarget();
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		i =count; //ﾘﾋﾟｰﾄしない
		break;
	case VK_NEXT:
		cur.D.y += max(m_cydsp_page-1, 1);
		cur.D.x = m_cursor.GetTarget();
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		i =count; //ﾘﾋﾟｰﾄしない
		break;
	case VK_LEFT:
		if (!(bExpand_Old && !m_cursor.IsExpand()))
			pHint->PrevCur(cur, GetEnv()->m_bFreeCursor);
		bSetTarget = true;
		break;
	case VK_RIGHT:
		if (!(bExpand_Old && !m_cursor.IsExpand()))
			pHint->NextCur(cur, GetEnv()->m_bFreeCursor);
		bSetTarget = true;
		break;
	case VK_UP:
		cur.D.y = cur.D.y>0 ? (cur.D.y - 1) : 0;
		cur.D.x = m_cursor.GetTarget();
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		break;
	case VK_DOWN:
		cur.D.y++;
		cur.D.x = m_cursor.GetTarget();
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		break;
	default:
		//nop!!!
		pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
		break;
	}
	}//for	
	m_cursor.SetBlinkCursor(cur, bSetTarget);
	
	if (coursorOld == m_cursor) {
		if (cur.D.y>0) {
			if (nChar==VK_DOWN || nChar==VK_NEXT)
				iScroll  = 1;
		}
	}else {
		if (nChar==VK_PRIOR)
			iScroll = -2;
		if (nChar==VK_NEXT)
			iScroll = 2;
	}
	return iScroll;	//スクロールする
}

bool CViewEd::InputString(CAction *paction, ACT act)
{
	assert(IsValidEnv());
	CLineHint *pHint = GetHint();
	CCursor cursor_befor = m_cursor;//save now cursor condifion.

	if (act==act_input_ime && theIme.IsInComposition()) { //WordWrapで前の行へ追い込みチェック一番最初だけ。
		if (!m_cursor.IsExpand() && m_cursor.GetBlinkCursor().D.x==0) {
			CUR cur = m_cursor.Befor(); //現在カーソルの最初をデフォルトセット
			OBJOFF row = cur.D.y;
			if (cur.D.x == 0 && pHint->GetThis_LineTop(row) < row) {
				//直前行の行末にセット
				cur.D.y = row-1;
				cur.D.x = NPOS;
				pHint->SetCur(cur, false);
				cursor_befor.SetBlinkCursor(cur);
			}
		}
	}


	assert(cursor_befor.IsValid());
	
	CActionLgc *pactLgc = new CActionLgc;
	paction->m_pactLgc = pactLgc;

	assert(paction->m_pstring);
	paction->SetBeforCursor(cursor_befor);

	pHint->SetActionLgc(pactLgc->m_actBlock[CActionLgc::act_block_del],
		pactLgc->m_actBlock[CActionLgc::act_block_ins],
		pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor,
		pactLgc->m_actBlock[CActionLgc::act_block_after].m_cursor,
		paction->m_pstring, (CTextCashTbl *)m_pObjLayTbl->m_pEditObj);
	paction->clear_str();

	if (ActionUndoRedo(pactLgc->m_actBlock[CActionLgc::act_block_befor],
		pactLgc->m_actBlock[CActionLgc::act_block_after])) {
	//	pactLgc->m_cursor_after = m_cursor;
		if (act==act_input_ime) {
			m_cursor.SetExpand();
			CUR cur = cursor_befor.Befor();
			pHint->SetCur(cur, GetEnv()->m_bFreeCursor);
			m_cursor.SetOrgCursor(cur);
			assert(m_cursor.IsValid());
			//blink cursorはそのまま
		}else if (act==act_input_ime_result) {
			if (m_pObjLayTbl->GetActions().IsBeforInputIme()) {
				CAction *pactBef = m_pObjLayTbl->GetActions().pop_undo();
				if (paction->m_pactLgc->m_actBlock[CActionLgc::act_block_after].m_string.size()==0) {
				//	m_pObjLayTbl->GetActions().Erase();
					assert(m_cursor.IsValid());
					return false;//リザルト文字列がなければアクションを未登録に
				}else{
					paction->SetBeforCursor(pactBef->m_pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor);
					//paction->m_pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor =
					//	pactBef->m_pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor;
					m_pObjLayTbl->GetActions().pop_redo();
					m_pObjLayTbl->GetActions().ChangeBefor2ImeResultAction();
					assert(m_cursor.IsValid());
				}
			}else if (paction->m_pactLgc->m_actBlock[CActionLgc::act_block_after].m_string.size()==0) {
				assert(m_cursor.IsValid());
				return false;//リザルト文字列がなければアクションを未登録に
			}

		}else{
//			pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor = cursor_befor;
		}
		assert(m_cursor.IsValid());
		//m_pObjLayTbl->GetActions().push_action(paction);->move to caller for update window!
	//	paction->m_pactLgc = null;
	}

	return true;
}

void CViewEd::VGetScrollInfo_NoSwitch(int nBar, LPSCROLLINFO lpInfo, UINT nMask) 
{
	assert((nMask & SIF_ALL) == SIF_ALL);
	m_pWnd->GetScrollInfo(nBar, lpInfo, nMask);
	if (nBar==SB_HORZ)
		m_posScrollH = lpInfo->nPos;
	else
		m_posScrollV = lpInfo->nPos;
	if (IsVert() && nBar==SB_HORZ) {
		int nRev = lpInfo->nMax - lpInfo->nPage;
		lpInfo->nPos = (nRev - lpInfo->nPos)<0 ? 0:(nRev - lpInfo->nPos);
		lpInfo->nTrackPos = (nRev - lpInfo->nTrackPos)<0 ? 0:(nRev - lpInfo->nTrackPos);
		assert(lpInfo->nPos>=0);
	}
}
void CViewEd::VSetScrollInfo_NoSwitch(int nBar, LPSCROLLINFO lpInfo, BOOL bRedraw) 
{
	assert((lpInfo->fMask & SIF_RANGE) && (lpInfo->fMask & SIF_PAGE) && (lpInfo->fMask & SIF_POS));
	if (IsVert() && nBar==SB_HORZ) {
		int nRev = lpInfo->nMax - lpInfo->nPage;
		lpInfo->nPos = (nRev - lpInfo->nPos)<0 ? 0:(nRev - lpInfo->nPos);
		lpInfo->nTrackPos = (nRev - lpInfo->nTrackPos)<0 ? 0:(nRev - lpInfo->nTrackPos);
		assert(lpInfo->nPos>=0);
	}
	if (nBar==SB_HORZ)
		m_posScrollH = lpInfo->nPos;
	else
		m_posScrollV = lpInfo->nPos;
	m_pWnd->SetScrollInfo(nBar, lpInfo, bRedraw);
}
int CViewEd::VGetScrollPos_NoSwitch(int nBar) 
{
	if (IsVert() && nBar==SB_HORZ) {
		SCROLLINFO sb_info;
		VGetScrollInfo_NoSwitch(nBar, &sb_info) ;
		assert(sb_info.nPos>=0);
		return sb_info.nPos;
	}else{
		if (m_pWnd==null) //takaso2010 bugfix
			return 0;
		return (nBar==SB_HORZ ? 
			m_posScrollH = m_pWnd->GetScrollPos(nBar):
			m_posScrollV = m_pWnd->GetScrollPos(nBar));
	}
}
void CViewEd::VSetScrollPos_NoSwitch( int nBar, int nPos, BOOL bRedraw)
{
	if (IsVert() && nBar==SB_HORZ) {
		SCROLLINFO sb_info;
		m_pWnd->GetScrollInfo(nBar, &sb_info);
		nPos = abs(sb_info.nMax - (int)sb_info.nPage - nPos);
		assert(nPos>=0);
	}
	if (nBar==SB_HORZ)
		m_posScrollH = nPos;
	else
		m_posScrollV = nPos;
	m_pWnd->SetScrollPos(nBar, nPos, bRedraw); 
}


UINT CViewEd::VSwitchSBCode(UINT nSBCode, const int nBar)
{
	if (IsVert() && nBar==SB_HORZ) {
		switch(nSBCode)	{
		case SB_TOP:
			nSBCode = SB_BOTTOM;
			break;
		case SB_LINEUP:
			nSBCode = SB_LINEDOWN;
			break;
		case SB_LINEDOWN:
			nSBCode = SB_LINEUP;
			break;
		case SB_PAGEUP:
			nSBCode = SB_PAGEDOWN;
			break;
		case SB_PAGEDOWN:
			nSBCode = SB_PAGEUP;
			break;
		case SB_BOTTOM:
			nSBCode = SB_TOP;
			break;
		default:
			break;
		}
	}
	return nSBCode;
}
///////////////////////////////////////////////////////////////////////////////////////
//CViewEdTbl class
CViewEdTbl::CViewEdTbl()
{
}

CViewEdTbl::~CViewEdTbl()
{
	for (iterator iter=begin(); iter!=end(); iter++) {
		delete (*iter);
	}
}

CViewEd *CViewEdTbl::GetEntry(CObjLayTbl *pLay)
{
	CViewEd *pViewEd;
	//既存の余ってるのを取得
	for (iterator iter=begin(); iter!=end(); iter++) {
		pViewEd = (*iter);
		if (pViewEd->m_pObjLayTbl==pLay && pViewEd->m_pWnd==null)
			return pViewEd;
	}
	//新しくつくらにゃあかんで
	pViewEd = new CViewEd;
	push_back(pViewEd);
	return pViewEd;
}

