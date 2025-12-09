// Profile.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProfile theProfile;
/////////////////////////////////////////////////////////////////////////////
// CProfile
CProfile::CProfile()
{
	m_pArc = null;
	m_sizefileArc = 0;
}

CProfile::~CProfile()
{
	if (m_pArc) {
		ResetArc();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProfile メッセージ ハンドラ
// プロファイルを読み書きする。
bool CProfile::SetProfile(const char *szProfile, const char *szProfileDirDir, const char*szExtent)
{
//	assert(!IsOpen_ArcFile());

	assert(IsValidDataSz(szProfile));
	m_strProfile_nameonly = szProfile;

	if (szProfileDirDir) {
		m_strProfile = szProfileDirDir;
		m_strProfile += szProfile;
	}else
		m_strProfile = szProfile;
	if (szExtent) {
		m_strProfile += '.';
		m_strProfile += szExtent;
	}
	if (m_strProfile.size()==0)
		return false;

	return true;
}

bool CProfile::ResetArcMode(UINT mode, bool bCreate)
{
	if (m_pArc && IsOpen_ArcFile()) {
		if (mode==CArchive::load ? m_pArc->IsLoading():m_pArc->IsStoring())
			return true;
	}

	return SetArc(mode, bCreate);
}
bool CProfile::WriteString(const char *szSect, const char *szKey, const char *szVal)
{
	return WritePrivateProfileString(szSect, szKey, szVal, m_strProfile.c_str())>0;
}
bool CProfile::WriteInt(const char *szSect, const char *szKey, const int val)
{
	return WritePrivateProfileString(szSect, szKey, itostring(val).c_str(), m_strProfile.c_str())>0;
}
string CProfile::ReadString(const char *szSect, const char *szKey, const char *def)
{
	string str = "";
	char sz[1024];
	if (GetPrivateProfileString(szSect, szKey, def, sz, sizeof(sz), m_strProfile.c_str())>0)
		str = sz;
	return str;
}
int CProfile::ReadInt(const char *szSect, const char *szKey, int def)
{
	return GetPrivateProfileInt(szSect, szKey, def, m_strProfile.c_str());
}
//////////////////////////////////////////////////////////
//Arc->
void CProfile::ReadWriteString(const int mode, string &str) 
{
	if (mode==__read)
		str = ReadString();
	else
		WriteString(str.data());
}
string CProfile::ReadString(bool bthrow)
{
	string str = "";
	try {
	if (m_pArc) {
		CString cstr;
		assert(m_pArc->IsLoading());
		//I don't use read/write Stirng function!
		//m_pArc->ReadString(cstr);
		LONG len;
		*m_pArc >> len;
		if (len>m_sizefileArc)
			throw new CUserException;
		else  if (len>0) {
			void *p = new BYTE[len];
			m_pArc->Read((void*)p, len); //値
			str.append((char*)p, len);
			delete [] p;
		}
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return str;
}
bool CProfile::WriteString(const char *szVal, bool bthrow)
{
	try{
	if (m_pArc) {
		int len = strlen(szVal);
		assert(m_pArc->IsStoring());
		//I don't use read/write Stirng function!
		//m_pArc->WriteString(szVal);
		*m_pArc << len;
		if (len>0)
			m_pArc->Write((void*)szVal, len);
		return true;
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return false;
}
void CProfile::ReadWriteInt(int mode, int &val)
{
	if (mode==__read)
		val = ReadInt();
	else
		WriteInt(val);
}
void CProfile::ReadWriteBool(int mode, bool &val)
{
	if (mode==__read)
		val = ReadBool();
	else
		WriteBool(val);
}
int CProfile::ReadInt(bool bthrow)
{
	try {
	if (m_pArc) {
		LONG value;
		assert(m_pArc->IsLoading());
		*m_pArc >> value;
		return value;
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return 0;
}
bool CProfile::WriteInt(const int val, bool bthrow)
{
	try{
	if (m_pArc) {
		assert(m_pArc->IsStoring());
		*m_pArc << (LONG)val;	//値
		return true;
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return false;
}
void CProfile::ReadWriteBytes(int mode, void *pval, int size)
{
	if (mode==__read)
		ReadBytes(pval, size);
	else
		WriteBytes(pval, size);
}
bool CProfile::ReadBytes(void *pval, const int size, bool bthrow)
{
	try{
	if (m_pArc) {
		assert(m_pArc->IsLoading());
		LONG value;
		*m_pArc >> value;
		assert(value==size);
		if (value>m_sizefileArc)
			throw new CUserException ;
		else if (value>0)
			m_pArc->Read(pval, value); //値
		return true;
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return false;
}
bool CProfile::WriteBytes(const void *pval, int size, bool bthrow)
{
	try{
	if (m_pArc) {
		assert(m_pArc->IsStoring());
		*m_pArc << (LONG)size;
		if (size>0)
			m_pArc->Write(pval, size);	//値
		return true;
	}
	}catch(CException *e) {
		if (bthrow)
			throw;
		e->Delete();
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//
bool CProfile::IsOpen_ArcFile()
{
	CFileStatus status;
	return m_fileArc.GetStatus( status )==TRUE;
}

void CProfile::ResetArc()
{
	if (!m_pArc)
		return;
	m_sizefileArc = 0;
	try{
		if (!m_pArc->IsLoading()) {
			m_pArc->Flush();
			if (m_pArc->GetFile())
				m_pArc->GetFile()->Close();
		}
	}catch(CException *e) {
		e->Delete();
	}
	delete m_pArc;
	m_pArc = null;
	m_fileArc.Abort();
}
bool CProfile::SetArc(UINT mode, bool bCrate)
{
	assert(m_strProfile.size()>0);
	ResetArc();
	bool bExist = IsExistFile(m_strProfile.data());
	bool bSucess;

	for(int i=0; i<5; i++) {
		if( bSucess = (m_fileArc.Open( m_strProfile.data(), (mode == __read ?  CFile::modeRead:CFile::modeWrite) | ((!bExist && (bCrate || mode == __write)) ? CFile::modeCreate:0))!=0) )
			break;
		MyPeekMessage(NULL);
	}
	if (!bSucess) {
		#ifdef _DEBUG
		afxDump << "Unable to open file" << "\n";
		//AfxMessageBox("CProfile::SetArc() - Open Error");
		#endif
		return false;
	}

	try{
		m_sizefileArc = m_fileArc.GetLength();
	}catch(CException *e) {
		e->Delete();
	}

	//CArchive::load:CArchive::store
	m_pArc = new CArchive( &m_fileArc, mode);
	return m_pArc!=null;
}
