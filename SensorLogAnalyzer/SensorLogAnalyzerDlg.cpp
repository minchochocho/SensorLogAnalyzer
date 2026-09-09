
// SensorLogAnalyzerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SensorLogAnalyzer.h"
#include "SensorLogAnalyzerDlg.h"
#include "afxdialogex.h"

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


// CSensorLogAnalyzerDlg 대화 상자



CSensorLogAnalyzerDlg::CSensorLogAnalyzerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENSORLOGANALYZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSensorLogAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SENSOR_DATA, m_sensorList);
}

BEGIN_MESSAGE_MAP(CSensorLogAnalyzerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CSV, &CSensorLogAnalyzerDlg::OnBnClickedButtonOpenCsv)
END_MESSAGE_MAP()


// CSensorLogAnalyzerDlg 메시지 처리기

BOOL CSensorLogAnalyzerDlg::OnInitDialog()
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
	// 행 전체 선택과 격자선 표시
	m_sensorList.SetExtendedStyle(
		m_sensorList.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_DOUBLEBUFFER
	);

	// 데이터 열 구성
	m_sensorList.InsertColumn(0, _T("행"), LVCFMT_RIGHT, 60);
	m_sensorList.InsertColumn(1, _T("시간"), LVCFMT_LEFT, 170);
	m_sensorList.InsertColumn(2, _T("거리(cm)"), LVCFMT_RIGHT, 110);
	m_sensorList.InsertColumn(3, _T("조도(ADC)"), LVCFMT_RIGHT, 110);
	m_sensorList.InsertColumn(4, _T("거리 상태"), LVCFMT_LEFT, 110);
	m_sensorList.InsertColumn(5, _T("밝기 상태"), LVCFMT_LEFT, 110);
	m_sensorList.InsertColumn(6, _T("오류 내용"), LVCFMT_LEFT, 180);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSensorLogAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSensorLogAnalyzerDlg::OnPaint()
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
HCURSOR CSensorLogAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CSV 열기 버튼
void CSensorLogAnalyzerDlg::OnBnClickedButtonOpenCsv() {
	CFileDialog fileDialog(
		TRUE,
		_T("csv"),
		nullptr,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		_T("CSV 파일 (*.csv)|*.csv|모든 파일 (*.*)|*.*||"),
		this
	);

	if (fileDialog.DoModal() != IDOK) {
		return;
	}

	SetDlgItemText(
		IDC_EDIT_FILE_PATH,
		fileDialog.GetPathName()
	);
}
