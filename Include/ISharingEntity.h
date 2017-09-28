/////////////////////////////////////////////////////////////
//  �ļ���:		ISharingEntity.h  
//
//  ����:		�汾��1.0      �������ڣ�2016-8-30
//
//  ����:		FTP�ĵ�����ӿ�
//                       
//										Add:  Carl   
/////////////////////////////////////////////////////////////

#pragma once

#ifdef _ISHARING_
#define DllExport_ISharing __declspec(dllexport)
#else
#define DllExport_ISharing __declspec(dllimport) 
#endif

class ISharingEntity
{
public:
	// create ftp connection
	virtual void FtpConnection() = 0;

	// open file by IEActiveX
	virtual void OpenFile() = 0;
};

DllExport_ISharing ISharingEntity* CreateSharingEntity();