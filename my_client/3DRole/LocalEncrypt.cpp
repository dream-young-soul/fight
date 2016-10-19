
//**********************************************************
// 代码编辑器
//**********************************************************

// LocalEncrypt
// 仙剑修，2002.3.27

////////////////////////////////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "nb30.h"
#ifdef	PALED_DEBUG
#include "assert.h"

#define LOGERROR printf
#endif
const int	MAX_PASSWORDSIZE	= 16;
const int	MAX_MACADDRSIZE		= 6;



typedef struct tagASTAT
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
} ASTAT, *LPASTAT;

//存储网卡的MAC地址的结构
typedef struct tagMAC_ADDRESS
{
    BYTE b1, b2, b3, b4, b5, b6;
} MAC_ADDRESS, *LPMAC_ADDRESS;

//网卡信息的数据结构，包括记录网卡的厂商及型号，与之绑定的IP地址，网关
//DNS序列，子网掩码和物理地址
typedef struct tagNET_CARD
{
    TCHAR szDescription[256];
    BYTE  szMacAddr[6];
    TCHAR szGateWay[128];
    TCHAR szIpAddress[128];
    TCHAR szIpMask[128];
    TCHAR szDNSNameServer[128];
} NET_CARD, *LPNET_CARD;


/*********************************************************************
 * 函数名称:CTcpCfg::GetAddressByIndex
 * 说明:得到索引号为lana_num的网卡的状态信息
 * 入口参数:
 * int lana_num -- 网卡的索引号
 * ASTAT & Adapter -- 用于返回网卡的状态信息
 * 作者: orbit
 * 时间 : 2000-12-10 20:14:38
*********************************************************************/
UCHAR GetAddressByIndex(int lana_num, ASTAT& Adapter)
{
    NCB ncb;
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = lana_num;
    //指定网卡号,首先对选定的网卡发送一个NCBRESET命令,以便进行初始化
    uRetCode = Netbios(&ncb );
    memset(&ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_num;   //指定网卡号
    strcpy((char*)ncb.ncb_callname, "*      " );
    ncb.ncb_buffer = (unsigned char*)&Adapter;
    //指定返回的信息存放的变量
    ncb.ncb_length = sizeof(Adapter);
    //接着,可以发送NCBASTAT命令以获取网卡的信息
    uRetCode = Netbios(&ncb );
    return uRetCode;
}


/*********************************************************************
 * 函数名称:CTcpCfg::GetMacAddress
 * 说明:得到系统安装的所有网卡的Mac地址并返回网卡的数目
 * 入口参数:
 * LPMAC_ADDRESS pMacAddr -- 指向网卡Mac地址结构数组的指针
 * 返回值:
 * int  -- 网卡的数目
 * 作者: orbit
 * 时间 : 2000-12-10 19:48:15
*********************************************************************/
bool GetMacAddr(char bufMacAddr[12])
{
    NCB ncb;
    UCHAR uRetCode;
    int num = 0;
    LANA_ENUM lana_enum;
    memset(&ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (unsigned char*)&lana_enum;
    ncb.ncb_length = sizeof(lana_enum);
    //向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡
    //每张网卡的编号等
    uRetCode = Netbios(&ncb);
    if (uRetCode == 0)
    {
        num = lana_enum.length;
        //对每一张网卡,以其网卡编号为输入编号,获取其MAC地址
        for (int i = 0; i < num; i++)
        {
            ASTAT Adapter;
            if (GetAddressByIndex(lana_enum.lana[i], Adapter) == 0)
            {
                memcpy(bufMacAddr, Adapter.adapt.adapter_address, MAX_MACADDRSIZE);
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 外部函数
bool		SaveUserName(LPCTSTR szUserName)		// return false: can't save.
{
    LONG lRtn;
    HKEY hMainKey;
    lRtn = ::RegCreateKey(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), &hMainKey);
    ::RegCloseKey(hMainKey);
    lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), 0, KEY_WRITE, &hMainKey);
    if (lRtn == ERROR_SUCCESS)
    {
        DWORD dwDataSize = MAX_PASSWORDSIZE;
        ::RegSetValueEx(hMainKey, ("NetBarUserName"), 0, REG_BINARY, (LPBYTE)szUserName, MAX_PASSWORDSIZE);
        ::RegCloseKey(hMainKey);
        return true;
    }
    else
    {
        ::RegCloseKey(hMainKey);
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 外部函数
bool		GetUserName(char* buf16)		// return bool: Don't Found UserName.
{
    LONG lRtn;
    HKEY hMainKey;
    lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), 0, KEY_READ, &hMainKey);
    if (lRtn == ERROR_SUCCESS)
    {
        DWORD dwType, dwDataSize = MAX_PASSWORDSIZE;
        ::RegQueryValueEx(hMainKey, ("NetBarUserName"), NULL, &dwType, (LPBYTE)buf16, &dwDataSize);
        ::RegCloseKey(hMainKey);
        return true;
    }
    else
    {
        ::RegCloseKey(hMainKey);
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 外部函数
bool		SavePassword(LPCTSTR szPassword)		// return false: can't save.
{
    if (!szPassword || !szPassword[0])
    {
        return false;
    }
    try
    {
        char	bufMacAddr[MAX_PASSWORDSIZE];
        if (GetMacAddr(bufMacAddr))
        {
#ifdef	PALED_DEBUG
            {
                LPCTSTR pBuf = bufMacAddr;
                int		nLen = MAX_MACADDRSIZE;
                char	bufText[1024];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
                }
                LOGERROR("MAC：%s\n", bufText);
            }
#endif
#ifdef	PALED_DEBUG
            {
                LPCTSTR pBuf = szPassword;
                int		nLen = MAX_PASSWORDSIZE;
                char	bufText[1024];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
                }
                LOGERROR("PIN：%s\n", bufText);
            }
#endif
            //			assert(MAX_MACADDRSIZE == 6);
            //			assert(MAX_PASSWORDSIZE == 16);
            memcpy(bufMacAddr + MAX_MACADDRSIZE, bufMacAddr, MAX_MACADDRSIZE);
            memcpy(bufMacAddr + MAX_MACADDRSIZE * 2, bufMacAddr, MAX_PASSWORDSIZE - MAX_MACADDRSIZE);
            for(int i = 0; i < MAX_PASSWORDSIZE; i++)
            {
                char cTemp = bufMacAddr[i];
                bufMacAddr[i] = cTemp ^ szPassword[i];
            }
            LONG lRtn;
            HKEY hMainKey;
            lRtn = ::RegCreateKey(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), &hMainKey);
            ::RegCloseKey(hMainKey);
            lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), 0, KEY_WRITE, &hMainKey);
            if (lRtn == ERROR_SUCCESS)
            {
                DWORD dwDataSize = MAX_PASSWORDSIZE;
                ::RegSetValueEx(hMainKey, ("NetBarPassword"), 0, REG_BINARY, (LPBYTE)bufMacAddr, MAX_PASSWORDSIZE);
            }
            ::RegCloseKey(hMainKey);
#ifdef	PALED_DEBUG
            {
                LPCTSTR pBuf = bufMacAddr;
                int		nLen = MAX_PASSWORDSIZE;
                char	bufText[1024];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
                }
                LOGERROR("REG：%s\n\n", bufText);
            }
#endif
            return true;
        }
    }
    catch(...)
    {
        //	LOGERROR("CATCH: SavePassword()");
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 外部函数
bool		GetPassword(char* buf16)		// return bool: Don't Found password.
{
    try
    {
        LONG lRtn;
        HKEY hMainKey;
        char szPassword[MAX_PASSWORDSIZE];
        char szTemp[MAX_PASSWORDSIZE];
        lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\天晴娱乐\\幻灵游侠"), 0, KEY_READ, &hMainKey);
        if (lRtn == ERROR_SUCCESS)
        {
            DWORD dwType, dwDataSize = MAX_PASSWORDSIZE;
            ::RegQueryValueEx(hMainKey, ("NetBarPassword"), NULL, &dwType, (LPBYTE)szTemp, &dwDataSize);
            memcpy(szPassword, szTemp, MAX_PASSWORDSIZE);
        }
        ::RegCloseKey(hMainKey);
#ifdef	PALED_DEBUG
        {
            LPCTSTR pBuf = szPassword;
            int		nLen = MAX_PASSWORDSIZE;
            char	bufText[1024];
            for(int i = 0; i < nLen; i++)
            {
                sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
            }
            LOGERROR("REG：%s\n", bufText);
        }
#endif
        char	bufMacAddr[MAX_PASSWORDSIZE];
        if (GetMacAddr(bufMacAddr))
        {
#ifdef	PALED_DEBUG
            {
                LPCTSTR pBuf = bufMacAddr;
                int		nLen = MAX_MACADDRSIZE;
                char	bufText[1024];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
                }
                LOGERROR("MAC：%s\n", bufText);
            }
#endif
            //			assert(MAX_MACADDRSIZE == 6);
            //			assert(MAX_PASSWORDSIZE == 16);
            memcpy(bufMacAddr + MAX_MACADDRSIZE, bufMacAddr, MAX_MACADDRSIZE);
            memcpy(bufMacAddr + MAX_MACADDRSIZE * 2, bufMacAddr, MAX_PASSWORDSIZE - MAX_MACADDRSIZE);
            for(int i = 0; i < MAX_PASSWORDSIZE; i++)
            {
                char cTmep = szPassword[i];
                szPassword[i] = cTmep ^ bufMacAddr[i];
            }
            szPassword[MAX_PASSWORDSIZE - 1] = 0;
            strcpy(buf16, szPassword);
#ifdef	PALED_DEBUG
            {
                LPCTSTR pBuf = szPassword;
                int		nLen = MAX_PASSWORDSIZE;
                char	bufText[1024];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(bufText + 3 * i, "%02X ", (unsigned char)pBuf[i]);
                }
                LOGERROR("PIN：%s\n", bufText);
            }
#endif
            return true;
        }
    }
    catch(...)
    {
        //LOGERROR("CATCH: GetPassword()");
    }
    buf16[0] = 0;
    return false;
}
