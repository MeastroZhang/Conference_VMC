#pragma once

#ifdef CAMERA_SHARE
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport) 
#endif

/* �ⲿ�ӿڵ��� */
DECLSPEC void InitCameraShare();

DECLSPEC void ReleaseCamera();