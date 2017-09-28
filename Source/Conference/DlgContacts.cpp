#pragma once

#include "StdAfx.h"
#include "DlgContacts.h"
#include "../../include/Util/Util.h"
#include "WSgSoap.h"
#include "DlgSetting.h"
#include "DlgAVSetting.h"
#include "DlgRoomSetting.h"
#include "WndInputPin.h"
#include "MsgCommon.h"
#include <ctime>
#include <stdlib.h>

CDlgContacts::CDlgContacts(HWND hParent)
{
	m_hParent = hParent;
	m_ptAccountData = NULL;
	m_ptAccountList = NULL;
}

CDlgContacts::~CDlgContacts(void)
{
	m_hParent = NULL;
}

LRESULT CDlgContacts::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CDlgContacts::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntContactsUI, NULL);

	bHandled = TRUE;
	return 0;
}

void CDlgContacts::InitWindow()
{
	m_pLabCaption	= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	m_pOptSearch	= static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optSearch")));
	m_pBtnSetting	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSetting")));
	m_pEditSearch	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editSearch")));
	m_pTileRoom		= static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileRoom")));

	m_pTileRoom->SetFixedColumns(4);
	SIZE sz = {180, 180};
	m_pTileRoom->SetItemSize(sz);
	RECT rc = {80,40,0,40};
	m_pTileRoom->SetPadding(rc);

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("CONTACTS"), _T("editSearch"), szText, MAX_PATH);
	m_pEditSearch->SetText(szText);
}

LRESULT CDlgContacts::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_TIMER:		   lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgContacts::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

void CDlgContacts::InitUI()
{
	// Init Data
	ReleaseUI();

	m_ptAccountData = new VidyoClientAccountData;
	memset(m_ptAccountData, 0, sizeof(VidyoClientAccountData));
	SoapMyAccount( m_ptAccountData );
	UpdataCaption();

	m_ptAccountList = new VidyoClientAccountList;
	m_ptAccountList->total = 0;
	m_ptAccountList->accountData = NULL;
	SoapGetContactList( m_ptAccountList );
	UpdataContactsList();
}

void CDlgContacts::ReleaseUI()
{
	// Release Data
	if (m_ptAccountData)
	{
		delete m_ptAccountData;
		m_ptAccountData = NULL;
	}

	if (m_ptAccountList)
	{
		if (m_ptAccountList->accountData != NULL)
		{
			delete [] m_ptAccountList->accountData;
			m_ptAccountList->accountData = NULL;
		}
		delete m_ptAccountList;
		m_ptAccountList = NULL;
	}
}

void CDlgContacts::UpdateUI()
{
	SoapMyAccount( m_ptAccountData );
	UpdataCaption();

	SoapGetContactList( m_ptAccountList );
	UpdataContactsList();
}

void CDlgContacts::UpdataCaption()
{
	TCHAR		tcTemp[VIDYO_CLIENT_USERNAME_SIZE*2];
	ChangUtf8ToText(m_ptAccountData->displayName, tcTemp);
	m_pLabCaption->SetText(tcTemp);
}

void CDlgContacts::UpdataContactsList(bool bSearch)
{
	// clear room tile
	m_pTileRoom->RemoveAll();
	m_pTileRoom->SetChildPadding(40);
	m_pTileRoom->SetChildVPadding(30);

	// add date time to tile
	if (!bSearch)
	{
		CDialogBuilder builder1;
		CContainerUI* pContainerDate = static_cast<CContainerUI*>(builder1.Create(_T ("\\DlgContacts\\elementDateTime.xml"), (UINT)0, NULL));
		CLabelUI* pLabWeek = static_cast<CLabelUI*>(pContainerDate->FindSubControl(_T("labWeek")));
		CLabelUI* pLabClock = static_cast<CLabelUI*>(pContainerDate->FindSubControl(_T("labClock")));
		CLabelUI* pLabWeekDay = static_cast<CLabelUI*>(pContainerDate->FindSubControl(_T("labWeekDay")));

		time_t t = time(0);
		char tmp[64]; 
		TCHAR temp[64];
		strftime( tmp, sizeof(tmp), "%a",localtime(&t) ); 
		puts( tmp ); 
		ChangUtf8ToText(tmp, temp);
		pLabWeek->SetText(temp);

		strftime( tmp, sizeof(tmp), "%d",localtime(&t) ); 
		puts( tmp ); 
		ChangUtf8ToText(tmp, temp);
		pLabWeekDay->SetText(temp);

		pContainerDate->SetTag(-1);
		m_pTileRoom->Add(pContainerDate);
		SetTimer(m_hWnd, TIMER_CLOCK, 1000, NULL);
		CalculateTime();
	}

	TCHAR szBuf[MAX_PATH] = {0};
	CDuiString strTemp;
	for( unsigned int i = 0; i < m_ptAccountList->total; i++ )
	{
		CDialogBuilder builder;
		CContainerUI* pRoot = static_cast<CContainerUI*>(builder.Create(_T ("\\DlgContacts\\elementRoom.xml"), (UINT)0, NULL));
		pRoot->SetTag(i);

		CLabelUI* pLabIsOwner = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labIsOwner")));
		if (m_ptAccountList->accountData[i].canControl) pLabIsOwner->SetVisible();

		CLabelUI* pLabRoomName = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labRoomName")));
		ChangUtf8ToText(m_ptAccountList->accountData[i].displayName, szBuf);
		pLabRoomName->SetText(szBuf);

		CLabelUI* pLabRoomID = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labRoomID")));
		ChangUtf8ToText(m_ptAccountList->accountData[i].extension, szBuf);
		strTemp.Format(_T("(%s)"), szBuf);
		pLabRoomID->SetText(strTemp);

		CLabelUI* pLabPin = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labPin")));
		if (m_ptAccountList->accountData[i].roomMode.hasPIN) pLabPin->SetVisible();

		CLabelUI* pLabFull = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labFull")));
		if (m_ptAccountList->accountData[i].roomStatus == VIDYO_CLIENT_ROOM_FULL) pLabFull->SetVisible();

		CLabelUI* pLabLock = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labLock")));
		if (m_ptAccountList->accountData[i].roomMode.isRoomLocked) pLabLock->SetVisible();

		m_pTileRoom->Add(pRoot);
	}
}

void CDlgContacts::OnBtnSearch(bool bClick)
{
	CDuiString	dStrEntityID = m_pEditSearch->GetText();
	char		szName[256];

	if (m_ptAccountList->accountData != NULL)
	{
		delete [] m_ptAccountList->accountData;
		m_ptAccountList->accountData = NULL;
		m_ptAccountList->total = 0;
	}

	if (!bClick) m_pOptSearch->Selected(true);

	if (dStrEntityID.IsEmpty() || (m_pOptSearch->IsSelected()&&bClick))
	{
		SoapGetContactList( m_ptAccountList );
	}
	else
	{
		SoapSearch(ChangTextToUtf8(dStrEntityID.GetData(), szName), m_ptAccountList);
		UpdataContactsList(true);
		return ;
	}

	UpdataContactsList();
}

LRESULT CDlgContacts::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_SEARCH)
	{
		KillTimer(m_hWnd, TIMER_SEARCH);

		OnBtnSearch(false);
	}
	else if (wParam == TIMER_CLOCK)
	{
		CalculateTime();
	}

	bHandled = FALSE;
	return 0;
}

void CDlgContacts::CalculateTime()
{
	CControlUI* pControl = m_pTileRoom->GetItemAt(0);
	if (!pControl) return;
	CContainerUI* pRoot = (CContainerUI*)pControl;
	CLabelUI* pLabClock = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labClock")));
	if (pLabClock)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CDuiString strTime;
		strTime.Format(_T("%02d:%02d:%02d"), sys.wHour, sys.wMinute, sys.wSecond);
		pLabClock->SetText(strTime);
	}
}

LRESULT CDlgContacts::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
				_tcsicmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_TEXT) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_EDIT) != 0)
				return HTCAPTION;
	}

	bHandled = TRUE;
	return lRes;
}

void CDlgContacts::Notify(TNotifyUI& msg)
{
	CDuiString strCtrlName = msg.pSender->GetName();
	if( msg.sType == _T("click") ) 
	{
		if( strCtrlName == _T("btnClose") ) 
		{
			//::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntContactsUI, NULL);
			ShowWindow(false);
		}
		else if (strCtrlName == _T("btnMin"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (msg.pSender->GetName() == _T("btnRecord"))
		{
			tRecordLibrary* ptRecordLibrary = new tRecordLibrary;
			ptRecordLibrary->token[0] = 0;
			ptRecordLibrary->URL[0] = 0;
			if (SoapGetVidyoReplayLibrary(ptRecordLibrary))
			{
				CDuiString strURL;
				TCHAR tchTemp1[128] = {0}, tchTemp2[128] = {0};
				ChangUtf8ToText(ptRecordLibrary->token, tchTemp1);
				ChangUtf8ToText(ptRecordLibrary->URL, tchTemp2);
				strURL.Format(_T("%s?token=%s"), tchTemp2, tchTemp1);
				ShellExecute(0, _T("open"), strURL, NULL, NULL, SW_SHOW);
			}
			else
			{
				MsgCommon* pPopWnd = new MsgCommon(MSG_RECORD_DISABLE);
				pPopWnd->Create(m_hWnd, _T("MsgCommon"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
			
			delete ptRecordLibrary;
			ptRecordLibrary = NULL;
		}
		else if (strCtrlName == _T("btnAdd"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();
			if (SoapAddToMyContacts(m_ptAccountList->accountData[iIndex].entityID))
			{
				m_ptAccountList->accountData[iIndex].isInMyContacts = TRUE;
			}
		}
		else if (strCtrlName == _T("btnRemove"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();
			if (SoapRemoveFromMyContacts(m_ptAccountList->accountData[iIndex].entityID))
			{
				for (int i = 0; i < m_pTileRoom->GetCount(); i++)
				{
					if (iIndex == m_pTileRoom->GetItemAt(i)->GetTag())
						m_pTileRoom->RemoveAt(i);
				}
			}
		}
		else if (strCtrlName == _T("btnRoomLock"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();
			if (SoapLockRoom(m_ptAccountList->accountData[iIndex].entityID, ""))
			{
				m_ptAccountList->accountData[iIndex].roomMode.isRoomLocked = TRUE;
			}
		}
		else if (strCtrlName == _T("btnRoomUnLock"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();
			if (SoapUnLockRoom(m_ptAccountList->accountData[iIndex].entityID, ""))
			{
				m_ptAccountList->accountData[iIndex].roomMode.isRoomLocked = FALSE;
			}
		}
		else if (strCtrlName == _T("btnRoomSetting"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();

			CDlgRoomSetting* pPopWnd = new CDlgRoomSetting((LPVOID*)&m_ptAccountList->accountData[iIndex]);
			pPopWnd->Create(m_hWnd, _T("RoomSetting"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			pPopWnd->CenterWindow();
			pPopWnd->ShowModal();
		}
		else if (strCtrlName == _T("btnRoomInvite"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();
			SoapInvite(m_ptAccountList->accountData[iIndex].entityID);
		}
		else if (strCtrlName == _T("btnRoomJoin"))
		{
			CContainerUI* pRoot = (CContainerUI*)((msg.pSender->GetParent())->GetParent());
			int iIndex = pRoot->GetTag();

			int iResult = SoapJoinConference(m_ptAccountList->accountData[iIndex].entityID);
			if (!iResult)
			{
				::PostMessageW(m_hParent, WM_JOIN_PROCESS, NULL, NULL);
			}
			else if (iResult == 1)
			{
				CWndInputPin* pPopWnd = new CWndInputPin(ntRoomPin, m_ptAccountList->accountData[iIndex].entityID);
				pPopWnd->Create(m_hWnd, _T("InputPin"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
			else if (iResult == 2)
			{
				MsgCommon* pPopWnd = new MsgCommon(MSG_ROOMLOCKED);
				pPopWnd->Create(m_hWnd, _T("MsgCommon"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
		}
		else if (msg.pSender == m_pOptSearch)
		{
			OnBtnSearch();
		}
		else if (msg.pSender == m_pBtnSetting)
		{
			CDlgSetting* pPopWnd = new CDlgSetting();
			pPopWnd->Create(m_hWnd, _T("Setting"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			RECT rc, rcClient;
			::GetClientRect(m_hWnd, &rc);
			::GetClientRect(pPopWnd->GetHWND(), &rcClient);
			POINT pt = msg.ptMouse;
			ClientToScreen(m_hWnd, &pt);
			SetWindowPos(pPopWnd->GetHWND(), NULL, rc.right-rcClient.right+pt.x-msg.ptMouse.x-4, m_PaintManager.GetCaptionRect().bottom+pt.y-msg.ptMouse.y+4, rcClient.right, rcClient.bottom, NULL);
			pPopWnd->InitUI();
			pPopWnd->ShowModal();
		}
		else if (msg.pSender->GetName() == _T("btnAVSetting"))
		{
			CDlgAVSetting* pPopWnd = new CDlgAVSetting();
			pPopWnd->Create(m_hWnd, _T("Setting"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			RECT rc, rcClient;
			::GetClientRect(m_hWnd, &rc);
			::GetClientRect(pPopWnd->GetHWND(), &rcClient);
			POINT pt = msg.ptMouse;
			ClientToScreen(m_hWnd, &pt);
			SetWindowPos(pPopWnd->GetHWND(), NULL, rc.right-rcClient.right+pt.x-msg.ptMouse.x-4, m_PaintManager.GetCaptionRect().bottom+pt.y-msg.ptMouse.y+4, rcClient.right, rcClient.bottom, NULL);
			pPopWnd->InitUI();
			pPopWnd->ShowModal();
		}
	}
	else if ( msg.sType == _T("mouseenter") )
	{
		for (int i = 0; i < m_pTileRoom->GetCount(); i++)
		{
			CControlUI* pControl = m_pTileRoom->GetItemAt(i);
			if (pControl)
			{
				CContainerUI* pRoot = (CContainerUI*)pControl;
				int iIndex = pRoot->GetTag();

				CButtonUI* pBtnAdd = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnAdd")));
				CButtonUI* pBtnRemove = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnRemove")));
				CHorizontalLayoutUI* pHorRoomFlg = static_cast<CHorizontalLayoutUI*>(pRoot->FindSubControl(_T("horRoomFlg")));
				CHorizontalLayoutUI* pHorRoomControl = static_cast<CHorizontalLayoutUI*>(pRoot->FindSubControl(_T("horRoomControl")));
				if (!pBtnAdd || !pBtnRemove || !pHorRoomFlg || !pHorRoomControl) continue;

				RECT rc = pControl->GetClientPos();
				if (::PtInRect(&rc, msg.ptMouse))
				{
					pHorRoomFlg->SetVisible(false);
					pHorRoomControl->SetVisible();
					CLabelUI* pLabIsOwner = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labIsOwner")));
					CButtonUI* pBtnIsMyContacts = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnIsMyContacts")));
					CButtonUI* pBtnLock = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnRoomLock")));
					CButtonUI* pBtnUnLock = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnRoomUnLock")));
					CButtonUI* pBtnSetting = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnRoomSetting")));
					CButtonUI* pBtnInvite = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnRoomInvite")));
					pBtnLock->SetVisible(m_ptAccountList->accountData[iIndex].canControl
						&&!m_ptAccountList->accountData[iIndex].roomMode.isRoomLocked);
					pBtnUnLock->SetVisible(m_ptAccountList->accountData[iIndex].canControl
						&&m_ptAccountList->accountData[iIndex].roomMode.isRoomLocked);
					pBtnSetting->SetVisible(m_ptAccountList->accountData[iIndex].canControl&&true);
					pBtnInvite->SetVisible(m_ptAccountList->accountData[iIndex].canControl&&true);

					pBtnRemove->SetVisible(m_ptAccountList->accountData[iIndex].isInMyContacts&&true);
					pBtnAdd->SetVisible(!m_ptAccountList->accountData[iIndex].isInMyContacts&&true);
				}
				else
				{
					pHorRoomFlg->SetVisible();
					pHorRoomControl->SetVisible(false);

					CLabelUI* pLabLock = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labLock")));
					pLabLock->SetVisible(m_ptAccountList->accountData[iIndex].roomMode.isRoomLocked&&true);

					CLabelUI* pLabPin = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labPin")));
					pLabPin->SetVisible(m_ptAccountList->accountData[iIndex].roomMode.hasPIN&&true);

					CLabelUI* pLabFull = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labFull")));
					if (m_ptAccountList->accountData[iIndex].roomStatus == VIDYO_CLIENT_ROOM_FULL) pLabFull->SetVisible();
					else pLabFull->SetVisible(false);

					if(pBtnAdd) pBtnAdd->SetVisible(false);
					if(pBtnRemove) pBtnRemove->SetVisible(false);
				}
			}
		}
	}
	else if( msg.sType == _T("setfocus") ) 
	{	
		if( msg.pSender == m_pEditSearch ) 
		{
			m_pEditSearch->SetText(_T(""));
		}
		else if ( strCtrlName == _T("btnRoomName"))
		{
			CButtonUI* pBtnAdd = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnAdd")));

			pBtnAdd->SetVisible();
		}
	}
	else if( msg.sType == _T("killfocus") ) 
	{	
		//if( msg.pSender == m_pEditSearch ) 
		//{
		//	m_pEditSearch->SetText(_T("ËÑË÷»áÒéÊÒ"));
		//}
	}
	else if ( msg.sType == DUI_MSGTYPE_TEXTCHANGED )
	{
		if( msg.pSender == m_pEditSearch ) 
		{
			CDuiString strSearch = m_pEditSearch->GetText();
			if (!strSearch.IsEmpty())
			{
				SetTimer(m_hWnd, TIMER_SEARCH, 500, NULL);
			}
		}
	}
}

