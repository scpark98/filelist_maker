
// file_list_makerDlg.h: 헤더 파일
//

#pragma once

#include <deque>

#include "../Common/zip/zip/zip.h"
#include "../Common/CStatic/SCStatic/SCStatic.h"

// CfilelistmakerDlg 대화 상자
class CfilelistmakerDlg : public CDialogEx
{
// 생성입니다.
public:
	CfilelistmakerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void		thread_make_list();

	//target_folder의 모든 파일/폴더를 zip_path 압축파일로 만든다.
	ZRESULT		zip_folder(CString target_folder, CString zip_path);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_LIST_MAKER_DIALOG };
#endif

protected:
	std::deque<CString> m_files;
	std::deque<CString> m_dropList;
	CString m_droppedFolder;

	//일부 변경된 파일들만 drag&drop하여 추가할 경우
	//drop된 파일들이 모두 zip파일이 아닌 경우에만 진행.
	bool	check_dropped_files_not_zip();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSCStatic m_static_status;
	CButton m_check_filesize;
	CButton m_check_fileversion;
	afx_msg void OnBnClickedCheckFilesize();
	afx_msg void OnBnClickedCheckFileversion();
	CProgressCtrl m_progress;
};
