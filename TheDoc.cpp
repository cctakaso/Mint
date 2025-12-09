// TheDoc.cpp : CTheDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "The.h"
#include "TheDoc.h"
#include "ObjLay.h"
#include "MainFrm.h"
#include "TheView.h"
#include "FileSelectBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTheDoc

IMPLEMENT_DYNCREATE(CTheDoc, CDocument)

BEGIN_MESSAGE_MAP(CTheDoc, CDocument)
	//{{AFX_MSG_MAP(CTheDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE_AS_SJIS, &CTheDoc::OnFileSaveAsSjis)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_SJIS, &CTheDoc::OnUpdateFileSaveAsSjis)
	ON_COMMAND(ID_FILE_SAVE_AS_JIS, &CTheDoc::OnFileSaveAsJis)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_JIS, &CTheDoc::OnUpdateFileSaveAsJis)
	ON_COMMAND(ID_FILE_SAVE_AS_EUC, &CTheDoc::OnFileSaveAsEuc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_EUC, &CTheDoc::OnUpdateFileSaveAsEuc)
	ON_COMMAND(ID_FILE_SAVE_AS_UTF8, &CTheDoc::OnFileSaveAsUtf8)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_UTF8, &CTheDoc::OnUpdateFileSaveAsUtf8)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTheDoc クラスの構築/消滅

CTheDoc::CTheDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
	m_pMainObjLay = null;
}

CTheDoc::~CTheDoc()
{
}

BOOL CTheDoc::OnNewDocument()
{
//	if (m_pMainObjLay && !m_pMainObjLay->IsFormatAll()) {
//		AfxMessageBox("遅延ﾛｰﾃﾞｨﾝｸﾞ中です。");
//		return FALSE;
//	}
	if (!CDocument::OnNewDocument())
		return FALSE;

	bool bFirst = m_pMainObjLay==null;
	//bool bEditMenuOld = bFirst ? false:m_pMainObjLay->m_bEditMenu;
	OBJOFF idxMax = theObjTbl.size();
	m_pMainObjLay = theObjTbl.NewMainFile();
	if (m_pMainObjLay) {
		m_pMainObjLay->m_strTitle = GetTitle();
#ifdef SINGLE_TEMPLATE
		UpdateAllViews(NULL, HINT_CLOSE_DOC);
		theObjTbl.CloseBeforFiles(idxMax);
#endif //#ifdef SINGLE_TEMPLATE
		//if (bEditMenuOld != m_pMainObjLay->m_bEditMenu && AfxGetMainWnd()) {
			//set editmode! ダーティの時は、EditMenuからEditModeへ
		//	((CMainFrame*)AfxGetMainWnd())->OnEditMenu_NoChange();
		//}
	}
	return m_pMainObjLay!=null;
}



/////////////////////////////////////////////////////////////////////////////
// CTheDoc シリアライゼーション

void CTheDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		if (m_pMainObjLay) {
			int irtn;
			if ((irtn=m_pMainObjLay->SaveData(ar))<0) {
				AfxMessageBox(irtn==-1 ? 
					"バックアップ作成できない為、保存しませんでした。\nディスク領域等を確認して下さい。":
					"保存に失敗しました。\nディスク領域等を確認して下さい。");
			}else{
				m_pMainObjLay->GetActions().Initialize();
				SetDirtyDocTitle(false);
			}
		}
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTheDoc クラスの診断

#ifdef _DEBUG
void CTheDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTheDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTheDoc コマンド

BOOL CTheDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (m_pMainObjLay && !m_pMainObjLay->IsFormatAll()) {
//		AfxMessageBox("遅延ﾛｰﾃﾞｨﾝｸﾞ中です。");
//		return FALSE;
//	}

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	bool bFirst = m_pMainObjLay==null;
	//bool bEditMenuOld = bFirst ? false:m_pMainObjLay->m_bEditMenu;
	OBJOFF idxMax = theObjTbl.size();
	m_pMainObjLay = theObjTbl.NewMainFile(lpszPathName,
		(ios::in | ios::binary), 0, AfxGetMainWnd()==null ?  "":(LPSTR)((CMainFrame*)AfxGetMainWnd())->getShiteiKnjcode().c_str());
	
	if (m_pMainObjLay) {
		//takaso2008
		//m_pMainObjLay->m_strTitle = GetTitle();
		CFile cf(lpszPathName, CFile::modeRead|CFile::shareDenyNone); 
		m_pMainObjLay->m_strTitle = cf.GetFileTitle ();
#ifdef SINGLE_TEMPLATE
		UpdateAllViews(NULL, HINT_CLOSE_DOC);
		theObjTbl.CloseBeforFiles(idxMax);
#endif //#ifdef SINGLE_TEMPLATE
		//if (bEditMenuOld != m_pMainObjLay->m_bEditMenu && AfxGetMainWnd()) {
		//	//set editmode! ダーティの時は、EditMenuからEditModeへ
		//	((CMainFrame*)AfxGetMainWnd())->OnEditMenu_NoChange();
		//}
	}
	
	return m_pMainObjLay!=null;
}

void CTheDoc::SelectDocument(CObjLayTbl *pLay)
{
	m_pMainObjLay = pLay;
	bool bDirty = m_pMainObjLay->GetActions().IsEnableUndo();
	SetModifiedFlag(bDirty);
	if (m_pMainObjLay->GetPathName().size()>0) {
		SetPathName(m_pMainObjLay->GetPathName().data(), (CMainFrame*)AfxGetMainWnd());
	} else {
		m_strPathName = "";
		m_bEmbedded = FALSE;
		SetTitle(m_pMainObjLay->m_strTitle.data());
	}
	//イニシャルセット
	if (bDirty) {
		CString strTitle = GetTitle();
		SetDirtyPahtName(strTitle, bDirty);
		SetTitle(strTitle);
	}
}

void CTheDoc::OnCloseDocument() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//	if (m_pMainObjLay && !m_pMainObjLay->IsFormatAll()) {
//		AfxMessageBox("遅延ﾛｰﾃﾞｨﾝｸﾞ中です。");
//		return;
//	}
	CDocument::OnCloseDocument();
}

BOOL CTheDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return CDocument::OnSaveDocument(lpszPathName);
}

void CTheDoc::SetDirtyDocTitle(bool bDirty)
{
	CString strTitle = GetTitle();
	bool bDirtyOld = IsModified()==TRUE;
	if (bDirty != bDirtyOld /*|| 
		(bDirty && !strTitle.IsEmpty() && strTitle.Right(1)!='*')*/) {
		SetModifiedFlag(bDirty);
		SetDirtyPahtName(strTitle, bDirty);
		SetTitle(strTitle);
		((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.SetDocTitle(strTitle);
		assert(m_pMainObjLay);
		//if (bDirty && !m_pMainObjLay->m_bEditMenu && AfxGetMainWnd()) {
			//set editmode! ダーティの時は、EditMenuからEditModeへ
		//	((CMainFrame*)AfxGetMainWnd())->OnEditMenu();
		//}
	}
}
void CTheDoc::SetDirtyPahtName(CString &str, bool bDirty)
{
	bool bDirtyOld= (str.GetLength()>1 && str.Right(1)=='*');
	if (!bDirtyOld && bDirty)
		str += " *";
	else if (bDirtyOld && !bDirty)
		str.GetBufferSetLength(str.GetLength()-2);
}
void CTheDoc::SetPathName(LPCTSTR lpszPathName, CMainFrame *pMainWnd, BOOL bAddToMRU) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	//タブの設定
	CDocument::SetPathName(lpszPathName, bAddToMRU);
	CString str = GetTitle();
	bool bDirtyOld= (str.GetLength()>1 && str.Right(1)=='*');
	bool bDirty = IsModified()==TRUE;
	if (!bDirtyOld && bDirty)
		str += " *";
//takaso2008
	pMainWnd->m_wndFileSelectBar.SetDocTitle(str);
	//	((CMainFrame*)AfxGetMainWnd())->m_wndFileSelectBar.SetDocTitle(str);
}

void CTheDoc::OnFileSave() 
{
	CString newName = GetPathName();
	if (newName.IsEmpty()){
		OnFileSaveAs();
		return;
	}
	CDocument::OnFileSave();
}

void CTheDoc::OnFileSaveAs() 
{
	CString newName = GetPathName();
	CString str_back = GetTitle();
	if (newName.IsEmpty()){
		newName = str_back;
		SetDirtyPahtName(newName, false);
		if (newName.GetLength()>5 && newName.Left(5)=="無題 ") {
			CString strT = "無題";
			strT += newName.Mid(5);
			newName = strT;
		}
		if (newName.Find(".")<0) {
			newName += ".txt";
		}
	}

	CDocTemplate* pTemplate = GetDocTemplate();

	if (!AfxGetApp()->DoPromptFileName(newName,
	  AFX_IDS_SAVEFILE, // : AFX_IDS_SAVEFILECOPY,
	  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
		return ;       // don't even try to save

	if (newName.Find('.')==-1) {
		newName += ".txt";
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		// be sure to delete the file
		TRY
		{
			CFile::Remove(newName);
		}
		CATCH_ALL(e)
		{
			TRACE0("Warning: failed to delete file after failed SaveAs\n");
		}
		END_CATCH_ALL
		EndWaitCursor();
		return ;
	}

	SetPathName(newName, (CMainFrame*)AfxGetMainWnd());
	ASSERT(m_strPathName == newName);       // must be set

	EndWaitCursor();
}

void CTheDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified() && m_pMainObjLay && m_pMainObjLay->IsFormatAll());
}

void CTheDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pMainObjLay && m_pMainObjLay->IsFormatAll());
}


void CTheDoc::OnFileClose() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
}

void CTheDoc::OnFileSaveAsSjis()
{
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("S");
	OnFileSaveAs();
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("");
}

void CTheDoc::OnUpdateFileSaveAsSjis(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pMainObjLay && m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck(m_pMainObjLay && (m_pMainObjLay->getKnjcode()=="S" || m_pMainObjLay->getKnjcode().empty()));
}

void CTheDoc::OnFileSaveAsJis()
{
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("J");
	OnFileSaveAs();
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("");
}

void CTheDoc::OnUpdateFileSaveAsJis(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pMainObjLay && m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck(m_pMainObjLay && m_pMainObjLay->getKnjcode()=="J");
}

void CTheDoc::OnFileSaveAsEuc()
{
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("E");
	OnFileSaveAs();
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("");
}

void CTheDoc::OnUpdateFileSaveAsEuc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pMainObjLay && m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck(m_pMainObjLay && m_pMainObjLay->getKnjcode()=="E");
}

void CTheDoc::OnFileSaveAsUtf8()
{
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("U");
	OnFileSaveAs();
	((CMainFrame*)AfxGetMainWnd())->setShiteiKnjcode("");
}

void CTheDoc::OnUpdateFileSaveAsUtf8(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pMainObjLay && m_pMainObjLay->IsFormatAll());
	pCmdUI->SetCheck(m_pMainObjLay && m_pMainObjLay->getKnjcode()=="U");
}
