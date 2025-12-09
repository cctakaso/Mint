// HeaderCtrlEx.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "The.h"
#include "HeaderCtrlEx.h"
#include "ViewEnv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
	m_nItem = -1;
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx メッセージ ハンドラ

int CHeaderCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHeaderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//ﾌｫﾝﾄの設定
	LOGFONT lf;
	InitialHorzFont(&lf);
	lf.lfHeight = 13;
	strcpy(lf.lfFaceName, "ＭＳ Ｐゴシック"); //Terminal
	lf.lfFaceName[sizeof lf.lfFaceName-1] = 0;
	
	//lf.lfItalic = TRUE;
	VERIFY(m_font.CreateFontIndirect(&lf));

	m_pfontOld = GetFont();
	SetFont(&m_font);

	//ビットマップの取得
	m_bmpDown.LoadBitmap(IDB_DOWN);

	HD_ITEM hd;
	memset(&hd, 0, sizeof(hd));
	hd.mask = HDI_BITMAP | HDI_FORMAT | HDI_WIDTH;
	hd.fmt = HDF_CENTER | HDF_BITMAP;
	hd.hbm = m_bmpDown;
	hd.cxy = DCX_DOWN_BMP;
	CHeaderCtrl::InsertItem(0, &hd);
	CRect rc(0,0, DCX_DOWN_BMP, DCX_DOWN_BMP);
	MoveWindow(&rc);
	return 0;
}

//file index origin = 1
void CHeaderCtrlEx::RemoveItem(int nItem)
{
	int n = GetItemCount();
	if (nItem<0) {
		while(n--)
			DeleteItem(n);
	}else if (nItem<n)
		DeleteItem(n);
}
#define DCX_MARGIN		(6)
void CHeaderCtrlEx::AddItem(const char *szFileName)
{
	int pos = GetItemCount();
	if (pos<0)
		return;
	HD_ITEM hd;
	memset(&hd, 0, sizeof(hd));
	hd.mask = HDI_TEXT | HDI_FORMAT /*| HDI_WIDTH*/;
	hd.fmt = HDF_CENTER | HDF_STRING;
	if (IsValidDataSz(szFileName)) {
		CDC *pDC = GetDC();
		hd.cxy = pDC->GetTextExtent("あ", 2).cx * strlen(szFileName)/2 + DCX_MARGIN;
		ReleaseDC(pDC);
	}else
		hd.cxy = DCX_MUGEN;
	hd.pszText = (char*)szFileName;
	hd.cchTextMax = strlen(szFileName);
	CHeaderCtrl::InsertItem(pos,&hd);
	
	CRect rc;
	GetClientRect(&rc);
	rc.right += hd.cxy;
	MoveWindow(&rc);
/*	int n = GetItemCount();
	HD_ITEM hd;
	memset(&hd, 0, sizeof(hd));
	hd.mask = HDI_WIDTH;
	for (int pos=0; pos<n; pos++)
		{
		CHeaderCtrl::GetItem(pos,&hd);
		m_cxs[pos] = hd.cxy;
		ASSERT(pos<HEADERITEM_NUM);
		}
*/
}

BOOL CHeaderCtrlEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	HD_NOTIFY FAR * phdn = (HD_NOTIFY FAR *)lParam; 
	if (message==WM_NOTIFY && (int)wParam==2)
		{
		switch(phdn->hdr.code) {
			case HDN_ITEMCHANGED://phdn->hdr.code==HDN_ENDTRACK)
				//
				break;
			case HDN_ITEMCLICK:
				if (phdn->iButton != 0)	//others of left button case is break.
					break;
				//if index=0 then show popup menu.
			/*	if (phdn->iItem==0)
					{
						CMenu popMenu;
						popMenu.LoadMenu(IDR_POPUP);
						CRect rc;
						GetWindowRect(&rc);
						popMenu.GetSubMenu(0)->TrackPopupMenu(0,rc.left,rc.bottom,AfxGetMainWnd());
						break;
					}
			*/
				//others indexs are select files!
				break;
			}
		}

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	return CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

