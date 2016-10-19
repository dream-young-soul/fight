
//**********************************************************
// 代码编辑器
//**********************************************************

// GameMsg.cpp: implementation of the GameMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "basefunc.H"
#include "nd_bitmap.H"
#include "GameMsg.h"
#include "Player.h"
#include "Hero.h"
#include "GameDataSet.h"
#include "3droledata.h"
//--------------------------------------------------------------------
// define string constants
//--------------------------------------------------------------------
const int  FONT_EDGE_COLOR				= 0x000000;

const OBJID STRING_PREFIX_GLOBAL		= 100005;//"【千里传音】";
const OBJID STRING_PREFIX_SYSTEM		= 100006;//"【系统】";
const OBJID STRING_PREFIX_ACTION		= 100007;//"【动作】";
const OBJID STRING_PREFIX_WHISPER		= 100008;//"【私聊】";
const OBJID STRING_PREFIX_TEAM			= 100009;//"【队伍】";
const OBJID STRING_PREFIX_CLAN			= 100010;//"【帮派】";
const OBJID STRING_PREFIX_FAMILY		= 100011;//"【家庭】";
const OBJID STRING_PREFIX_TALK			= 100012;//"【交谈】";
const OBJID STRING_PREFIX_YELP			= 100013;//"【叫喊】";
const OBJID STRING_PREFIX_FRIEND		= 100014;//"【朋友】";
const OBJID STRING_PREFIX_GM			= 100015;//"【ＧＭ】";
const OBJID STRING_PREFIX_GHOST			= 100016;//"【幽灵】";
const OBJID STRING_PREFIX_LEAVEWORD		= 100119;//"【留言】";
const OBJID STRING_PREFIX_SERVE			= 100157;//"【服务】";

const OBJID STRING_TO					= 100017;//"对";
const OBJID STRING_SAY					= 100018;//"说：";

const OBJID STR_SYSTEM					= 100020;
const OBJID STR_ALLUSERS				= 100026;

const char STRING_SYSTEM[]				= "SYSTEM";
const char STRING_ALL_USERS[]			= "ALLUSERS";

const int	LAST_SHOWLINE_INDEX			= 17;
const int	FONT_NAME					= 10049;
const int  MSGFONT_WIDTH				= 7;
const int  MSGFONT_HEIGHT				= 14;
const int  WARNING_CHAR1				= 100300;		//"密";
const int  WARNING_CHAR2				= 100301;		//"码";
const int  WARNING_CHAR3				= 100302;		//"MIMA";
const int  WARNING_CHAR4				= 100303;		//"PASSWORD";
//--------------------------------------------------------------------

// global...
CGameMsg	g_objGameMsg;
#ifdef _ANALYSIS_ON
DWORD g_dwTxtMsgTimeCost	= 0;
#endif

//--------------------------------------------------------------------
// CGameMsgUnit Construction/Destruction
//--------------------------------------------------------------------
CGameMsgUnit::CGameMsgUnit(void)
{
    this->m_nPrevPosX	= 0;
    this->m_nPrevPosY	= 0;
    this->m_dwTime		= 2500;
    this->m_nStartX		= 0;
    this->m_nDragStep	= 0;
    this->m_nQuakeFlag	= 1;
    this->m_bSuper		= false;
    memset(m_szSender, 0L, sizeof(m_szSender));
    memset(m_szReceiver, 0L, sizeof(m_szReceiver));
    memset(m_szEmotion, 0L, sizeof(m_szEmotion));
    memset(m_szMsg, 0L, sizeof(m_szMsg));
    //	CMyBitmap::GetFontSize(m_sizeFont);
    m_sizeFont.iWidth = MSGFONT_WIDTH;
    m_sizeFont.iHeight = MSGFONT_HEIGHT;
    CMyBitmap::GetScreenSize(m_sizeScreen.iWidth, m_sizeScreen.iHeight);
}
//--------------------------------------------------------------------
CGameMsgUnit::~CGameMsgUnit(void)
{
    return;
}
//--------------------------------------------------------------------
void CGameMsgUnit::GetDateTimeStr(DWORD dwDateTime, char* pszStr)
{
    // DWORD: 2002-10-10,10:20  200210101020
    char szTemp[64];
    int nYear, nMonth, nDay, nHour, nMinute;
    nYear	= dwDateTime / 100000000;
    nMonth	= (dwDateTime / 1000000) % 100;
    nDay	= (dwDateTime / 10000) % 100;
    nHour	= (dwDateTime / 100) % 100;
    nMinute	= dwDateTime % 100;
    sprintf(szTemp, "【%d:%d】", nHour, nMinute);
    strcpy(pszStr, szTemp);
}
//--------------------------------------------------------------------
char* CGameMsgUnit::Show(int nStartX, int nStartY, BOOL bCapture, int nScreenWidth)
{
    if (m_nDragStep == 0)
    {
        m_nStartX = nScreenWidth;
        m_nDragStep = 1;
    }
    if (this->m_unTxtStyle & 0x0001) // 拖动效果
    {
        if (nStartX < m_nStartX)
        {
            m_nStartX = m_nStartX - 6;
        }
        else
        {
            m_nStartX = nStartX;
        }
    }
    else
    {
        m_nStartX = nStartX;
    }
    if (this->m_unTxtStyle & 0x0002 && (::TimeGet() - this->m_dwBeginTime) <=  3000) // 闪烁效果
    {
        this->m_dwWordsColorFlash += 0x00000500;
        if (this->m_dwWordsColorFlash > 0xFF00FF00)
        {
            this->m_dwWordsColorFlash = _COLOR_SYSTEM_SPEAKER;
        }
        this->m_dwSystemColor			= this->m_dwWordsColorFlash;
        this->m_dwSystemMessageColor	= this->m_dwWordsColorFlash;
        this->m_dwSenderColor			= this->m_dwWordsColorFlash;
        this->m_dwEmotionColor			= this->m_dwWordsColorFlash;
        this->m_dwNormalColor			= this->m_dwWordsColorFlash;
        this->m_dwReceiverColor			= this->m_dwWordsColorFlash;
        this->m_dwWordsColor			= this->m_dwWordsColorFlash;
    }
    else
    {
        this->m_dwSystemColor			= _COLOR_SYSTEM_SPEAKER;
        this->m_dwSystemMessageColor	= _COLOR_SYSTEM_MESSAGE;
        this->m_dwSenderColor			= _COLOR_PLAYER_SPEAKER;
        this->m_dwEmotionColor			= _COLOR_EMOTION;
        this->m_dwNormalColor			= _COLOR_NORMAL;
        this->m_dwReceiverColor			= _COLOR_RECEIVER;
    }
    if (this->m_unTxtStyle & 0x0008) // 地震效果
    {
        if (this->m_nQuakeFlag < 0)
        {
            if (this->m_nQuakeFlag++ > 0)
            {
                this->m_nStartX += 2;
            }
        }
        else
        {
            this->m_nQuakeFlag = (-10);
            this->m_nStartX -= 2;
        }
    }
    DWORD dwColorTitle = this->m_dwSystemColor;
    if (strstr(this->m_szMsg, g_objHero.GetName()))
    {
        dwColorTitle = _COLOR_SELF;
    }
    else if (strstr(this->m_szSender, g_objHero.GetName()))
    {
        dwColorTitle = _COLOR_SELF;
    }
    else if (strstr(this->m_szReceiver, g_objHero.GetName()))
    {
        dwColorTitle = _COLOR_SELF;
    }
    // 显示前缀
    CMyPos pos;
    pos.x = m_nStartX;
    pos.y = nStartY;
    switch(this->m_unTxtAttribute)
    {
    case _TXTATR_SYSTEM:
        {
            if (this->m_unTxtStyle & 0x0002 && (::TimeGet() - this->m_dwBeginTime) <=  3000) // 闪烁效果
            {
                pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM), nScreenWidth);
            }
            else
            {
                dwColorTitle = 0XFFFE00FB;
                pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM), nScreenWidth);
                this->m_dwWordsColor = 0XFFFFFF00;
            }
            break;
        }
    case _TXTATR_ACTION:
        {
            dwColorTitle = 0xFFB9FFC0;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_ACTION), nScreenWidth);
            break;
        }
    case _TXTATR_PRIVATE:
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_WHISPER), nScreenWidth);
            break;
        }
    case _TXTATR_TEAM:
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_TEAM), nScreenWidth);
            break;
        }
    case _TXTATR_SYNDICATE:
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_CLAN), nScreenWidth);
            break;
        }
    case _TXTATR_FAMILY:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_FAMILY), nScreenWidth);
        break;
    case _TXTATR_NORMAL:
    case _TXTATR_TALK:
        if (0 != strcmp(m_szSender, STRING_SYSTEM))
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_TALK), nScreenWidth);
        }
        else
        {
            dwColorTitle = 0xFF00FF00;
            this->m_dwWordsColor = 0xFFFFFF00;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM), nScreenWidth);
        }
        break;
    case _TXTATR_YELP:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_YELP), nScreenWidth);
        break;
    case _TXTATR_FRIEND:
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_FRIEND), nScreenWidth);
            break;
        }
    case _TXTATR_GLOBAL:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_GLOBAL), nScreenWidth);
        break;
    case _TXTATR_GM:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_GM), nScreenWidth);
        break;
    case _TXTATR_GHOST:
        {
            dwColorTitle = 0xFFFE00FB;
            pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_GHOST), nScreenWidth);
            break;
        }
    case _TXTATR_LEAVEWORD:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_LEAVEWORD), nScreenWidth);
        break;
    case _TXTATR_SERVE:
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_SERVE), nScreenWidth);
        break;
    default:
        this->m_unTxtAttribute = _TXTATR_SYSTEM;
        pos = ShowColorStr(pos.x, pos.y, dwColorTitle, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM), nScreenWidth);
        break;
    }
    // 显示表情
    if (strlen(this->m_szEmotion) > 0)
    {
        char szTemp[_MAX_WORDSSIZE * 2];
        int nIndex = 0;
        nIndex = atoi(this->m_szEmotion);
        if (m_unTxtAttribute != _TXTATR_ACTION && m_unTxtAttribute != _TXTATR_SYSTEM)
            sprintf(szTemp,	g_obj3DRoleData.GetTextActionText(nIndex),
                    this->m_szSender,
                    this->m_szReceiver);
        pos = ShowColorStr(pos.x, pos.y, m_dwEmotionColor, szTemp, nScreenWidth);
    }
    else		// 显示信息
    {
        const OBJID idStrSystem = 100020;
        if (_TXTATR_ACTION != m_unTxtAttribute &&
                _TXTATR_SYSTEM != m_unTxtAttribute &&
                0 != strcmp(m_szSender, g_objGameDataSet.GetStr(idStrSystem)) &&
                0 != strcmp(m_szSender, STRING_SYSTEM))
        {
            pos = ShowColorStr(pos.x, pos.y, 0xFFB9FFC0, m_szSender, nScreenWidth);
            //			pos = ShowColorStr(pos.x, pos.y, m_dwEmotionColor, m_szEmotion, nScreenWidth);
            pos = ShowColorStr(pos.x, pos.y, 0xFFFFFFFF, g_objGameDataSet.GetStr(STRING_TO), nScreenWidth);
            pos = ShowColorStr(pos.x, pos.y, 0xFFB9FFC0, m_szReceiver, nScreenWidth);
            pos = ShowColorStr(pos.x, pos.y, 0xFFFFFFFF, g_objGameDataSet.GetStr(STRING_SAY), nScreenWidth);
        }
    }
    pos = ShowColorStr(pos.x, pos.y, this->m_dwWordsColor, this->m_szMsg, nScreenWidth);
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    OBJID idStrAllUser = 100026;
    if (this->m_unTxtAttribute != _TXTATR_SYSTEM && 0 != strcmp(m_szSender, STRING_SYSTEM) && bCapture)
    {
        if ( 0 == strcmp(g_objHero.GetName(), this->m_szSender) && 0 == strcmp(this->m_szReceiver, g_objGameDataSet.GetStr(idStrAllUser)))
        {
            return NULL;
        }
        CMySize sizeFont;
        //		CMyBitmap::GetFontSize(sizeFont);
        sizeFont.iWidth = MSGFONT_WIDTH;
        sizeFont.iHeight = MSGFONT_HEIGHT;
        CMyPos posRectBegin, posRectEnd;
        posRectBegin.x = m_nStartX;
        posRectBegin.y = nStartY;
        posRectEnd.x = m_nStartX + nScreenWidth - 2;
        posRectEnd.y = pos.y + sizeFont.iHeight + 1;
        if (posMouse.x > posRectBegin.x && posMouse.y > posRectBegin.y && posMouse.x < posRectEnd.x && posMouse.y < posRectEnd.y)
        {
            static DWORD dwFrame = 0;
            dwFrame ++;
            DWORD dwCurrentFrame = dwFrame % 31;
            DWORD dwR, dwG, dwB;
            if (dwCurrentFrame <= 15)
            {
                dwR = 0x11 * dwCurrentFrame;
                dwG = 0x11 * dwCurrentFrame;
                dwB = 0xff - 0x11 * dwCurrentFrame;
            }
            else
            {
                dwR = 0xff - 0x11 * (31 - dwCurrentFrame);
                dwG = 0xff - 0x11 * (31 - dwCurrentFrame);
                dwB = 0x11 * (31 - dwCurrentFrame);
            }
            DWORD dwColor = 0xff000000 + (dwR << 16) + (dwG << 8) + dwB;
            CMyBitmap::ShowRect(posRectBegin.x, posRectBegin.y, posRectEnd.x, posRectEnd.y, dwColor);
            if ( 0 != strcmp(g_objHero.GetName(), this->m_szSender) )
            {
                return this->m_szSender;
            }
            else
            {
                return this->m_szReceiver ;
            }
        }
    }
    /*
    char szTemp[64];
    GetDateTimeStr(m_dwTime, szTemp);
    pos = ShowColorStr(pos.x, pos.y, this->m_dwWordsColor, szTemp);
    */
    return NULL;
}
//--------------------------------------------------------------------
int CGameMsgUnit::GetLines( int nScreenWidth )
{
    return Scan(nScreenWidth);
}
//--------------------------------------------------------------------
CMyPos CGameMsgUnit::ShowColorStr(int nPosX, int nPosY
                                  , DWORD dwColor, const char* pszString, int nScreenWidth)
{
    CMyPos posReturn;
    posReturn.x = nPosX;
    posReturn.y = nPosY;
    if (!pszString || strlen(pszString) < 1)
    {
        return posReturn;
    }
    const char* buf = pszString;
    while(true)
    {
        int nLineLen = (nScreenWidth - m_sizeFont.iWidth * 2 - posReturn.x) / m_sizeFont.iWidth;
        if (nLineLen <= 0)
        {
            posReturn.x = m_nStartX;
            posReturn.y += m_sizeFont.iHeight + 2;
            continue;
        }
        int nStrLen = strlen(buf);
        if (nLineLen >= nStrLen)
        {
            // 最后一行
            CMyBitmap::ShowEmotionString(posReturn.x + 1, posReturn.y + 1, FONT_EDGE_COLOR, buf, m_dwBeginTime, false, (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            CMyBitmap::ShowEmotionString(posReturn.x, posReturn.y, dwColor, buf , m_dwBeginTime, true, (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            posReturn.x += m_sizeFont.iWidth * nStrLen;
            break;
        }
        if (CMyBitmap::IsDBCSLeadByte(buf, nLineLen - 1))
        {
            nLineLen --;
        }
        //----------Begin the english version's process(jump line)----------//
        if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
        {
            // Search the last word
            for ( int i = nLineLen - 1; i >= 0; i-- )
            {
                if ( buf[i] == ' ' )
                {
                    nLineLen -= nLineLen - i - 1;
                    break;
                }
            }
        }
        //----------End the english version's process(jump line)----------//
        char szFace[3] = "";
        strncpy(szFace, buf + nLineLen - 1, 3);		//emotion ani jump line
        if (szFace[0] == '#'
                && (szFace[1] >= '0' && szFace[1] <= '9')
                && (szFace[1] >= '0' && szFace[1] <= '9'))
        {
            nLineLen = nLineLen - 1;
        }
        strncpy(szFace, buf + nLineLen - 2, 3);		//emotion ani jump line
        if (szFace[0] == '#'
                && (szFace[1] >= '0' && szFace[1] <= '9')
                && (szFace[1] >= '0' && szFace[1] <= '9'))
        {
            nLineLen = nLineLen - 2;
        }
        // end emotion ani jump line
        char szMsg[2048] = "";
        strncpy(szMsg, buf, nLineLen);
        CMyBitmap::ShowEmotionString(posReturn.x + 1, posReturn.y + 1, FONT_EDGE_COLOR, szMsg, m_dwBeginTime, false, (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
        CMyBitmap::ShowEmotionString(posReturn.x, posReturn.y, dwColor, szMsg , m_dwBeginTime, true, (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
        buf += nLineLen;
        posReturn.x = m_nStartX;
        posReturn.y += m_sizeFont.iHeight + 2;
    }
    return posReturn;
}
//--------------------------------------------------------------------
BOOL CGameMsgUnit::GetText(char* pszText, BOOL bTimes)
{
    if (strlen(pszText) <= 0 || pszText == NULL)
    {
        return false;
    }
    char szTemp[_MAX_WORDSSIZE * 2] = "";
    // 组合信息的前缀
    if (strlen(this->m_szEmotion) > 0)
    {
        int nIndex = 0;
        nIndex = atoi(this->m_szEmotion);
        if (m_unTxtAttribute != _TXTATR_ACTION && m_unTxtAttribute != _TXTATR_SYSTEM)
            sprintf(szTemp,	g_obj3DRoleData.GetTextActionText(nIndex),
                    this->m_szSender,
                    this->m_szReceiver);
    }
    else
    {
        switch(m_unTxtAttribute)
        {
        case _TXTATR_SYSTEM:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM));
            break;
        case _TXTATR_ACTION:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_ACTION));
            break;
        case _TXTATR_PRIVATE:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_WHISPER));
            break;
        case _TXTATR_TEAM:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_TEAM));
            break;
        case _TXTATR_SYNDICATE:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_CLAN));
            break;
        case _TXTATR_FAMILY:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_FAMILY));
            break;
        case _TXTATR_NORMAL:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_TALK));
            break;
        case _TXTATR_TALK:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_TALK));
            break;
        case _TXTATR_YELP:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_YELP));
            break;
        case _TXTATR_FRIEND:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_FRIEND));
            break;
        case _TXTATR_GLOBAL:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_GLOBAL));
            break;
        case _TXTATR_GM:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_GM));
            break;
        case _TXTATR_GHOST:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_GHOST));
            break;
        case _TXTATR_LEAVEWORD:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_LEAVEWORD));
            break;
        case _TXTATR_SERVE:
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_SERVE));
            break ;
        default:
            this->m_unTxtAttribute = _TXTATR_SYSTEM;
            strcpy(szTemp, g_objGameDataSet.GetStr(STRING_PREFIX_SYSTEM));
            break;
        }
        if (m_unTxtAttribute != _TXTATR_ACTION && m_unTxtAttribute != _TXTATR_SYSTEM)
            sprintf(szTemp, "%s%s%s%s%s",
                    szTemp,
                    this->m_szSender,
                    //				this->m_szEmotion,
                    g_objGameDataSet.GetStr(STRING_TO),
                    this->m_szReceiver,
                    g_objGameDataSet.GetStr(STRING_SAY));
    }
    sprintf(szTemp, "%s%s", szTemp, this->m_szMsg);
    szTemp[_MAX_WORDSSIZE - 1] = '\0' ;
    strcpy(pszText, szTemp);
    if ( bTimes )
    {
        GetDateTimeStr(m_dwTime, szTemp);
        sprintf(pszText, "%s%s", pszText, szTemp);
    }
    return true;
}
//--------------------------------------------------------------------

int CGameMsgUnit::Scan(int nScreenWidth)
{
    char szTemp[_MAX_WORDSSIZE * 2];
    GetText(szTemp, false);
    //--------------------------------------
    int nLines = 1;
    char* pszTemp = szTemp;
    unsigned char ucSingle[2] = {'\0', '\0'};
    unsigned char ucDouble[3] = {'\0', '\0', '\0'};
    int nPosX = m_nStartX;
    while((ucSingle[0] = *pszTemp))
    {
        if (ucSingle[0] < 0x80)
        {
            nPosX += m_sizeFont.iWidth;
            if ((nPosX - m_nStartX) > (nScreenWidth - m_sizeFont.iWidth * 2))
            {
                nPosX = m_nStartX;
                nLines++;
            }
        }
        else
        {
            if (ucDouble[0] == '\0')
            {
                ucDouble[0] = ucSingle[0];
            }
            else if (ucDouble[1] == '\0')
            {
                ucDouble[1] = ucSingle[0];
                nPosX = nPosX + m_sizeFont.iWidth * 2;
                if ((nPosX - m_nStartX) > (nScreenWidth - m_sizeFont.iWidth * 2))
                {
                    nPosX = m_nStartX;
                    nLines++;
                }
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
            }
        }
        pszTemp++;
    }
    return nLines;
}
//--------------------------------------------------------------------
// CGameMsg Construction/Destruction
//--------------------------------------------------------------------
CGameMsg::CGameMsg()
{
    m_setMsg.SetMaxMsg(2048);
    m_dwTime	= 0;
    m_bChannelTalk			=	true;
    m_bChannelSyndicate		=	true;
    m_bChannelFriend		=	true;
    m_bChannelTeam			=	true;
    m_bChannelAction        =   true;
    m_bChannelServe			=   true;
    m_bChannelPrivateExceptFriend	=	true;
    m_bChannelPrivateFriend			=	true;
    strcpy(m_szCaptureName, "");
    m_colorChannelTalk			=	0xffffffff;
    m_colorChannelSyndicate		=	0xffffff00;
    m_colorChannelFriend		=	0xffffff00;
    m_colorChannelTeam			=	0xffffff00;
    m_colorChannelPrivate		=	0xffffff00;
    m_colorChannelAction		=	0xffffff00;
    m_colorChannelServe			=	0xffff0000;
    m_usCurShowChannel = _TXTATR_NORMAL ;
    m_nMaxMsgLine[0] = 128 ;
    m_nMaxMsgLine[1] = 16 ;
    m_nMaxMsgLine[2] = 128 ;
    m_nMaxMsgLine[3] = 20 ;
    m_nMaxMsgLine[4] = 3 ;
    m_nMaxMsgLine[5] = 16 ;
    m_nMaxMsgLine[6] = 128 ;
    m_nMaxMsgLine[7] = 16 ;
    m_nMaxMsgLine[8] = 128 ;
    m_nMaxMsgLine[9] = 16 ;
    m_nMaxMsgLine[10] = 1 ;
    m_nMaxMsgLine[11] = 16 ;
    m_nMaxMsgLine[12] = 16 ;
    m_nMaxMsgLine[13] = 20 ;
    m_nLastShowLineIndex[0] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[1] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[2] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[3] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[4] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[5] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[6] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[7] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[8] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[9] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[10] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[11] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[12] = LAST_SHOWLINE_INDEX ;
    m_nLastShowLineIndex[13] = LAST_SHOWLINE_INDEX ;
    m_nNorAreaAmount = 0 ;
    m_NorAreaShowPos.x = 0;
    m_NorAreaShowPos.y = 0;
    m_nNorAreaWidth = 0 ;
    m_nSysAreaAmount = 0 ;
    m_SysAreaShowPos.x = 2;
    m_SysAreaShowPos.y = MSGFONT_HEIGHT * (2 + m_nMaxMsgLine[4]);
    m_nSysAreaWidth = 0 ;
    m_nManAreaAmount = 0 ;
    m_ManAreaShowPos.x = 100;
    m_ManAreaShowPos.y = (_SCR_HEIGHT - MSGFONT_HEIGHT - 130) / 2;
    m_nManAreaWidth = 0 ;
}
//--------------------------------------------------------------------
CGameMsg::~CGameMsg()
{
    this->ClearMsg();
    this->DestroyBlackName();
    this->DestroyLeaveWord();
}
void CGameMsg::ShowEx(BOOL bCapture, BOOL bSystem)
{
#ifdef _ANALYSIS_ON
    DWORD dwTimeBegin = ::TimeGet();
#endif
    int nScrWidth, nScrHeight;
    CMyBitmap::GetScreenSize(nScrWidth, nScrHeight);
    int nFontWidth, nFontHeight;
    nFontHeight	= MSGFONT_HEIGHT;
    nFontWidth	= MSGFONT_WIDTH;
    int i;
    CMyPos pos;
    // Display normal message
    int nTotalLines = 0;
    int nTotalUpLines = 0;
    int nTotalDownLines = 0;
    bool bMsgArea = 0;  // 0 down 1 up
    pos.x = m_NorAreaShowPos.x;
    pos.y = m_NorAreaShowPos.y;
    int pos_Upy = m_NorAreaShowPos.y - (m_nMaxDownLine + 1) * (nFontHeight + 2) - 20;
    BOOL bGetCapturename = false;
    if ( m_usCurShowChannel == _TXTATR_NORMAL )
    {
        for ( i = m_nNorAreaAmount - 1; i >= 0; i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_nNorAreaShowIndex[i * 2]][m_nNorAreaShowIndex[i * 2 + 1]] ;
            if ( pMsg && this->GetChannelStatus(pMsg->m_unTxtAttribute))
            {
                int nLines = pMsg->GetLines( m_nNorAreaWidth );
                nTotalLines += nLines;
                if (strstr(pMsg->m_szMsg, g_objHero.GetName()))
                {
                    nTotalDownLines += nLines;
                    pos.y = pos.y - (nFontHeight + 6) * nLines;
                    bMsgArea = 0;
                }
                else if (strcmp(pMsg->m_szSender, g_objHero.GetName()) == 0)
                {
                    nTotalDownLines += nLines;
                    pos.y = pos.y - (nFontHeight + 6) * nLines;
                    bMsgArea = 0;
                }
                else if (strcmp(pMsg->m_szReceiver, g_objHero.GetName()) == 0)
                {
                    nTotalDownLines += nLines;
                    pos.y = pos.y - (nFontHeight + 6) * nLines;
                    bMsgArea = 0;
                }
                else
                {
                    nTotalUpLines += nLines;
                    pos_Upy = pos_Upy - (nFontHeight + 6) * nLines ;
                    bMsgArea = 1;
                }
                if (nTotalUpLines > m_nMaxUpLine && bMsgArea == 1)
                {
                    continue;
                }
                if (nTotalDownLines > m_nMaxDownLine && bMsgArea == 0)
                {
                    continue;
                }
                int tPosy = 0;
                if (bMsgArea == 1)
                {
                    tPosy = pos_Upy;
                }
                else
                {
                    tPosy = pos.y;
                }
                char* pszName = pMsg->Show(pos.x, tPosy, bCapture, m_nNorAreaWidth);
                if (pszName)
                {
                    strcpy(m_szCaptureName, pszName);
                    bGetCapturename = true;
                }
            }
        }
        if (bCapture)
        {
            const OBJID idStr = 100130;
            CMyBitmap::ShowString(pos.x + 16, pos.y - MSGFONT_HEIGHT * 2 - 1, 0xff000000, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            CMyBitmap::ShowString(pos.x + 15, pos.y - MSGFONT_HEIGHT * 2 - 2, 0xffffff00, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
        }
    }
    else
    {
        int nLast = ( m_nMaxLine < (int)m_dequeMsg[m_usCurShowChannel].size() ) ? (int)m_dequeMsg[m_usCurShowChannel].size() - m_nMaxLine : 0 ;
        for ( i = m_dequeMsg[m_usCurShowChannel].size() - 1 - ( m_nLastShowLineIndex[m_usCurShowChannel] - m_nMaxLine );
                i >= nLast - ( m_nLastShowLineIndex[m_usCurShowChannel] - m_nMaxLine );
                i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_usCurShowChannel][i] ;
            if ( pMsg && this->GetChannelStatus(pMsg->m_unTxtAttribute))
            {
                int nLines = pMsg->GetLines( m_nNorAreaWidth );
                nTotalLines += nLines;
                if (nTotalLines > m_nMaxLine)
                {
                    break;
                }
                pos.y = pos.y - (nFontHeight + 6) * nLines;
                char* pszName = pMsg->Show(pos.x, pos.y, bCapture, m_nNorAreaWidth);
                if (pszName)
                {
                    strcpy(m_szCaptureName, pszName);
                    bGetCapturename = true;
                }
            }
        }
        if (bCapture)
        {
            const OBJID idStr = 100130;
            CMyBitmap::ShowString(pos.x + 16, pos.y - MSGFONT_HEIGHT * 2 - 1, 0xff000000, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            CMyBitmap::ShowString(pos.x + 15, pos.y - MSGFONT_HEIGHT * 2 - 2, 0xffffff00, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
        }
    }
    if (!bGetCapturename)
    {
        strcpy(m_szCaptureName, "");
    }
    m_dwTime++;
#ifdef _ANALYSIS_ON
    g_dwTxtMsgTimeCost += ::TimeGet() - dwTimeBegin;
#endif
}

void CGameMsg::Show(BOOL bCapture, BOOL bSystem)
{
#ifdef _ANALYSIS_ON
    DWORD dwTimeBegin = ::TimeGet();
#endif
    int nScrWidth, nScrHeight;
    CMyBitmap::GetScreenSize(nScrWidth, nScrHeight);
    int nFontWidth, nFontHeight;
    nFontHeight	= MSGFONT_HEIGHT;
    nFontWidth	= MSGFONT_WIDTH;
    int i;
    CMyPos pos;
    if ( bSystem )
    {
        // Display system area message
        pos.x = m_SysAreaShowPos.x;
        pos.y = m_SysAreaShowPos.y;
        for ( i = m_nSysAreaAmount - 1; i >= 0; i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_nSysAreaShowIndex[i * 2]][m_nSysAreaShowIndex[i * 2 + 1]] ;
            if ( pMsg )
            {
                if ( ::TimeGet() > pMsg->m_dwBeginTime + 15 * 1000 )
                {
                    m_dequeMsg[m_nSysAreaShowIndex[i * 2]].erase ( m_dequeMsg[m_nSysAreaShowIndex[i * 2]].begin() + i ) ;
                    SAFE_DELETE(pMsg);
                    m_nSysAreaAmount-- ;
                    for ( int j = i;  j < m_nSysAreaAmount; j++ )
                    {
                        m_nSysAreaShowIndex[j * 2] = m_nSysAreaShowIndex[( j + 1 ) * 2]  ;
                        m_nSysAreaShowIndex[j * 2 + 1] = m_nSysAreaShowIndex[( j + 1 ) * 2 + 1]  ;
                        m_nSysAreaShowIndex[j * 2 + 1]-- ;
                    }
                    break;
                }
                int nLines = pMsg->GetLines();
                pos.y = pos.y - (nFontHeight + 2) * nLines;
                pMsg->Show(pos.x, pos.y, false);
            }
        }
        // Display manage message
        pos.x = m_ManAreaShowPos.x;
        pos.y = m_ManAreaShowPos.y;
        for ( i = m_nManAreaAmount - 1; i >= 0; i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_nManAreaShowIndex[i * 2]][m_nManAreaShowIndex[i * 2 + 1]] ;
            if ( pMsg )
            {
                if ( ::TimeGet() > pMsg->m_dwBeginTime + 15 * 1000 )
                {
                    m_dequeMsg[m_nManAreaShowIndex[i * 2]].erase ( m_dequeMsg[m_nManAreaShowIndex[i * 2]].begin() + i ) ;
                    SAFE_DELETE(pMsg);
                    m_nManAreaAmount-- ;
                    for ( int j = i;  j < m_nManAreaAmount; j++ )
                    {
                        m_nManAreaShowIndex[j * 2] = m_nManAreaShowIndex[( j + 1 ) * 2]  ;
                        m_nManAreaShowIndex[j * 2 + 1] = m_nManAreaShowIndex[( j + 1 ) * 2 + 1]  ;
                        m_nManAreaShowIndex[j * 2 + 1]-- ;
                    }
                    break;
                }
                int nLines = pMsg->GetLines();
                pos.y = pos.y - (nFontHeight + 6) * nLines;
                pMsg->Show(pos.x, pos.y, false);
            }
        }
    }
    else
    {
        // Display normal message
        int nTotalLines = 0;
        pos.x = m_NorAreaShowPos.x;
        pos.y = m_NorAreaShowPos.y;
        BOOL bGetCapturename = false;
        if ( m_usCurShowChannel == _TXTATR_NORMAL )
        {
            for ( i = m_nNorAreaAmount - 1; i >= 0; i-- )
            {
                CGameMsgUnit* pMsg = m_dequeMsg[m_nNorAreaShowIndex[i * 2]][m_nNorAreaShowIndex[i * 2 + 1]] ;
                if ( pMsg && this->GetChannelStatus(pMsg->m_unTxtAttribute))
                {
                    int nLines = pMsg->GetLines( m_nNorAreaWidth );
                    nTotalLines += nLines;
                    if (nTotalLines > m_nMaxLine)
                    {
                        break;
                    }
                    pos.y = pos.y - (nFontHeight + 6) * nLines;
                    char* pszName = pMsg->Show(pos.x, pos.y, bCapture, m_nNorAreaWidth);
                    if (pszName)
                    {
                        strcpy(m_szCaptureName, pszName);
                        bGetCapturename = true;
                    }
                }
            }
            if (bCapture)
            {
                const OBJID idStr = 100130;
                CMyBitmap::ShowString(pos.x + 4, this->GetNorAeroTop() - 1 - 40, 0xff000000, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
                CMyBitmap::ShowString(pos.x + 3, this->GetNorAeroTop() - 2 - 40, 0xffffff00, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            }
        }
        else
        {
            int nLast = ( m_nMaxLine < (int)m_dequeMsg[m_usCurShowChannel].size() ) ? (int)(m_dequeMsg[m_usCurShowChannel].size() - m_nMaxLine) : 0 ;
            for ( i = m_dequeMsg[m_usCurShowChannel].size() - 1 - ( m_nLastShowLineIndex[m_usCurShowChannel] - m_nMaxLine );
                    i >= nLast - ( m_nLastShowLineIndex[m_usCurShowChannel] - m_nMaxLine );
                    i-- )
            {
                CGameMsgUnit* pMsg = m_dequeMsg[m_usCurShowChannel][i] ;
                if ( pMsg && this->GetChannelStatus(pMsg->m_unTxtAttribute))
                {
                    int nLines = pMsg->GetLines( m_nNorAreaWidth );
                    nTotalLines += nLines;
                    if (nTotalLines > m_nMaxLine)
                    {
                        break;
                    }
                    pos.y = pos.y - (nFontHeight + 6) * nLines;
                    char* pszName = pMsg->Show(pos.x, pos.y, bCapture, m_nNorAreaWidth);
                    if (pszName)
                    {
                        strcpy(m_szCaptureName, pszName);
                        bGetCapturename = true;
                    }
                }
            }
            if (bCapture)
            {
                const OBJID idStr = 100130;
                CMyBitmap::ShowString(pos.x + 4, this->GetNorAeroTop() - 1 - 40, 0xff000000, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
                CMyBitmap::ShowString(pos.x + 3, this->GetNorAeroTop() - 2 - 40, 0xffffff00, g_objGameDataSet.GetStr(idStr), (char*)g_objGameDataSet.GetStr(FONT_NAME), MSGFONT_HEIGHT);
            }
        }
        if (!bGetCapturename)
        {
            strcpy(m_szCaptureName, "");
        }
    }
    m_dwTime++;
#ifdef _ANALYSIS_ON
    g_dwTxtMsgTimeCost += ::TimeGet() - dwTimeBegin;
#endif
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
BOOL CGameMsg::AddMsg ( const char* pszSender,
                        char* pszReceiver,
                        char* pszEmotion,
                        char* pszMsg,
                        DWORD dwWordsColor,
                        unsigned short unTxtStyle,
                        unsigned short unTxtAttribute,
                        DWORD dwTime)
{
    // Test if every elements is availability
    if ( ( strlen( pszSender ) == 0 ) ||
            ( !pszMsg || _MAX_WORDSSIZE < strlen( pszMsg ) ) ||
            ( !pszSender || _MAX_NAMESIZE < strlen( pszSender ) ) ||
            ( !pszReceiver || _MAX_NAMESIZE < strlen( pszReceiver ) ) ||
            ( pszEmotion && _MAX_EMOTIONSIZE < strlen( pszEmotion ) ) )
    {
        return false;
    }
    if ( ( unTxtAttribute < _TXTATR_NORMAL || unTxtAttribute > _TXTATR_SERVE ) && unTxtAttribute != _TXTATR_LEAVEWORD )
    {
        return false ;
    }
    //密码警告提示
    if (unTxtAttribute != _TXTATR_SYSTEM && m_nWarningTime < 3)
    {
        char* szMsgUp = _strupr(_strdup(pszMsg));			//大写化字符串
        char* nPos1 = strstr(szMsgUp, g_objGameDataSet.GetStr(WARNING_CHAR1));
        char* nPos2 = strstr(szMsgUp,  g_objGameDataSet.GetStr(WARNING_CHAR2));
        int nResoult1 = nPos1 - szMsgUp + 1;
        int nResoult2 = nPos1 - szMsgUp + 1;
        char* nPos3 = strstr(szMsgUp,  g_objGameDataSet.GetStr(WARNING_CHAR3));
        char* nPos4 = strstr(szMsgUp,  g_objGameDataSet.GetStr(WARNING_CHAR4));
        if (( (nPos1 != NULL && nPos2 != NULL) && (nPos1 - nPos2) < 3)
                || nPos3 != NULL || nPos4 != NULL)
        {
            m_dwShowWarning = ::TimeGet();
            m_nWarningTime++;
            strcpy(m_szWarning, g_objGameDataSet.GetStr(20000)); //
            this->AddMsg(_TXTATR_GM, (char*)g_objGameDataSet.GetStr(20000));
        }
    }
    if (!g_objHero.IsGhost() && unTxtAttribute == _TXTATR_GHOST)
    {
        return false;
    }
    // Create a temp message unit and fill it
    if ( unTxtAttribute == _TXTATR_SYSTEM )
    {
        for (int i = m_nSysAreaAmount - 1; i >= 0; i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_nSysAreaShowIndex[i * 2]][m_nSysAreaShowIndex[i * 2 + 1]] ;
            if ( pMsg )
            {
                if ( 0 == strcmp(pMsg->m_szMsg, pszMsg))
                {
                    pMsg->m_dwBeginTime = ::TimeGet();
                    pMsg->m_unTxtStyle |= 0x0002;
                    return true;
                }
            }
        }
    }
    CGameMsgUnit* pNewMsg = new CGameMsgUnit;
    if (!pNewMsg)
    {
        return false;
    }
    char szTemp[_MAX_WORDSSIZE * 2];
    strcpy(szTemp, pszMsg);
    if (_TXTATR_ACTION == unTxtAttribute)
    {
        sprintf(szTemp, pszMsg, pszSender, pszReceiver);    // 替换动作中的%s
    }
    strcpy(pNewMsg->m_szMsg, szTemp);
    strcpy(pNewMsg->m_szSender, pszSender);
    if (strcmp(pszReceiver, STRING_ALL_USERS) == 0)
    {
        strcpy(pNewMsg->m_szReceiver, g_objGameDataSet.GetStr(STR_ALLUSERS));
    }
    else
    {
        strcpy(pNewMsg->m_szReceiver, pszReceiver);
    }
    if (pszEmotion)
    {
        strcpy(pNewMsg->m_szEmotion, pszEmotion);
    }
    g_objGameMsg.GetChannelColor(unTxtAttribute, pNewMsg->m_dwWordsColor);
    pNewMsg->m_unTxtStyle	= unTxtStyle;
    pNewMsg->m_unTxtAttribute = unTxtAttribute;
    pNewMsg->m_dwTime		= dwTime;
    pNewMsg->m_dwBeginTime	= ::TimeGet();
    pNewMsg->m_nStartX		= 0;
    pNewMsg->m_nDragStep	= 0;
    pNewMsg->m_nQuakeFlag	= 1;
    pNewMsg->m_bSuper		= false;
    m_objChatLog.Log ( pNewMsg ) ;
    // Add the leaveword to deque
    if ( pNewMsg->m_unTxtAttribute == _TXTATR_LEAVEWORD )
    {
        if ( strcmp( pszSender, g_objHero.GetName() ) == 0 )
        {
            SAFE_DELETE( pNewMsg ) ;
            return false ;
        }
        else
        {
            m_dequeLeaveMsg.push_back( pNewMsg ) ;
            return true;
        }
    }
    // Add the message to the deque
    int		nDequeIndex = pNewMsg->m_unTxtAttribute - _TXTATR_NORMAL - 1 ;
    // The normal channel
    if ( pNewMsg->m_unTxtAttribute == _TXTATR_NORMAL )
    {
        nDequeIndex = 6 ;
    }
    m_dequeMsg[nDequeIndex].push_back ( pNewMsg ) ;
    // Test if the current channel message is out of range
    int	nDequeSize = ( int ) m_dequeMsg[nDequeIndex].size() ;
    if ( nDequeSize > m_nMaxMsgLine[nDequeIndex] )
    {
        // The deque is full, kick off the first message
        SAFE_DELETE ( m_dequeMsg[nDequeIndex][0] ) ;
        m_dequeMsg[nDequeIndex].pop_front();
        nDequeSize-- ;
        // process every area
        if ( pNewMsg->m_unTxtAttribute == _TXTATR_SYSTEM )
            // The system area
        {
            ProcessArea ( m_nSysAreaAmount, _SYSAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nSysAreaShowIndex, true ) ;
        }
        else if ( pNewMsg->m_unTxtAttribute == _TXTATR_GM )
            // The manager area
        {
            ProcessArea ( m_nManAreaAmount, _MANAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nManAreaShowIndex, true ) ;
        }
        else
            // The normal area
        {
            ProcessArea ( m_nNorAreaAmount, _NORAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nNorAreaShowIndex, true ) ;
        }
    }
    else
    {
        // process every area
        if ( pNewMsg->m_unTxtAttribute == _TXTATR_SYSTEM )
            // The system area
        {
            ProcessArea ( m_nSysAreaAmount, _SYSAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nSysAreaShowIndex, false ) ;
        }
        else if ( pNewMsg->m_unTxtAttribute == _TXTATR_GM )
            // The manager area
        {
            ProcessArea ( m_nManAreaAmount, _MANAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nManAreaShowIndex, false ) ;
        }
        else
            // The normal area
        {
            ProcessArea ( m_nNorAreaAmount, _NORAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nNorAreaShowIndex, false ) ;
        }
    }
    pNewMsg->GetText(szTemp);
    szTemp[_MAX_WORDSSIZE-1] = '\0';
    m_setMsg.AddMsg(szTemp);
    return true;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Add the message have not sender and receiver
BOOL CGameMsg::AddMsg ( const char* pszMsg,
                        DWORD dwChannel,
                        DWORD dwWordsColor,
                        DWORD dwStyle, BOOL bSuper)
{
    // Test if the message is availability
    if ( !pszMsg || _MAX_WORDSSIZE <= strlen ( pszMsg ) )
    {
        return false;
    }
    if ( dwChannel < _TXTATR_NORMAL || dwChannel > _TXTATR_SERVE )
    {
        return false ;
    }
    if (!g_objHero.IsGhost() && dwChannel == _TXTATR_GHOST)
    {
        return false;
    }
    // Create a temp message unit and fill it
    if ( dwChannel == _TXTATR_SYSTEM )
    {
        for (int i = m_nSysAreaAmount - 1; i >= 0; i-- )
        {
            CGameMsgUnit* pMsg = m_dequeMsg[m_nSysAreaShowIndex[i * 2]][m_nSysAreaShowIndex[i * 2 + 1]] ;
            if ( pMsg )
            {
                if ( 0 == strcmp(pMsg->m_szMsg, pszMsg))
                {
                    pMsg->m_dwBeginTime = ::TimeGet();
                    pMsg->m_unTxtStyle |= 0x0002;
                    return true;
                }
            }
        }
    }
    CGameMsgUnit* pNewMsg = new CGameMsgUnit;
    if (!pNewMsg)
    {
        return false;
    }
    strcpy(pNewMsg->m_szMsg, pszMsg);
    strcpy(pNewMsg->m_szSender, STRING_SYSTEM);
    strcpy(pNewMsg->m_szReceiver, g_objGameDataSet.GetStr(STR_ALLUSERS));
    strcpy(pNewMsg->m_szEmotion, "");
    pNewMsg->m_dwWordsColor		= dwWordsColor;
    g_objGameMsg.GetChannelColor((unsigned short)dwChannel, pNewMsg->m_dwWordsColor);
    pNewMsg->m_unTxtStyle		= (unsigned short)dwStyle;
    pNewMsg->m_unTxtAttribute	= (unsigned short)dwChannel;
    pNewMsg->m_nStartX			= 0;
    pNewMsg->m_nDragStep		= 0;
    pNewMsg->m_nQuakeFlag		= 1;
    pNewMsg->m_bSuper			= bSuper;
    pNewMsg->m_dwTime		= ::SysTimeGet();
    pNewMsg->m_dwBeginTime	= ::TimeGet();
    m_objChatLog.Log ( pNewMsg ) ;
    // Add the message to the deque
    int		nDequeIndex = pNewMsg->m_unTxtAttribute - _TXTATR_NORMAL - 1 ;
    // The normal channel
    if ( pNewMsg->m_unTxtAttribute == _TXTATR_NORMAL )
    {
        nDequeIndex = 6 ;
    }
    m_dequeMsg[nDequeIndex].push_back ( pNewMsg ) ;
    int	nDequeSize = ( int ) m_dequeMsg[nDequeIndex].size() ;
    if ( nDequeSize > m_nMaxMsgLine[nDequeIndex] )
    {
        // The deque is full, kick off the first message
        SAFE_DELETE ( m_dequeMsg[nDequeIndex][0] ) ;
        m_dequeMsg[nDequeIndex].pop_front();
        nDequeSize-- ;
        // process every area
        if ( pNewMsg->m_unTxtAttribute == _TXTATR_SYSTEM )
            // The system area
        {
            ProcessArea ( m_nSysAreaAmount, _SYSAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nSysAreaShowIndex, true ) ;
        }
        else if ( pNewMsg->m_unTxtAttribute == _TXTATR_GM )
            // The manager area
        {
            ProcessArea ( m_nManAreaAmount, _MANAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nManAreaShowIndex, true ) ;
        }
        else
            // The normal area
        {
            ProcessArea ( m_nNorAreaAmount, _NORAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nNorAreaShowIndex, true ) ;
        }
    }
    else
    {
        // process every area
        if ( pNewMsg->m_unTxtAttribute == _TXTATR_SYSTEM )
            // The system area
        {
            ProcessArea ( m_nSysAreaAmount, _SYSAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nSysAreaShowIndex, false ) ;
        }
        else if ( pNewMsg->m_unTxtAttribute == _TXTATR_GM )
            // The manager area
        {
            ProcessArea ( m_nManAreaAmount, _MANAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nManAreaShowIndex, false ) ;
        }
        else
            // The normal area
        {
            ProcessArea ( m_nNorAreaAmount, _NORAREA_MSG_LINE, nDequeIndex, nDequeSize, m_nNorAreaShowIndex, false ) ;
        }
    }
    char szTemp[_MAX_WORDSSIZE * 2];
    pNewMsg->GetText(szTemp);
    szTemp[_MAX_WORDSSIZE-1] = '\0';
    m_setMsg.AddMsg(szTemp);
    return true;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
BOOL CGameMsg::AddMsg(DWORD dwChannel, const char* fmt, ...)
{
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    return this->AddMsg(szMsg, dwChannel);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void CGameMsg::ClearMsg(BOOL bSystem)
{
    // Clear every channel's message
    for ( int i = 0; i < 14; i++ )
    {
        if ( !bSystem && i == 4 )
        {
            continue ;
        }
        int nCount = m_dequeMsg[i].size() ;
        for ( int j = 0; j < nCount; j++ )
        {
            SAFE_DELETE ( m_dequeMsg[i][j] ) ;
        }
        m_dequeMsg[i].clear() ;
    }
    m_nNorAreaAmount = 0 ;
    if ( bSystem )
    {
        m_nSysAreaAmount = 0 ;
    }
    m_nManAreaAmount = 0 ;
    m_nWarningTime = 0;
    m_dwShowWarning = 0;
    strcpy(m_szWarning, "");
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void CGameMsg::ProcessArea ( int& nCurAmount, int nMsgAmount, int nDequeIndex, int nDequeSize, int* pIndex, BOOL bFull )
{
    nDequeSize-- ;
    if ( nCurAmount == nMsgAmount )
        // The area show message is full, kick off the first message
    {
        for ( int i = 0; i < nMsgAmount - 1; i++ )
        {
            pIndex[i * 2] = pIndex[( i + 1 ) * 2] ;
            pIndex[i * 2 + 1] = pIndex[( i + 1 ) * 2 + 1] ;
            if ( bFull && pIndex[i * 2] == nDequeIndex )
            {
                pIndex[i * 2 + 1]-- ;
            }
        }
        pIndex[ ( nMsgAmount - 1 ) * 2] = nDequeIndex ;
        pIndex[ ( nMsgAmount - 1 ) * 2 + 1] = nDequeSize ;
    }
    else
    {
        for ( int i = 0; i < nCurAmount; i++ )
        {
            if ( bFull && pIndex[i * 2] == nDequeIndex )
            {
                pIndex[i * 2 + 1]-- ;
            }
        }
        if ( m_nSysAreaShowIndex[1] < 0 )
        {
            int x = 0;
        }
        pIndex[nCurAmount * 2] = nDequeIndex ;
        pIndex[nCurAmount * 2 + 1] = nDequeSize ;
        nCurAmount++ ;
    }
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void	CGameMsg::DestroyBlackName()
{
    int nAmount = m_setBlackName.size();
    for(int i = 0; i < nAmount; i ++)
    {
        char* pszName = m_setBlackName[i];
        if (pszName)
        {
            delete[] pszName;
        }
    }
    m_setBlackName.clear();
}
//--------------------------------------------------------------------
void	CGameMsg::AddBlackName(char* pszName)
{
    if (pszName && strlen(pszName) > 0)
    {
        for(int i = 0; i < this->GetBlackNameAmount(); i++)
        {
            if (strcmp(pszName, this->GetBlackNameByIndex(i)) == 0)
            {
                this->AddMsg((char*)g_objGameDataSet.GetStr(20001));	//黑名单中已经添加过这个名字
                return;
            }
        }
        char* pszBlackName = new char[strlen(pszName)+1];
        strcpy(pszBlackName, pszName);
        m_setBlackName.push_back(pszBlackName);
        if (this->GetBlackNameAmount() > 6)
        {
            this->DelBlackName(0);
        }
    }
}
//--------------------------------------------------------------------
void	CGameMsg::DelBlackName(int nIndex)
{
    int nAmount = this->GetBlackNameAmount();
    if ((nIndex < 0) && (nIndex >= nAmount))
    {
        return;
    }
    char* pszName = m_setBlackName[nIndex];
    delete[] pszName;
    m_setBlackName.erase(m_setBlackName.begin() + nIndex);
}
//--------------------------------------------------------------------
int		CGameMsg::GetBlackNameAmount()
{
    return m_setBlackName.size();
}
//--------------------------------------------------------------------
char*	CGameMsg::GetBlackNameByIndex(int nIndex)
{
    int nAmount = this->GetBlackNameAmount();
    if ((nIndex < 0) && (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setBlackName[nIndex];
}
//--------------------------------------------------------------------
BOOL	CGameMsg::IsBlackName(char* pszName)
{
    if (!pszName)
    {
        return true;
    }
    int nAmount = this->GetBlackNameAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        char* pszMyName = m_setBlackName[i];
        if (pszMyName && strcmp(pszName, pszMyName) == 0)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------
void	CGameMsg::GetChannelColor(unsigned short usChannel, DWORD& dwColor)
{
    switch(usChannel)
    {
    case _TXTATR_GHOST:
        dwColor = m_colorChannelTalk;
        break;
    case _TXTATR_NORMAL:
        dwColor = m_colorChannelServe;
        break;
    case _TXTATR_TALK:
        dwColor = m_colorChannelTalk;
        break;
    case _TXTATR_FRIEND:
        dwColor = m_colorChannelFriend;
        break;
    case _TXTATR_SYNDICATE:
        dwColor = m_colorChannelSyndicate;
        break;
    case _TXTATR_TEAM:
        dwColor = m_colorChannelTeam;
        break;
    case _TXTATR_PRIVATE:
        dwColor = m_colorChannelPrivate;
        break;
    case _TXTATR_ACTION:
        dwColor = m_colorChannelAction;
        break ;
    case _TXTATR_SERVE:
        dwColor = m_colorChannelServe;
        break ;
    default:
        break;
    }
}
//--------------------------------------------------------------------
void	CGameMsg::SetChannelColor(unsigned short usChannel, DWORD dwColor)
{
    switch(usChannel)
    {
    case _TXTATR_NORMAL:
    case _TXTATR_GHOST:
    case _TXTATR_TALK:
        m_colorChannelTalk = dwColor;
        break;
    case _TXTATR_FRIEND:
        m_colorChannelFriend = dwColor;
        break;
    case _TXTATR_SYNDICATE:
        m_colorChannelSyndicate = dwColor;
        break;
    case _TXTATR_TEAM:
        m_colorChannelTeam = dwColor;
        break;
    case _TXTATR_PRIVATE:
        m_colorChannelPrivate = dwColor;
        break;
    case _TXTATR_ACTION:
        m_colorChannelAction = dwColor;
        break ;
    case _TXTATR_SERVE:
        m_colorChannelServe = dwColor;
        break ;
    default:
        break;
    }
}
//--------------------------------------------------------------------
void	CGameMsg::SetChannelStatus(unsigned short usChannel, BOOL bOn)
{
    switch(usChannel)
    {
    case _TXTATR_TALK:
        m_bChannelTalk = bOn;
        break;
    case _TXTATR_FRIEND:
        m_bChannelFriend = bOn;
        break;
    case _TXTATR_SYNDICATE:
        m_bChannelSyndicate = bOn;
        break;
    case _TXTATR_TEAM:
        m_bChannelTeam = bOn;
        break;
    case _TXTATR_ACTION:
        m_bChannelAction = bOn;
        break ;
    case _TXTATR_SERVE:
        m_bChannelServe = bOn;
        break ;
    default:
        break;
    }
}
//--------------------------------------------------------------------
BOOL	CGameMsg::GetChannelStatus(unsigned short usChannel)
{
    switch(usChannel)
    {
    case _TXTATR_TALK:
    case _TXTATR_NORMAL:
        return m_bChannelTalk;
    case _TXTATR_FRIEND:
        return m_bChannelFriend;
    case _TXTATR_SYNDICATE:
        return m_bChannelSyndicate;
    case _TXTATR_TEAM:
        return m_bChannelTeam;
    case _TXTATR_ACTION:
        return m_bChannelAction;
    case _TXTATR_SERVE:
        return m_bChannelServe;
    case _TXTATR_PRIVATE:
        return m_bChannelPrivateExceptFriend;
    default:
        return true;
    }
    return true;
}
//--------------------------------------------------------------------
void	CGameMsg::SetPrivateChannelStatusExceptFriend(BOOL bOn)
{
    m_bChannelPrivateExceptFriend = bOn;
}
//--------------------------------------------------------------------
void	CGameMsg::SetPrivateChannelStatusFriendOnly(BOOL bOn)
{
    m_bChannelPrivateFriend = bOn;
}
//--------------------------------------------------------------------
BOOL	CGameMsg::TestPriveteChannel(char* pszName)
{
    if (m_bChannelPrivateExceptFriend)
    {
        return true;
    }
    if (m_bChannelPrivateFriend)
    {
        if (!pszName)
        {
            return false;
        }
        if (g_objHero.GetFriendByName(pszName))
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void	CGameMsg::SetNorAreaShowStates ( const CMyPos& pos, int nWidth )
{
    m_NorAreaShowPos.x = pos.x ;
    m_NorAreaShowPos.y = pos.y ;
    m_nNorAreaWidth = nWidth ;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void	CGameMsg::SetSysAreaShowStates ( const CMyPos& pos, int nWidth )
{
    m_SysAreaShowPos.x = pos.x ;
    m_SysAreaShowPos.y = pos.y ;
    m_nSysAreaWidth = nWidth ;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void	CGameMsg::SetManAreaShowStates ( const CMyPos& pos, int nWidth )
{
    m_ManAreaShowPos.x = pos.x ;
    m_ManAreaShowPos.y = pos.y ;
    m_nManAreaWidth = nWidth ;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void	CGameMsg::SetCurShowChannel( unsigned short usCurChannel )
{
    m_usCurShowChannel = usCurChannel ;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
BOOL	CGameMsg::SetLastShowLineIndex( int nIndex, unsigned short usChannel )
{
    if ( usChannel < 0 || usChannel > 13 || m_nMaxLine >= (int)m_dequeMsg[usChannel].size() )
    {
        return false ;
    }
    else
    {
        if ( nIndex == -1 )
        {
            if ( m_nLastShowLineIndex[usChannel] < (int)m_dequeMsg[usChannel].size() )
            {
                m_nLastShowLineIndex[usChannel]++ ;
            }
        }
        else
        {
            if ( m_nLastShowLineIndex[usChannel] > m_nMaxLine )
            {
                m_nLastShowLineIndex[usChannel]-- ;
            }
        }
    }
    return true ;
}
//--------------------------------------------------------------------
BOOL	CGameMsg::GetLeaveWordByIndex(int nIndex, char* szName, char* strTxt, char* strTime)
{
    int nAmount = m_dequeLeaveMsg.size();
    if (nIndex > nAmount || nIndex < 0)
    {
        return false;
    }
    CGameMsgUnit* pMsg = m_dequeLeaveMsg[nIndex];
    if (pMsg)
    {
        strcpy( strTxt, pMsg->m_szMsg ) ;
        strcpy( szName, pMsg->m_szSender ) ;
        int nHour, nMinute, nMonth, nDay;
        strncpy(strTime, pMsg->m_szEmotion + 4, 8);
        int nTime = atoi(strTime);
        nMonth = ( nTime / 1000000) % 100;
        nDay = ( nTime / 10000 ) % 100;
        nHour	= ( nTime / 100 ) % 100 ;
        nMinute	=  nTime  % 100 ;
        sprintf( strTime, "【%d-%d %d:%d】", nMonth, nDay, nHour, nMinute ) ;
        return true ;
    }
    return false;
}
//--------------------------------------------------------------------
BOOL	CGameMsg::GetLeaveWordByIndexName( int nIndex, char* strName, char* strTxt,  char* strTime )
{
    int nAmount = m_dequeLeaveMsg.size();
    int nReAmount = 0;
    for(int i = 0; i < nAmount; i++)
    {
        CGameMsgUnit* pMsg = m_dequeLeaveMsg[i];
        if (pMsg && 0 == strcmp(pMsg->m_szSender, strName) )
        {
            if (nReAmount == nIndex)
            {
                strcpy( strTxt, pMsg->m_szMsg ) ;
                int nHour, nMinute, nMonth, nDay;
                strncpy(strTime, pMsg->m_szEmotion + 4, 8);
                int nTime = atoi(strTime);
                nMonth = ( nTime / 1000000) % 100;
                nDay = ( nTime / 10000 ) % 100;
                nHour	= ( nTime / 100 ) % 100 ;
                nMinute	=  nTime  % 100 ;
                sprintf( strTime, "【%d-%d %d:%d】", nMonth, nDay, nHour, nMinute ) ;
                return true ;
            }
            nReAmount++;
        }
    }
    return false;
}
//--------------------------------------------------------------------
int    CGameMsg::GetLeaveWordAmountByName(const char* pszName)
{
    int nAmount = m_dequeLeaveMsg.size();
    int nReAmount = 0;
    for(int i = 0; i < nAmount; i++)
    {
        CGameMsgUnit* pMsg = m_dequeLeaveMsg[i];
        if ( 0 == strcmp(pMsg->m_szSender, pszName) )
        {
            nReAmount++;
        }
    }
    return nReAmount;
}
//--------------------------------------------------------------------
int		CGameMsg::GetLeaveWordAmount()
{
    return m_dequeLeaveMsg.size() ;
}
//--------------------------------------------------------------------
void	CGameMsg::DelLeaveWordByIndexName(int nIndex, char* strName)
{
    int nAmount = this->GetLeaveWordAmount();
    int nReAmount = this->GetLeaveWordAmountByName(strName);
    if (nIndex < 0 || nIndex > nReAmount )
    {
        return;
    }
    for(int i = nAmount - 1; i >= 0 ; i--)
    {
        CGameMsgUnit* pMsg = m_dequeLeaveMsg[i];
        if (pMsg && 0 == strcmp(pMsg->m_szSender, strName))
        {
            nReAmount--;
            if (nIndex == nReAmount)
            {
                m_dequeLeaveMsg.erase(m_dequeLeaveMsg.begin() + i);
                return;
            }
        }
    }
}
//--------------------------------------------------------------------
void	CGameMsg::DestroyLeaveWord()
{
    int nCount = m_dequeLeaveMsg.size() ;
    for ( int i = 0; i < nCount; i++ )
    {
        SAFE_DELETE( m_dequeLeaveMsg[i] ) ;
    }
    m_dequeLeaveMsg.clear() ;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void	CGameMsg::SetShowLines( UINT uMaxLines )
{
    m_nMaxLine = (int)uMaxLines ;
    for ( int i = 0; i < 14; i++ )
    {
        m_nLastShowLineIndex[i] = m_nMaxLine ;
    }
}
//--------------------------------------------------------------------

void CGameMsg::GetShowLinesEx(int& nMaxUpLines, int& nMaxDownLines)
{
    nMaxUpLines = m_nMaxUpLine;
    nMaxDownLines = m_nMaxDownLine;
    return;
}

void CGameMsg::SetShowLinesEx(UINT uMaxUpLines, UINT uMaxDownLines)
{
    m_nMaxUpLine = (int)uMaxUpLines;
    m_nMaxDownLine = (int)uMaxDownLines;
}

int CGameMsg::GetNorAeroTop()
{
    int ntopy = m_NorAreaShowPos.y - (MSGFONT_HEIGHT + 6) * m_nMaxLine;
    return ntopy;
}



int CGameMsg::GetNorUpAreaTop()
{
    int nTop = m_NorAreaShowPos.y - (MSGFONT_HEIGHT + 4) * (m_nMaxUpLine + m_nMaxDownLine + 1) - 20;
    return nTop;
}

int CGameMsg::GetNorDownAreaTop()
{
    int nTop = m_NorAreaShowPos.y - (MSGFONT_HEIGHT + 4 ) * (m_nMaxDownLine + 1);
    return nTop;
}


int CGameMsg::ShowEmotionWnd (int x, int y, int nPage, int nXAmount, int nYAmount)
{
    int nAmount = g_objGameDataSet.GetEmotionAmount ();
    int nPageAmount = nXAmount * nYAmount;
    if (nPage * nPageAmount > nAmount)
    {
        return -1;
    }
    bool bMouseIn = false;
    CMyPos pMousePoint = {0, 0};
    CMyPos pShowPoint = {0, 0};
    int  nMouseIndex = -1;
    int nIndex = nPage * nPageAmount;
    nAmount = nAmount - nIndex;
    if (nAmount > nPageAmount)
    {
        nAmount = nPageAmount;
    }
    for(int i = 0; i < nAmount; i++)
    {
        CAni* pAni = g_objGameDataSet.GetEmotionIcon (i + nIndex);
        if (pAni)
        {
            int nFrame = ::TimeGet() / 200 % pAni->GetFrameAmount();
            pShowPoint.x = x + (i % nXAmount) * (32);
            pShowPoint.y = y + (i / nXAmount) * (32);
            CMyPos PointE = {pShowPoint.x + 32, pShowPoint.y + 32};
            CMyPos pMousePos = {0, 0};
            ::MouseCheck(pMousePos.x, pMousePos.y);
            if (pMousePos.x > pShowPoint.x - 2 && pMousePos.y > pShowPoint.y - 2
                    && pMousePos.x < PointE.x && pMousePos.y < PointE.y )
            {
                CMyBitmap::ShowRect(pShowPoint.x , pShowPoint.y, PointE.x, PointE.y, 0x4e00ff00);
                bMouseIn = true;
                pMousePoint = pShowPoint;
                nMouseIndex = i + nIndex;
            }
            pAni->Show( nFrame, pShowPoint.x, pShowPoint.y); //,NULL,22,22);
        }
    }
    if (bMouseIn)
    {
        char szCode[_MAX_NAMESIZE] = "";
        if (nMouseIndex < 10)
        {
            sprintf(szCode, "#0%d", nMouseIndex);
        }
        else
        {
            sprintf(szCode, "#%d", nMouseIndex);
        }
        char szEmotionShutCut[_MAX_NAMESIZE] = "";
        strcpy(szEmotionShutCut, g_objGameDataSet.GetEmotionShutCut (nMouseIndex));
        int nMsgLength = max((int)strlen(szCode), (int)strlen(szEmotionShutCut));
        CMyBitmap::ShowBlock(pMousePoint.x - 2 , pMousePoint.y - 35, pMousePoint.x + nMsgLength * 7, pMousePoint.y - 3, 0xff000000);
        CMyBitmap::ShowString(pMousePoint.x , pMousePoint.y - 33, 0xffffffff, szCode);
        if (szEmotionShutCut && strlen(szEmotionShutCut) > 0)
        {
            pMousePoint.y = pMousePoint.y - 19;
            CMyBitmap::ShowString(pMousePoint.x , pMousePoint.y, 0xffffffff, szEmotionShutCut);
        }
    }
    return nMouseIndex;
}
