
//**********************************************************
// 代码编辑器
//**********************************************************

#if !defined(AFX_MYCHECK_H__2A2CC6D4_5479_4754_A9E3_1D533D17CD04__INCLUDED_)
#define AFX_MYCHECK_H__2A2CC6D4_5479_4754_A9E3_1D533D17CD04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCheck window
class CMyCheck : public CButton
{
    // Construction
public:
    CMyCheck();

    // Attributes
private:
    bool	m_bInJustNow;		// 鼠标是否刚刚进入
    DWORD	m_dwInTime;			// 鼠标进入的时间
    char m_szTip[32];
    DWORD m_dwTipColor;
    struct MyCheckParameter
    {
        UINT		m_uFrame ;			// the frame
        char		m_strCheckID[64] ;	// the check id
        CPoint		m_IntPoint ;		// the check comparative to the parent dialog point
        BYTE		m_btType ;			// the check type
        BYTE		m_btCurrent ;		// the check current type
        CRect m_ShowRect ;				// the bmp show rect
        int m_nWidth ;					// the check width
        int m_nHeight ;					// the check height
        BOOL m_bStaticMode;
        DWORD m_dwStaticColor;
    } m_ChkPar ;

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyCheck)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void ShowTip(int x, int y);
    void SetTipShow(char* szTip, DWORD dwColor = 0xffffff);
    void SetStaticMode( BOOL bStatic = true, DWORD dwColor = 0xffffff);
    void SetCheckValue( BYTE btValue );
    BYTE GetCheckValue();
    void Show ( int x, int y);
    BOOL IsMouseFocus ();
    BOOL Init ( int xPos, int yPos, char* CheckID, BYTE btType );
    void ReInit(int xPos, int yPos);
    virtual ~CMyCheck();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyCheck)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCHECK_H__2A2CC6D4_5479_4754_A9E3_1D533D17CD04__INCLUDED_)
