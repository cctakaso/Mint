///////////////////////////////////////////////
//Keyword.cpp

#include "StdAfx.h"
#include "Keyword.h"
#include "Profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
enum {
	keycpos_no,
	keycpos_rowend,
	keycpos_left,
	keycpos_right,
};
*/
///////////////////////////////////////////////
//CKeywordConf
CKeywordConf::CKeywordConf()
{
	m_bEnable = true;
	m_color = RGB(0,0,0);
	m_bWord = true;		//単語単位
	m_bCase = true;		//完全一致か否か
	m_bUnderline = false;	//下線強調
	m_keycpos = keycpos_no;			//行末まで強調==keycpos_rowend
	m_bStartLight = true;		//開始キーワード強調
	//m_strKeyEnd = "";		//終了キーワード文字列
	m_bEndLight = false;			//終了キーワード強調
	m_bIgnoreCRLF = false;	//改行を無視して終了キーワードを探す
	//m_strTagStart = "";
	//m_strTagEnd = "";
}
CKeywordConf::~CKeywordConf()
{
}
bool CKeywordConf::ReadWriteProfile(int mode, CProfile &profile)
{
	profile.ReadWriteBool(mode, m_bEnable);
	profile.ReadWriteBytes(mode, &m_color, sizeof(m_color));
	profile.ReadWriteBool(mode, m_bWord);
	profile.ReadWriteBool(mode, m_bCase);
	profile.ReadWriteBool(mode, m_bUnderline);
	profile.ReadWriteInt(mode, m_keycpos);
	profile.ReadWriteBool(mode, m_bStartLight);
	profile.ReadWriteString(mode, m_strKeyEnd);
	profile.ReadWriteBool(mode, m_bEndLight);
	profile.ReadWriteBool(mode, m_bIgnoreCRLF);
	profile.ReadWriteString(mode, m_strTagStart);
	profile.ReadWriteString(mode, m_strTagEnd);
	return true;
}
const CKeywordConf& CKeywordConf::operator =(const CKeywordConf &src)
{
	m_bEnable = src.m_bEnable;
	m_color = src.m_color;
	m_bWord = src.m_bWord;
	m_bCase = src.m_bCase;
	m_bUnderline = src.m_bUnderline;
	m_keycpos = src.m_keycpos;
	m_bStartLight = src.m_bStartLight;
	m_strKeyEnd = src.m_strKeyEnd;
	m_bEndLight = src.m_bEndLight;
	m_bIgnoreCRLF = src.m_bIgnoreCRLF;
	m_strTagStart = src.m_strTagStart;
	m_strTagEnd = src.m_strTagEnd;
	return *this;
}
#ifdef _DEBUG
void CKeywordConf::Dump()
{
	TRACE("    bEnable=%d, color=%x, bWord=%d, bCase=%d, bUnderline=%d,\n"\
		"    keycpos=%d, bStartLight=%d, strEndKey=%s, bEndLight=%d, bIgnoreCRLF=%d\n"\
		"    tagStart=%s, tagEnd=%s\n",
		m_bEnable, m_color, m_bWord, m_bCase, m_bUnderline,
		m_keycpos, m_bStartLight, m_strKeyEnd.c_str(), m_bEndLight, m_bIgnoreCRLF,
		m_strTagStart.c_str(), m_strTagEnd.c_str());
}
#endif
///////////////////////////////////////////////
//CKeyword
CKeyword::CKeyword()
{
	m_pconf = null;
	m_pKeywordParent = null;
}
CKeyword::CKeyword(const char *szGroup, CKeywordConf *pconf)
{
	if (IsValidDataSz(szGroup))
		m_strGroup = szGroup;
	assert(pconf);
	m_pconf = pconf;
	m_pKeywordParent = null;
}
CKeyword::~CKeyword()
{
	if (!m_pKeywordParent && m_pconf) {
		delete m_pconf;
	}
	m_pconf = null;
}

bool CKeyword::ReadWriteProfile(int mode, CProfile &profile)
{
	profile.ReadWriteString(mode, m_strGroup);
	if (mode==CProfile::__read) {
		if (!m_pKeywordParent && m_pconf)
			delete m_pconf;
		m_pconf = new CKeywordConf;
	}
	m_pconf->ReadWriteProfile(mode, profile);
	return true;
}
const CKeyword& CKeyword::operator =(const CKeyword &src)
{
	assert(!m_pKeywordParent && !src.m_pKeywordParent);
	m_strGroup = src.m_strGroup;
	if (src.m_pconf) {
		if (!m_pconf)
			m_pconf = new CKeywordConf;
		*m_pconf = *src.m_pconf;
	}else{
		if (m_pconf)
			delete m_pconf;
		m_pconf =null;
	}
	return *this;
}

#ifdef _DEBUG
void CKeyword::Dump()
{
	TRACE("  strGroup=%s, pconf--->\n", m_strGroup.c_str());
	m_pconf->Dump();
}
#endif
///////////////////////////////////////////////
//CKeywords
CKeywords::CKeywords()
{
}
CKeywords::~CKeywords()
{
	Delete();
}
void CKeywords::Delete()
{
	for (iterator iter=begin(); iter!=end(); iter++) {
		CKeyword *pkeyword = (*iter).second;
		delete pkeyword;
	}
	clear();
}
#ifdef _DEBUG
void CKeywords::Dump()
{
	TRACE("CKeyword  size = %d --->\n", size());
	int i=0;
	for (iterator iter=begin(); iter!=end(); iter++, i++) {
		TRACE(" CKeyword[%d(%s)]=\n", i, (*iter).first.c_str());
		CKeyword *pkeyword = (*iter).second;
		pkeyword->Dump();
	}
}
#endif
const CKeywords& CKeywords::operator =(/*const*/ CKeywords &src)
{
	CKeyword *pKeyword;
	Delete();
	iterator iter = src.begin(); //STR2KEYWORD::
	for (; iter!=src.end(); iter++) {
		string strName = (*iter).first;
		pKeyword = new CKeyword;
		*pKeyword = *(*iter).second;
		(*this)[strName] = pKeyword;
	}
	return *this;
}

bool CKeywords::ReadWriteProfile(int mode, CProfile &profile)
{
	string strName;
	CKeyword *pKeyword;
	
	if (mode==CProfile::__write) {
		int len = size();
		profile.ReadWriteInt(mode, len);
		for (iterator iter=begin(); iter!=end(); iter++) {
			strName = (*iter).first;
			profile.ReadWriteString(mode, strName);
			pKeyword = (*iter).second;
			pKeyword->ReadWriteProfile(mode, profile);
		}
	}else{
		int len = size();
		profile.ReadWriteInt(mode, len);
		for (int i=0; i<len; i++) {
			profile.ReadWriteString(mode, strName);
			pKeyword = new CKeyword;
			pKeyword->ReadWriteProfile(mode, profile);
			(*this)[strName] = pKeyword;
		}
	}
	return true;
}

CKeywords *CKeywords::CopyNew()
{
#ifdef _DEBUG
//	Dump();
#endif
	CKeywords *pKeywords = new CKeywords;
	*pKeywords = *this;
#ifdef _DEBUG
//	pKeywords->Dump();
#endif
	return pKeywords;
}

void CKeywords::Copy_OnlyEnable(CKeywords* &pKeywords_word, CKeywords* &pKeywords_unword)
{
#ifdef _DEBUG
//	Dump();
#endif
	if (pKeywords_word)
		delete pKeywords_word;
	if (pKeywords_unword)
		delete pKeywords_unword;

	if (size()==0) {
		pKeywords_word = null;
		pKeywords_unword = null;
		return;
	}

	pKeywords_word = new CKeywords;
	pKeywords_unword = new CKeywords;
	for (iterator iter=begin(); iter!=end(); iter++) {
		string strName = (*iter).first;
		CKeyword *pKeyword = (*iter).second;
		if (pKeyword->m_pconf->m_bEnable) {
			if (pKeyword->m_pconf->m_bWord)
				pKeywords_word->AddBreakEntry(strName, pKeyword);//(*pKeywords_word)[strName] = pKeyword_new; //アドレスをコピーする
			else
				pKeywords_unword->AddBreakEntry(strName, pKeyword);//(*pKeywords_unword)[strName] = pKeyword_new; //アドレスをコピーする
		}
	}
	if (pKeywords_word->size()==0) {
		delete pKeywords_word;
		pKeywords_word = null;
	}

	if (pKeywords_unword->size()==0) {
		delete pKeywords_unword;
		pKeywords_unword = null;
	}

#ifdef _DEBUG
//	pKeywords_word->Dump();
//	pKeywords_unword->Dump();
#endif
}

bool CKeywords::AddNewEntry(string strName)
{
	CKeyword *pkeyword = new CKeyword;
	size_type len = size();
	insert(value_type(strName, pkeyword));
	if (len== size()) {
		//インサート失敗の場合がある
		//キーが重複している場合は、追加しない
		delete pkeyword;
		return false;
	}
	return true;
}
void CKeywords::AddBreakEntry(string strName, CKeyword *pKeyword)
{
	if (pKeyword->m_strGroup.size()>0)
		strName = pKeyword->m_strGroup;
	string str_part;
	CKeywordConf *pconf = new CKeywordConf;
	*pconf = *pKeyword->m_pconf;
	CKeyword *pkeyword_part = null;
	CKeyword *pkeyword_parent = null;
	while((str_part=GetPartString(strName, ' ')).size()>0) {
		pkeyword_part = new CKeyword(null/*"\1"*/, pconf); //'\1'ｸﾞﾙｰﾌﾟﾏｰｸ
		pkeyword_part->m_pKeywordParent = pkeyword_parent;
		size_type len = size();
		insert(value_type(str_part, pkeyword_part));
		if (len== size()) {
			//インサート失敗の場合がある
			//キーが重複している場合は、追加しない
			delete pkeyword_part;
			pkeyword_part = null;
		}else{
			if (!pkeyword_parent)
				pkeyword_parent = pkeyword_part;
		}
	}
}
void CKeywords::Copy(CKeywords *pKeywords, bool bUpdate)
{
#ifdef _DEBUG
//	pKeywords->Dump();
#endif
	assert(pKeywords);
	if (bUpdate) {
		*this = *pKeywords;
	}
#ifdef _DEBUG
//	Dump();
#endif
}


/*
step1:アーカイブからの読み込み時
	ｸﾞﾙｰﾋﾟﾝｸﾞされている
	CKeywords colorEntrys;
step2:通常動作時
	ｸﾞﾙｰﾋﾟﾝｸﾞを別に取っておいて削除し
	分散させて各項目を追加する
	CKeywords colorEntrys;
	CKeywords colorEntrys_gruop;
step3:ダイアログの表示及び、アーカイブへの書き込み時
	分散させたｸﾞﾙｰﾋﾟﾝｸﾞを再びｸﾞﾙｰﾌﾟ化する
	
	CKeywords colorEntrys_gruop;の各項目に対して
	m_pconfをキー、colorEntrysから同じ値のm_pconfを削除していく。
*/
CKeywords::iterator CKeywords::FindWordKeyword(const BYTE *sz)
{
	string strword;
	if (IsSJIS_1stByte(*sz))
		strword = PickupDbcsWord(sz);
	else
		strword = PickupSbcsWord(sz);

	iterator iter = find(strword);
	if (iter!=end())
		return iter;
	else{
		_strupr((char*)strword.data());
		iter = find(strword);
		if (iter!=end() && !(*iter).second->m_pconf->m_bCase)
			return iter;
	}
	return end();
}

CKeywords::iterator CKeywords::FindUnwordKeyword(const BYTE *sz)
{
	string strName;
	CKeyword *pKeyword;
	for (iterator iter=begin(); iter!=end(); iter++) {
		strName = (*iter).first;
		pKeyword = (*iter).second;
		if ((pKeyword->m_pconf->m_bCase ? strncmp(strName.data(), (char*)sz, strName.size())==0:
								strnicmp(strName.data(), (char*)sz, strName.size())==0)) {
			return iter;
		}
	}
	return end();
}
