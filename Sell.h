#ifndef _SELL_H__
#define _SELL_H__

class CViewEd;

enum LD_WHICH{
	ld_logic,
	ld_dsp,
};
class CByteCash;
class CTextCashTbl;
class SELL
{
public:
	SELL() { Initialize(); }
	SELL(DSP_Y iy, DSP_X ix, OBJSIZE le=0, OBJOFF li=0, CByteCash *pb=null) {
		 y=iy; x=ix; len=le; line=li; pbytes=pb; }
	SELL(CTextCashTbl *pt, OBJOFF of, OBJSIZE le=0, OBJOFF li=0, CByteCash *pb=null) {
		ptbl=pt; off=of; len=le; line=li; pbytes=pb; }
	~SELL() {}
public:

	const SELL& operator =(const SELL &sel);
	void Initialize();
	bool IsInSell_Lgc(const SELL &sel) {
		return ptbl == sel.ptbl && pbytes == sel.pbytes &&
			(off <= sel.off && (off+len) > sel.off); }
	bool IsInSameBlock(const SELL &sel, OBJSIZE len) const {
		return ptbl == sel.ptbl && pbytes == sel.pbytes &&
			(sel.off <= off && off <(sel.off+len)); }
	bool IsNextBlock(const SELL &sel) {
		return ptbl == sel.ptbl && pbytes == sel.pbytes &&
			off == (sel.off+sel.len); }
	bool IsEmpty() {
		return y==0 && x==0 && len==0; }
	union {
		DSP_Y		y;
		CTextCashTbl	*ptbl;
	};
	union {
		DSP_X		x;
		OBJOFF		off;
	};
	OBJSIZE		len;
	//->ロジック・隠れたヒントメンバー
	OBJOFF		line;
	CByteCash	*pbytes;
	//<-
protected:
	friend int	comp(const SELL &sel1, const SELL &sel2);
};
int	comp(const SELL &sel1, const SELL &sel2);
inline bool operator ==(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)==0; }
inline bool operator !=(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)!=0; }
inline bool operator >(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)>0; }
inline bool operator >=(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)>=0; }
inline bool operator <(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)<0; }
inline bool operator <=(const SELL &sel1, const SELL &sel2) {
	return comp(sel1, sel2)<=0; }
inline void SELL::Initialize() {
	x = 0;
	y = 0;
	len = 0;
	line = 0;
	pbytes = null;
}
inline int	comp(const SELL &sel1, const SELL &sel2) {
	if (sel1.y == sel2.y) {
		if (sel1.x == sel2.x)
			return 0;
		return sel1.x > sel2.x ? 1:-1;
	}else{
		return sel1.y > sel2.y ? 1:-1;
	}
}

inline const SELL& SELL::operator =(const SELL &sel) {
	x = sel.x;
	y = sel.y;
	len = sel.len;
	line = sel.line;
	pbytes = sel.pbytes;
	return *this;
}


class CUR
{
public:
	CUR() {};
	~CUR() {};
public:
	SELL L;	//logic cursor
	SELL D;	//display cursor

public:
	void Initialize() {
		L.Initialize();
		D.Initialize(); }
	CUR& operator =(const CUR &cur);
	bool IsEmpty() {
		return L.IsEmpty() && D.IsEmpty(); }
protected:
	friend int	comp(const CUR &cur1, const CUR &cur2, LD_WHICH ld);
};

inline CUR& CUR::operator =(const CUR &cur) {
	L = cur.L;
	D = cur.D;
	return *this;
}

inline int comp(const CUR &cur1, const CUR &cur2, LD_WHICH ld=ld_dsp)
{
	return (ld == ld_logic ? comp(cur1.L, cur2.L):comp(cur1.D, cur2.D));
}
inline bool operator ==(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)==0; }
inline bool operator !=(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)!=0; }
inline bool operator >(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)>0; }
inline bool operator >=(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)>=0; }
inline bool operator <(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)<0; }
inline bool operator <=(const CUR &cur1, const CUR &cur2) {
	return comp(cur1, cur2)<=0; }

class CUR_SIZE
{
public:
	CUR_SIZE() { cx=cy=0; };
	CUR_SIZE(DSP_X x, DSP_Y y) { cx=x; cy=y; };
	~CUR_SIZE() {};
public:
	DSP_X	cx;
	DSP_Y	cy;

	void clear() {
		cx=cy=0; }
	const CUR_SIZE& operator =(const CUR_SIZE &cursize){
		cx = cursize.cx;
		cy = cursize.cy;
		return *this;	}
	const CUR_SIZE& operator +=(const CUR_SIZE &cursize) {
		cx += cursize.cx;
		cy += cursize.cy;
		return *this;	}
	const CUR_SIZE& operator -=(const CUR_SIZE &cursize) {
		cx -= cursize.cx;
		cy -= cursize.cy;
		return *this;	}
};
inline CUR_SIZE operator +(const CUR_SIZE &cs1, const CUR_SIZE &cs2){
	CUR_SIZE cs;
	cs.cx = cs1.cx + cs2.cx;
	cs.cy = cs1.cy + cs2.cy;
	return cs;
}
inline CUR_SIZE operator -(const CUR_SIZE &cs1, const CUR_SIZE &cs2){
	CUR_SIZE cs;
	cs.cx = cs1.cx - cs2.cx;
	cs.cy = cs1.cy - cs2.cy;
	return cs;
}

class CCursor
{
	friend class CViewEd;
public:
	CCursor() {
		Initialize(); }
	~CCursor() {};
public:
	CUR org;
	CUR exp;
	bool m_bExpand;
	OBJOFF m_colTarget;

public:
	void MoveSell(UINT nChar, bool bControl, bool bShift);
	void MoveSell(DSP_Y y, DSP_X x, bool bControl, bool bShift);
	const CUR& GetBlinkCursor(void){
		return m_bExpand ? exp:org; }
	void SetBlinkCursor(const CUR& cur, bool bTarget=true);
	const CUR& GetOrgCursor() {
		return org;	}
	void SetOrgCursor(const CUR& cur) {
		org = cur;}
	void SetTarget(OBJOFF colTarget) {
		m_colTarget = colTarget; }
	OBJOFF GetTarget() {
		return m_colTarget; }
//	bool IsInCoursorY(ObjTable::size_type y);
//	bool IsBlinkCoursorY(ObjTable::size_type y);
	bool IsExpand() {
		return m_bExpand; }
	void SetExpand(bool b=true);
	void SetCursor(CUR *pcurBlink, CUR *pcurOrg=null);
	void ResetExpand_IfCuremp() {
		if (org == exp) m_bExpand = false; }
	const CUR& Befor();
	const CUR& End();
	CCursor& operator =(const CCursor &cur);
	void Initialize();
	CCursor MargeCursor(CCursor cur2);
	CCursor DiffCursor(CCursor cur2);
	int DiffLines(CCursor cur2);
	bool IsEmpty() {
		return org.IsEmpty() && exp.IsEmpty(); }
#if defined(_DEBUG) || defined(_VERIFY)
	bool IsValid();
#endif
protected:
	friend int	comp(const CCursor &cur1, const CCursor &cur2);
};

int	comp(const CCursor &cur1, const CCursor &cur2);
inline bool operator ==(const CCursor &cur1, const CCursor &cur2) {
	return comp(cur1, cur2)==0; }
inline bool operator !=(const CCursor &cur1, const CCursor &cur2) {
	return comp(cur1, cur2)!=0; }

class SELLS: public vector<SELL>//deque<SELL>
{
	friend class CTheView;
	friend class CViewEd;
	friend class CBookMark;
public:
	SELLS() {
		m_rowMax_Calced_LineNum = NPOS; }
	~SELLS() {};
	OBJOFF GetByteOffsetInCursor(const CUR &curBef, OBJOFF _row, OBJOFF off_in_this_row);
	OBJSIZE GetRows_ThisLine(OBJOFF row);
	bool IsLineTopRow(OBJOFF row) {
		assert(row<size());
		return at(row).line>0; }
	OBJOFF FindLine(OBJOFF line_hit, SELL &sellLgc);
	OBJOFF FindByte(OBJOFF byte_hit, SELL &sellLgc);
protected:
	OBJOFF m_rowMax_Calced_LineNum;
protected:
	void replace(SELLS &sells_ins, /*SELLS::iterator iter_src_first, SELLS::iterator iter_src_last,*/
							SELLS::iterator iter_des_first, SELLS::iterator iter_des_last);
	//void replace(iterator iter_src_first, iterator iter_src_last,
	//			iterator iter_des_first, iterator iter_des_last);
	
	OBJOFF RecalcLineNum(OBJOFF row_first=NPOS, OBJOFF row_last=NPOS, OBJOFF line_check=0);
	OBJOFF FindRow(const SELL &sellLgc, OBJOFF row);
	iterator FindTopLine(const SELL &sellLgc, bool bPrev, iterator iter);
	string GetPrevPartStringOfLine(const CUR &cur);
	string GetPartStringOfLine(CCursor &cursor);
	string GetNextPartStringOfLine(const CUR &cur);

	OBJOFF GetThis_LineTop(OBJOFF row);
	OBJOFF GetNext_LineTop(OBJOFF row);
	OBJOFF GetPrev_LineTop(OBJOFF row);

	void BuildDsp2LineSells(CCursor &cursor, SELLS &sells);
	void BuildDsp2LineSells(OBJOFF row_first, OBJOFF row_last, SELLS &sells);
	SELL ScanMakeLineSell(OBJOFF &row, OBJOFF line=1);
	iterator GetThis_LineTop(iterator iter);
	bool IsAbsFileRow(OBJOFF row, const CTextCashTbl *pEditTbl) const ;
	bool IsMultiLinesExpand(CCursor cursor);
	OBJOFF iter2row(iterator iter) {
		return (iter-begin()); }
	iterator row2iter(OBJOFF row) {
		return (begin()+row); }

};

#endif//#ifndef _SELL_H__