// Cell.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Cell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCell

//IMPLEMENT_DYNCREATE(CCell, CCmdTarget)
CCell::CCell()
{
	m_bEdit = FALSE;
}

CCell::CCell(BOOL bEdit)
{
	m_bEdit = bEdit;
}

CCell::~CCell()
{
}
const CCell& CCell::operator=(const CCell &src)
{
	m_bEdit = src.m_bEdit;
	return *this;
}
/*
BEGIN_MESSAGE_MAP(CCell, CCmdTarget)
	//{{AFX_MSG_MAP(CCell)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// CCellArray
CCellArray::CCellArray():CCellArrayBase()
{
	m_bCheck=FALSE;
	m_color = RGB(255,255,255);
}
const CCellArray& CCellArray::operator=(const CCellArray &src)
{
	RemoveAll();
	for(int i=0; i<src.GetSize(); i++) {
		CCell obj = src.GetAt(i);
		Add(obj);
	}
	m_bCheck = src.m_bCheck;
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// CCellTable

//IMPLEMENT_SERIAL(CCellTable, CObArray, 0)
CCellTable::~CCellTable()
{
	RemoveAll();
}

const CCellTable& CCellTable::operator=(const CCellTable &src)
{
	RemoveAll();
	for(int i=0; i<src.GetSize(); i++) {
		CCellArray *pobj = new CCellArray;
		*pobj = *(CCellArray *)src.CObArray::GetAt(i);
		Add(pobj);
	}
	return *this;
}

const int CCellTable::GetCols()
{
	if (GetRows()==0)
		return 0;
	return ((CObArray *)(CObArray::GetAt(0)))->GetSize();
}

BOOL CCellTable::SetSize(int row, int col)
{
	if (row != GetRows()) {
		SetSize(row);
	}
	if (col != GetCols()) {
		for (int i=0; i<GetRows(); i++) {
			((CCellArray *)(CObArray::GetAt(i)))->SetSize(col);
		}
	}
	return TRUE;
}

BOOL CCellTable::SetSize(int newSize)
{
	int oldSize = GetRows();
	int cols = GetCols();
	if (newSize > oldSize) {
		CObArray::SetSize(newSize);
		while (oldSize<newSize)	{
			CCellArray *pobj = new CCellArray;
			pobj->SetSize(cols);
			CObArray::SetAt(oldSize++, pobj);
		}
	}
	else if (newSize < oldSize)	{
		while (oldSize>newSize)
			delete CObArray::GetAt(--oldSize);
		CObArray::SetSize(newSize);
	}
	return TRUE;
}

CCell CCellTable::GetAt(int row, int col)
{
	CCell var;
	if (row>=GetRows() || col>=GetCols())
		return var;
	return ((CCellArray *)(CObArray::GetAt(row)))->GetAt(col);
}


BOOL CCellTable::SetAt(int row, int col, CCell &var)
{
	if (row>=GetRows() || col>=GetCols())
		return FALSE;
	((CCellArray *)(CObArray::GetAt(row)))->SetAt(col, var);
	return TRUE;
}
BOOL CCellTable::IsCheckAt(int row)
{
	if (row>=GetRows()) {
		ASSERT(FALSE);
		return FALSE;
	}
	return ((CCellArray *)(CObArray::GetAt(row)))->m_bCheck;
}
void CCellTable::SetCheckAt(int row, BOOL bCheck)
{
	if (row>=GetRows()) {
		ASSERT(FALSE);
		return;
	}
	((CCellArray *)(CObArray::GetAt(row)))->m_bCheck = bCheck;
}
COLORREF CCellTable::GetColorAt(int row)
{
	if (row>=GetRows()) {
		ASSERT(FALSE);
		return FALSE;
	}
	return ((CCellArray *)(CObArray::GetAt(row)))->m_color;
}
BOOL CCellTable::SetAtExpand(int row, int col, CCell &var)
{
	int oldRows = GetRows();
	int oldCols = GetCols();
	
	if (row>=oldRows || col>=oldCols)
		if (!SetSize(row>=oldRows ? row+1:oldRows,
			col>=oldCols ? col+1:oldCols))
			return FALSE;
	((CCellArray *)(CObArray::GetAt(row)))->SetAt(col, var);
	return TRUE;
}

void CCellTable::RemoveAll()
{
	SetSize(0);
	CObArray::RemoveAll();
}
/*
void CCellTable::Serialize(CArchive& ar)
{

	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		{
			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
			ar << pNode->data;
		}
	}
	else
	{
		DWORD nNewCount = ar.ReadCount();
		CObject* newData;
		while (nNewCount--)
		{
			ar >> newData;
			AddTail(newData);
		}
	}
}
*/

