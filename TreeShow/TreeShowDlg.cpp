
// TreeShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeShow.h"
#include "TreeShowDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
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


// CTreeShowDlg 对话框


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


// CTreeShowDlg 消息处理程序

BOOL CTreeShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	//-----------------------------------------------------------------------------
	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1
	SetMenu(&m_Menu);
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("未能创建工具栏\n");
		AfxMessageBox(_T("未能创建工具栏\n"));
		return FALSE;      // 未能创建  
	}
	m_ToolBar.ShowWindow(SW_SHOW);
	//控件条定位  
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	//------------------------------------------------------------------
	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

									// TODO: 在此添加额外的初始化代码
	m_ImageList.Create(32, 32, ILC_COLOR32, 10, 30);     //创建图像序列CImageList对象 
	HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //图标句柄
	m_ImageList.Add(hIcon);                          //图标添加到图像序列
	m_list.SetImageList(&m_ImageList, LVSIL_NORMAL);  //为树形控件设置图像序列   
	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_hRoot = m_tree.InsertItem("我的电脑");         //插入根节点
	GetLogicalDrives(m_hRoot);                       //自定义函数 获取驱动
	GetDriveDir(m_hRoot);                            //自定义函数 获取驱动子项
	m_tree.Expand(m_hRoot, TVE_EXPAND);               //展开或折叠子项列表 TVE_EXPAND展开列表 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTreeShowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTreeShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//*******************************************************
//添加自定义函数

//函数功能:获取驱动器 参数:路径名                       
void CTreeShowDlg::GetLogicalDrives(HTREEITEM hParent)
{
	//获取系统分区驱动器字符串信息
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //驱动器总长度
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //建立数组
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //驱动大小
	while (szDriveString > 0)
	{
		m_tree.InsertItem(pDriveStrings, hParent);       //在父节点hParent添加盘符
		pDriveStrings += szDriveString + 1;             //pDriveStrings即C:\ D:\ E:\盘
		szDriveString = strlen(pDriveStrings);
	}
}

//函数功能:获取驱动盘符下所有子项文件夹
void CTreeShowDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_tree.GetChildItem(hParent);   //获取指定位置中的子项
	while (hChild)
	{
		CString strText = m_tree.GetItemText(hChild);  //检索列表中项目文字
		if (strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符
			strText += _T("\\");
		strText += "*.*";
		//将当前目录下文件枚举并InsertItem树状显示
		CFileFind file;                                       //定义本地文件查找
		BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //查找下一个文件
			if (file.IsDirectory() && !file.IsDots() && !file.IsHidden())          //找到文件为内容且不为点"."
				m_tree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹
		}
		GetDriveDir(hChild);                                  //递归调用
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件(TVGN_NEXT)下一兄弟项
	}
}


//函数功能:展开事件函数
void CTreeShowDlg::OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	TVITEM item = pNMTreeView->itemNew;                  //发送\接受关于树形视图项目信息
	if (item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //获取指定位置中的子项
	while (hChild)
	{
		AddSubDir(hChild);                               //添加子目录
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //获取树形控件TVGN_NEXT下兄弟项
	}
	*pResult = 0;
}


//函数功能:添加子目录
void CTreeShowDlg::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //获取全路径
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (file.IsDirectory() && !file.IsDots() && !file.IsHidden())
			m_tree.InsertItem(file.GetFileName(), hParent);
	}
}



//函数功能:获取树项目全根路径
CString CTreeShowDlg::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while (hCurrent != m_hRoot)
	{
		strTemp = m_tree.GetItemText(hCurrent);    //检索列表中项目文字
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = m_tree.GetParentItem(hCurrent); //返回父项目句柄
	}
	return strReturn;
}


//函数功能:选中事件显示图标
void CTreeShowDlg::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
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

	//// 获取编程语言列表视图控件的位置和大小   
	//m_list.GetClientRect(&rect);

	//// 为列表视图控件添加全行选中和栅格风格   
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
//	// TODO: 在此添加控件通知处理程序代码
//	//m_list.DeleteAllItems();
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	TVITEM item = pNMTreeView->itemNew;
//	if (m_list.GetItemCount() <= 0)
//	{
//		return;
//	}
//	//没有列被选中不显示
//	if (m_list.GetSelectedCount() > 0)
//	{
//		//if (item.hItem == m_hRoot)
//		//	return;
//		//CString str = GetFullPath(item.hItem);
//		//CImage image;
//		//image.Load(str);
//
//		////获取图片客户区位置
//		//CRect imageRect;
//		//GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&imageRect);
//
//		////CDC类定义的是设备上下文对象的类，又称设备环境对象类。具体我也不懂，差了很多感觉就是画图时需要
//		//CDC *picDC = GetDC();
//		//image.Draw(picDC->m_hDC, imageRect);
//		//ReleaseDC(picDC);        //释放指针
//	}
//}


//  添加右键菜单
void CTreeShowDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_list.GetItemCount() <= 0)
	{
		return;
	}
	//没有列被选中不显示
	if (m_list.GetSelectedCount() > 0)
	{
		CMenu menu, *popup;
		if (menu.LoadMenu(IDR_MENU2) == NULL)
		{
			//加载menu为空
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strLangName;    // 选择语言的名称字符串   
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{

		// 获取被选择列表项第一个子项的文本   
		strLangName = m_list.GetItemText(pNMListView->iItem, 0);
		//DWORD dw = m_list.GetItemData(pNMListView->iItem);
		//strLangName.Format(_T("%d"), dw);
		char* PATH = (char*)m_list.GetItemData(pNMListView->iItem);
		// 将选择的语言显示与编辑框中   
		SetDlgItemText(IDC_EDIT1, PATH);
		//CImage image;
		//int cx, cy;
		//CRect imageRect;
		//image.Load(strLangName);
		//cx = image.GetWidth();
		//cy = image.GetHeight();

		////获取图片客户区位置
		//CWnd *pWnd = NULL;
		//pWnd = GetDlgItem(IDC_PIC_SHOW);//获取控件句柄
		//// 获取Picture Control控件的客户区
		//pWnd->GetClientRect(&imageRect);
		//CDC *pDc = NULL;  
		//pDc = pWnd->GetDC();//获取picture control的DC  
		////设置指定设备环境中的位图拉伸模式
		//int ModeOld=SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		////从源矩形中复制一个位图到目标矩形，按目标设备设置的模式进行图像的拉伸或压缩
		//image.StretchBlt(pDc->m_hDC, imageRect,SRCCOPY);
		//SetStretchBltMode(pDc->m_hDC,ModeOld); 
		//ReleaseDC(pDc);
		
	
	}
}
