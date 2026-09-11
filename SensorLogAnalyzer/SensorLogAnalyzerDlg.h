
// SensorLogAnalyzerDlg.h: 헤더 파일
//

#pragma once

#include "SensorGraphCtrl.h"
#include <vector>

struct SensorRecord {
	int rowNumber = 0;

	CString timestamp;
	CString distance;
	CString light;
	CString distanceStatus;
	CString lightStatus;
	CString errorMessage;

	double distanceValue = 0.0;
	long lightValue = 0;
	bool isValid = false;
};


// CSensorLogAnalyzerDlg 대화 상자
class CSensorLogAnalyzerDlg : public CDialogEx
{
// 생성입니다.
public:
	CSensorLogAnalyzerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORLOGANALYZER_DIALOG };
#endif

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
	afx_msg void OnBnClickedButtonOpenCsv();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonApplyThresholds();
	afx_msg void OnBnClickedButtonResetThresholds();
	CListCtrl m_sensorList;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	CTabCtrl m_dataFilterTab;

	CSensorGraphCtrl m_distanceGraph;
	CSensorGraphCtrl m_lightGraph;

private:
	std::vector<SensorRecord> m_sensorRecords;
	double m_dangerDistance = 30.0;
	double m_cautionDistance = 60.0;
	long m_darkAdc = 190;

	void RefreshSensorList();
	void RefreshGraphs();
	void RecalculateStatuses();
};
