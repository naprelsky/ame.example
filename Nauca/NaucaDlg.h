#pragma once
#include <chrono>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>

#include "ChartCtrl.h"
#include "ChartLineSerie.h"

#include "Resource.h"
#include "Coefficient.h"


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
	Coefficient* m_coeffA;
	Coefficient* m_coeffB;
	Coefficient* m_coeffC;

	// ������� �������� ������� ����������
	std::atomic<double> posX;
	std::atomic<double> posY;

	// ����� ��� �������� ����� �������
	std::vector<SinPoint> m_points;

	// ������ ������������� ������� � ������
	std::mutex m_ValueLock;

	// ����� ������ � ����
	std::ofstream outStream;

	// ���������� ��� ���������� ���������� �������
	bool m_plotThreadStarted;
	bool m_writeThreadStarted;

	// ����� ����� ��� ���������� �������
	CChartLineSerie* pLineSeries;

	std::string getEditText(int id);

	// ������� ������� ���������� � ������ � ����
	void PlotGraphThread(LPVOID pParam);
	void WriteGraphThread(LPVOID pParam);

public:
	afx_msg LRESULT OnUpdateCoeffLabels(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnAStart();
	afx_msg void OnBnClickedBtnAStop();
	afx_msg void OnEnChangeEditA();
	afx_msg void OnBnClickedBtnWriteFile();
	afx_msg void OnBnClickedBtnShow();
	afx_msg void OnBnClickedBtnBStart();
	afx_msg void OnBnClickedBtnCStart();
	afx_msg void OnBnClickedBtnBStop();
	afx_msg void OnBnClickedBtnCStop();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnEnChangeEditC();
	afx_msg void OnBnClickedOk();
};
