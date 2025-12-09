// PageDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "version.h"
#include "the.h"
#include "ViewEd.h"
#include "PageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTheApp theApp;
extern CViewEdTbl theViewEdTbl;

#ifdef SPEC_PRINT
/////////////////////////////////////////////////////////////////////////////
// CPageDlg ダイアログ


CPageDlg::CPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageDlg)
	m_iMarginTop = 0;
	m_iMarginRight = 0;
	m_iMarginLeft = 0;
	m_iMarginBottom = 0;
	m_strHeaderLeft = _T("");
	m_strHeaderRight = _T("");
	m_strHeaderCenter = _T("");
	m_strFooterRight = _T("");
	m_strFooterLeft = _T("");
	m_strFooterCenter = _T("");
	m_bColor = FALSE;
	m_bLineNum = FALSE;
	m_bHeaderLine = FALSE;
	m_bFooterLine = FALSE;
	//}}AFX_DATA_INIT
}


void CPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDlg)
	DDX_Text(pDX, IDC_EDIT_TM, m_iMarginTop);
	DDV_MinMaxInt(pDX, m_iMarginTop, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_RM, m_iMarginRight);
	DDV_MinMaxInt(pDX, m_iMarginRight, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_LM, m_iMarginLeft);
	DDV_MinMaxInt(pDX, m_iMarginLeft, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_BM, m_iMarginBottom);
	DDV_MinMaxInt(pDX, m_iMarginBottom, 0, 1000);
	DDX_CBString(pDX, IDC_CMB_HEADER_LEFT, m_strHeaderLeft);
	DDX_CBString(pDX, IDC_CMB_HEADER_RIGHT, m_strHeaderRight);
	DDX_CBString(pDX, IDC_CMB_HEADER_CENTER, m_strHeaderCenter);
	DDX_CBString(pDX, IDC_CMB_FOOTER_RIGHT, m_strFooterRight);
	DDX_CBString(pDX, IDC_CMB_FOOTER_LEFT, m_strFooterLeft);
	DDX_CBString(pDX, IDC_CMB_FOOTER_CENTER, m_strFooterCenter);
	DDX_Check(pDX, IDC_CHK_COLOR, m_bColor);
	DDX_Check(pDX, IDC_CHK_LINENUM, m_bLineNum);
	DDX_Check(pDX, IDC_CHK_HEADER_LINE, m_bHeaderLine);
	DDX_Check(pDX, IDC_CHK_FOOTER_LINE, m_bFooterLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDlg, CDialog)
	//{{AFX_MSG_MAP(CPageDlg)
	ON_BN_CLICKED(IDC_BTN_PAGE_SETUP, OnBtnPageSetup)
	ON_BN_CLICKED(IDC_BTN_PRINT_SETUP, OnBtnPrintSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDlg メッセージ ハンドラ

BOOL CPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPrintEnv *pPrintEnv = &theViewEnvTbl.m_printenv;
	CRect rcMargin = pPrintEnv->m_rcMargin;
	m_iMarginTop = rcMargin.top;
	m_iMarginBottom = rcMargin.bottom;
	m_iMarginLeft = rcMargin.left;
	m_iMarginRight = rcMargin.right;

	m_bLineNum = (pPrintEnv->m_iLineNum!=0);
	m_bColor =  bool2BOOL(pPrintEnv->m_bColor);
	m_strHeaderLeft = string2CString(pPrintEnv->m_strHeaderLeft);
	m_strHeaderCenter = string2CString(pPrintEnv->m_strHeaderCenter);
	m_strHeaderRight = string2CString(pPrintEnv->m_strHeaderRight);
	m_strFooterLeft = string2CString(pPrintEnv->m_strFooterLeft);
	m_strFooterCenter = string2CString(pPrintEnv->m_strFooterCenter);
	m_strFooterRight = string2CString(pPrintEnv->m_strFooterRight);
	m_bHeaderLine = bool2BOOL(pPrintEnv->m_bHeaderLine);
	m_bFooterLine = bool2BOOL(pPrintEnv->m_bFooterLine);
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPageDlg::OnBtnPageSetup() 
{
	UpdateData(TRUE);

	PRINTDLG pd;
	//CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		AfxMessageBox("Default Printer ERR!");
		return;
	}

	//PAGESETUPDLG psd;    // common dialog box structure
	//HWND hwnd;           // owner window// Initialize PAGESETUPDLG
	//ZeroMemory(&psd, sizeof(PAGESETUPDLG));
	//psd.lStructSize = sizeof(PAGESETUPDLG);
	//psd.hwndOwner   = AfxGetMainWnd()->GetSafeHwnd();
	//psd.hDevMode    = NULL; // Don't forget to free or store hDevMode.
	//psd.hDevNames   = NULL; // Don't forget to free or store hDevNames.
	//psd.Flags       = PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS;
	//psd.lpfnPagePaintHook = NULL;

	//CRect rcMargin = CRect(m_iMarginLeft, m_iMarginTop, m_iMarginRight, m_iMarginBottom);
	//int nUnitsPerInch = 100;//2540;
	//MulDivRect(&psd.rtMargin, rcMargin, nUnitsPerInch, 1);
	//RoundRect(&psd.rtMargin);
	//if (PageSetupDlg(&psd)==TRUE) {
		// check paper size and margin values here}


	CPageSetupDialog dlg;
	PAGESETUPDLG *psd = &dlg.m_psd;
	psd->hDevNames = pd.hDevNames;
	psd->hDevMode = pd.hDevMode;
	CRect rcMargin = CRect(m_iMarginLeft, m_iMarginTop, m_iMarginRight, m_iMarginBottom);
#ifdef _DBCS
	psd->Flags |= PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS;
	int nUnitsPerInch = 100;//2540;
	MulDivRect(&psd->rtMargin, rcMargin, nUnitsPerInch, 1);
#else
	psd->Flags |= PSD_MARGINS | PSD_INTHOUSANDTHSOFINCHES;
	int nUnitsPerInch = 1000;
	MulDivRect(&psd->rtMargin, rcMargin, nUnitsPerInch, 1440);
#endif
	RoundRect(&psd->rtMargin);
	// get the current device from the app
//	PRINTDLG pd;
//	pd.hDevNames = NULL;
//	pd.hDevMode = NULL;
//	theApp.GetPrinterDeviceDefaults(&pd);
//	psd.hDevNames = dlgPrint.m_pd.hDevNames;
//	psd.hDevMode = dlgPrint.m_pd.hDevMode;
#ifdef _DEBUG
	dlg.Dump(afxDump);
	afxDump << "DevMode pointer = " << dlg.GetDevMode() << "\n";
	afxDump << "DevNames pointer = " << psd->hDevNames << "\n";
	afxDump << "DevName = " << dlg.GetDriverName() << "\n";
	afxDump << "DevName = " << dlg.GetDeviceName() << "\n";
	afxDump << "DevName = " << dlg.GetPortName() << "\n";
#endif //_DEBUG


	if (dlg.DoModal() == IDOK) {
		RoundRect(&psd->rtMargin);
#ifdef _DBCS
		MulDivRect(rcMargin, &psd->rtMargin, 1, nUnitsPerInch);
		m_iMarginTop = rcMargin.top;
		m_iMarginBottom = rcMargin.bottom;
		m_iMarginLeft = rcMargin.left;
		m_iMarginRight = rcMargin.right;
#else
		MulDivRect(rcMargin, &psd->rtMargin, 1440, nUnitsPerInch);
#endif
		UpdateData(FALSE);
		theApp.SelectPrinter(psd->hDevNames, psd->hDevMode, FALSE);
		//theApp.NotifyPrinterChanged();
	}
}

void CPageDlg::OnOK() 
{
	UpdateData(TRUE);
	CPrintEnv *pPrintEnv = &theViewEnvTbl.m_printenv;
	pPrintEnv->m_rcMargin = CRect(m_iMarginLeft, m_iMarginTop, m_iMarginRight, m_iMarginBottom);
	pPrintEnv->m_iLineNum = (m_bLineNum ? 1:0);
	pPrintEnv->m_bColor = BOOL2bool(m_bColor);
	pPrintEnv->m_strHeaderLeft = CString2string(m_strHeaderLeft);
	pPrintEnv->m_strHeaderCenter = CString2string(m_strHeaderCenter);
	pPrintEnv->m_strHeaderRight = CString2string(m_strHeaderRight);
	pPrintEnv->m_strFooterLeft = CString2string(m_strFooterLeft);
	pPrintEnv->m_strFooterCenter = CString2string(m_strFooterCenter);
	pPrintEnv->m_strFooterRight = CString2string(m_strFooterRight);
	pPrintEnv->m_bHeaderLine = BOOL2bool(m_bHeaderLine);
	pPrintEnv->m_bFooterLine = BOOL2bool(m_bFooterLine);
	CDialog::OnOK();
}
void CPageDlg::OnBtnPrintSetup() 
{
	CPrintDialog pd(TRUE);
	AfxGetApp()->DoPrintDialog(&pd);
}

#ifdef NOTUSE
typedef enum {
	UNIT_MMx10,
	UNIT_INCHx100,
}UNIT;
typedef struct {
	short	dmPaperSize;
	UNIT	unit;
	int		cx;
	int		cy;
}PAPER_SIZE;

const static PAPER_SIZE svPaperSizes[] = {
	{DMPAPER_A4,		UNIT_MMx10,	2100, 2970}, //DMPAPER_A4   A4 シート、210 × 297 mm
	{DMPAPER_B5,		UNIT_MMx10,	1820, 2570}, //DMPAPER_B5   B5 シート、182 × 257 mm
	{DMPAPER_A3,		UNIT_MMx10,	2970, 4200}, //DMPAPER_A3   A3 シート、297 × 420 mm
	{DMPAPER_A4SMALL,	UNIT_MMx10,	2100, 2970}, //DMPAPER_A4SMALL   A4 small シート、210 × 297 mm
	{DMPAPER_A5,		UNIT_MMx10,	1480, 2100}, //DMPAPER_A5   A5 シート、148 × 210 mm
	{DMPAPER_B4,		UNIT_MMx10,	2500, 3540}, //DMPAPER_B4   B4 シート、250 × 354 mm
	{DMPAPER_LETTER,	UNIT_INCHx100,	850, 1100}, //DMPAPER_LETTER   Letter、8 1/2 × 11 インチ
	{DMPAPER_LEGAL,		UNIT_INCHx100,	850, 14000}, //DMPAPER_LEGAL   Legal、8 1/2 × 14 インチ
	{DMPAPER_CSHEET,	UNIT_INCHx100,	1700, 2200}, //DMPAPER_CSHEET   C シート、17 × 22 インチ
	{DMPAPER_DSHEET,	UNIT_INCHx100,	2200, 3400}, //DMPAPER_DSHEET   D シート、22 × 34 インチ
	{DMPAPER_ESHEET,	UNIT_INCHx100,	3400, 4400}, //DMPAPER_ESHEET   E シート、34 × 44 インチ
	{DMPAPER_LETTERSMALL,	UNIT_INCHx100,	850, 1100}, //DMPAPER_LETTERSMALL   Letter Small、8 1/2 × 11 インチ
	{DMPAPER_TABLOID,	UNIT_INCHx100,	1100, 1700}, //DMPAPER_TABLOID   Tabloid、11 × 17 インチ
	{DMPAPER_LEDGER,	UNIT_INCHx100,	1700, 1100}, //DMPAPER_LEDGER   Ledger、17 × 11 インチ
	{DMPAPER_STATEMENT,	UNIT_INCHx100,	550, 850}, //DMPAPER_STATEMENT   Statement、5 1/2 × 8 1/2 インチ
	{DMPAPER_EXECUTIVE,	UNIT_INCHx100,	725, 1050}, //DMPAPER_EXECUTIVE   Executive、7 1/4 × 10 1/2 インチ
	{DMPAPER_FOLIO,	UNIT_INCHx100,	850, 1300}, //DMPAPER_FOLIO   Folio、8 1/2 × 13 インチ
	{DMPAPER_QUARTO,	UNIT_MMx10,	2150, 2750}, //DMPAPER_QUARTO   Quarto、215 × 275 mm
	{DMPAPER_10X14,	UNIT_INCHx100,	1000, 1400}, //DMPAPER_10X14   10 × 14 インチ シート
	{DMPAPER_11X17,	UNIT_INCHx100,	1100, 1700}, //DMPAPER_11X17   11 × 17 インチ シート
	{DMPAPER_NOTE,	UNIT_INCHx100,	850, 1100}, //DMPAPER_NOTE   Note、 8 1/2 × 11 インチ
	{DMPAPER_ENV_9,	UNIT_INCHx100,	386, 886}, //DMPAPER_ENV_9   #9 Envelope、3 7/8 × 8 7/8 インチ
	{DMPAPER_ENV_10,	UNIT_INCHx100,	416, 950}, //DMPAPER_ENV_10   #10 Envelope、4 1/8 × 9 1/2 インチ
	{DMPAPER_ENV_11,	UNIT_INCHx100,	450, 1038}, //DMPAPER_ENV_11   #11 Envelope、4 1/2 × 10 3/8 インチ
	{DMPAPER_ENV_12,	UNIT_INCHx100,	475, 1100}, //DMPAPER_ENV_12   #12 Envelope、4 3/4 × 11 インチ
	{DMPAPER_ENV_14,	UNIT_INCHx100,	500, 1150}, //DMPAPER_ENV_14   #14 Envelope、5 × 11 1/2 インチ
	{DMPAPER_ENV_DL,	UNIT_MMx10,	1100, 2200}, //DMPAPER_ENV_DL   DL Envelope、110 × 220 mm
	{DMPAPER_ENV_C5,	UNIT_MMx10,	1620, 2290}, //DMPAPER_ENV_C5   C5 Envelope、162 × 229 mm
	{DMPAPER_ENV_C3,	UNIT_MMx10,	3240, 4580}, //DMPAPER_ENV_C3   C3 Envelope、324 × 458 mm
	{DMPAPER_ENV_C4,	UNIT_MMx10,	2290, 3240}, //DMPAPER_ENV_C4   C4 Envelope、229 × 324 mm
	{DMPAPER_ENV_C6,	UNIT_MMx10,	1140, 1620}, //DMPAPER_ENV_C6   C6 Envelope、114 × 162 mm
	{DMPAPER_ENV_C65,	UNIT_MMx10,	1140, 2290}, //DMPAPER_ENV_C65   C65 Envelope、114 × 229 mm
	{DMPAPER_ENV_B4,	UNIT_MMx10,	2500, 3530}, //DMPAPER_ENV_B4   B4 Envelope、250 × 353 mm
	{DMPAPER_ENV_B5,	UNIT_MMx10,	1760, 2500}, //DMPAPER_ENV_B5   B5 Envelope、176 × 250 mm
	{DMPAPER_ENV_B6,	UNIT_MMx10,	1760, 1250}, //DMPAPER_ENV_B6   B6 Envelope、176 × 125 mm
	{DMPAPER_ENV_ITALY,	UNIT_MMx10,	1100, 2300}, //DMPAPER_ENV_ITALY   Italy Envelope、110 × 230 mm
	{DMPAPER_ENV_MONARCH,	UNIT_INCHx100,	386, 750}, //DMPAPER_ENV_MONARCH   Monarch Envelope、3 7/8 × 7 1/2 インチ
	{DMPAPER_ENV_PERSONAL,	UNIT_INCHx100,	363, 650}, //DMPAPER_ENV_PERSONAL   6 3/4 Envelope、3 5/8 × 6 1/2 インチ
	{DMPAPER_FANFOLD_US,	UNIT_INCHx100,	1486, 1100}, //DMPAPER_FANFOLD_US   US Std Fanfold、14 7/8 × 11 インチ
	{DMPAPER_FANFOLD_STD_GERMAN,	UNIT_INCHx100,	850, 1200}, //DMPAPER_FANFOLD_STD_GERMAN   German Std Fanfold、8 1/2 × 12 インチ
	{DMPAPER_FANFOLD_LGL_GERMAN,	UNIT_INCHx100,	850, 1300}, //DMPAPER_FANFOLD_LGL_GERMAN   German Legal Fanfold、8 1/2 × 13 インチ 
};

int MMx10_from_Inchx100(int inch) {
	return (int)(((unsigned long)inch) * 254 / 100);
}
bool ConvertMargin(CRect &rcDes_MM10, const CRect &rcSrc_MM10, const CSize &sizePaperSize_MM10, const short dmPaperSize, const short dmOrientation)
{
	int max = sizeof(svPaperSizes)/sizeof(PAPER_SIZE);
	for(const PAPER_SIZE *p=&svPaperSizes[0]; max--; p++) {
		if (p->dmPaperSize == dmPaperSize) {
#if	0
			int cx, cy;
			if (dmOrientation==DMORIENT_PORTRAIT) {
				cx = p->cx;
				cy = p->cy;
			}else{
				cx = p->cy;
				cy = p->cx;
			}
			if (p->unit==UNIT_INCHx100) {
				cx = MMx10_from_Inchx100(cx);
				cy = MMx10_from_Inchx100(cy);
			}
			MulDivRect(rcDes_M10, rcSrc_M10, sizePaperSize_MM10.cy, cy);
#endif
			int max_size, max_base;
			max_size = max(sizePaperSize_MM10.cx, sizePaperSize_MM10.cy);
			max_base = max(p->cx, p->cy);
			if (p->unit==UNIT_INCHx100) {
				//max_size = MMx10_from_Inchx100(max_size);
				max_base = MMx10_from_Inchx100(max_base);
			}
			MulDivRect(&rcDes_MM10, (const LPRECT)&rcSrc_MM10, max_size, max_base);
			return true;
		}
	}
	return false;
}

bool ConvertMargin(CRect &rc_MM10, const CSize &sizePaperSize_MM10, DEVMODE devmode)
{
	if ((devmode.dmFields & 3) == 3) {
		CRect rcDes_MM10;
		if (ConvertMargin(rcDes_MM10, rc_MM10, sizePaperSize_MM10, devmode.dmPaperSize, devmode.dmOrientation))
			rc_MM10 = rcDes_MM10;
		return true;
	}
	return false;
}
#endif //NOTUSE
#endif //#ifdef SPEC_PRINT
