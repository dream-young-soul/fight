
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYPROGRESS_H__5EB25FBE_E467_490D_8D85_C86A9AA0B826__INCLUDED_)
#define AFX_MYPROGRESS_H__5EB25FBE_E467_490D_8D85_C86A9AA0B826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProgress window
const int		MY_PROGRESS_V		=	0;
const int		MY_PROGRESS_HLEFT	=	10;
const int		MY_PROGRESS_HRIGHT	=	11;
const int		MY_PROGRESS_BLOCK   =	26;

class CMyProgress : public CProgressCtrl
{
    // Construction
public:
    CMyProgress();

    // Attributes
private:
    struct MyProgressParameter
    {
        char		m_strProgressID[64] ;	// the progress ID
        BOOL		m_bBorder ;			// the border
        CPoint		m_BorderOffset ;	// the border's offset
        CPoint		m_IntPoint ;		// the progress comparative to the parent dialog point
        int			m_nWidth ;			// the progress bitmap's width
        int			m_nHWidth ;			// the progress h frame's width
        int			m_nHeight ;			// the progress bitmap's height
        float		m_nStep ;			// the progress step
        float		m_fAddStep ;		// the progress add step
        BYTE		m_btType ;			// the progress type
        int			m_nMax ;			// the progress's max value
        int			m_nUpper ;			// the progress's upper
        int			m_nLower ;			// the progress's lower
        BYTE		m_btShowMode ;		// the progress's mode
    } m_PrgPar ;

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyProgress)
    //}}AFX_VIRTUAL

    // Implementation
public:
    void ShowMaxPercent(int x, int y , int nMax, int nPercent, int nWidth = 39);
    void SetProgressID(char* ProgressID);
    char* GetProgressID();
    void SetWidthAndHeight( int nWidth, int nHeight, int nHWidth );
    void SetShowMode ( BYTE btShowMode );
    void GetRange ( int& nUpper, int& nLower );
    void SetRange( int nUpper, int nLower );
    void ResetValue( int nLower, int nUpper );
    BOOL Init ( int xPos, int yPos, char* ProgressID, int nLower, int nUpper, BYTE btType, BOOL bBorder = false, CPoint BorderPnt = CPoint ( 0, 0 ) );
    void Show ( int x, int y, int nPos, int nCurpos, UCHAR ucAlpha = 255);
    virtual ~CMyProgress();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyProgress)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESS_H__5EB25FBE_E467_490D_8D85_C86A9AA0B826__INCLUDED_)
