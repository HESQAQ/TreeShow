#pragma once

#include "PictureEx.h"

// CShowDlg 对话框

class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void ShowGifPicture(CString path);
	void ShowPicture(CString path);
	void ShowMaxPicture(CString path);
	void Effect_Top2Bottom(CDC * pDC, int x, int y);
	void Effect_Bottom2Top(CDC * pDC, int x, int y);
	void Effect_Left2Right(CDC * pDC, int x, int y);
	void Effect_Right2Left(CDC * pDC, int x, int y);
	void Effect_FullView(CString path);
	void Show2Picture();
	void ShowRotate(CDC * pDC);
	BOOL PreTranslateMessage(MSG * pMsg);
public:
	CString path;//要显示图片的路径
	CStringArray* pathArray;
	int num;     // CListControl 中item数量
	int ci;      // 下标
	CString name; // item 名
	CImage image;
	CPictureEx m_GifPic;
	BOOL flag;
	CToolBar  m_ToolBar;
};
