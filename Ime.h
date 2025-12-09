#ifndef __IME_H__
#define __IME_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "imm.h"


#define IME_IN_COMPOSITION	1
#define IME_IN_CHOSECAND		2
//#define MAX_LISTCAND	32

#define CheckProperty		{				\
	if ( ( m_ImeUIData.fdwProperty & IME_PROP_SPECIAL_UI ) ||	\
	!( m_ImeUIData.fdwProperty & IME_PROP_AT_CARET ) )	\
	goto call_defwinproc;					\
}

typedef struct _IMEUIDATA {
	int		ImeState;   // Current Ime state.
	UINT	uCompLen;   // To save previous composition string length.
	DWORD	fdwProperty;
	//HWND	hListCand[ MAX_LISTCAND ];
	//HGLOBAL	hListCandMem[ MAX_LISTCAND ];
} IMEUIDATA;

enum IME_STRING_ATTR{
	isa_no,
	isa_dot,
	isa_invert,
	isa_line,
};
#define COMP_CUR_TAIL	(-1)
class CIme
{
public:
	CIme();
	~CIme();
public:
	bool WINAPI CIme::ImeWndProc(CWnd *pWnd, UINT msg, /*UINT*/WPARAM wParam, /*LONG*/LPARAM lParam );
	IME_STRING_ATTR GetImeStringAttr(int pos);
	bool IsInComposition() {
		return m_ImeUIData.ImeState & IME_IN_COMPOSITION; }
	void SetCandidatePos(CPoint &pt) {
		m_posCandidate = pt; }
	bool IsNotCompCursor() { //未確定文字がありながら、カーソルが無い状態？
		return IsInComposition() && !m_bCompCursor; } 
	bool IsCompCursor() { //未確定文字がありながら、カーソルが未確定文字内部にある状態？
		return IsInComposition() && (m_offCompCursor!=COMP_CUR_TAIL); } 
	CUR GetBlinkCursor() {
		return m_curComp;
	}

	void ImeOpenClose( HWND hWnd, BOOL fFlag );
	bool IsImeOpen( HWND hWnd);
	void SetCandidateFont(CWnd *pWnd);
private:
	void ImeCreate();
	void ResetCaret( HWND hWnd );
	void SetIMECompFormPos( HWND hWnd , CPoint point);
	//void ImeCommand( HWND hWnd, UINT cmd, LPARAM lParam );
	//BOOL IsDBCSTrailByte( char *base, char *p );
	void SetCaretPos( HWND hwnd );
	void InputChangeHandler( HWND hWnd );
	void SetCandidateWindow(HWND hWnd, bool bVert=false);

	void ImeUIStartComposition( HWND hwnd );
	//void ImeUIComposition( HWND hwnd, WPARAM wParam, LPARAM CompFlag, string &strIme, string &strImeAttr);
	void GetCompositionStr( HWND hwnd, LPARAM CompFlag, string &strIme, string &strImeAttr);
	void GetResultStr( HWND hwnd, string &strIme );

	void ImeUIEndComposition( HWND hwnd );
	//BOOL ImeUINotify( HWND hwnd, WPARAM wParam, LPARAM lParam );
	//void ImeUIOpenCandidate( HWND hwnd, LPARAM CandList );
	//void ImeUICloseCandidate( HWND hwnd, LPARAM CandList );
	//void ImeUIChangeCandidate( HWND hwnd, LPARAM CandList );
	//void ImeUISetOpenStatus( HWND hwnd );
	//void DisplayCompString( HWND hwnd, LPSTR lpStr, LPSTR lpStrAttr );
	//void DisplayResultString( HWND hwnd, LPSTR lpStr );
	//void RestoreImeUI( HWND hwnd );
	//void ImeUIMoveCandWin( HWND hwnd );
	void ImeUIClearData( HWND hwnd );
protected:
	IMEUIDATA	m_ImeUIData;			// IMEUI's global data.
	HKL			m_hCurKL;				// the current keyboad layout.
	string		m_strIme;
	string		m_strAttr;				//atribute of ime string.
	CPoint		m_posCandidate;
	bool		m_bCompCursor;
	int			m_offCompCursor;
	CUR			m_curComp;
	/*
	0=入力-点線
	1=変換中-反転
	2=変換中未選択-点線
	3=変換選択中選択-下線
	*/
};
extern	CIme	theIme;		//ime handling
#endif // __IME_H__

