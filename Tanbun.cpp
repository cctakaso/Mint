///////////////////////////////////////////////
//Tanbun.cpp

#include "StdAfx.h"
#include "The.h"
#include "Tanbun.h"
#include "Profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

///////////////////////////////////////////////
extern CTheApp theApp;
CProfile theTanbunProfile;
///////////////////////////////////////////////
//CTanbunData
CTanbunData::CTanbunData()
{
	m_bEnable = true;
	//m_strData = "";
	m_fmt = FMT_NO;
}
CTanbunData::CTanbunData(const char* szData, const int fmt, const bool bEnable)
{
	m_bEnable = bEnable;
	m_strData = szData;
	m_fmt = fmt;
}
CTanbunData::~CTanbunData()
{
}
bool CTanbunData::ReadWriteProfile(int mode, CProfile &profile)
{
	profile.ReadWriteBool(mode, m_bEnable);
	profile.ReadWriteString(mode, m_strData);
	profile.ReadWriteInt(mode, m_fmt);
	return true;
}
const CTanbunData& CTanbunData::operator =(const CTanbunData &src)
{
	m_bEnable = src.m_bEnable;
	m_strData = src.m_strData;
	m_fmt = src.m_fmt;
	return *this;
}
#ifdef _DEBUG
void CTanbunData::Dump()
{
	TRACE("    bEnable=%d, str=%s, fmt=%d", m_bEnable, m_strData.c_str(), m_fmt);
}
#endif
///////////////////////////////////////////////
//CTanbun
CTanbun::CTanbun()
{
	m_pdata = null;
	m_pGroup = null;
}
CTanbun::CTanbun(CTanbunData *pdata)
{
	assert(pdata);
	m_pdata = pdata;
	m_pGroup = null;
}
CTanbun::~CTanbun()
{
	if (m_pdata)
		delete m_pdata;
	if (m_pGroup)
		delete m_pGroup;
}
bool CTanbun::ReadWriteProfile(int mode, CProfile &profile)
{
	if (mode==CProfile::__read) {
		if (m_pdata)
			delete m_pdata;
		m_pdata = new CTanbunData;
	}
	m_pdata->ReadWriteProfile(mode, profile);
	if (m_pGroup)
		m_pGroup->ReadWriteProfile(mode, profile);
	return true;
}
const CTanbun& CTanbun::operator =(const CTanbun &src)
{
	if (src.m_pdata) {
		if (!m_pdata)
			m_pdata = new CTanbunData;
		*m_pdata = *src.m_pdata;
	}else{
		if (m_pdata)
			delete m_pdata;
		m_pdata =null;
	}
	if (src.m_pGroup) {
		if (!m_pGroup)
			m_pGroup = new CTanbuns;
		*m_pGroup = *src.m_pGroup;
	}else{
		if (m_pGroup)
			delete m_pGroup;
		m_pGroup =null;
	}
	return *this;
}
#ifdef _DEBUG
void CTanbun::Dump()
{
	if (m_pdata) {
		TRACE("  pdata--->\n");
		m_pdata->Dump();
	}
	if (m_pGroup) {
		TRACE("  pGroup--->\n");
		m_pGroup->Dump();
	}
}
#endif
///////////////////////////////////////////////
//CTanbuns
CTanbuns::CTanbuns()
{
	m_bEnable = true;
}
CTanbuns::~CTanbuns()
{
	for (iterator iter=begin(); iter!=end(); iter++) {
		CTanbun *pTanbun = (*iter).second;
		if (pTanbun)
			delete pTanbun;
	}
	clear();
}
#ifdef _DEBUG
void CTanbuns::Dump()
{
	TRACE("CTanbun  size = %d --->\n", size());
	int i=0;
	for (iterator iter=begin(); iter!=end(); iter++, i++) {
		TRACE(" CTanbun[%d(%s)]= %s\n", i, (*iter).first.c_str(), m_bEnable ? "Enable":"Disable");
		CTanbun *pTanbun = (*iter).second;
		pTanbun->Dump();
	}
}
#endif
const CTanbuns& CTanbuns::operator =(/*const*/ CTanbuns &src)
{
	CTanbun *pTanbun;
	iterator iter;
    for (iter=begin(); iter!=end(); iter++) {
		CTanbun *pTanbun = (*iter).second;
		delete pTanbun;
	}
	clear();
	for (iter=src.begin(); iter!=src.end(); iter++) {
		string strName = (*iter).first;
		pTanbun = new CTanbun;
		*pTanbun = *(*iter).second;
		//(*this)[strName] = pTanbun;
		insert(value_type(strName, pTanbun));
	}
	return *this;
}

bool CTanbuns::ReadWriteProfile(int mode)
{
	//’Z•¶“o˜^Ì§²Ù‚Ì“Ç‚Ý‘‚«-->
	VERIFY(theTanbunProfile.SetProfile("’Z•¶“o˜^", 
		GetModuleDirDir(theApp.m_hInstance).data(),
		"DDD"));
	if (!theTanbunProfile.ResetArcMode(mode, true)) { //bCreate)) {
		return  false;
	}
	//if (bCreate && mode==CProfile::__read)
	//	return true; //ƒfƒtƒHƒ‹ƒg‚Ì’l‚ð‚»‚Ì‚Ü‚ÜŽg‚¤
	try {
		VERIFY(ReadWriteProfile(mode, theTanbunProfile));
	}catch(CException *e) {
		e->Delete();
		theTanbunProfile.ResetArc();
		return false;
	}
	theTanbunProfile.ResetArc();
	//<----’Z•¶“o˜^Ì§²Ù‚Ì“Ç‚Ý‘‚«
	return true;
}
bool CTanbuns::ReadWriteProfile(int mode, CProfile &profile)
{
	string strName;
	CTanbun *pTanbun;
	
	if (mode==CProfile::__write) {
		int len = size();
		profile.ReadWriteInt(mode, len);
		for (iterator iter=begin(); iter!=end(); iter++) {
			strName = (*iter).first;
			profile.ReadWriteString(mode, strName);
			pTanbun = (*iter).second;
			pTanbun->ReadWriteProfile(mode, profile);
		}
	}else{
		int len;
		profile.ReadWriteInt(mode, len);
		for (int i=0; i<len; i++) {
			profile.ReadWriteString(mode, strName);
			pTanbun = new CTanbun;
			pTanbun->ReadWriteProfile(mode, profile);
			//(*this)[strName] = pTanbun;
			insert(value_type(strName, pTanbun));
		}
	}
	return true;
}

CTanbuns *CTanbuns::CopyNew()
{
#ifdef _DEBUG
//	Dump();
#endif
	CTanbuns *pTanbuns = new CTanbuns;
	*pTanbuns = *this;
#ifdef _DEBUG
//	pTanbuns->Dump();
#endif
	return pTanbuns;
}

bool CTanbuns::AddNewEntry(const char* szName, CTanbunData *pdata)
{
	assert(szName);
	assert(pdata);
	CTanbun *pTanbun = new CTanbun(pdata);
	size_type len = size();
	string strName = szName;
	insert(value_type(strName, pTanbun));
	if (len== size()) {
		//ƒCƒ“ƒT[ƒgŽ¸”s‚Ìê‡‚ª‚ ‚é
		//ƒL[‚ªd•¡‚µ‚Ä‚¢‚éê‡‚ÍA’Ç‰Á‚µ‚È‚¢
		delete pTanbun;
		return false;
	}
	return true;
}
CTanbunData *CTanbuns::FindData(const string &szKey)
{
	iterator iter = find(szKey);
	if (iter != end()) {
		return (*iter).second->m_pdata;
	}
	return null;
}

bool CTanbuns::UpdateEntry(int index, const char *szKey, const char *szData, const int fmt)
{
	iterator iter = Index2Iter(index);
	if (iter != end()) {
		CTanbun *pTanbun = (*iter).second;
		CTanbunData *pdata = pTanbun->m_pdata;
		if ((*iter).first != szKey) {
			erase(iter);
			string strName = szKey;
			pdata->m_strData = szData;
			pdata->m_fmt = fmt;
			size_type len = size();
			insert(value_type(strName, pTanbun));
			if (len== size()) {
				return false;
			}
		}else{
			pdata->m_strData = szData;
			pdata->m_fmt = fmt;
		}
		return true;
	}
	return false;
}
bool CTanbuns::RemoveEntry(int index)
{
	iterator iter = Index2Iter(index);
	if (iter != end()) {
		CTanbun *pTanbun = (*iter).second;
		if (pTanbun)
			delete pTanbun;
		erase(iter);
		return true;
	}
	return false;
}

CTanbuns::iterator CTanbuns::Index2Iter(int index)
{
	int i=0;
	for (iterator iter=begin(); iter!=end(); iter++, i++) {
		if (i==index)
			return iter;
	}
	return end();
}

#endif//#ifdef SHAR
