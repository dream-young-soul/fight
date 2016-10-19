
//**********************************************************
// 代码编辑器
//**********************************************************

// Check.cpp: implementation of the CCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "Check.h"
#include "Tlhelp32.h"
#include "AllMsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheck::CCheck()
{
    m_dwTimeCheck = ::TimeGet() + 5 * 60 * 1000 + ::RandGet(5 * 60 * 1000);
    m_bReportWSProcess = false;
    m_bReportFYProcess = false;
    m_bReportUseLifeMedicine = false;
    m_bReportUseManaMedicine = false;
    m_nUseLifeMedicineAmount = 0;
    m_nUseManaMedicineAmount = 0;
    m_dwLastDecLife = 0;
    m_dwLastDecMana = 0;
}

CCheck::~CCheck()
{
}

void CCheck::Reset()
{
    m_dwTimeCheck = ::TimeGet() + 5 * 60 * 1000 + ::RandGet(5 * 60 * 1000);
    m_bReportWSProcess = false;
    m_bReportFYProcess = false;
    m_bReportUseLifeMedicine = false;
    m_bReportUseManaMedicine = false;
    m_nUseLifeMedicineAmount = 0;
    m_nUseManaMedicineAmount = 0;
    m_dwLastDecLife = 0;
    m_dwLastDecMana = 0;
}

BOOL CCheck::Process()
{
    if (m_bReportWSProcess &&
            m_bReportFYProcess && m_bReportUseLifeMedicine &&
            m_bReportUseManaMedicine)
    {
        return false;
    }
    if (::TimeGet() < m_dwTimeCheck)
    {
        return false;
    }
    // check ...
#ifndef _DEBUG
    PROCESSENTRY32 pe;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    //	BOOL bFound = false;
    int	 nType = 0;
    BOOL bDevFound = false;
    // 先搜索系统中第一个进程的信息
    Process32First(hSnapshot, &pe);
    // 下面对系统中的所有进程进行枚举
    do
    {
        int nLength = strlen(pe.szExeFile);
        if (strstr(pe.szExeFile, "MSDEV.EXE"))
        {
            bDevFound = true;
        }
        strlwr(pe.szExeFile);
        if (
            nLength == 8 &&
            pe.szExeFile[4] == '.' && pe.szExeFile[0] == 'z' && pe.szExeFile[2] == 'w' &&
            pe.szExeFile[5] == 'e' && pe.szExeFile[7] == 'e' && pe.szExeFile[1] == 'f' &&
            pe.szExeFile[3] == 's' && pe.szExeFile[6] == 'x' && pe.cntThreads >= 2 && !m_bReportWSProcess
        )
        {
            m_bReportWSProcess = true;
            nType = _TYPE_WS;
            if (!bDevFound)
            {
                CMsgAction msgAction;
                if (msgAction.Create(809834, 0, 0, nType, actionXpCLear, 100))
                {
                    msgAction.Send();
                }
            }
        }
        if (
            nLength == 10 &&
            pe.szExeFile[4] == 'o' && pe.szExeFile[0] == 'z' && pe.szExeFile[2] == 'h' &&
            pe.szExeFile[5] == 'k' && pe.szExeFile[7] == 'e' && pe.szExeFile[1] == 'f' &&
            pe.szExeFile[3] == 'o' && pe.szExeFile[6] == '.' && pe.szExeFile[8] == 'x' && !m_bReportFYProcess
        )
        {
            m_bReportFYProcess = true;
            nType = _TYPE_FY;
            if (!bDevFound)
            {
                CMsgAction msgAction;
                if (msgAction.Create(809834, 0, 0, nType, actionXpCLear, 100))
                {
                    msgAction.Send();
                }
            }
        }
    }
    while (Process32Next(hSnapshot, &pe));
    // 关闭快照句柄
    CloseHandle(hSnapshot);
    if (m_nUseLifeMedicineAmount > 8 && !m_bReportUseLifeMedicine)
    {
        m_bReportUseLifeMedicine = true;
        nType = _TYPE_USE_LIFE_MEDICINE;
        CMsgAction msgAction;
        if (msgAction.Create(809834, 0, 0, nType, actionXpCLear, 100))
        {
            msgAction.Send();
        }
        m_nUseLifeMedicineAmount = 0;
    }
    if (m_nUseManaMedicineAmount > 8 && !m_bReportUseManaMedicine)
    {
        //		bFound = true;
        nType = _TYPE_USE_MANA_MEDICINE;
        CMsgAction msgAction;
        if (msgAction.Create(809834, 0, 0, nType, actionXpCLear, 100))
        {
            msgAction.Send();
        }
        m_nUseManaMedicineAmount = 0;
    }
    /*	if (bFound && !bDevFound)
    	{
    		CMsgAction msgAction;
    		if (msgAction.Create(809834, 0, 0, nType, actionXpCLear, 100))
    		{
    			m_bReport = true;
    			msgAction.Send();
    		}
    	}*/
    // reset time ...
    m_dwTimeCheck = ::TimeGet() + 5 * 60 * 1000 + ::RandGet(5 * 60 * 1000);
#endif
    return true;
}

void CCheck::DecLife()
{
    m_dwLastDecLife = ::TimeGet();
}

void CCheck::DecMana()
{
    m_dwLastDecMana = ::TimeGet();
}

void CCheck::AddLife()
{
    if (::TimeGet() - m_dwLastDecLife < 200)
    {
        m_nUseLifeMedicineAmount ++;
    }
    else
    {
        m_nUseLifeMedicineAmount = 0;
    }
}

void CCheck::AddMana()
{
    if (::TimeGet() - m_dwLastDecMana < 200)
    {
        m_nUseManaMedicineAmount ++;
    }
    else
    {
        m_nUseManaMedicineAmount = 0;
    }
}
