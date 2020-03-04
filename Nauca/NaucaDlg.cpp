#include "stdafx.h"
#include "Nauca.h"
#include "NaucaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE_COEFF_LABEL (WM_USER + 100)

BEGIN_MESSAGE_MAP(CNaucaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATE_COEFF_LABEL, &CNaucaDlg::OnUpdateCoeffLabels)
	ON_BN_CLICKED(IDC_BTN_A_START, &CNaucaDlg::OnBnClickedBtnAStart)
	ON_BN_CLICKED(IDC_BTN_A_STOP, &CNaucaDlg::OnBnClickedBtnAStop)
	ON_EN_CHANGE(IDC_EDIT_A, &CNaucaDlg::OnEnChangeEditA)
	ON_BN_CLICKED(IDC_BTN_WRITE_FILE, &CNaucaDlg::OnBnClickedBtnWriteFile)
	ON_BN_CLICKED(IDC_BTN_SHOW, &CNaucaDlg::OnBnClickedBtnShow)
	ON_BN_CLICKED(IDC_BTN_B_START, &CNaucaDlg::OnBnClickedBtnBStart)
	ON_BN_CLICKED(IDC_BTN_C_START, &CNaucaDlg::OnBnClickedBtnCStart)
	ON_BN_CLICKED(IDC_BTN_B_STOP, &CNaucaDlg::OnBnClickedBtnBStop)
	ON_BN_CLICKED(IDC_BTN_C_STOP, &CNaucaDlg::OnBnClickedBtnCStop)
	ON_EN_CHANGE(IDC_EDIT_B, &CNaucaDlg::OnEnChangeEditB)
	ON_EN_CHANGE(IDC_EDIT_C, &CNaucaDlg::OnEnChangeEditC)
	ON_BN_CLICKED(IDOK, &CNaucaDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CNaucaDlg::CNaucaDlg(CWnd* pParent): CDialogEx(IDD_NAUCA_DIALOG, pParent)
{
	// Создание экземпляров коэффициентов A, B и C с указанными пределами изменения
	m_coeffA = new Coefficient(3, 7);
	m_coeffB = new Coefficient(1, 3);
	m_coeffC = new Coefficient(1, 10);

	posX = 0;
	posY = 0;

	m_plotThreadStarted = false;
	m_writeThreadStarted = false;

	// Создание и запуск отсоединенного потока построения графика
	std::thread plotThread(&CNaucaDlg::PlotGraphThread, this, this);
	plotThread.detach();

	// Создание и запуск отсоединенного потока записи значений в файл
	std::thread writeThread(&CNaucaDlg::WriteGraphThread, this, this);
	writeThread.detach();
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
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->EnableScrollBar(true);
	pLeftAxis->SetMinMax(0, 10);

	// Создание серии точек для построения графика
	pLineSeries = m_ChartCtrl.CreateLineSerie();

	return TRUE;
}

void CNaucaDlg::OnBnClickedBtnAStart()
{
	GetDlgItem(IDC_BTN_A_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_A_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_A)->EnableWindow(FALSE);

	m_coeffA->StartChanging();
}

void CNaucaDlg::OnBnClickedBtnBStart()
{
	GetDlgItem(IDC_BTN_B_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_B_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_B)->EnableWindow(FALSE);

	m_coeffB->StartChanging();
}

void CNaucaDlg::OnBnClickedBtnCStart()
{
	GetDlgItem(IDC_BTN_C_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_C_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_C)->EnableWindow(FALSE);

	m_coeffC->StartChanging();
}


void CNaucaDlg::OnBnClickedBtnAStop()
{
	GetDlgItem(IDC_BTN_A_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_A_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_A)->EnableWindow(TRUE);

	m_coeffA->StopChanging();
}

void CNaucaDlg::OnBnClickedBtnBStop()
{
	GetDlgItem(IDC_BTN_B_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_B_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_B)->EnableWindow(TRUE);

	m_coeffB->StopChanging();
}

void CNaucaDlg::OnBnClickedBtnCStop()
{
	GetDlgItem(IDC_BTN_C_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_C_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_C)->EnableWindow(TRUE);

	m_coeffC->StopChanging();
}

void CNaucaDlg::OnEnChangeEditA()
{
	std::string manualValue = getEditText(IDC_EDIT_A);
	m_coeffA->SetValue(std::atof(manualValue.c_str()));
}

void CNaucaDlg::OnEnChangeEditB()
{
	std::string manualValue = getEditText(IDC_EDIT_B);
	m_coeffB->SetValue(std::atof(manualValue.c_str()));
}

void CNaucaDlg::OnEnChangeEditC()
{
	std::string manualValue = getEditText(IDC_EDIT_C);
	m_coeffB->SetValue(std::atof(manualValue.c_str()));
}

void CNaucaDlg::OnBnClickedBtnWriteFile()
{
	m_writeThreadStarted = true;
	GetDlgItem(IDC_BTN_WRITE_FILE)->EnableWindow(FALSE);
}

void CNaucaDlg::OnBnClickedBtnShow()
{
	m_plotThreadStarted = true;
	GetDlgItem(IDC_BTN_SHOW)->EnableWindow(FALSE);
}

void CNaucaDlg::OnBnClickedOk()
{
	m_plotThreadStarted = false;
	m_writeThreadStarted = false;

	m_coeffA->StopChanging();
	m_coeffB->StopChanging();
	m_coeffC->StopChanging();

	CDialogEx::OnOK();
}


LRESULT CNaucaDlg::OnUpdateCoeffLabels(WPARAM wParam, LPARAM lParam)
{
	double* coeffs = (double*)lParam;

	CString strBuffer;

	strBuffer.Format(L"%f", coeffs[0]);
	((CStatic*)GetDlgItem(IDC_LBL_VAL_A))->SetWindowTextW(strBuffer);

	strBuffer.Format(L"%f", coeffs[1]);
	((CStatic*)GetDlgItem(IDC_LBL_VAL_B))->SetWindowTextW(strBuffer);

	strBuffer.Format(L"%f", coeffs[2]);
	((CStatic*)GetDlgItem(IDC_LBL_VAL_C))->SetWindowTextW(strBuffer);
	return 0;
}

/**
 * Получение текста поля ввода.
 * @param id - идентификатор элемента формы.
 * @return текст, введенный в поле ввода.
 */
std::string CNaucaDlg::getEditText(int id)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(id);

	CString value;
	int len = pEdit->LineLength(0);
	pEdit->GetLine(0, value.GetBufferSetLength(len), len);
	value.ReleaseBuffer();

	std::string manualValue = CStringA(value);
	return manualValue;
}

/**
 * Поток построения графика.
 */
void CNaucaDlg::PlotGraphThread(LPVOID pParam)
{
	CNaucaDlg* pDlg = (CNaucaDlg*)pParam;

	while (true) {
		if (m_plotThreadStarted) {
			// Получение текущих значений коэффициентов A, B и C
			double valA = m_coeffA->GetValue();
			double valB = m_coeffB->GetValue();
			double valC = m_coeffC->GetValue();

			// Обновление значений коэффициентов на форме
			double coeffs[3] = { valA, valB, valC };
			::PostMessage(pDlg->m_hWnd, WM_UPDATE_COEFF_LABEL, 1, (LPARAM)&coeffs);

			// Расчет текущих координат
			posX = posX + 1;
			posY = valA + valB * sin(valC * posX);

			pLineSeries->AddPoint(posX, posY);

			// Сохранение текущих координат в буфер.
			// Сохраняются в случае, если работает поток записи в файл.
			if (m_writeThreadStarted) {
				m_ValueLock.lock();
				m_points.push_back({ posX, posY });
				m_ValueLock.unlock();
			}
		}

		// Пауза потока построения
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/**
 * Поток записи значений в файл.
 */
void CNaucaDlg::WriteGraphThread(LPVOID pParam)
{
	while (true) {
		if (m_writeThreadStarted) {
			outStream.open("data.dat", std::ios::app);

			// Получение значения из буфера и запись его в файл
			m_ValueLock.lock();
			while (!m_points.empty())
			{
				SinPoint point = m_points.back();
				m_points.pop_back();

				outStream << point.X << " " << point.Y << std::endl;
			}
			m_ValueLock.unlock();

			outStream.close();
		} 

		// Пауза потока записи в файл
		std::this_thread::sleep_for(std::chrono::microseconds(500));
	}
}
