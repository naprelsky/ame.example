#pragma once
#include <chrono>
#include <fstream>
#include <thread>

#include "Resource.h"

#include "ChartCtrl.h"
#include "ChartLineSerie.h"

// Структура для хранения координат
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
	// Поток записи координат в файл
	std::ofstream outStream;

	// Переменная для управления состоянием потока построения
	bool m_plotThreadStarted;

	// Серия точек для построения графика
	CChartLineSerie* pLineSeries;

	// Функция потока построения графика
	void PlotGraphThread();

public:
	afx_msg void OnBnClickedBtnShow();
	afx_msg void OnBnClickedOk();
};
