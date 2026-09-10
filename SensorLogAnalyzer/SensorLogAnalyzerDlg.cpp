
// SensorLogAnalyzerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SensorLogAnalyzer.h"
#include "SensorLogAnalyzerDlg.h"
#include "afxdialogex.h"
#include <cmath>
#include <cerrno>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {

	// 실수(float) 검사, 거리(distance)가 숫자고 허용 범위 안인지
	bool TryParseDistance(const CString& text, double& value) {
		const TCHAR* start = text.GetString();
		TCHAR* end = nullptr;

		errno = 0;
		value = _tcstod(start, &end);

		if (end == start ||
			*end != _T('\0')||
			errno == ERANGE ||
			!std::isfinite(value)
		)
		{
			return false;
		}

		return true;
	}

	// 정수 검사
	bool TryParseAdc(const CString& text, long& value) {
		const TCHAR* start = text.GetString();
		TCHAR* end = nullptr;

		errno = 0;
		value = _tcstol(start, &end, 10);

		if (end == start ||
			*end != _T('\0') ||
			errno == ERANGE
		) 
		{
			return false;
		}
		return true;
	}
}

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

	CString filePath = fileDialog.GetPathName();

	SetDlgItemText(
		IDC_EDIT_FILE_PATH,
		fileDialog.GetPathName()
	);

	// CSV 파일 열기
	CStdioFile file;

	if (!file.Open(
		filePath,
		CFile::modeRead |
		CFile::typeText |
		CFile::shareDenyNone
	)) {
		AfxMessageBox(_T("CSV 파일을 열 수 없습니다"));
		return;
	}

	// 첫 번째 줄 읽기
	CString header;

	if (!file.ReadString(header)) {
		AfxMessageBox(_T("CSV 파일이 비어 있습니다."));
		return;
	}
	AfxMessageBox(_T("CSV 파일이 열렸습니다."));

	
	header.Trim();

	if (header != _T("timestamp,distance_cm,light_adc")) {
		file.Close();
		AfxMessageBox(_T("CSV 헤더 형식이 올바르지 않습니다."));
		return;
	}
	
	// 이전 목록 데이터 제거
	m_sensorList.DeleteAllItems();

	CString line;
	int rowNumber = 0;

	while (file.ReadString(line)) {
		line.Trim();

		// 빈 줄 건너뜀
		if (line.IsEmpty()) {
			continue;
		}
		rowNumber++;

		CString timestamp;
		CString distance;
		CString light;

		// 쉼표를 기준으로 앞의 3개 값 분리
		AfxExtractSubString(timestamp, line, 0, _T(','));
		AfxExtractSubString(distance, line, 1, _T(','));
		AfxExtractSubString(light, line, 2, _T(','));

		timestamp.Trim();
		distance.Trim();
		light.Trim();

		CString errorMessage;
		double distanceValue = 0.0;
		long lightValue = 0;

		// 쉼표 개수 확인
		int commaCount = 0;

		for (int i = 0; i < line.GetLength(); i++) {
			if (line[i]==_T(',')) {
				commaCount++;
			}
		}

		// 열은 정확하게 3개여야 하므로 쉼표는 2개
		if (commaCount !=2) {
			errorMessage = _T("열 개수 오류");
		}
		else if (timestamp.IsEmpty()) {
			errorMessage = _T("시간 누락");
		}
		else if (distance.IsEmpty()) {
			errorMessage = _T("거리 누락");
		}
		else if (light.IsEmpty()) {
			errorMessage = _T("조도 누락");
		}
		else if (!TryParseDistance(distance, distanceValue)) {
			errorMessage = _T("거리 숫자 형식 오류");
		}
		else if (distanceValue <= 0.0 || distanceValue > 400.0) {
			errorMessage = _T("거리 범위 오류");
		}
		else if (!TryParseAdc(light,lightValue)) {
			errorMessage = _T("조도 숫자 형식 오류");
		}
		else if (lightValue < 0 || lightValue > 255) {
			errorMessage = _T("조도 범위 오류");
		}

		CString rowText;
		rowText.Format(_T("%d"), rowNumber);

		int listIndex = m_sensorList.InsertItem(
			m_sensorList.GetItemCount(),
			rowText		
		);

		m_sensorList.SetItemText(listIndex, 1, timestamp);
		m_sensorList.SetItemText(listIndex, 2, distance);
		m_sensorList.SetItemText(listIndex, 3, light);
		m_sensorList.SetItemText(listIndex, 6, errorMessage);
	}

	file.Close();

	CString message;
	message.Format(_T("%d개의 데이터 행을 읽었습니다."), rowNumber);
	AfxMessageBox(message);



}
