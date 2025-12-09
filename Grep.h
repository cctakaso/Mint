/*    PortTool v2.2     grep.h          */

#ifndef __GREP_H__
#define __GREP_H__

#include "..\mdfc\ch.h"

#ifndef __cplusplus
#error Magical Design Foundation Classes require C++ compilation (use a .cpp suffix)
#endif


// 外部スペック
// 検索文字列.......検索文字列を","で区切って続けて入力できる。
// 特殊文字......?[任意の１文字],*[任意の文字列],\0xNNNN..(文字ｺｰﾄﾞ)
//		\? \, \* (文字ｺｰﾄﾞ)
#include "..\mdfc\dstring.h"
#include "metachar.h"
#include "greppat.h"
#include "strspec.h"

#define GREP_OPT_PARFECT 0	// 完全一致
#define GREP_OPT_CASE 1		// 英字大小文字無視
#define GREP_OPT_FAZ 2		// ﾌｧｼﾞｰ

class CFindFile;

//#define M_ALLOC


//#define chPrefixMatch        '^'
//#define chMatchNBS           's'
#define chMatchTab           't'
#define chMatchNewLine       'n'
//#define chMatchNBS           's'
//#define chMatchNRH           '-'
//#define chMatchNBH           '~'
//#define chMatchEol           'p'

#define	wSpecialSkip		(0x7000) //must be upper byte value!!!!
#define	wSpecialSub			(0x6000) //must be upper byte value!!!!
#define wSpecialAny			(0x5000) //must be upper byte value!!!!
#define wSpecialStr			(0x4000) //must be upper byte value!!!!
#define wSpecialNL			(0x3000) //must be upper byte value!!!!


//
//
//
//


// グレップエンジンの基本クラス
class CGrepEngin : public CObject
{
protected:
	CFindFile *m_pcFiles;
	STRSPEC_ATTR *m_pAttrData;
	STRSPEC_METACHARS *m_pMetaCharsData;
	int m_iSpecialSub;	//ｽﾍﾟｼｬﾙ・ｻﾌﾞﾃﾞｰﾀｶｳﾝﾀ･ｸﾘｱ
	BOOL m_fMetaChars;
	BOOL m_fDbcs;
	BOOL m_fCase;
	BOOL m_fBytes;
	BOOL m_fUmura;
	BOOL	m_fSpace;
//	BOOL	m_fTab;
	BOOL	m_fLF;
	BOOL	m_fNoBreak;
	BOOL	m_fMatchWholeWord;

	// Grep ﾒﾝﾊﾞｰ
	//CPtrArray m_arypPat;
	CKeyOutLogic *m_pKeyOutLogic;
	CGrepPattern *m_pGrepPattern;
	
	BYTE m_bBMTble[256];          /* table for search algorithm */
public:
	CGrepEngin() {
		m_pGrepPattern = NULL; };
	~CGrepEngin() {
		if (m_pGrepPattern)
			delete m_pGrepPattern; };
	virtual int InitEngin()=0;
	// 一つの検索ﾊﾟﾀｰﾝによる下準備をする。
	virtual BOOL InitPattern(CGrepPattern  *pGrepPattern)=0;
	virtual BOOL FDoGrep()=0;
	void SetFileObject(CFindFile *pcFiles) {
		ASSERT(pcFiles);
		m_pcFiles = pcFiles;
		}
	BOOL FMetaChars() {
		return m_fMetaChars; }
};

//グレップのエンジンをダイナミックに選択するグレップ処理の入り口
class CGrep
{
protected:
	CGrepEngin *m_pcEngin;

public:	
	CGrep() {
		m_pcEngin=NULL; }

	~CGrep() {
		if (m_pcEngin)
			delete m_pcEngin;
		}

	// 初期設定
	int Init(CFindFile *pcFiles);

	CGrepEngin *GetEngin() {
		ASSERT(m_pcEngin);
		return m_pcEngin;
		}
};

//以下は、いろいろなグレップエンジン
// シンプル処理の検索エンジン(US-Match Uper Lower 切換機能付き)
#define PATTERNS
class CGrepEngin_SIMPLE : public CGrepEngin
{
protected:
	BYTE ChUpper(BYTE ch);
	BOOL FUpper(BYTE ch);
	BOOL FLower(BYTE ch);
	int  WchConvertSzForSearch(BOOL fSearchCase, WORD *szSrc,
						WORD *szDis1, WORD *szDis2, WORD *szDis3, WORD *szDis4);

	void SetForNext(int cchT, BYTE far *&lpchDoc, long &ccpRemain,
					int &cchMatched, WORD *&pwPat, WORD *&pwPatLast, CGrepPattern *pGrepPattern = NULL);


	BOOL FMatchCh(BYTE far *&lpchDoc, int &cchMatched, long &ccpRemain,
		 WORD *&pwPat, WORD &wSpecial, CGrepPattern *&pGrepPattern);

#ifdef PATTERNS
	WORD *PGetPatLast(CGrepPattern *pGrepPattern) {
		return &pGrepPattern->m_pwPattern[pGrepPattern->m_cchPattern - 1]; }	// ﾊﾟﾀｰﾝの末尾へのﾎﾟｲﾝﾀ };
	//BOOL FNextPattern(int &indexPat, CGrepPattern *&pGrepPattern, WORD *&pwPatLast);
#endif

public:	
	CGrepEngin_SIMPLE() {
		};
	~CGrepEngin_SIMPLE() {
		};

	int InitEngin();

	// 一つの検索ﾊﾟﾀｰﾝによる下準備をする。
	BOOL InitPattern(CGrepPattern  *pGrepPattern);

	int InitBMTble(CGrepPattern  *pGrepPattern, int cchMatched);
	// １ファイルの検索の実行
	BOOL FDoGrep();
};
// FUZY処理の検索エンジン


#endif // __GREP_H__
