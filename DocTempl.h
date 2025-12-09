/////////////////////////////////////////////////////////////////////////////
// class CDocTemplate creates documents

#if !defined(__DOCTEMPL_H__)
#define __DOCTEMPL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ObjLay.h"
// SDI support (1 document only)
class CTheDocTemplate : public CSingleDocTemplate
{
	DECLARE_DYNAMIC(CTheDocTemplate)

// Constructors
public:
	CTheDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

// Implementation
public:
	virtual ~CTheDocTemplate();
//	virtual void AddDocument(CDocument* pDoc);
//	virtual void RemoveDocument(CDocument* pDoc);
//	virtual POSITION GetFirstDocPosition() const;
//	virtual CDocument* GetNextDoc(POSITION& rPos) const;
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible = TRUE);
	virtual void SetDefaultTitle(CDocument* pDocument, CMainFrame* pFrame);
//	virtual void CloseAllDocuments(BOOL);
	CDocument* SelectDocumentFile(CObjLayTbl *pLay);
#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

protected:  // standard implementation
//	CDocument* m_pOnlyDoc;
//	CPtrList m_docList;          // open documents of this type
	UINT m_nUntitledCount;   // start at 0, for "Document1" title
private:
	void SetKnjIndicators(CDocument *pDocument);
};


#endif //__DOCTEMPL_H__