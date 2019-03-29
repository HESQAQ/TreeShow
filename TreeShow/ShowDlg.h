#pragma once

#include "PictureEx.h"

// CShowDlg �Ի���

class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CString path;//Ҫ��ʾͼƬ��·��
	CStringArray* pathArray;
	int num;     // CListControl ��item����
	int ci;      // �±�
	CString name; // item ��
	CImage image;
	CPictureEx m_GifPic;
	BOOL flag;
	CToolBar  m_ToolBar;
};
