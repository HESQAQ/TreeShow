
// TreeShowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTreeShowDlg �Ի���
class CTreeShowDlg : public CDialogEx
{
// ����
public:
	CTreeShowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TREESHOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMenu m_Menu;
	CToolBar  m_ToolBar;
	CListCtrl m_list;        //�б�ؼ� IDC_LIST
	CTreeCtrl m_tree;        //���οؼ� IDC_TREE
	CImageList m_ImageList;  //ͼ���б�(��ͬ��Сͼ�񼯺�) 
	HTREEITEM m_hRoot;       //��� CTreeCtrl�ĸ����"�ҵĵ���"
protected:
	
public:
	void GetLogicalDrives(HTREEITEM hParent);  //��ȡ����
	void GetDriveDir(HTREEITEM hParent);       //��ȡ����
	afx_msg void OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult);
	void AddSubDir(HTREEITEM hParent);         //�����Ŀ¼
	CString GetFullPath(HTREEITEM hCurrent);   //��ȡ����Ŀȫ��·��
	afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_PicShow;
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
