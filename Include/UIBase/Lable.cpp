// Label.cpp : implementation file   
//   

#include "stdafx.h"   
#include "Lable.h"   

#ifdef _DEBUG   
#define new DEBUG_NEW   
#undef THIS_FILE   
static char THIS_FILE[] = __FILE__;   
#endif   

/////////////////////////////////////////////////////////////////////////////   
// CLabel   

CLabel::CLabel(void)   
{   
	//��ʼ���ı���ɫ�ͱ�����ɫ   
	m_crText = GetSysColor(COLOR_WINDOWTEXT);   
	m_crBkground = GetSysColor(COLOR_3DFACE);  
	m_crOldText = m_crText;

	//��ʼ������ṹ   
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);   

	//��ʼ������   
	m_bLink = FALSE;   

	//��ʼ����˸���͡��Ƿ���˸   
	m_Type = CLabel::None;   
	m_bFlash=FALSE;   

	//��ʼ�������   
	m_hCursor=NULL;   

	//��ʼ������ɽ����־   
	m_bStartTimerFlag=FALSE;   
}   

//�������캯��   
CLabel::CLabel(const CLabel & p)   
{   
	//��������   
	m_lf=p.m_lf;   

	//�����Ƿ�����   
	m_bLink=p.m_bLink;   

	//������˸����   
	m_Type=p.m_Type;   

	//���ñ���ɫ���ı�ɫ   
	m_crBkground=p.m_crBkground;   
	m_crText=p.m_crText; 
	m_crOldText = p.m_crOldText;

	//���ù��   
	if(p.m_hCursor != NULL)   
	{   
		m_hCursor=CopyCursor(p.m_hCursor);   
	}   

	//��ʱ��������־����˸��־   
	m_bStartTimerFlag=FALSE;   
	m_bFlash=FALSE;    
}   

//��ֵ���캯��   
CLabel& CLabel::operator=(const CLabel& p)   
{   
	if(this != &p)   
	{   
		//��������   
		m_lf=p.m_lf;   

		//�����Ƿ�����   
		m_bLink=p.m_bLink;   

		//������˸����   
		m_Type=p.m_Type;   

		//���ñ���ɫ���ı�ɫ   
		m_crBkground=p.m_crBkground;   
		m_crText=p.m_crText;
		m_crOldText = p.m_crOldText;

		//���ù��   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
			m_hCursor=NULL;   
		}   
		if(p.m_hCursor != NULL)   
		{   
			m_hCursor=CopyCursor(p.m_hCursor);   
		}   

		//��ʱ��������־����˸��־   
		m_bStartTimerFlag=FALSE;   
		m_bFlash=FALSE;    
	}   

	return *this;   
}   

//��������   
CLabel::~CLabel(void)   
{   
	//���ٹ��   
	if(m_hCursor != NULL)   
	{   
		DestroyCursor(m_hCursor);   
		m_hCursor=NULL;   
	}   

	//��������   
	m_textfont.DeleteObject();   
}   

//�����ı�   
void CLabel::SetText(IN const CString& strText)   
{   
	//������ʾ�ı�   
	SetWindowText(strText);   

	//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
	Invalidate();   
}   

//�����ı���ɫ   
void CLabel::SetTextColor(IN COLORREF crText)   
{   
	//�����ı���ɫ   
	m_crText = crText; 
	m_crOldText = m_crText;

	//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
	Invalidate();   
}   

//�����Ƿ�Ӵ���ʾ   
BOOL CLabel::SetFontBold(IN BOOL bBold)   
{      
	BOOL returnvalue=FALSE;   

	//�����Ƿ�Ӵ�   
	if(bBold == TRUE)   
	{   
		m_lf.lfWeight=FW_BOLD;   
	}   
	else   
	{   
		m_lf.lfWeight=FW_NORMAL;   
	}   

	//��������   
	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//�����Ƿ��»�����ʾ   
BOOL CLabel::SetFontUnderline(IN BOOL bSet)   
{      
	BOOL returnvalue=FALSE;   

	//�����Ƿ��»���   
	m_lf.lfUnderline = bSet;   

	//��������   
	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//�����Ƿ�б����ʾ   
BOOL CLabel::SetFontItalic(IN BOOL bSet)   
{   
	BOOL returnvalue=FALSE;   

	//�����Ƿ�б��   
	m_lf.lfItalic = bSet;   

	//��������   
	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//�����ֺ�   
BOOL CLabel::SetFontSize(IN long nSize)   
{   
	BOOL returnvalue=FALSE;   

	//�������ֺ�   
	if(nSize >=0 )   
	{   
		m_lf.lfHeight = nSize;   
	}   
	else   
	{   
		m_lf.lfHeight = -nSize;   
	}   

	//��������   
	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//��ȡ�Ƿ�Ӵ�   
BOOL CLabel::IsFontBold(void)   
{   
	BOOL returnvalue=FALSE;   

	//�ж�����Ȩ���Ƿ���ڵ���700   
	if(m_lf.lfWeight >= 700)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//��ȡ�Ƿ�б��   
BOOL CLabel::IsFontItalic(void)   
{   
	BOOL returnvalue=FALSE;   

	//�ж�����б�������Ƿ�Ϊ��   
	if(m_lf.lfItalic != 0)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//��ȡ�Ƿ��»���   
BOOL CLabel::IsFontUnderline(void)   
{   
	BOOL returnvalue=FALSE;   

	//�ж������»��������Ƿ�Ϊ��   
	if(m_lf.lfUnderline != 0)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//��ȡ�ֺ�   
long     CLabel::GetFontSize(void)   
{   
	long returnvalue=0;   

	//��ȡ����ľ���ֵ   
	returnvalue=abs(m_lf.lfWidth);   

	return returnvalue;   
}   

//��ȡ������   
CString CLabel::GetFontName(void)   
{   
	CString tempstring("");   

	tempstring=m_lf.lfFaceName;   

	return tempstring;   
}   

// ��ȡ����
LOGFONT CLabel::GetLogFont(void) 
{
	return m_lf;
}

//���ñ���ɫ   
void CLabel::SetBkColor(IN COLORREF crBkgnd)   
{   
	//���ñ���ɫ   
	m_crBkground=crBkgnd;   

	//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
	Invalidate();   
}   

//��ȡ����ɫ   
COLORREF CLabel::GetBkColor(void)   
{   
	return m_crBkground;   
}   

//��ȡǰ��ɫ   
COLORREF CLabel::GetTextColor(void)   
{   
	return m_crText;   
}   

//����������   
BOOL CLabel::SetFontName(IN const CString& strFont)   
{      
	BOOL returnvalue=FALSE;   

	//����������   
	_tcscpy(m_lf.lfFaceName,strFont);   

	//��������   
	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   


BEGIN_MESSAGE_MAP(CLabel, CStatic)   
	//{{AFX_MSG_MAP(CLabel)   
	ON_WM_TIMER()   
//	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()   
	ON_WM_ERASEBKGND()   
	ON_WM_PAINT()  
	ON_WM_DESTROY()   
	//}}AFX_MSG_MAP   
END_MESSAGE_MAP()   

/////////////////////////////////////////////////////////////////////////////   
// CLabel message handlers   

//WM_TIMER��Ӧ����   
void CLabel::OnTimer(IN UINT nIDEvent)    
{   
	//�޸���˸��־   
	m_bFlash = !m_bFlash;   

	//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
	Invalidate();   

	//  CStatic::OnTimer(nIDEvent);   
}   

//�����Ƿ��������ı���ʾ   
void CLabel::SetLink(BOOL bLink, COLORREF crLinkText /*= RGB(0,0,255)*/)   
{   
	HINSTANCE hInstance=AfxGetInstanceHandle();   

	//�޸����ӱ�־   
	m_bLink=bLink;   

	if(bLink == TRUE)   
	{   
		//����ԭ�ı���ɫ
		m_crOldText = m_crText;

		//�����ı���ɫ   
		m_crText=crLinkText;   

		//�����»���   
		m_lf.lfUnderline=TRUE;   

		//�����¹��   
		m_hCursor=::LoadCursor(NULL, IDC_HAND);
		
		//�޸�NOTIFY��񣬸÷��ɼ���WM_SETCURSOR   
		ModifyStyle(0,SS_NOTIFY);   
	}   
	else   
	{   
		//�����ı���ɫΪԭ�ı�ɫ   
		m_crText=m_crOldText;   

		//�����»���   
		m_lf.lfUnderline=FALSE;   

		//���ٹ��   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
			m_hCursor=NULL;   
		}   

		//�޸�NOTIFY��񣬸÷��ɼ���WM_SETCURSOR   
		ModifyStyle(SS_NOTIFY,0);   
	}   

	if(CreateFont()==TRUE)   
	{   
		//�ػ洰��(�������ԣ�Invalidate����ʱ��ԶС��RedrawWindow   
		Invalidate();   
	} 
}   

//�Ƿ�����   
BOOL    CLabel::IsLink(void)   
{   
	return m_bLink;   
}   

//WM_LBUTTONDOWN��Ϣ��Ӧ����   
//void CLabel::OnLButtonDown(IN UINT nFlags, IN CPoint point)    
//{   
//	const int   nTextLength=1024;   
//	TCHAR text[nTextLength];   
//	int length=0;   
//
//	//�ж��Ƿ������ӱ�־   
//	if(m_bLink == TRUE)   
//	{   
//		//��ȡ�����ı�   
//		length=GetWindowText(text,nTextLength-1);   
//		text[min(length,nTextLength-1)]='\0';   
//		
//		//ִ������   
//		ShellExecute(NULL,_T("open"),L"http://192.168.0.160:8080/ecim/search/viewuser.jsp?sessionid=300&flag=true",NULL,NULL,SW_SHOWMAXIMIZED);   
//	}   
//}   

//WM_SETCURSOR��Ϣ��Ӧ����   
BOOL CLabel::OnSetCursor(IN CWnd* pWnd,IN UINT nHitTest,IN  UINT message)    
{   
	BOOL returnvalue=FALSE;   

	if(m_hCursor != NULL)   
	{   
		//�����¹��   
		::SetCursor(m_hCursor);   

		returnvalue=TRUE;   
	}   
	else   
	{   
		if(CStatic::OnSetCursor(pWnd, nHitTest, message) != 0)   
		{   
			returnvalue=TRUE;   
		}   
	}   

	return returnvalue;   
}   

//WM_ERASEBKGND��Ϣ��Ӧ����   
BOOL CLabel::OnEraseBkgnd(IN CDC* pDC)    
{   
	// TODO: Add your message handler code here and/or call default   
	return TRUE;   
}   

//WM_PAINT��Ϣ��Ӧ����   
void CLabel::OnPaint(void)    
{   
	CPaintDC dc(this); // device context for painting   

	// TODO: Add your message handler code here   

	// Do not call CStatic::OnPaint() for painting messages   
	CRect clientrect(0,0,0,0);//�ͻ���RECT    
	
	//��ȡ�ͻ����ߴ�   
	GetClientRect(&clientrect);   

	//���Ʊ���   
	DrawBackground(dc,clientrect);   
	//�����ı�   
	DrawText(dc,clientrect);   
}   

//������˸����   
BOOL CLabel::SetFlashType(IN int iFlashType)   
{   
	BOOL returnvalue=FALSE;   

	//��֤iFlashType����Ч��   
	if(iFlashType == CLabel::None    
		|| iFlashType == CLabel::Text   
		|| iFlashType == CLabel::Background)   
	{   
		m_Type = iFlashType;   
		returnvalue=TRUE;   
	}   
	else   
	{   
		m_Type = ErrorType;   
	}   

	return returnvalue;   
}   

//��ȡ��˸����   
int CLabel::GetFlashType(void)   
{   
	return m_Type;   
}   

//������˸   
BOOL CLabel::StartFlash(IN UINT iIntervalTime)   
{   
	BOOL returnvalue=FALSE;   
	UINT_PTR bSetTimer;   

	//����ֹͣ��ʱ��   
	StopFlash();   

	//������ʱ��   
	bSetTimer=SetTimer(IDT_TIMER1,iIntervalTime,NULL);   
	if(bSetTimer != 0)   
	{   
		m_bStartTimerFlag=TRUE;   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//�ر���˸   
void CLabel::StopFlash(void)   
{   
	if(m_bStartTimerFlag == TRUE)   
	{   
		KillTimer(IDT_TIMER1);   

		m_bStartTimerFlag=FALSE;   
	}   
}   

//���ù��   
BOOL CLabel::SetCursor(IN UINT nIDCursor)   
{   
	HCURSOR hCursor=NULL;   
	BOOL    returnvalue=FALSE;   
	HINSTANCE hInstance=AfxGetInstanceHandle();   


	//װ�ع����Դ   
	hCursor=::LoadCursor(NULL, MAKEINTRESOURCE(nIDCursor));   

	if(hCursor != NULL)   
	{   
		//�����ǰ�����Ч����������   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
		}   
		//�����¹��   
		m_hCursor=hCursor;   

		ModifyStyle(0,SS_NOTIFY);   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//WM_DESTROY��Ӧ����   
void CLabel::OnDestroy(void)    
{   
	//���ٶ�ʱ��   
	if(m_bStartTimerFlag == TRUE)   
	{   
		KillTimer(IDT_TIMER1);   

		m_bStartTimerFlag=FALSE;   
	}   

	CStatic::OnDestroy();   

	// TODO: Add your message handler code here   
}   

//���ù�֧꣨�ֶ�̬��꣩   
BOOL CLabel::SetCursor(IN LPCTSTR lpCursorFileName)   
{   
	HCURSOR hCursor=NULL;   
	BOOL    returnvalue=FALSE;   

	//װ�ع����Դ   
	hCursor=::LoadCursorFromFile(lpCursorFileName);   

	if(hCursor != NULL)   
	{   
		//�����ǰ�����Ч������������ǰ���   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
		}   
		//�����¹��   
		m_hCursor=hCursor;   

		ModifyStyle(0,SS_NOTIFY);   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//��������   
BOOL CLabel::CreateFont(void)   
{   
	BOOL bCreateFont=FALSE;   
	BOOL returnvalue=FALSE;   

	//�����ͷ�   
	m_textfont.DeleteObject();   

	//��������   
	bCreateFont=m_textfont.CreateFontIndirect(&m_lf);   

	if(bCreateFont == TRUE)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//�����ı�   
void CLabel::DrawText(CDC &dc, CRect &clientrect)   
{   
	CString strText("");   
	UINT nTextFlags=DT_EDITCONTROL | DT_WORDBREAK | DT_END_ELLIPSIS |  DT_NOPREFIX;   
	UINT maskchar=0x03;   
	DWORD windowstyle=GetStyle();   

	
	//��ȡ�ı����Ʊ�־   
	maskchar &= windowstyle;   
	if(maskchar == SS_CENTER)   
	{   
		nTextFlags |= DT_CENTER;   
	}   
	else if(maskchar == SS_LEFT) 
	{   
		nTextFlags |= DT_LEFT;   
	}   
	else
	{   
		nTextFlags |= DT_RIGHT;   
	} 

	if ( windowstyle & SS_LEFTNOWORDWRAP )
	{
		nTextFlags |= DT_VCENTER | DT_SINGLELINE | DT_LEFT; 
	}

	//����Ϊ͸��   
	dc.SetBkMode(TRANSPARENT);   

	//�����ı�   
	if(m_Type == CLabel::Text && !m_bFlash)   
	{   
	}   
	else   
	{   
		//�����ı���ɫ   
		if(windowstyle & WS_DISABLED)   
		{   
			dc.SetTextColor(GetSysColor(COLOR_INACTIVECAPTIONTEXT));   
		}   
		else   
		{   
			dc.SetTextColor(m_crText);   
		}   

		//ѡ������   
		dc.SelectObject(m_textfont);   

		//��ȡ��ʾ�ı�   
		GetWindowText(strText);   
		dc.DrawText(strText,&clientrect,nTextFlags);   
	}   
}   

//���Ʊ���   
void CLabel::DrawBackground(CDC &dc, CRect &clientrect)   
{     
	//CBrush bkbrush(RGB(0,0,0));//��ˢ   

	////���Ʊ���   
	//bkbrush.DeleteObject();   
	//if(m_Type == CLabel::Background && !m_bFlash)   
	//{   
	//	bkbrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));   
	//}   
	//else   
	//{   
	//	bkbrush.CreateSolidBrush(m_crBkground);   
	//}   
	//dc.FillRect(&clientrect,&bkbrush);   
	//bkbrush.DeleteObject();   

	CBrush *pbkbrush = NULL;
	
	if ( m_Type == CLabel::Background && !m_bFlash )
	{
		pbkbrush = new CBrush;
		pbkbrush->CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	}
	else if ( m_Type == CLabel::None )
	{
		pbkbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	}
	else
	{
		pbkbrush = new CBrush;
		pbkbrush->CreateSolidBrush(m_crBkground);
	}
	dc.FillRect(&clientrect,pbkbrush);
	pbkbrush->DeleteObject();
	pbkbrush = NULL;
}
