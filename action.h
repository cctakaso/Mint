#ifndef __ACTION_H__
#define __ACTION_H__

#include "sell.h"
//#include "hint.h"
class VK
{
public:
	VK() {
		m_vk=0; m_flg=0; m_count=0;}
	~VK() {};
protected:
	enum {
		vkb_control=1,
		vkb_shift=2,
	};
public:
	bool IsEntryMoveKey(UINT nChar, bool bControl=false, bool bShift=false, int count=1);
	bool IsDeleteKey(UINT nChar) {
		return nChar == VK_DELETE; }
	bool IsEmpty() {
        return this==null;/*&VK::vk==0;*/ }
	UINT vk() const {	return (UINT)m_vk; }
	bool bControl() const {return (m_flg & vkb_control)>0; }
	bool bShift() const {return (m_flg & vkb_shift)>0; }
	int count() const {return m_count; }
protected:
	unsigned short m_vk;
	unsigned short m_flg;
	int			m_count;
};

typedef deque<VK> VKS;

class CActionBlock
{
public:
	CActionBlock() {};
	~CActionBlock() {};
public:
	bool Mage(CActionBlock &after, bool bChangeCursor, bool bCheck=false);
	void Copy(CActionBlock &after);
	CCursor	m_cursor;	//編集直前/直後の論理セル状態
	SELLS	m_sellsLgc;		//追加/削除した論理行セルs
	string	m_string;		//追加/削除した文字列
};
class CActionLgc
{
public:
	CActionLgc() {};
	~CActionLgc() {};
public:
	bool Mage(CActionLgc *pact);
	void AfterCopy(CActionLgc *pact);
	enum {
		act_block_del = 0,
		act_block_ins = 1,
		act_block_befor = 0,
		act_block_after = 1,
		act_block_max = 2,
	};
	//論理行とは？　表示最大桁に寄らない、CRLFまでの論理行を収めたもの
	//m_lineメンバはその時意味がないので、アクション時のm_line_prevを参照して
	//Undo/Redoで挿入・削除する。
	//複数の表示行->１論理行
	CActionBlock	m_actBlock[act_block_max];
	//CCursor		m_cursor_befor;		//編集直前の論理セル状態
	//CCursor		m_cursor_after;		//編集直後の論理セル状態
	//SELLS	m_sellsLgc_ins;	//追加した論理行セルs
	//SELLS	m_sellsLgc_del;	//削除した論理行セルs
	//string	m_string_ins;		//追加した文字列
	//string	m_string_del;		//削除した文字列
};

enum ACT {
	act_nop,
	act_undo,
	act_redo,
	//
	act_move,
	act_input,
	act_input_ime,
	act_input_ime_result,
	act_delete_del,
	act_delete_back,
	act_past,
	act_copy,
};

#define IsActDelete(p)	(p==act_delete_del || p==act_delete_back)

class CViewEd;
class CAction
{
	friend class CViewEd;
	friend class CTheView;
	friend class CFileSelectBar;
public:
	CAction();
	CAction(ACT act);
	~CAction();
public:
	void push_vk(ACT act, const VK &vk);
	bool push_char(const BYTE byte, int nRep=1);
	bool push_string(ACT act, const char *sz, OBJOFF len);
	bool push_ime_string(const string &str, bool bResult=false);
	void clear_str();
	bool IsEmpty() {
		return m_act==act_nop; }
	bool IsEnableMarge(CAction *pact);
	bool Mage(CAction *pact);
	bool IsReturnAction();
	int HowMultiLineChange();
	void SetBeforCursor(CCursor cursorBef) {
		assert(m_pactLgc);
		m_pactLgc->m_actBlock[CActionLgc::act_block_befor].m_cursor = cursorBef; }
public:
	ACT	m_act;
protected:

	VK			m_vk;
	string*		m_pstring;
	CActionLgc*	m_pactLgc;
};

class CActions :public deque<CAction*>
{
public:
	CActions() {Initialize(); }
	~CActions() {Erase(0); }
public:
	bool IsEnableUndo() {
		return size()>0 && m_idx>0; }
	bool IsEnableRedo() {
		return m_idx<size(); }
	void ResetMargeFlg() {
		m_bMargeFlg = false; }

	void push_action(CAction *paction, bool bMargeAble=true);
	CAction *pop_undo() {
		assert(IsEnableUndo());
		assert(!at(m_idx-1)->IsEmpty());
		ResetMargeFlg();
		return at(--m_idx); }
	CAction *pop_redo() {
		assert(IsEnableRedo());
		assert(!at(m_idx)->IsEmpty());
		ResetMargeFlg();
		return at(m_idx++); }
	void Initialize() {
		Erase(0);
		ResetMargeFlg();
		m_idx=0;
	}
	bool IsBeforInputIme() {
		return IsEnableUndo() && at(m_idx-1)->m_act==act_input_ime; }
	void ChangeBefor2ImeResultAction() {
		assert(IsBeforInputIme());
		at(m_idx-1)->m_act=act_input_ime_result;
		m_bMargeFlg=true;}
	void Erase(OBJOFF befor=NPOS, OBJOFF last=NPOS);
private:
	OBJOFF m_idx; //Redoを行えるポイントを指す
	bool m_bMargeFlg;
	
	OBJOFF iter2row(iterator iter) {
		return (iter-begin()); }
	iterator row2iter(OBJOFF row) {
		return (begin()+row); }
	bool IsEnableMarge(CAction *pact) {
		return IsEnableUndo() && m_bMargeFlg &&
			at(m_idx-1)->IsEnableMarge(pact); }
};


#endif //#ifndef __ACTION_H__
