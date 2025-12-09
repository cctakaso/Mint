#if !defined(AFX_CONFIGSHEET_H__94EE5437_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
#define AFX_CONFIGSHEET_H__94EE5437_2460_11D2_AEAE_0060976DB3A9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConfigSheet.h : ヘッダー ファイル
//
#include "disppage.h"
#include "keywordpage.h"
#include "formatpage.h"
#include "formatcharpage.h"
#include "filepage.h"
#include "ViewEnv.h"
/////////////////////////////////////////////////////////////////////////////
// CConfigSheet

class CConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigSheet)

// コンストラクション
public:
	CConfigSheet(CWnd* pWndParent=null);
	CConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// アトリビュート
public:
// Attributes
	CDispPage m_dispPage;
#ifdef SHAR
	CKeywordPage m_keywordPage;
#endif//#ifdef SHAR
	CFormatPage m_formatPage;
	CFormatCharPage m_formatCharPage;
	CFilePage m_filePage;

// オペレーション
public:
	bool UpdateEnv(bool bSetToEnv, CViewEnv *pEnv);
	int IsChangeFormat() {
		return m_formatPage.IsChangeFormat() || m_formatCharPage.IsChangeFormat(); }

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfigSheet)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CConfigSheet();
private:
	void AddPages();
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CConfigSheet)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFIGSHEET_H__94EE5437_2460_11D2_AEAE_0060976DB3A9__INCLUDED_)
