///////////////////////////////////////////////
//TextCash.cpp

#include "StdAfx.h"
#include "MainFrm.h"
#include "TextCash.h"
#include "ViewEnv.h"
#include "chara.h"
#include "hint.h"
#include "The.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CTheApp theApp;
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////
//CByteCash class
CByteCash::CByteCash()
{
	m_off = 0;
	m_size = 0;
	m_count_lock = 0;
	m_pdata = null;
	m_bABLS_BEF = false;
}

CByteCash::~CByteCash()
{
	if (m_pdata)
		delete m_pdata;
}

bool CByteCash::Load(CObjCashFile *pObjCashFile, bool bNoSeek)
{
	assert(!IsEnable());
	OBJSIZE len;
	if (pObjCashFile->InitBlockRead(m_pdata)<=0) {
		assert(false);
		return false;
	}
	try{
		len = pObjCashFile->ReadBlock(m_pdata, bNoSeek ? NPOS : m_off); //takaso 2014.05.18
		if (len != m_size) {
			assert(false);
			Unload();
			return false;
		}
		m_bABLS_BEF = true;
		return true;
	}
	catch (exception &e) {
		TraceException(e);
		assert(false);
		Unload();
		return false;
	}
	m_bABLS_BEF = true;
	return true;
}
void CByteCash::CopyData(BYTE *pdata, OBJOFF size)
{
	m_bABLS_BEF = false;
	assert(pdata && size>0 && !IsEnable());
	m_pdata = new BYTE[size+1];
	assert(m_pdata);
	memcpy(m_pdata, pdata, size);
	m_off = 0;
	m_size = size;
	m_pdata[m_size] = '\0';
}
void CByteCash::replace(OBJOFF src_first, OBJOFF src_last, BYTE *pbytes, OBJSIZE len)
{
	assert(IsEnable());
	assert(src_first!=NPOS && src_last!=NPOS);
	assert(src_first<=src_last);
	assert(src_last<=m_size);
	OBJSIZE lenSrc = src_last - src_first;
	BYTE *pdataNew = new BYTE[m_size+(len - lenSrc)+1];
	memcpy(pdataNew, m_pdata, src_first);
	memcpy(pdataNew+src_first, pbytes, len);
	memcpy(pdataNew+src_first+len, m_pdata+src_last, m_size - src_last);
	delete m_pdata;
	m_pdata = pdataNew;
	m_size = m_size+(len - lenSrc);
	m_pdata[m_size] = '\0';
}

///////////////////////////////////////////////
//CPosCash class
CPosCash::CPosCash()
{
	m_pbytes = null;
	m_off = 0;
	m_size = 0;
	m_pdata = null;
}

CPosCash::~CPosCash()
{
	if (m_pdata)
		delete m_pdata;
}
void CPosCash::Reset()
{
	if (IsEnable())
		delete m_pdata;
	m_pbytes = null;
	m_off = 0;
	m_size = 0;
	m_pdata = null;
}

void CPosCash::Initialize(CByteCash *pbytes, OBJOFF off, OBJSIZE size)
{
	assert(size>0);
	if (IsEnable())
		delete m_pdata;
	m_pbytes = pbytes;
	m_off = off;
	m_size = size;
	m_pdata = new OBJOFF[size+1];	//+1は、EntrySell()の為
	memset(m_pdata, 0, sizeof(OBJOFF)*(size+1));	//initial 0 clear! 
	//HOW TO USE:各バイトオフセットに桁が入る（１～N）
	//・取得したオフセット位置のPOSキャッシュデータを、
	//　sell.D.xに格納する場合は－１する事。
	//・対象バイトオフセット位置のデータが0の場合は、
	//　POSキャッシュデータの設定を行う事
	//・漢字２バイト目は、0を入れる。
}

OBJOFF CPosCash::FindOffset_Of_Keta(OBJOFF off, OBJOFF &keta, int iPrevfit, bool &bNextRow, bool bFree)
{
	//1234\n
	assert(IsValidOffset(off));
	OBJSIZE size = m_size;
	assert(size>0);

	OBJOFF *p_first = GetDataPtr(off);
	OBJOFF *p = p_first;
	
	if (bFree && iPrevfit==SETSEL_NEXTCHAR) //フリーカーソルは、SETモードにする
		iPrevfit=SETSEL_SET;

	if (iPrevfit==SETSEL_SET) {
		for(; (size>0 && *p!=NPOS && (*p==0 || *p<keta)); p++,size--);
		if (size==0) { //行末が改行でない場合、それを超えたクリック
			//行末文字右に置く
			p--;
			if (bFree) {
				if (keta==NPOS)//NPOS以外は、桁はそのまま
					keta = *p;
				return off + p - p_first +1;
			}
		} if (*p==NPOS) { //改行を超えてクリック
			//改行直前にカーソルを置く
			for(p--; (p>=p_first && *p==0); p--);
			if (bFree) {
				if (keta==NPOS)//NPOS以外は、桁はそのまま
					keta = *p;
				return off + p - p_first +1; //桁はそのまま
			}
			if (p<p_first) { //改行/EOF だけの行の場合
				keta = 0;
				return off;
			}
		}
		//TRACE("pos=0x%x, keta=%d, off=0x%x\n", (m_size-size), *p, off + p - p_first +1);
	}else if (iPrevfit==SETSEL_PREVCHAR) {
		//逆方向からのスキャン
		if (keta==NPOS){
			p += (size-2);//最終文字の直前から
			size--;
		}else{
			p += (size-1);//最終文字から
		}
		for(; (size>0 && (*p==0 || *p>keta)); p--,size--);
		if (size==0) { //行頭への移動の場合
			keta = 0;
			return off;
		}
		//TRACE("pos=0x%x, keta=%d, off=0x%x\n", (m_size-size), *p, off + p - p_first +1);
	}else if (iPrevfit==SETSEL_NEXTCHAR){
		for(; (size>0 && *p!=NPOS && (*p==0 || *p<keta)); p++,size--);
		//TRACE("pos=0x%x, keta=%d, off=0x%x\n", (m_size-size), *p, off + p - p_first +1);
		if (size==0) {
			//次の行へトライ
			bNextRow = true;
			//次の行がない場合を考慮して
			p--;//行末文字右に置く
		} if (*p==NPOS || IsLastOffsetOfRow(off + p - p_first)) { //改行をみつけた
		//}else if (IsLastOffsetOfRow(off + p - p_first)) {
			//次の行へトライ
			bNextRow = true;
			//次の行がない場合を考慮して
			//改行直前にカーソルを置く
			for(p--; (p>=p_first && *p==0); p--);
			if (p<p_first) { //改行/EOF だけの行の場合
				keta = 0;
				return off;
			}
		}
	}

	keta = *p;
	return  off + p - p_first +1;
}
OBJOFF CPosCash::IsLastOffsetOfRow(OBJOFF off)
{
	assert(IsValidOffset(off));
	OBJSIZE size = m_size - (off - m_off) -1;
	OBJOFF *p=GetDataPtr(off);
	OBJOFF keta;
    for(keta=*p++; (size>0 && *p==0); p++,size--);
	return (size==0 || *p<=keta);
}
#ifdef _DEBUG
void CPosCash::Dump()
{
	OBJSIZE size = m_size;
	OBJOFF *p = GetDataPtr(m_off);
	TRACE("off=0x%x, size=0x%x\n",m_off, m_size);
	CString str, strT;
	while(size--) {
		strT.Format("%02x ",*p++);
		str += strT;
	}
	str += "\n";
	TRACE(str);
}
#endif
///////////////////////////////////////////////
//CTextCashTbl class
CTextCashTbl::CTextCashTbl()
{
	m_pchBuf = null;
	m_ocf = OCF_TEXT;
	m_pFormatChars_Loader = null;
	m_iEateNextChar = 0;
#ifdef _VERIFY
	m_iCall=0;
#endif
}
CTextCashTbl::~CTextCashTbl()
{
	if (m_pchBuf)
		delete m_pchBuf;
	for (TEXTBLOCK_IDX i=0; i<size(); i++)
		delete at(i);
}

OBJSIZE CTextCashTbl::Read(BYTE *&pBuf, OBJOFF off_first)
{
	pBuf = null;	//initial null.
	TEXTBLOCK_IDX idx = off_first/m_sizeBlock;
	if (idx>=size()) {
		return 0;
	}

	CByteCash *pbytes = at(idx);
	assert(pbytes->m_off == idx*m_sizeBlock);
	OBJOFF	off = off_first%m_sizeBlock;
	if (!pbytes->IsValidOffset(off)) {
//		assert(false);
		return 0;
	}

	if (!Lock(idx)) {
		assert(false);
		return 0;
	}
	pBuf = pbytes->GetDataPtr(off);
	return pbytes->m_size - off;
}

OBJSIZE CTextCashTbl::ReadBlockCash(BYTE *&pBuf, OBJOFF off_first, CByteCash *pbytes)
{
	if (pbytes) { //このパラメータがNullでない場合は、１ブロックには必ず１論理行の終わりまでは行っている。
		pBuf = pbytes->GetDataPtr(off_first);
		return pbytes->m_size - off_first;
	}
	return Read(pBuf, off_first);
}
CPosCash *CTextCashTbl::ReadLineCash_With_PosCash(BYTE *&pBuf, OBJOFF off_first, CByteCash *pbytes, OBJSIZE len, CFormatChars *pFormatChars)
{
	assert(pFormatChars);
	OBJSIZE lenRead = ReadLineCash(pBuf, off_first, pbytes);
	assert(lenRead>=len);
	assert(pbytes || m_textLine.IsHit(off_first));//必ずキャッシュされている

	if (m_posLine.IsHit(pbytes, off_first))
		return &m_posLine;	//既に作成済みデータがある。

	if (!m_posLine.IsHit_Enable(pbytes, off_first)) {
		//新規領域作成
		m_posLine.Initialize(pbytes, off_first, len);
	}
	OBJOFF *pPos = m_posLine.GetDataPtr(off_first);
	pFormatChars->Initialize(off_first,0);
	BYTE *pch, *pchMax;
	BYTE ch, ch2;
	OBJOFF off = off_first, off_pos=0;
	SELL sell;
	OBJOFF keta=0;
	for (pch=pBuf, pchMax = pch + len, ch=*pch; pch <pchMax; ) {
		ch2 = *(++pch);
		++off;
		if (pFormatChars->EntrySell(pch, ch, ch2, sell, off)) {
			if (IsCRLF(ch))
				keta = NPOS;
			else
				keta = pFormatChars->GetSaveKeta();
		}else
			keta = pFormatChars->GetKeta();
		off_pos = off-off_first-1;
		if (off_pos < len)
			pPos[off_pos] = keta;
		ch = ch2;
	}
	ch = *(pchMax-1); 
	if (ch==VK_EOF || IsCRLF(ch))
		pPos[SIZEMAC(len)] = NPOS;
#ifdef _DEBUG
//	m_posLine.Dump();
#endif
	return &m_posLine;
}

OBJSIZE CTextCashTbl::ReadLineCash(BYTE *&pBuf, OBJOFF off_first, CByteCash *pbytes)
{
	if (pbytes) { //このパラメータがNullでない場合は、１ブロックには必ず１論理行の終わりまでは行っている。
		pBuf = pbytes->GetDataPtr(off_first);
		return pbytes->m_size - off_first;
	}
	if (m_textLine.IsHit(off_first))
		return m_textLine.Read(pBuf, off_first);
	m_textLine.Initialize(off_first);
	OBJOFF off = off_first;
	OBJSIZE len;
	BYTE *pch, *pchMax;
	bool bFindCRLF = false;
	BYTE ch=0;
	do {
		len = Read(pBuf, off);
		if (len==0)
			break;
		for (pch = pBuf, pchMax=pch+len; pch<pchMax; ) {
			ch = *pch++;
			if (IsCRLF(ch)) {
				bFindCRLF = true;
				break;
			}else if (ch=='\0'){
				//HACK!!
				*(pch-1)=0x20;
			}
		}
		m_textLine.append((char*)pBuf, pch - pBuf);
		off += len;
	}while (!bFindCRLF);
	m_textLine.SetSize();
	if (bFindCRLF) {
		//コピーする場合など、改行があれば必ずCRLFにする。
#ifdef BUG_1999_2_1 //必ずCR+NLにする
		if (ch==VK_NL)
#endif
			m_textLine[m_textLine.size()-1] = VK_RETURN;
		m_textLine += VK_NL;
	}
	m_textLine+=' ';
#ifdef _VERIFY
//		char sz[256];
//		memcpy(sz, pBuf, min(off-off_first, 255));
//		sz[min(off-off_first, 255)]='\0';
//		MessageBox(null, sz, "The", IDOK);
#endif
	return m_textLine.Read(pBuf, off_first);
}


int CTextCashTbl::Lock(TEXTBLOCK_IDX idx)
{
	assert(size()>0 && idx<size());
	CByteCash *pbytes = at(idx);
	//unloadされている！
	if (!pbytes->IsEnable()) {
		VERIFY(pbytes->Load(this));
	}
	return pbytes->Lock();
}

int CTextCashTbl::Unlock(TEXTBLOCK_IDX idx)
{
	assert(size()>0 && idx<size());
	return at(idx)->Unlock();
}
//これから読むキャッシュ分迄のエレメントを追加する
void CTextCashTbl::InitCashFiles(OBJOFF off_last)
{
	OBJSIZE off = size() * m_sizeBlock;
	TEXTBLOCK_IDX idx=size(),
		idxMax = DIVMOD(off_last, m_sizeBlock);
	CByteCash *pbytes;
	do {
		pbytes = new CByteCash;
		assert(pbytes);
		pbytes->m_off = off;
		pbytes->m_size = m_sizeBlock;
		push_back(pbytes);
		off += m_sizeBlock;
	}while(++idx<idxMax);

	//最終エレメントのサイズ調整
	if (off>m_size) {
		pbytes->m_size = m_size - (off-m_sizeBlock); // +1 is for VK_EOF
	}
}

bool CTextCashTbl::FormatObj(bool bfirst, CLineHint *pHint, bool bfastReturn)
{
	bool bChange = false;
	if (bfirst || !m_bFormatAll) {
		if (IsOpenFile()) {
			bChange = FormatObj_Loader(bfirst, pHint, bfastReturn);
		}else{
			SELL sell = SELL(this, 0, 1, 1);
			//Win95 コンパイラエラー VC\INCLUDE\xstring(40) : fatal error C1001: 内部ｺﾝﾊﾟｲﾗ ｴﾗｰ
			//(ｺﾝﾊﾟｲﾗ ﾌｧｲﾙ 'msc1.cpp', 行番号 1188) change string str(1,(char)VK_EOF);->append();
			//string str(1,(char)VK_EOF);
			string str;
			str.append(1,(char)VK_EOF);
			sell.pbytes = NewElement(str.c_str(), str.size());
			pHint->push_back(sell);
			m_bFormatAll = true;
			bChange = true;
		}
	}
	return bChange;
}

bool CTextCashTbl::FormatObj_Loader(bool bfirst, CLineHint *pHint, bool bfastReturn)
{
	assert(bfastReturn);
	assert(GetEnv());

	//CByteCash *pbytes;
	OBJOFF	off,off_scan;
	OBJSIZE len;
	BYTE *pch, *pchMax;
	BYTE ch, ch2;
	SELL sell;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	HWND hWnd = null;
	if (pMainFrame)
		hWnd = pMainFrame->m_hWnd;

	if (bfirst) {
#ifdef _VERIFY
		assert(m_iCall==0);
#endif
		m_bFormatAll = false;
		off=0;
		m_iEateNextChar = 0;
		InitCashSizes();
		InitCashFiles(off+m_sizeBlock);
		if (!at(0)->Load(this)) {
			m_bFormatAll = true;
			assert(false);
			return false;
		}
		assert(	m_pFormatChars_Loader == null);
		m_pFormatChars_Loader = m_pViewEnv->NewFormatChars();
	}
	assert(	m_pFormatChars_Loader);

	//if (InitCashRead(m_pchBuf)<=0)
	//	return false;
	if (bfastReturn) {
		off = m_pFormatChars_Loader->Pop_ReadOffset();
	}
	sell.ptbl = this;
	while ((len = bfirst ? at(0)->Read(m_pchBuf, off):
							ReadCash(m_pchBuf, off))>0) {
		// 0,1,2,(3)
		if (!bfirst) {
			if (MyPeekMessage(hWnd)) {
				len=0;
				break;
			}
			InitCashFiles(off+len);
			if (MyPeekMessage(hWnd)) {
				len=0;
				break;
			}
		}

		off_scan = off;
		if (m_iEateNextChar) {//前回先読みしたバイト分先に進める。
			off_scan += m_iEateNextChar;
		}
		OBJOFF off_indicator_lim = (m_numBlock==1 ? NPOS:m_size/20),
				off_idicator = off_scan;
		for (pch = m_pchBuf + m_iEateNextChar + (bfirst ? 0:ABLS_BEF),
			pchMax = m_pchBuf + len + (bfirst ? 0:ABLS_BEF),
							ch = *pch; pch <pchMax; ) {
			ch2 = *(++pch);
			++off_scan;
			if (m_pFormatChars_Loader->EntrySell(pch, ch, ch2, sell, off_scan)) {
				//sell.pbytes = pbytes;
#ifdef _VERIFY
				if (m_iCall==0) {
					assert(sell.pbytes==null);
					assert(sell.off==0);
					//char sz[256];
					//memcpy(sz, m_pdata, min(len, 255));
					//sz[min(len, 255)]='\0';
					//MessageBox(null, sz, "The", IDOK);
				}
				assert(++m_iCall);
#endif
				pHint->push_back(sell);
				//状況表示
				if (off_scan - off_idicator>off_indicator_lim && m_numBlock>1 && pMainFrame) {
					if (theApp.m_bDestroy)
						return false;
					pMainFrame->SetPaneText_Loading(off_scan, m_size);
					off_idicator = off_scan;
				}
				if (!bfirst)
					if (MyPeekMessage(hWnd)) {
					//	len=0;
					//	break;
						return false;
					}
			}
			ch = ch2;
		}

		//set bytes size
		off += len;
		if (off_scan != off) {
			//assert(off_scan>off);
			m_iEateNextChar = off_scan-off;//前回先読みしたバイト分先に進める。
		}else
			m_iEateNextChar = 0;

			//if (!bfirst && !m_file.eof()) {
		//delete m_pchBuf;
			//}
		//m_pchBuf = null; //entory 済み
		if (bfastReturn) {
			m_pFormatChars_Loader->Push_ReadOffset(off);
			break;
		}
		//if (InitCashRead(m_pchBuf)<=0)
		//	return false;
	}

	//if (m_pchBuf) {
	//	delete m_pchBuf;
	//}


	if (/*m_file.eof() ||*/ off>=m_size) {
		if (!bfirst && m_pchBuf) {
			delete m_pchBuf;
		}
		if (m_pFormatChars_Loader->EntryLastSell(sell, m_size)) {
			if (m_iEateNextChar>0)
				sell.len -= m_iEateNextChar;
			pHint->push_back(sell);
		}
		m_pchBuf = null;
		delete m_pFormatChars_Loader;
		m_pFormatChars_Loader = null;
		m_bFormatAll = true;	//正常にフォーマットし終わったものとする。
								//異常終了時は、リトライしない。
		//状況表示
		if (pMainFrame && m_numBlock>1)
			pMainFrame->SetPaneText_Loading(off, m_size);
	}else{
		if (bfirst) {
			m_pchBuf = null;
			VERIFY(InitCashRead(m_pchBuf));
		}
	}

	return true;
}

CByteCash *CTextCashTbl::NewElement(const char *sz, OBJSIZE len)
{
	CByteCash *pByteCash = new CByteCash;
	//実メモリへの行登録
	pByteCash->CopyData((BYTE*)sz, len);
	push_back(pByteCash);
	return pByteCash;
}

