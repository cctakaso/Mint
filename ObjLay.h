#ifndef __OBJLAY__H__
#define __OBJLAY__H__

#include "sell.h"
#include "ObjCash.h"
#include "hint.h"
#include "ViewEnv.h"

class CObjTbl;
class CViewEd;


class CObjLayTbl	:protected CObjCashFile
{
	friend class CObjTbl;
	friend class CViewEd;
	friend class CWinView;
	friend class CTheDoc;
	friend class CFindDlg;
	friend class CFormatPage;
	friend class CFormatCharPage;
	friend class CTheApp;
	friend class CMainFrame;
	friend class CFileSelectBar;
	friend class CTheDocTemplate;
public:
	CObjLayTbl();
	~CObjLayTbl();

protected:
	//initialize function
	CViewEnv *Attach_Env(CViewEnv *pViewEnv);
	void Initialize(CObjCashFile* pMainObj, CObjCashFile* pEditObj, const char *szfilename) {
		m_pMainObj = pMainObj;
		m_pEditObj = pEditObj;
		if (IsValidDataSz(szfilename))
			m_filename = szfilename;
	}
	int SaveData(CArchive& ar);
	string GetPathName() {
		return m_filename; }
	//object set & infomation function
	CUR_SIZE GetCurSize() const {
		return m_cursize; }
	bool FormatObj(bool bfirst, CLineHint *pHint, bool bfastReturn=true);

	//object sell!
	void SetCur(CUR &cur, bool bFree) {
		m_phint->SetCur(cur, bFree);
	}
	void RecalcCurSize();

	CActions& GetActions() {
		return m_actions; }
	string& getKnjcode() {
		return m_knjcode;
	}
#ifdef EDITMENU_OFLAY
	bool GetEditMenu() {
		return m_bEditMenu; }
	bool ChangeEditMenu() {
		return m_bEditMenu = !m_bEditMenu; }
#endif
private:
	bool		m_bFirstSave;	//takaso2010 move from CObjCashFile
	string		m_knjcode;		//takaso2010 move from CObjCashFile
	string		m_strTitle;
	CLineHint *m_phint;
	CObjCashFile*  m_pMainObj;
	CObjCashFile*  m_pEditObj;
	CUR_SIZE	m_cursize;
	CActions	m_actions;
#ifdef EDITMENU_OFLAY
	bool m_bEditMenu;
#endif
private:
	void setIsFirstSave(bool f) {//takaso2010 
		m_bFirstSave = f;
	}
	void setKnjcode(const char *sz) {
		m_knjcode = sz;
	}
	void ResetCash() {
		m_pMainObj->ResetCash();
		m_pEditObj->ResetCash(); }
};

#endif //__OBJLAY__H__
