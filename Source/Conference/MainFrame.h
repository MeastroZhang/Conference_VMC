#pragma once

#include <vector>
#include "../../include/Util/Util.h"
#include "DlgConference.h"
#include "DlgLogin.h"
#include "DlgGuest.h"
#include "DlgContacts.h"
#include "WndProcessing.h"
#include "MenuTray.h"
#include "resource.h"

class MainFrame : public WindowImplBase
{
public:
	MainFrame(void);
	~MainFrame(void);

protected:
	virtual CDuiString	GetSkinFolder()				{ return _T ("skin"); };
	virtual CDuiString	GetSkinFile()				{ return _T ("FrameWnd.xml"); };
	virtual LPCTSTR		GetWindowClassName() const	{ return _T ("FrameWnd"); };
	virtual void		InitWindow();
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CDlgConference*		m_pDlgConference;
	CDlgLogin*			m_pDlgLogin;
	CDlgGuest*			m_pDlgGuest;
	CDlgContacts*		m_pDlgContacts;
	CWndProcessing*		m_pWndProcessing;

	// UI������ʾ����ջ
	vector<eModeType>	m_ModeTypeStack;		// �����ʾ��ģ���ջ
	int					m_iConferenceUICount;
	int					m_iLoginUICount;
	int					m_iContactsUICount;

	// �������λ��
	int					m_iMouseXPos;
	int					m_iMouseYPos;

	CTrayIcon			m_trayIcon;
private: 
	void Push(eModeType eType);					// ѹջ
	void Pop(eModeType eType);					// ��ջ
	void ShowUI();								// ��ʾģ�����
	void HideAllUI();							// �������н���
	void ClearStack();							// ���ջ
	void DestroyAllUI();						// �ͷ����еĽ���

	void CreateConferenceUI();
	void CreateLoginUI();
	void CreateGuestUI();
	void CreateContactsUI();
	void CreateProcessingWnd();

	BOOL ProCmdline();
public:
	LRESULT OnVidyoMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTrayMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowProcess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitVidyoSystem();
	void ReleaseVidyoSystem();
	void ShowTrayMenu();
	void TrayLogout();
	void TrayExitConf();
};