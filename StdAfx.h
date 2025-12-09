// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__D38F0927_108B_11D2_AE68_0060976DB3A9__INCLUDED_)
#define AFX_STDAFX_H__D38F0927_108B_11D2_AE68_0060976DB3A9__INCLUDED_


#ifndef _UNICODE
#define VC_EXTRALEAN        // use stripped down Win32 headers
#endif

#define CONVERTERS


//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

//#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC の OLE オートメーション クラス
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "swenv.h"
#include "globdefs.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__D38F0927_108B_11D2_AE68_0060976DB3A9__INCLUDED_)
