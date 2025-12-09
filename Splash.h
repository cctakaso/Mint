// Splash.h : ヘッダー ファイル
//
#ifndef __SPLASH_H__
#define __SPLASH_H__

/////////////////////////////////////////////////////////////////////////////
// CSplash ダイアログ

class CSplash : public CDialog
{
// コンストラクション
public:
	CSplash(CWnd* pParent = NULL);   // 標準のコンストラクタ
	
	BOOL Create(CWnd* pParent);

// ダイアログ データ
	//{{AFX_DATA(CSplash)
	enum { IDD = IDD_SPLASH };
	CStatic	m_bmpSplash;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSplash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSplash)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifndef __SPLASH_H__
