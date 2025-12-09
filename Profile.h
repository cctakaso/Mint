// Profile.h : ヘッダー ファイル
//
#ifndef __PROFILE_H__
#define __PROFILE_H__
#include "util.h"
/////////////////////////////////////////////////////////////////////////////
// CProfile 
class CProfile
{
public:
	CProfile();           // 動的生成に使用されるプロテクト コンストラクタ
//	CProfile(const char *szProfile, const char *szProfileDirDir=NULL) {
//		VERIFY(SetProfile(szProfile, szProfileDirDir)); }
	~CProfile();

// アトリビュート
protected:
	string	m_strProfile_nameonly;
	string	m_strProfile;
	CArchive *m_pArc;
	CFile	m_fileArc;
// オペレーション
public:
	enum {
		__read=CArchive::load,
		__write=CArchive::store
		};

	//初期化メンバー --------->
	bool SetProfile(const char *szProfile, const char *szProfileDirDir=null, const char*szExtent=null);
	bool ResetArcMode(UINT mode=CArchive::load, bool bCrate=true);
	const char *GetSzProfile() {
		return m_strProfile.c_str(); }
	const char *GetSzProfileNameOnly() {
		return m_strProfile_nameonly.c_str(); }

	//通常プロファイラ特有のメンバー --------->
	bool WriteString(const char *szSect, const char *szKey, const char *szVal);
	bool WriteString(const char *szSect, const int iKey, const char *szVal) {
		return WriteString(szSect, itostring(iKey).c_str(), szVal); }
	bool WriteInt(const char *szSect, const char *szKey, const int val);
	//bool WriteInt(const char *szSect, const int iKey, const int val) {
	//	return WriteInt(szSect, itostring(iKey).c_str(), val); }
	bool WriteBool(const char *szSect, const char *szKey, const bool val) {
		return WriteInt(szSect, szKey, val ? 1:0); }
	//bool WriteBool(const char *szSect, const int iKey, const bool val) {
	//	return WriteBool(szSect, itostring(iKey).c_str(), val); }
	bool WriteBytes(const char *szSect, const char *szKey, const void *pval, int size);

	string ReadString(const char *szSect, const char *szKey, const char *def="");
	string ReadString(const char *szSect, const int iKey, const char *def="") {
		return ReadString(szSect, itostring(iKey).c_str(), def); }
	int ReadInt(const char *szSect, const char *szKey, int def = 0);
	//int ReadInt(const char *szSect, const int iKey, int def = 0) {
	//	return ReadInt(szSect, itostring(iKey).c_str(), def); }
	bool ReadBool(const char *szSect, const char *szKey, bool def = false) {
		return ReadInt(szSect, szKey, def ? 1:0)==1 ? true:false; }
	//bool ReadBool(const char *szSect, const int iKey, bool def = false) {
	//	return ReadBool(szSect, itostring(iKey).c_str(), def); }

	//アーカイバ特有のメンバー --------->
	void ReadWriteString(const int mode, string &str);
	void ReadWriteInt(int mode, int &val);
	void ReadWriteBool(int mode, bool &val);
	void ReadWriteBytes(int mode, void *pval, int size);
	void ResetArc();
protected:
	bool WriteString(const char *szVal,bool bthrow=true);
	string ReadString(bool bthrow=true);
	bool WriteInt(const int val,bool bthrow=true);
	int ReadInt(bool bthrow=true);
	bool WriteBool(const bool val,bool bthrow=true) {
		return WriteInt(val ? 1:0, bthrow); }
	bool ReadBool(bool bthrow=true) {
		return ReadInt(bthrow)==1 ? true:false; }
	bool WriteBytes(const void *pval, int size,bool bthrow=true);
	bool ReadBytes(void *pval, const int size, bool bthrow=true);

	bool IsOpen_ArcFile();
	bool SetArc(UINT mode, bool bCrate);
	DWORD m_sizefileArc;
};
extern CProfile theProfile;
/////////////////////////////////////////////////////////////////////////////
#endif //__PROFILE_H__
