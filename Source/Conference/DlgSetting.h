#pragma once

class CDlgSetting : public WindowImplBase
{
public:
	CDlgSetting(HWND hParent = NULL);
	~CDlgSetting(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgSetting\\DlgSetting.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Setting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	void Notify(TNotifyUI& msg);
	void OnSelectChanged( TNotifyUI &msg );

	HWND m_hParent;
	CWndShadow	m_WndShadow;
public:
	void InitUI();

private:
	COptionUI*	m_pOptCommon;
	COptionUI*	m_pOptLanguage;
	COptionUI*	m_pOptPhoto;
	COptionUI*	m_pOptUpdate;

	CTabLayoutUI*	m_pTabSetting;

	COptionUI*	m_pOptShowLogWnd;
	COptionUI*	m_pOptShowParticipantName;
	COptionUI*	m_pOptMusic;
	COptionUI*	m_pOptFullScreen;
	COptionUI*	m_pOptProxy;

	COptionUI*	m_pOptLangCN;
	COptionUI*	m_pOptLangTW;
	COptionUI*	m_pOptLangEN;

	CLabelUI*	m_pLabPhoto;
	CEditUI*	m_pEditPath;
	CButtonUI*	m_pBtnChangePath;
	CButtonUI*	m_pBtnOpen;

	CLabelUI*	m_pLabVersionTip;
	CLabelUI*	m_pLabVersion;
};

