// CDxdoc2txt.h  : Microsoft Visual C++ で作成された ActiveX コントロール ラッパー クラスの宣言です。

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDxdoc2txt

class CDxdoc2txt : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDxdoc2txt)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x4FD6BD6E, 0x471A, 0x48A6, { 0x8C, 0x52, 0x5E, 0x83, 0x91, 0xEC, 0xBA, 0x9F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:

// 操作
public:

	void Convert(LPCTSTR inPath, CString *result)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)
			result, parms, inPath);
	}
/*  CString のコピーをなくす
	CString Convert(LPCTSTR inPath)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, inPath);
		return result;
	}
*/
	void Load(LPCTSTR inPath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, inPath);
	}


};
