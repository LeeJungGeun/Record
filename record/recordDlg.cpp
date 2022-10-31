
// recordDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CrecordDlg ��ȭ ����



CrecordDlg::CrecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RECORD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_state = 0;  // ���� ����
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


// CrecordDlg �޽��� ó����

BOOL CrecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CrecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CrecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CrecordDlg::OnBnClickedOk()
{
	if (0 == m_state) {  // ���� ���¿����� ���� ���·� ����
		SetDlgItemText(IDOK, L"���� ��...");

		m_rec_bytes = 0;
		m_rec_sound.StartDevice();
		m_state = 1;
	}
	else {  // ���� ���¿����� ��� ���·� ����
		m_rec_sound.StopDevice();
		SetDlgItemText(IDOK, L"���� ����");
		m_state = 0;
	}
		
}


void CrecordDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// MM_WIM_DATA�� ���尡 ���� ���ۿ� ������ ��� �߻��ϴ� �޼����̴�.
	if (message == MM_WIM_DATA) {
		if (m_rec_bytes < m_rec_sound.GetQueueSize()*50) {
			// lParam ���ڿ� WAVEHDR ����ü ���·� �Ҹ������� ����ִ�.
			WAVEHDR *p_wave_header = (WAVEHDR *)lParam;
			memcpy(mp_rec_buffer + m_rec_bytes, p_wave_header->lpData, p_wave_header->dwBytesRecorded);
			m_rec_bytes += p_wave_header->dwBytesRecorded;
			m_rec_sound.ReuseQueue(p_wave_header);

			if (m_state == 1) {
				CString str;
				str.Format(L"���� �� : %d bytes", m_rec_bytes);
				SetDlgItemText(IDC_EDIT1, str);
			}
		}
		else {
			SetDlgItemText(IDC_EDIT1, L"�ִ� ���� �ð� �ʰ�!!");
		}
	}
	else if (message == MM_WOM_DONE) { // MM_WOM_DONE�� ��� ���ۿ��� ���尡 ��� ��µ� ��� �߻��ϴ� �޼����̴�.
		WAVEHDR *p_wave_header = (WAVEHDR *)lParam;
		// ��� ť�� ������ ����ϴٺ��� ��� ��û�� ũ��� ���� ����� ũ�Ⱑ �ٸ���.
		// �׷��� ť�� ����Ϸ�� ������ ���� ��� ũ�⸦ ����Ѵ�.
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
			str.Format(L"��� �� : %d bytes", m_real_play_bytes);
			SetDlgItemText(IDC_EDIT1, str);
		}
		else {
			// ����� �Ϸ� ��
			PostMessage(WM_COMMAND, IDC_EDIT1);  // ��ư�� ������ó�� ���۽�Ŵ
		}
		return 1;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CrecordDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (0 == m_state) {  // ���� ���¿����� ���� ���·� ����
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

		SetDlgItemText(IDC_BUTTON1, L"��� ��...");
		m_state = 2;
	}
	else {  // ���� ���¿����� ��� ���·� ����
		m_play_sound.StopDevice();
		m_state = 0;
		SetDlgItemText(IDC_BUTTON1, L"��� ����");
	}
}
