// Layout.cpp : Layout クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <mbstring.h>
#include "The.h"

#include "Hint.h"
#include "ViewEnv.h"
#include "TextCash.h"
#include "action.h"
#include "chara.h"
#include "MainFrm.h"
#include "ViewEd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// static proc
static int FindToTopProc(const string &str, void *pv, OBJOFF &off_find)
{
	find_data *pdata = (find_data*)pv;
	if (pdata->bCase ? _mbschr((BYTE*)str.data(), pdata->chUpper):strchr2((BYTE*)str.data(), pdata->chUpper, pdata->chLower)) {
		if (!pdata->bCase)
			_mbsupr((BYTE*)str.data());
		//if ((off_find = str.find(pdata->strFind.data()))!=string::npos && !IsDBCSTrailByte(str.data(), str.data()+off_find)) {
		OBJOFF off=0;
		off_find = string::npos;
		while((off = findStr(str, pdata->strFind, off))!=string::npos) {
			off_find = off;
			off++;
		}
		if (off_find != string::npos)
			return pdata->strFind.size();	//find size retrun!
	}
	return 0; //not find
}
static int FindToEndProc(const string &str, void *pv, OBJOFF &off_find)
{
	find_data *pdata = (find_data*)pv;
	if (pdata->bCase ? _mbschr((BYTE*)str.data(), pdata->chUpper):strchr2((BYTE*)str.data(), pdata->chUpper, pdata->chLower)) {
		if (!pdata->bCase)
			_mbsupr((BYTE*)str.data());
		//if ((off_find = str.find(pdata->strFind.data()))!=string::npos && !IsDBCSTrailByte(str.data(), str.data()+off_find)) {
		if ((off_find = findStr(str, pdata->strFind))!=string::npos) {
			return pdata->strFind.size();	//find size retrun!
		}
	}
	return 0; //not find
}
/////////////////////////////////////////////////////////////////////////////
//CBookMark
void CBookMark::SetMark(CViewEd *pviewed, BMARK bmark)
{
	CUR cur = pviewed->m_cursor.GetBlinkCursor();
	m_row = cur.D.y;
	m_col = cur.D.x;
	OBJOFF rowLine = pviewed->GetHint()->GetThis_LineTop(m_row);
	m_line = pviewed->GetHint()->at(rowLine).line;
	m_bmark = bmark;
}
bool CBookMark::JumpMark(CViewEd *pviewed) 
{
	if (m_line==0)
		return false;
	CCursor	cursor;
	OBJOFF rowJump;
	OBJOFF rowLine = pviewed->GetHint()->GetThis_LineTop(m_row);
	if (m_line == pviewed->GetHint()->at(rowLine).line) {
		rowJump = m_row;
		return pviewed->JumpLine(rowJump+1, JUMP_ROW/*takaso2010 bugfix true-> */, m_col); //speed upの為,rowﾓｰﾄﾞでジャンプする
	}
	return pviewed->JumpLine(m_line, JUMP_LINE/*takaso2010 bugfix false-> */, m_col);
}
int	comp(const CBookMark &mark1, const CBookMark &mark2) {
	return mark1.m_line!=mark2.m_line ? (mark1.m_line>mark2.m_line ? 1:-1):
		(mark1.m_row!=mark2.m_row ? (mark1.m_row>mark2.m_row ? 1:-1):0);
}
CBookMark& CBookMark::operator =(const CBookMark &mark)
{
	m_line = mark.m_line;
	m_row = mark.m_row;
	m_col = mark.m_col;
	m_bmark = mark.m_bmark;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
//CBookMarks
CBookMarks::CBookMarks()
{
	//予約マークの挿入
//	CBookMark mark;
//	mark.m_bmark = bmark_flp;
//	push_back(mark);
//	mark.m_bmark = bmark_input;
//	push_back(mark);
}
bool CBookMarks::SetMark(CViewEd *pviewed, CBookMark *pmark, BMARK bmark)
{
	CBookMark mark;
	if (!pmark) {
		mark = CBookMark(pviewed, bmark);
		pmark = &mark;
	}
	if (bmark!=bmark_normal)
		RemoveUnnormal(bmark);

	for(iterator iter=begin(); iter<end(); iter++) {
		int cmp = comp(*pmark, *iter);
		if (cmp<0 || (bmark!=bmark_normal && cmp==0)) {
			insert(iter, *pmark);
			return true;
		}else if (cmp==0 && bmark==bmark_normal) {//ノーマルmark
			erase(iter); //削除
			return false; //alrady set remove mark!
		}
	}
	push_back(*pmark);
	return true;
}
void CBookMarks::RecalcPosition(OBJOFF line_first, OBJOFF line_num_del, OBJOFF line_num_ins, 
					OBJOFF row_first, OBJOFF row_num_del, OBJOFF row_num_ins) 
{
	assert(row_num_del>0 ? row_num_del>=line_num_del:true);
	assert(row_num_ins>0 ? row_num_ins>=line_num_ins:true);
	iterator iterdel_first = end();
	bool bDel = line_num_del > line_num_ins && row_num_del > row_num_ins;
	OBJOFF rowDel = bDel ? (row_num_del-row_num_ins):0;
	OBJOFF countDel = 0;
	for(iterator iter=begin(); iter<end(); iter++) {
		OBJOFF line = (*iter).m_line;
		OBJOFF row = (*iter).m_row;

		if (row_num_ins && line >= line_first) {
			(*iter).m_line += line_num_ins;
			if (row >= row_first) {
				(*iter).m_row += row_num_ins;
			}
		}
		if(row_num_del && row >= row_first) {
			if (bDel && row < (row_first + rowDel)) {
				if (iterdel_first >= end()) {
					iterdel_first = iter; //最初の削除項目を記憶
				}
				countDel ++ ;
			}else {
				(*iter).m_line -= line_num_del;
				(*iter).m_row -= row_num_del;
			}
		}
	}
	//単純に削除だけの場合。
	if (countDel>0)
		erase(iterdel_first, iterdel_first + countDel);
}
bool CBookMarks::JumpPrev(CViewEd *pviewed) 
{
	CBookMark mark(pviewed);
	iterator iterJump = end();
	for(iterator iter=begin(); iter<end(); iter++) {
		int cmp = comp(*iter, mark);
		if (cmp>=0) {
			break;
		}else if (cmp<0 && (*iter).m_bmark==bmark_normal)
			iterJump = iter;
	}
	if (iterJump >=end() && size()>0) {
		iterJump = begin() + (size() -1);
		while((*iterJump).m_bmark!=bmark_normal) {
			if (iterJump>begin()) 
				iterJump--; //次の候補
			else {
				iterJump = end();
				break;
			}
		}
	}
	if (iterJump < end())
		return (*iterJump).JumpMark(pviewed);
	return false;
}
bool CBookMarks::JumpNext(CViewEd *pviewed) 
{
	CBookMark mark(pviewed);
	iterator iterJump = end();
	for(iterator iter=begin(); iter<end(); iter++) {
		int cmp = comp(*iter, mark);
		if (cmp>0 && (*iter).m_bmark==bmark_normal) {
			iterJump = iter;
			break;
		}
	}
	if (iterJump >=end() && size()>0) {
		iterJump = begin();
		while((*iterJump).m_bmark!=bmark_normal) {
			if (iterJump<end()) 
				iterJump++; //次の候補
			else {
				iterJump = end();
				break;
			}
		}
		//if ((*iterJump).m_bmark!=bmark_normal) 
		//	iterJump++; //次の候補
	}
	if (iterJump < end())
		return (*iterJump).JumpMark(pviewed);
	return false;
}
bool CBookMarks::JumpUnnormalMark(CViewEd *pviewed, BMARK bmark) 
{
//	CBookMark mark(pviewed, bmark_flp /*takaso2010 bugfix true-> */); //現在位置を記憶
	bool b=false;
	for(iterator iter=begin(); iter<end(); iter++) {
		if ((*iter).m_bmark==bmark) {
			b = (*iter).JumpMark(pviewed);
//			if (bmark==bmark_flp)
//				erase(iter); //削除
			break;
		}
	}
//	if (bmark==bmark_flp)
//		SetMark(pviewed, &mark, bmark_flp /*takaso2010 bugfix true-> */); //新しい位置を登録
	return b;
}
CBookMark CBookMarks::RemoveUnnormal(BMARK bmark)
{
	CBookMark mark;
	for(iterator iter=begin(); iter<end(); iter++) {
		if ((*iter).m_bmark==bmark) {
			mark = *iter; 
			erase(iter);
			break;
		}
	}
	return mark;
}
void CBookMarks::RemoveAll_normal()
{
	CBookMarks marks;
	marks.clear();	//予約マークを削除しておく
	for(iterator iter=begin(); iter<end(); iter++) {
		if ((*iter).m_bmark != bmark_normal)
			marks.push_back(*iter); 
	}
	//erase(begin(), end());
	clear();
	*this = marks;
}
/////////////////////////////////////////////////////////////////////////////
//SELLS
//src->des!
void SELLS::replace(SELLS &sells_ins, /*SELLS::iterator iter_src_first, SELLS::iterator iter_src_last,*/
							SELLS::iterator iter_des_first, SELLS::iterator iter_des_last)
{
	SELLS::iterator iter_src_first;
	SELLS::size_type len_src = sells_ins.size();//iter_src_last - iter_src_first;
	SELLS::size_type len_des = iter_des_last - iter_des_first;

	OBJOFF row_des_first = iter2row(iter_des_first);
	OBJOFF row_copy = min(len_src,len_des);
	if (row_copy>0) {
		iter_src_first = sells_ins.begin();
		iter_des_first = copy(iter_src_first, iter_src_first + row_copy, iter_des_first);
	}
	if (len_src<len_des) {
		iter_des_first = row2iter(row_des_first+row_copy);
		//erase(iter_des_first, iter_des_last);
		assert(begin()+size() == end());
		assert(iter_des_first == (begin()+(row_des_first+row_copy)));
		if (begin()+size() == end()) {
			erase(iter_des_first, iter_des_first+(len_des-len_src));
			assert(begin()+size() == end());
		}
	}else if (len_src>len_des) {
		iter_des_first = row2iter(row_des_first+row_copy);
		assert(begin()+size() == end());
		insert(iter_des_first, sells_ins.begin() + row_copy, sells_ins.end());
		assert(begin()+size() == end());
	}
	
}

OBJOFF SELLS::RecalcLineNum(OBJOFF row_first/*=NPOS*/, OBJOFF row_last/*=NPOS*/, OBJOFF line_check/*=0*/)
{
//TODO:Speed Up
	if (row_first != NPOS) {
		m_rowMax_Calced_LineNum = row_first;
		assert(row_last!=NPOS);
	}else
		row_last = size();

	if (m_rowMax_Calced_LineNum >= size())
		return m_rowMax_Calced_LineNum;
	OBJOFF row = GetPrev_LineTop(m_rowMax_Calced_LineNum);
	OBJOFF line = row==NPOS ? 0:at(row).line;
	if (line==0) {
		m_rowMax_Calced_LineNum = 0;
	}

	do {
		for (; m_rowMax_Calced_LineNum<row_last;
				m_rowMax_Calced_LineNum = GetNext_LineTop(m_rowMax_Calced_LineNum)) {
			SELL *pSell = &(at(m_rowMax_Calced_LineNum));
			assert(pSell->line!=0);
			pSell->line = ++line;
		}
		row_last = size();
	}while (row_first!=NPOS && line_check != (line+1) && m_rowMax_Calced_LineNum<size());
	//チェック範囲の次の行番号が合わなければ、残りを引き続き実施する
	return m_rowMax_Calced_LineNum;
}
OBJOFF SELLS::FindLine(OBJOFF line_hit, SELL &sellLgc)
{
//speed up!
	OBJOFF row_last = size();
	int line = 0;
	for (OBJOFF row=0; row<row_last; row = GetNext_LineTop(row)) {
		line = at(row).line;
		if (line == line_hit) {
			sellLgc = at(row);
			return row;
		}
	}	
	assert(line<line_hit);
	return NPOS; //見つからない
}

OBJOFF SELLS::FindByte(OBJOFF byte_hit, SELL &sellLgc)
{
//speed up!
	OBJOFF row_last = size();
	int line = 0, row_befor=0;
	for (OBJOFF row=0; row<row_last; row = GetNext_LineTop(row)) {
		sellLgc = at(row);
		if (sellLgc.pbytes == null && (/*sellLgc.off <= byte_hit &&*/
			(sellLgc.off+sellLgc.len) > byte_hit)) {
			if (row>0 && sellLgc.off > byte_hit)
				row = row_befor; //takaso2010 //row--;
			return row;
		}
		row_befor=row; //takaso2010
	}	
	return NPOS; //見つからない
}

OBJOFF SELLS::FindRow(const SELL &sellLgc, OBJOFF row)
{
//TODO:Speed Up,Neweeton method!
	//unsigned long a = max_size(); //214,748,364
	for ( ; row<size(); row++) {
		if (at(row).IsInSell_Lgc(sellLgc))
			break;
	}
	if (row==size())
		return NPOS; //見つからない
	assert(row<size());
#ifdef _DEBUG
//	if (row==size()) {
//		SELL sell = at(SIZEMAC(size()));
//	}
#endif
	return row;	//指定行の直後行へのインデックスを返す
}
SELLS::iterator SELLS::FindTopLine(const SELL &sellLgc, bool bPrev, iterator iter)
{
//TODO:Speed Up,Neweeton method!
	for ( ; iter<end(); iter++) {
		if ((*iter).IsInSell_Lgc(sellLgc)) {
			iter = GetThis_LineTop(iter);
			break;
		}
	}
	assert(iter<end());
	if (bPrev)
		return iter;
	if (iter<end())
		iter++;
	for (; iter<end(); iter++) {
		SELL *pSell = &(*iter);
		if (pSell->line != 0)
				break;
	}
	if (iter>=end()) //STL bug follow !?
		iter = end();
	return iter;	//指定行の直後行へのインデックスを返す
}

OBJOFF SELLS::GetByteOffsetInCursor(const CUR &curBef, OBJOFF _row, OBJOFF off_in_this_row)
{
	OBJOFF off = 0;
	if (_row == curBef.D.y) {
		assert(off_in_this_row >= curBef.L.off);
		off = off_in_this_row - curBef.L.off;
	}else{
		off = at(curBef.D.y).off + at(curBef.D.y).len - curBef.L.off;
		for (OBJOFF row = curBef.D.y+1; row<_row; row++) {
			off += at(row).len;
		}
		off += off_in_this_row - at(_row).off;
	}
	return off;
}

string SELLS::GetPrevPartStringOfLine(const CUR &cur)
{
	string str;
	assert(cur.D.y<size());
	OBJOFF row = GetThis_LineTop(cur.D.y);
	SELL sellLgc = at(row);
	if ((cur.L.off - sellLgc.off)>0) {
		BYTE *pch;
		OBJSIZE len = sellLgc.ptbl->ReadLineCash(pch, sellLgc.off, sellLgc.pbytes);
		assert(pch);
		assert(len>=cur.L.off - sellLgc.off);
		len = cur.L.off - sellLgc.off; //copy length.
		str.append((char*)pch, len);
	}
	return str;
}

string SELLS::GetPartStringOfLine(CCursor &cursor)
{
	string str;
	CUR cur = cursor.Befor();
	OBJOFF off_end = cursor.End().L.off;
	if (off_end - cur.L.off>0) {
		assert(cur.D.y<size());
		//assert(cur.D.x<at(cur.D.y).len);
		BYTE *pch;
		OBJSIZE len = cur.L.ptbl->ReadLineCash(pch, cur.L.off, cur.L.pbytes);
		assert(AfxIsValidAddress(pch, len, true));
		assert(len>=off_end - cur.L.off);
		str.append((char*)pch, off_end - cur.L.off);
	}
	return str;
}


string SELLS::GetNextPartStringOfLine(const CUR &cur)
{
	string str;
	assert(cur.D.y<size());
	BYTE *pch;
	OBJSIZE len = cur.L.ptbl->ReadLineCash(pch, cur.L.off, cur.L.pbytes);
	assert(pch);
	//assert(len>=cur.L.len);
	str.append((char*)pch, len);
	return str;
}


void SELLS::BuildDsp2LineSells(CCursor &cursor, SELLS &sells)
{
	BuildDsp2LineSells(GetThis_LineTop(cursor.Befor().D.y),
								cursor.End().D.y+1, sells);
}

void SELLS::BuildDsp2LineSells(OBJOFF row_first, OBJOFF row_last, SELLS &sells)
{
	sells.clear();
	for (OBJOFF row=row_first; row<row_last;) {
		sells.push_back(ScanMakeLineSell(row, sells.size()+1));
	}
	//return row-cursor.Befor().D.y;	//何行スキャンしたか？
}

SELL SELLS::ScanMakeLineSell(OBJOFF &row, OBJOFF line)
{
	assert(at(row).line>0);
	SELL sell = at(row), *psell;
	OBJSIZE max = size();
	if (row<max) {
		++row;
		sell.line = line;	//行番号を入れておく
	}

	for(; row<max && (psell=&at(row))->line==0; row++)
		sell.len += psell->len;
	return sell;	//row is point to next line top.
}


SELLS::iterator SELLS::GetThis_LineTop(SELLS::iterator iter)
{
	assert(iter<end());
	for (; iter>=begin() && (*iter).line==0; iter--);
	assert(iter>=begin());
	return iter;
}


OBJOFF SELLS::GetThis_LineTop(OBJOFF row)
{
	assert(row<size());
	for (; row>=0 && at(row).line==0; row--);
	assert(row!=NPOS);
	return row;
}

OBJOFF SELLS::GetPrev_LineTop(OBJOFF row)
{
	row = GetThis_LineTop(row);
	if (row==0)
		return NPOS;	//this row is in first line.
	return GetThis_LineTop(--row);
}

OBJOFF SELLS::GetNext_LineTop(OBJOFF row)
{
	assert(row<=size());
	if (row<size())
		++row;
	for (; row<size() && at(row).line==0; row++);
	return row;	//if r==size() then this row is last line.
}
OBJSIZE SELLS::GetRows_ThisLine(OBJOFF row)
{
	OBJOFF row_first = GetThis_LineTop(row);
	OBJOFF row_last = GetNext_LineTop(row);
	assert(row_first<size());
	assert(row_first<row_last);
	return row_last - row_first;
}

bool SELLS::IsAbsFileRow(OBJOFF row, const CTextCashTbl *pEditTbl) const
{
	if (row>=size())
		return false; //メインファイル行がない
	return at(row).ptbl != pEditTbl; //エディット行でない場合
}
bool SELLS::IsMultiLinesExpand(CCursor cursor)
{
	if (!cursor.IsExpand())
		return false;
	OBJOFF rowBef, rowEnd;
	rowBef = cursor.Befor().D.y;
	rowEnd = cursor.End().D.y;
	if (rowBef==rowEnd)
		return false;
	rowBef = GetThis_LineTop(cursor.Befor().D.y);
	rowEnd = GetThis_LineTop(cursor.End().D.y);
	return rowBef!=rowEnd; 
}
/////////////////////////////////////////////////////////////////////////////
//CLineHint
CLineHint::CLineHint()
{
	m_pFormatChars = null;
}
CLineHint::~CLineHint()
{
	if (m_pFormatChars)
		delete m_pFormatChars;
}
void CLineHint::Initialize(CViewEnv *pViewEnv)
{
	if (m_pFormatChars)
		delete m_pFormatChars;
	m_pFormatChars = pViewEnv->NewFormatChars();
}

CUR CLineHint::GetCur_0()
{
	CUR cur;
	SetCur(cur);
	return cur;
}
CUR CLineHint::GetCur_NPOS()
{
	CUR cur;
	cur.D.y = NPOS;
	cur.D.x = NPOS;
	SetCur(cur);
	return cur;
}

void CLineHint::NextCur(CUR &cur, bool bFree)
{
	cur.D.x++;
	SetCur(cur, bFree, SETSEL_NEXTCHAR);
}

void CLineHint::PrevCur(CUR &cur, bool bFree)
{
	if (cur.D.x>0)
		cur.D.x--;
	else if (cur.D.y>0) {
		cur.D.y--;
		cur.D.x=NPOS;
	}
	SetCur(cur, bFree, SETSEL_PREVCHAR);
}

void CLineHint::ResetUnFreeCursor(CCursor &cursor)
{
	CUR cur = cursor.GetBlinkCursor();
	SetCur(cur, false);
	cursor.SetBlinkCursor(cur);
	if (cursor.IsExpand()) {
		cur = cursor.GetOrgCursor();
		SetCur(cur, false);
		cursor.SetOrgCursor(cur);
	}
}

void CLineHint::SetCur(CUR &cur, bool bFree, int iPrevfit, bool bExpand)
{
//	if (!bFree && size()==0)
//		return;

	if ((cur.D.y == NPOS) ||// 文章末の意味
		(cur.D.y>=size())) {//文章末以上の場合は、文章末にする 
		cur.D.y = size_mac();
	}

	//見つからない場合：指定の直前行となる
	cur.L = at_mac(cur.D.y);

	if (bFree && cur.D.y>=size()) {
		if (cur.D.x == NPOS) // 行末の意味
			cur.D.x = 0;
	}else{
		//Xの取得-行頭の場合
		if (cur.D.x == 0) { //行頭の場合
			return;
		}

		//行頭からのデータ取得
		BYTE *pch;
		OBJSIZE len=cur.L.len, off=cur.L.off;
		CPosCash *pPosLine = cur.L.ptbl->ReadLineCash_With_PosCash(pch, off, cur.L.pbytes, len, m_pFormatChars);
		bool bNextRow = false;
		OBJOFF keta = cur.D.x;
		if (cur.D.x==NPOS && bExpand)
			bNextRow = true;
		else //if (cur.D.x!=NPOS)
			off = pPosLine->FindOffset_Of_Keta(off, keta, iPrevfit, bNextRow, bFree);
		if (bNextRow && cur.D.y < SIZEMAC(size())) {
			cur.D.y++;
			cur.D.x = 0;
			cur.L = at(cur.D.y);
		}else{
			cur.D.x = keta;
			if (off==(cur.L.off+cur.L.len) && cur.D.y < SIZEMAC(size()))
				cur.L = at(cur.D.y+1);
			else
				cur.L.off = off;
		}

		/*
		SELL sell;	//EntrySell()'s dumy!
		BYTE *pch, *pchMax;
		OBJSIZE len, off=cur.L.off;
		len = cur.L.ptbl->ReadLineCash(pch, off, cur.L.pbytes);
		len = cur.L.len; //for WordWrap!
		BYTE ch,ch2;
		m_pFormatChars->Initialize(off,0);
		//１．マウスカーソルセット、上下、ページキー
		//　　その場所へ
		//２．左右キー
		//　　行末は１文字前の位置へ

		//１．行末改行の場合
		//　  　 |-----prv2
		//　  　 | |---prv
		//　  　 | |--GetSaveKeta()
		//　  　 | |　　　　|--off
		//あああＸＹ(retrun)
		//２．きっちり最大桁まである場合／ぶら下げ
		//　　 |-----prv2
		//　　 | |---prv
		//　 　| | |--GetSaveKeta()/off
		//あああＸＹ
		//３．追い出し時
		//　 　 |-----prv2
		//　 　 | |---prv
		//　 　 | |---GetSaveKeta()/off
		//aああＸＹ
		OBJOFF keta, keta_prv=0, off_prv=off, keta_prv2=0, off_prv2=off;
		for (pchMax = pch + len, ch=*pch; pch <pchMax; ) {
			ch2 = *(++pch);
			++off;
			if (m_pFormatChars->EntrySell(pch, ch, ch2, cur.L, off)) {
				if (bFree || cur.D.y==size_mac()) {
					if (IsCRLF(ch))
						cur.L.off = off_prv;
					else
						cur.L.off = off;
					if (!bFree || cur.D.x==NPOS)
						cur.D.x = m_pFormatChars->GetSaveKeta();
				}else if (iPrevfit != SETSEL_NO) {
					if (IsCRLF(ch)) {
						if (bExpand && cur.D.x==NPOS) {
							cur.D.y++;
							cur.D.x = 0;
							cur.L = at(cur.D.y);
						}else{
							cur.D.x = keta_prv;
							cur.L.off = off_prv;
						}
					}else if (m_pFormatChars->GetSaveKeta()>cur.D.x) {
						cur.D.x = keta_prv;
						cur.L.off = off_prv;
					}else{
						if (iPrevfit == SETSEL_PREVCHAR) {
							if (m_pFormatChars->GetSaveKeta()==keta_prv) {//追い出し
								cur.D.x = keta_prv2;
								cur.L.off = off_prv2;
							}else{
								cur.D.x = keta_prv;
								cur.L.off = off_prv;
							}
						}else{
							cur.L.off = off;
							if (!bFree || cur.D.x==NPOS)
								cur.D.x = m_pFormatChars->GetSaveKeta();
						}
					}
				}else{
					cur.D.y++;
					cur.D.x = 0;
					cur.L = at(cur.D.y);
				}
				return;
			}
			keta=m_pFormatChars->GetKeta();
			if (keta>=cur.D.x) {
				if (keta>cur.D.x && (iPrevfit != SETSEL_NO)) {
					cur.D.x = keta_prv;
					cur.L.off = off_prv;
				}else if (pch==pchMax && (!bFree && cur.D.x!=NPOS)) {//行末 for WordWrap
					cur.D.y++;
					cur.D.x = 0;
					cur.L = at(cur.D.y);
				}else{
					if (!bFree && ch==VK_EOF && keta>0) {
						keta--;
						assert(off>0);
						off--;
					}
					cur.D.x = keta;
					cur.L.off = off;
				}
				return;
			}
			off_prv2 = off_prv;
			keta_prv2= keta_prv;
			off_prv = off;
			keta_prv= keta;
			ch = ch2;
		}
		if (*(pchMax-1)==VK_EOF && keta>0) {
			keta--;
			assert(off>0);
			off--;
		}
		if (!bFree || cur.D.x==NPOS) {
			if (iPrevfit == SETSEL_PREVCHAR) { //for WordWrap
				cur.D.x = keta_prv2;
				cur.L.off = off_prv2;
				return;
			}else
				cur.D.x = keta;
		}
		cur.L.off = off;
		*/
	}
	return;
}

bool CLineHint::SetCur_4Lgc(CUR &cur, OBJOFF row)
{
//	if (size()==0)
//		return;

	if (cur.L.y == NPOS)	// 文章末の意味
		cur.D.y = size_mac();
	else {
		OBJOFF y = FindRow(cur.L, row);
		if (y==NPOS)
			return false;
		cur.D.y = y;
	}

		//Xの取得-行頭の場合
		if (cur.L.off == at(cur.D.y).off) { //行頭の場合
			cur.D.x = 0;
			return true;
		}

		//行頭からのデータ取得
		SELL sell;	//EntrySell()'s dumy!
		BYTE *pch, *pchMax;
		OBJSIZE len, off=at(cur.D.y).off;
		len = cur.L.ptbl->ReadLineCash(pch, off, cur.L.pbytes);
		BYTE ch,ch2;
		m_pFormatChars->Initialize(off,0);
		OBJOFF keta=0;
		for (pchMax = pch + len, ch=*pch; pch <pchMax; ) {
			ch2 = *(++pch);
			++off;
			if (m_pFormatChars->EntrySell(pch, ch, ch2, cur.L, off)) {
				if (cur.D.y < SIZEMAC(size())) {
					cur.D.y++;
					cur.D.x = 0;
					cur.L = at(cur.D.y);
				}else{
					cur.L.off = off;
					cur.D.x = m_pFormatChars->GetSaveKeta();
					cur.L = at(cur.D.y);
					break;
				}
			}
			keta=m_pFormatChars->GetKeta();
			if (off>=cur.L.off) {
				cur.D.x = keta;
				cur.L.off = off;
				break;
			}
			ch = ch2;
		}
	return true;
}

OBJOFF CLineHint::String2Keta(const string &str, bool &bOver) const
{
	SELL sell;	//EntrySell()'s dumy!
	BYTE *pch, *pchMax;
	OBJSIZE len, off=0;
	len = str.size();
	pch = (BYTE*)str.data();
	BYTE ch,ch2;
	m_pFormatChars->Initialize(off,0);
	OBJOFF keta=0;
	for (pchMax = pch + len, ch=*pch; pch <pchMax; ) {
		ch2 = *(++pch);
		++off;
		if (m_pFormatChars->EntrySell(pch, ch, ch2, sell, off)) {
			bOver = true;
			return m_pFormatChars->GetSaveKeta();
		}
		keta=m_pFormatChars->GetKeta();
		ch = ch2;
	}
	bOver = false;
	return keta;
}

//bool CLineHint::DoActionLgc(CActionLgc *pactLgc, CTextCashTbl *pEditTbl)
bool CLineHint::DoActionLgc(CActionBlock &actBlock_del, CActionBlock &actBlock_ins,
							CCursor &cursor_befor, CCursor &cursor_after, CTextCashTbl *pEditTbl)
{
	iterator iterFirst_del, iterLast_del;
	SELLS sellsDsp;
	SELLS::iterator iterFirst_ins, iterLast_ins;
	OBJOFF row_cur_hint = 0;
	CUR cur = cursor_after.GetBlinkCursor();
	bool bMultiLine = false;

	if (actBlock_del.m_sellsLgc.size()>0 || actBlock_ins.m_sellsLgc.size()>0) {
		//複数行のリプレース
		bMultiLine = true;
		if (actBlock_del.m_sellsLgc.size()>0) {
			//削除位置の取得
			iterFirst_del = FindTopLine(actBlock_del.m_sellsLgc[0], true, begin());
			iterLast_del = FindTopLine(actBlock_del.m_sellsLgc[SIZEMAC(actBlock_del.m_sellsLgc.size())], false, iterFirst_del);
		}else{
			//挿入位置の取得
			iterFirst_del = FindTopLine(cursor_befor.GetBlinkCursor().L, false, begin());
			iterLast_del = iterFirst_del;
		}
		if (actBlock_ins.m_sellsLgc.size()>0) {
			BuildLine2DspSells(actBlock_ins.m_sellsLgc.begin(), actBlock_ins.m_sellsLgc.end(), sellsDsp);
		}
	}else{
		//１行内での文字列リプレース
		SELL sellLgc_befor = cursor_befor.Befor().L;
		SELL sellLgc_end = cursor_befor.End().L;
		//選択状態ではないが、削除するものがある場合は、セルを削除状態にする
		if (!cursor_befor.IsExpand() && actBlock_del.m_string.size()>0) {
			sellLgc_befor.off -= actBlock_del.m_string.size();
		}
		//実メモリのリプレース
		if (sellLgc_befor.pbytes==null) {
			//実メモリへの行登録
			sellLgc_befor.pbytes = pEditTbl->NewElement(actBlock_ins.m_string.c_str(), actBlock_ins.m_string.size());
			cur.L.pbytes = sellLgc_befor.pbytes;
			iterFirst_del = end();
			iterLast_del = end();
		}else{
			sellLgc_befor.pbytes->replace(sellLgc_befor.off, sellLgc_end.off, (BYTE*)actBlock_ins.m_string.c_str(), actBlock_ins.m_string.size());
			iterFirst_del = FindTopLine(sellLgc_befor, true, begin());
			iterLast_del = FindTopLine(sellLgc_end, false, iterFirst_del);
		}
		sellLgc_befor.ptbl = pEditTbl;
		sellLgc_befor.off = 0;
		sellLgc_befor.len = sellLgc_befor.pbytes->m_size;
		BuildLine2DspSell(sellLgc_befor, sellsDsp, 0);
	}
	iterFirst_ins = sellsDsp.begin();
	iterLast_ins = sellsDsp.end();

	row_cur_hint = iter2row(iterFirst_del);//カーソルヒント行位置を計算
	//line numberを振り直す
	OBJOFF row_first = row_cur_hint;
	OBJOFF row_last = row_first + (iterLast_ins - iterFirst_ins);
	OBJOFF line_num_first = iterFirst_del < end() ? (*iterFirst_del).line:0;
	OBJOFF line_num_last = iterLast_del < end() ? (*iterLast_del).line:0;
	OBJOFF line_num_last_real;
	OBJOFF row_last_real = iter2row(iterLast_del);
	if (line_num_last>0)
		line_num_last_real = line_num_last;
	else {
		OBJOFF row = GetThis_LineTop(SIZEMAC(size()));
		line_num_last_real = at(row).line;
	}

	assert(iterLast_del<=end());
	replace(sellsDsp/*iterFirst_ins, iterLast_ins*/, iterFirst_del, iterLast_del);
	RecalcLineNum(row_first, row_last, line_num_last);

	//カーソルの設定
	SetCur_4Lgc(cur, line_num_last==0 ? 0:row_cur_hint);
	cursor_after.SetBlinkCursor(cur);

	//ﾌﾞｯｸﾏｰｸの更新処理
	//BookMark 計算し直し
	//123456	//123
	//78		//123456
	//1234		//
	if (!m_bookMarks.IsEmpty_normal(true) && bMultiLine || line_num_first>0) {
		OBJOFF row = GetThis_LineTop(row_last>=size() ? SIZEMAC(size()):row_last);
		OBJOFF line_num_last_T = at(row).line;
		m_bookMarks.RecalcPosition(line_num_first, 
			bMultiLine ? (line_num_last_real-line_num_first):1,
			bMultiLine ? (line_num_last_T-line_num_first):1, 
			row_first, row_last_real - row_first, sellsDsp.size()); 
	}

	return true;
}



bool CLineHint::SetActionLgc(CActionBlock &actBlock_del, CActionBlock &actBlock_ins,
							CCursor &cursor_befor, CCursor &cursor_after, const string *pstring, CTextCashTbl *pEditTbl)
{
/*	CCursor	m_cursor_befor;		//編集直前の論理セル状態
	CCursor	m_cursor_after;		//編集直後の論理セル状態
	//OBJOFF	m_line_next;//編集直後論理行番号（m_sellsLgc_delがなく、m_sellsLgc_insがある場合）
	//SELL	m_sellLgc_off;	//編集セル位置<-m_cur_prevから求める
	SELLS	m_sellsLgc_ins;	//追加した論理行セルs
	SELLS	m_sellsLgc_del;	//削除した論理行セルs
	string	m_string_ins;		//追加した文字列
	string	m_string_del;		//削除した文字列
*/
	assert(pstring);

	bool bAbsFileLine = IsAbsFileRow(cursor_befor.GetBlinkCursor().D.y, pEditTbl);
	CUR cur;
	
	//セル状態による処理分岐
	
	//1行全て以上にわたる選択状態か？
	//AbsFileLineか？
	if (IsMultiLinesExpand(cursor_befor) || bAbsFileLine) {
		//元の行を削除する
		//削除する行情報を登録する
		BuildDsp2LineSells(cursor_befor, actBlock_del.m_sellsLgc);
		assert(actBlock_del.m_sellsLgc.begin()+actBlock_del.m_sellsLgc.size() == actBlock_del.m_sellsLgc.end());
		//カーソルの選択範囲より前の文字列と、選択範囲以後の文字列の余りで、
		//インプット文字列をサンドイッチしてストリングブロックとする
		string str;
		str = GetPrevPartStringOfLine(cursor_befor.Befor());
		str += *pstring;
		OBJOFF offset = str.size();
		str += GetNextPartStringOfLine(cursor_befor.End());
		if (str.size()>0) {
			cur.L = BuildString2LineSells(actBlock_ins.m_sellsLgc, &str, pEditTbl, offset);
			assert(actBlock_ins.m_sellsLgc.begin()+actBlock_ins.m_sellsLgc.size() == actBlock_ins.m_sellsLgc.end());
		}else{
			cur.L = cursor_befor.End().L;
		}
		cursor_after.SetBlinkCursor(cur);
		return true;
	}else if (cursor_befor.IsExpand()) {
		//EditFileLineで、１行内でのリプレース場合
		actBlock_del.m_string = GetPartStringOfLine(cursor_befor);
	}

	//エンターが含まれた文字列か？
	if (pstring->find(VK_RETURN)!=string::npos || pstring->find(VK_NL)!=string::npos) {
		//TODO:改行直前でEnterkeyのみ挿入の場合は、元の行を削除せず
		//新規
		//現在の行を削除登録する
		BuildDsp2LineSells(cursor_befor, actBlock_del.m_sellsLgc);
		string str;
		str = GetPrevPartStringOfLine(cursor_befor.Befor());
		str += *pstring;
		OBJOFF offset = str.size();
		str += GetNextPartStringOfLine(cursor_befor.End());
		cur.L = BuildString2LineSells(actBlock_ins.m_sellsLgc, &str, pEditTbl, offset);
		cursor_after.SetBlinkCursor(cur);
	}else{
		//通常の文字列の挿入の場合
		actBlock_ins.m_string += *pstring;
		cur = cursor_befor.Befor();
		cur.L.ptbl = pEditTbl;
		cur.L.off += pstring->size();
		cur.L.line = 1;
		cursor_after.SetBlinkCursor(cur);
	}
	return true;
}

void CLineHint::GetCurLeftRightChStrLine(CUR &cur, string &strLeft, string &strRight)
{
	OBJOFF rowTop = GetThis_LineTop(cur.D.y);
	CCursor cursor;
	cursor.SetExpand();
	cursor.SetBlinkCursor(cur);
	CUR curT = cur;
	//左
	PrevCur(curT, false);
	if (rowTop == GetThis_LineTop(curT.D.y)) {
		cursor.SetOrgCursor(curT);
		strLeft = GetPartStringOfLine(cursor);
	}else
		strLeft = "";
	//右
	curT = cur;
	NextCur(curT, false);
	if (rowTop == GetThis_LineTop(curT.D.y)) {
		cursor.SetOrgCursor(curT);
		strRight = GetPartStringOfLine(cursor);
	}else
		strRight = "";

}
void CLineHint::ReBuildDsp2DspSells(CViewEnv *pViewEnv, CLineHint &sellsDest)
{
	sellsDest.clear();
	
	sellsDest.Initialize(pViewEnv);
	Initialize(pViewEnv);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	assert(pMainFrame);
	int row_last = size();
	if (pViewEnv->m_xdspMax == SHRT_MAX) {
		BuildDsp2LineSells(0, row_last, sellsDest);
	}else{
        OBJOFF row;
		for (row=0; row<row_last;) {
			BuildLine2DspSell(ScanMakeLineSell(row), sellsDest, NPOS);
			if (row%100==0)
				pMainFrame->SetPaneText_Layout(row, size());
		}
		pMainFrame->SetPaneText_Layout(row, size());
	}
}

void CLineHint::BuildLine2DspSells(const SELLS::iterator iter_begin, const SELLS::iterator iter_end, SELLS &sellsDsp)
{
	for (SELLS::iterator iter=iter_begin; iter<iter_end; iter++) {
		BuildLine2DspSell(*iter, sellsDsp, 0);
	}
}
void CLineHint::BuildLine2DspSell(SELL sellLine, SELLS &sellsDsp, OBJOFF line)
{
	BYTE *pch, *pchMax;
	OBJOFF off = sellLine.off;
	OBJSIZE len = sellLine.ptbl->ReadLineCash(pch, off, sellLine.pbytes);
	m_pFormatChars->Initialize(off, line);
	BYTE ch, ch2;
	for (pchMax = pch + len, ch=*pch; pch <pchMax; ) {
		ch2 = *(++pch);
		++off;
		if (m_pFormatChars->EntrySell(pch, ch, ch2, sellLine, off)) {
			sellsDsp.push_back(sellLine);
		}
		ch = ch2;
	}

	if (m_pFormatChars->EntryLastSell(sellLine, off)) {
		sellsDsp.push_back(sellLine);
	}
	//sellsDsp.push_back(sellLine);
}

SELL CLineHint::BuildString2LineSells(SELLS &sells, const string *pstr, CTextCashTbl *pEditTbl, OBJOFF off_cur)
{
	OBJSIZE off=0, off_next = 0, off_next_CR;
//	OBJSIZE off_CR, off_LF;
	SELL sell, sell_cur;
	sell = SELL(pEditTbl, 0);
	do {
		//off_CR = pstr->find(VK_RETURN, off);
#ifdef BUG_1999_2_1 //必ずCR+NLにする
		off_next = pstr->find(VK_NL, off);
#else
		off_next = pstr->find(VK_NL, off);
		off_next_CR = pstr->find(VK_RETURN, off);
#if bug_111201 //VK_NLだけの場合でも下のif文がTrueになってしまう。
		if (off_next == string::npos || off_next_CR+1 < off_next)
#else
		if (off_next == string::npos ||
			(off_next_CR!=string::npos && off_next_CR+1 < off_next)) //if (off_next == string::npos || off_next_CR+1 < off_next)
#endif
			off_next = off_next_CR;
#endif
		//off_next = min(off_CR, off_next);
		if (off_next != string::npos)
			off_next++;
		//実メモリへの行登録
		sell.len = off_next != string::npos ? off_next-off: pstr->size()-off;
		sell.pbytes = pEditTbl->NewElement(pstr->c_str()+off, sell.len);
		sells.push_back(sell);
		if (off_cur>=off && off_cur<off_next) {
			sell_cur = sell;
			sell_cur.off = off_cur-off;
		}
		off = off_next;
	}while(/*off!=string::npos &&*/ off<pstr->size());
	return sell_cur;
}

//選択範囲の文字列を１論理行単位で取得して、指定された関数をコールする
//行末は必ず、CRLFになっている
int CLineHint::ScanDataString(CCursor &cursor, ScanDataLineProc pProc, void *pv)
{
	int rtn;
	//string str;
	CUR cur;
	if (!IsMultiLinesExpand(cursor)) {
		//str = GetPartStringOfLine(cursor); //first line string.
		rtn = (*pProc)(GetPartStringOfLine(cursor), pv);
	}else{
		//str = GetNextPartStringOfLine(cursor.Befor()); //first line string.
		if ((rtn = (*pProc)(GetNextPartStringOfLine(cursor.Befor()), pv))<0)
			return rtn;
		cur.D.y = cursor.Befor().D.y;
		OBJOFF rowEnd = GetThis_LineTop(cursor.End().D.y);
		cur.D.x = 0;
		while ((cur.D.y=GetNext_LineTop(cur.D.y))<rowEnd) {
			SetCur(cur, false);
			//str = GetNextPartStringOfLine(cur); //middle line string.
			if ((rtn = (*pProc)(GetNextPartStringOfLine(cur), pv))<0) {
				return rtn;
			}
		}
		//str = GetPrevPartStringOfLine(cursor.End()); //last line string.
		//if (cursor.End().D.x>0)
		rtn = (*pProc)(GetPrevPartStringOfLine(cursor.End()), pv);
	}
	return rtn;
}
OBJSIZE CLineHint::GetNext_BlockTop(CUR &cur, const OBJSIZE &lenCheck)
{
	//cur.Dでは、yだけみる
	OBJOFF row = cur.D.y;
	OBJOFF off = cur.L.off;
	assert(row<size());
	
	SELL *psell=&at(row);
	OBJOFF len = psell->len - (off - psell->off);
	if (lenCheck < len ) {
		cur.L.off += lenCheck;
		return lenCheck;
	}

	while(++row<size()) {
		if (!at(row).IsNextBlock(*psell)) {
			cur.D.y = row;
			cur.D.x = 0;
			SetCur(cur, false);
			return len;
		}
		psell = &at(row);
		if ((len + psell->len)>lenCheck) {
			//10+6>12
			//012345
			//  ^
			//16+6>16
			//012345
			//  ^
			cur.L.off = psell->off + (lenCheck-len);
			len = lenCheck;
			break;
		}
		len += psell->len;
	}

	cur.D.y = row;
	return len; //最後のスキャンが終わったら、NPOSとなる
}


//選択範囲の文字列をなるべく大きなデータ単位でコピーなど余計な処理をせず、指定された関数をコールする
//行末はCRLFになっているとは限らない。CRだけ、LFだけの場合がある。
int CLineHint::ScanDataBlock(CUR _cur, ScanDataBlockProc pProc, void *pv, const CUR *pcur_last)
{
	CScanDataBlock scan;
	scan.Initialize(_cur, this, pcur_last);
	
	int rtn=0;
	OBJSIZE len;
	while(len = scan.ReadBuffer()) {
		if ((rtn = (*pProc)(scan.GetBuffer(), len, pv))<0)
			break;
	}
	return rtn;
}




HGLOBAL CLineHint::GetCursorData(const CCursor &_cursor, Normal_Proc pProc, SSDP_DATA *pdata)
//const string *pstring, HGLOBAL hData, OBJSIZE lenData)
{
	SSDP_DATA data;
	if (!pdata) {
		memset(&data, 0, sizeof(data));
		pdata = &data;
	}

	CCursor cursor = _cursor;
	CUR cur = cursor.GetBlinkCursor();
	if (!cursor.IsExpand()) {
		//カーソルラインのコピー
		cursor.SetExpand();
		cur.D.x = 0;
		SetCur(cur, false);
		cursor.SetBlinkCursor(cur);
		cur.D.x = NPOS;
		SetCur(cur, false, SETSEL_SET);
		cursor.SetOrgCursor(cur);
	}
	
	if (pdata->pstrAdd || pdata->pstrRemove) {
		ScanDataString(cursor, (ScanDataLineProc)pProc, pdata);
	}else{
		ScanDataBlock(cursor.Befor(), (ScanDataBlockProc)pProc, pdata, &cursor.End());
	}
	return pdata->hData;
}



bool CLineHint::FindData(CCursor &cursor, string strFind, bool bCase, bool bToDocEnd)
{
	find_data	data;
	if (bCase) {
		data.chUpper = strFind[0];
		data.chLower = 0;
	}else{
		_mbslwr((BYTE*)strFind.data());
		data.chLower = strFind[0];
		_mbsupr((BYTE*)strFind.data());
		data.chUpper = strFind[0];
	}
	data.strFind = strFind;
	data.bCase = bCase;

	return bToDocEnd ? FindData_ToEnd(cursor, FindToEndProc, &data):
				FindData_ToTop(cursor, FindToTopProc, &data);
}

bool CLineHint::FindData_ToTop(CCursor &cursor, FindDataLineProc pProc, void *pv, bool bExpand)
{
	string str;
	CUR cur;
	OBJOFF off_find;
	OBJOFF rowChk;
	int len_find;

	if (!IsMultiLinesExpand(cursor)) {
		if ((len_find = (*pProc)(GetPartStringOfLine(cursor), pv, off_find))>0) {
			cur = cursor.Befor();
			goto FIND;
		}
	}else{
		//last line string.
		if ((len_find = (*pProc)(GetPrevPartStringOfLine(cursor.End()), pv, off_find))>0) {
			cur.D.y = GetThis_LineTop(cursor.End().D.y);
			cur.D.x = 0;
			SetCur(cur, false);
			goto FIND;
		}
		cur.D.y = cursor.End().D.y;
		rowChk = GetThis_LineTop(cursor.Befor().D.y);
		cur.D.x = 0;
		while ((cur.D.y=GetPrev_LineTop(cur.D.y))>rowChk) {
			if (MyPeekMessage_EscCheck(/*AfxGetMainWnd()->m_hWnd*/::GetActiveWindow()))
				return false;
			SetCur(cur, false);
			if ((len_find = (*pProc)(GetNextPartStringOfLine(cur), pv, off_find))>0) {
				goto FIND;
			}
		}
		if ((len_find = (*pProc)(GetNextPartStringOfLine(cursor.Befor()), pv, off_find))>0) {
			cur = cursor.Befor();
			goto FIND;
		}
	}
	return false;
FIND:
	assert(len_find>0);
	cursor.SetExpand(bExpand);
	cur.L.off += (off_find);
	OBJOFF row_hint = cur.D.y;
	SetCur_4Lgc(cur, row_hint);
	if (bExpand) {
		cursor.SetOrgCursor(cur);
		cur.L.off += (len_find);
		SetCur_4Lgc(cur, row_hint);
	}
	cursor.SetBlinkCursor(cur);
	return true;
}
bool CLineHint::FindData_ToEnd(CCursor &cursor, FindDataLineProc pProc, void *pv, bool bExpand)
{
	string str;
	CUR cur;
	OBJOFF off_find;
	OBJOFF rowChk;
	int len_find;

	if (!IsMultiLinesExpand(cursor)) {
		if ((len_find = (*pProc)(GetPartStringOfLine(cursor), pv, off_find))>0) {
			cur = cursor.Befor();
			goto FIND;
		}
	}else{
		if ((len_find = (*pProc)(GetNextPartStringOfLine(cursor.Befor()), pv, off_find))>0) {
			cur = cursor.Befor();
			goto FIND;
		}
		cur.D.y = cursor.Befor().D.y;
		rowChk = GetThis_LineTop(cursor.End().D.y);
		cur.D.x = 0;
		while ((cur.D.y=GetNext_LineTop(cur.D.y))<rowChk) {
			if (MyPeekMessage_EscCheck(/*AfxGetMainWnd()->m_hWnd*/::GetActiveWindow()))
				return false;
			SetCur(cur, false);
			if ((len_find = (*pProc)(GetNextPartStringOfLine(cur), pv, off_find))>0) {
				goto FIND;
			}
		}
		//last line string.
		if ((len_find = (*pProc)(GetPrevPartStringOfLine(cursor.End()), pv, off_find))>0) {
			cur.D.y = GetThis_LineTop(cursor.End().D.y);
			cur.D.x = 0;
			SetCur(cur, false);
			goto FIND;
		}
	}
	return false;
FIND:
	assert(len_find>0);
	cursor.SetExpand(bExpand);
	cur.L.off += (off_find);
	OBJOFF row_hint = cur.D.y;
	SetCur_4Lgc(cur, row_hint);
	if (bExpand) {
		cursor.SetOrgCursor(cur);
		cur.L.off += (len_find);
		SetCur_4Lgc(cur, row_hint);
	}
	cursor.SetBlinkCursor(cur);
	return true;
}
bool CLineHint::FindDiffChType(CUR &cur, int& _chtype, bool bEnableSpace, bool bTalCurOk, bool bFoward)
{
//	TRACE("y=%d, x=%d\n",cur.D.y,cur.D.x);
	int chtype, chtype2, chtype_chk;
	BYTE *pch, *pch_first, *pch_chk, *pch_max, ch, ch2;
	SELL sell = cur.L;
	int row = cur.D.y;
	if (!bFoward)
		cur.L = at(row=GetThis_LineTop(row));
	int off_first = cur.L.off;
	OBJSIZE len = cur.L.ptbl->ReadLineCash(pch_first, off_first, cur.L.pbytes);
	pch = pch_first;
	if (bFoward) {
		pch_max = pch+len;
	}else {
		pch_max = pch + min(sell.off - cur.L.off, len);
		pch_chk = pch;
	}
	ch = *pch++;
	chtype = GetChType(ch);
	bool bKanj;
	while(pch<=pch_max) {	//while(pch<pch_max) {
		ch2 = *pch++;
		chtype2 = GetChType(ch2);
		if (bKanj=(chtype&SJ1 && chtype2&SJ2)) {
			//kanj
			chtype_chk = GetDbcsChType(ch, ch2);
		}else{
			if (IsCRLF(ch))
				break;
			chtype_chk = AndSbcsChType(chtype);
		}
		if (chtype_chk != _chtype) {
			if (_chtype==0) {
				_chtype = chtype_chk;
				if (bFoward && _chtype&WHT && !bEnableSpace )
					return true;
			}else{
				if (bFoward)
					break;
				else {
					pch_chk = pch;
					if (bTalCurOk)
						_chtype = chtype_chk;
				}
			}
		}
		if (bKanj) {
			ch = *pch++;
			chtype = GetChType(ch);
		}else{
			ch = ch2;
			chtype = chtype2;
		}
	}
	OBJSIZE delta; 
	if (bFoward) {
		delta = max(pch - pch_first- 2, 0);
		if (delta==0)
			return true;
//		TRACE("offset=%d\n",pch - pch_first-2);
	}else{
		delta = max(pch_chk - pch_first - (bTalCurOk ? 2:1), 0);
//		TRACE("offset=%d\n",pch_chk - pch_first-1);
	}
	cur.L.off += delta;
	if (cur.L.off >= (off_first + len))
		cur.L.off = off_first + len -1;
	if (!SetCur_4Lgc(cur, row))
		return false;	//stop !!
//	if (bFoward && bTalCurOk && (_chtype==0 || _chtype&WHT) && off_first>0 ) {
//		PrevCur(cur);
//		return FindDiffChType(cur, bEnableSpace, bTalCurOk, _chtype, bFoward);
//	}
	return true;
}

///////////////////////////////////////////////
//CScanDataBlock
CScanDataBlock::CScanDataBlock()
{
}
CScanDataBlock::~CScanDataBlock()
{
}
void CScanDataBlock::Initialize(CUR _cur, CLineHint *pHint, const CUR *pcur_last)
{
	assert(pHint);
	m_cur = _cur;
	pHint->SetCur(m_cur, false);
	m_pHint = pHint;
	m_sizeHint = pHint->size();
	m_pcur_last = pcur_last;

	m_bLoop = true;
	m_pch = null;
	m_len = 0;
}
OBJSIZE CScanDataBlock::ReadBuffer()
{
	if (m_cur.D.y<m_sizeHint && m_bLoop) {
		m_len = m_cur.L.ptbl->ReadBlockCash(m_pch, m_cur.L.off, m_cur.L.pbytes);
		m_cur_prv = m_cur;
		m_len = m_pHint->GetNext_BlockTop(m_cur, m_len);
		if (m_pcur_last && m_pcur_last->L.IsInSameBlock(m_cur_prv.L, m_len)) {
			assert(m_pcur_last->L.off>=m_cur_prv.L.off);
			if ((m_len = m_pcur_last->L.off - m_cur_prv.L.off)==0) {
				m_len = 0;
			}
			m_bLoop = false;
		}else if (m_cur.D.y>=m_sizeHint && m_len>0 && m_pch[m_len-1]==VK_EOF) {//付加したEOFの削除
			m_len--;
		}
	}else
		m_len = 0;

	return m_len;
}
CUR CScanDataBlock::GetCur(OBJOFF off)
{
	//現在のリードバッファのoffオフセット位置での論理カーソルを返す
	CUR cur = m_cur_prv;
	cur.L.off += off;
	m_pHint->SetCur_4Lgc(cur, cur.D.y);
	return cur;
}

