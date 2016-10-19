
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYSLIDER_H__C9956441_F84B_45CF_A08D_B46B11C7ADB9__INCLUDED_)
#define AFX_MYSLIDER_H__C9956441_F84B_45CF_A08D_B46B11C7ADB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySlider.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySlider window
class CMySlider : public CButton
{
    // Construction
public:
    CMySlider();

    // Attributes
private:
private:
    struct MySliderParameter
    {
        UINT		m_uFrame ;				// the slider frame
        char		m_strSliderID[64] ;			// the sliderID
        CPoint		m_IntPoint ;			// the slider comparative to the parent dialog point
        BOOL		m_bBtnDown ;			// the slider is down or not
        WORD		m_wdSliderWidth ;		// the slider's width
        WORD		m_wdSliderMaxValue ;	// the slider's max value
        int			m_nSliderPos ;			// the slider's postion
        int			m_nSliderCurPos ;		// the slider's current position
        CPoint		m_MousePoint ;			// the slider's mouse point
        BOOL		m_bV ;
    } m_SliPar ;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMySlider)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void SetSliderValue ( int nValue );
    BOOL Init( int xPos, int yPos, char* SliderID, BOOL bType = false );
    int GetSliderValue ();
    void SetMaxValue ( int nValue );
    void SetSliderWidth ( int nWidth );
    void Show ( int x, int y );
    virtual ~CMySlider();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMySlider)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDER_H__C9956441_F84B_45CF_A08D_B46B11C7ADB9__INCLUDED_)
