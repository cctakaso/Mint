/*    PortTool v2.2     CREJIT.CPP          */
// crejit.cpp : インプリメンテーション ファイル


//
//#include "stdlib.h"
#include "stdafx.h"
#include "The.h"
#include "crejit.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef SHAR

void BeepMessage(UINT uid)
{
	::MessageBeep(MB_ICONINFORMATION);
	AfxMessageBox(uid);
}
CString GetTimerEntryName()
{
	CString str, strVersion;
	int pos;
	str.LoadString(IDS_VERSION);
	pos = str.Find('.');
	if (pos>1) {
		CString strT = str.Mid(pos-2);
		pos = strT.Find('\n');
		if (pos>0) {
			strVersion = strT.Left(pos);
			strVersion.TrimLeft();
			strVersion.TrimRight();
		}
	}
	if (strVersion.GetLength()==0)
		strVersion = CRJ_TIME_STR;
	return strVersion;
}

/////////////////////////////////////////////////////////////////////////////
// CCrejit ダイアログ

CCrejit::CCrejit(CWnd* pParent /*=NULL*/)
	: CDialog(CCrejit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrejit)
	m_tLimitTime = 0;
	m_uiTimer = 0;
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
// CCrejit メッセージハンドラ
BEGIN_MESSAGE_MAP(CCrejit, CDialog)
	//{{AFX_MSG_MAP(CCrejit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

WORD CCrejit::FInitial()
{
	GetProfile();
	// ﾊﾟｽﾜｰﾄﾞﾁｪｯｸ
	if (!FValidID(&m_sUserID))
		{
		// ﾀｲﾏﾘｸｴｽﾄ
		return CRJ_ER_PASSWORD;
		}
	else
		return CRJ_ER_OK;
}



/*************************************************************************
-- Routine: BOOL CCrejit::WriteLimitTime(time_t ltime)
-- Description and Usage:
		iniファイルに使用有効期限を設定します。
		一度だけしか起動しません。
-- Arguments:
		time_t ltime : iniへ書き込む使用有効期限。 
-- Returns:
		成功した時は TRUE を、失敗した時は FALSE を返します。
-- Notes:
-- History:
		'94.12	- created		(M.Design)
*************************************************************************/
BOOL CCrejit::WriteLimitTime(time_t ltime)
{
	char vz[256];
	_ltoa(ltime, vz, 10); 
	AfxGetApp()->WriteProfileString("USER", GetTimerEntryName(), vz);
	return TRUE;
}


/*************************************************************************
-- Routine: void CCrejit::GetProfile()
-- Description and Usage:
		ユーザーIDを取得します。
-- Arguments:
		char* lpUserID : ユーザーIDの入った文字列のポインタ。
-- Returns:
		成功した時は TRUE を、失敗した時は FALSE を返します。
-- Notes:
-- History:
		'94.12	- created		(M.Design)
*************************************************************************/

void CCrejit::GetProfile()
{

	m_sUserID = AfxGetApp()->GetProfileString("USER", CRJ_ID_STR, NULL);

	if (!FValidID(&m_sUserID))
		{
		CString strTime;
		strTime = AfxGetApp()->GetProfileString("USER", GetTimerEntryName(), NULL);
		m_tLimitTime = atol(strTime.GetBuffer(0));
		m_sUserID.Empty();
		}
}

/*************************************************************************
-- Routine: BOOL CCrejit::SetProfile()
-- Description and Usage:
		ユーザーIDを取得します。
-- Arguments:
		char* lpUserID : ユーザーIDの入った文字列のポインタ。
-- Returns:
		成功した時は TRUE を、失敗した時は FALSE を返します。
-- Notes:
-- History:
		'94.12	- created		(M.Design)
*************************************************************************/

BOOL CCrejit::SetProfile()
{
	AfxGetApp()->WriteProfileString("USER", CRJ_ID_STR, m_sUserID.GetBuffer(CRJ_NEW_USERID_LENGTH));
	return TRUE;
}

void CCrejit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrejit)
	DDX_Control(pDX, IDC_TIMER_TIME, m_csTime);
	DDX_Control(pDX, IDC_TIMER_USERID, m_editUserID);
	DDX_Text(pDX, IDC_TIMER_USERID, m_sUserID);
	DDX_Control(pDX, IDC_CRJTMPID, m_csTmpID);
	DDX_Text(pDX, IDC_STC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}


BOOL CCrejit::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//CenterWindow();


	GetProfile();
	if (FValidID(&m_sUserID))
		{
		SetDlgItemText(IDC_MITOROKU, ""); //未登録を削除
		m_uiTimer = 0;
		}
	else
		{
		// TODO: この位置にその他の初期化用コードを追加してください。
		CTime tm = CTime::GetCurrentTime();		// 現在時刻の取得
    	
		if(m_tLimitTime == 0 || m_tLimitTime > (tm.GetTime() + CRJ_TIME_LIMIT) )
			{
			m_tLimitTime = tm.GetTime() + CRJ_TIME_LIMIT;	// 7*24*60*60= 604800
			WriteLimitTime(m_tLimitTime);
			}
		

		srand((unsigned)time(NULL));
	

		for (m_uiTimer = 16; SetTimer(m_uiTimer, 1000, NULL) == 0 && --m_uiTimer>0; );
		if (m_uiTimer>0)
			OnTimer(0);
		}

	m_editUserID.SetFocus();

	
	m_strVersion.LoadString(IDS_VERSION);
	UpdateData(FALSE);

	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります。
}

void CCrejit::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください。
	UpdateData(TRUE);
	if (FValidID(&m_sUserID))
		{
		SetProfile();
		AfxMessageBox(IDS_CRJ_OK);
		if (m_uiTimer>0)
			KillTimer(m_uiTimer);
		EndDialog(IDOK);
		}
	else if (m_sUserID.IsEmpty())
		{
		CString cstr;
		m_csTime.GetWindowText(cstr);
		if (m_uiTimer>0)
			KillTimer(m_uiTimer);
#ifdef TIME_OVER_IS_BYE		//時間が０なら動作しない
		if (cstr[0]=='0' || cstr[0]=='-')
			EndDialog(IDCANCEL);
		else
#endif 
			EndDialog(IDOK);
	} else if (m_sUserID=="APP_C") {
		m_sUserID = "";
		AfxGetApp()->WriteProfileString("USER", CRJ_ID_STR, "");
		EndDialog(IDOK);
	} else
		{
		BeepMessage(IDS_CRJ_NG);
		m_editUserID.SetSel(0,-1);
		m_editUserID.SetFocus();
		}
}

BOOL CCrejit::FValidID(CString *pcstr)
{
	//tY6\ryeark\53bY3\rmonth148
	//tY699k8iir/3bY311148
	//tY698kTUW403bY310148
	//tY699kGEaXf3bY39148
	//---99-12345----12---
	//---99-12345----1---
	//01234567890123456789
	if (pcstr->GetLength() >= 19)	{
		long ryear = atol(pcstr->Mid(3, 2));
		long rmonth = atol((pcstr->GetLength() == 19) ? pcstr->Mid(15, 1) : pcstr->Mid(15, 2));
		long nNumber = atol(pcstr->Mid(6, 5));
		//az7**m*****6r45*78w
		//az794m346796r451078w
		//---99-12345----12---
		//---99-12345----1---
		//01234567890123456789
		if (pcstr->Left(3) == "az7"
			&& pcstr->Mid(5, 1) == "m"
			&& pcstr->Mid(11, 4) == "6r45"
			&& pcstr->Right(3) == "78w"
			&& (ryear >= 94
			|| ryear <= 10)
			&& (rmonth >= 1
			&& rmonth <= 12)
			)
			return TRUE;
	}
	return FALSE;

}


void CCrejit::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラのコードを追加するか、またはデフォルト処理の呼び出しを行う。
	char vz[256];	
	long nTimeLimit;
	CTime tm = CTime::GetCurrentTime();
	nTimeLimit = m_tLimitTime - tm.GetTime();
	if( nTimeLimit < 0 )
		{
		if (m_uiTimer>0)
			{
			KillTimer(m_uiTimer);
			m_uiTimer = 0;
			}
		AfxMessageBox(IDS_CRJ_OVER);
		m_tLimitTime = nTimeLimit;
		m_csTime.SetWindowText("0");
		//EndDialog(IDCANCEL);
		return;
	} 
	CString cstr;
	_ltoa( nTimeLimit, vz, 10);
	TRACE("Time Limit is %s\n",vz);
	m_csTime.GetWindowText(cstr);
	if (cstr!=vz)
		m_csTime.SetWindowText(vz);
}


void CCrejit::OnCancel()
{
	// TODO: この位置にその他の後処理用のコードを追加してください。
	if (m_uiTimer>0)
		KillTimer(m_uiTimer);
	EndDialog(IDCANCEL);
}

#endif //#ifdef SHAR
