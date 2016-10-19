
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgSchoolMember.cpp
// Create by: Huang Yuhang
// Create on: 2004/9/4 16:10
//--------------------------------------------------------------------------------------
#include "AllMsg.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSchoolMember::CMsgSchoolMember()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgSchoolMember::~CMsgSchoolMember()
{
}

//-------------------------------------------------------------------

BOOL CMsgSchoolMember::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MESSAGESCHOOLMEMBER != this->GetType())
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------

void CMsgSchoolMember::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process CMsgSchoolMember,  Action:%d, Name:%s",
              m_pInfo->ucAction ,
              m_pInfo->setMember[0].szName);
#endif
    switch(m_pInfo->ucAction)
    {
    case MESSAGEBOARD_DELMEMBER:
        {
            int nAmount = m_pInfo->ucAmount;
            for(int i = 0; i < nAmount; i++)
            {
                OBJID idSchoolMember = m_pInfo->setMember[i].idMember;
                g_objHero.DelSchoolMember(idSchoolMember);
            }
        }
        break;
    case MESSAGEBOARD_ADDMEMBER:
        {
            int nAmount = m_pInfo->ucAmount;
            if (nAmount <= 0)
            {
                return;
            }
            if (m_pInfo->setMember[0].ucRelation == RELATION_STUDENTOFSTUDENT)
            {
                for(int j = 0; j < nAmount; j++)
                {
                    g_objHero.AddSecSchoolMember(m_pInfo->setMember[j].szName);
                }
                ::PostCmd(CMD_FLASH_SECSCHOOLLIST);
            }
            else
            {
                for(int i = 0; i < nAmount; i++)
                {
                    CSchoolMember* pSchoolMember = CSchoolMember::CreateNew(m_pInfo->setMember[i].idMember,
                                                   m_pInfo->setMember[i].ucStatus, m_pInfo->setMember[i].szName, m_pInfo->setMember[i].ucRelation,
                                                   m_pInfo->setMember[i].szMate, m_pInfo->setMember[i].ucLevel, m_pInfo->setMember[i].ucProfession,
                                                   m_pInfo->setMember[i].usPkValue, m_pInfo->setMember[i].ucNobilityRank, m_pInfo->setMember[i].dwSynID_Rank);
                    if (pSchoolMember)
                    {
                        g_objHero.AddSchoolMember(pSchoolMember);
                    }
                }
            }
        }
        break;
    case MESSAGEBOARD_UPDATEMEMBER:
        {
            int nAmount = m_pInfo->ucAmount;
            for(int i = 0; i < nAmount; i++)
            {
                g_objHero.UpdataSchoolMember(m_pInfo->setMember[i].idMember,
                                             m_pInfo->setMember[i].ucStatus, m_pInfo->setMember[i].szName, m_pInfo->setMember[i].ucRelation,
                                             m_pInfo->setMember[i].szMate, m_pInfo->setMember[i].ucLevel, m_pInfo->setMember[i].ucProfession,
                                             m_pInfo->setMember[i].usPkValue, m_pInfo->setMember[i].ucNobilityRank, m_pInfo->setMember[i].dwSynID_Rank);
            }
        }
        break;
    default:
        break;
    }
}
//-------------------------------------------------------------------
