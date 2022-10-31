
// recordDlg.h : 헤더 파일
//
#include "TW_Sound.h"

#pragma once


// CrecordDlg 대화 상자
class CrecordDlg : public CDialogEx
{
// 생성입니다.
public:
	CrecordDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD_DIALOG };
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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	CRichEditCtrl	m_status;

	int m_state;
	int m_rec_bytes;
	int m_play_bytes, m_real_play_bytes;
	char *mp_rec_buffer;

	TW_SoundIn m_rec_sound;
	TW_SoundOut m_play_sound;

	LRESULT CrecordDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
};
