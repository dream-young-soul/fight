
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_STATIC		0
#define MY_PICTURE		1
#define MY_MULTILINE	2

const int POS_LEFT	 = 0;
const int POS_MIDDLE = 1;
const int POS_RIGHT  = 2;
/////////////////////////////////////////////////////////////////////////////
// CMyStatic

CMyStatic::CMyStatic()
{
    m_dwFontColor = 0xffffff ;
    m_bImageLoad = false ;
    m_nLightType = -1 ;
    m_uFrame = 0 ;
    m_nStaticWidth = 0;
    m_nFontType = 0;
    m_nStaticPosType = POS_LEFT;
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
    //{{AFX_MSG_MAP(CMyStatic)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatic message handlers

BOOL CMyStatic::Init(int xPos, int yPos, BYTE nType, char* PictureID)
{
    CRect rect ;
    CMyStatic::GetWindowRect ( rect ) ;
    m_IntPoint.x = rect.left - xPos ;
    m_IntPoint.y = rect.top - yPos ;
    m_nStaticWidth = rect.Width();
    m_nNum =(int)( ( float ) ( rect.right - rect.left ) / 7.5f);
    m_btType = nType ;
    if ( nType == MY_PICTURE )
    {
        strcpy ( m_strPictureID, PictureID ) ;
    }
    CMyStatic::ShowWindow ( SW_HIDE ) ;
    return true ;
}

void CMyStatic::Show(int x, int y)
{
    switch ( m_btType )
    {
    case MY_STATIC:
        {
            char Static[1024] ;
            CMyStatic::GetWindowText ( Static, 1024 ) ;
            int posBX = 0;
            CMySize psizeFont;
            CMyBitmap::GetFontSize(psizeFont);
            switch(m_nStaticPosType)
            {
            case POS_LEFT:
                posBX = 0;
                break;
            case POS_MIDDLE:
                {
                    int nLength = strlen(Static) * (psizeFont.iWidth + m_nFontType);
                    posBX = (m_nStaticWidth - nLength) / 2;
                }
                break;
            case POS_RIGHT:
                {
                    int nLength = strlen(Static) * (psizeFont.iWidth + m_nFontType);
                    posBX = m_nStaticWidth - nLength;
                }
                break;
            default:
                break;
            }
            switch(m_nFontType)
            {
            case 0:
                CMyBitmap::ShowString ( m_IntPoint.x + x + posBX,
                                        m_IntPoint.y + y,
                                        m_dwFontColor,
                                        Static, NULL, 0) ;
                break;
            case 1:
                CMyBitmap::ShowString ( m_IntPoint.x + x + posBX,
                                        m_IntPoint.y + y,
                                        m_dwFontColor,
                                        Static, (char*)g_objGameDataSet.GetStr(10049), 14) ;
                break;
            case 2:
                CMyBitmap::ShowString ( m_IntPoint.x + x + posBX + 1,
                                        m_IntPoint.y + y,
                                        m_dwFontColor,
                                        Static, (char*)g_objGameDataSet.GetStr(10049), 18) ;
                CMyBitmap::ShowString ( m_IntPoint.x + x + posBX,
                                        m_IntPoint.y + y,
                                        m_dwFontColor,
                                        Static, (char*)g_objGameDataSet.GetStr(10049), 18) ;
                break;
            default:
                break;
            }
            // show the light type
            if ( m_nLightType != -1 )
            {
                char strLight[32] ;
                sprintf ( strLight, "Light%d", m_nLightType ) ;
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                strLight,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni != NULL )
                    ShowAni->Show ( m_uFrame,
                                    m_IntPoint.x + x - 30,
                                    m_IntPoint.y + y - 6) ;
            }
        }
        break ;
    case MY_MULTILINE:
        ShowMultiline(m_IntPoint.x + x, m_IntPoint.y + y);
        break;
    case MY_PICTURE:
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        m_strPictureID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            if (m_bImageLoad )
                ShowAni->Show ( m_uFrame,
                                m_IntPoint.x + x,
                                m_IntPoint.y + y);
        }
        break ;
    }
}

void CMyStatic::SetFontColor(DWORD dwColor)
{
    m_dwFontColor = dwColor ;
}

void CMyStatic::InsertImage()
{
    m_bImageLoad = true ;
}

void CMyStatic::RemoveImage()
{
    m_bImageLoad = false ;
}

void CMyStatic::SetLightType( int nType )
{
    m_nLightType = nType ;
}

void CMyStatic::SetCurFrame(UINT uFrame)
{
    m_uFrame = uFrame ;
}

void CMyStatic::ShowMultiline(int nStartX, int nStartY)
{
    //------------------------------------------------------
    char Static[1024] ;
    CMyStatic::GetWindowText ( Static, 1024 ) ;
    if (!Static || strlen(Static) == 0)
    {
        return;
    }
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    int nMaxLen = m_nStaticWidth / psizeFont.iWidth;
    int nDescLine = 0 ;
    int nCharIndex[10];
    nCharIndex[0] = 0;
    int nSingle = 0;
    int nLineNum = 0;
    int i = 0;
    for ( ; i < (int)strlen ( Static ); i++ )
    {
        if ( CMyBitmap::IsDBCSLeadByte( Static, i ) )
        {
            nLineNum += 2;
            i++;
        }
        else
        {
            nSingle++;
            nLineNum++;
        }
        if ( nLineNum >= nMaxLen )
        {
            //----------Begin the english version's process(jump line)----------//
            if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
            {
                // Search the last word
                for ( int j = i; j >= 0; j-- )
                {
                    if ( Static[j] == '~' )
                    {
                        i -= i - j;
                        break;
                    }
                }
            }
            //----------End the english version's process(jump line)----------//
            if ( nSingle % 2 == 0 )
            {
                nSingle = 0;
                nLineNum = 0;
                nDescLine++;
                nCharIndex[nDescLine] = i + 1;
            }
            else
            {
                nDescLine++;
                nCharIndex[nDescLine] = i - ( nLineNum - nMaxLen );
                i -= nLineNum - 2 ;
                nSingle = 0;
                nLineNum = 0;
            }
        }
    }
    if ( nLineNum < nMaxLen && nLineNum != 0 )
    {
        nDescLine++ ;
        nCharIndex[nDescLine] = i;
    }
    int nPosX  = nStartX;
    int nPosY  = nStartY;
    char strTemp[128] ;
    int nOffset = 0;
    for (int i = 0; i < nDescLine; i++ )
    {
        strncpy ( strTemp, Static + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
        strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
        CMyBitmap::ShowString (nPosX,
                               nPosY + nOffset,
                               m_dwFontColor,
                               CMyShellApp::StringChange ( strTemp, 1 ) ) ;
        nOffset += psizeFont.iHeight + 2;
    }
    //------------------------------------------------------
}
