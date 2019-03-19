
// TreeShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TreeShow.h"
#include "TreeShowDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

														// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTreeShowDlg �Ի���


CTreeShowDlg::CTreeShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTreeShowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTreeShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_PIC_SHOW, m_PicShow);
}

BEGIN_MESSAGE_MAP(CTreeShowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, &CTreeShowDlg::OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTreeShowDlg::OnSelchangedTree)
//	ON_NOTIFY(NM_CLICK, IDC_LIST, &CTreeShowDlg::OnClickList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CTreeShowDlg::OnNMRClickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CTreeShowDlg::OnNMClickList)
END_MESSAGE_MAP()


// CTreeShowDlg ��Ϣ�������

BOOL CTreeShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	//-----------------------------------------------------------------------------
	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1
	SetMenu(&m_Menu);
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("δ�ܴ���������\n");
		AfxMessageBox(_T("δ�ܴ���������\n"));
		return FALSE;      // δ�ܴ���  
	}
	m_ToolBar.ShowWindow(SW_SHOW);
	//�ؼ�����λ  
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	//------------------------------------------------------------------
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

									// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ImageList.Create(32, 32, ILC_COLOR32, 10, 30);     //����ͼ������CImageList���� 
	HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //ͼ����
	m_ImageList.Add(hIcon);                          //ͼ����ӵ�ͼ������
	m_list.SetImageList(&m_ImageList, LVSIL_NORMAL);  //Ϊ���οؼ�����ͼ������   
	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_hRoot = m_tree.InsertItem("�ҵĵ���");         //������ڵ�
	GetLogicalDrives(m_hRoot);                       //�Զ��庯�� ��ȡ����
	GetDriveDir(m_hRoot);                            //�Զ��庯�� ��ȡ��������
	m_tree.Expand(m_hRoot, TVE_EXPAND);               //չ�����۵������б� TVE_EXPANDչ���б� 
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



void CTreeShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTreeShowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTreeShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//*******************************************************
//����Զ��庯��

//��������:��ȡ������ ����:·����                       
void CTreeShowDlg::GetLogicalDrives(HTREEITEM hParent)
{
	//��ȡϵͳ�����������ַ�����Ϣ
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //�������ܳ���
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //��������
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //������С
	while (szDriveString > 0)
	{
		m_tree.InsertItem(pDriveStrings, hParent);       //�ڸ��ڵ�hParent����̷�
		pDriveStrings += szDriveString + 1;             //pDriveStrings��C:\ D:\ E:\��
		szDriveString = strlen(pDriveStrings);
	}
}

//��������:��ȡ�����̷������������ļ���
void CTreeShowDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_tree.GetChildItem(hParent);   //��ȡָ��λ���е�����
	while (hChild)
	{
		CString strText = m_tree.GetItemText(hChild);  //�����б�����Ŀ����
		if (strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�
			strText += _T("\\");
		strText += "*.*";
		//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ
		CFileFind file;                                       //���屾���ļ�����
		BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //������һ���ļ�
			if (file.IsDirectory() && !file.IsDots() && !file.IsHidden())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."
				m_tree.InsertItem(file.GetFileName(), hChild); //����̷�·������״�ļ���
		}
		GetDriveDir(hChild);                                  //�ݹ����
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //��ȡ���οؼ�(TVGN_NEXT)��һ�ֵ���
	}
}


//��������:չ���¼�����
void CTreeShowDlg::OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TVITEM item = pNMTreeView->itemNew;                  //����\���ܹ���������ͼ��Ŀ��Ϣ
	if (item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //��ȡָ��λ���е�����
	while (hChild)
	{
		AddSubDir(hChild);                               //�����Ŀ¼
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //��ȡ���οؼ�TVGN_NEXT���ֵ���
	}
	*pResult = 0;
}


//��������:�����Ŀ¼
void CTreeShowDlg::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //��ȡȫ·��
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //���Ұ����ַ������ļ�
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //������һ���ļ�
		if (file.IsDirectory() && !file.IsDots() && !file.IsHidden())
			m_tree.InsertItem(file.GetFileName(), hParent);
	}
}



//��������:��ȡ����Ŀȫ��·��
CString CTreeShowDlg::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while (hCurrent != m_hRoot)
	{
		strTemp = m_tree.GetItemText(hCurrent);    //�����б�����Ŀ����
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = m_tree.GetParentItem(hCurrent); //���ظ���Ŀ���
	}
	return strReturn;
}


//��������:ѡ���¼���ʾͼ��
void CTreeShowDlg::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == m_hRoot)
		return;
	CString str = GetFullPath(item.hItem);
	if (str.Right(1) != "\\")
		str += "\\";
	str += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(str);
	// TODO: Add extra initialization here   
	//CRect rect;

	//// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	//m_list.GetClientRect(&rect);

	//// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_list.InsertColumn(0, _T(" ") , LVCFMT_CENTER, rect.Width() / 3, 0);
	//m_list.InsertColumn(1, _T("url"), LVCFMT_CENTER, rect.Width() / 3, 1);
	//m_list.InsertColumn(1, _T("path"), LVCFMT_CENTER, rect.Width() / 3, 2);

	while (bContinue)
	{
		bContinue = file.FindNextFile();
		if (!file.IsDots() && !file.IsHidden())
		{
			/*CString temp = str;
			int index = temp.Find("*.*");
			temp.Delete(index, 3);*/
			/*HICON hIcon = (HICON)::LoadImage(NULL, file.GetFilePath(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			int i = m_ImageList.Add(hIcon);
			m_list.InsertItem(i, file.GetFileName(), i);*/
			////------------------------------------------------------
			//CBitmap bitmap;
			//HBITMAP hbitmap;
			//hbitmap = (HBITMAP)LoadImage(NULL, file.GetFilePath(), IMAGE_BITMAP, 0, 0,
			//	LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
			//bitmap.Attach(hbitmap);
			//int i = m_ImageList.Add(&bitmap, RGB(0, 128, 128));
			//m_list.InsertItem(i, file.GetFileName(), i);
			////------------------------------------------------------
			SHFILEINFO info;
			CString temp = str;
			int index = temp.Find("*.*");
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_ImageList.Add(info.hIcon);


			CString path = _T("Hello");
			int num = path.GetLength();
			char* result = new char[num + 1];
			strcpy_s(result, num +1 , path );

			

			m_list.InsertItem(i, info.szDisplayName, i);
			m_list.SetItemData(i, (DWORD)result);

			/*CString tmp = file.GetFileURL();
			long PID = _ttol(tmp);
			DWORD dw = (DWORD)PID;
			m_list.SetItemData(i, dw);*/
			
			
		}
	}
	*pResult = 0;
}

//void CTreeShowDlg::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//m_list.DeleteAllItems();
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	TVITEM item = pNMTreeView->itemNew;
//	if (m_list.GetItemCount() <= 0)
//	{
//		return;
//	}
//	//û���б�ѡ�в���ʾ
//	if (m_list.GetSelectedCount() > 0)
//	{
//		//if (item.hItem == m_hRoot)
//		//	return;
//		//CString str = GetFullPath(item.hItem);
//		//CImage image;
//		//image.Load(str);
//
//		////��ȡͼƬ�ͻ���λ��
//		//CRect imageRect;
//		//GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&imageRect);
//
//		////CDC�ඨ������豸�����Ķ�����࣬�ֳ��豸���������ࡣ������Ҳ���������˺ܶ�о����ǻ�ͼʱ��Ҫ
//		//CDC *picDC = GetDC();
//		//image.Draw(picDC->m_hDC, imageRect);
//		//ReleaseDC(picDC);        //�ͷ�ָ��
//	}
//}


//  ����Ҽ��˵�
void CTreeShowDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_list.GetItemCount() <= 0)
	{
		return;
	}
	//û���б�ѡ�в���ʾ
	if (m_list.GetSelectedCount() > 0)
	{
		CMenu menu, *popup;
		if (menu.LoadMenu(IDR_MENU2) == NULL)
		{
			//����menuΪ��
			DWORD dwRect = GetLastError();
			DWORD a = dwRect;
			return;
		}
		popup = menu.GetSubMenu(0);
		CPoint point;
		ClientToScreen(&point);
		GetCursorPos(&point);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		*pResult = 0;
	}
	else
	{
		return;
	}
}


void CTreeShowDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strLangName;    // ѡ�����Ե������ַ���   
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{

		// ��ȡ��ѡ���б����һ��������ı�   
		strLangName = m_list.GetItemText(pNMListView->iItem, 0);
		//DWORD dw = m_list.GetItemData(pNMListView->iItem);
		//strLangName.Format(_T("%d"), dw);
		char* PATH = (char*)m_list.GetItemData(pNMListView->iItem);
		// ��ѡ���������ʾ��༭����   
		SetDlgItemText(IDC_EDIT1, PATH);
		//CImage image;
		//int cx, cy;
		//CRect imageRect;
		//image.Load(strLangName);
		//cx = image.GetWidth();
		//cy = image.GetHeight();

		////��ȡͼƬ�ͻ���λ��
		//CWnd *pWnd = NULL;
		//pWnd = GetDlgItem(IDC_PIC_SHOW);//��ȡ�ؼ����
		//// ��ȡPicture Control�ؼ��Ŀͻ���
		//pWnd->GetClientRect(&imageRect);
		//CDC *pDc = NULL;  
		//pDc = pWnd->GetDC();//��ȡpicture control��DC  
		////����ָ���豸�����е�λͼ����ģʽ
		//int ModeOld=SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		////��Դ�����и���һ��λͼ��Ŀ����Σ���Ŀ���豸���õ�ģʽ����ͼ��������ѹ��
		//image.StretchBlt(pDc->m_hDC, imageRect,SRCCOPY);
		//SetStretchBltMode(pDc->m_hDC,ModeOld); 
		//ReleaseDC(pDc);
		
	
	}
}
