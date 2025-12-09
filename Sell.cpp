// Sell.cpp : SELL クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Sell.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// SELL

/////////////////////////////////////////////////////////////////////////////
// CCursor
int	comp(const CCursor &cur1, const CCursor &cur2)
{
	if (cur1.m_bExpand != cur2.m_bExpand)
		return 1;
	if (cur1.m_bExpand) {
		int cmp = comp(cur1.org, cur2.org);
		return cmp==0 ? comp(cur1.exp, cur2.exp):cmp;
	}else{
		//return ((cur1.org == cur2.org) ? 0:1);
		return comp(cur1.org, cur2.org);
	}
}

void CCursor::Initialize() 
{
	m_bExpand = false;
	org.Initialize();
	exp.Initialize();
	m_colTarget = 0;
}
void CCursor::SetCursor(CUR *pcurBlink, CUR *pcurOrg)
{
	assert(pcurBlink);
	exp = *pcurBlink;
	if (m_bExpand = (pcurOrg != null))
		org = *pcurOrg;
	else
		org = exp;
}

CCursor& CCursor::operator =(const CCursor &cur)
{
	m_bExpand = cur.m_bExpand;
	org = cur.org;
	exp = cur.exp;
	m_colTarget = cur.m_colTarget;
	return *this;
}

void CCursor::SetBlinkCursor(const CUR& cur, bool bTarget)
{
	if (m_bExpand)
		exp = cur; 
	else 
		org = cur;
	if (bTarget)
		m_colTarget = cur.D.x;
}

const CUR& CCursor::Befor()
{
	if (m_bExpand) {
		if (org>exp)
			return exp;
	}
	return org;
}

const CUR& CCursor::End()
{
	if (m_bExpand) {
		if (org<exp)
			return exp;
	}
	return org;
}

void CCursor::MoveSell(DSP_Y y, DSP_X x, bool bControl, bool bShift)
{
	
	if (bShift) {
		if (!m_bExpand) {
			exp = org;
			exp.D.y = y;
			exp.D.x = x;
			m_bExpand = true;
		}
	}else{
		if (m_bExpand) {
			m_bExpand = false;
			exp.Initialize();
			org.D.y = y;
			org.D.x = x;
		}
	}
}
void CCursor::MoveSell(UINT nChar, bool bControl, bool bShift)
{
	
	if (bShift) {
		if (!m_bExpand) {
			exp = org;
			m_bExpand = true;
		}
	}else{
		if (m_bExpand) {
			if (nChar== VK_LEFT || nChar==VK_PRIOR || nChar==VK_UP) {
				if (exp<org)
					org = exp;
				else
					m_colTarget = org.D.x;
			}else if (nChar== VK_RIGHT || nChar==VK_NEXT || nChar==VK_DOWN) {
				if (exp>org)
					org = exp;
				else
					m_colTarget = org.D.x;
				
			}
			m_bExpand = false;
			exp.Initialize();
		}
	}
}

int CCursor::DiffLines(CCursor cur2)
{
	return End().D.y - cur2.End().D.y;
}
CCursor CCursor::MargeCursor(CCursor cur2)
{
	CCursor cur;
	if (*this==cur2)
		return cur=*this;
//	else if (IsExpand() != cur2.IsExpand()) {
//		if (IsExpand())
//			cur = *this;
//		else
//			cur = cur2;
	else{
		cur.SetExpand();
		if (Befor()<=cur2.Befor())
			cur.SetOrgCursor(Befor());
		else
			cur.SetOrgCursor(cur2.Befor());
		if (End().D.x==NPOS || End()>=cur2.End())
			cur.SetBlinkCursor(End());
		else
			cur.SetBlinkCursor(cur2.End());
	}
	return cur;
}

CCursor CCursor::DiffCursor(CCursor cur2)
{
	//リターンカーソルの、Expand()範囲が変更個所である
	//リターンカーソルが、Expand()でない場合->変更なし
	CCursor cur;
	if (*this==cur2)
		return cur;
	else if (IsExpand() != cur2.IsExpand()) {
		if (IsExpand())
			cur = *this;
		else
			cur = cur2;
	}else{
		if (!IsExpand()/*&& GetBlinkCursor().D.y == GetBlinkCursor().D.y*/)
			return cur;
		cur.SetExpand();
		if (GetOrgCursor() == cur2.GetOrgCursor()) {
			cur.SetOrgCursor(GetBlinkCursor());
			cur.SetBlinkCursor(cur2.GetBlinkCursor());
		}else{
			if (Befor()<=cur2.Befor())
				cur.SetOrgCursor(Befor());
			else
				cur.SetOrgCursor(cur2.Befor());
			if (End()>=cur2.End())
				cur.SetBlinkCursor(End());
			else
				cur.SetBlinkCursor(cur2.End());
		}
	}
	return cur;
}
void CCursor::SetExpand(bool b)
{
	if (!m_bExpand && b)
		exp = org;
	m_bExpand = b;
}

#if defined(_DEBUG) || defined(_VERIFY)
bool CCursor::IsValid()
{
	if (IsExpand()) {
		SELL selBef = Befor().L;
		SELL selEnd = End().L;
		if (selBef.ptbl == selEnd.ptbl &&
			selBef.pbytes == selEnd.pbytes) {
			return selBef.off<=selEnd.off;
		}
	}
	return true;
}
#endif

/*
bool CCursor::IsInCoursorY(ObjTable::size_type y)
{
	if (!m_bExpand) {
		return y == org.y;
	}else{
		return (y<=org.y && y>=exp.y) ||
			(y<=exp.y && y>=org.y);
	}
}
bool CCursor::IsBlinkCoursorY(ObjTable::size_type y)
{
	if (!m_bExpand) {
		return y == org.y;
	}else{
		return y == exp.y;
	}
}
*/