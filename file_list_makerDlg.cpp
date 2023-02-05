
// file_list_makerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "file_list_maker.h"
#include "file_list_makerDlg.h"
#include "afxdialogex.h"

#include "../Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfilelistmakerDlg 대화 상자



CfilelistmakerDlg::CfilelistmakerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_LIST_MAKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfilelistmakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfilelistmakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CfilelistmakerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CfilelistmakerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CfilelistmakerDlg 메시지 처리기

BOOL CfilelistmakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	DragAcceptFiles();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CfilelistmakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CfilelistmakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CfilelistmakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CfilelistmakerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];
	UINT drop_num = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	m_dropList.clear();

	for (int i = 0; i < drop_num; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);
		m_dropList.push_back(sfile);
	}

	if (m_dropList.size() == 0)
		return;

	if (PathIsDirectory(m_dropList[0]))
		m_droppedFolder = GetParentDirectory(m_dropList[0]);
	else
		m_droppedFolder = GetFolderNameFromFullPath(m_dropList[0]);

	make_list();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CfilelistmakerDlg::make_list()
{
	int i;
	long t0 = clock();

	m_files.clear();

	for (i = 0; i < m_dropList.size(); i++)
	{
		std::deque<CString> files;

		if (IsFolder(m_dropList[i]))
		{
			//약 2,000개 파일 검색 소요시간
			//FindAllFiles	: 2,902ms
			//find_all_files: 163ms (94%정도 시간단축)
			if (false)
			{
				//FindAllFiles(m_dropList[i], &m_files, _T(""), _T(""), true);
			}
			else
			{
				files = find_all_files(m_dropList[i]);
				m_files.insert(m_files.end(), files.begin(), files.end());
			}
		}
		else
		{
			m_files.push_back(m_dropList[i]);
		}
	}

	int pos;

	while ((pos = find_dqstring(m_files, _T("filelist.lst"))) >= 0)
	{
		m_files.erase(m_files.begin() + pos);
	}

	FILE* fp = NULL;

	//_tfopen_s(&fp, m_droppedFolder + _T("\\filelist.lst"), _T("wt"));
	fp = fopen(m_droppedFolder + _T("\\filelist.lst"), _T("wt"));
	if (fp == NULL)
	{
		AfxMessageBox(_T("filelist.lst 파일 생성 실패"));
		return;
	}

	for (i = 0; i < m_files.size(); i++)
	{
		m_files[i].Replace(m_droppedFolder + _T("\\"), _T(""));
		//m_files[i].Replace(_T("\\"), _T("/"));
		//_ftprintf(fp, _T("%s\n"), m_files[i]);
		fprintf(fp, _T("%s\n"), m_files[i]);
	}

	fclose(fp);

	CString str;
	str.Format(_T("Filelist_maker (%ld ms)"), clock() - t0);
	SetWindowText(str);

	MessageBeep(0);
}


void CfilelistmakerDlg::OnBnClickedOk()
{
}


void CfilelistmakerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
