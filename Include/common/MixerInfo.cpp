#include "StdAfx.h"
#include ".\MixerInfo.h"
#include "../../include/Util/Util.h"
 #include <mmsystem.h>

CMixerInfo::CMixerInfo(HWND hWnd,DWORD dwComponentType, DWORD dwMinValue, DWORD dwMaxValue)
{
	m_hWnd					= hWnd;
	m_dwMinValue			= dwMinValue;
	m_dwMaxValue			= dwMaxValue;			
	m_hMixer				= NULL;								
	m_strDeviceName			= _T("");

	switch(dwComponentType)
	{
	case MIXERLINE_COMPONENTTYPE_DST_SPEAKERS:
		m_dwComponentType=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		InitVolumeInfo(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS);
		break;
	case MIXERLINE_COMPONENTTYPE_DST_WAVEIN:
		m_dwComponentType=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		InitVolumeInfo(MIXERLINE_COMPONENTTYPE_DST_WAVEIN);
		break;
	}
}

CMixerInfo::~CMixerInfo(void)
{
	m_hWnd = NULL;
	if (m_hMixer != NULL)
	{	
		//�رջ����
		::mixerClose(m_hMixer);
		m_hMixer = NULL;
	}
}

CDuiString CMixerInfo::GetDeviceName()
{
	return m_strDeviceName;
}

DWORD CMixerInfo::GetVolumeValue()
{
	return GetValue(m_dwComponentType);
}

void CMixerInfo::SetVolumeValue(DWORD dwValue)
{
	SetValue(dwValue, m_dwComponentType);
}

BOOL CMixerInfo::GetVolumeMute()							// �ж��Ƿ���
{
	return GetMute(m_dwComponentType);
}

void CMixerInfo::SetVolumeMute(BOOL bMute)				// ���þ���
{
	SetMute(bMute, m_dwComponentType);
}

void CMixerInfo::InitVolumeInfo(DWORD dwComponentType)
{	
	UINT		uNumMixers;				// �����������											
	MIXERCAPS	MixerCaps;				// ��ǰ����������ܲ���	

	uNumMixers = ::mixerGetNumDevs();	//��ʼ������ȡ��ǰ����豸���� 
	::ZeroMemory(&MixerCaps, sizeof(MIXERCAPS));
	if(uNumMixers !=0)
	{	
		for(unsigned int i = 0; i < uNumMixers; i ++)
		{
			// �򿪻���豸
			if (::mixerOpen(&m_hMixer, i, (DWORD)m_hWnd, NULL, /*MIXER_OBJECTF_MIXER |*/ CALLBACK_WINDOW) != MMSYSERR_NOERROR)
				continue ;

			MIXERLINE MixerLine;   
			//�ṹ��Ĵ�С   
			MixerLine.cbStruct = sizeof(MixerLine);   
			//¼���豸����   
			MixerLine.dwComponentType   =   dwComponentType;   
			//�õ�¼�������е�������   
			if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue; 
			else
				break;
		}

		// ȷ���������豸������
		if (::mixerGetDevCaps((UINT)m_hMixer, &MixerCaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
			return ;
	}

	m_strDeviceName.Format(_T("%s"), MixerCaps.szPname);
}

DWORD CMixerInfo::GetValue(DWORD dwComponentType)
{
	//��û������Ϣ
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return  101;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//��û�����߿ؼ�
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return 101;

	//��ȡ��ǰ����
	DWORD NewVolume = 0;	
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails		= &mxcdVolume;

	//��ȡָ��������ؼ� 
	if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return 101;

	NewVolume = (mxcdVolume.dwValue*m_dwMaxValue)/mxc.Bounds.dwMaximum;

	return NewVolume;
}

void  CMixerInfo::SetValue(DWORD dwValue, DWORD dwComponentType)
{
	//��û������Ϣ
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//��û�����߿ؼ�
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return;

	//��������
	MIXERCONTROLDETAILS_UNSIGNED NewmxcdVolume = { (dwValue*mxc.Bounds.dwMaximum)/m_dwMaxValue };
	MIXERCONTROLDETAILS mxcd;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails		= &NewmxcdVolume;

	//���û�����ؼ� 
	if (::mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return ;
}

BOOL CMixerInfo::GetMute(DWORD dwComponentType)
{
	//��û������Ϣ
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return FALSE;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_MUTE;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//��û�����߿ؼ�
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return FALSE;


	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcMute;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails		= &mxcMute;

	//��ȡָ��������ؼ� 
	if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return FALSE;

	return !mxcMute.fValue;
}

void CMixerInfo::SetMute(BOOL bMute, DWORD dwComponentType)
{
	//��û������Ϣ
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_MUTE;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//��û�����߿ؼ�
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return;


	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcMute;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails		= &mxcMute;

	mxcMute.fValue      = !bMute;

	//���û�����ؼ� 
	if (::mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return ;
}