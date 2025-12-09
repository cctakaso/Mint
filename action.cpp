// action.cpp : CAction クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "action.h"
#include "chara.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VK
static UINT vk_entry_move[] = {
	//VK_DELETE,
	VK_HOME,
	VK_END,
	VK_PRIOR,
	VK_NEXT,
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
};

bool VK::IsEntryMoveKey(UINT nChar, bool bControl, bool bShift, int count)
{
	m_vk = (unsigned short)(nChar & 0xffff);
	m_flg = (bControl ? vkb_control:0) | (bShift ? vkb_shift:0);
	m_count = count;
	for (int i=0; i<sizeof(vk_entry_move)/sizeof(UINT); i++) {
		if (m_vk == vk_entry_move[i])
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
// CActionBlock
bool CActionBlock::Mage(CActionBlock &after, bool bChangeCursor, bool bCheck)
{
	if (bCheck) {
		if (after.m_string.size()>0) {
			char ch = m_string.size()>0 ? m_string[m_string.size()-1]:0;
			char ch_after = after.m_string[0];
			if ((ch==' ' && ch_after!=' ') ||
#if bug_111201
				(ch==VK_TAB || ch==VK_RETURN) ||
				(ch_after==VK_TAB || ch_after==VK_RETURN))
#else
				(ch==VK_TAB || ch==VK_RETURN || ch==VK_NL) ||
				(ch_after==VK_TAB || ch_after==VK_RETURN || ch_after==VK_NL))
#endif
				return false;
		}else
			return false;
	}
	if (bChangeCursor)
		m_cursor = after.m_cursor;
	m_sellsLgc.insert(m_sellsLgc.end(), after.m_sellsLgc.begin(), 
										after.m_sellsLgc.end());
	m_string += after.m_string;
	return true;
}
void CActionBlock::Copy(CActionBlock &after)
{
	m_cursor = after.m_cursor;
	if (after.m_sellsLgc.size()>0)
		m_sellsLgc.insert(m_sellsLgc.end(), after.m_sellsLgc.begin(), after.m_sellsLgc.end());
	m_string = after.m_string;
}

/////////////////////////////////////////////////////////////////////////////
// CActionLgc
bool CActionLgc::Mage(CActionLgc *pact) 
{
	if (!m_actBlock[act_block_after].Mage(pact->m_actBlock[act_block_after], true, true))
		return false;
	//delete 系はノーチェック
	m_actBlock[act_block_befor].Mage(pact->m_actBlock[act_block_befor], false);
	return true;
}
void CActionLgc::AfterCopy(CActionLgc *pact) 
{
	m_actBlock[act_block_after].Copy(pact->m_actBlock[act_block_after]);
}
/////////////////////////////////////////////////////////////////////////////
// CAction
CAction::CAction()
{
	m_act = act_nop;
	m_pstring = null;
	m_pactLgc = null;
}
CAction::CAction(ACT act)
{
	m_act = act;
	m_pstring = null;
	m_pactLgc = null;
}
CAction::~CAction()
{
	if (m_pstring)
		delete m_pstring;
	if (m_pactLgc)
		delete m_pactLgc;
}
void CAction::push_vk(ACT act, const VK &vk)
{
	assert(m_act==act_nop || m_act == act);
	m_act = act;
	m_vk = vk;
}

bool CAction::push_char(const BYTE byte, int nRep)
{
	if (byte==VK_ESCAPE)
		return false;
	if (m_pstring==null)
		m_pstring = new string;
	m_act = act_input;
	if (!IsDelete(byte)) {
		if (byte==VK_RETURN) {
			assert(nRep);
			//assert(m_pstring->size()==0);
			//　キーボードバッファにいっぱいある場合は、RETRUNの前に文字列有ってもしょうがない
			m_pstring->append(1, (char)VK_RETURN);
			m_pstring->append(1, (char)VK_NL);
		}else
			m_pstring->append(nRep, (char)byte);
	}
	return true;
}

bool CAction::push_string(ACT act, const char *sz, OBJOFF len)
{
	if (m_pstring==null)
		m_pstring = new string;
	m_act = act; //act_input;
	m_pstring->append(sz, len);
	return true;
}

bool CAction::IsReturnAction()
{
	assert(m_pstring /*&& m_pstring->size()>0*/);
	return m_act == act_input && m_pstring->size()>0 &&
#if bug_111201
		m_pstring->find(VK_RETURN)!=string::npos;
#else
		(m_pstring->find(VK_RETURN)!=string::npos || m_pstring->find(VK_NL)!=string::npos);
#endif
}

bool CAction::push_ime_string(const string &str, bool bResult)
{
	assert(m_pstring==null);
	m_pstring = new string;
	if (bResult) {
		m_act = act_input_ime_result;
	}else{
		m_act = act_input_ime;
	}
	*m_pstring = str;
	return true;
}
void CAction::clear_str()
{
	if (m_pstring)
		delete m_pstring;
	m_pstring = null; 
}
bool CAction::IsEnableMarge(CAction *pact)
{
	return (m_act == pact->m_act && (m_pactLgc!=null && pact->m_pactLgc!=null)
		&& m_act != act_past);
}
bool CAction::Mage(CAction *pact)
{
	assert(m_act == pact->m_act);
	assert(m_pactLgc && pact->m_pactLgc);
	if (m_act == act_input_ime || m_act == act_input_ime_result) {
		m_pactLgc->AfterCopy(pact->m_pactLgc);
		return true;
	}
	return m_pactLgc->Mage(pact->m_pactLgc);
}
int CAction::HowMultiLineChange()
{
	if (m_pactLgc) {
		CActionBlock *pBef, *pAft;
		pBef = &m_pactLgc->m_actBlock[CActionLgc::act_block_befor];
		pAft = &m_pactLgc->m_actBlock[CActionLgc::act_block_after];
		if (pBef->m_string.size()>0 || pAft->m_string.size()>0)
			return 0;
		return pAft->m_sellsLgc.size() - pBef->m_sellsLgc.size();
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//CActions
void CActions::push_action(CAction *paction, bool bMargeAble)
{
	assert(!paction->IsEmpty());
	//m_idx以後のRedoエリアはクリアされる
	if (m_idx<size())
		Erase(m_idx);
	if (bMargeAble && IsEnableMarge(paction)) {
		if (at(m_idx-1)->Mage(paction))
			delete paction;	//eate action
		else
			push_back(paction);
	}else{
		push_back(paction);
	}
	m_idx = size();
	m_bMargeFlg = true;
}
void CActions::Erase(OBJOFF befor/*=NPOS*/, OBJOFF last/*=NPOS*/)
{
	if (befor==NPOS)
		befor = SIZEMAC(m_idx);
	if (last==NPOS)
		last = size();
	if (last==0)
		return;
	assert(befor<last && last<=size());
	for(OBJOFF row=befor; row<last; row++) {
		delete at(row);
	}
	erase(row2iter(befor), row2iter(last));
	m_idx = size();
}

//行単位で操作する
//元がメインファイル行だったら
//<insert>
//undo:prev line sell
//delete:main file lines sell
//insert:edit file lines sell
//<delete character>
//undo:prev line sell
//delete:main file lines sell
//insert:edit file lines sell
//<delete lines>
//undo:prev line sell
//delete:main file lines sell
//<replace>
//undo:prev line sell
//delete:main file lines sell
//insert:edit file lines sell
//
//元がエディトファイル行だったら
//<insert chars>
//redo:input characters
//undo:(ｷｬﾗｸﾀ自体を消去)
//insert:edit file sell
//<insert lines>
//undo:prev line sell
//insert:edit file lines sell
//<delete chars>
//undo:delete characters
//delete:edit file sell
//<delete lines>
//undo:prev line sell
//delete:edit file sell
//<replace chars>
//undo:old characters
//delete:edit file sell
//insert:edit file sell
//<replace lines>
//delete:edit file sell
//insert:edit file sell
//
//
