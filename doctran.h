/***********************************************************************************
-- Class Name: C F i l e S c a n A r c  c l a s s
-- Description and Usage:
		アーカイブファイルを管理するクラス
-- Notes:---------------------------------------------------------------------------
	DLLの管理と、与えられたFIND_DATAをもとに圧縮ファイル内の
	検索および、指定ファイルの解凍を行う。
-- History:
		'95.11	- created		(M.Design)
**************************************************************************************/

#ifndef __DOCTRAN_H__
#define __DOCTRAN_H__

#ifdef takaso2010
#endif /* takaso2010 */

/////////////////////////////////////////////////////////////////////////////

/*************************************************************************
	Extension type define
*************************************************************************/
enum EXT_TYPE
{
	EXT_NON=-1,
	EXT_MIN=0,
	EXT_KFT=0,
	EXT_MAX
};

class CDLL
{
public:
	CDLL() {
		m_ext =EXT_NON;
		m_hDll = NULL; }
	virtual ~CDLL() {
		FreeLib();
		};
	//ＤＬＬのﾁｪｯｸ等
	virtual bool FInitialize(EXT_TYPE at=EXT_NON,  bool fActive=TRUE);

	bool FActive() {
		return m_hDll!=NULL;}

	virtual bool FActiveUpdate();

	//Ａｒｃファイルをセットする
	virtual bool FSetSrcFile(const char *szArcFile);

//	string *PGetSrcFile() {
//		return &m_dFileSrc; }

	// 指定ファイルを展開する
	virtual int DoTrans(const char *szSrcFilename, const char *szDesFilename,
		const char *szSrcCode, const char *szDesCode)=0;

	EXT_TYPE Getext() {
		return m_ext; }


	HINSTANCE GetDll() {
		return m_hDll; }
protected:
	EXT_TYPE m_ext;					// 種別
	HINSTANCE m_hDll;			// ＤＬＬのハンドル
//	string m_dFileSrc;				// アーカイブファイル
	void FreeLib();
};

/*
typedef	WORD (WINAPI* DGetVersion)(VOID);
typedef	bool (WINAPI* DSetBackGroundMode)(const bool BackGroundMode);
typedef	bool (WINAPI* DSetCursorMode)(const bool CursorMode);
typedef	int (WINAPI* DCommand)(const HWND _hwnd, LPCSTR szCmdLine, LPSTR szOutput, const DWORD wSize);
//typedef	bool (WINAPI* DCheckArchive)(LPCSTR _szFileName, const int _iMode);
typedef	HANDLE (WINAPI* DOpenArchive)(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode);
typedef	int (WINAPI* DCloseArchive)(HANDLE _harc);
//typedef	int (WINAPI* DFindFirst)(HARC _harc, LPCSTR _szWildName, INDIVIDUALINFO *_lpSubInfo);
//typedef	int (WINAPI* DFindNext)(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
typedef bool (WINAPI *DSetOwnerWindow)(const HWND _hwnd);
typedef bool (WINAPI *DClearOwnerWindow)(VOID);
typedef bool (WINAPI *DGetRunning)(VOID);
*/

#define WM_ARCEXTRACT_TXT	"wm_arcextract"

extern UINT WM_ARCEXTRACT_MSG;

#include "..\Unkanj\unkanj.h"



class CDLLDoc : public CDLL
{
#define UNKANJI_SUB_DIR	("UNKANJI")
public:
	CDLLDoc();
	virtual ~CDLLDoc();

	//ＤＬＬのﾁｪｯｸ等
	virtual bool FInitialize(EXT_TYPE at=EXT_NON, bool fActive=TRUE);

	// 指定ファイルを展開する
	int DoTrans(const char *szSrcFilename, const char *szDesFilename,
		const char *szSrcCode, const char *szDesCode);
	bool DoOpenTran(const char *szSrcFile);
	unsigned int DoReadTran(void* lpBuf, UINT nCount, int *pRtn=NULL);
	void DoCloseTran();
	string DoGetType();
	bool DoIsEOFTran();
protected:
	HANDLE m_handle;
	DGetVersion GetVersion;
	DCommand ArcCmd;
	DOpen OpenTran;
	DRead ReadTran;
	DClose CloseTran;

	DSetBackGroundMode SetBackGroundMode;
	DSetOwnerWindow SetOwnerWindow;
	//DClearOwnerWindow ClearOwnerWindow;
	DGetRunning GetRunning;

	DGetType GetType;
	DIsEOF	IsEOFTran;
};

class CDLLArray :public CPtrArray
{
public:
	CDLLArray() {};
	virtual ~CDLLArray();

	bool FInitialize();

	CDLL *PDll(EXT_TYPE ext);

	bool FActiveAndCheck(EXT_TYPE ext);
protected:

};


#endif // __DOCTRAN_H__

