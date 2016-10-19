
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: HeroSchool.cpp
// Create by: Huang Yuhang
// Create on: 2004/9/4 14:59
//--------------------------------------------------------------------------------------
#include "Hero.h"
#include "AllMsg.h"
#include "GamePlayerSet.h"
//--------------------------------------------------------------------------------------
BOOL	CHero::AddSchoolMember(CSchoolMember* pSchoolMember)
{
    if (!pSchoolMember)
    {
        return false;
    }
    if (this->GetSchoolMember(pSchoolMember->GetID()))
    {
        return false;
    }
    m_setSchoolMember.push_back(pSchoolMember);
    {
        ::PostCmd(CMD_FLASH_SCHOOLINFO);
        return true;
    }
}

//--------------------------------------------------------------------------------------
BOOL	CHero::DelSchoolMember(OBJID id)
{
    if (id == ID_NONE)
    {
        return false;
    }
    int nAmount = m_setSchoolMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        CSchoolMember* pSchoolMember = m_setSchoolMember[i];
        if (pSchoolMember && pSchoolMember->GetID() == id)
        {
            SAFE_DELETE(pSchoolMember);
            m_setSchoolMember.erase(m_setSchoolMember.begin() + i);
            ::PostCmd(CMD_FLASH_SCHOOLINFO);
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
int		CHero::GetSchoolMemberAmount()
{
    return m_setSchoolMember.size();
}
//--------------------------------------------------------------------------------------
CSchoolMember* CHero::GetSchoolMember(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    int nAmount = m_setSchoolMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        CSchoolMember* pSchoolMember = m_setSchoolMember[i];
        if (pSchoolMember && pSchoolMember->GetID() == id)
        {
            return pSchoolMember;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
CSchoolMember* CHero::GetSchoolMemberByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nAmount = m_setSchoolMember.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setSchoolMember[nIndex];
}
//--------------------------------------------------------------------------------------
CSchoolMember* CHero::GetSchoolMemberByName(const char* pszName)
{
    if (!pszName)
    {
        return NULL;
    }
    int nAmount = this->GetSchoolMemberAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CSchoolMember* pSchoolMember = this->GetSchoolMemberByIndex(i);
        if ((pSchoolMember) && (strcmp(pSchoolMember->GetName(), pszName) == 0 ))
        {
            return pSchoolMember;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
void	CHero::ClearSchoolMemberList()
{
    int nAmount = m_setSchoolMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        CSchoolMember* pSchoolMember = m_setSchoolMember[i];
        SAFE_DELETE(pSchoolMember);
    }
    m_setSchoolMember.clear();
}
//--------------------------------------------------------------------------------------
BOOL	CHero::TeacherApply(OBJID idPlayer)
{
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (!pPlayer)
    {
        return false;
    }
    CSchoolMember* pSchoolMember = this->GetSchoolMember(idPlayer);
    if (pSchoolMember)
    {
        if (RELATION_TEACHER == pSchoolMember->GetRelation() )
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100201)); //此人已经是你的师父
            return false;
        }
        else if (RELATION_STUDENT == pSchoolMember->GetRelation() )
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100202));	//此人已经是你的徒弟
            return false;
        }
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionTeacherApply, idPlayer))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------------------------------
BOOL	CHero::StudentApply(OBJID idPlayer)
{
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (!pPlayer)
    {
        return false;
    }
    CSchoolMember* pSchoolMember = this->GetSchoolMember(idPlayer);
    if (pSchoolMember)
    {
        if (RELATION_TEACHER == pSchoolMember->GetRelation() )
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100201));
            return false;
        }
        else if (RELATION_STUDENT == pSchoolMember->GetRelation() )
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100202));
            return false;
        }
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionStudentApply, idPlayer))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------------------------------
BOOL	CHero::AgreeTeacherApply()
{
    if (m_idLastTeacherApply == ID_NONE)
    {
        return false;
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionAgreeTeacherApply, m_idLastTeacherApply))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------------------------------
BOOL	CHero::AgreeStudentApply()
{
    if (m_idLastStudentApply == ID_NONE)
    {
        return false;
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionAgreeStudentApply, m_idLastStudentApply))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	CHero::SetLastTeacherApply(OBJID idPlayer)
{
    m_idLastTeacherApply = ID_NONE;
    strcpy(m_szLastTeacherApply, "");
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (pPlayer)
    {
        m_idLastTeacherApply = idPlayer;
        strcpy(m_szLastTeacherApply, pPlayer->GetName());
        ::PostCmd(CMD_ACTION_TEACHAPPLY);
    }
}
//--------------------------------------------------------------------------------------
void	CHero::SetLastStudentApply(OBJID idPlayer)
{
    m_idLastStudentApply = ID_NONE;
    strcpy(m_szLastStudentApply, "");
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (pPlayer)
    {
        m_idLastStudentApply = idPlayer;
        strcpy(m_szLastStudentApply, pPlayer->GetName());
        ::PostCmd(CMD_ACTION_STUDENTAPPLY);
    }
}
//--------------------------------------------------------------------------------------
const	char*	CHero::GetLastTeacherApply()
{
    return m_szLastTeacherApply;
}
//--------------------------------------------------------------------------------------
const	char*	CHero::GetLastStudentApply()
{
    return m_szLastStudentApply;
}
//--------------------------------------------------------------------------------------

BOOL CHero::DismissStudent(OBJID idPlayer)
{
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionDismissStudent, idPlayer))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------------------------------

BOOL CHero::LeaveTeacher()
{
    int nAmount = this->GetSchoolMemberAmount();
    if (nAmount > 0)
    {
        for(int i = 0; i < nAmount; i++)
        {
            CSchoolMember* pSchoolMember = this->GetSchoolMemberByIndex(i);
            if (pSchoolMember && pSchoolMember->GetRelation() == RELATION_TEACHER)
            {
                CMsgAction msg;
                if (msg.Create(this->GetID(), 0, 0, 0, actionLeaveTeacher, pSchoolMember->GetID()))
                {
                    msg.Send();
                }
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL CHero::IsHaveTeacher()
{
    int nAmount = this->GetSchoolMemberAmount();
    if (nAmount > 0)
    {
        for(int i = 0; i < nAmount; i++)
        {
            CSchoolMember* pSchoolMember = this->GetSchoolMemberByIndex(i);
            if (pSchoolMember->GetRelation() == RELATION_TEACHER)
            {
                return true;
            }
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------
BOOL CHero::IsHaveStudent()
{
    int nAmount = this->GetSchoolMemberAmount();
    if (nAmount > 0)
    {
        for(int i = 0; i < nAmount; i++)
        {
            CSchoolMember* pSchoolMember = this->GetSchoolMemberByIndex(i);
            if (pSchoolMember->GetRelation() == RELATION_STUDENT)
            {
                return true;
            }
        }
    }
    return false;
}

BOOL CHero::UpdataSchoolMember(OBJID id, UCHAR ucStatus, char* pszName, UCHAR ucRelation, char* pszMate, UCHAR ucLevel, UCHAR ucProfession, USHORT usPkValue, UCHAR ucNobilityRank, DWORD dwSynID_Rank)
{
    CSchoolMember* pSchoolMember = this->GetSchoolMember(id);
    if (pSchoolMember)
    {
        pSchoolMember->SetStatus(ucStatus);
        pSchoolMember->SetName(pszName);
        pSchoolMember->SetRelation(ucRelation);
        pSchoolMember->SetMate(pszMate);
        pSchoolMember->SetLevel(ucLevel);
        pSchoolMember->SetNobilityRank(ucNobilityRank);
        pSchoolMember->SetProfession(ucProfession);
        pSchoolMember->SetPk(usPkValue);
        pSchoolMember->SetSynRank(dwSynID_Rank);
        ::PostCmd(CMD_FLASH_SCHOOLINFO);
        return true;
    }
    else
    {
        CSchoolMember* pSchoolMemberAdd = CSchoolMember::CreateNew(id,
                                          ucStatus, pszName, ucRelation, pszMate, ucLevel, ucProfession, usPkValue, ucNobilityRank, dwSynID_Rank);
        if (pSchoolMemberAdd)
        {
            this->AddSchoolMember(pSchoolMember);
            return true;
        }
    }
    return false;
}

void CHero::QuerySchoolMember()
{
    if (!m_bQuerySchoolMember)
    {
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionQuerySchoolMember))
        {
            msg.Send();
        }
    }
}

void CHero::SetQuerySchoolMember(bool bFlag)
{
    m_bQuerySchoolMember = bFlag;
}

void CHero::QueryTeacherBoardInfo(OBJID idTBoard)
{
    CMsgAnnounceInfo msgAnnounceInfo;
    msgAnnounceInfo.QueryAnnounceInfo(idTBoard);
}

void CHero::QueryTeacherBoardList(int nIndex)
{
    CMsgAnnounceList msgAnnounceList;
    if (msgAnnounceList.Create(CHECK_BY_INDEX, nIndex))
    {
        msgAnnounceList.Send();
    }
}

void CHero::QueryMyTeacherBoard()
{
    CMsgAnnounceInfo msgAnnounceInfo;
    msgAnnounceInfo.QueryMyAnnounceInfo();
}

void CHero::UpTeacherBoard(char* pszTitle, char* pszInfo)
{
    CMsgAnnounceInfo msgAnnounceInfo;
    msgAnnounceInfo.UpAnnounce(pszTitle, pszInfo);
}

void CHero::CancelTeacherBoard(OBJID idAnnounce)
{
    CMsgAnnounceInfo msgAnnounceInfo;
    msgAnnounceInfo.CancelAnnounce(idAnnounce);
}

void CHero::RequestTeacherBoard(OBJID idUser)
{
    if (idUser)
    {
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionTeacherRequest, idUser))
        {
            msg.Send();
        }
    }
}

void CHero::ClearAnnounceList()
{
    int nAmount = m_setAnnounceList.size();
    for(int i = 0; i < nAmount; i++)
    {
        ST_ANNOUNCE_TITLE* pAnnounceTitle = m_setAnnounceList[i];
        SAFE_DELETE(pAnnounceTitle);
    }
    m_setAnnounceList.clear();
}

ST_ANNOUNCE_TITLE* CHero::GetAnnounceByIndex(int nIndex)
{
    int nAmount = m_setAnnounceList.size();
    if (nIndex >= nAmount || nIndex < 0)
    {
        return NULL;
    }
    return m_setAnnounceList[nIndex];
}


int CHero::GetAnnounceAmount()
{
    return m_setAnnounceList.size();
}

void CHero::AddAnnounceList(OBJID idAnnounce, const char* szTitle)
{
    ST_ANNOUNCE_TITLE* pAnnounceTitle = new ST_ANNOUNCE_TITLE;
    MYASSERT(pAnnounceTitle);
    pAnnounceTitle->idAnnounce = idAnnounce;
    strcpy(pAnnounceTitle->szTitle, szTitle);
    m_setAnnounceList.push_back(pAnnounceTitle);
}


int CHero::GetSecSchoolMemberAmount()
{
    return m_setSecSchoolMember.size();
}

const char* CHero::GetSecSchoolMemberByIndex(int nIndex)
{
    int nAmount = m_setSecSchoolMember.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    else
    {
        return m_setSecSchoolMember[nIndex].c_str();
    }
}

void CHero::AddSecSchoolMember(char* strName)
{
    if (strName && strlen(strName) > 0)
    {
        string str = strName;
        m_setSecSchoolMember.push_back(str);
    }
    else
    {
        return;
    }
}

void CHero::QuerySecSchoolMember(OBJID IdMember)
{
    if (IdMember == ID_NONE)
    {
        return;
    }
    else
    {
        m_setSecSchoolMember.clear();
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionQueryStudentsOfStudent, IdMember))
        {
            msg.Send();
        }
    }
}
