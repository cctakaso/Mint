#ifndef __VIEWED_H__
#define __VIEWED_H__
#include <set>

#include "resource.h"
#include "sell.h"
#include "ObjLay.h"
#include "action.h"
#include "ViewEnv.h"
#include "ime.h"

class CKeywords;
class CViewEd;

class CCharsFontAttr
{
	friend class CViewEd;
public:
	CCharsFontAttr();
	~CCharsFontAttr() {};

protected:
	void UpdateSizeOnly(bool bGet, CViewEd *pViewEd);
public:
	CSize		sizeFont;
	int			cyLine;
	int			cyBeforLine;
	int			cxhalf_font;
	int			cyhalf_font;
	int			width_linenum;
};

#ifdef SPEC_PRINT
typedef enum {
	DRAW_DSP,
	DRAW_PRN,
	DRAW_PRV,
}DRAW_MODE;
class CCharsPrintAttr : public CCharsFontAttr
{
	friend class CViewEd;
public:
	CCharsPrintAttr();
	~CCharsPrintAttr() {};

	void SetPrinter(CViewEd *pViewEd, CDC *pDCPrint);
	void SetDisplay(CViewEd *pViewEd, CDC *pDCPrint);

private:
	void Update(bool bGet, CViewEd *pViewEd);
	void SetPrinterDefault(bool bColor);
public:
	CCharsPrintAttr	*pOld;
	DRAW_MODE	modeDraw;
	bool		bViewTab;
	bool		bViewCRLF;
	bool		bViewEOF;
	bool		bVert;
	bool		bLineNum;
	COLORREF colorText;
	COLORREF colorLineNum;
	COLORREF colorEOF;
	COLORREF colorTextBk;
	COLORREF colorText_Select;
	COLORREF colorTextBk_Select;
	COLORREF colorText_Mail;
	COLORREF colorText_Url;
};
#endif //SPEC_PRINT


class DrawRect: public CRect
{
public:
	DrawRect();
	bool isHit();
	bool Click();
};

typedef set<DrawRect> DrawRectSet;



class CViewEd
{
	friend class CWinView;
	friend class CObjCashFile;
	friend class CFindDlg;
	friend class CTheDoc;
	friend class CJumpDlg;
	friend class CMainFrame;
	friend class CFlpMark;
	friend class CBookMark;
	friend class CViewEdTbl;
	friend class CFileSelectBar;
	friend class CTheDocTemplate;
	friend class CIme;
	friend class CCharsFontAttr;
#ifdef SPEC_PRINT
	friend class CCharsPrintAttr;
#endif //#ifdef SPEC_PRINT

public:
    CViewEd();
    ~CViewEd();
public:
//vertical drawing
	const bool IsVert() {
		return (m_pObjLayTbl && m_pObjLayTbl->GetEnv()) ? m_pObjLayTbl->GetEnv()->m_bVert:false; }
	const bool IsHorz() {
		return !IsVert(); }

protected:
//env handling
	CObjLayTbl *Attach(CObjLayTbl *pObjLayTbl, CWnd *pWnd);
	CObjLayTbl *Detach(bool bCloseDoc=false);
	void ChangeFonts(CWnd *pWnd, CRect rcClient, int iZoom=0);
	void ChangeColors(CWnd *pWnd, bool bAct=true, bool bSetClassOnly=false);
	bool ChangeLineNum();
	CViewEnv *GetEnv() {
		assert(IsValidEnv());
		return m_pObjLayTbl->GetEnv(); }
	bool IsValidEnv() {
		return m_pObjLayTbl ? m_pObjLayTbl->IsValidEnv():false; }
	CPoint Sell2Point_Win(OBJOFF x, OBJOFF y, bool bLast=false);
	void ReBuildHint();
	CLineHint *GetHint() {
		assert(IsValidEnv());
		return m_pObjLayTbl->m_phint; }
//display handling
#ifdef SPEC_PRINT
	bool OnDraw(CDC *pDC, int xPos, int yPos, CRect rcClip, bool bEraseBack=true, DRAW_MODE modeDraw=DRAW_DSP);
#else
	bool OnDraw(CDC *pDC, int xPos, int yPos, CRect rcClip, bool bEraseBack=true);
#endif
	void CViewEd::InvalidateRectLine(CWnd *pWnd, CRect *prc);
//scrollbar handling
	bool SetScrolls(CWnd *pWnd, CRect rcClient, bool bVSwitch=false, int posH=-1, int posV=-1);
	void PushPopScrolls(CWnd *pWnd, CRect rcClient, bool bPop);

//action handling
	int Action(CAction *paction, Normal_Proc pProc=null, void *pData=null);
	int MoveCoursor(UINT nChar, const  bool bControl=false, const bool bShift=false, const int count=1);
	bool InputString(CAction *paction, ACT act);
	bool ActionUndoRedo(CActionBlock &actBlock_befor, CActionBlock &actBlock_after);
	const SELL& GetBlinkSell(void){
		return GetBlinkCursor().D; }
	CSize GetSizeFont() {
		return m_sizeFont; }
	CSize VGetSizeFont() {
		return VSwitchSize(m_sizeFont); }
	CSize GetSizeCell() {
		return CSize(m_sizeFont.cx, m_cyLine); }
	CSize VGetSizeCell() {
		return VSwitchSize(GetSizeCell()); }

	CPoint GetViewOrg() {
		return CPoint(m_width_linenum, 0); }
	CUR_SIZE GetCurSize() {
		assert(m_pObjLayTbl && m_sizeFont.cy);
		return CUR_SIZE(m_pObjLayTbl->GetCurSize().cx + GetViewOrg().x/m_sizeFont.cx,
			m_pObjLayTbl->GetCurSize().cy + GetViewOrg().y/m_sizeFont.cy); }

//Mouse handling
	bool OnMouseMove(int yPos, int xPos, CPoint point, UINT nFlags, bool bDown=false, bool bUp=false);
	bool OnMouseDblClk(int yPos, int xPos, CPoint point, UINT nFlags);
	string GetCursorWord_2Expand(bool bTalCurOk=false);
	string GetCursorWord(bool bMultiLinesOk=false);
	string GetCursorMultiLineWord();
	void GetWordCursor(CCursor &cursor, bool bEnableSpace, bool bTalCurOk=false);
	void SetMouseCursor() {
		SetCursor((IsVert() && m_bIbeam) ? AfxGetApp()->LoadCursor(IDC_CUR_VERT):
		AfxGetApp()->LoadStandardCursor(m_bIbeam ? IDC_IBEAM:IDC_ARROW)); }
	void SetExpandLineCursor(CCursor *pCursor=null);
//Cursor hadling
	void SetMCursor(const CCursor &cursor);
//Jump handling
	bool JumpLine(OBJOFF lineJump, JUMPTYPE JumpType, OBJOFF col=0);
//Ime handling
	void DrawISA(int xdot, int ydot, int xdot_first, IME_STRING_ATTR isa, IME_STRING_ATTR isa_new=isa_no);

//CCaret handling
	CRect GetBlinkCursorRect(bool bLine=false);
	CRect GetBlinkCursorRect_Win();
	const CUR& GetBlinkCursor(void) {
		return m_cursor.GetBlinkCursor(); }
	void DrawBlinkCursor(CWnd *pWnd, bool bDraw);
	void InvalidateCursor(CWnd *pWnd, CCursor cursor, int xPos, int yPos);
	void ResetCaret(CWnd *pWnd);

private:
	void VCopyDC(CDC *pDC) {
		m_pDC = pDC; }
	void VCopyWnd(CWnd *pWnd) {
		m_pWnd = pWnd; }
	void VTextOut(int x, int y, LPCTSTR lpsz, int nCount) {
		IsHorz() ? m_pDC->ExtTextOut(x, y, 0 , null, lpsz, nCount, null):
					m_pDC->ExtTextOut(y, x, 0 , null, lpsz, nCount, null); }
		//IsHorz() ? m_pDC->TextOut(x,y,lpsz,nCount):m_pDC->TextOut(y,x,lpsz,nCount); }
	void VFillRect(LPCRECT lpRect, CBrush* pBrush) {
		IsHorz() ? m_pDC->FillRect(lpRect, pBrush):m_pDC->FillRect(VSwitchRect_View2(*lpRect), pBrush); }
	void VFillSolidRect(LPCRECT lpRect, COLORREF color) {
		IsHorz() ? m_pDC->FillSolidRect(lpRect, color):m_pDC->FillSolidRect(VSwitchRect_View2(*lpRect), color); }
	void VMoveTo(int x, int y) {
		IsHorz() ? m_pDC->MoveTo(x, y):m_pDC->MoveTo(y, x); }
	void VLineTo(int x, int y) {
		IsHorz() ? m_pDC->LineTo(x, y):m_pDC->LineTo(y, x); }
	void VInvertRect(LPCRECT lpRect) {
		CRect rc(lpRect->top, lpRect->left, lpRect->bottom, lpRect->right);
		m_pDC->InvertRect(IsHorz() ? lpRect:(LPCRECT)rc); }
	CWnd* VGetScrollBarCtrl(int nBar) {
		return m_pWnd->GetScrollBarCtrl(VSwitchBar(nBar)); }

	UINT VSwitchKey(UINT nChar);
	UINT VSwitchSBCode(UINT nSBCode, const int nBar);
	void VGetScrollInfo(int nBar, LPSCROLLINFO lpInfo, UINT nMask = SIF_ALL) {
		VGetScrollInfo_NoSwitch(VSwitchBar(nBar), lpInfo, nMask); }
	void VSetScrollInfo(int nBar, LPSCROLLINFO lpInfo, BOOL bRedraw = TRUE) {
		VSetScrollInfo_NoSwitch(VSwitchBar(nBar), lpInfo, bRedraw); }
	int  VGetScrollPos(int nBar) {
		return VGetScrollPos_NoSwitch(VSwitchBar(nBar)); }
	void VSetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE) {
		VSetScrollPos_NoSwitch(VSwitchBar(nBar), nPos, bRedraw); }

	void VGetScrollInfo_NoSwitch(int nBar, LPSCROLLINFO lpInfo, UINT nMask = SIF_ALL);
	void VSetScrollInfo_NoSwitch(int nBar, LPSCROLLINFO lpInfo, BOOL bRedraw = TRUE);
	int  VGetScrollPos_NoSwitch(int nBar);
	void VSetScrollPos_NoSwitch(int nBar, int nPos, BOOL bRedraw = TRUE);

	CSize VSwitchSize(const CSize &size) {
		return IsHorz() ? CSize(size):CSize(size.cy, size.cx); }
	CRect VSwitchRect_Win(const CRect &rc) {
		return IsHorz() ? rc:CRect(m_rcClient.Width()-rc.bottom,
			rc.left, m_rcClient.Width()-rc.top, rc.right); }
	CRect VSwitchRect_View(const CRect &rc) {
		return IsHorz() ? rc:CRect(rc.top, m_rcClient.right-rc.right, rc.bottom, m_rcClient.right-rc.left); }
	CRect VSwitchRect_View2(const CRect &rc) {
		return IsHorz() ? rc:CRect(rc.top, rc.left, rc.bottom, rc.right); }
	CPoint VSwitchPoint_Win(const CPoint &pt) {
		return IsHorz() ? pt:CPoint(m_rcClient.right - pt.y, pt.x); }
	CPoint VSwitchPoint_View(const CPoint &pt) {
		return IsHorz() ? pt:CPoint(pt.y, m_rcClient.right-pt.x); }
	CPoint VSwitchPoint(const CPoint &pt) {
		return IsHorz() ? pt:CPoint(-pt.y, pt.x); }
	int VSwitchBar(const int &nBar) {
		return IsHorz() ?  nBar:(nBar==SB_HORZ ? SB_VERT:SB_HORZ); }
	//CUR_SIZE VSwitchCurSize() {
	//	return IsHorz() ? GetCurSize():CUR_SIZE(GetCurSize().cy, GetCurSize().cx); }
	//const SELL& VSwitchBlinkSell(void){
	//	return IsHorz() ? GetBlinkSell():SELL(GetBlinkSell().y, GetBlinkSell().x); }
	void ChoosePrintFont(CDC *pdcPrint);
	void ResetCharSizes(CDC *pDC, CCharsFontAttr &chardata);
	OBJSIZE GetLineMaxNum(bool bRow=false);

	CDC		*m_pDC;
	bool	m_bAct;
	CWnd	*m_pWnd;
	CRect	m_rcClient;
	int m_leftClip_save;
	CRect	m_rcClip_save;
	int		m_posScrollH;
	int		m_posScrollV;

	COLORREF m_colorText;
	COLORREF m_colorLineNum;
	COLORREF m_colorEOF;
	COLORREF m_colorTextBk;
	COLORREF m_colorText_Select;
	COLORREF m_colorTextBk_Select;
	COLORREF m_colorText_Mail;
	COLORREF m_colorText_Url;

	bool m_bViewTab;
	bool m_bViewCRLF;
	bool m_bViewEOF;
	bool m_bViewMail;
	bool m_bViewURL;

	CPen m_penLineNumLine;
	CPen m_penTab;
	CPen m_penCRLF;
	CBrush m_brushTextBk;

	CCursor		m_cursor;
	CFont		m_font;
	CSize		m_sizeFont;
	int			m_cyLine;
	int			m_cyBeforLine;
	int			m_cxhalf_font;
	int			m_cyhalf_font;
	CObjLayTbl* m_pObjLayTbl;
	DSP_Y		m_cydsp_page;
//	DSP_X		m_colTarget;
	int			m_width_linenum;
	bool		m_bIbeam;
	CKeywords	*m_pkeywords_word;
	CKeywords	*m_pkeywords_unword;
	DrawRectSet m_drawRectSet;

#ifdef SPEC_PRINT
	DRAW_MODE	m_modeDraw;
#endif
//	CActions	m_actions;->move to layout!

//display handling
	void ResetDrawFlgs();
	bool OnDraw(OBJOFF row, int ydot, int xdot, int left, int right);
	CSize GetExpandCursorLgcSize(OBJOFF row, bool &bHit);
	bool IsExpandCursorArea(const OBJOFF &y, const OBJOFF &x, const SELL &sellBefor, const SELL &sellEnd) {
		return m_cursor.IsExpand() &&
			(sellBefor.y <= y && y <= sellEnd.y) &&
			(sellBefor.y != y || sellBefor.x<=x) &&
			(sellEnd.y != y || x<sellEnd.x); }

	void DrawTextBlock(int xdot, int ydot, BYTE *sz, int len, int len_befor, COLORREF color, int &leftNormal, bool bUnderline=true);
	void DrawCRLF(int xdot, int ydot, bool bEnableView);
	void DrawTAB(int xdot, int ydot, int lenTab, bool bEnableView);
	void DrawEOF(int xdot, int ydot);
	void DrawLineNum(OBJOFF num, int ydot, int xdotLeft);

	//const CSize &sizeClientDsp2Lgc(const CSize &sizeClient) {
	CSize sizeClientDsp2Lgc(const CSize &sizeClient) {
		return CSize(sizeClient.cx/m_sizeFont.cx + sizeClient.cx%m_sizeFont.cx,
			sizeClient.cy/m_sizeFont.cy + sizeClient.cy%m_sizeFont.cy);
	}
	void SetFont(CWnd *pWnd, int iZoom=0);
	CPoint Sell2Point_Win(OBJOFF x, OBJOFF y, int xPos, int yPos, bool bLast=false);
	CRect Cursor2Rect_OneLine(const SELL &sell_first, const SELL &sell_last, const int xPos, const int yPos);
};

class CViewEdTbl : public vector<CViewEd *>//deque<SELL>
{
public:
	CViewEdTbl();
	~CViewEdTbl();
public:
	CViewEd *GetEntry(CObjLayTbl *pLay);
};

class CCaret
{
public:
	CCaret();
	~CCaret();
public:
	void Initialize(CWnd *pWnd=null, const CSize *psize=null);
	void MyDestroyCaret();
	void ShowCaret();
	void HideCaret();
	void ResetCaret(CWnd *pWnd, const CRect *prc=null, COLORREF colorref=RGB(0,0,0));
	void MoveCaret(const CPoint pt);
private:
	bool IsValid() {
		return m_pWnd!=null; }
	CWnd	*m_pWnd;
	bool	m_bShow;
	CPoint	m_pt;
	CSize	m_size;
};


#endif //#define __VIEWED_H__
