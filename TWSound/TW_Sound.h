#ifndef _TW_SOUND_H_
#define _TW_SOUND_H_

// �̵� �����ϱ����� ���Ǵ� �������
#include <mmsystem.h>



// �̵� �����ϱ����� ���Ǵ� ���̺귯��
#pragma comment (lib, "winmm.lib")

#ifdef _DEBUG 
	#pragma comment(lib, "D_TW_Sound_2015.lib") 
#else
	#pragma comment(lib, "R_TW_Sound_2015.lib") 
#endif




#define	MAX_QUEUE_NUMBER		3    // ������ ����

class TW_Sound
{
protected:
	// ��ġ�� ���� �ڵ�
	HANDLE mh_sound;
	// ���� �Ǵ� ����� ����� ť�� ũ��
	unsigned int m_queue_size;
	// ���� �Ǵ� ����� ���۵Ǿ������� ������ ����
	char m_is_start;
	// ���̺� ����� ������ �����ϱ� ���� ����ü
	WAVEFORMATEX m_wave_format;
	// Wave ������ �Ҹ� ������ �����ϱ� ���� �޸𸮸� ����
	WAVEHDR *mp_wave_header[MAX_QUEUE_NUMBER];

public:
	TW_Sound();
	virtual ~TW_Sound();

	// ������� ��ġ�� �ڵ� ���� ��´�.
	inline HANDLE GetDeviceHandle() { return mh_sound; }
	// ������� ť�� ũ�⸦ ��´�.
	inline int GetQueueSize() { return m_queue_size; }
	// ������ ť�� ������ ��´�.
	inline WAVEHDR *GetQueueData(int a_index) { return mp_wave_header[a_index]; }
	// ��ġ�� ������������ ��´�.
	inline char IsStart() { return m_is_start; }

	// WAVE ���� �Ǵ� ��� �Ӽ��� �����ϴ� ��쿡 ���
	// �⺻ �Ӽ��� 8Khz, 16bit, mono �� -> 1�ʴ� 16000 Bytes.(8000 * 2 * 1)
	void SetSoundFormat(WAVEFORMATEX *ap_user_format, HWND ah_notify_wnd);

	// ��ġ�� ����. 
	// MM_WIM_DATA �Ǵ� MM_WOM_DATA �޽����� ���� ������ �ڵ��� ah_notify_wnd�� ����Ѵ�.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// ��ġ�� �ݴ´�.
	virtual void CloseDevice();
	// ��ġ�� ���۽�Ų��.
	virtual int StartDevice();
	// ��ġ�� ������Ų��.
	virtual void StopDevice();

	// ������ ���, ������ ť�� ��ȯ�Ǹ� ť�� ����� �Ҹ� �����͸� ����ϰ� �ٽ� ť��
	// ���� ��ġ�� �����ؾ� �ϴµ� �� �۾��� ReuseQueue �Լ��� ����ؼ� ó���ϸ� �ȴ�.
	virtual void ReuseQueue(WAVEHDR *ap_queue);
	// ����� ��� ��� ť�� �Ҹ� �����͸� �����ؼ� ��� ��ġ�� �����ؾ� �Ѵ�.
	// SetSoundData�� ��ġ�� �����ؼ� �Ҹ� �����͸� �����ϴ� ��� �Ʒ��� �Լ��� ����ϸ� �ȴ�.
	virtual void SetSoundData(int a_queue_index, char *ap_data, int a_data_size);
	// SetSoundData�� ��ġ�� �������� �ʰ� ���������� ���������� ��ġ�� �����ǰ� �ϰ� �ʹٸ�
	// �Ʒ��� �Լ��� ����ϸ� �ȴ�.
	virtual void SetSoundData(char *ap_data, int a_data_size);
};

// ���� ��ġ�� ���� Ŭ����
class TW_SoundIn : public TW_Sound
{
public:
	TW_SoundIn();
	virtual ~TW_SoundIn();

	// ��ġ�� ����. 
	// MM_WIM_DATA �Ǵ� MM_WOM_DATA �޽����� ���� ������ �ڵ��� ah_notify_wnd�� ����Ѵ�.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// ��ġ�� �ݴ´�.
	virtual void CloseDevice();
	// ��ġ�� ���۽�Ų��.
	virtual int StartDevice();
	// ��ġ�� ������Ų��.
	virtual void StopDevice();

	// ������ ���, ������ ť�� ��ȯ�Ǹ� ť�� ����� �Ҹ� �����͸� ����ϰ� �ٽ� ť��
	// ���� ��ġ�� �����ؾ� �ϴµ� �� �۾��� ReuseQueue �Լ��� ����ؼ� ó���ϸ� �ȴ�.
	virtual void ReuseQueue(WAVEHDR *ap_queue);
};

class TW_SoundOut : public TW_Sound
{
protected:
	// ��� ť�� ������ �ڵ����� ����ϱ� ���� ����
	int m_auto_index;

public:
	TW_SoundOut();
	virtual ~TW_SoundOut();

	// ��ġ�� ����. 
	// MM_WIM_DATA �Ǵ� MM_WOM_DATA �޽����� ���� ������ �ڵ��� ah_notify_wnd�� ����Ѵ�.
	virtual int OpenDevice(HWND ah_notify_wnd);
	// ��ġ�� �ݴ´�.
	virtual void CloseDevice();
	// ��ġ�� ���۽�Ų��.
	virtual int StartDevice();
	// ��ġ�� ������Ų��.
	virtual void StopDevice();

	// ����� ��� ��� ť�� �Ҹ� �����͸� �����ؼ� ��� ��ġ�� �����ؾ� �Ѵ�.
	// SetSoundData�� ��ġ�� �����ؼ� �Ҹ� �����͸� �����ϴ� ��� �Ʒ��� �Լ��� ����ϸ� �ȴ�.
	virtual void SetSoundData(int a_queue_index, char *ap_data, int a_data_size);
	// SetSoundData�� ��ġ�� �������� �ʰ� ���������� ���������� ��ġ�� �����ǰ� �ϰ� �ʹٸ�
	// �Ʒ��� �Լ��� ����ϸ� �ȴ�.
	virtual void SetSoundData(char *ap_data, int a_data_size);
};

#endif
