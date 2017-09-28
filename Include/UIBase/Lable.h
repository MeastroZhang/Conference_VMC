/************************************************************  
������ CLabel 
����������CStatic��չ:֧�����塢�ֺš��Ӵ֡�б�塢�»�������; 
		  ֧�־�̬���Ͷ�̬��ꣻ֧�ֳ�������ʽ��ʾ��֧���ı� 
		  ��˸�ͱ�����˸��֧����˸������ã� 
���ߣ����� 
�汾��1.0.0 
������ڣ�2005/06/12 
�޸ļ�¼��										 
�޸��� felix     �޸�ʱ��      �°汾��      �޸�����			 
************************************************************/ 
#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_) 
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_ 
 
#if _MSC_VER >= 1000 
#pragma once 
#endif // _MSC_VER >= 1000 
// Label.h : header file 
 
class CLabel : public CStatic 
{ 
public: 
	enum FlashType {None, Text, Background, ErrorType };//��˸���� 
	enum TIMER{IDT_TIMER1=0x1001};//��ʱ�� 
 
	CLabel(void); 
	CLabel(const CLabel& p);//�������캯�� 
	CLabel& operator=(const CLabel& p);//��ֵ���캯�� 
 
	virtual ~CLabel(void); 
 
	 
	//���ù��,��һ��Ϊĳ����Դ��ID�ţ��ڶ���Ϊ����ļ���(�ڶ���֧�ֶ������) 
	BOOL SetCursor(IN UINT nIDCursor); 
	BOOL SetCursor(IN LPCTSTR lpCursorFileName); 
 
	//��ָ��ʱ����������˸��ֹͣ��˸��������˸����(��˸�ı����Ǳ���) 
	BOOL StartFlash(IN UINT iIntervalTime); 
	void StopFlash(void); 
	virtual BOOL SetFlashType(IN int iFlashType); 
 
	//��ȡ��˸���� 
	int	GetFlashType(void); 
 
	//���ñ���ɫ��ǰ��ɫ 
	void SetBkColor(IN COLORREF crBkgnd); 
	void SetTextColor(IN COLORREF crText); 
 
	//��ȡ����ɫ��ǰ��ɫ 
	COLORREF GetBkColor(void); 
	COLORREF GetTextColor(void); 
 
	//������ʾ�ı� 
	void SetText(IN const CString& strText); 
 
	//�����Ƿ�Ӵ֡��»��ߡ�б�塢�������ơ��ֺ� 
	BOOL SetFontBold(IN BOOL bBold); 
	BOOL SetFontName(IN const CString& strFont); 
	BOOL SetFontUnderline(IN BOOL bSet); 
	BOOL SetFontItalic(IN BOOL bSet); 
	BOOL SetFontSize(IN long nSize); 
 
	//��ȡ�Ƿ�Ӵ֡��»��ߡ�б�塢�ֺš������� 
	BOOL IsFontBold(void); 
	BOOL IsFontItalic(void); 
	BOOL IsFontUnderline(void); 
	long GetFontSize(void); 
	CString GetFontName(void); 
	LOGFONT GetLogFont(void); /*Add:Felix 12/27/2011*/
 
	//�Ƿ��������ı���ʾ 
	void SetLink(BOOL bLink, COLORREF crLinkText = RGB(0,0,255)); 
 
	//�Ƿ����� 
	BOOL	IsLink(void); 
	 
 
private: 
	//���Ʊ������ı� 
	void DrawBackground(CDC& dc,CRect& clientrect); 
	void DrawText(CDC& dc,CRect& clientrect); 
 
	BOOL CreateFont(void);//�������� 
 
	CFont m_textfont;//���� 
 
	COLORREF    m_crOldText;//ԭ�ı�ɫ
	COLORREF	m_crText;//�ı�ɫ 
	COLORREF	m_crBkground;//����ɫ 
 
	LOGFONT		m_lf;//����ṹ 
 
	BOOL		m_bLink;//�Ƿ������� 
 
	int			m_Type;//��˸���� 
	BOOL		m_bFlash;//�Ƿ���˸ 
	 
	HCURSOR		m_hCursor;//����� 
 
	BOOL		m_bStartTimerFlag;//������ʱ����־ 
 
	//{{AFX_VIRTUAL(CLabel) 
	//}}AFX_VIRTUAL 
protected: 
	//{{AFX_MSG(CLabel) 
	afx_msg void OnTimer(UINT nIDEvent); 
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message); 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); 
	afx_msg void OnPaint(void); 
	afx_msg void OnDestroy(void); 
	//}}AFX_MSG 
 
	DECLARE_MESSAGE_MAP() 
}; 
 
///////////////////////////////////////////////////////////////////////////// 
 
//{{AFX_INSERT_LOCATION}} 
// Microsoft Developer Studio will insert additional declarations immediately before the previous line. 
 
#endif // !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_) 