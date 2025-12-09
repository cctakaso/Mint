/*    PortTool v2.2     crejit.h          */
#ifdef SHAR

// crejit.h : ヘッダー ファイル
//
#ifndef __CREJIT_H_
#define __CREJIT_H_

#include "resource.h"
#include "version.h"
/////////////////////////////////////////////////////////////////////////////
// CCrejit ダイアログ
//efine CRJ_TIME_INI       20
//#define CRJ_TIME_LENGTH	7
//#define CRJ_TIME_LIMIT		604800		// 7*24*60*60
#define CRJ_TIME_LIMIT		(3*604800)		// 3*7*24*60*60


#define CRJ_ER_OK			0
#define CRJ_ER_PASSWORD		1
#define CRJ_ER_TIMERREQ		2
#define CRJ_ID_STR			"USER_ID"

class CCrejit : public CDialog
{
// コンストラクタ
public:
	CCrejit(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCrejit)
	enum { IDD = IDD_CREJIT };
	CStatic	m_csTime;
	CEdit	m_editUserID;
	CString	m_sUserID;
	CStatic	m_csTmpID;
	CString	m_strVersion;
	//}}AFX_DATA

	WORD FInitial();
	CString *GetTmpID() {
		return &m_sTmpID;	}

private:
	BOOL FValidID(CString *pcstr);
	BOOL FValidTmpID(CString *pcstr) {
		pcstr->MakeUpper();
		return (*pcstr == m_sTmpID);
		}
	void GetProfile();			// ini の読込み
	BOOL SetProfile();			// ini への書き込み
	BOOL WriteLimitTime(time_t ltime);


// ﾒﾝﾊﾞ変数
	UINT m_uiTimer;
	time_t m_tLimitTime;	// 使用有効期限
	CString m_sTmpID;		// ﾃﾝﾎﾟﾗﾘID
// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	// メッセージマップ関数の生成
	//{{AFX_MSG(CCrejit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif // __CREJIT_H_

#endif //#ifdef SHAR
