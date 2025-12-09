// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DocTempl.h"
#include "MainFrm.h"
#include "FileSelectBar.h"
#include "TheDoc.h"
#include "TheView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate construction/destruction
IMPLEMENT_DYNAMIC(CTheDocTemplate, CSingleDocTemplate)

CTheDocTemplate::CTheDocTemplate(UINT nIDResource,
	CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
	CRuntimeClass* pViewClass)
		: CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	m_nUntitledCount = 0;   // start at 1

}


CTheDocTemplate::~CTheDocTemplate()
{
#ifdef _DEBUG
	if (m_pOnlyDoc != NULL)
		TRACE0("Warning: destroying CSingleDocTemplate with live document.\n");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CTheDocTemplate attributes
/////////////////////////////////////////////////////////////////////////////
// CTheDocTemplate commands


CDocument* CTheDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	return OpenDocumentFile(lpszPathName, TRUE,  bMakeVisible);
}

CDocument* CTheDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;
	BOOL bCreated = FALSE;      // => doc and frame created
	BOOL bWasModified = FALSE;

	if (AfxGetMainWnd() && IsWindow(AfxGetMainWnd()->GetSafeHwnd())) {
		AfxGetMainWnd()->SetForegroundWindow();//SetFocus();全然効かない
//		AfxGetMainWnd()->InvalidateRect(NULL);
//		AfxGetMainWnd()->UpdateWindow();
	}

	if (m_pOnlyDoc != NULL)
	{
		// already have a document - reinit it
		pDocument = m_pOnlyDoc;
//		if (!pDocument->SaveModified())
//			return NULL;        // leave the original one

		pFrame = (CFrameWnd*)AfxGetMainWnd();
		ASSERT(pFrame != NULL);
		ASSERT_KINDOF(CFrameWnd, pFrame);
		ASSERT_VALID(pFrame);

		if (lpszPathName != NULL) {
			if (!pDocument->IsModified() && 
				((CMainFrame*)pFrame)->m_wndFileSelectBar.GetWindowCount()==1) {
				//最後の１つのドキュメントが新規で作成されたもので、ダーティでない場合
				//それを、破棄して指定ファイルをオープンする
				if (((CTheDoc*)pDocument)->m_pMainObjLay->GetPathName().size()==0) {
					if (!IsExistFile(lpszPathName)) {
						CString str;
						//str.LoadString(AFX_IDP_FILE_NOT_FOUND, lpszPathName);
						//str += "\n";
						//str += lpszPathName;
						AfxFormatString1(str, AFX_IDP_FILE_NOT_FOUND, lpszPathName);
						AfxMessageBox(str);
						return NULL;
					}
					theObjTbl.CloseLayouts(CLOSE_THIS, ((CTheDoc*)pDocument)->m_pMainObjLay);
					((CMainFrame*)pFrame)->m_wndFileSelectBar.CloseTabs(CLOSE_THIS, 0);
					((CTheView*)pFrame->GetActiveView())->m_pViewEd->Detach(true);
				}
			}
			if (((CMainFrame*)pFrame)->m_wndFileSelectBar.SelectDoc(lpszPathName)){
				//既にオープンしているファイルはそれをアクティブにする	
				return NULL;
			}
		}
	}
	else
	{
		// create a new document
		pDocument = CreateNewDocument();
		ASSERT(pFrame == NULL);     // will be created below
		bCreated = TRUE;
	}

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pDocument == m_pOnlyDoc);

	if (pFrame == NULL)
	{
		ASSERT(bCreated);

		// create frame - set as main document frame
		BOOL bAutoDelete = pDocument->m_bAutoDelete;
		pDocument->m_bAutoDelete = FALSE;
					// don't destroy if something goes wrong
		pFrame = CreateNewFrame(pDocument, NULL);
		pDocument->m_bAutoDelete = bAutoDelete;
		if (pFrame == NULL)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			delete pDocument;       // explicit delete on error
			return NULL;
		}
	}

	if (lpszPathName == NULL)
	{
		// create a new document
		SetDefaultTitle(pDocument, (CMainFrame*)pFrame);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		if (!pDocument->OnNewDocument())
		{
			// user has been alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE.\n");
			if (bCreated)
				pFrame->DestroyWindow();    // will destroy document
			return NULL;
		}
		//タブの設定
	//	((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.AddTab(pDocument->GetTitle(), ((CTheDoc*)pDocument)->m_pMainObjLay);
//wp
		//タブの設定
		((CMainFrame*)pFrame)->m_wndFileSelectBar.AddTab(pDocument->GetTitle(), ((CTheDoc*)pDocument)->m_pMainObjLay);
	}
	else
	{
		CWaitCursor wait;

		// open an existing document
		bWasModified = pDocument->IsModified();
		pDocument->SetModifiedFlag(FALSE);  // not dirty for open
		
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has been alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE.\n");
			if (bCreated) {
				pFrame->DestroyWindow();    // will destroy document
			} else if (!pDocument->IsModified()) {
				// original document is untouched
				pDocument->SetModifiedFlag(bWasModified);
			} else {
				// we corrupted the original document
				SetDefaultTitle(pDocument, (CMainFrame*)pFrame);

				if (!pDocument->OnNewDocument()) {
					TRACE0("Error: OnNewDocument failed after trying to open a document - trying to continue.\n");
					// assume we can continue
				}
			}
			return NULL;        // open failed
		}
		//タブの設定
		((CMainFrame*)pFrame)->m_wndFileSelectBar.AddTab("", ((CTheDoc*)pDocument)->m_pMainObjLay);
		//takaso2008
		((CTheDoc*)pDocument)->SetPathName(lpszPathName, (CMainFrame*)pFrame);
	}

	CWinThread* pThread = AfxGetThread();
	if (bCreated && pThread->m_pMainWnd == NULL)
	{
		// set as main frame (InitialUpdateFrame will show the window)
		pThread->m_pMainWnd = pFrame;
	}


	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);

	SetKnjIndicators(pDocument);

	return pDocument;
}

void CTheDocTemplate::SetKnjIndicators(CDocument *pDocument)
{
	string strT;
	if (pDocument && ((CTheDoc*)pDocument)->m_pMainObjLay){
		CObjLayTbl* pMainObjLay = ((CTheDoc*)pDocument)->m_pMainObjLay;
		strT = pMainObjLay->getKnjcode();
		if (strT.empty() || strT=="S")
			strT = "SJIS";
		else if (strT=="J")
			strT = "JIS";
		else if (strT=="E")
			strT = "EUC";
		else if (strT=="U")
			strT = "UTF-8";
		((CMainFrame*)AfxGetMainWnd())->SetPaneText((char*)strT.c_str(), 3);
	}
}


CDocument* CTheDocTemplate::SelectDocumentFile(CObjLayTbl *pLay)
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;

	assert(m_pOnlyDoc != NULL);
	pDocument = m_pOnlyDoc;
	pFrame = (CFrameWnd*)AfxGetMainWnd();
	ASSERT(pFrame != NULL);
	ASSERT_KINDOF(CFrameWnd, pFrame);
	ASSERT_VALID(pFrame);

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pDocument == m_pOnlyDoc);

	((CTheDoc*)pDocument)->SelectDocument(pLay);

	InitialUpdateFrame(pFrame, pDocument, true);
	SetKnjIndicators(pDocument);

	return pDocument;
}

void CTheDocTemplate::SetDefaultTitle(CDocument* pDocument, CMainFrame* pFrame)
{
	CString strDocName;
	if (GetDocString(strDocName, CDocTemplate::docName) &&
		!strDocName.IsEmpty())
	{

	}
	else
	{
		// use generic 'untitled' - ignore untitled count
		VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
	}
//WP
	if (((CMainFrame*)pFrame)->m_wndFileSelectBar.GetWindowCount()==0)
		m_nUntitledCount = 0;

	TCHAR szNum[8];
	wsprintf(szNum, _T(" %d"), ++m_nUntitledCount);
	strDocName += szNum;

	pDocument->SetTitle(strDocName);
}

/////////////////////////////////////////////////////////////////////////////
// CTheDocTemplate diagnostics
#ifdef _DEBUG
void CTheDocTemplate::Dump(CDumpContext& dc) const
{
	CDocTemplate::Dump(dc);

	dc << "\nm_nUntitledCount = " << m_nUntitledCount;
	if (m_pOnlyDoc)
		dc << "with document: " << (void*)m_pOnlyDoc;
	else
		dc << "with no document";
	dc << "\n";
}

void CTheDocTemplate::AssertValid() const
{
	CDocTemplate::AssertValid();
	if (m_pOnlyDoc)
		ASSERT_VALID(m_pOnlyDoc);
}
#endif //_DEBUG


