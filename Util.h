#ifndef __UTIL_H__
#define __UTIL_H__
//#include <iostream>
//#include <string>
//#include <deque>	/* Compile options needed:-GX */
#include <sys/types.h>
#include <sys/stat.h>

//template <typename T_char>
//T_char ToUpper (T_char cX) { return toupper(cX); }  // テンプレート関数として ToUpper を定義する。
//template <typename T_char>
//T_char ToLower (T_char cX) { return toulower(cX); }  // テンプレート関数として ToLower を定義する。
struct ToUpper {
    char operator()(char c) { return toupper(c); }
};
struct ToLower {
    char operator()(char c) { return tolower(c); }
};

void RoundRect(LPRECT r1);
void MulDivRect(LPRECT r1, const LPRECT r2, const int num, const int div);
#ifdef FREE
void StopFreeMsg();
#endif
#define IsValidDataSz(p)	(p!=null && *p!='\0')
#define IsValidDataPStr(p)	(p!=null && p->size()>0 && (*p)[0]!='\0')
#define IsValidPStr(p)	(p!=null)
#define Empty2Null(p)	((LPCSTR)p==NULL ? NULL:(strlen((LPCSTR)p)==0 ? NULL:(LPCSTR)p))
#define Null2Empty(p)	((LPCSTR)p==NULL ? "":p)
inline bool BOOL2bool(BOOL p) {
	return p ? true:false;
}
inline BOOL bool2BOOL(bool p) {
	return p ? TRUE:FALSE;
}
inline void SetBool2(bool &b1, bool b2) {
	if (b2)
		b1 = b2;
}
inline void SetBool2(bool &b1, BOOL b2) {
	if (b2)
		b1 = BOOL2bool(b2);
}
#define CString2string(p)	(string(p.GetBuffer(0)))
#define string2CString(p)	(CString(p.c_str()))

typedef bool (*IsChar_Proc)(BYTE ch);
HPALETTE WINAPI GetSystemPalette(void);
void GetTextBlockFirst(BYTE* &pch, BYTE* pch_last, IsChar_Proc pProc);
void GetTextBlockLast(BYTE* pch, BYTE* &pch_last, IsChar_Proc pProc);
void GetMailAddressBlock(BYTE* &pch_first, BYTE *pch_point, BYTE* &pch_last);
void GetUrlAddressLast(BYTE* pch_first, BYTE* &pch_last);
bool IsMailAddressChar(BYTE ch);
bool IsUrlAddressChar(BYTE ch);

bool IsDBCSTrailByte(const char *base, const char *p);
OBJOFF findStr(const string &str, const string &str_find, const OBJOFF off_first=0);
OBJOFF findSz(const string &str, const char *sz_find, const OBJOFF off_first=0);
/*
COLORREF Sz2Colorref(const char *sz);
COLORREF Str2Colorref(const string &str);
*/
bool Trans_LineHead_block(string &str, const char *sz, const OBJSIZE len, bool bAppend, const char *szMark, const char *szMark2=null, const char *szMark3=null);
string Trans_LineHead_line(const char *sz, const OBJSIZE len, bool bAppend, const char *szMark, const char *szMark2=null, const char *szMark3=null);
string Trans_LineHead_string(const string &_str, bool bAppend, const char *szMark, const char *szMark2=null, const char *szMark3=null);
bool RemoveHeadStr(string &str, const char *szMark);

//bool FAppendCpyStr2GData(HGLOBAL &hData, OBJSIZE &lenData, const string &str);
bool FAppendCpySz2GData(HGLOBAL &hData, OBJSIZE &lenData, const BYTE *sz, OBJSIZE len);
HGLOBAL AppendSz2GData(HGLOBAL hData, OBJSIZE &lenData, const BYTE *sz, OBJSIZE len);

typedef int (*Normal_Proc)();
typedef int (*ScanHitChar)(const BYTE *sz, void *pv);
int ScanHitLineHeader(const BYTE *sz, void *pv);
bool IncLineHeaderNumStr(string &str);
bool TrimLeftChar(string &str, const string *ptrRemove);
bool TrimLeftLineHeader(string &str);
bool Trim(string &str, bool bDbcsWhite=true);
bool TrimLeft(string &str, bool bDbcsWhite=true);
bool TrimRight(string &str, bool bDbcsWhite=true);
bool Trim(string &str, bool bLeft, bool bRightWhite, bool bWhiteDo, bool bDbcsWhite, ScanHitChar pProc=null, void *pv=null);

extern bool vbEscHit;
/*
class CPeekMsg
{
public:
	CPeekMsg();
	~CPeekMsg();
public:
	bool CheckEsc() {
		return m_bEsc; }
	bool CheckMsg() {
		return m_bMsg; }
	bool CheckClose() {
		return m_bClose; }

private:
	bool m_bEsc;
	bool m_bMsg;
	bool m_bClose;
	MSG	m_msg;
	HWND	m_hWnd;
};
thePeekMsg.CheckEscCheck())
*/
inline bool MyPeekMessage_EscCheck(HWND hWnd) {
	MSG msg;
	vbEscHit = false;
	if (::PeekMessage (&msg, hWnd, 0, 0, PM_REMOVE)) {
		if (msg.message==WM_KEYDOWN) {
			TranslateMessage(&msg);
			if (msg.wParam==VK_ESCAPE) {
				vbEscHit = true;
				AfxGetApp()->DoWaitCursor(-1);
				CString msg;
				msg.Format("処理を中断しました。");
				AfxMessageBox(msg);
				return true;
			}
		}
		AfxGetApp()->DoWaitCursor(1); 
	}
	return false;
}

inline bool MyPeekMessage(HWND hWnd) {
	MSG msg;
	if (::PeekMessage (&msg, hWnd, 0, 0, PM_REMOVE)) {
		if (msg.message==WM_MOUSEMOVE && ::GetCapture()!=hWnd) {
			; //eate!
		}else{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
			if (msg.message==WM_CLOSE)
				return true;
		}
	}
	return false;
}


inline BYTE *strchr2(BYTE *sz, BYTE ch1, BYTE ch2) {
	BYTE ch;
	while(ch=*sz++) {
		if (ch==ch1 || ch==ch2)
			return sz-1;
	}
	return null;
}

string itostring(int i, int base=10);
//string 64tostring(__int64 dl, int base=10);
string ultostring(unsigned long l, int base=10);



//unsigned long fileLegnth(int handle);

#ifdef _MFC_VER
inline void TraceException(exception *e)
{
	#ifndef NDEBUG
	static char szErrMsg[256];	//static is because for low memory error.
	if (e->GetErrorMessage(szErrMsg, sizeof(szErrMsg))) {
		cout << szErrMsg << endl;
	}
	#endif
}
#else
inline void TraceException(const exception &e)
{
	#ifndef NDEBUG
	cout << e.what();
	#endif
}
#endif
string Trans2ValidFileName(const char *sz);
string GetPartStringOfPath(string &strSrc);
string GetPartString(string &strSrc, char ch);
void PartString2IntArray(string &strSrc, int *pInt, const int max, char ch=',');
string IntArray2PartString(int *pInt, const int max, char ch=',');
int	FindStringArray(const CStringArray &aryStr, int first, const char *sz);

bool IsExistFile(const char *szFileName);
bool TrashFile(const char *szFileName, HWND hWnd);
bool FCreateDirectory(const char * lpszPath);
string GetCurrentPath(void);
string GetFileDir(const string &strSrc);
string GetFileDirDir(const string &strSrc);
inline string GetFileDirDir(const char *szSrc) {
	return GetFileDirDir(string(Null2Empty(szSrc))); }
string GetFileExtent(const string &strSrc);
inline string GetFileExtent(const char *szSrc) {
	return GetFileExtent(string(Null2Empty(szSrc))); }
string GetFileName(const string &strSrc);
inline string GetFileName(const char *szSrc) {
	return GetFileName(string(Null2Empty(szSrc))); }
string GetModuleDirDir(HINSTANCE hInst);
bool GetFileStat(const char *szFileName, struct _stat &filestat);
void RemoveLastBackSlash(string *pcstr);

class ParseOption
{
public:
	ParseOption() {
		bOption = false; };
	~ParseOption() {};
public:
	void Set(const char *sz);
	string	st;
	bool	bOption;
	string	stOption;
	string	stCol;
};

typedef	deque<ParseOption> DEQUE_OPTIONS;	
class ParseOptions
{
public:
	ParseOptions() {};
	~ParseOptions() {};
public:
	ParseOption &operator[](int i) {
		return options[i];
	};
	void Set(const char *szOptions);
	DEQUE_OPTIONS	options;	
};


#endif //__UTIL_H__
