// HeaderCtrlEx.h : ヘッダー ファイル
//
#ifndef __HEADERCTRLEX_H__
#define __HEADERCTRLEX_H__
/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx ウィンドウ
#define DCX_DOWN_BMP	(16)
#define DCX_MUGEN		(5000)

class CHeaderCtrlEx : public CHeaderCtrl
{
// コンストラクション
public:
	CHeaderCtrlEx();

// アトリビュート
public:
protected:
	CFont m_font;
	CFont *m_pfontOld;
	int	m_nItem;	//sel!
	CBitmap	m_bmpDown;
	
// オペレーション
public:
	void AddItem(const char *szFileName);
	void RemoveItem(int nIndex=-1); //-1:All remove
// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHeaderCtrlEx();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__HEADERCTRLEX_H__