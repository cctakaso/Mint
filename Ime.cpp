// Ime.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "sell.h"
#include "Ime.h"
#include "TheView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIme	theIme;		//ime handling
/////////////////////////////////////////////////////////////////////////////
// CIme
CIme::CIme()
{
	m_ImeUIData.ImeState = 0;
	m_posCandidate.x = 0;
	m_posCandidate.y = 0;
	m_bCompCursor = false;
	m_offCompCursor = COMP_CUR_TAIL;
}
CIme::~CIme()
{
}

IME_STRING_ATTR CIme::GetImeStringAttr(int pos)
{
	IME_STRING_ATTR isa=isa_dot;
	assert(pos>=0);
	if (pos>=m_strAttr.size()) {
		return isa;
	}
	assert(m_strAttr.size()>0);
	char attr = m_strAttr[pos];
	if (attr==1) {
		isa = isa_invert;
		m_bCompCursor = false;
	}else if (attr>=3){
		isa = isa_line;
		m_bCompCursor = false;
	}else //if (attr==0 || attr==2 || attr<0)
		isa = isa_dot;
	return isa;
}

void CIme::ImeOpenClose( HWND hWnd, BOOL fFlag )
{
	HIMC			hIMC;
	//
	// If fFlag is true then open IME; otherwise close it.
	//

	if ( !( hIMC = ImmGetContext( hWnd ) ) )
		return;
	ImmSetOpenStatus( hIMC, fFlag );
	ImmReleaseContext( hWnd, hIMC );
	m_offCompCursor = COMP_CUR_TAIL;
	m_strIme = "";
}

bool CIme::IsImeOpen( HWND hWnd)
{
	HIMC			hIMC;
	if ( !( hIMC = ImmGetContext( hWnd ) ) )
		return false;
	bool b = (ImmGetOpenStatus( hIMC ) == TRUE);
	ImmReleaseContext( hWnd, hIMC );
	return b;
}
//*********************************************************************
//
// WM_CREATE message handler
//
//*********************************************************************
void CIme::SetCandidateFont(CWnd *pWnd)
{
	HWND hWnd = pWnd->m_hWnd;
	//
	// Sets the logical font to be used to display characters in the 
	// composition window. Especially for at caret or near caret operation, 
	// application should set composition font.
	//
	// If Application provides user to dynamicly change font, each time after
	// user change font, application should set composition font again.
	//
	LOGFONT lf;
   ::GetObject(GetStockObject(OEM_FIXED_FONT), sizeof(LOGFONT), &lf);
	HIMC	hImc;
	//for IME VERT
	if (((CTheView*)pWnd)->m_pViewEd->IsVert()) {
		//lf = ((CTheView*)pWnd)->m_pViewEd->GetEnv()->m_lfVert;
		lf.lfFaceName[0] = '@';
		strncpy(lf.lfFaceName+1, ((CTheView*)pWnd)->m_pViewEd->GetEnv()->m_lfVert.lfFaceName, LF_FACESIZE-1 );
		lf.lfEscapement = 2700;
		lf.lfOrientation = 2700;
		//true type font only!
		//lf.lfEscapement = 900;
		//lf.lfOrientation = 900;
	//}else{
	//	lf = ((CTheView*)pWnd)->m_pViewEd->GetEnv()->m_lf;	
	}
	if ( (  hImc = ImmGetContext( hWnd ) ) ) {
		ImmSetCompositionFont( hImc, &lf );
		ImmReleaseContext( hWnd, hImc );
	}
}

void CIme::ImeCreate()
{

	//
	// Get the property and apiabilities of current keyboard layout(IME).
	// If the keyboard layout is US, the return value will be zero.
	//

	m_ImeUIData.fdwProperty = ImmGetProperty( GetKeyboardLayout(0),
						IGP_PROPERTY );

	//
	// Initialize candidate list window array.
	//

//	for(int i = 0; i < MAX_LISTCAND; i++ )
//	{
//		m_ImeUIData.hListCand[ i ] = NULL;
//		m_ImeUIData.hListCandMem[ i ] = NULL;
//	}


	//
	// Initialise the current keyboard layout.
	//
	m_hCurKL = GetKeyboardLayout(0L);
}


/************************************************************************
*
*   ResetCaret - Reset caret shape to match input mode (overtype/insert)
*
************************************************************************/

void CIme::ResetCaret( HWND hWnd )
{

	if ( !( m_ImeUIData.fdwProperty & IME_PROP_AT_CARET ) &&
		!( m_ImeUIData.fdwProperty & IME_PROP_SPECIAL_UI ) ) {
	// near caret.
		SetIMECompFormPos( hWnd , CPoint(0,0));
	}
}

//**********************************************************************
//
// SetIMECompFormPos()
//
//**********************************************************************

void CIme::SetIMECompFormPos( HWND hWnd , CPoint point)
{

	HIMC hIMC = ImmGetContext(hWnd);
	COMPOSITIONFORM CompForm;

	CompForm.dwStyle = CFS_POINT;

	CompForm.ptCurrentPos.x = (long) point.x;
	CompForm.ptCurrentPos.y = (long) point.y;

	if ( hIMC )
		ImmSetCompositionWindow(hIMC,&CompForm);
	ImmReleaseContext( hWnd , hIMC);
}


/************************************************************************
*
*   ImeCommand - WM_COMMAND handler
*
************************************************************************/

/*
void CIme::ImeCommand( HWND hWnd, UINT cmd, LPARAM lParam )
{
	switch( cmd )
	{
	case IDC_OPENIME:
		ImeOpenClose( hWnd, TRUE );
		goto RETURN;
	case IDC_CLOSEIME:
		ImeOpenClose( hWnd, FALSE );
		goto RETURN;
	}
	yPos = FIRSTROW;
	xPos = FIRSTCOL;
	InvalidateRect( hWnd, (LPRECT)NULL, TRUE );
	ResetCaret(hWnd);
RETURN:
	return;
}
*/

/************************************************************************
*
*   IsDBCSTrailByte - returns TRUE if the given byte is a DBCS trail byte
*
*					The algorithm searchs backward in the string, to some
*					known character boundary, counting consecutive bytes
*					in the lead byte range. An odd number indicates the
*					current byte is part of a two byte character code.
*
*   INPUT: PCHAR  - pointer to a preceding known character boundary.
*		 PCHAR  - pointer to the character to test.
*
*   OUTPUT:BOOL   - indicating truth of p==trailbyte.
*
************************************************************************/
/*
BOOL CIme::IsDBCSTrailByte( char *base, char *p )
{
	int lbc = 0;	// lead byte count

	assert(base <= p);

	while ( p > base ) {
	if ( !IsDBCSLeadByte(*(--p)) )
		break;
	lbc++;
	}
	return (lbc & 1);
}
*/

//**********************************************************************
//
// void SetCaretPos()
//
//**********************************************************************

void CIme::SetCaretPos( HWND hwnd )
{
	m_offCompCursor = COMP_CUR_TAIL;
	if (!IsInComposition() || !m_bCompCursor)
		return;

	HIMC		hIMC;
	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return ;
	
	m_offCompCursor = (ImmGetCompositionString( hIMC, GCS_CURSORPOS, (void FAR *)NULL, 0 ) & 0xffff);
	if (m_offCompCursor == m_strIme.size())
		m_offCompCursor = COMP_CUR_TAIL;

	ImmReleaseContext( hwnd, hIMC );
}



/************************************************************************
*
*   InputChangeHandler - WM_INPUTLANGCHANGE handler
*
************************************************************************/
void CIme::SetCandidateWindow(HWND hWnd, bool bVert)
{
	HIMC hIMC;
	if ( hIMC = ImmGetContext( hWnd ) ) {
		UINT i;
		CANDIDATEFORM CandForm;
		bool bUpdate=true;
//		if (bVert){
//			CandForm.ptCurrentPos = CPoint(m_posCandidate.x-90, m_posCandidate.y);
//			CandForm.rcArea = CRect(m_posCandidate.x-50, m_posCandidate.y,
//			m_posCandidate.x, m_posCandidate.y+1);
//		}else {
			CandForm.ptCurrentPos = m_posCandidate;
			CandForm.rcArea = CRect(m_posCandidate.x, m_posCandidate.y,
			m_posCandidate.x+1, m_posCandidate.y+1);
//		}
		for (i = 0; i < 4; i++) {
			if ( m_ImeUIData.fdwProperty & IME_PROP_AT_CARET ) {
				CandForm.dwIndex = i;
				CandForm.dwStyle = CFS_CANDIDATEPOS /*| CFS_EXCLUDE*/;
				// the position you want to set
				ImmSetCandidateWindow( hIMC, &CandForm );
			} else {
				if ( !ImmGetCandidateWindow( hIMC, i, &CandForm ) ) {
					bUpdate = false;
					continue;
				}

				if ( CandForm.dwStyle == CFS_DEFAULT ) {
					bUpdate = false;
					continue;
				}
				CandForm.dwStyle = CFS_DEFAULT;
				ImmSetCandidateWindow( hIMC, &CandForm );
			}
		}
		
		if (!bUpdate) {
			CandForm.dwIndex = 0;
			CandForm.dwStyle = CFS_CANDIDATEPOS;
			//if (bVert)
			//	CandForm.ptCurrentPos = CPoint(m_posCandidate.x-90, m_posCandidate.y);
			//else
				CandForm.ptCurrentPos = m_posCandidate;
			ImmSetCandidateWindow(hIMC, &CandForm);
		}

		ImmReleaseContext( hWnd, hIMC );
	}

}
void CIme::InputChangeHandler( HWND hWnd )
{
	// 
	// If the old keyboard layout is IME, the ime ui data have to be free.
	// 
	  
	if (ImmIsIME(m_hCurKL))
	{
		//
		// If application prefers to use near caret provded by IME, or
		// IME provides special UI, then no need to clean UD data.
		// 
		if ( m_ImeUIData.fdwProperty & IME_PROP_SPECIAL_UI )
			;
		else if ( m_ImeUIData.fdwProperty & IME_PROP_AT_CARET )
			ImeUIClearData(hWnd);
		else
			;
	}

	//
	// Set new keyboard layout.
	//
	m_hCurKL = GetKeyboardLayout(0L);

	//
	// Get new property.
	//

	m_ImeUIData.fdwProperty = ImmGetProperty( m_hCurKL, IGP_PROPERTY );

	// if this application set the candidate position, it need to set
	// it to default for the near caret IME
	SetCandidateWindow(hWnd);

	return;
}


/************************************************************************
*
*   ImeWndProc - Ime class window procedure
*
************************************************************************/

bool WINAPI CIme::ImeWndProc(CWnd *pWnd, UINT msg, /*UINT*/WPARAM wParam, /*LONG*/LPARAM lParam )
{
	HWND hWnd = pWnd->m_hWnd;
	static LPARAM lParam_save = 0;
	static string str_save;
	static bool bImeOpen_save = false;
	switch( msg ) {
	case WM_CREATE:
		ImeCreate();
		SetCandidateFont(pWnd);
		return false;
		break;
	case WM_SETFOCUS:
		if (bImeOpen_save) {
			bImeOpen_save = false;
			ImeOpenClose(hWnd, true);
		}
		return false;
		break;
	case WM_KILLFOCUS:
		if (bImeOpen_save = IsImeOpen(hWnd)) {
			ImeOpenClose(hWnd, false);//close IME!
		}
		return false;
		break;
//	case WM_MOVE:
//		ImeUIMoveCandWin( hWnd );
//		break;
//	case WM_COMMAND:
//		ImeCommand( hWnd, wParam, lParam );
//		break;
//	case WM_PAINT:
//		RestoreImeUI( hWnd );
//		break;
	case WM_INPUTLANGCHANGE:
		InputChangeHandler( hWnd );
		goto call_defwinproc;
		break;
	case WM_IME_SETCONTEXT:
		//
		// The application have to pass WM_IME_SETCONTEXT to DefWindowProc.
		// When the application want to handle the IME at the timing of
			// focus changing, the application should use WM_SETFOCUS or
			// WM_KILLFOCUS.
			//
		if ( m_ImeUIData.fdwProperty & IME_PROP_SPECIAL_UI )
			goto call_defwinproc;
		else if ( m_ImeUIData.fdwProperty & IME_PROP_AT_CARET ) {
		//
		// application wants to draw UI ny itself.
		//
			lParam &= ~(ISC_SHOWUICOMPOSITIONWINDOW /*| ISC_SHOWUIALLCANDIDATEWINDOW*/);
		}
		//return DefWindowProc( hWnd, msg, wParam, lParam );
		return false;
	case WM_IME_STARTCOMPOSITION:
		//
		// CheckProperty is a macro, if IME already provides near caret or
		// special UI then let IME handle this message.
		CheckProperty;
		ImeUIStartComposition( hWnd );
		break;
	case WM_IME_COMPOSITION:
		{
			CheckProperty;
			if ( lParam & GCS_RESULTSTR ) {
				m_bCompCursor = true;
				GetResultStr( hWnd, m_strIme);
				m_offCompCursor = COMP_CUR_TAIL;
				((CTheView*)pWnd)->OnImeString(m_strIme, m_strAttr, true);
			}
			if ( lParam & GCS_COMPSTR ) {
				string strIme_old = m_strIme;
				string strAttr_old = m_strAttr;
				m_bCompCursor = true;
				GetCompositionStr( hWnd, lParam, m_strIme, m_strAttr );
				//m_strIme.substr(0,str_save.size())
				if (m_strIme!=strIme_old || m_strAttr!=strAttr_old) {
					string str;
					str.append(m_strIme.data(), str_save.size());
					((CTheView*)pWnd)->OnImeString(m_strIme, m_strAttr, false,
						lParam!=lParam_save || 
						(m_strIme.size()>str_save.size() ? str!=str_save:true));
				}
				lParam_save = lParam;
				str_save = m_strIme;
#ifdef _DEBUG
				TRACE("\n strAttr[%d]", m_strAttr.size());
				for(int i=0; i<m_strAttr.size(); i++)
					TRACE(" %0X", m_strAttr[i]);
#endif
			}
			if ( lParam &  GCS_CURSORPOS) {
				bool bPrv = IsCompCursor();
				SetCaretPos(hWnd);
				if (bPrv || IsCompCursor()) {
					CViewEd *pViewEd = ((CTheView*)pWnd)->m_pViewEd;
					if (pViewEd) {
						if (IsCompCursor()) {
							CLineHint *pHint = pViewEd->GetHint();
							CUR cur = pViewEd->m_cursor.GetOrgCursor();
							ASSERT(m_offCompCursor>=0);
							cur.L.off += m_offCompCursor;
							OBJOFF row_hint = cur.D.y;
							pHint->SetCur_4Lgc(cur, row_hint);
							m_curComp = cur;
						}

						CCursor cursorOld = pViewEd->m_cursor;
						((CTheView*)pWnd)->ScrollForSell(cursorOld);
					}
				}
			}
			//for ATOK!
			//SetCandidateWindow(hWnd, ((CTheView*)pWnd)->m_pViewEd->IsVert());
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		{
		CheckProperty;
		m_strIme = "";
		ImeUIEndComposition( hWnd );
		((CTheView*)pWnd)->OnImeString(m_strIme, m_strAttr, true);
		m_strIme = "";
		}
		break;
	case WM_IME_COMPOSITIONFULL:
		//
		// Make sure the size for drawing the composition string.
		// Application should draw the composition string correctly.
		// 
		break;
	case WM_IME_NOTIFY:
		CheckProperty;
		if (wParam==IMN_OPENCANDIDATE)
			SetCandidateWindow(hWnd, ((CTheView*)pWnd)->m_pViewEd->IsVert());
		//if ( !ImeUINotify( hWnd, wParam, lParam ) )
		// This application does not handle all notification message.
		// So we pass those notification messages which are not hanlded
		// by this application to the DefWindowProc.
			goto call_defwinproc;
		break;
	case WM_IME_CONTROL:
		//
		// This message is not received by the application window.
		// But don't pass it to DefWindowProc().
		//
		break;
	default:
call_defwinproc:
		return false;
		//return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//static DWORD CompColor[ 4 ] = { RGB( 255, 0, 0 ),//入力-点線
//								RGB( 255, 0 , 255 ),//変換中-反転
//								RGB( 0, 0, 255 ),	//変換中未選択-点線
//								RGB( 0, 255, 0 ) };	//変換選択中選択-下線

//**********************************************************************
//
// void ImeUIStartComposition()
//
// This handles WM_IME_STARTCOMPOSITION message. 
//
//**********************************************************************

void CIme::ImeUIStartComposition( HWND hwnd )
{
		
	//
	// Change caption title to DBCS composition mode.
	//

//	SetWindowText( hwnd, (LPSTR)szImeCompTitle );	

	//
	// Reset global variables.
	// 

	m_ImeUIData.uCompLen = 0;			// length of composition string.

	m_ImeUIData.ImeState |= IME_IN_COMPOSITION;
}


//**********************************************************************
//
// void GetCompositionStr()
//
// This handles WM_IME_COMPOSITION message with GCS_COMPSTR flag on.
//
//**********************************************************************

void CIme::GetCompositionStr( HWND hwnd, LPARAM CompFlag, string &strIme, string &strImeAttr)
{
	DWORD	dwBufLen;			//Stogare for len. of composition str
	LPSTR	 lpCompStr;			 // Pointer to composition str.
	HIMC	  hIMC;				  // Input context handle.
	HLOCAL	hMem;				  // Memory handle.
	LPSTR	 lpCompStrAttr;		 // Pointer to composition str array.
	HLOCAL	hMemAttr;			  // Memory handle for comp. str. array.
	DWORD	 dwBufLenAttr;
		
	//
	// If fail to get input context handle then do nothing.
	// Applications should call ImmGetContext API to get
	// input context handle.
	//
	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return;
	//
	// Determines how much memory space to store the composition string.
	// Applications should call ImmGetCompositionString with
	// GCS_COMPSTR flag on, buffer length zero, to get the bullfer
	// length.
	//
	if ( ( dwBufLen = ImmGetCompositionString( hIMC, GCS_COMPSTR, 
										(void FAR*)NULL, 0l ) ) < 0 )
		goto exit2;

	//
	// Allocates memory with dwBufLen+1 bytes to store the composition
	// string. Here we allocale on more byte to put null character.
	//
	if ( !( hMem = LocalAlloc( LPTR, (int)dwBufLen + 1 ) ) )
		goto exit2;

	if ( !( lpCompStr = (LPSTR) LocalLock( hMem ) ) )
		goto exit1;

	//
	// Reads in the composition string.
	//

	ImmGetCompositionString( hIMC, GCS_COMPSTR, lpCompStr, dwBufLen );

	//
	// Null terminated.
	//

	lpCompStr[ dwBufLen ] = 0;

	//
	// If GCS_COMPATTR flag is on, then we need to take care of it.
	//

	if ( CompFlag & GCS_COMPATTR ) {

		if ( ( dwBufLenAttr = ImmGetCompositionString( hIMC, GCS_COMPATTR,
							  ( void FAR *)NULL, 0l ) ) < 0 )
			goto nothing;

		//
		// Allocate memory to store attributes of composition strings.
		//

		if ( !( hMemAttr = LocalAlloc( LPTR, (int)dwBufLenAttr + 1 ) ) )
			goto nothing;

		if ( !( lpCompStrAttr = (LPSTR) LocalLock( hMemAttr ) ) ) {
			LocalFree( hMemAttr );
			goto nothing;
		}

		//
		// Reads in the attribute array.
		//

		ImmGetCompositionString( hIMC, GCS_COMPATTR, lpCompStrAttr,
								 dwBufLenAttr );

		lpCompStrAttr[ dwBufLenAttr ] = 0;

	} else {

nothing:
		lpCompStrAttr = NULL;
	}


	//
	// Display new composition chars.
	//
	//DisplayCompString( hwnd, lpCompStr, lpCompStrAttr );
	strIme = lpCompStr;
	strImeAttr = lpCompStrAttr;

	//
	// Keep the length of the composition string for using later.
	//

	m_ImeUIData.uCompLen = (UINT)dwBufLen;

	LocalUnlock( hMem );

	if ( lpCompStrAttr ) {
		LocalUnlock( hMemAttr );
		LocalFree( hMemAttr );
	}

exit1:

	LocalFree( hMem );

exit2:

	ImmReleaseContext( hwnd, hIMC );

}


//***********************************************************************
//
// void GetResultStr()
//
// This handles WM_IME_COMPOSITION with GCS_RESULTSTR flag on.
//
//***********************************************************************

void CIme::GetResultStr( HWND hwnd, string &strIme)
{
	DWORD	  dwBufLen;			  // Storage for length of result str.
	LPSTR	  lpResultStr;			// Pointer to result string.
	HIMC		hIMC;				  // Input context handle.
	HLOCAL	 hMem;				  // Memory handle.

	//
	// If fail to get input context handle then do nothing.
	//

	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return;

	//
	// Determines how much memory space to store the result string.
	// Applications should call ImmGetCompositionString with
	// GCS_RESULTSTR flag on, buffer length zero, to get the bullfer
	// length.
	//

	if ( ( dwBufLen = ImmGetCompositionString( hIMC, GCS_RESULTSTR,
								 (void FAR *)NULL, (DWORD) 0 ) ) <= 0 )
		goto exit2;

	//
	// Allocates memory with dwBufLen+1 bytes to store the result
	// string. Here we allocale on more byte to put null character.
	//

	if ( !( hMem = LocalAlloc( LPTR, (int)dwBufLen + 1 ) ) )
		goto exit2;

	if ( !( lpResultStr = (LPSTR) LocalLock( hMem ) ) )
		goto exit1;

	//
	// Reads in the result string.
	//

	ImmGetCompositionString( hIMC, GCS_RESULTSTR, lpResultStr, dwBufLen );

	//
	// Displays the result string.
	//
	//DisplayResultString( hwnd, lpResultStr );
	strIme = lpResultStr;

	LocalUnlock( hMem );

exit1:
	
	LocalFree( hMem );

exit2:

	ImmReleaseContext( hwnd, hIMC );

}


//**********************************************************************
//
// void ImeUIEndComposition
//
// This handles WM_IME_ENDCOMPOSITION message.
//
//**********************************************************************

void CIme::ImeUIEndComposition( HWND hwnd )
{
		
//	RECT	  rect;

	//
	// Change caption title to normal
	//

//	SetWindowText( hwnd, (LPSTR)szImeTitle );

	//
	// Update client area.
	//

//	GetClientRect( hwnd, (LPRECT)&rect );

//	InvalidateRect( hwnd, (LPRECT)&rect, FALSE );

	//
	// Reset the length of composition string to zero.
	//

	m_ImeUIData.uCompLen = 0;

	m_ImeUIData.ImeState &= ~IME_IN_COMPOSITION;

}
//**********************************************************************
//
// void ImeUIClearData()
//
// Handler routine of WM_IME_SELECT message.
//
//**********************************************************************

void CIme::ImeUIClearData( HWND hwnd )
{

	RECT			rect;		  
//	int			i;
   
//	SetWindowText( hwnd, (LPSTR)szImeTitle );

	//
	// If user switches to other IME, here we destroy all candidate
	// windows which has been opened and erase all composition
	// chars if any.
	//
/*
	for( i = 0; i < MAX_LISTCAND; i++ )
	{
		if ( m_ImeUIData.hListCand[ i ] )
		{
			//
			// The i-th candidate list has already been displayed,
			// destroy it and free memory which stores candidate
			// strings.
			//

			DestroyWindow( m_ImeUIData.hListCand[ i] );
			GlobalFree( m_ImeUIData.hListCandMem[ i ] );

			m_ImeUIData.hListCand[ i ] = null;
			m_ImeUIData.hListCandMem[ i ] = null;

		}
	}
*/
	//
	// Update client area.
	//

	GetClientRect( hwnd, (LPRECT)&rect );

	InvalidateRect( hwnd, (LPRECT)&rect, FALSE );


	//
	// Reset IMEUI's global data.
	//

	m_ImeUIData.uCompLen = m_ImeUIData.ImeState = 0;

	//
	// Reset caret to the original position.
	//

//	HideCaret( hwnd );
//	ResetCaret( hwnd );
//	ShowCaret( hwnd );

}


//**********************************************************************
//
// BOOL ImeUINotify()
//
// This handles WM_IME_NOTIFY message.
//
//**********************************************************************
/*
BOOL CIme::ImeUINotify( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	switch (wParam )
	{
		case IMN_OPENCANDIDATE:
			ImeUIOpenCandidate( hwnd, lParam );
			break;

		case IMN_CLOSECANDIDATE:

			ImeUICloseCandidate( hwnd, lParam );
			break;

		case IMN_CHANGECANDIDATE:

			ImeUIChangeCandidate( hwnd, lParam );
			break;

		case IMN_SETOPENSTATUS:

			ImeUISetOpenStatus( hwnd );
			break;

	default:
		return FALSE;

	}
	
	return TRUE;
}
*/
//**********************************************************************
//
// void ImeUIOpenCandidate()
//
// This handles WM_IME_NOTIFY message with wParam = IMN_OPENCANDIDATE.
//
//**********************************************************************
/*
void CIme::ImeUIOpenCandidate( HWND hwnd, LPARAM CandList )
{
	HIMC		hIMC;				  // Input context handle.
	LPCANDIDATELIST lpCandList;		// Storage for LP to candidate list.
	DWORD	  dwBufLen;			  // Storage for candidate strings.
	LPSTR	  lpStr;				 // Storage for LP to a string.
	DWORD	  dwIndex;				// Storage for index of ListCand array
	DWORD	  i;					 // Loop count.
	int		width = 0;			 // Storage for width of listCand
	int		CurNumCandList = 0;	// Storage for number of cand. lists.
	DWORD	  dwPreferNumPerPage;	// Storage for PreferNumPerPage
	POINT	  point;				 // Storage for caret position.

	//
	// If fail to get input context handle, do nothing.
	//

	if ( ! (hIMC = ImmGetContext( hwnd ) ) )
		return;

	//
	// Change caption title to DBCS candidate mode.
	//

//	SetWindowText( hwnd, (LPSTR)szImeCandTitle );

	//
	// Find out how many candidate windows have already been opened.
	//

	for( i = 0; i < MAX_LISTCAND; i++ )
	{
		if ( m_ImeUIData.hListCand[ i ] )
			CurNumCandList++;
	}

	//
	// Check which candidate lists should be displayed by loopping
	// through all possible candidate lists.
	//

	for( dwIndex = 0; dwIndex < MAX_LISTCAND ; dwIndex ++ )
	{

		if ( CandList & ( 1 << dwIndex ) )
		{   
			//
			// The dwIndex-th candidate list contains candidate strings.
			// So here we want to display them.
			//

			//
			// Determines how musch memory space should be allocated to
			// read in the corresponding candidate list .
			//

			if ( ! ( dwBufLen = ImmGetCandidateList( hIMC, dwIndex, 
					  lpCandList, 0 ) ) )					 
				goto exit2;
					  
			
			//
			// Allocate memory space.
			//

			if( !( m_ImeUIData.hListCandMem[ dwIndex ]  = 
				  GlobalAlloc( LPTR, (int)dwBufLen ) ) )
				goto exit2;

			if( !( lpStr =
				(LPSTR)GlobalLock( m_ImeUIData.hListCandMem[ dwIndex ] ) ) )
			{   
				GlobalFree( m_ImeUIData.hListCandMem[ dwIndex ] );
				m_ImeUIData.hListCandMem[ dwIndex ] = NULL;
				goto exit2;
			}
					
			lpCandList = (LPCANDIDATELIST) lpStr;

			//
			// Reads in the corresponding candidate list.
			//

			ImmGetCandidateList( hIMC, dwIndex, lpCandList, dwBufLen );

			//
			// Get current caret position.
			//

			GetCaretPos( (POINT FAR*)&point );
			ClientToScreen( hwnd, (LPPOINT)&point );

			//
			// Determines how many candidate strings per page.
			//

			dwPreferNumPerPage = ( !lpCandList->dwPageSize ) ?
								DEFAULT_CAND_NUM_PER_PAGE :
								lpCandList->dwPageSize;
			//
			// Determining maximum character length the list box
			// will display by loopping through all candidate strings.
			//

			for( i = 0; i < lpCandList->dwCount; i++ )
			{
				//
				// Get the pointer to i-th candidate string.
				//

				lpStr = (LPSTR)lpCandList + 
						lpCandList->dwOffset[ i ];

				width = ( width < lstrlen( lpStr ) ) ? lstrlen( lpStr ) :
						 width;

			}

			//
			// Create a candidate window for the candidate list.
			//


			m_ImeUIData.hListCand[ dwIndex ] = CreateWindow(		 
								(LPCTSTR)szCandClass,
								NULL,
								WS_BORDER | WS_POPUP | WS_DISABLED,
								CurNumCandList * X_INDENT + point.x,
								CurNumCandList * Y_INDENT + 
								point.y + cyMetrics,
								( width ) * cxMetrics + 10,
								(int)(dwPreferNumPerPage) * cyMetrics + 5,,
								hwnd,
								(HMENU)NULL,
								(HANDLE)GetWindowLong( hwnd, GWL_HINSTANCE ),
								(LPVOID)NULL
								);
			//
			// If fail to create the candidate window then do nothing.
			//

			if ( m_ImeUIData.hListCand[ dwIndex ] < 0 )
			{
				GlobalUnlock( m_ImeUIData.hListCandMem[ dwIndex ] );
				GlobalFree( m_ImeUIData.hListCandMem[ dwIndex ] );
				goto exit2;
			}

 
			//
			// Show candidate window.
			//

			ShowWindow( m_ImeUIData.hListCand[ dwIndex ], SW_SHOWNOACTIVATE );

			//
			// Display candidate strings.
			//

			DisplayCandStrings( m_ImeUIData.hListCand[ dwIndex ], lpCandList );

			GlobalUnlock( m_ImeUIData.hListCandMem[ dwIndex ] );

			CurNumCandList++;
		}
	}

	//
	// Reset IME state.
	//

	m_ImeUIData.ImeState |= IME_IN_CHOSECAND;

exit2:

	ImmReleaseContext( hwnd, hIMC );

}
*/
//**********************************************************************
//
// void ImeUICloseCandidate()
//
// This handles WM_IME_NOTIFY message with
// wParam = IMN_CLOSECANDIDATE.
//
//**********************************************************************
/*
void CIme::ImeUICloseCandidate( HWND hwnd, LPARAM CandList )
{
	int		index;

	//
	// Change window's caption title to normal.
	//

	SetWindowText( hwnd, (LPSTR)szImeCompTitle );

	//
	// If the i-th bit on of CandList that means the i-th
	// candidate list should be closed.
	//

	for( index = 0; index < MAX_LISTCAND; index ++ )
	{
		if (( CandList & ( 1 << index ) ) && m_ImeUIData.hListCand[ index ])
		{

			//
			// Destroy the candidate window.
			//

			DestroyWindow( m_ImeUIData.hListCand[ index ] );

			m_ImeUIData.hListCand[ index ] = NULL;

			//
			// Free memory.
			//

			GlobalFree( m_ImeUIData.hListCandMem[ index ] );

			m_ImeUIData.hListCandMem[ index ] = NULL;

		}
	}

	m_ImeUIData.ImeState &= ~IME_IN_CHOSECAND;

}
*/
//**********************************************************************
//
// void ImeUIChangeCandidate()
//
// This handles WM_IME_NOTIFY message with wParam = IMN_CHANGECANDIDATE.
//
//**********************************************************************
/*
void CIme::ImeUIChangeCandidate( HWND hwnd, LPARAM CandList )
{
	HIMC			hIMC;
	LPCANDIDATELIST lpCandList = NULL;		
	DWORD		  dwIndex;
	DWORD 		dwBufLen;
	LPSTR		lpStr;
	DWORD		i;
	RECT		rect;
	int			width = 0;
	DWORD		dwPreferNumPerPage;


	//
	// If fail to get input context, do nothing.
	//

	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return;

	//
	// Determine which candidate list should be updated.
	//

	for ( dwIndex = 0; dwIndex < MAX_LISTCAND; dwIndex++ )
		if ( CandList & ( 1 << dwIndex ) )
			break;

	//
	// If dwIndex == MAX_LISTCAND, then something wrong, do nothing.
	//

	if ( dwIndex == MAX_LISTCAND )
		return;

	//
	// Determines how much memory space should be allocated to read in the
	// corresponding candidate list.
	//

	if ( !( dwBufLen = ImmGetCandidateList( hIMC, dwIndex, lpCandList, 0 ) ) )
	goto exit2;

	//
	// Relocate memory space.
	//

	if ( !( m_ImeUIData.hListCandMem[ dwIndex ] = GlobalReAlloc(
		m_ImeUIData.hListCandMem[ dwIndex ], (int)dwBufLen, LPTR ) ) )
	goto exit2;

	if ( !( lpStr = 
	(LPSTR)GlobalLock( m_ImeUIData.hListCandMem[ dwIndex ] ) ) )
	{
	GlobalFree( m_ImeUIData.hListCandMem[ dwIndex ] );
		m_ImeUIData.hListCandMem[ dwIndex ] = NULL;
	goto exit2;
	}

	lpCandList = (LPCANDIDATELIST) lpStr;

	//
	// Reads in the corresponding candidate list.
	//

	ImmGetCandidateList( hIMC, dwIndex, lpCandList, dwBufLen );

	//
	// Determines how many candidate strings per page.
	//

	dwPreferNumPerPage = ( !lpCandList->dwPageSize ) ?
						DEFAULT_CAND_NUM_PER_PAGE :
						lpCandList->dwPageSize;
	// 
	// Determining maximum character length  the list box
	// will display by loopping through all candidate strings.
	//

	for( i = 0; i < lpCandList->dwCount; i++ )
	{
		//
		// Get the pointer to i-th candidate string.
		//

		lpStr = (LPSTR)lpCandList + 
				lpCandList->dwOffset[ i ];

		width = ( width < lstrlen( lpStr ) ) ? lstrlen( lpStr ) :
				 width;

	}

	GetWindowRect( m_ImeUIData.hListCand[ dwIndex ] , (LPRECT) &rect);

	SetWindowPos( m_ImeUIData.hListCand[ dwIndex ],
		 hwnd,
		 rect.left,
		 rect.top,
		 ( width ) * cxMetrics + 10,
		 (int)(dwPreferNumPerPage) * cyMetrics + 5,
		 SWP_NOZORDER | SWP_NOACTIVATE );
		 

	DisplayCandStrings( m_ImeUIData.hListCand[ dwIndex ], lpCandList );

	GlobalUnlock( m_ImeUIData.hListCandMem[ dwIndex ] );

exit2:

	return;

}
*/

//***********************************************************************
//
// void ImeUISetOpenStatus()
//
// This handles WM_IME_REPORT message with wParam = IR_NOTIFY &
// lParam = IMC_SETOPENSTATUS.
//
//**********************************************************************
/*
void CIme::ImeUISetOpenStatus( HWND hwnd )
{
	int		i;	  // Lopp counter
	HIMC		hIMC;	// Storage for input context handle.

	//
	// If fail to get input context handle then do nothing
	//

	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return;

	if ( ImmGetOpenStatus( hIMC ) )
	{

		//
		// If the IME conversion engine is open, here we change
		// window's caption title to DBCS composition mode.
		// 

		SetWindowText( hwnd, (LPSTR)szImeCompTitle );

	} else
	{
		RECT	rect;		
		
		//
		// If the IME conversion engine is closed, here we
		// erase all already displayed composition chars if any,
		// change the window's caption title to normal.
		//

		GetClientRect( hwnd, (LPRECT)&rect );
		InvalidateRect( hwnd, (LPRECT)&rect, FALSE );

		SetWindowText( hwnd, (LPSTR)szImeTitle );

		//
		// Here we close and destroy all of candidate windows
		// if IME conversion engine is closed.
		//

		for( i = 0; i <= MAX_LISTCAND; i++ )
		{
			if ( m_ImeUIData.hListCand[ i ] ) 
			{
				DestroyWindow( m_ImeUIData.hListCand[ i ] );
				m_ImeUIData.hListCand[ i ] = NULL;
				GlobalFree( m_ImeUIData.hListCandMem[ i ] );
				m_ImeUIData.hListCandMem[ i ] = NULL;
			}
		}

		//
		// Reset IMEUI's global data. 
		//

		m_ImeUIData.uCompLen = 0;
		m_ImeUIData.ImeState = 0;

		//ResetCaret( hwnd );

	}

	ImmReleaseContext( hwnd, hIMC );
}
*/
//*********************************************************************
//
// void DisplayCompString()
//
// This displays composition string.
//
// This function supports only fixed pitch font.
//
//*********************************************************************
/*
void CIme::DisplayCompString( HWND hwnd, LPSTR lpStr, LPSTR lpStrAttr )
{
	HDC		hdc;							
	int		StrLen = lstrlen( lpStr );
	RECT		rect;
	DWORD	  dwColor;
	int		i;
	BOOL		fDBCSTrailByte = FALSE;


	hdc = GetDC( hwnd );

//	HideCaret( hwnd );

	//
	// Determine OPAQUE rect.
	//

	rect.left = xPos * cxMetrics;
	rect.top  = yPos * cyMetrics;
	rect.bottom = rect.top + cyMetrics;
	rect.right = ( (int)m_ImeUIData.uCompLen > StrLen ) ?
				( xPos + m_ImeUIData.uCompLen ) * cxMetrics:
				( xPos + StrLen ) * cxMetrics;
   
	//
	// This example we use red to display composition chars
	// with attribute 000, pink for attribute 001,
	// blue for attribute 010, green for attribute 011.
	//

	//
	// Each composition character has different attribute.
	// We here use different kinds of color to represent attributes.
	// Red, pink, blue and green are for attribute 000, 001, 010 and 011,
	// respectively.
	//
  

	dwColor = GetTextColor( hdc );

	if ( ( StrLen % 2 ) )
	{
		if ( IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 1 ] ) )
		{
			textbuf[ yPos ][ xPos + StrLen ] = ' ';
			fDBCSTrailByte = TRUE;
		}
	} else
	{
		if ( !IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 2 ] ) )
		{
			if ( IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 1 ] ) )
		{
				textbuf[ yPos ][ xPos + StrLen ] = ' ';
				fDBCSTrailByte = TRUE;
			}
		}
	}

	if ( !lpStrAttr )
	{
		//
		// If there are not attribute array, here we use default color, RED,
		// to display all of composition characters.
		//

		SetTextColor( hdc, CompColor[ 0 ] ); // default color

		ExtTextOut( hdc, xPos * cxMetrics, yPos * cyMetrics,
				ETO_OPAQUE, &rect, lpStr, StrLen, 0 );

	} else
	{

		int  ColorIndex;

		ExtTextOut( hdc, xPos * cxMetrics, yPos * cyMetrics, ETO_OPAQUE,
					&rect, NULL, 0, 0 );

		for( i = 0; *lpStr;)
		{
			int cnt = IsDBCSLeadByte(*lpStr) ? 2 : 1;

			ColorIndex = ( ((int)*lpStrAttr) < 0 ) ? 0 : (int)*lpStrAttr;
			ColorIndex = ( ColorIndex > 3 ) ? 3 : ColorIndex;

			SetTextColor( hdc, CompColor[ ColorIndex ] );
			
			TextOut( hdc, ( i + xPos ) * cxMetrics, yPos * cyMetrics,
					lpStr, cnt );
			lpStr += cnt;
			lpStrAttr += cnt;
			i += cnt;
		}
	}


	SetTextColor( hdc, dwColor );

	if ( fDBCSTrailByte )
	{
		TextOut( hdc, ( xPos + StrLen ) * cxMetrics, yPos * cyMetrics,
			" ", 1 );
	}

//	SetCaretPos( ( xPos + StrLen ) * cxMetrics, yPos * cyMetrics );

//	ShowCaret( hwnd );

	ReleaseDC( hwnd, hdc );

}
*/
//*********************************************************************
//
// void DisplayResultString()
//
// This displays result string.
//
// This function supports only fixed pitch font.
//
//*********************************************************************
/*
void CIme::DisplayResultString( HWND hwnd, LPSTR lpStr )
{

	int		StrLen;		// Storage for string length.
	int		i;			 // Loop counter.
	HDC		hdc;			// Display context handle.

	StrLen = lstrlen( lpStr );

	//
	// If there is no room for compsoition string, discard it
	//

	if ( xPos == ( LASTCOL - StrLen - 1 ) )
		return;

	//
	// if insert mode or during composition session,
	// move rest of line to the right by StrLen bytes.
	//

	if ( fInsertMode )
	{

		for( i = LASTCOL; i > xPos; i-- )
			textbuf[ yPos ][ i ] = textbuf[ yPos ][ i - StrLen ];

		//
		// If the row ends on a lead byte, blank it out,
		// To do this we must first traverse the string starting
		// from a know character boundry until we reach the last column.
		// If the last column is a character boundry then the last 
		// character is either a string byte or a lead byte.
		//

		for( i = xPos + StrLen; i < LASTCOL; )
		{
			if ( IsDBCSLeadByte( textbuf[ yPos ][ i ] ) )
				i++;
			i++;
		}

		if ( i == LASTCOL && IsDBCSLeadByte( textbuf[ yPos ][ i ] ) )
			textbuf[ yPos ][ LASTCOL ] = ' ';

	} else
	{
		//
		// overtype mode
		//

		if ( ( StrLen % 2 ) )
		{
			if ( IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 1 ] ) )
				textbuf[ yPos ][ xPos + StrLen ] = ' ';
		} else
		{
			if ( !IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 2 ] ) )
			{
				//
				// Overtyping the current byte, plus the following byte
				// which could be a lead byte.
				//

				if ( IsDBCSLeadByte( textbuf[ yPos ][ xPos + StrLen - 1 ] ) )
					textbuf[ yPos ][ xPos + StrLen ] = ' ';
			}
		}
	}

	//
	// Store input character at current caret position.
	//

	for( i = 0; i <= LASTCOL && *lpStr; i++ )
		textbuf[ yPos ][ xPos + i ] = *lpStr++;

	//
	// Display input character
	//


	hdc = GetDC( hwnd );

//	HideCaret( hwnd );

	//
	// Displays result string with normal color.
	//

	TextOut( hdc, xPos *cxMetrics, yPos * cyMetrics,
			&( textbuf[ yPos][xPos] ), MAXCOL - xPos );


//	ShowCaret( hwnd );

	ReleaseDC( hwnd, hdc );

	//
	// Reset Caret position
	//

	xPos += StrLen;

	if ( xPos > LASTCOL )
		xPos = LASTCOL;

//	ResetCaret( hwnd );

	m_ImeUIData.uCompLen = 0;

}
*/
//**********************************************************************
//
// void RestoreImeUI()
//
// This repaints all displayed composition string if need.
// Main window procedure will call this upon receiving
// WM_PAINT message.
//
//**********************************************************************
/*
void CIme::RestoreImeUI( HWND hwnd )
{
	HIMC		hIMC;		  // Storage for input context handle.
	DWORD	  dwBufLen;	  // 

	
	//
	// If fail to get input context handle then do nothing.
	//

	if ( !( hIMC = ImmGetContext( hwnd ) ) )
		return;

	//
	// If IME conversion engine is open and there are any composition
	// string in the context then we redisplay them.
	//

	if ( ImmGetOpenStatus( hIMC ) && m_ImeUIData.ImeState &&
		( dwBufLen = ImmGetCompositionString( hIMC, GCS_COMPSTR,
					 (void FAR*)NULL, 0l ) ) > 0 )
	{
		LPSTR	  lpCompStr;	// Pointer to composition string
		HLOCAL	 hMem;		 // Storage for memory handle.
		LPSTR	  lpCompStrAttr; // Pointer to composition string's attribute
		DWORD	  dwBufLenAttr;  // 
		HLOCAL	 hMemAttr;	 // Memory handle for composition string's
								  // attributes.

		//
		// If fail to allocate and lock memory space for reading in
		// the composition string then do nothing.
		//

		if ( !( hMem = LocalAlloc( LPTR, (int)dwBufLen + 1 ) ) )
			goto exit2;

		if( !( lpCompStr = (LPSTR) LocalLock( hMem ) ) )
		{
			LocalFree( hMem );
			goto exit2;
		}

		//
		// Get composition string and redisplay them.
		//

		if ( ImmGetCompositionString( hIMC, GCS_COMPSTR, lpCompStr,
									 dwBufLen ) > 0 )
		{

			//
			// MAke sure whether we need to handle composition string's
			// attributes.
			//

			if ( ( dwBufLenAttr = ( ImmGetCompositionString( hIMC,
					  GCS_COMPATTR, (void FAR*)NULL, 0l ) ) ) > 0 )
			{
				//
				// If fail to allocate and lock memory space for reading in
				// the composition string's attribute then we assume
				// no attribute array.
				//

				if ( !( hMemAttr = LocalAlloc(LPTR, (int)dwBufLenAttr + 1 )))
					goto nothing;

				if ( !( lpCompStrAttr = (LPSTR) LocalLock( hMemAttr ) ) )
				{
					LocalFree( hMemAttr );
					goto nothing;
				}

				ImmGetCompositionString( hIMC, GCS_COMPATTR, lpCompStrAttr,
										dwBufLenAttr );

				lpCompStrAttr[ dwBufLenAttr ] = 0;
			} else
			{
nothing:
				lpCompStrAttr = NULL;
			}


			lpCompStr[ dwBufLen ] = 0;

			DisplayCompString( hwnd, lpCompStr, lpCompStrAttr );
	
		}

		LocalUnlock( hMem );
		LocalFree( hMem );
		
		if ( lpCompStrAttr )
		{
			LocalUnlock( hMemAttr );
			LocalFree( hMemAttr );
		}


	}

exit2:

	ImmReleaseContext( hwnd, hIMC );

}
*/
//**********************************************************************
//
// void ImeUIMove()
//
// Handler routine of WM_MOVE message.
//
//*********************************************************************
/*
void CIme::ImeUIMoveCandWin( HWND hwnd )
{

	if ( m_ImeUIData.ImeState & IME_IN_CHOSECAND )
	{
		POINT		  point;		 // Storage for caret position.
		int			i;			 // loop counter.
		int			NumCandWin;	// Storage for num of cand win.
		RECT			rect;		  // Storage for client rect.

		//
		// If current IME state is in chosing candidate, here we
		// move all candidate windows, if any, to the appropriate
		// position based on the parent window's position.
		//

		NumCandWin = 0;

		GetCaretPos( (LPPOINT)&point );
		ClientToScreen( hwnd, (LPPOINT)&point );

		for ( i = 0; i < MAX_LISTCAND ; i++ )
		{
			if ( m_ImeUIData.hListCand[ i ] )
			{
				GetClientRect( m_ImeUIData.hListCand[ i ], &rect );				
				
				MoveWindow( m_ImeUIData.hListCand[ i ], 
							point.x + X_INDENT * NumCandWin,
							point.y + Y_INDENT * NumCandWin + cyMetrics,
							( rect.right - rect.left + 1 ),
							( rect.bottom - rect.top + 1 ), TRUE );

				NumCandWin++;
			}
		}
	}
}
*/
