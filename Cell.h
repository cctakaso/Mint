#if !defined(AFX_CELL_H__F13D6D42_9434_11D1_AA07_444553540000__INCLUDED_)
#define AFX_CELL_H__F13D6D42_9434_11D1_AA07_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Cell.h : ヘッダー ファイル
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCell
class CCell// : public CCmdTarget
{
//	DECLARE_DYNCREATE(CCell)
public:
	CCell();
	CCell(int bEdit);
	virtual ~CCell();

public:
	BOOL	m_bEdit;
	const CCell& operator=(const CCell &src);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCell)
	//}}AFX_VIRTUAL

// インプリメンテーション

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCell)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCellArray
typedef CArray<CCell, CCell&> CCellArrayBase;
class CCellArray : public CCellArrayBase
{
	friend class CCellTable;
public:
	CCellArray();
	CCellArray(const CCellArray &src) {
		*this = src; }
	const CCellArray& operator=(const CCellArray &src);
protected:
	BOOL m_bCheck;
	COLORREF m_color;
};
/////////////////////////////////////////////////////////////////////////////
// CCellTable
class CCellTable : public CObArray
{
//	DECLARE_SERIAL(CCellTable);
public:
	CCellTable():CObArray() {};
	virtual ~CCellTable();
public:
	void RemoveAll();
	BOOL SetSize(int newSize);
	const int GetRows() {
		return CObArray::GetSize(); }
	const int GetCols();
	BOOL SetSize(int row, int col);
	CCell GetAt(int row, int col);
	BOOL SetAt(int row, int col, CCell &var);

	BOOL IsCheckAt(int row);
	void SetCheckAt(int row, BOOL bCheck);

	COLORREF GetColorAt(int row);
	void SetColorAt(int row, COLORREF color);
	
	BOOL SetAtExpand(int row, int col, CCell &var);
	virtual const CCellTable& operator=(const CCellTable &src);
//	void Serialize(CArchive& ar) {
//		CObArray::Serialize(ar); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CELL_H__F13D6D42_9434_11D1_AA07_444553540000__INCLUDED_)
