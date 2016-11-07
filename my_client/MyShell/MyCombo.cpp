
//**********************************************************
// 代码编辑器
//**********************************************************

// MyCombo.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCombo

CMyCombo::CMyCombo()
{
    m_CobPar.m_btRightLeft = 5 ;
    m_CobPar.m_btEndCharPos = 10 ;
    m_CobPar.m_bBorder = false ;
    m_CobPar.m_bChinese = true;
}

CMyCombo::~CMyCombo()
{
    m_CobPar.m_Bmp.Destroy () ;
}


BEGIN_MESSAGE_MAP(CMyCombo, CEdit)
    //{{AFX_MSG_MAP(CMyCombo)
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCombo message handlers
void CMyCombo::Show(int x, int y)
{
    m_CobPar.m_Pnt.x = m_CobPar.m_IntPnt.x + x ;
    m_CobPar.m_Pnt.y = m_CobPar.m_IntPnt.y + y ;
    if ( m_CobPar.m_bBorder )
        m_CobPar.m_Bmp.Show ( m_CobPar.m_Pnt.x,
                              m_CobPar.m_Pnt.y) ;
    CMyCombo::ShowChar();
}

void CMyCombo::ShowChar()
{
	
	CString strTemp; 
	const int nLineNum = 0; //行索引
	int nLineLength = CMyCombo::LineLength(CMyCombo::LineIndex(nLineNum)) + 1;
	CMyCombo::GetLine(nLineNum,strTemp.GetBufferSetLength(nLineLength));
	strTemp.ReleaseBuffer(); 
	char strChar[1024] = {0};
	strcpy(strChar,(LPCTSTR)strTemp);
	
    
	if(strlen(strChar) == 0)
	//if ( 0 >= CMyCombo::GetLine ( 0, strChar, sizeof( strChar ) ) )//GetLine 后面会出现乱码 改用CString 代替
    {
        // Show the cursor pos
        if ( ::timeGetTime() / 500 % 2 == 0 && this->GetFocus() == this )
        {
  /*          CMyBitmap::ShowBlock ( m_CobPar.m_Pnt.x,
                                   m_CobPar.m_Pnt.y,
                                   m_CobPar.m_Pnt.x + 2,
                                   m_CobPar.m_Pnt.y + CMyBitmap::GetFontSize() + 4,
                                   0xffffffff );*/
			CMyBitmap::ShowString(m_CobPar.m_Pnt.x - 2,
                                   m_CobPar.m_Pnt.y,
								    0xffffffff,
									"|");
        }
        return;
    }
    int nSelStart = 0, nSelEnd = 0;
    CMyCombo::GetSel( nSelStart, nSelEnd );
    m_CobPar.m_nSelNum = nSelEnd - nSelStart;
    if ( nSelStart == nSelEnd )
    {
        m_CobPar.m_nLastSelEnd = nSelEnd;
    }
    int nCursor = nSelStart;
    CPoint pntCursor = this->PosFromChar( nSelStart );
    if ( pntCursor.x < 0 && pntCursor.x != -1 )
    {
        nCursor = nSelEnd;
        pntCursor = this->PosFromChar( nSelEnd );
    }
    else if ( pntCursor.x > 0 && nSelEnd > m_CobPar.m_nLastSelEnd )
    {
        nCursor = nSelEnd;
        pntCursor = this->PosFromChar( nSelEnd );
    }
    if ( nSelEnd <= m_CobPar.m_nLastSelEnd )
    {
        m_CobPar.m_nLastSelEnd = nSelEnd;
    }
    int nTotalNum = strlen( strChar );
    int nCharNum = 0;
    int nStart = 0;
    if ( pntCursor.x == -1 )
    {
        if ( nTotalNum < m_CobFnt.m_nCharNum )
        {
            nStart = 0;
            nCharNum = nTotalNum;
        }
        else
        {
            nStart = nCursor - m_CobFnt.m_nCharNum;
            nCharNum = m_CobFnt.m_nCharNum;
        }
    }
    else
    {
        nStart = nCursor - pntCursor.x / ( CMyBitmap::GetFontSize() / 2 );
        nCharNum = ( ( nTotalNum - nStart ) > m_CobFnt.m_nCharNum ) ? m_CobFnt.m_nCharNum : nTotalNum - nStart;
    }
    if ( nCharNum > 0 )
    {
        char strNorChar [1024] = "";
        memset(strNorChar, 0L, sizeof(strNorChar));
        // Test to show the chinese normal
        if ( nCharNum >= m_CobFnt.m_nCharNum - 1 && strChar[nStart] < 0 )
        {
            int nChinese = 0;
            for ( int i = nStart; i < nStart + nCharNum; i++ )
            {
                if ( strChar[i] < 0 )
                {
                    nChinese++;
                }
                else
                {
                    break;
                }
            }
            if ( nChinese % 2 != 0 )
            {
                nStart++;
            }
        }
        strcpy( strNorChar, strChar + nStart );
        strNorChar[nCharNum] = '\0';
        // Show the normal char
        CMyBitmap::ShowString ( m_CobPar.m_Pnt.x,
                                m_CobPar.m_Pnt.y,
                                m_CobFnt.m_dwFontColor,
                                strNorChar ) ;
        if ( m_CobPar.m_nSelNum > 0 )
        {
            char strSelChar [128] = "";
            memset(strSelChar, 0L, sizeof(strSelChar));
            if ( nSelStart < nStart )
            {
                m_CobPar.m_nSelNum -= nStart - nSelStart;
                nSelStart = nStart ;
            }
            strncpy ( strSelChar,
                      strNorChar + ( nSelStart - nStart ),
                      m_CobPar.m_nSelNum );
            strSelChar[m_CobPar.m_nSelNum] = 0;
            // Show the select block
            if (m_CobPar.m_nWidth > (int)((( nSelStart - nStart + m_CobPar.m_nSelNum ) * ( CMyBitmap::GetFontSize() / 2 ))))
                CMyBitmap::ShowBlock( m_CobPar.m_Pnt.x + ( nSelStart - nStart ) * ( CMyBitmap::GetFontSize() / 2 ),
                                      m_CobPar.m_Pnt.y,
                                      m_CobPar.m_Pnt.x + ( nSelStart - nStart + m_CobPar.m_nSelNum ) * ( CMyBitmap::GetFontSize() / 2 ),
                                      m_CobPar.m_Pnt.y + CMyBitmap::GetFontSize() + 4,
                                      0xffffffff );
            else
                CMyBitmap::ShowBlock( m_CobPar.m_Pnt.x + ( nSelStart - nStart ) * ( CMyBitmap::GetFontSize() / 2 ),
                                      m_CobPar.m_Pnt.y,
                                      m_CobPar.m_Pnt.x + m_CobPar.m_nWidth,
                                      m_CobPar.m_Pnt.y + CMyBitmap::GetFontSize() + 4,
                                      0xffffffff );
            // Show the select char
            CMyBitmap::ShowString( m_CobPar.m_Pnt.x + ( nSelStart - nStart ) * ( CMyBitmap::GetFontSize() / 2 ),
                                   m_CobPar.m_Pnt.y,
                                   m_CobFnt.m_dwBackColor,
                                   strSelChar );
        }
        // Show the cursor pos
        if ( ::timeGetTime() / 500 % 2 == 0 && this->GetFocus() == this )
        {
			CMyBitmap::ShowString(m_CobPar.m_Pnt.x + ( nCursor - nStart ) * ( CMyBitmap::GetFontSize() / 2 ) - 2,
                                   m_CobPar.m_Pnt.y,
								    0xffffffff,
									"|");
    /*        CMyBitmap::ShowBlock ( m_CobPar.m_Pnt.x + ( nCursor - nStart ) * ( CMyBitmap::GetFontSize() / 2 ),
                                   m_CobPar.m_Pnt.y,
                                   m_CobPar.m_Pnt.x + ( nCursor - nStart ) * ( CMyBitmap::GetFontSize() / 2 ) + 2,
                                   m_CobPar.m_Pnt.y + CMyBitmap::GetFontSize() + 4,
                                   0xffffffff ) ;*/
        }
    }
}

void CMyCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

BOOL CMyCombo::Init(int xPos, int yPos, char* FileName, DWORD dwFontColor, DWORD dwBackColor )
{
    CMyCombo::SetLimitText ( 256 ) ;
    CRect rect;
    CMyCombo::GetWindowRect ( rect ) ;
    m_CobPar.m_IntPnt.x = rect.left - xPos ;
    m_CobPar.m_IntPnt.y = rect.top - yPos ;
    m_CobPar.m_nWidth = rect.right - rect.left ;
    m_CobPar.m_nHeight = rect.bottom - rect.top ;
    m_CobFnt.m_btFontWidth = (BYTE)(CMyBitmap::GetFontSize ( ) / 2 + 2);
    m_CobFnt.m_nCharNum = m_CobPar.m_nWidth / ( CMyBitmap::GetFontSize () / 2 );
    // Init the font
    m_CobFnt.m_dwFontColor = dwFontColor ;
    m_CobFnt.m_dwBackColor = dwBackColor ;
    if ( m_EditRect.m_hWnd == NULL )
    {
        // Hide the edit and create a rect for receive mouse&button message
        this->MoveWindow( m_CobPar.m_IntPnt.x, m_CobPar.m_IntPnt.y, 0, 0 );
        CRect EditRect = CRect( m_CobPar.m_IntPnt.x,
                                m_CobPar.m_IntPnt.y,
                                m_CobPar.m_IntPnt.x + rect.Width(),
                                m_CobPar.m_IntPnt.y + rect.Height() );
        m_EditRect.Create( "",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                           EditRect,
                           this->GetParent(),
                           0 );
        m_EditRect.Init( this );
    }
    if ( FileName )
    {
        char DrpName [128] ;
        int len = strlen ( FileName ) ;
        strncpy ( DrpName, FileName, len - 4 ) ;
        DrpName [len - 4] = 0 ;
        strcat ( DrpName, "Drp" ) ;
        strcat ( DrpName, FileName + len - 4 ) ;
        m_CobPar.m_bBorder = true ;
        return true ;
    }
    return true ;
}

void CMyCombo::ReInit(int xPos, int yPos)
{
    CRect rect;
    m_EditRect.GetWindowRect ( rect ) ;
    m_CobPar.m_IntPnt.x = rect.left - xPos ;
    m_CobPar.m_IntPnt.y = rect.top - yPos ;
    m_CobPar.m_nWidth =	  rect.right - rect.left ;
    m_CobPar.m_nHeight =  rect.bottom - rect.top ;
    m_CobFnt.m_btFontWidth =(BYTE)( CMyBitmap::GetFontSize ( ) / 2 + 2);
    m_CobFnt.m_nCharNum = m_CobPar.m_nWidth / ( CMyBitmap::GetFontSize () / 2 );
}

LRESULT CMyCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return CEdit::WindowProc(message, wParam, lParam);
}

void CMyCombo::InitListBox(CDlgListBox* ListBox)
{
    m_CobLstBox = ListBox ;
}

void CMyCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch ( nChar )
    {
    case 38:
        CMyCombo::GetParent()->PostMessage ( WM_VSCROLL, SB_LINEUP ) ;
        return ;
    case 40:
        CMyCombo::GetParent()->PostMessage ( WM_VSCROLL, SB_LINEDOWN ) ;
        return ;
    }
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( nFlags == 0 && !m_CobPar.m_bChinese )
    {
        return;
    }
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CMyCombo::EnableChinese( BOOL bChinese )
{
    m_CobPar.m_bChinese = bChinese;
}
