
// TreeShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeShow.h"
#include "TreeShowDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ShowDlg.h"
#include <gdiplus.h>
using namespace Gdiplus;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define THUMBNAIL_WIDTH     100
#define THUMBNAIL_HEIGHT    75
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
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CTreeShowDlg::OnNMRClickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CTreeShowDlg::OnNMClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTreeShowDlg::OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON1, &CTreeShowDlg::OnBnClickedButton1)
	ON_COMMAND(ID_OPEN_FILE_1, &CTreeShowDlg::OnOpenFile1)
	ON_COMMAND(ID_LEFT_1, &CTreeShowDlg::OnLeft1)
	ON_COMMAND(ID_RIGHT_1, &CTreeShowDlg::OnRight1)
	ON_COMMAND(ID_SELECTMENU_1, &CTreeShowDlg::OnSelectmenuShow)
	ON_COMMAND(ID_SELECTMENU_2, &CTreeShowDlg::OnSelectmenuCopy)
	ON_COMMAND(ID_SELECTMENU_3, &CTreeShowDlg::OnSelectmenuPaste)
	ON_COMMAND(ID_SELECTMENU_4, &CTreeShowDlg::OnSelectmenuShear)
	ON_COMMAND(ID_SELECTMENU_DEL, &CTreeShowDlg::OnSelectmenuDelete)
END_MESSAGE_MAP()


// CTreeShowDlg 消息处理程序

BOOL CTreeShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	//  初始化CListControl
	//------------------------------------------------------------------------------
	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_list.InsertColumn(0, _T(" "), LVCFMT_IMAGE, rect.Width(), 0);
	m_list.InsertColumn(1, _T("name"), LVCFMT_CENTER, 0, 1);
	m_list.InsertColumn(2, _T("path"), LVCFMT_CENTER, 0, 2);
	m_list.InsertColumn(3,_T("ParentPath"), LVCFMT_CENTER,0,3);
	m_list.InsertColumn(4, _T("size"), LVCFMT_CENTER, 0, 4);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT );//  删除  | LVS_EX_GRIDLINES   不显示网格线
	//------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1
	SetMenu(&m_Menu);
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR2))
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
	GetDlgItem(IDC_PIC_SHOW)->ShowWindow(FALSE);   //  将PictureControl禁用
	GetDlgItem(IDC_PIC_SHOW)->ShowWindow(TRUE);    //  显示picturecontrol
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == m_hRoot)
		return;
	CString str = GetFullPath(item.hItem);
	//  添加历史路径
	PathList.Add(str);
	count++;

	showListControl(str);
	*pResult = 0;
}

void CTreeShowDlg::showListControl(CString path) {
	m_list.DeleteAllItems();
	CString str = path;
	//CString str = _T("C:\\Users\\Public\\Pictures\\Sample Pictures\\");
	if (str.Right(1) != "\\")
		str += "\\";
	//SetDlgItemText(IDC_EDIT1, str);
	//SetDlgItemText(IDC_STATIC, _T(" "));
	str += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(str);
	//// TODO: Add extra initialization here   


	while (bContinue)
	{
		bContinue = file.FindNextFile();
		if (!file.IsDots() && !file.IsHidden())
		{
			SHFILEINFO info;
			CString temp = str;
			int index = temp.Find("*.*");
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_ImageList.Add(info.hIcon);
			m_list.SetImageList(&m_ImageList, LVSIL_SMALL);

			int N = m_list.InsertItem(i, info.szDisplayName, i);
			m_list.SetItemText(N, 1, file.GetFileName());
			m_list.SetItemText(N, 2, file.GetFilePath());
			m_list.SetItemText(N, 3, path);
			CString size;
			size.Format(_T("%d"),file.GetLength());
			m_list.SetItemText(N, 4, size);
		}
	}
	pathArray.RemoveAll();
	CString test;
	num = m_list.GetItemCount();
	//  测试
	//test.Format("%d", num);
	pathArray.SetSize(num);
	//SetDlgItemText(IDC_EDIT1, test);
	for (int i = 0; i < num; i++)
	{
		pathArray.SetAt(i, m_list.GetItemText(i, 2));
	}
	test.Format("%d", pathArray.GetSize());
	SetDlgItemText(IDC_EDIT1, test);
}



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
	CString path;    // 选择的文件的路径   
	CString name;    //  选择的文件的名称

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	int ci = pNMListView->iItem;
	if (-1 != ci)        // 如果iItem不是-1，就说明有列表项被选择   
	{

		// 获取被选择列表项第一个子项的文本
		path = m_list.GetItemText(ci, 2);
		name = m_list.GetItemText(ci, 1);
		CString Parentpath = m_list.GetItemText(ci, 3);
		// 将选择的语言显示与编辑框中   
		//SetDlgItemText(IDC_EDIT1, Parentpath);
		//SetDlgItemText(IDC_STATIC, name);
		//  判断是否为图片，
		if(name.Find(_T(".jpg")) == -1 && name.Find(_T(".bmp")) == -1 && name.Find(_T(".gif")) == -1 && name.Find(_T(".png")) == -1 && name.Find(_T(".ico")) == -1 && name.Find(_T(".mng")) == -1 && name.Find(_T(".pnm")) == -1) {
			GetDlgItem(IDC_PIC_SHOW)->ShowWindow(FALSE);   //  将PictureControl禁用
			GetDlgItem(IDC_PIC_SHOW)->ShowWindow(TRUE);    //  显示picturecontrol
			return;	
		}
		else {
			CImage image;
			int cx, cy;
			CRect imageRect;
			image.Load(path);
			cx = image.GetWidth();
			cy = image.GetHeight();

			//获取图片客户区位置
			CWnd *pWnd = NULL;
			pWnd = GetDlgItem(IDC_PIC_SHOW);//获取控件句柄
											// 获取Picture Control控件的客户区
			pWnd->GetClientRect(&imageRect);
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//获取picture control的DC  
								//设置指定设备环境中的位图拉伸模式
			int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
			//从源矩形中复制一个位图到目标矩形，按目标设备设置的模式进行图像的拉伸或压缩
			image.StretchBlt(pDc->m_hDC, imageRect, SRCCOPY);
			SetStretchBltMode(pDc->m_hDC, ModeOld);
			ReleaseDC(pDc);
		}
		
	}
}

//  双击事件
void CTreeShowDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	CString path;    // 选择的文件的路径   
	CString name;    //  选择的文件的名称
	

	//CString test;
	//int num = m_list.GetItemCount();
	////  测试
	////test.Format("%d", num);
	//pathArray.SetSize(num);
	////SetDlgItemText(IDC_EDIT1, test);
	//for (int i = 0; i < num; i++)
	//{
	//	pathArray.SetAt(i,m_list.GetItemText(i, 2));
	//}
	//test.Format("%d", pathArray.GetSize());
	////SetDlgItemText(IDC_EDIT1, test);


	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	int ci = pNMListView->iItem;
	if (-1 != ci)        // 如果iItem不是-1，就说明有列表项被选择   
	{

		// 获取被选择列表项第一个子项的文本
		path = m_list.GetItemText(ci, 2);
		//SetDlgItemText(IDC_EDIT1, path);
		CFileFind file;
		BOOL b = file.FindFile(path);
		file.FindNextFile();
		CString name = file.GetFileName();
		if (b && file.IsDirectory()) {
			//SetDlgItemText(IDC_EDIT1, path);
			m_list.DeleteAllItems();
			showListControl(path);
			//  添加历史路径
			PathList.Add(path);
			count++;
		}
		if (b && !file.IsDirectory()) {
			if ( name.Find(_T(".jpg")) != -1 || name.Find(_T(".bmp")) != -1 || name.Find(_T(".gif")) != -1 || name.Find(_T(".png")) != -1 ||  name.Find(_T(".ico")) != -1 || name.Find(_T(".mng")) != -1 || name.Find(_T(".pnm")) != -1 ) 
			{
				CShowDlg spic;
				spic.name = name;
				spic.path = path;
				spic.num = num;
				spic.ci = ci;
				spic.pathArray = ReturnCStringArray();
				spic.DoModal();
			}
			else
			{
				return;
			}
		}
		
	}
}


//  测试按钮
void CTreeShowDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

CStringArray* CTreeShowDlg::ReturnCStringArray()
{
	return &pathArray;
}




void CTreeShowDlg::OnOpenFile1()
{
	// TODO: 在此添加命令处理程序代码
}

//  历史路径
void CTreeShowDlg::OnLeft1()
{
	// TODO: 在此添加命令处理程序代码
	if (count <= 1) {
		return;
	}
	else {
		int i = count - 1;
		CString strpath = PathList.GetAt(i - 1);
		showListControl(strpath);
		count--;
	}
}
void CTreeShowDlg::OnRight1()
{
	// TODO: 在此添加命令处理程序代码
	if (count >= PathList.GetSize()) {
		return;
	}
	else {
		int i = count;
		CString strpath = PathList.GetAt(i);
		showListControl(strpath);
		count++;
	}
}


//  显示
void CTreeShowDlg::OnSelectmenuShow()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int index = m_list.GetNextSelectedItem(pos);


	//CString test;
	//int num = m_list.GetItemCount();
	////  测试
	////test.Format("%d", num);
	//pathArray.SetSize(num);
	////SetDlgItemText(IDC_EDIT1, test);
	//for (int i = 0; i < num; i++)
	//{
	//	pathArray.SetAt(i, m_list.GetItemText(i, 2));
	//}
	//test.Format("%d", pathArray.GetSize());
	////SetDlgItemText(IDC_EDIT1, test);


	if (index == -1) {
		return;
	}
	else {
		CString str = m_list.GetItemText(index, 2);
		CShowDlg spic1;
		spic1.path = str;
		spic1.num = num;
		spic1.ci = index;
		spic1.pathArray = ReturnCStringArray();
		spic1.DoModal();
	}
}




//  复制
void CTreeShowDlg::OnSelectmenuCopy()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int index = m_list.GetNextSelectedItem(pos); 
	//  判断是否有选中Item
	if (index == -1) {
		return;
	}
	else {
		 srcPath = m_list.GetItemText(index, 2);
		 FLAG = FALSE;
	}
}

//  粘贴
void CTreeShowDlg::OnSelectmenuPaste()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int index = m_list.GetNextSelectedItem(pos);
	//  判断是否有选中Item
	if (index == -1 && srcPath.IsEmpty()) {
		return;
	}
	else 
	{
		CString destPath = m_list.GetItemText(index,3);
		if (FLAG == FALSE) 
		{
			CopyDirectory(srcPath, destPath);
		}
		else 
		{
			CopyDirectory(srcPath , destPath);
			CFileFind finder;
			BOOL b = finder.FindFile(srcPath);
			finder.FindNextFile();
			if (b && finder.IsDirectory() && !finder.IsDots()) 
			{
				system("rmdir /s/q \""+ srcPath +"\"");
			}
			else if(!finder.IsDirectory() && !finder.IsDots())
			{
				DeleteFile(srcPath);
			}
			else
			{
				return;
			}
		}
		showListControl(destPath);
	}
}

//  复制文件
BOOL CTreeShowDlg::CopyDirectory(CString strSouDir, CString strDesDir)
{
	//CFileFind m_sFileFind;
	//if (strSrcPath.IsEmpty())
	//{
	//	OutputDebugString("源文件名为空，无法进行拷贝!");
	//	return FALSE;
	//}
	//if (!m_sFileFind.FindFile(strDestPath))
	//{
	//	CreateDirectory(strDestPath, NULL);//创建目标文件夹
	//}
	//CFileFind finder;
	////CString path;
	////path.Format("%s/*.*", strSrcPath);
	////AfxMessageBox(path);
	//BOOL bWorking = finder.FindFile(strSrcPath);
	//while (bWorking)
	//{
	//	bWorking = finder.FindNextFile();
	//	//AfxMessageBox(finder.GetFileName());
	//	if (finder.IsDirectory() && !finder.IsDots())//是文件夹 而且 名称不含 . 或 ..  
	//	{
	//		CopyDirectory(finder.GetFilePath(), strDestPath + "\\" + finder.GetFileName());        //递归创建文件夹+"/"+finder.GetFileName()  
	//	}
	//	else
	//	{//是文件，则直接复制
	//	//AfxMessageBox("复制文件"+finder.GetFilePath());//+finder.GetFileName()  
	//		CopyFile(finder.GetFilePath(), strDestPath + "\\" + finder.GetFileName(), FALSE);
	//	}
	//}
	//
	//return TRUE;

	CFileFind file;
	BOOL flag = file.FindFile(strSouDir);
	file.FindNextFile();
	if (flag && file.IsDirectory() && !file.IsDots()) {
		CString cmd = "xcopy /cherky \"" + strSouDir + "\" \"" + strDesDir +"\\"+ file.GetFileName()+"\\\"";
		SetDlgItemText(IDC_EDIT1, cmd);
		system(cmd);
	}
	else if(flag && !file.IsDirectory() && !file.IsDots()){
		system("copy \""+strSouDir+ "\" \"" +strDesDir+"\" /y ");
	}
	else {
		return FALSE;
	}
	//system("xcopy /cherky \" "+strSouDir+"\" \" "+ strDesDir +"\"");
	return TRUE;
}

//  剪切
void CTreeShowDlg::OnSelectmenuShear()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int index = m_list.GetNextSelectedItem(pos);
	//  判断是否有选中Item并且
	if (index == -1 ) {
		return;
	}
	else {
		srcPath = m_list.GetItemText(index, 2);
		FLAG = TRUE;
	}
	
}

//  删除
void CTreeShowDlg::OnSelectmenuDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int index = m_list.GetNextSelectedItem(pos);
	//  判断是否有选中Item并且
	if (index == -1) {
		return;
	}
	else
	{
		CString src = m_list.GetItemText(index,2);
		CString dest = m_list.GetItemText(index,3);
		CFileFind finder;
		BOOL b = finder.FindFile(src);
		finder.FindNextFile();
		if (b && finder.IsDirectory() && !finder.IsDots())
		{
			system("rmdir /s/q \"" + src + "\"");
		}
		else if (!finder.IsDirectory() && !finder.IsDots())
		{
			DeleteFile(src);
		}
		else
		{
			return;
		}
		showListControl(dest);
	}
	
}

//  属性