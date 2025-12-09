/*    PortTool v2.2     GREP.CPP          */

#include "stdafx.h"
#include "Fdiver.h"
#include "mainfrm.h"
#include "ffile.h"
#include "findlist.h"
#include "findopti.h"
#include "finddlg.h"
#include "grep.h"
extern CFindDlg *vpFindDlg; // ﾌｧｲﾝﾄﾞﾀﾞｲｱﾛｸﾞ
extern CFindList *vpFindList;		// 検索結果ﾘｽﾄへのﾎﾟｲﾝﾀ
extern CFindOption *vpFindOption;
extern CKeyOutLogic *vpKeyOutLogic;
BOOL FCheckEsc();	// グローバル ESCチェック関数
#ifdef _DBCS
#include "..\mdfc\dbcs.h"
CT CtByteLp(BYTE far *szFirst, BYTE far *szCheck);
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 初期設定
int CGrep::Init(CFindFile *pcFiles)
{
	// Make Search Engin.
	if (m_pcEngin)
		delete m_pcEngin;

	STRSPEC_ATTR *pAttrData = vpFindDlg->m_strSpec.GetSelAttrData();
//	if (pAttrData->m_chPrefix==STRSPEC_ATTR_PREF_DBCS &&
//		(pAttrData->m_fCase || pAttrData->m_fBytes || pAttrData->m_fKata))
//		m_pcEngin = new CGrepEngin_DBCS_FUZY();
//	else
		m_pcEngin = new CGrepEngin_SIMPLE();

	m_pcEngin->SetFileObject(pcFiles);
	return m_pcEngin->InitEngin();
}

// input parameters:
// BYTE *szPattern
//  vpFindDlg->m_fMatchCase
//  vpFindDlg->m_fFuzy
// 検索ﾊﾟﾀｰﾝによる下準備をする。

BOOL FIsDigit(char	ch);
BOOL FIsDigitO(char	ch);
BOOL FIsDigitX(char	ch);
int CchChFromAnsiOrOem(BYTE *rgch, int	ich, BYTE *pch, int base);
char ChAnsiFromOem(char ch);
BOOL FSameCharCategory(BYTE ch1, BYTE ch2, BOOL fDbcs);//WbFromCh(char ch)
int CharCategory(BYTE ch, BOOL fDbcs);


BOOL CGrepEngin_SIMPLE::InitEngin()
{

	CString cstrKeyWord = vpFindDlg->GetStrBuffer(); // ｷｰﾜｰﾄﾞを入れておく
	ASSERT(!cstrKeyWord.IsEmpty());

	// キーロジックへのポインタをメンバに入れる
	m_pKeyOutLogic = vpKeyOutLogic;

	m_pMetaCharsData = NULL;
	// 正規表現オブジェクトへのポインタ取得
	if (m_fMetaChars = vpFindDlg->m_fMetaChars) //代入
		m_pMetaCharsData = &vpFindOption->m_metaChars.m_metaCharsData;

	// 文字列検索方法を取得する
	m_pAttrData = vpFindDlg->m_strSpec.GetSelAttrData();

	m_fCase = m_pAttrData->m_fCase;
	m_fSpace = m_pAttrData->m_fSpace;
	m_fLF = m_pAttrData->m_fLF;

#ifdef _DBCS
	m_fDbcs = m_pAttrData->m_chPrefix==STRSPEC_ATTR_PREF_DBCS;
	m_fBytes = (m_fDbcs && m_pAttrData->m_fBytes);
	m_fUmura = FALSE;
	m_fNoBreak = FALSE;
#else
	m_fDbcs = FALSE;
	m_fBytes = FALSE;
	//m_fUmura = (!m_fDbcs && m_pAttrData->m_fUmura);//umuraの代りにWholeWordを入れる
	m_fUmura = FALSE;
	m_fNoBreak = (!m_fDbcs && m_pAttrData->m_fNoBreak);
#endif

	m_fMatchWholeWord = m_pAttrData->m_fMatchWholeWord;

	if (m_fMatchWholeWord && m_fDbcs)
		{
		// 両端の文字が半角ｱﾙﾌｧﾍﾞｯﾄや数字やシンボルのみ摘要する。
		if (CharCategory(cstrKeyWord[0], m_fDbcs)==2 || CharCategory(cstrKeyWord[cstrKeyWord.GetLength()-1], m_fDbcs)==2)
			m_fMatchWholeWord = FALSE;
		}


	// 正規表現以外でのｸﾞﾚｯﾌﾟﾊﾟﾀｰﾝ格納ﾊﾞｯﾌｧは、あったらクリア
	if (m_pGrepPattern)
		delete m_pGrepPattern;
	m_pGrepPattern = NULL;


	int cchMatched=0;

	if (m_fMetaChars)
		{		
		ScanLogic sl;
		CGrepPattern *pGrepPattern;
		cchMatched=0;
		int iBlock=0;
		while ( (sl = m_pKeyOutLogic->ScanStrNext(pGrepPattern, cchMatched))!=slEndBlock)
			{
			ASSERT(sl==slNextBlock || sl == slNextPat);
			iBlock++;
			// パターンを解釈
			if (InitPattern(pGrepPattern)==FALSE)
				return FALSE;
		 	cchMatched += pGrepPattern->m_dcpMatchAny;
			}
		// １つのブロックしかない場合は、*?などがあるかどうかチェックして、なければメタキャラのフラグをオフする
		if (iBlock==1)
			{
			CString cstrCheck;
			cstrCheck = m_pMetaCharsData->str[mcPrefix];
			cstrCheck += m_pMetaCharsData->str[mcMatchAny];
			cstrCheck += m_pMetaCharsData->str[mcMatchStr];
			cstrCheck += m_pMetaCharsData->str[mcNot];
			cstrCheck.MakeLower();
			cstrKeyWord.MakeLower();
			if (cstrKeyWord.FindOneOf(cstrCheck)==-1)
				m_fMetaChars = FALSE;		// メタキャラのフラグを降ろす
			}
		if (m_fMetaChars)
			{
			cchMatched = m_pKeyOutLogic->GetMinMatchedForGrep();
			// グレープパターン配列初期化
			memset(m_bBMTble, (BYTE)cchMatched, sizeof(m_bBMTble));// B&Mテーブルの初期化(最大値の設定)

			cchMatched=0;
			while ( (sl = m_pKeyOutLogic->ScanStrNext(pGrepPattern, cchMatched))!=slEndBlock)
				{
				ASSERT(sl==slNextBlock || sl == slNextPat);
				//B&Mテーブルの作成
				cchMatched = InitBMTble(pGrepPattern, cchMatched);
				}
			// InitBMTble()が壊すため、'\0'に正常値を入れる
			m_bBMTble[0] = m_pKeyOutLogic->GetMinMatchedForGrep();
			}
		}


	if (!m_fMetaChars)
		{
		// グレップパターン構造体を作成してグレップパターン配列に追加
		m_pGrepPattern = new CGrepPattern;
		if (m_pGrepPattern==NULL)
			return FALSE;
		m_pGrepPattern->Initialize(vpFindDlg->GetStrBuffer());
		if (InitPattern(m_pGrepPattern)==FALSE)
			return FALSE;
		cchMatched = m_pGrepPattern->m_dcpMatchAny;
		// グレープパターン配列初期化
		memset(m_bBMTble, (BYTE)cchMatched, sizeof(m_bBMTble));// B&Mテーブルの初期化(最大値の設定)
		// パターンを解釈B&Mテーブルの作成
		InitBMTble(m_pGrepPattern, 0);
		// InitBMTble()が壊すため、'\0'に正常値を入れる
		m_bBMTble[0] = cchMatched;
		}

#ifdef _DEBUG
	if (m_fMetaChars)
		m_pKeyOutLogic->Dump(afxDump);
#endif //_DEBUG

	// 最後の文字の手前に無視する文字があるとするＤＢＣチューニングは省き、最終パターンは１byte文字とする
	BYTE *pbBMTble = m_bBMTble;   /* local for speed */
	// \nがﾊﾟﾀｰﾝで指定されていて場合か？
	if ((m_bBMTble[0]>pbBMTble[chNL]) && (pbBMTble[chNL] == pbBMTble[chReturn]))
		{
		pbBMTble[chReturn]++;
		}
	if (m_fLF)
		{
		if (pbBMTble[chNL]>1)
			pbBMTble[chNL] = 1;
		if (pbBMTble[chReturn]>2)
			pbBMTble[chReturn] = 2;
		}
	if (m_fNoBreak)
		{
		if (pbBMTble[chHyphen]>1)
			pbBMTble[chHyphen] = 1;
		if (pbBMTble[chNonBreakHyphen]>1)
			pbBMTble[chNonBreakHyphen] = 1;
		if (pbBMTble[chNonReqHyphen]>1)
			pbBMTble[chNonReqHyphen] = 1;
		if (pbBMTble[chNonBreakSpace]>1)
			pbBMTble[chNonBreakSpace] = 1;
		}
	if (m_fSpace)
		{
#ifdef _DBCS
//#define wchSpace1st				HighByte(wchSpace)
//#define wchSpace2nd				LowByte(wchSpace)
		if (m_fDbcs)
			{
			if (pbBMTble[wchSpace1st]>2)
				pbBMTble[wchSpace1st] = 2;
			if (pbBMTble[wchSpace2nd]>1)
				pbBMTble[wchSpace2nd] = 1;
			}
#endif
		if (pbBMTble[chSpace]>1)
			pbBMTble[chSpace] = 1;
		if (pbBMTble[chTab]>1)
			pbBMTble[chTab] = 1;
		}
	if (m_pAttrData->m_strOthers.GetLength()>0)
		{
#ifdef BUG	//BugFix:'97.2/5 無視する文字列に漢字が含まれていた場合 tok < 0 となる為、
			//		pbBMTbleより前の不定メモリを差すためバグとなる。
		char *ptok = m_pAttrData->m_strOthers.GetBuffer(0);
		char tok;
#else
		BYTE *ptok = (BYTE *)m_pAttrData->m_strOthers.GetBuffer(0);
		BYTE tok;
#endif //BUG
		for(; (tok=*ptok) != '\0'; ptok++)
			{
#ifdef _DBCS
			if ( m_fDbcs && IsDBCSLeadByte ( tok ) )
				{
				if (pbBMTble[tok]>2)
					pbBMTble[tok] = 2;
				ptok++;
				if (pbBMTble[*ptok]>1)
					pbBMTble[*ptok] = 1;
				}
			else
#endif //_DBCS
			if (pbBMTble[tok]>1)
				pbBMTble[tok] = 1;
			}
		}

	return TRUE;
}

// "abc[def]jkm
// "abc[def|ghi]jkm
// "abc[def|ghi|NULL]jkm
// "abc[def^ghi]jkm
// "abc[def!ghi]jkm
BOOL CGrepEngin_SIMPLE::InitPattern(CGrepPattern  *pGrepPattern)
{
	// ?ABC?や*ABC*など最後がマッチング・メタキャラクタである場合は、それを削除する
	if (m_fMetaChars)
		{
		CDString cdstr = pGrepPattern->m_cstr;
		WORD wch;
		while(wch = *cdstr.GetBuffer(0))
			{
			if (LowByte(wch)==(BYTE)m_pMetaCharsData->str[mcMatchAny][0] ||
			 LowByte(wch)==(BYTE)m_pMetaCharsData->str[mcMatchStr][0])
				{
				cdstr = cdstr.Right(cdstr.GetLength()-1);
				}
			else
				break;
			}
		while((cdstr.GetByteLength()>0) && (wch = cdstr[cdstr.GetLength()-1]))
			{
			if (wch != 0 && HighByte(wch)==0 && 
			(LowByte(wch)==(BYTE)m_pMetaCharsData->str[mcMatchAny][0] ||
			 LowByte(wch)==(BYTE)m_pMetaCharsData->str[mcMatchStr][0]))
				{
				VERIFY(cdstr.FRemoveTail());
				}
			else
				break;
			}
		if (cdstr.GetByteLength()>0)
			pGrepPattern->m_cstr = cdstr;
		}

	BYTE *szPattern = (BYTE *)pGrepPattern->m_cstr.GetBuffer(0);
	int	ich, iwPattern, cch, cchPattern, iwMatchAny;
	int	cchMatchAny = 0;  /* number of unescaped ?'s in string */
	WORD *pwPattern,*pwPatOppCase,*pwSpecial;		//ﾛｰｶﾙｽﾄｱﾊﾞｯﾌｧ
	BYTE chPattern;	//ﾛｰｶﾙｽﾄｱﾊﾞｯﾌｧ
	int iPatBlock;
	int cchPatternMin;
//#ifdef _DBCS
	int	iwMatchAnyPos=0;
//	BOOL	fSlowSearch = FALSE;  //for Hiragana (Semi-)Sonant backword search!
//#endif //_DBCS

	cch = strlen((char *)szPattern);

#ifdef _DBCS
	if (m_fBytes)
		iPatBlock = 2;
	else
#endif
		iPatBlock = 1;


	// ﾊﾟﾀｰﾝ格納バッファの作成
	if ((pGrepPattern->m_pwSpecial = new WORD[cch+1])==NULL) // +1 is for terminate code when m_fBytes is TRUE;
	 	{
		ASSERT(FALSE);
		return FALSE; //メモリがない
		}
	if ((pGrepPattern->m_pwPattern =  new WORD[cch*iPatBlock*2])==NULL)
	 	{
		ASSERT(FALSE);
		return FALSE; //メモリがない
		}

	pGrepPattern->m_cchBlockSize = cch;

	// ﾛｰｶﾙﾊﾞｯﾌｧｾｯﾄ
	pwSpecial    = pGrepPattern->m_pwSpecial;

	memset(pGrepPattern->m_pwPattern, (BYTE)0, (cch*iPatBlock*2)*sizeof(WORD));	// initialize to not special.

	memset(pwSpecial, (BYTE)0, (cch+1)*sizeof(WORD));	// initialize to not special.
#ifdef _DBCS
	if (m_fBytes)
		pwPattern    = pGrepPattern->m_pwSpecial; //same as specail!
	else
#endif
		pwPattern    = pGrepPattern->m_pwPattern;
	pwPatOppCase = &pGrepPattern->m_pwPattern[cch];	// Upper or Lower case の文字列を入れるバッファアドレス

	for (iwMatchAny =  -1, iwPattern = ich = 0; ich < cch; ich++)
		{{
		chPattern = szPattern[ich];
#ifdef _DBCS
		if ( m_fDbcs && IsDBCSLeadByte(chPattern) )
			{
			WORD wch = 0;
			ich++;
			if (m_fCase)
				wch = WchChangeCase(chPattern<<8 | szPattern[ich]);
			pwPattern[iwPattern] = chPattern;
			pwPatOppCase[iwPattern++] = HighByte(wch);
			chPattern = szPattern[ich];
			pwPattern[iwPattern] = chPattern;
			pwPatOppCase[iwPattern++] = LowByte(wch);
			continue;
			}
		else
#endif //_DBCS
		if ( m_fMetaChars && chPattern == m_pMetaCharsData->str[mcMatchAny][0] )
			{
			iwMatchAny = iwPattern;
			// m_fBytes == TRUE時に、pwPatternとpwSpecialは、同じエリアを指すので以下のコードは
			// 気を付ける事！！！！
			//pwPattern[iwPattern] = chPattern;
			pwSpecial[iwPattern++] = chPattern | wSpecialAny;
			cchMatchAny++;
			continue;
			}
		else if ( m_fMetaChars && chPattern == m_pMetaCharsData->str[mcMatchStr][0])//chMatchStr
			{
			iwMatchAny = iwPattern;
			// m_fBytes == TRUE時に、pwPatternとpwSpecialは、同じエリアを指すので以下のコードは
			// 気を付ける事！！！！
			//pwPattern[iwPattern] = chPattern;
			pwSpecial[iwPattern++] = chPattern | wSpecialStr;
			cchMatchAny++;
			continue;
			}
		else if ( m_fMetaChars && chPattern == m_pMetaCharsData->str[mcPrefix][0])//chPrefix:
			{
			chPattern = szPattern[ich + 1];
#ifdef _DBCS
			if ( m_fDbcs && IsDBCSLeadByte(chPattern) )
				continue;
			else
#endif //_DBCS
			if ( chPattern == m_pMetaCharsData->str[mcPrefix][0] ||
				chPattern == m_pMetaCharsData->str[mcMatchAny][0] ||
				chPattern == m_pMetaCharsData->str[mcMatchStr][0])
				{
				//wSpecial = 0;
				;
				}
			else if (chPattern==chMatchTab)
				{
				chPattern = chTab;
				}
			else if (chPattern==chMatchNewLine)
				{
				ich++;
				pwPattern[iwPattern] = chNL;
				pwPatOppCase[iwPattern] = chReturn;
				pwSpecial[iwPattern++] = chNL | wSpecialNL;
				continue;
				}
			else
				{
				int base = 10;
				if (chPattern=='x' || chPattern=='X')
					{
					base = 16;
					chPattern = szPattern[++ich+1];
					}
				else if (chPattern=='o' || chPattern=='O')
					{
					base = 8;
					chPattern = szPattern[++ich+1];
					}
				if (FIsDigit (chPattern))
					{
					int	cchT;
					BYTE chT;
					cchT = CchChFromAnsiOrOem(szPattern, ich, &chT, base);
					chPattern = chT;
					ich += (cchT - 1);  // ich is now pointing to 2nd-to-last
										//digit (or still the ^ if only one digit
					}
				else  if (chPattern == '\0')
					chPattern = m_pMetaCharsData->str[mcPrefix][0];//chPrefix;
				//else  // treat \x as x
				//	wNew = chPattern;
				}
			ich++;
			}
		if (m_fCase)
			{
#ifdef WIN32
			if (IsCharUpper (chPattern))
				pwPatOppCase[iwPattern] = (BYTE)CharLower((char far *)MAKEWORD(chPattern,0));
			else
				pwPatOppCase[iwPattern] = (BYTE)CharUpper((char far *)MAKEWORD(chPattern,0));
#else
			if (IsCharUpper (chPattern))
				pwPatOppCase[iwPattern] = (BYTE)AnsiLower((char far *)MAKELP(0,chPattern));
			else
				pwPatOppCase[iwPattern] = (BYTE)AnsiUpper((char far *)MAKELP(0,chPattern));
#endif
			}
		pwPattern[iwPattern++] = chPattern;
		}}


	cchPattern = iwPattern;

	if (cchMatchAny == cch)
		{
		// ?文字しかない場合は、?自体を見つける。
#ifdef _DBCS
		if (m_fBytes)
			{
			for (iwPattern = 0; iwPattern < cch; iwPattern++)
				pwSpecial[iwPattern] &= 0x00FF;	// reset high byte.
			}
		else
#endif //_DBCS

/*    PortTool v2.2     1995/10/16    8:3          */
/*      Found   : (WORD)          */
/*      Issue   : ３２ビット値を不正にキャストしていないかチェックして下さい          */
/*      Suggest : 可能なら３２ビットデータに変更して下さい          */
/*      Help available, search for WORD in WinHelp file API32WH.HLP          */
			memset(pwSpecial, (BYTE)0, (cch+1)*sizeof(WORD));	// initialize to not special.
		iwMatchAny = -1;
		}


#ifdef _DBCS
	if (m_fBytes)
		{
		// pwSpecialが指す所に、ソースパターンが入っている
		// pwPatternを本来のオリジナルパターンポインタに戻す
		pwPattern = pGrepPattern->m_pwPattern; // set one of the real pattern strings .
		// ﾃﾞｨｽﾃｨﾈｰｼｮﾝﾊﾞｯﾌｧを全てNULLにしておく

/*    PortTool v2.2     1995/10/16    8:3          */
/*      Found   : (WORD)          */
/*      Issue   : ３２ビット値を不正にキャストしていないかチェックして下さい          */
/*      Suggest : 可能なら３２ビットデータに変更して下さい          */
/*      Help available, search for WORD in WinHelp file API32WH.HLP          */
		memset(pGrepPattern->m_pwPattern, (BYTE)0, (cch*iPatBlock*2)*sizeof(WORD));	// initialize to all NULL.
		cchPattern = WchConvertSzForSearch(m_fCase, pwSpecial /* this is source string */,
			pwPattern, pwPatOppCase,
			&pGrepPattern->m_pwPattern[cch*2], &pGrepPattern->m_pwPattern[cch*3]);

		iwMatchAnyPos = iwMatchAny = -1;

		for (ich = iwPattern = 0; ich < cchPattern; ich++)
			{
			switch (pwSpecial[ich] & 0xff00)
				{
			default:
				/* if NULL is not inc. to iwPattern */
				if (HighByte(pwPattern[ich]))
					iwPattern++;
				
				if (LowByte(pwPattern[ich]))
					iwPattern++;
//				else
					/* slow search flg.
					 if the search pattern contain (semi-)sonant then
					 set for search pattern character's Boyer/Moore table volue
					 to always 0
					*/
//					fSlowSearch = TRUE;
				break;
			case wSpecialAny:
			case wSpecialStr:
				iwMatchAnyPos = ich;
				iwMatchAny = iwPattern;
				iwPattern++;
				break;
				}
			}
		cchPatternMin = iwPattern;
		}
	else
#endif //_DBCS
		{
		cchPatternMin = cchPattern;
		iwMatchAnyPos = iwMatchAny;
		}

//cchPattern.........
//cchPatternMin......
//iwMatchAny.........
//iwMatchAnyPos......
	pGrepPattern->m_cchPattern = cchPattern;      /* length of search string */
	pGrepPattern->m_iwMatchAnyPos = iwMatchAnyPos;
	pGrepPattern->m_dcpMatchAny = cchPatternMin - iwMatchAny - 1;;
	//pGrepPattern->m_cchPatternMin = cchPatternMin;
	//pGrepPattern->m_iwMatchAny = iwMatchAny;
	return TRUE;
}

int CGrepEngin_SIMPLE::InitBMTble(CGrepPattern  *pGrepPattern, int cchMatched)
{
	int	iwPattern, cch;
	BYTE *pbBMTble;   /* local for speed */
	WORD *pwPattern,*pwPatOppCase;		//ﾛｰｶﾙｽﾄｱﾊﾞｯﾌｧ
	BYTE chbuf;
	int	dcp, dcpMatchAny;

	// ﾊﾟﾀｰﾝ末尾から、ﾜｲﾙﾄﾞｶｰﾄﾞ(?)の位置まで位置ｵﾌｾｯﾄの値で、B&Mﾃｰﾌﾞﾙ内全ての文字
	// 位置にｾｯﾄする。
	dcpMatchAny = pGrepPattern->m_dcpMatchAny + cchMatched;
	cch = pGrepPattern->m_cchBlockSize;	

	// ﾜｲﾙﾄﾞｶｰﾄﾞ(?)の位置以降のﾊﾟﾀｰﾝ文字について、B&Mﾃｰﾌﾞﾙに値を入れる。
#ifdef _DBCS
	if (m_fBytes)
 	{
	int id;
	WORD whbuf;
	int dcpfirst;
	int dcpSecnd;

/*	if (fSlowSearch)
		for (iwPattern = pGrepPattern->m_iwMatchAnyPos + 1; 
			iwPattern <pGrepPattern->cchPattern; iwPattern++)
			{
			for(id = 0, pwPattern = pGrepPattern->m_pwPattern; id<4; id++, pwPattern+=cch)
				{
				if (HighByte(whbuf=pwPattern[iwPattern]))
					{
					if ( m_bBMTble[chbuf = LowByte(whbuf)] > 1)	
						m_bBMTble[chbuf]= 1;
					m_bBMTble[HighByte(whbuf)]= 0;
					}
				else
					m_bBMTble[whbuf]= 0;
				}
			}
	else
*/
	for (iwPattern = pGrepPattern->m_iwMatchAnyPos + 1, dcp = dcpMatchAny - 1; 
			dcp >= 0; iwPattern++, dcp--)
		{
	  	if (HighByte(whbuf=pGrepPattern->m_pwPattern[iwPattern]))
	  		{
	  		dcpfirst = dcp;
	  		dcpSecnd = dcp-1;
			if (m_bBMTble[chbuf = LowByte(whbuf)] > dcpfirst)	
				m_bBMTble[chbuf]= dcpfirst;	
			if (m_bBMTble[chbuf = HighByte(whbuf)] > dcpSecnd)	
				m_bBMTble[chbuf] = dcpSecnd;	
	  		dcp--;
	  		}	
		else
			{
			if (whbuf==0) // it's blank area!
				{
				dcp++;
				continue;
				}
			if (m_bBMTble[whbuf] > dcp)	
	  			m_bBMTble[whbuf]=dcp;	
			}
	  	dcpfirst = dcp+1;
		dcpSecnd = dcp;
		for(id = 1, pwPattern = pGrepPattern->m_pwPattern + cch; id<4; id++, pwPattern+=cch)
			{
			if (HighByte(whbuf=pwPattern[iwPattern]))
				{
				if (m_bBMTble[chbuf = LowByte(whbuf)] > dcpfirst)	
					m_bBMTble[chbuf]= dcpfirst;	
				if (m_bBMTble[chbuf = HighByte(whbuf)] > dcpSecnd)	
					m_bBMTble[chbuf]= dcpSecnd;	
				}	
			else
				{
				if (m_bBMTble[whbuf] > dcp)	
					m_bBMTble[whbuf]=dcp;	
				}
			}
		}
	}
	else
#endif /* _DBCS */
	for (pwPattern = pGrepPattern->m_pwPattern, pwPatOppCase = &pGrepPattern->m_pwPattern[pGrepPattern->m_cchBlockSize],
		pbBMTble = m_bBMTble, iwPattern = pGrepPattern->m_iwMatchAnyPos + 1, dcp = dcpMatchAny - 1; 
			dcp >= 0; iwPattern ++, dcp--)
		{
//		if (pGrepPattern->m_pwPattern[iwPattern] >= 256)   /* don't trash memory when */
//			continue;                      /* we encounter wMatchWhite */
		// 同類ｳﾑﾗｳﾄ文字区別なし？
		if (m_fUmura)
			{{ /* NATIVE  */
			BYTE	chIntl, chUpper;
			// 同類文字の大小文字区別なしとする。
			chUpper = (BYTE)pwPattern[iwPattern];
			for (chIntl = 0; chIntl < 256; chIntl++)
				if (ChUpper(chIntl) == chUpper)
					{
					if (pbBMTble[chIntl]>dcp)
						pbBMTble[chIntl] = dcp;
					}
			if (pbBMTble[chbuf = (BYTE)pwPatOppCase[iwPattern]]>dcp)
				pbBMTble[chbuf] = dcp;
			}}
		else
			{
			if (pbBMTble[chbuf = (BYTE)pwPattern[iwPattern]]>dcp)
				pbBMTble[chbuf] = dcp;
			// 大文字小文字区別なし？
			if (m_fCase)
				{
				if (pbBMTble[chbuf = (BYTE)pwPatOppCase[iwPattern]]>dcp)
					pbBMTble[chbuf] = dcp;
				}
			}
		}
	return dcpMatchAny;
}
//						  ロジック		インデックス
//						Out		In		Out		In

// New File		---- 	Nutral	Nutral	Init	Init
// End File		---- 	Nutral	Nutral	Init	Init
// New pDoc		---- 	  -		Nutral	Init	Init
// New KeyOut	---- 	  -		 -		-		-
// New KeyIn	---- 	  -		 -		-		-

//  File		---- 	Nutral	Nutral	Init	Init
// New pDoc		---- 	  -		Nutral	Init	Init
// New KeyOut	---- 	  -		 -		-		-
// New KeyIn	---- 	  -		 -		-		-
BOOL CGrepEngin_SIMPLE::FDoGrep()
{
//　前処理
	BYTE far *lpchDoc;	// ﾄﾞｷｭﾒﾝﾄｽｷｬﾝﾎﾟｲﾝﾀ
	BYTE chDoc;					// ﾄﾞｷｭﾒﾝﾄｷｬﾗｸﾀﾊﾞｯﾌｧ

	CGrepPattern *pGrepPattern; // ｸﾞﾚｯﾌﾟﾊﾟﾀｰﾝを指すﾎﾟｲﾝﾀ
#ifdef PATTERNS
	VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse)); // ﾃﾝﾎﾟﾗﾙｴﾝﾄﾘｰは破棄する
	ScanLogic slTmp;	
	ReturnLogic rlFile = rlNutral;
	ReturnLogic rlLine;
	int cchMatchedIO;
	if (m_fMetaChars)
		{
		m_pKeyOutLogic->SetNutralForAll();	// ﾊﾟﾀｰﾝﾛｼﾞｯｸをニュートラルへ
		m_pKeyOutLogic->ScanStrNext(pGrepPattern, cchMatchedIO = 0 /*initial*/);
		}
	else
		pGrepPattern = m_pGrepPattern;
	int indexLogicCan = -1; // ｷｬﾝｾﾙ･ｲﾝﾃﾞｯｸｽﾛｼﾞｯｸを指す
	long ccpRemainCan;				// ｷｬﾝｾﾙ･ﾄﾞｷｭﾒﾝﾄｽｷｬﾝ残りｻｲｽﾞ
#endif
	ASSERT(pGrepPattern!=NULL);
	WORD *pwPatLast = PGetPatLast(pGrepPattern);	// ﾊﾟﾀｰﾝの末尾へのﾎﾟｲﾝﾀ
	WORD *pwPat;					// ﾊﾟﾀｰﾝ文字列ｽｷｬﾝﾎﾟｲﾝﾀ

	long ccpRemain;				// ﾄﾞｷｭﾒﾝﾄｽｷｬﾝ残りｻｲｽﾞ
	int cchMatched;				// ﾄﾞｷｭﾒﾝﾄで見つかっている文字数
	int cchT;					// ﾃﾝﾎﾟﾗﾘで使用する、ｽｷｯﾌﾟﾊﾞｲﾄ数
	BOOL fFirstFind = TRUE;
	// 見つけてからの変数
	long lMatchFirst;
	BOOL fWild = FALSE;
	WORD *pwPatWild=NULL;	// ﾊﾟﾀｰﾝ内のﾜｲﾙﾄﾞｶｰﾄﾞを指すﾎﾟｲﾝﾀ
	WORD wSpecial;


	// ﾊﾟﾀｰﾝが"!string"等のために、ファイル名だけはｴﾝﾄﾘｰしておく
	if (m_fMetaChars && fFirstFind)
		{
		// 検索結果ﾘｽﾄへの追加(ﾃﾞｨﾚｸﾄﾘ＆ﾌｧｲﾙ)
		if (!vpFindList->FAddList(m_pcFiles, rlNutral))
			return FALSE;	// もうこれ以上リストに入れられない。
		fFirstFind = FALSE;
		}


// ﾌｧｲﾙを読み込む、途中までの情報はｸﾘｱされる。
LFRead:
	if (FCheckEsc())
		{
#ifdef PATTERNS
		VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse)); // ﾃﾝﾎﾟﾗﾙｴﾝﾄﾘｰは破棄する
#endif //PATTERNS
		return FALSE;	// ﾕｰｻﾞによる中断
		}
//#ifdef MATCHANY
	
	if ((ccpRemain=m_pcFiles->ReadBuffer())==0)
		{
#ifdef PATTERNS
		if (m_fMetaChars)
			{
			m_pKeyOutLogic->SetFalseForAllNutral();
			rlFile = m_pKeyOutLogic->CheckLogic();	// ﾊﾟﾀｰﾝﾛｼﾞｯｸをﾁｪｯｸ
			ASSERT(rlFile==rlTrue || rlFile==rlFalse);
			if (!vpFindList->FAddListFromTempList(m_pcFiles, rlFile))
				return FALSE;	// もうこれ以上リストに入れられない。
			}
#endif //PATTERNS
		return TRUE;
		}
//#else
//	if ((ccpRemain=m_pcFiles->ReadBuffer())==0)
//		return TRUE;
//#endif
	else if (ccpRemain==-1)
		{
		// エラー
#ifdef PATTERNS
		VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse)); // ﾃﾝﾎﾟﾗﾙｴﾝﾄﾘｰは破棄する
#endif //PATTERNS
		return FALSE;
		}

#ifdef MATCHANY
	ccpRemain-=m_pcFiles->GetOffSetOfScanStart();
	lpchDoc=m_pcFiles->GetBuffer()+m_pcFiles->GetOffSetOfScanStart();	// ﾄﾞｷｭﾒﾝﾄｽｷｬﾝﾎﾟｲﾝﾀ
	if (ccpRemain==0)
		goto LFRead;
	ASSERT(ccpRemain>0);
#else
	lpchDoc=m_pcFiles->GetBuffer();	// ﾄﾞｷｭﾒﾝﾄｽｷｬﾝﾎﾟｲﾝﾀ
#endif

	if (ccpRemainCan==-1)
		ccpRemainCan = ccpRemain;
	else
		ccpRemainCan = -10;


	pwPat = pwPatLast;
	cchMatched = 0;

// ｻｰﾁ処理を行う
// 通常処理
LFcharLoop:
	if (cchMatched != 0)
		{
LFSlowcharLoop:
		chDoc=*lpchDoc;
		wSpecial = pGrepPattern->m_pwSpecial[ pwPat - pGrepPattern->m_pwPattern ] & 0xFF00;
		if (wSpecial == wSpecialNL) //ここでなければならない。改行ｺｰﾄﾞを無視するより前に処理
			{
			if (chDoc==chNL)
				{
				// 0x0D 0x0Aか？
				if (ccpRemain < m_pcFiles->GetReadSize() && *(lpchDoc-1) == chReturn)
					{
					cchMatched++;
					lpchDoc--;
					ccpRemain++;
					}
				goto LFcharMatch;
				}
			}
		if (FMatchCh(lpchDoc, cchMatched, ccpRemain, pwPat, wSpecial, pGrepPattern))
		 	goto LFcharMatch;

		switch(wSpecial)
			{
			case wSpecialAny:
#ifdef _DBCS
				if (m_fDbcs &&
					CtByteLp(m_pcFiles->GetBuffer(), lpchDoc)==ctDbc2b)
					{
					pwPat++;	//もう一度'?'でマッチングする
					}
#endif //_DBCS
				goto LFcharMatch;
				break;
			case wSpecialStr:
				fWild = TRUE;
				pwPatWild=pwPat; // ﾊﾟﾀｰﾝ内のﾜｲﾙﾄﾞｶｰﾄﾞを指すﾎﾟｲﾝﾀ
				if (--pwPat < pGrepPattern->m_pwPattern)
					goto LFMatchFound;
				//次のパターンを同一ﾄﾞｷｭﾒﾝﾄﾎﾟｲﾝﾀで
				goto LFMatchAny;
				break;
			default:
				break;
			}
		if (fWild && (m_pAttrData->m_fLF || chDoc!=chNL))
			{
			ASSERT(pwPatWild);
			pwPat=pwPatWild;	// ﾊﾟﾀｰﾝ内のﾜｲﾙﾄﾞｶｰﾄﾞを指すﾎﾟｲﾝﾀをｾｯﾄして
								// 一つ手前のﾊﾟﾀｰﾝ文字列のﾏｯﾁﾝｸﾞを最初からおこなう
			goto LFcharMatch;
			}

RtnForWholeWord:
#ifdef PATTERNS
		//このパターンは一致しなかった
		if (m_fMetaChars)
			{
			cchMatchedIO = cchMatched;
			slTmp = m_pKeyOutLogic->ScanPatternNext(pGrepPattern, rlFalse, cchMatchedIO, rlLine, rlFile);
			if (rlLine==rlTrue)
				{
				cchMatched++;
				goto LNoMatchFound;
				}
			switch(slTmp)
				{
				case slNextPat:
					if (cchMatchedIO != cchMatched)
						{
						ASSERT(cchMatched>cchMatchedIO);
						SetForNext(cchMatched-cchMatchedIO, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast, pGrepPattern);
						cchMatched = cchMatchedIO;
						}
					else
						pwPat = pwPatLast = PGetPatLast(pGrepPattern);
					fWild = FALSE;
					goto LFSlowcharLoop;
					break;
				case slNextBlock:
					SetForNext(cchMatched, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast, pGrepPattern);
					fWild = FALSE;
					goto LFSlowcharLoop;
					break;
				case slEndBlock:
					m_pKeyOutLogic->ScanPatternFirst(pGrepPattern);
					pwPat = pwPatLast = PGetPatLast(pGrepPattern);
					break; //thow
				case slFalseLogic:
					VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse));
					return TRUE;
					break;
				}
			}
#endif //PATTERNS
		/* B&M処理へ戻る準備 */
		SetForNext(max(cchMatched + 1, m_bBMTble[chDoc]), lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast);
		/* B&M処理へ */
		}
	else
		// B&M処理
		{
		fWild = FALSE;	// ﾜｲﾙﾄﾞｶｰﾄﾞ・ﾌﾗｸﾞｵﾌ
		do
			{
			chDoc = *lpchDoc;
			if ((cchT = m_bBMTble[chDoc]) == 0)
#ifdef PATTERNS
				{
//				if (m_fMetaChars)
//					m_pKeyOutLogic->ScanPatternFirst(pGrepPattern);
#endif //PATTERNS
				goto LFSlowcharLoop;
#ifdef PATTERNS
				}
#endif //PATTERNS
			lpchDoc += cchT;
			}
		while ((ccpRemain -= cchT) > 0);
			//このパターンは一致しなかった
		}
LFCheckRemain:
	if (ccpRemain > 0)
		goto LFcharLoop;
//#ifdef PATTERNS
//	//このパターンは一致しなかった
//	if (FNextPattern(pGrepPattern, pwPatLast))
//		{
//		SetForNext(cchMatched, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast);
//		fWild = FALSE;
//		goto LFSlowcharLoop;
//		}
//#endif //PATTERNS
	// あきらめて次へゆく
	goto LFRead;

LFcharMatch:
	/* chDoc is a match   */
	cchMatched++;
	if (--pwPat < pGrepPattern->m_pwPattern)
		goto LFMatchFound;
//LFBackUp:
	lpchDoc--;
	ccpRemain++;

LFMatchAny:
	if (ccpRemain <= m_pcFiles->GetReadSize())
		goto LFCheckRemain;
// リードしたバッファ以前にまたがる
//ここでリードしてはいけない。
//	goto LFRead;

#ifdef PATTERNS
	//このパターンは一致しなかった
	if (m_fMetaChars /*&& FNextPattern(pGrepPattern, pwPatLast)*/)
		{
		cchMatchedIO = cchMatched;
		slTmp = m_pKeyOutLogic->ScanPatternNext(pGrepPattern, rlNutral, cchMatchedIO, rlLine, rlFile);
		if (rlLine==rlTrue)
			{
			lpchDoc++;
			ccpRemain--;
			goto LNoMatchFound;
			}
		switch(slTmp)
			{
#ifdef _DEBUG
			case slNextPat:
				ASSERT(FALSE);
				break;
#endif //_DEBUG
			case slNextBlock:
				SetForNext(cchMatched, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast, pGrepPattern);
				fWild = FALSE;
				goto LFSlowcharLoop;
				break;
			case slEndBlock:
				m_pKeyOutLogic->ScanPatternFirst(pGrepPattern);
				pwPat = pwPatLast = PGetPatLast(pGrepPattern);
				break; //thow
			case slFalseLogic:
				VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse));
				return TRUE;
				break;
			}
		}
#endif //PATTERNS

	// あきらめて次へゆく
	SetForNext(cchMatched+1, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast);
	goto LFCheckRemain;
		
/*ASSERT(FALSE);
	return FALSE;
*/
LFMatchFound:
	if (FCheckEsc())
		{
#ifdef PATTERNS
		VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse)); // ﾃﾝﾎﾟﾗﾙｴﾝﾄﾘｰは破棄する
#endif //PATTERNS
		return FALSE;	// ﾕｰｻﾞによる中断
		}

	if (m_fMatchWholeWord)
		{
		// ｱﾙﾌｧﾍﾞｯﾄの境目であるかチェックする
		if ((ccpRemain < m_pcFiles->GetReadSize()))
			cchT = !FSameCharCategory((BYTE)*lpchDoc, (BYTE)*(lpchDoc-1), m_fDbcs);
		else
			cchT = m_pcFiles->FFirstRead();
		if (cchT==(int)TRUE)
			{
			if ((ccpRemain-cchMatched)>=0)
				cchT = !FSameCharCategory((BYTE)*(lpchDoc+cchMatched), (BYTE)*(lpchDoc+cchMatched-1), m_fDbcs);
			else
				cchT = m_pcFiles->FLastRead();
			}
		if (cchT==(int)FALSE)
			{
			cchMatched--;
			++pwPat;
			goto RtnForWholeWord;
			}
		}

	if (m_fMetaChars)
		{
		indexLogicCan = m_pKeyOutLogic->GetNowIndexLogic();
		cchMatchedIO = cchMatched;
		slTmp = m_pKeyOutLogic->ScanPatternNext(pGrepPattern, rlTrue, cchMatchedIO, rlLine, rlFile);
		if (slTmp == slFalseLogic)
			{
			VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse));
			return TRUE;
			}
		}
	else
		{
		if (fFirstFind)
			{
			// 検索結果ﾘｽﾄへの追加(ﾃﾞｨﾚｸﾄﾘ＆ﾌｧｲﾙ)
			if (!vpFindList->FAddList(m_pcFiles, rlTrue))
				return FALSE;	// もうこれ以上リストに入れられない。
			fFirstFind = FALSE;
			}
		}
LNoMatchFound:
	if (vpFindDlg->m_fDispDoc || vpFindDlg->m_optDispNum>0)
		{
		lMatchFirst = m_pcFiles->GetSeek()+(lpchDoc - m_pcFiles->GetBuffer());
		// 検索結果ﾘｽﾄへの追加(位置&文書)
#ifdef PATTERNS
		//つぎのパターンを調べる
		if (m_fMetaChars)
			{
			if (rlLine==rlTrue)
				{
				// ここでは、テンポラリにのみ追加
				//if (rlFile == rlTrue && !vpFindList->FAddListFromTempList(m_pcFiles, rlTrue))
				//	return FALSE;	// もうこれ以上リストに入れられない。
				if (!vpFindList->FAddList(m_pcFiles, lMatchFirst, lpchDoc, rlNutral))
					return FALSE;	// もうこれ以上リストに入れられない。
				ccpRemainCan = ccpRemain - cchMatched;
				}

			switch(slTmp)
				{
				case slNextPat:
					lpchDoc--;
					ccpRemain++;
					if (cchMatchedIO != cchMatched)
						{
						ASSERT(cchMatched>cchMatchedIO);
						SetForNext(cchMatched-cchMatchedIO, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast, pGrepPattern);
						cchMatched = cchMatchedIO;
						}
					else
						pwPat = pwPatLast = PGetPatLast(pGrepPattern);
					fWild = FALSE;
					ccpRemainCan = -10;
					goto LFSlowcharLoop;
					break;
				case slNextBlock:
					if (rlLine==rlFalse && ccpRemain==ccpRemainCan && m_pKeyOutLogic->FResetSXorIndexLogic(indexLogicCan))
						{
						vpFindList->DeleteLastOneOfList();
						ccpRemainCan = -10;
						}
					lpchDoc--;
					ccpRemain++;
					SetForNext(cchMatched, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast, pGrepPattern);
					fWild = FALSE;
					goto LFSlowcharLoop;
					break;
				case slEndBlock:
					if (rlLine==rlFalse && ccpRemain==ccpRemainCan && m_pKeyOutLogic->FResetSXorIndexLogic(indexLogicCan))
						{
						vpFindList->DeleteLastOneOfList();
						ccpRemainCan = -10;
						}
					m_pKeyOutLogic->ScanPatternFirst(pGrepPattern);
					pwPat = pwPatLast = PGetPatLast(pGrepPattern);
					break; //thow
				//case slFalseLogic:
				//	VERIFY(vpFindList->FAddListFromTempList(m_pcFiles, rlFalse));
				//	return TRUE;
				//	break;
				}
			}
		else
#endif //PATTERNS
		if (!vpFindList->FAddList(m_pcFiles, lMatchFirst, lpchDoc, rlTrue))
			return FALSE;	// もうこれ以上リストに入れられない。
		// 次のを見つけに行く場合のみの処理
		SetForNext(cchMatched, lpchDoc, ccpRemain, cchMatched, pwPat, pwPatLast);
		goto LFCheckRemain;
		}
	else
		return TRUE;
// 見つかったら、表示する。
// ユーザアボート受け付け処理
}

BOOL CGrepEngin_SIMPLE::FMatchCh(BYTE far *&lpchDoc, int &cchMatched, long &ccpRemain,
		 WORD *&pwPat, WORD &wSpecial, CGrepPattern *&pGrepPattern)
{

	BYTE chDoc=*lpchDoc;
	if (m_fBytes)
		{
		WORD whbuf;
		int cch = pGrepPattern->m_cchBlockSize;
		WORD *pwPattern=pwPat;
		//szSpec=　(3)
		//szDis4=カ゛
		//szDis3=　ガ
		//szDis2=
		//szDis1=ｶ ﾞ
		for(int id = 0; id<4; id++, pwPattern+=cch)
			{
			if (HighByte(whbuf=*pwPattern))
				{
				if (ccpRemain < m_pcFiles->GetReadSize())
					{
					if (whbuf == *((WORD *)(lpchDoc-1)))
						{
						cchMatched++;
						lpchDoc--;
						ccpRemain++;
						if (wSpecial==wSpecialSkip)
							--pwPat;
						return TRUE; //goto LFcharMatch;
						}
					}
				}
			else if (whbuf!=0 && whbuf==chDoc)
				{
				return TRUE; //goto LFcharMatch;
				}
			}
		}
	else
		{
		BYTE chPat;
		if ((chPat = (BYTE)*pwPat) == chDoc)
			return TRUE; //goto LFcharMatch;
		if (m_fCase)
			{
			if (chDoc == *(pwPat + pGrepPattern->m_cchBlockSize))
				return TRUE;//goto LFcharMatch;
			/* if it failed all above, and could need special intl
					upper casing, try ChUpper
				*/
			if (m_fUmura && chDoc > 127)
				if (ChUpper(chDoc) == chPat)
					return TRUE;//goto LFcharMatch;
			}
		}

	if (chDoc == chNL || chDoc == chReturn)
		{
		if (m_fLF)
			{
			pwPat++;
			return TRUE;
			}
		}
	else if ( m_fNoBreak && 
		(chDoc == chHyphen || chDoc == chNonBreakHyphen || chDoc == chNonReqHyphen || chDoc ==chNonBreakSpace))
		{
		pwPat++;
		return TRUE;
		}
	else if (m_fSpace && (chDoc == chSpace || chDoc == chTab) )
		{
		pwPat++;
		return TRUE;
		}
	else if (m_pAttrData->m_strOthers.GetLength()>0)
		{
#ifdef BUG	//BugFix:'97.2/5 無視する文字列に漢字が含まれていた場合 tok < 0 となる為、
			//		pbBMTbleより前の不定メモリを差すためバグとなる。
		char *ptok = m_pAttrData->m_strOthers.GetBuffer(0);
		char tok;
#else
		BYTE *ptok = (BYTE *)m_pAttrData->m_strOthers.GetBuffer(0);
		BYTE tok;
#endif //BUG
		for(; (tok=*ptok) != '\0'; ptok++)
			{
#ifdef _DBCS
			if ( m_fDbcs && IsDBCSLeadByte ( tok ) )
				{
				if (*((WORD *)ptok)== *((WORD *)(lpchDoc-1)))
					{
					cchMatched++;
					lpchDoc--;
					ccpRemain++;
					pwPat++;
					return TRUE;
					}
				ptok++;
				}
			else
#endif //_DBCS
			if (chDoc==tok)
				{
				pwPat++;
				return TRUE;
				}
			}
		}
#ifdef _DBCS
	else if ( m_fDbcs && m_fSpace && 
			ccpRemain < m_pcFiles->GetReadSize() && wchSpaceStr == *((WORD *)(lpchDoc-1)) )
		{
		cchMatched++;
		lpchDoc--;
		ccpRemain++;
		pwPat++;
		return TRUE;
		}
#endif //_DBCS

	return FALSE;// next pattern
}


void CGrepEngin_SIMPLE::SetForNext(int cchT, BYTE far *&lpchDoc, long &ccpRemain,
							int &cchMatched, WORD *&pwPat, WORD *&pwPatLast, CGrepPattern *pGrepPattern/*=NULL*/)
{
	ASSERT(cchT>=0);
	lpchDoc += cchT;
	ccpRemain -= cchT;
	cchMatched = 0;
	if (pGrepPattern)
		pwPatLast = PGetPatLast(pGrepPattern);
	pwPat = pwPatLast;
	//ASSERT(ccpRemain>=0);
}

#ifdef PATTERNSaaaaaaaa
BOOL CGrepEngin_SIMPLE::FNextPattern(CGrepPattern *&pGrepPattern, WORD *&pwPatLast)
{
	int iub = m_arypPat.GetUpperBound();
	if (iub==0)
		return FALSE;	// 1つのパターンの場合は何もしない。
	else if (indexPat == iub)
		indexPat = 0;	// 最初のパターンをイニシャルセット
	else
		indexPat++;		// 次のパターンへ
	if (!m_pKeyOutLogic->ScanPatternNext(FALSE, NULL, pGrepPattern))
		return FALSE;
	pGrepPattern = (CGrepPattern *)m_arypPat[indexPat];
	pwPatLast = &pGrepPattern->m_pwPattern[pGrepPattern->m_cchPattern - 1];	// ﾊﾟﾀｰﾝの末尾へのﾎﾟｲﾝﾀ
	if (indexPat==0)
		return FALSE;	// 最初のパターンをセットした場合は、次のドキュメントポインタへ
	else
		return TRUE;	// 次のパターンを同一ドキュメントポインタで
}
#endif //PATTERNS

#ifdef _DBCS
CT CtByteLp(BYTE far *szFirst, BYTE far *szCheck)
{
	BYTE far *sz = szFirst;
	while ( TRUE )
		{
		if (sz==szCheck)
			break;
		else
			{
			if ( IsDBCSLeadByte ( *sz ) )
				{
				sz++ ;
				if (sz==szCheck)
					return ctDbc2b;
				}
			sz++;
			}
		}
	return IsDBCSLeadByte ( *sz ) ? ctDbc1b:ctSbc;
}
#endif //_DBCS


BOOL CGrepEngin_SIMPLE::FUpper(BYTE ch)
{

	return  (((UINT)(ch - 'A') <= ('Z' - 'A')) ||
			/* foreign */
	((UINT)(ch - 0x00C0) <= (0x00D6 - 0x00C0)) ||
			((UINT)(ch - 0x00D8) <= (0x00DE - 0x00D8)));
}
/*  %%Function: FLower  %%Owner: bradv  */

BOOL CGrepEngin_SIMPLE::FLower(BYTE ch)
{
	return (((UINT)(ch - 'a') <= ('z' - 'a')) ||
			/* foreign */
	((UINT)(ch - 0x00DF) <= (0x00F6 - 0x00DF)) ||
			((UINT)(ch - 0x00F8) <= (0x00FF - 0x00F8)));
}


BYTE CGrepEngin_SIMPLE::ChUpper(BYTE ch)
{
#define chFirstUpperTbl  (224)

	if ((UINT)(ch - 'a') <= ('z' - 'a'))
		return (ch - ('a' - 'A'));
	else  if (ch >= chFirstUpperTbl)
		{
		return ((ch == 247 || ch == 255) ? ch : ch - 32);
		}
	else
		return (ch);
}

BOOL FIsDigit(char	ch)
{
	return ('0' <= ch && ch <= '9');
}
BOOL FIsDigitO(char	ch)
{
	return ('0' <= ch && ch <= '8');
}
BOOL FIsDigitX(char	ch)
{
	return ('0' <= ch && ch <= '9') || 
			('a' <= ch && ch <= 'f') ||
			('A' <= ch && ch <= 'F');
}

BOOL FSameCharCategory(BYTE ch1, BYTE ch2, BOOL fDbcs)//WbFromCh(char ch)
{ /* Return word-breakness of ch */
	int ic1,ic2;

	ic1 = CharCategory(ch1, fDbcs);
	ic2 = CharCategory(ch2, fDbcs);
	if (ic1==ic2)
		return TRUE;
	else
		{
		if ((ch1=='_' && ic2==1) ||
			(ch2=='_' && ic1==1))
			return TRUE;
		else
			return FALSE;
		}
}

int CharCategory(BYTE ch, BOOL fDbcs)
{
	if (IsCharAlphaNumeric(ch) || ch=='_')
		return 0;
	else
		{
		if (ch<0x80)
			return 1;		//Symbol
		else
			{
			if (!fDbcs)
				return 0;	// USでは、AlphaNumericと同様の扱い
			else
				return 2;
			}
		}
}

int CchChFromAnsiOrOem(BYTE *rgch, int	ich, BYTE *pch, int base)
{
	char chPattern, ch;
	BOOL fAnsi;   /* whether we want to interpret as ansi */
	int	iDigit;
	int	cDigitMax;    /* max number of digits in ansi or oem code; any
		digits beyond this limit will be treated as
			normal text */

	ASSERT(base == 10 || base == 8 || base == 16);
	chPattern = rgch[ich++];
	ch = 0;

	fAnsi = (chPattern == '0');
	cDigitMax = (base == 16 ? (fAnsi ? 3 : 2):(fAnsi ? 4 : 3));     /* ansi will have a leading 0 */

	for (iDigit = 0; iDigit < cDigitMax && (chPattern = rgch[ich])!='\0' && 
			(base == 10 && FIsDigit(chPattern) ||
			base == 16 && FIsDigitX(chPattern) ||
			base == 8 && FIsDigitO(chPattern) );
			iDigit++, ich++)
		{
		if (base == 16)
			{
			if ('0' <= chPattern && chPattern <= '9')
				ch = ch * base + chPattern - '0';
			else if ('a' <= chPattern && chPattern <= 'f')
				ch = ch * base + chPattern - 'a' + 10;
			else if ('A' <= chPattern && chPattern <= 'F')
				ch = ch * base + chPattern - 'A' + 10;
			else
				ASSERT(FALSE);
			}
		else
			ch = ch * base + (chPattern - '0');
		}
	if (fAnsi || ch < chSpace)
		*pch = ch;
	else  /* interpret as Oem */
		*pch = ChAnsiFromOem(ch);

	return (iDigit);
}
char ChAnsiFromOem(char ch)
{
	char szOem[2];
	char szAnsi[2];

	szOem[0] = ch;
	szOem[1] = '\0';
	OemToAnsi((LPSTR) szOem, (LPSTR) szAnsi);
	return (szAnsi[0]);
}


#ifdef _DBCS
/* 																					*/
/*  W C H   C O N V E R T   S Z   F O R   S E A R C H 				*/
/* Description and Usage:														*/
/* this codes is used search.c only											*/
/*        translate to Single or Doble and Uper or Lower case 		*/
/*			characters.                         							*/
/*			This function call when search white							*/
/*				 ignore byte of characters.									*/
/*			so always translate Single/Double byte. 						*/
/*			and if fSearchCase is FALSE than 								*/
/*				translate Single/Double byte.									*/
/*  input  : 														*/
/*           fSearchCase = upper/lower case flg 			         */
/*           WORD *szSrc = source string	                        */
/*           WORD *szDis1 = searching pattern string					*/
/*           WORD *szDis2 = searching pattern string					*/
/*           WORD *szDis3 = searching pattern string					*/
/*           WORD *szDis4 = searching pattern string					*/
/*  returns: Converted Word Number	  		                        */
#define	MakeStrWord(w)	((WORD)(((BYTE)(HighByte(w))) | ((WORD)((BYTE)(LowByte(w)))) << 8))

#ifdef _JPN
/* Single Katakana Character define */
#define __Ka	0xb6
#define __To	0xc4
#define __Ha	0xca
#define __Ho	0xce

#define	HiraFromKata(w)	 	((WORD)(w-0xA1))
#define	KataFromHira(w)	 	((WORD)(w+0xA1))
#define	AddSonant(w)			 ((WORD)(w+0x1))
#define	AddSemiSonant(w) 		 ((WORD)(w+0x2))
#define	StripSonant(w)			 ((WORD)(w-0x1))
#define	StripSemiSonant(w) 	 ((WORD)(w-0x2))
#define	_SingleSonant				(0xde)
#define	_DoubleSonant				(0x814A)
#define	_SingleSemiSonant			(0xdf)
#define	_DoubleSemiSonant			(0x814B)


int  CGrepEngin_SIMPLE::WchConvertSzForSearch(BOOL fSearchCase, WORD *szSrc,
						WORD *szDis1, WORD *szDis2, WORD *szDis3, WORD *szDis4)
{
		WORD wchStr;			/* cuting word code buffer in String */
		WORD wchWord;			/* build codes buffer of String */
		WORD ch;					/* single byte temp buffer */
		WORD wch;				/* double bytes temp buffer */
		BOOL fDbcs;				/* Dbcs character flg */
		WORD *szDisFirst = szDis1;	/* first scan point */
		WORD *szSpec=szSrc;		/* special character save pointer */


		while(TRUE)
			{
			if (!(wchStr=wchWord=*szSrc++))	 /* if NULL = break */
				break;
			if (ch=HighByte(wchStr))					/* Special switch code? */
				{
				*szSpec++=wchStr;
				if (ch==wSpecialNL)
					*szDis1++ = chReturn;
				else
					szDis1++;
				szDis2++; szDis3++; szDis4++;
				continue;						/* not save codes to szDis1 -- 4 */
				}
			else if (IsDBCSLeadByte(LowByte(wchStr)))	/* Dbcs character ? */
				{
				//Assert(FIsDbcs2B(*szSrc));
				wchStr = MakeWord(wchStr,*szSrc);	
				wchWord = MakeWord(*szSrc++,wchWord);
				fDbcs = TRUE;					/* indicate Double byte character*/
				}
			else
				fDbcs = FALSE;				/* indicate Single byte character*/


	
			/*	case _Knj: */				/* Kanji	*/
			if (FKanjiWch(wchWord))
				{	/* save only 1 character */
				*szDis1++=wchStr;	
				szDis2++; szDis3++; szDis4++;
				*szSpec++=0;
				}

			/*	case _Num: */				/* Number	*/
			else if (FDigitWch(wchWord))
				{	/* save single and double characters */
				if (fDbcs)
					{
					*szDis1++=WchSingle(wchWord);
					*szDis2++=wchStr;
					}
				else
					{
					*szDis1++=wchStr;
					wch = WchDouble(wchWord);
					*szDis2++=MakeStrWord(wch);
					}
				szDis3++;
				szDis4++;
				*szSpec++=0;
				}

			/*	case _Alp: */				/* Alphabet	*/
			else if (FAlphaWch(wchWord))
				{	/* save single and double and upper/lower characters */
				if (fDbcs)
					{
					*szDis1++=ch=WchSingle(wch=wchWord);
					*szDis2++=wchStr;
					}
				else
					{
					*szDis1++=ch=wchStr;
					wch = WchDouble(wchWord);
					*szDis2++=MakeStrWord(wch);
					}
				if (fSearchCase)
					{
					*szDis3++=WchChangeCase(ch);
					wch=WchChangeCase(wch);
					*szDis4++=MakeStrWord(wch);
					}
				else
					{
					szDis3++;
					szDis4++;
					}
				*szSpec++=0;
				}
			/* !!! Katakana check must be befor katakana check !!! */
			/* case _Snt: */				/* Sonant */
			/* case _Sem: */				/* Semi-sonant */
			else if (FDakutenWch(wchWord) | FHandakutenWch(wchWord))
				{
				ch=(szDis1>szDisFirst) ? *(szDis1-1):0;
				if (FDakutenWch(wchWord))
					{
					if (ch>= __Ka && ch <= __To || ch>= __Ha && ch <= __Ho)
						{
						*(szDis1-1) = MakeWord(ch, chDakuten);
						wch = AddSonant(WchDouble(ch));
						*(szDis2-1) = MakeStrWord(wch);
						}
					else
						{
						*szDis1++=_SingleSonant;
						*szDis2++=MakeStrWord(_DoubleSonant);
						szDis3++; szDis4++;
						*szSpec++=0;
						}
					/* befor single character save */
					}
				else
					{
					if (ch>= __Ha && ch <= __Ho)
						{
						*(szDis1-1) = MakeWord(ch, chHandakuten);
						wch = AddSemiSonant(WchDouble(ch));
						*(szDis2-1) = MakeStrWord(wch);
						}
					else
						{
						*szDis1++=_SingleSemiSonant;
						*szDis2++=MakeStrWord(_DoubleSemiSonant);
						szDis3++; szDis4++;
						*szSpec++=0;
						}
					/* befor single character save */
					}
				}
			/*	case _Hiragana: */		/* Hiragana */
			/*	case _Kta: */				/* 2 byte Katakana */
			else if (FHiraganaWch(wchWord) || FKatakanaCharWch(wchWord))
				{	/* save single and double characters */
				if (fDbcs)
				{								/* Hiragana or 2 byte Katakana */
				/* Sonant or Semi-Sonant ari ? */
				BOOL fKatakana;
				if (FKatakanaWch(wchWord))
					{
					fKatakana = TRUE;
					wch = WchSingle(wchWord);
					}
				else
					{
					fKatakana = FALSE;
					wch = WchSingle(KataFromHira(wchWord));
					}
				if( wch>0xff )
					{
					*szSpec++=0;
					szDis3++;
					szDis4++;
					/* single strip (Semi-)Sonant character */
					if (fKatakana)
						{
						//szSpec=
						//szDis4=
						//szDis3=　
						//szDis2=ガ
						//szDis1=ｶﾞ
						*szDis1++=MakeStrWord(wch);
						*szDis2++=wchStr;
						}
					else
						{
						//szSpec=
						//szDis4=
						//szDis3=
						//szDis2=
						//szDis1=が
						*szDis1++=wchStr;
						*szDis2++=0;
						}
					}
				else
					{
					if (fKatakana)
						{
						*szDis1++=wch; 		/* wch is saved single code */
						*szDis2++=wchStr;
						}
					else
						{
						*szDis1++=wchStr;
						szDis2++;
						}
					szDis3++; szDis4++;
					*szSpec++=0;
					}
				}
				else
				{
				/* 1 byte Katakana */
				*szDis1++=wchStr;
				wch = WchDouble(wchWord);
				*szDis2++= MakeStrWord(wch);
				szDis3++; szDis4++;
				*szSpec++=0;
				}
				} /* Katakana if { } */
			/* case _Sym: */				/* Symbol	*/
			else
				{
				if (fDbcs)
					{
					wch = WchSingle(wchWord);
					if (wch && wch != wchWord)
						{
						*szDis1++=wch;
						*szDis2++=wchStr;
						}
					else
						{
						*szDis1++=wchStr;
						szDis2++;
						}
					}
				else
					{
					*szDis1++=wchStr;
					wch = WchDouble(wchWord);
					if (wch && wch != wchWord)
						*szDis2++=MakeStrWord(wch);
					else
						szDis2++;
					}
				szDis3++; szDis4++;
				*szSpec++=0;
				}
			}
			return (int)(szDis1-szDisFirst);	 /* convert characters byte */
}


#else

int  CGrepEngin_SIMPLE::WchConvertSzForSearch(BOOL fSearchCase, WORD *szSrc,
						WORD *szDis1, WORD *szDis2, WORD *szDis3, WORD *szDis4)
{
	WORD wchStr;			/* cuting word code buffer in String */
	WORD wchWord;			/* build codes buffer of String */
	WORD wch;				/* double bytes temp buffer */
	BOOL fDbcs;				/* Dbcs character flg */
	WORD *szDisFirst = szDis1;	/* first scan point */
	WORD *szSpec=szSrc;		/* special character save pointer */


	while(TRUE)
		{
		if (!(wchStr=wchWord=*szSrc++))	 /* if NULL = break */
			break;
		if (HighByte(wchStr))					/* Special switch code? */
			{
			*szSpec++=wchStr;
			szDis1++; szDis2++; szDis3++; szDis4++;
			continue;						/* not save codes to szDis1 -- 4 */
			}
		else if (IsDBCSLeadByte(LowByte(wchStr)))	/* Dbcs character ? */
			{
			//Assert(FIsDbcs2B(*szSrc));
			wchStr = MakeWord(wchStr,*szSrc);	
			wchWord = MakeWord(*szSrc++,wchWord);
			fDbcs = TRUE;					/* indicate Double byte character*/
			}
		else
			fDbcs = FALSE;				/* indicate Single byte character*/

		*szSpec++=0;


		if (fDbcs)
			{
			wch = WchSingle(wchWord);
			if (wch && wch != wchWord)
				{
				*szDis1++=wch;
				*szDis2++=wchStr;
				wch = wchWord;			/* for Alphabet */
				}
			else
				{
				*szDis1++=wchStr;
				szDis2++;
				}
			}
		else
			{
			*szDis1++=wchStr;
			wch = WchDouble(wchWord);
			if (wch && wch != wchWord)
				*szDis2++=MakeStrWord(wch);
			else
				szDis2++;
			}
		if (FAlphaWch(wchWord) && fSearchCase)
			{
			*szDis3++=WchChangeCase(*(szDis1-1));
			wch=WchChangeCase(wch);			/* use saved wch */
			*szDis4++=MakeStrWord(wch);
			}
		else
			{
			szDis3++;
			szDis4++;
			}
		}

	return (int)(szDis1-szDisFirst);	 /* convert characters byte */
}
#endif /* _JPN */



#endif /* _DBCS */
