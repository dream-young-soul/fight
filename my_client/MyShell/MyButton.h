
//**********************************************************
// 代码编辑器
//**********************************************************

#if !defined(AFX_NEWBUTTON_H__210E1717_078A_44B7_8C57_C239445F919F__INCLUDED_)
#define AFX_NEWBUTTON_H__210E1717_078A_44B7_8C57_C239445F919F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window
class CMyButton : public CButton
{
    // Construction
public:
    CMyButton();

    // Attributes
public:
    CPoint m_BtnPnt ;
    CPoint m_IntPnt ;
    int	   m_nTipPosX;
    int    m_nTipPosY;
    BOOL   m_bStaticMode;
    DWORD  m_dwStaticColor;
    CRect m_ShowRect ;
    int m_nWidth ;
    int m_nHeight ;
    UINT m_uFrame ;
    BYTE m_btOwnerID ;
    int m_nButtonID ;
    char m_strButtonID[64] ;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyButton)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void SetTipPos(int x, int y);
    void SetTipShow(char* szTip, DWORD dwColor = 0xffffff);
    void ShowTip();
    BOOL IsMouseFocus();
    void ReInit( int xPos, int yPos );
    BOOL IsStaticMode() {return m_bStaticMode;}
    void SetStaticMode(BOOL bStatic = true, DWORD dwColor = 0xffffff) ;
    void ChangeImage ( char* ButtonID );
    void SetCurFrame(UINT uFrame);
    BOOL Init ( int xPos, int yPos, char* ButtonID, BYTE btOwnerID = 0, int nImageID = 0 );
    void Show ( int x, int y );
    virtual ~CMyButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyButton)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    char m_szTip[32];
    DWORD m_dwTipColor;
    bool	m_bInJustNow;		// 鼠标是否刚刚进入
    DWORD	m_dwInTime;			// 鼠标进入的时间
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWBUTTON_H__210E1717_078A_44B7_8C57_C239445F919F__INCLUDED_)
