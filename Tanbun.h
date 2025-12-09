#ifndef __TANBUN_H__
#define __TANBUN_H__
#include "profile.h"

#ifdef SHAR

extern 	CProfile theTanbunProfile;
class CTanbunData
{
public:
	CTanbunData();
	CTanbunData(const char* szData, const int fmt=FMT_NO, const bool bEnable=true);
	~CTanbunData();
public:
	bool ReadWriteProfile(int mode, CProfile &profile);
#ifdef _DEBUG
	void Dump();
#endif
public:
	const CTanbunData& operator =(const CTanbunData &conf);
	enum {
		FMT_NO=-1,
		FMT_LEFT,
		FMT_CENTER,
		FMT_RIGHT,
	};
	bool		m_bEnable;
	string		m_strData;
	int			m_fmt;	//FMT_NO,FMT_LEFT,....
};

class CTanbun
{
	friend class CTanbuns;
	friend class CViewEd;
	friend class CTanbunEdit;
public:
	CTanbun();
	CTanbun(CTanbunData *pdata);
	~CTanbun();
#ifdef _DEBUG
	void Dump();
#endif
public:
	bool ReadWriteProfile(int mode, CProfile &profile);
	bool IsGroup() {
		return m_pGroup!=null; }
protected:
	const CTanbun& operator =(const CTanbun &src);
	CTanbunData	*m_pdata;	//いつも必ずある。
	CTanbuns	*m_pGroup;	//現在は未使用いつもnull
};

//first = key string!
//class CTanbuns;
class CTanbuns :protected multimap<string, CTanbun *>
{
	friend class CTanbun;
	friend class CTanbunPage;
	friend class CViewEd;
	friend class CTanbunEdit;
public:
	CTanbuns();
	~CTanbuns();
#ifdef _DEBUG
	void Dump();
#endif
public:
	bool ReadWriteProfile(int mode);
	CTanbuns *CopyNew();
	bool AddNewEntry(const char* szName, CTanbunData *pdata);
	bool UpdateEntry(int index, const char *szKey, const char *szData, const int fmt);
	bool RemoveEntry(int index);
	const CTanbuns& operator =(/*const*/ CTanbuns &src);
	CTanbunData *FindData(const string &szKey);
protected:
	bool		m_bEnable;
private:
	bool ReadWriteProfile(int mode, CProfile &profile);
	iterator Index2Iter(int index);
};
#endif //#ifdef SHAR


#endif //__TANBUN_H__
