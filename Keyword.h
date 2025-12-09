#ifndef __KEYWORD_H__
#define __KEYWORD_H__
#include <functional> 
#include "profile.h"


enum {
	keycpos_no,
	keycpos_rowend,
	keycpos_left,
	keycpos_right,
};

class CKeywordConf
{
public:
	CKeywordConf();
	~CKeywordConf();
public:
	bool ReadWriteProfile(int mode, CProfile &profile);
#ifdef _DEBUG
	void Dump();
#endif
public:
	const CKeywordConf& operator =(const CKeywordConf &conf);
	bool		m_bEnable;
	COLORREF	m_color;
	bool		m_bWord;		//単語単位
	bool		m_bCase;		//完全一致
	bool		m_bUnderline;	//下線強調
	int			m_keycpos;			//行末まで強調==keycpos_rowend
	bool		m_bStartLight;	//開始キーワード強調
	string		m_strKeyEnd;	//終了キーワード文字列
	bool		m_bEndLight;	//終了キーワード強調
	bool		m_bIgnoreCRLF;	//改行を無視して終了キーワードを探す
	string		m_strTagStart;	//タグ初め
	string		m_strTagEnd;	//タグ終わり
};

class CKeyword
{
	friend class CKeywords;
	friend class CKeywordPage;
	friend class CViewEd;
public:
	CKeyword();
	CKeyword(const char *szGroup, CKeywordConf *pconf);
	~CKeyword();
#ifdef _DEBUG
	void Dump();
#endif
public:
	bool ReadWriteProfile(int mode, CProfile &profile);
	bool IsGroup() {
		return m_strGroup.size()>0; }
protected:
	const CKeyword& operator =(const CKeyword &src);
	string		m_strGroup;	//ｸﾞﾙｰﾌﾟ時にｴﾝﾄﾘｰされている 空白区切り
	CKeywordConf	*m_pconf;	//いつも必ずある。
	CKeyword		*m_pKeywordParent;		//m_pconf実態ポインタ
};
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
//first = key string!
//class CKeywords;
//typedef map<string, CKeyword *> STR2KEYWORD;

//const Compare& Compare(const string& s1, const string& s2)
//{
//   return s1 < s2 ? 1 : 0;
//}

class CKeywords :protected  map<string, CKeyword *>
//class CKeywords :protected  map<string, CKeyword *, const Compare& comp = Compare()>
//v1.36 7 error! ↓
//class CKeywords :protected  map<string, CKeyword *, greater_equal<string>, allocator<CKeyword *> >
{
	friend class CKeywordPage;
	friend class CViewEd;
public:
	CKeywords();
	~CKeywords();
#ifdef _DEBUG
	void Dump();
#endif
public:
	bool ReadWriteProfile(int mode, CProfile &profile);
	CKeywords *CopyNew();
	void Copy_OnlyEnable(CKeywords* &pkeywords_word, CKeywords* &pkeywords_unword);
	void Copy(CKeywords *pKeywords, bool bUpdate);
	bool AddNewEntry(string strName);

	void Delete();
	iterator FindWordKeyword(const BYTE *sz);
	iterator FindUnwordKeyword(const BYTE *sz);
private:
	const CKeywords& operator =(/*const*/ CKeywords &src);
	void AddBreakEntry(string strName, CKeyword *pKeyword);
	//key_compare key_comp() const {
	//	return s1 < s2 ? 1 : 0;
	//}
		//bool operator(const Key& x, const Key& y);
};
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


#endif //__KEYWORD_H__
