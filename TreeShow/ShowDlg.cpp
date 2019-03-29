// ShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeShow.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "time.h"
#include <gdiplus.h>
using namespace Gdiplus;
// CShowDlg 对话框

IMPLEMENT_DYNAMIC(CShowDlg, CDialogEx)

CShowDlg::CShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOW, pParent)
{

}

CShowDlg::~CShowDlg()
{
}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialogEx)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CShowDlg 消息处理程序

BOOL CShowDlg::OnInitDialog()
{

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

	//ShowGifPicture(path);           //  显示GIF图
	ShowPicture(path);                //  显示普通大小的图片
	//ShowPicture(path);              //  显示普通大小的图片
	//Effect_FullView(path);          //  全屏显示
	//Sleep(1000);
	//Effect_FullView(path);          //  全屏显示
	//ShowMaxPicture(path);           //  显示大图
	//Show2Picture();                 //  显示多张图片
	//ShowSlide(1000);                   //  幻灯片显示
	return TRUE;
}

//  显示GIF图
void CShowDlg::ShowGifPicture(CString path) {
	int cx, cy;
	image.Load(path);
	cx = image.GetWidth();
	cy = image.GetHeight();
	int nScreenWidth, nScreenHeight;
	//获得屏幕尺寸
	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	ShowWindow(SW_MAXIMIZE); //最大化显示
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC);//获取控件句柄
								  //Picture控件全屏显示
								  //pWnd->MoveWindow(CRect(0, 0, nScreenWidth, nScreenHeight));
	pWnd->MoveWindow(CRect((nScreenWidth - cx) / 2, (nScreenHeight - cy) / 2, (nScreenWidth + cx) / 2, (nScreenHeight + cy) / 2));
	CRect imageRect((nScreenWidth - cx) / 2, (nScreenHeight - cy) / 2, (nScreenWidth + cx) / 2, (nScreenHeight + cy) / 2);
	m_GifPic.Create(NULL, WS_CHILD | WS_VISIBLE | SS_ENHMETAFILE, imageRect, this, 1234);
	m_GifPic.Load(path);
	m_GifPic.Draw();
}
//  显示图片
void CShowDlg::ShowPicture(CString path) {
	CFileFind file;
	BOOL b = file.FindFile(path);
	file.FindNextFile();
	CString name = file.GetFileName();
	if (b && !file.IsDirectory()) {
		if (name.Find(_T(".jpg")) != -1 || name.Find(_T(".bmp")) != -1 || name.Find(_T(".gif")) != -1 || name.Find(_T(".png")) != -1 || name.Find(_T(".ico")) != -1 || name.Find(_T(".mng")) != -1 || name.Find(_T(".pnm")) != -1) 
		{
			int cx, cy;
			image.Load(path);
			cx = image.GetWidth();
			cy = image.GetHeight();
			int nScreenWidth, nScreenHeight;
			//获得屏幕尺寸
			nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
			nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
			CRect imageRect(0, 0, nScreenWidth, nScreenHeight);
			ShowWindow(SW_MAXIMIZE); //最大化显示
									 //获取图片客户区位置
			CWnd *pWnd = NULL;
			pWnd = GetDlgItem(IDC_STATIC);//获取控件句柄
										  //Picture控件全屏显示
										  //pWnd->MoveWindow(CRect(0, 0, nScreenWidth, nScreenHeight));
			pWnd->MoveWindow(CRect((nScreenWidth - cx) / 2, (nScreenHeight - cy) / 2, (nScreenWidth + cx) / 2, (nScreenHeight + cy) / 2));
			//隐藏按钮控件
			//GetDlgItem(IDC_BUTTON)->ShowWindow(SW_HIDE);
			// 获取Picture Control控件的客户区
			pWnd->GetClientRect(&imageRect);
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//获取picture control的DC  
								//Effect_Top2Bottom(pDc,cx,cy);// 从上到下
								//Effect_Bottom2Top(pDc, cx, cy);// 从下到上
								//Effect_Left2Right(pDc,cx,cy);// 从左到右
								//Effect_Right2Left(pDc,cx,cy);// 从右到左
								//ShowRotate(pDc); //  旋转显示 向左旋转90°

								//  无特效显示
								//设置指定设备环境中的位图拉伸模式
			int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
			//从源矩形中复制一个位图到目标矩形，按目标设备设置的模式进行图像的拉伸或压缩
			image.StretchBlt(pDc->m_hDC, imageRect, SRCCOPY);
			SetStretchBltMode(pDc->m_hDC, ModeOld);

			ReleaseDC(pDc);
			image.Destroy();
		}
		else
		{
			return;
		}
	}

	
}
//  显示带特效全屏图片
void CShowDlg::ShowMaxPicture(CString path) {
	int cx, cy;
	image.Load(path);
	cx = image.GetWidth();
	cy = image.GetHeight();
	int nScreenWidth, nScreenHeight;
	//获得屏幕尺寸
	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	CRect imageRect(0, 0, nScreenWidth, nScreenHeight);
	ShowWindow(SW_MAXIMIZE); //最大化显示
							 //获取图片客户区位置
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC);//获取控件句柄
								  //Picture控件全屏显示
	pWnd->MoveWindow(CRect(0, 0, nScreenWidth, nScreenHeight));
	//隐藏按钮控件
	//GetDlgItem(IDC_BUTTON)->ShowWindow(SW_HIDE);
	// 获取Picture Control控件的客户区
	pWnd->GetClientRect(&imageRect);
	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC  
						//Effect_Top2Bottom(pDc, nScreenWidth, nScreenHeight);// 从上到下
						//Effect_Bottom2Top(pDc, nScreenWidth, nScreenHeight);// 从下到上
						//Effect_Left2Right(pDc, nScreenWidth, nScreenHeight);// 从左到右
	Effect_Right2Left(pDc, nScreenWidth, nScreenHeight);// 从右到左
	ReleaseDC(pDc);
}

// 从上到下
void CShowDlg::Effect_Top2Bottom(CDC *pDC, int x, int y) {
	int height = image.GetHeight();
	/*for (int i = 1; i < height; i+=11)
	{
	if (height - i <= 11) {
	image.Draw(pDC->GetSafeHdc(),
	0, 0,
	image.GetWidth(), image.GetHeight(),
	0, 0,
	image.GetWidth(), image.GetHeight()
	);
	}
	else
	{
	image.Draw(pDC->GetSafeHdc(),
	0, 0,
	image.GetWidth(), i,
	0, height-i,
	image.GetWidth(), i
	);
	}
	Sleep(10);
	}*/
	//image.Draw(pDC->GetSafeHdc(),
	//	0, 0,
	//	x, y,
	//	0, 0,
	//	image.GetWidth(), image.GetHeight()
	//);

	for (int i = 1; i < height; i += 11)
	{
		if (height - i <= 12) {
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				x, y,
				0, 0,
				image.GetWidth(), image.GetHeight()
			);
		}
		else
		{
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				x, i * y / image.GetHeight(),
				0, 0,
				image.GetWidth(), i
			);
		}
		//Sleep(20);
	}
}
// 从下到上
void CShowDlg::Effect_Bottom2Top(CDC *pDC, int x, int y) {
	int height = image.GetHeight();
	for (int i = 1; i < height; i += 11)
	{
		if (height - i <= 11) {
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				x, y,
				0, 0,
				image.GetWidth(), image.GetHeight()
			);
		}
		else
		{
			image.Draw(pDC->GetSafeHdc(),
				0, y - i * y / image.GetHeight(),
				x, i * y / image.GetHeight(),
				0, 0,
				image.GetWidth(), i
			);
		}
		//Sleep(20);
	}
}
// 从左到右
void CShowDlg::Effect_Left2Right(CDC *pDC, int x, int y) {
	int w = image.GetWidth();
	for (int i = 1; i < w; i += 11)
	{
		if (w - i <= 11) {
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				x, y,
				0, 0,
				image.GetWidth(), image.GetHeight()
			);
		}
		else
		{
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				i*x / image.GetWidth(), y,
				w - i, 0,
				i, image.GetHeight()
			);
		}
		//Sleep(20);
	}
}
// 从右到左
void CShowDlg::Effect_Right2Left(CDC *pDC, int x, int y) {
	int w = image.GetWidth();
	for (int i = 1; i < w; i += 11)
	{
		if (w - i <= 11) {
			image.Draw(pDC->GetSafeHdc(),
				0, 0,
				x, y,
				0, 0,
				image.GetWidth(), image.GetHeight()
			);
		}
		else
		{
			image.Draw(pDC->GetSafeHdc(),
				x - i*x / w, 0,
				i*x / w, y,
				0, 0,
				i, image.GetHeight()
			);
		}
		//Sleep(20);
	}
}

// 全屏显示
void CShowDlg::Effect_FullView(CString path) {
	image.Load(path);
	int nScreenWidth, nScreenHeight;
	//获得屏幕尺寸
	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	CRect imageRect(0, 0, nScreenWidth, nScreenHeight);
	ShowWindow(SW_MAXIMIZE); //最大化显示
							 //获取图片客户区位置
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC);//获取控件句柄
								  //Picture控件全屏显示
	pWnd->MoveWindow(CRect(0, 0, nScreenWidth, nScreenHeight));
	//隐藏按钮控件
	//GetDlgItem(IDC_BUTTON)->ShowWindow(SW_HIDE);
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
//  幻灯片播放
void CShowDlg::Show2Picture() {
	//image.Load(path);
	//CString path[2] = { _T("F:\\Sample Pictures\\Koala.jpg"),_T("F:\\Sample Pictures\\Desert.jpg") };
	image.Load(path);
	int nScreenWidth, nScreenHeight;
	//获得屏幕尺寸
	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	CRect imageRect(0, 0, nScreenWidth, nScreenHeight);
	ShowWindow(SW_MAXIMIZE); //最大化显示
							 //获取图片客户区位置
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC);//获取控件句柄
								  //Picture控件全屏显示
	pWnd->MoveWindow(CRect(0, 0, nScreenWidth, nScreenHeight));
	//隐藏按钮控件
	//GetDlgItem(IDC_BUTTON)->ShowWindow(SW_HIDE);
	// 获取Picture Control控件的客户区
	pWnd->GetClientRect(&imageRect);
	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC  
						//设置指定设备环境中的位图拉伸模式
	int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//从源矩形中复制一个位图到目标矩形，按目标设备设置的模式进行图像的拉伸或压缩

	image.StretchBlt(pDc->m_hDC, imageRect, SRCCOPY);
	SetStretchBltMode(pDc->m_hDC, ModeOld);
	//CString path[2] = {pathArray->GetAt(0), pathArray->GetAt(1)};
	//for (int i = 0; i < 2; i++)
	//{
	//	//  判断image对象是否为空，易出现内存异常
	//	if (!image.IsNull()) {
	//		image.Destroy();
	//	}
	//	image.Load(path[i]);
	//	image.StretchBlt(pDc->m_hDC, imageRect, SRCCOPY);
	//	SetStretchBltMode(pDc->m_hDC, ModeOld);
	//	Sleep(1000);
	//}
	for (int i = 0; i < num; i++)
	{
		CString strPath = NULL;
		strPath = pathArray->GetAt(i);
		if (!image.IsNull()) {
			image.Destroy();
		}
		image.Load(strPath);
		image.StretchBlt(pDc->m_hDC, imageRect, SRCCOPY);
		SetStretchBltMode(pDc->m_hDC, ModeOld);
		Sleep(2000);
		if (i == num - 1) {
			return;
		}
	}


	ReleaseDC(pDc);
}
//  显示旋转90°图片
void CShowDlg::ShowRotate(CDC *pDC) {

	//读取原始图片信息
	CImage * srcImage = new CImage();
	srcImage->Load(path);
	BYTE* srcPtr = (BYTE*)srcImage->GetBits();
	int srcBitsCount = srcImage->GetBPP();
	int srcWidth = srcImage->GetWidth();
	int srcHeight = srcImage->GetHeight();
	int srcPitch = srcImage->GetPitch();
	//删除原始图片
	//DeleteFile(path);
	//创建新图像  
	CImage * destImage = new CImage();
	destImage->Create(srcHeight, srcWidth, srcBitsCount, 0);
	//加载调色板
	if (srcBitsCount <= 8 && srcImage->IsIndexed())//需要调色板  
	{
		RGBQUAD pal[256];
		int nColors = srcImage->GetMaxColorTableEntries();
		if (nColors>0)
		{
			srcImage->GetColorTable(0, nColors, pal);
			destImage->SetColorTable(0, nColors, pal);//复制调色板程序  
		}
	}
	//目标图像参数  
	BYTE *destPtr = (BYTE*)destImage->GetBits();
	int destPitch = destImage->GetPitch();
	//复制图像数据  
	for (int i = 0; i<srcHeight; i++)
	{
		for (int j = 0; j<srcWidth; j++)
		{
			if (srcBitsCount == 24) {
				*(destPtr + (srcWidth - 1 - j)*destPitch + 3 * i) = *(srcPtr + i*srcPitch + 3 * j);			//目的图片的第一列
				*(destPtr + (srcWidth - 1 - j)*destPitch + 3 * i + 1) = *(srcPtr + i*srcPitch + 3 * j + 1);
				*(destPtr + (srcWidth - 1 - j)*destPitch + 3 * i + 2) = *(srcPtr + i*srcPitch + 3 * j + 2);
			}
			else if (srcBitsCount == 8)
			{
				*(destPtr + (srcWidth - 1 - j)*destPitch + i) = *(srcPtr + i*srcPitch + j);//目的图片的第一列
			}
		}
	}
	//保存新图像
	destImage->Save(path);
	delete srcImage;
	delete destImage;
}
//  响应键盘消息 键盘切换图片
BOOL CShowDlg::PreTranslateMessage(MSG* pMsg) {
	//CStringArray *pathArr = pathArray;
	CString strPath;       //  图片路径
	int count = num;       //  图片数量
	int index = ci;        //  当前图片下标

	//判断是否为键盘消息
	if (pMsg->message == WM_KEYDOWN)
	{
		//判断是否按下键盘UP/LEFT键
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_LEFT)
		{
			if (ci == 0) {
				strPath = pathArray->GetAt(ci);
				ci = num - 1;
				//  判断image对象是否为空，易出现内存异常
				if (!image.IsNull()) {
					image.Destroy();
				}
				//  无特效显示不会出现刷新两次图片
				
				ShowPicture(strPath);
				return TRUE;
			}
			else if (ci > 0 && ci < num) {
				strPath = pathArray->GetAt(ci);
				ci = ci - 1;
				//  判断image对象是否为空，易出现内存异常
				if (!image.IsNull()) {
					image.Destroy();
				}
				//  无特效显示不会出现刷新两次图片
				ShowPicture(strPath);
				return TRUE;
			}
			else {
				return TRUE;
			}
		}
		//判断是否按下键盘DOWN/RIGHT键
		if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RIGHT)
		{
			if (ci == num-1) {
				strPath = pathArray->GetAt(ci);
				ci = 0;
				//  判断image对象是否为空，易出现内存异常
				if (!image.IsNull()) {
					image.Destroy();
				}
				//  无特效显示不会出现刷新两次图片

				ShowPicture(strPath);
				return TRUE;
			}
			else if (ci >= 0 && ci < num-1) {
				strPath = pathArray->GetAt(ci);
				ci = ci + 1;
				//  判断image对象是否为空，易出现内存异常
				if (!image.IsNull()) {
					image.Destroy();
				}
				//  无特效显示不会出现刷新两次图片
				ShowPicture(strPath);
				return TRUE;
			}
			else {
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

