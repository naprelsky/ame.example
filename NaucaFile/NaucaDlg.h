#pragma once
#include <chrono>
#include <fstream>
#include <thread>

#include "Resource.h"

#include "ChartCtrl.h"
#include "ChartLineSerie.h"

// ��������� ��� �������� ���������
struct SinPoint
{
	double X;
	double Y;
};


class CNaucaDlg : public CDialogEx
{
public:
	CNaucaDlg(CWnd* pParent = nullptr);

protected:
	CChartCtrl m_ChartCtrl;
	BOOL OnInitDialog();
	void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	// ����� ������ ��������� � ����
	std::ofstream outStream;

	// ���������� ��� ���������� ���������� ������ ����������
	bool m_plotThreadStarted;

	// ����� ����� ��� ���������� �������
	CChartLineSerie* pLineSeries;

	// ������� ������ ���������� �������
	void PlotGraphThread();

public:
	afx_msg void OnBnClickedBtnShow();
	afx_msg void OnBnClickedOk();
};
