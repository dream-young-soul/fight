
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYSTATIC_H__3ED7AB8A_9887_4F49_9ED6_3EBCB0568423__INCLUDED_)
#define AFX_MYSTATIC_H__3ED7AB8A_9887_4F49_9ED6_3EBCB0568423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatic.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMyStatic window
class CMyStatic : public CStatic
{
    // Construction
public:
    CMyStatic();
    CPoint		m_IntPoint ;			// the static comparative to the parent dialog point
    // Attributes
private:
    int			m_nFontType;
    int			m_nStaticPosType;		// the static's begin pos
    DWORD		m_dwFontColor ;			// the static's font color
    BYTE		m_btType ;				// the static's type
    BOOL		m_bImageLoad ;			// the picture box's image is load
    char		m_strPictureID[64] ;	// the picture id
    int			m_nNum ;				// the static's width
    int			m_nLightType ;			// the light behide the text
    UINT		m_uFrame ;				// the image current frame
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyStatic)
    //}}AFX_VIRTUAL

    // Implementation
public:
    int  m_nStaticWidth;
    void SetFontType(int nType )	{m_nFontType = nType;}
    void SetStaticPosType(int nType) {m_nStaticPosType = nType;}
    void ShowMultiline(int nStartX, int nStartY);
    void SetCurFrame ( UINT uFrame );
    void SetLightType( int nType );
    void RemoveImage();
    void InsertImage ( );
    void SetFontColor ( DWORD dwColor );
    void Show ( int x, int y );
    BOOL Init ( int xPos, int yPos, BYTE nType = 0, char* PictureID = NULL );
    virtual ~CMyStatic();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyStatic)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATIC_H__3ED7AB8A_9887_4F49_9ED6_3EBCB0568423__INCLUDED_)
