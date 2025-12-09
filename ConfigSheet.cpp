// ConfigSheet.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "ConfigSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigSheet

IMPLEMENT_DYNAMIC(CConfigSheet, CPropertySheet)

CConfigSheet::CConfigSheet(CWnd* pWndParent)
	: CPropertySheet(IDS_CONFIG, pWndParent)
{
	AddPages();
}

CConfigSheet::CConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CConfigSheet::CConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
}
void CConfigSheet::AddPages()
{
	AddPage(&m_dispPage);
	AddPage(&m_formatPage);
	AddPage(&m_formatCharPage);
	AddPage(&m_filePage);
#ifdef SHAR
	AddPage(&m_keywordPage);
#endif//#ifdef SHAR
}
CConfigSheet::~CConfigSheet()
{
}


BEGIN_MESSAGE_MAP(CConfigSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigSheet)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigSheet メッセージ ハンドラ
bool CConfigSheet::UpdateEnv(bool bSetToEnv, CViewEnv *pEnv)
{
	assert(pEnv);
	bool bUpdate = false;
	bUpdate = (m_dispPage.UpdateEnv(bSetToEnv, pEnv) || bUpdate);
	bUpdate = (m_formatPage.UpdateEnv(bSetToEnv, pEnv) || bUpdate);
	bUpdate = (m_formatCharPage.UpdateEnv(bSetToEnv, pEnv) || bUpdate);
	bUpdate = (m_filePage.UpdateEnv(bSetToEnv, pEnv) || bUpdate);
#ifdef SHAR
	bUpdate = (m_keywordPage.UpdateEnv(bSetToEnv, pEnv) || bUpdate);
#endif
	if (bUpdate) {
		pEnv->SetDirty();
		if (m_filePage.IsNowUpdate()) {
			assert(pEnv->m_bRockUpdate);
			//pEnv->m_bRockUpdate = false;
			if (!pEnv->ReadWriteProfile(CProfile::__write, false, true)) {
				CString cstr;
				cstr.Format("ドライブ領域が足りない、またはインストール先フォルダに書き込み権限が" \
					"ない等の理由により、以下の設定ファイルを更新できませんでした。\n%s", pEnv->m_profile.GetSzProfile());
				AfxMessageBox(cstr);
			}
			//pEnv->m_bRockUpdate = true;
		}
	}
	return bUpdate;
}
