#ifndef _TW_SOUND_H_
#define _TW_SOUND_H_

// 미디어를 제어하기위해 사용되는 헤더파일
#include <mmsystem.h>



// 미디어를 제어하기위해 사용되는 라이브러리
#pragma comment (lib, "winmm.lib")

#ifdef _DEBUG 
	#pragma comment(lib, "D_TW_Sound_2015.lib") 
#else
	#pragma comment(lib, "R_TW_Sound_2015.lib") 
#endif




#define	MAX_QUEUE_NUMBER		3    // 버퍼의 개수

class TW_Sound
{
protected:
	// 장치에 대한 핸들
	HANDLE mh_sound;
	// 녹음 또는 재생에 사용할 큐의 크기
	unsigned int m_queue_size;
	// 녹음 또는 재생이 시작되었는지를 저장할 변수
	char m_is_start;
	// 웨이브 입출력 형식을 저장하기 위한 구조체
	WAVEFORMATEX m_wave_format;
	// Wave 형식의 소리 정보를 관리하기 위한 메모리를 선언
	WAVEHDR *mp_wave_header[MAX_QUEUE_NUMBER];

public:
	TW_Sound();
	virtual ~TW_Sound();

	// 사용중인 장치의 핸들 값을 얻는다.
	inline HANDLE GetDeviceHandle() { return mh_sound; }
	// 사용중인 큐의 크기를 얻는다.
	inline int GetQueueSize() { return m_queue_size; }
	// 지정한 큐의 정보를 얻는다.
	inline WAVEHDR *GetQueueData(int a_index) { return mp_wave_header[a_index]; }
	// 장치가 동작중인지를 얻는다.
	inline char IsStart() { return m_is_start; }

	// WAVE 녹음 또는 재생 속성을 변경하는 경우에 사용
	// 기본 속성은 8Khz, 16bit, mono 임 -> 1초당 16000 Bytes.(8000 * 2 * 1)
	void SetSoundFormat(WAVEFORMATEX *ap_user_format, HWND ah_notify_wnd);

	// 장치를 연다. 
	// MM_WIM_DATA 또는 MM_WOM_DATA 메시지를 받을 윈도우 핸들을 ah_notify_wnd에 명시한다.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// 장치를 닫는다.
	virtual void CloseDevice();
	// 장치를 동작시킨다.
	virtual int StartDevice();
	// 장치를 중지시킨다.
	virtual void StopDevice();

	// 녹음의 경우, 녹음된 큐가 반환되면 큐에 저장된 소리 데이터를 사용하고 다시 큐를
	// 녹음 장치에 전달해야 하는데 이 작업을 ReuseQueue 함수를 사용해서 처리하면 된다.
	virtual void ReuseQueue(WAVEHDR *ap_queue);
	// 재생의 경우 재생 큐에 소리 데이터를 저장해서 재생 장치에 전달해야 한다.
	// SetSoundData에 위치를 지정해서 소리 데이터를 저장하는 경우 아래의 함수를 사용하면 된다.
	virtual void SetSoundData(int a_queue_index, char *ap_data, int a_data_size);
	// SetSoundData에 위치를 지정하지 않고 내부적으로 순차적으로 위치가 지정되게 하고 싶다면
	// 아래의 함수를 사용하면 된다.
	virtual void SetSoundData(char *ap_data, int a_data_size);
};

// 녹음 장치를 위한 클래스
class TW_SoundIn : public TW_Sound
{
public:
	TW_SoundIn();
	virtual ~TW_SoundIn();

	// 장치를 연다. 
	// MM_WIM_DATA 또는 MM_WOM_DATA 메시지를 받을 윈도우 핸들을 ah_notify_wnd에 명시한다.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// 장치를 닫는다.
	virtual void CloseDevice();
	// 장치를 동작시킨다.
	virtual int StartDevice();
	// 장치를 중지시킨다.
	virtual void StopDevice();

	// 녹음의 경우, 녹음된 큐가 반환되면 큐에 저장된 소리 데이터를 사용하고 다시 큐를
	// 녹음 장치에 전달해야 하는데 이 작업을 ReuseQueue 함수를 사용해서 처리하면 된다.
	virtual void ReuseQueue(WAVEHDR *ap_queue);
};

class TW_SoundOut : public TW_Sound
{
protected:
	// 재생 큐의 순서를 자동으로 계산하기 위한 변수
	int m_auto_index;

public:
	TW_SoundOut();
	virtual ~TW_SoundOut();

	// 장치를 연다. 
	// MM_WIM_DATA 또는 MM_WOM_DATA 메시지를 받을 윈도우 핸들을 ah_notify_wnd에 명시한다.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// 장치를 닫는다.
	virtual void CloseDevice();
	// 장치를 동작시킨다.
	virtual int StartDevice();
	// 장치를 중지시킨다.
	virtual void StopDevice();

	// 재생의 경우 재생 큐에 소리 데이터를 저장해서 재생 장치에 전달해야 한다.
	// SetSoundData에 위치를 지정해서 소리 데이터를 저장하는 경우 아래의 함수를 사용하면 된다.
	virtual void SetSoundData(int a_queue_index, char *ap_data, int a_data_size);
	// SetSoundData에 위치를 지정하지 않고 내부적으로 순차적으로 위치가 지정되게 하고 싶다면
	// 아래의 함수를 사용하면 된다.
	virtual void SetSoundData(char *ap_data, int a_data_size);
};

#endif
