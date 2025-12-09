#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <string>
#include <io.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <exception>
#include <deque>	/* Compile options needed:-GX */
#include <algorithm>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include <map>
#include <list>
#include <vector>
using namespace std ;

////////////////////// VERSION CONTROL CODES ------------------------------------->
// コーディングは、ISO/ANSI規約になるべく沿ったものとして
// MFCバージョンはデファインやタイプデフで対応する。

#ifdef _MSC_VER
	#define  fstream	::fstream	//for error C2872: 'fstream' : あいまいなｼﾝﾎﾞﾙです。
	#define  endl		::endl
#endif

// ★==================USER-TODO======================★
//   WINDOWS VERSION　であれば、下記をコメントにして下さい。
//   UnKanj.h　にあればも指定する必要があります。
//#undef WINVER
//   Microsoft funcdation class を使いたければ、下記をコメントにして下さい。
//   先の、#define  fstream	::fstream 以後に指定する必要があります。
#undef _MFC_VER
// ★==================USER-TODO======================★

#ifdef _MFC_VER
	#include "afxwin.h"		// Target version control
	#define fstream CFile
	#define string CString	//string class.
	#define exception	CException
#endif


#ifdef WINVER
	#pragma message ("<< WINDOWS VERSION >>")
//	#include "stdafx.h"
	#define cout afxDump
	extern void _assert(char *exp, char *szFile, int line);
	#undef assert
#ifdef _VERIFY
	#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#else
	#define assert ASSERT
#endif
//	#define endl  "\n"
	#define CH_DIR	((BYTE)'\\')
	#define CH_DRV	((BYTE)':')
	#define SZ_NET_DIR	"\\\\"
#else
	#pragma message ("<< COMMAND VERSION >>")
	#define CH_DIR	((BYTE)'/')
	#define CH_DRV	((BYTE)':')
	#define SZ_NET_DIR	"\\\\"
#endif



#ifdef _MFC_VER
	/*////////////MFC VERSION/////////*/
	#pragma message ("<< MFC VERSION >>")
	#ifndef NDEBUG
	#include <mbstring.h>
	#endif
	#ifndef NDEBUG
	#define new DEBUG_NEW
	//#undef THIS_FILE
	//static char THIS_FILE[] = __FILE__;
	#endif
	//#define mkdir CreateDirectory 	//WindowsでのSCLバージョンは、CreateDirectory()を使わないとWINVERsはだめになる！
	//#define getcwd	GetCurrentDirectory
	#define length GetLength	//string length.
	#define empty IsEmpty	//is string empty or not.
	#define c_str() GetBuffer(0)
	#define substr	Mid
	#define find	Find
	//#define resize(0)	Empty() :定義できないので=""を使う。
	//GetAt()->[i]->MFCでは、=='a' がVC5.0コンパイラーエラーとなるので *(c_str()+i)=='a'にする
	#ifndef NDEBUG
		#define realloc(p,nSize)	_realloc_dbg(p, nSize,_NORMAL_BLOCK, NULL, 0)
		#define malloc(nSize)		_malloc_dbg(nSize,_NORMAL_BLOCK, NULL, 0)
		#define free(p)				_free_dbg(p, _NORMAL_BLOCK)
		#define msize(p)			_msize_dbg(p, _NORMAL_BLOCK)
	#endif
	/////////////MFC VERSION//////////
#else
	/////////////STANDERD C++ LIBRARY VERSION//////////
	#pragma message ("<< STANDERD C++ LIBRARY VERSION >>")
//	#ifdef NDEBUG
//		#undef  assert
//		#define assert(exp)     ((void)0)
//	#endif //!
	#ifdef WINVER
		#define SZ_DIR	"\\"
		#define SZ_DRV	":"
	#else
		#define SZ_DIR	"/"
		#define SZ_DRV	":"
	#endif
	string GetFileName(const string &strSrc);
	/////////////STANDERD C++ LIBRARY VERSION//////////
#endif

#ifdef _MSC_VER
	#include <direct.h> //for _mkdir 引数が違うので
	#define mkdir _mkdir
	#define getcwd	_getcwd
	#ifndef msize
	#define msize	_msize
	#endif
#endif
//<------------------- VERSION CONTROL CODES //////////////////////
#endif //__SWITCH_H__