
// file_list_makerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "file_list_maker.h"
#include "file_list_makerDlg.h"
#include "afxdialogex.h"

#include <thread>

#include "../Common/Functions.h"
#include "../Common/zip/zip/zip.h"

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
	DDX_Control(pDX, IDC_STATIC_STATUS, m_static_status);
	DDX_Control(pDX, IDC_CHECK_FILESIZE, m_check_filesize);
	DDX_Control(pDX, IDC_CHECK_FILEVERSION, m_check_fileversion);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}

BEGIN_MESSAGE_MAP(CfilelistmakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CfilelistmakerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CfilelistmakerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_FILESIZE, &CfilelistmakerDlg::OnBnClickedCheckFilesize)
	ON_BN_CLICKED(IDC_CHECK_FILEVERSION, &CfilelistmakerDlg::OnBnClickedCheckFileversion)
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

	m_check_filesize.SetCheck(theApp.GetProfileInt(_T("setting"), _T("check filesize"), BST_CHECKED));
	m_check_fileversion.SetCheck(theApp.GetProfileInt(_T("setting"), _T("check fileversion"), BST_CHECKED));

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
	m_static_status.set_text(_T("파일, 폴더 정보를 분석중입니다."), GRAY128);

	TCHAR sfile[MAX_PATH];
	UINT drop_num = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	m_dropList.clear();

	//하나의 폴더가 드롭되면 해당 폴더내의 모든 폴더+파일들을 리스팅하는 것이므로
	//m_droppedFolder는 해당 폴더까지의 fullpath가 된다.
	//두개 이상의 파일 또는 폴더가 드롭되면 해당 폴더들이 존재하는 폴더가 m_droppedFolder가 된다.
	for (int i = 0; i < drop_num; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);
		m_dropList.push_back(sfile);
	}

	if (m_dropList.size() == 0)
		return;

	if (drop_num == 1)
	{
		m_droppedFolder = m_dropList[0];
	}
	else
	{
		m_droppedFolder = GetParentDirectory(m_dropList[0]);
	}

	if (!IsFolder(m_droppedFolder))
		m_droppedFolder = get_part(m_droppedFolder, fn_folder);

	std::thread th(&CfilelistmakerDlg::thread_make_list, this);
	th.detach();

	CDialogEx::OnDropFiles(hDropInfo);
}

//일부 변경된 파일들만 drag&drop하여 추가할 경우
//drop된 파일들이 모두 zip파일이 아닌 경우에만 진행.
bool CfilelistmakerDlg::check_dropped_files_not_zip()
{
	bool res = true;

	for (auto file : m_files)
	{
		if (file.Right(4).MakeLower() == _T(".zip"))
			return false;
	}

	return true;
}

void CfilelistmakerDlg::thread_make_list()
{
	m_static_status.set_text(_T("파일목록을 생성중입니다."));

	int i;
	long t0 = clock();
	CString str;
	FILE* fp = NULL;

	m_files.clear();

	m_progress.SetRange(0, m_dropList.size());

	for (i = 0; i < m_dropList.size(); i++)
	{
		m_progress.SetPos(i + 1);

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

	m_static_status.set_text(_T("filelist.lst 파일로 저장중입니다."));

	int pos;

	//확장자를 .lst로 한 이유는 NH UCTogether 당시 파일보안정책으로
	//서버에서 .txt파일을 열면 바로 암호화되어 파일 내용이 변경되는 문제가 발생하여
	//보안검열 대상이 아닌 lst 확장자로 변경함.

	//파일목록에 filelist.lst 자기자신이 있다면 삭제한다.
	while ((pos = find_dqstring(m_files, _T("filelist.lst"))) >= 0)
	{
		m_files.erase(m_files.begin() + pos);
	}

	//만약 기존 생성해놓은 flielist.lst가 존재하고
	//몇 개의 파일만 변경된 경우라면 다음과 같은 과정을 거쳐야 한다.
	//1.모든 파일들을 압축을 푼 후 압축파일들을 지운다.
	//2.해당 폴더를 다시 drag&drop하여 리스트 파일을 생성한다.
	//단 2단계라고 해도 sub folder들이 많을 경우 등
	//매우 번거로울 수 있으므로 몇 개 파일만 새로 갱신하는 기능 추가.
	//기존 filelist.lst 내용을 저장해놓고 새로 갱신할 파일들을 갱신한 후
	//기존 파일정보들을 filelist.lst에 추가 기록한다.
	//새로 추가된 파일들은 목록 맨 위에 오게된다.
	std::deque<CString> dq_already_exist_file_list;

	if (get_file_size(m_droppedFolder + _T("\\filelist.lst")) > 0 &&
		check_dropped_files_not_zip())
	{
		read_file(m_droppedFolder + _T("\\filelist.lst"), &dq_already_exist_file_list, true);
	}


	//m_files에 저장된 파일목록을 목록파일로 생성.
	uint64_t filesize;
	CString version;

	_tfopen_s(&fp, m_droppedFolder + _T("\\filelist.lst"), _T("wt")CHARSET);

	if (fp == NULL)
		return;

	//UTF-8로 저장하되 자동으로 붙는 BOM(3 char)를 날려준다.
	//그렇지 않으면 InternetReadFile()로 읽어올 때 BOM문자까지 읽어지므로 문제가 된다.
	fseek(fp, 0L, SEEK_SET);

	//fp = fopen(m_droppedFolder + _T("\\filelist.lst"), _T("wt"));
	if (fp == NULL)
	{
		m_static_status.set_text(_T("filelist.lst 파일 생성 실패."), red);
		return;
	}

	bool include_filesize = (m_check_filesize.GetCheck() == BST_CHECKED);
	bool include_fileversion = (m_check_fileversion.GetCheck() == BST_CHECKED);

	m_progress.SetRange(0, m_files.size());

	for (i = 0; i < m_files.size(); i++)
	{
		m_progress.SetPos(i + 1);

		//파일을 낱개로 추가하는 경우는 dq_already_exist_file_list에서는 제거시켜줘야 중복되지 않는다.
		CString fname = m_files[i];
		fname.Replace(m_droppedFolder + _T("\\"), _T(""));
		int index = find_dqstring(dq_already_exist_file_list, fname);
		if (index >= 0)
			dq_already_exist_file_list.erase(dq_already_exist_file_list.begin() + index);

		filesize = get_file_size(m_files[i]);
		version = get_file_property(m_files[i], _T("FileVersion"));

		//test.exe -> test.exe.zip으로 압축하고 원본 파일은 삭제한다.
		HZIP hz = CreateZip(m_files[i] + _T(".zip"), 0);
		ZRESULT zr;

		if (hz == NULL)
		{
			m_static_status.set_textf(red, _T("CreateZip() failed : %s"), m_files[i]);
			return;
		}

		zr = ZipAdd(hz, get_part(m_files[i], fn_name), m_files[i]);
		if (zr != ZR_OK)
		{
			m_static_status.set_textf(red, _T("ZipAdd() failed : %s"), m_files[i]);
			return;
		}

		zr = CloseZip(hz);
		DeleteFile(m_files[i]);



		//상대경로로 바꿔주고
		m_files[i].Replace(m_droppedFolder + _T("\\"), _T(""));
		

		//파일명 기록
		_ftprintf(fp, _T("%s"), m_files[i]);


		//파일크기 기록
		if (include_filesize)
		{
			_ftprintf(fp, _T("|%I64u"), filesize);
		}


		//파일버전 기록
		if (include_fileversion)
		{
			if (version == _T("(null)"))
				version = _T("");
			_ftprintf(fp, _T("|%s"), version);
		}

		_ftprintf(fp, _T("\n"));
	}

	//dq_other_files_List의 내용이 존재한다면 파일에 이어서 write해준다.
	for (auto list : dq_already_exist_file_list)
	{
		_ftprintf(fp, _T("%s\n"), list);
	}

	fclose(fp);

	m_static_status.set_text(_T("filelist.lst 파일 생성 완료."), blue);

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


void CfilelistmakerDlg::OnBnClickedCheckFilesize()
{
	theApp.WriteProfileInt(_T("setting"), _T("check filesize"), m_check_filesize.GetCheck());
}


void CfilelistmakerDlg::OnBnClickedCheckFileversion()
{
	theApp.WriteProfileInt(_T("setting"), _T("check fileversion"), m_check_fileversion.GetCheck());
}
