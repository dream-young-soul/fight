
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyProgress.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProgress

CMyProgress::CMyProgress()
{
    m_PrgPar.m_bBorder = false ;
    m_PrgPar.m_btShowMode = 0 ;
}

CMyProgress::~CMyProgress()
{
}


BEGIN_MESSAGE_MAP(CMyProgress, CProgressCtrl)
    //{{AFX_MSG_MAP(CMyProgress)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProgress message handlers
void CMyProgress::ShowMaxPercent(int x, int y, int nMax, int nPercent, int nWidth)
{
    if (m_PrgPar.m_btType != MY_PROGRESS_V
            ||  nPercent <= 0 || nPercent >= 1000)
    {
        return;
    }
    int upLeft = (int)( m_PrgPar.m_nHeight - ( nMax * m_PrgPar.m_nStep ));
    int nPosX = 0;
    int nLength = nWidth;
    int nPosY = abs(nWidth - upLeft);
    nLength =(int)(__min(nWidth, sqrt(float(nWidth * nWidth - nPosY * nPosY)))) ;
    nPosX = nWidth - nLength;
    RECT rect = { 0,
                  0,
                  nLength,
                  4
                } ;
    CAni*	ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                      "StatusLine",
                      EXIGENCE_IMMEDIATE ) ;
    if ( ShowAni != NULL )
    {
        ShowAni->ShowEx ( 0,
                          m_PrgPar.m_IntPoint.x + x + nPosX,
                          m_PrgPar.m_IntPoint.y + y + upLeft,
                          &rect, nLength, 4);
    }
}


void CMyProgress::Show(int x, int y, int nPos, int nCurpos, UCHAR ucAlpha)
{
    if ( nCurpos > m_PrgPar.m_nLower )
    {
        nCurpos = m_PrgPar.m_nLower ;
    }
    if ( nPos > m_PrgPar.m_nLower )
    {
        nPos = m_PrgPar.m_nLower ;
    }
    CAni* ShowAni ;
    switch  ( m_PrgPar.m_btType )
    {
    case MY_PROGRESS_V:
        {
            switch ( m_PrgPar.m_btShowMode )
            {
            case 0:
                {
                    if ( nPos <= nCurpos )
                    {
                        nCurpos = nPos ;
                        if ( nCurpos == 0 )
                        {
                            break ;
                        }
                        int upleft = (int)(m_PrgPar.m_nHeight - ( nPos * m_PrgPar.m_nStep ));
                        RECT rect = { 0,
                                      upleft ,
                                      m_PrgPar.m_nWidth,
                                      m_PrgPar.m_nHeight
                                    } ;
                        ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                                                m_PrgPar.m_strProgressID,
                                                                EXIGENCE_IMMEDIATE ) ;
                        if ( ShowAni != NULL )
                            ShowAni->ShowEx ( 0,
                                              m_PrgPar.m_IntPoint.x + x,
                                              m_PrgPar.m_IntPoint.y + y + upleft,
                                              &rect,
                                              m_PrgPar.m_nWidth,
                                              m_PrgPar.m_nHeight - upleft,
                                              ucAlpha) ;
                    }
                    else
                    {
                        int upleft = (int)(m_PrgPar.m_nHeight - ( nPos * m_PrgPar.m_nStep ));
                        RECT rect = { 0,
                                      upleft ,
                                      m_PrgPar.m_nWidth,
                                      m_PrgPar.m_nHeight
                                    } ;
                        ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                                                m_PrgPar.m_strProgressID,
                                                                EXIGENCE_IMMEDIATE ) ;
                        if ( ShowAni != NULL )
                        {
                            ShowAni->ShowEx ( 0,
                                              m_PrgPar.m_IntPoint.x + x,
                                              m_PrgPar.m_IntPoint.y + y + upleft,
                                              &rect,
                                              m_PrgPar.m_nWidth,
                                              (DWORD)(nPos * m_PrgPar.m_nStep) ,
                                              99);
                            nCurpos +=(int)( m_PrgPar.m_fAddStep / m_PrgPar.m_nStep + 0.5f);
                            upleft =(int)( m_PrgPar.m_nHeight - ( nCurpos * m_PrgPar.m_nStep ));
                            rect.top = upleft ;
                            ShowAni->ShowEx ( 0,
                                              m_PrgPar.m_IntPoint.x + x,
                                              m_PrgPar.m_IntPoint.y + y + upleft,
                                              &rect,
                                              m_PrgPar.m_nWidth,
                                              ( int ) ( nCurpos * m_PrgPar.m_nStep + 0.5f ) ,
                                              ucAlpha);
                        }
                    }
                }
                break ;
            case 1:
                {
                    if ( nPos <= nCurpos )
                    {
                        if ( nPos == 0 )
                        {
                            break ;
                        }
                        int upleft = (int)(m_PrgPar.m_nHeight - ( nPos * m_PrgPar.m_nStep ));
                        RECT rect = { 0,
                                      upleft ,
                                      m_PrgPar.m_nHWidth,
                                      m_PrgPar.m_nHeight
                                    } ;
                        ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                                                m_PrgPar.m_strProgressID,
                                                                EXIGENCE_IMMEDIATE ) ;
                        if ( ShowAni != NULL )
                            ShowAni->ShowEx ( 2,
                                              m_PrgPar.m_IntPoint.x + x,
                                              m_PrgPar.m_IntPoint.y + y + upleft,
                                              &rect,
                                              m_PrgPar.m_nHWidth,
                                              m_PrgPar.m_nHeight - upleft ,
                                              ucAlpha);
                    }
                    else
                    {
                        int upleft = (int)(m_PrgPar.m_nHeight - ( nPos * m_PrgPar.m_nStep ));
                        RECT rect = { 0,
                                      upleft ,
                                      m_PrgPar.m_nHWidth,
                                      m_PrgPar.m_nHeight
                                    } ;
                        ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                                                m_PrgPar.m_strProgressID,
                                                                EXIGENCE_IMMEDIATE ) ;
                        if ( ShowAni != NULL )
                        {
                            /*							ShowAni->ShowEx ( 3,
                            											  m_PrgPar.m_IntPoint.x + x,
                            											  m_PrgPar.m_IntPoint.y + y + upleft,
                            											  &rect,
                            											  m_PrgPar.m_nHWidth,
                            											  nPos * m_PrgPar.m_nStep );*/
                            upleft = (int)(m_PrgPar.m_nHeight - ( nCurpos * m_PrgPar.m_nStep ));
                            rect.top = upleft ;
                            ShowAni->ShowEx ( 2,
                                              m_PrgPar.m_IntPoint.x + x,
                                              m_PrgPar.m_IntPoint.y + y + upleft,
                                              &rect,
                                              m_PrgPar.m_nHWidth,
                                              ( int ) ( nCurpos * m_PrgPar.m_nStep + 0.5f ) ,
                                              ucAlpha);
                        }
                    }
                }
                break ;
            case 2:
                {
                    ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                                            m_PrgPar.m_strProgressID,
                                                            EXIGENCE_IMMEDIATE ) ;
                    if ( ShowAni != NULL )
                        ShowAni->Show ( 2,
                                        m_PrgPar.m_IntPoint.x + x,
                                        m_PrgPar.m_IntPoint.y + y ,
                                        ucAlpha) ;
                }
            }
        }
        break ;
    case MY_PROGRESS_HLEFT:
        {
            if ( nPos > 0 )
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                m_PrgPar.m_strProgressID,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni )
                {
                    int left = (int)(( float )nPos * m_PrgPar.m_nStep) ;
                    left = ( left == 0 ) ? 1 : left ;
                    RECT rect = { 0,
                                  0,
                                  left,
                                  m_PrgPar.m_nHeight
                                } ;
                    ShowAni->ShowEx ( 0,
                                      m_PrgPar.m_IntPoint.x + x,
                                      m_PrgPar.m_IntPoint.y + y,
                                      &rect,
                                      left,
                                      m_PrgPar.m_nHeight ) ;
                }
            }
            else if ( nPos < 0 )
            {
                int left = - (int)(( float )nPos * m_PrgPar.m_nStep );
                CMyBitmap::ShowBlock ( m_PrgPar.m_IntPoint.x + x,
                                       m_PrgPar.m_IntPoint.y + y,
                                       m_PrgPar.m_IntPoint.x + x + left,
                                       m_PrgPar.m_IntPoint.y + y + m_PrgPar.m_nHeight,
                                       0xffff0000) ;
            }
        }
        break ;
    case MY_PROGRESS_HRIGHT:
        {
            if ( nPos > 0 )
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                m_PrgPar.m_strProgressID,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni )
                {
                    int left = (int)(( float )nPos * m_PrgPar.m_nStep );
                    int nNum = left / MY_PROGRESS_BLOCK ;
                    int nLeft = left % MY_PROGRESS_BLOCK ;
                    RECT rect = { 0,
                                  0,
                                  nLeft,
                                  m_PrgPar.m_nHeight
                                } ;
                    if ( nLeft != 0 )
                    {
                        ShowAni->ShowEx( 0,
                                         m_PrgPar.m_IntPoint.x + x + nNum * MY_PROGRESS_BLOCK,
                                         m_PrgPar.m_IntPoint.y + y,
                                         &rect,
                                         nLeft,
                                         m_PrgPar.m_nHeight ) ;
                    }
                    for ( int i = 0; i < nNum; i++ )
                    {
                        ShowAni->Show( 0,
                                       m_PrgPar.m_IntPoint.x + x + i * MY_PROGRESS_BLOCK,
                                       m_PrgPar.m_IntPoint.y + y ) ;
                    }
                }
            }
        }
        break ;
    }
    if ( m_PrgPar.m_bBorder )
        g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                                      m_PrgPar.m_strProgressID,
                                      EXIGENCE_NORMAL )->Show ( 1,
                                              m_PrgPar.m_IntPoint.x + x - m_PrgPar.m_BorderOffset.x,
                                              m_PrgPar.m_IntPoint.y + y - m_PrgPar.m_BorderOffset.y,
                                              ucAlpha) ;
}

BOOL CMyProgress::Init(int xPos, int yPos, char* ProgressID, int nLower, int nUpper, BYTE btType, BOOL bBorder, CPoint BorderPnt )
{
    CRect rect ;
    CMyProgress::GetWindowRect ( rect ) ;
    m_PrgPar.m_IntPoint.x = rect.left - xPos ;
    m_PrgPar.m_IntPoint.y = rect.top - yPos ;
    CMyProgress::SetRange ( nLower, nUpper ) ;
    if ( bBorder )
    {
        m_PrgPar.m_bBorder = true ;
        m_PrgPar.m_BorderOffset = BorderPnt ;
    }
    m_PrgPar.m_btType = btType ;
    switch ( btType )
    {
    case MY_PROGRESS_V:
        strcpy ( m_PrgPar.m_strProgressID, ProgressID ) ;
        break ;
    case MY_PROGRESS_HLEFT:
        strcpy ( m_PrgPar.m_strProgressID, ProgressID ) ;
        m_PrgPar.m_nWidth = rect.right - rect.left ;
        m_PrgPar.m_nHeight = rect.bottom - rect.top;
        m_PrgPar.m_nStep = ( float )m_PrgPar.m_nWidth / ( nUpper - nLower ) ;
        break ;
    case MY_PROGRESS_HRIGHT:
        strcpy ( m_PrgPar.m_strProgressID, ProgressID ) ;
        m_PrgPar.m_nWidth = rect.right - rect.left ;
        m_PrgPar.m_nHeight = rect.bottom - rect.top;
        m_PrgPar.m_nStep = ( float )m_PrgPar.m_nWidth / ( nUpper - nLower ) ;
        break ;
    }
    return true ;
}

void CMyProgress::ResetValue(  int nLower, int nUpper )
{
    CRect rect ;
    CMyProgress::GetWindowRect ( rect ) ;
    CMyProgress::SetRange ( nLower, nUpper ) ;
    switch ( m_PrgPar.m_btType )
    {
    case MY_PROGRESS_V:
        m_PrgPar.m_nStep = 0 ;
        if ( ( nUpper - nLower ) != 0 )
        {
            if ( ( nUpper - nLower ) < 100 )
            {
                m_PrgPar.m_fAddStep = ( float ) m_PrgPar.m_nHeight / ( nUpper - nLower ) ;
            }
            else
            {
                m_PrgPar.m_fAddStep = ( float ) m_PrgPar.m_nHeight / 100.0f ;
            }
            m_PrgPar.m_nStep = ( float )m_PrgPar.m_nHeight / ( nUpper - nLower ) ;
        }
        break ;
    case MY_PROGRESS_HLEFT:
        m_PrgPar.m_nStep = 0 ;
        if ( nUpper - nLower != 0 )
        {
            m_PrgPar.m_nStep = ( float )m_PrgPar.m_nWidth / ( nUpper - nLower ) ;
        }
        break ;
    case MY_PROGRESS_HRIGHT:
        m_PrgPar.m_nStep = 0 ;
        if ( nUpper - nLower != 0 )
        {
            m_PrgPar.m_nStep = ( float )m_PrgPar.m_nWidth / ( nUpper - nLower ) ;
        }
        break ;
    }
}


void CMyProgress::SetRange(int nUpper, int nLower)
{
    m_PrgPar.m_nUpper = nUpper ;
    m_PrgPar.m_nLower = nLower ;
}

void CMyProgress::GetRange(int& nUpper, int& nLower)
{
    nUpper = m_PrgPar.m_nUpper ;
    nLower = m_PrgPar.m_nLower ;
}

void CMyProgress::SetShowMode(BYTE btShowMode)
{
    m_PrgPar.m_btShowMode = btShowMode ;
}

void CMyProgress::SetWidthAndHeight(int nWidth, int nHeight, int nHWidth)
{
    m_PrgPar.m_nWidth = nWidth ;
    m_PrgPar.m_nHeight = nHeight ;
    m_PrgPar.m_nHWidth = nHWidth ;
}

char* CMyProgress::GetProgressID()
{
    return m_PrgPar.m_strProgressID;
}

void CMyProgress::SetProgressID(char* ProgressID)
{
    if (ProgressID && strlen(ProgressID) > 0)
    {
        strcpy(m_PrgPar.m_strProgressID, ProgressID);
    }
}
