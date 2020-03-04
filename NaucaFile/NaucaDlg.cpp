#include "stdafx.h"
#include "Nauca.h"
#include "NaucaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNaucaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SHOW, &CNaucaDlg::OnBnClickedBtnShow)
	ON_BN_CLICKED(IDOK, &CNaucaDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CNaucaDlg::CNaucaDlg(CWnd* pParent): CDialogEx(IDD_NAUCA_DIALOG, pParent)
{
	pLineSeries = nullptr;
	m_plotThreadStarted = false;

	// Запуск потока построения графика
	std::thread plotThread(&CNaucaDlg::PlotGraphThread, this);
	plotThread.detach();
}

void CNaucaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
}

BOOL CNaucaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Создание оси абсцисс графика
	CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->EnableScrollBar(true);
	pBottomAxis->SetMinMax(0, 30);

	// Создание оси ординат графика
	CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0, 10);

	// Cоздание серии точек, на основе которых будет строиться график
	pLineSeries = m_ChartCtrl.CreateLineSerie();

	return TRUE;
}

void CNaucaDlg::OnBnClickedBtnShow()
{
	// Включение потока построения графика
	m_plotThreadStarted = true;
	GetDlgItem(IDC_BTN_SHOW)->EnableWindow(FALSE);
}

void CNaucaDlg::OnBnClickedOk()
{
	m_plotThreadStarted = false;
	CDialogEx::OnOK();
}

void CNaucaDlg::PlotGraphThread()
{
	std::streampos position = std::ios::beg;

	while (true) {
		if (m_plotThreadStarted) {
			// Открытие файла с данными графика
			std::ifstream in("data.dat");
			if (in.is_open()) {
				double posX;
				double posY;

				// Переход на последнюю прочитанную позицию файла
				in.seekg(position);

				// Считывание всех координат до конца файла
				// и добавление их в серию точек графика
				while (in >> posX >> posY) {
					pLineSeries->AddPoint(posX, posY);
					position = in.tellg();
				}
			}
			in.close();
		}

		// Пауза потока построения
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

