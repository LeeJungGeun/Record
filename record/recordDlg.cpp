
// recordDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "record.h"
#include "recordDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>

using namespace std;
#pragma comment(lib, "winmm")

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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CrecordDlg 대화 상자



CrecordDlg::CrecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RECORD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_state = 0;  // 중지 상태
	m_rec_bytes = 0;
	m_play_bytes = 0;
}

void CrecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_status);
}

BEGIN_MESSAGE_MAP(CrecordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CrecordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CrecordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CrecordDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CrecordDlg 메시지 처리기

BOOL CrecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	mp_rec_buffer = new char[m_rec_sound.GetQueueSize()*50];

	m_play_sound.OpenDevice(m_hWnd);
	m_rec_sound.OpenDevice(m_hWnd);

	return TRUE;
}

void CrecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CrecordDlg::OnPaint()
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
HCURSOR CrecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CrecordDlg::OnBnClickedOk()
{
	if (0 == m_state) {  // 중지 상태에서는 녹음 상태로 변경
		SetDlgItemText(IDOK, L"녹음 중...");

		m_rec_bytes = 0;
		m_rec_sound.StartDevice();
		m_state = 1;
	}
	else {  // 녹음 상태에서는 재생 상태로 변경
		m_rec_sound.StopDevice();
		SetDlgItemText(IDOK, L"녹음 시작");
		m_state = 0;
	}
		
}


void CrecordDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	exit(0);
	
}

void CrecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_rec_sound.CloseDevice();
	m_play_sound.CloseDevice();

	delete[] mp_rec_buffer;

}

LRESULT CrecordDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// MM_WIM_DATA는 사운드가 녹음 버퍼에 가득찬 경우 발생하는 메세지이다.
	if (message == MM_WIM_DATA) {
		if (m_rec_bytes < m_rec_sound.GetQueueSize()*50) {
			// lParam 인자에 WAVEHDR 구조체 형태로 소리정보가 들어있다.
			WAVEHDR *p_wave_header = (WAVEHDR *)lParam;
			memcpy(mp_rec_buffer + m_rec_bytes, p_wave_header->lpData, p_wave_header->dwBytesRecorded);
			m_rec_bytes += p_wave_header->dwBytesRecorded;
			m_rec_sound.ReuseQueue(p_wave_header);

			if (m_state == 1) {
				CString str;
				str.Format(L"녹음 중 : %d bytes", m_rec_bytes);
				SetDlgItemText(IDC_EDIT1, str);
			}
		}
		else {
			SetDlgItemText(IDC_EDIT1, L"최대 녹음 시간 초과!!");
		}
	}
	else if (message == MM_WOM_DONE) { // MM_WOM_DONE는 출력 버퍼에서 사운드가 모두 출력된 경우 발생하는 메세지이다.
		WAVEHDR *p_wave_header = (WAVEHDR *)lParam;
		// 재생 큐를 세개로 사용하다보니 출력 요청된 크기와 실제 재생된 크기가 다르다.
		// 그래서 큐가 재생완료될 때마다 실제 재생 크기를 계산한다.
		m_real_play_bytes += p_wave_header->dwBufferLength;

		if (m_play_bytes < m_rec_bytes) {
			int current_play_size;
			if (m_play_sound.GetQueueSize() < (m_rec_bytes - m_play_bytes)) current_play_size = m_play_sound.GetQueueSize();
			else current_play_size = m_rec_bytes - m_play_bytes;

			m_play_sound.SetSoundData(mp_rec_buffer + m_play_bytes, current_play_size);
			m_play_bytes += current_play_size;
		}

		if (m_real_play_bytes < m_rec_bytes) {
			CString str;
			str.Format(L"재생 중 : %d bytes", m_real_play_bytes);
			SetDlgItemText(IDC_EDIT1, str);
		}
		else {
			// 재생이 완료 됨
			PostMessage(WM_COMMAND, IDC_EDIT1);  // 버튼을 누른것처럼 동작시킴
		}
		return 1;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CrecordDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (0 == m_state) {  // 중지 상태에서는 녹음 상태로 변경
		m_state = 1;
		m_real_play_bytes = 0;
		m_play_bytes = 0;
		m_play_sound.StartDevice();

		int queue_size = m_play_sound.GetQueueSize(), current_play_size, i;
		for (i = 0; i < MAX_QUEUE_NUMBER && m_play_bytes < m_rec_bytes; i++) {
			if (queue_size < (m_rec_bytes - m_play_bytes)) current_play_size = queue_size;
			else current_play_size = m_rec_bytes - m_play_bytes;

			m_play_sound.SetSoundData(mp_rec_buffer + m_play_bytes, current_play_size);
			m_play_bytes += current_play_size;
		}

		SetDlgItemText(IDC_BUTTON1, L"재생 중...");
		m_state = 2;
	}
	else {  // 녹음 상태에서는 재생 상태로 변경
		m_play_sound.StopDevice();
		m_state = 0;
		SetDlgItemText(IDC_BUTTON1, L"재생 시작");
	}
}
