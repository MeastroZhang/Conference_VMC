#include "StdAfx.h"
#include "DlgSetting.h"
#include "../../include/Util/Util.h"
#include <atlimage.h>

extern VidyoClientRequestConfiguration reqConfig;

CDlgSetting::CDlgSetting(HWND hParent)
{
	m_hParent = hParent;
}


CDlgSetting::~CDlgSetting(void)
{
	m_hParent = NULL;
}

LRESULT CDlgSetting::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CDlgSetting::InitWindow()
{
	m_pOptCommon = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCommon")));
	m_pOptLanguage = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optLanguage")));
	m_pOptPhoto = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optPhoto")));
	m_pOptUpdate = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optUpdate")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETTING"), _T("optCommon"), szText, MAX_PATH);
	m_pOptCommon->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optLanguage"), szText, MAX_PATH);
	m_pOptLanguage->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optPhoto"), szText, MAX_PATH);
	m_pOptPhoto->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optUpdate"), szText, MAX_PATH);
	m_pOptUpdate->SetText(szText);

	m_pTabSetting = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabSetting")));

	m_pOptShowLogWnd = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optShowLogWnd")));
	m_pOptShowParticipantName = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optShowParName")));
	m_pOptMusic = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optMusic")));
	m_pOptFullScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optFullScreen")));
	m_pOptProxy = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optProxy")));
	project_language_loadstring(_T("SETTING"), _T("optShowLogWnd"), szText, MAX_PATH);
	m_pOptShowLogWnd->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optShowParName"), szText, MAX_PATH);
	m_pOptShowParticipantName->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optMusic"), szText, MAX_PATH);
	m_pOptMusic->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optFullScreen"), szText, MAX_PATH);
	m_pOptFullScreen->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optProxy"), szText, MAX_PATH);
	m_pOptProxy->SetText(szText);

	m_pOptLangCN = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optLangCN")));
	m_pOptLangTW = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optLangTW")));
	m_pOptLangEN = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optLangEN")));
	project_language_loadstring(_T("SETTING"), _T("optLangCN"), szText, MAX_PATH);
	m_pOptLangCN->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optLangTW"), szText, MAX_PATH);
	m_pOptLangTW->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optLangEN"), szText, MAX_PATH);
	m_pOptLangEN->SetText(szText);

	m_pLabPhoto	= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labPhoto")));
	m_pEditPath	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhoto")));
	m_pBtnChangePath = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnChangePath")));
	m_pBtnOpen	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOpen")));
	project_language_loadstring(_T("SETTING"), _T("labPhoto"), szText, MAX_PATH);
	m_pLabPhoto->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("btnChangePath"), szText, MAX_PATH);
	m_pBtnChangePath->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("btnOpen"), szText, MAX_PATH);
	m_pBtnOpen->SetText(szText);

	m_pLabVersionTip= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labVersionTip")));
	m_pLabVersion	= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labVersion")));
	project_language_loadstring(_T("SETTING"), _T("labVersionTip"), szText, MAX_PATH);
	m_pLabVersionTip->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labVersion"), szText, MAX_PATH);
	m_pLabVersion->SetText(szText);
}

LRESULT CDlgSetting::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgSetting::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

void CDlgSetting::OnSelectChanged( TNotifyUI &msg )
{
	if (m_pTabSetting == NULL)
	{
		return ;
	}

	if (msg.pSender->GetName() == _T("optCommon"))
	{
		m_pTabSetting->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optLanguage"))
	{
		m_pTabSetting->SelectItem(1);
	}
	else if (msg.pSender->GetName() == _T("optPhoto"))
	{
		m_pTabSetting->SelectItem(2);
	}
	else if (msg.pSender->GetName() == _T("optUpdate"))
	{
		VidyoClientInEventClientVersionOnServer InEventClientVersion = {0};
		VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_CLIENT_VERSION_ON_SERVER, &InEventClientVersion, sizeof(InEventClientVersion));
		m_pTabSetting->SelectItem(3);
	}
}

void CDlgSetting::InitUI()
{
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

	m_pOptShowLogWnd->Selected(reqConfig.enableAutoLogIn && true);
	m_pOptShowParticipantName->Selected(reqConfig.enableShowConfParticipantName && true);
	m_pOptMusic->Selected(reqConfig.enableEntryTone && true);
	m_pOptFullScreen->Selected(reqConfig.enableFullScreenVideo && true);
	m_pOptProxy->Selected(reqConfig.enableForceProxy && true);

	//VidyoClientRequestGetLanguage reqLang = {VIDYO_CLIENT_LANGUAGE_CODE_EN};
	//VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_LANGUAGE , &reqLang, sizeof(reqLang));
	int iLang = ReadIniInt(_T("general"), _T("language"), 1);
	switch(iLang)
	{
	case 2:
		m_pOptLangTW->Selected(true);
		break;
	case 3:
		m_pOptLangEN->Selected(true);
		break;
	default:
		m_pOptLangCN->Selected(true);
		break;
	}

	TCHAR szText[255]=_T("");
	ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, szText, 255);
	m_pEditPath->SetText(szText);
}

void CDlgSetting::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			Close(0);
		}
		else if (msg.pSender == m_pBtnChangePath)
		{
			//TCHAR szText[MAX_PATH];
			//project_language_loadstring(_T("SETTING"), _T("wndSetPath"), szText, MAX_PATH);
			TCHAR szBuffer[MAX_PATH] = {0}; 
			SetPath(NULL, szBuffer);

			if (szBuffer[0] != '\0')
			{
				m_pEditPath->SetText(szBuffer);
				WriteIniString(_T("SETTING"), _T("PhotoPath"), szBuffer);
			}
		}
		else if (msg.pSender == m_pBtnOpen)
		{
			CDuiString strPath = m_pEditPath->GetText();
			OpenPath(m_hWnd, strPath);
		}
		else if (msg.pSender == m_pOptLangCN)
		{
			WriteIniInt(_T("general"), _T("language"), 1);
		}
		else if (msg.pSender == m_pOptLangTW)
		{
			WriteIniInt(_T("general"), _T("language"), 2);
		}
		else if (msg.pSender == m_pOptLangEN)
		{
			WriteIniInt(_T("general"), _T("language"), 3);
		}
		else if (msg.pSender == m_pOptShowLogWnd)
		{
			reqConfig.enableAutoLogIn = !m_pOptShowLogWnd->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if ( msg.pSender == m_pOptShowParticipantName )
		{
			reqConfig.enableShowConfParticipantName = !m_pOptShowParticipantName->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if (msg.pSender == m_pOptMusic)
		{
			reqConfig.enableEntryTone = !m_pOptMusic->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if (msg.pSender == m_pOptFullScreen)
		{
			reqConfig.enableFullScreenVideo = !m_pOptFullScreen->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if (msg.pSender == m_pOptProxy)
		{
			reqConfig.enableForceProxy = !m_pOptProxy->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		OnSelectChanged(msg);
	}
}