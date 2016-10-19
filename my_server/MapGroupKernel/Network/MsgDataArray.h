
//**********************************************************
// 代码编辑器
//**********************************************************

///////////////////////////////////////////////////////////
//
//		CMsgDataArray.h
//

#ifndef		_MSGDATAARRAY_H_
#define		_MSGDATAARRAY_H_

#include "NetMsg.h"

enum
{
    _DATAARRAY_ACTION_MEDAL				=	0,	//	unsigned char	// 勋章
    _DATAARRAY_ACTION_HONORTITLE		=	1,	//	unsigned char	// 称号
    //---jinggy---2004-11-9---铸造系统---begin
    _DATAARRAY_ACTION_UPGRADE_QUALITY		= 2, //升级装备品质
    _DATAARRAY_ACTION_UPGRADE_SUPERADDITION	= 3, //升级装备追加
    _DATAARRAY_ACTION_UPGRADE_EQUIPLEVEL	= 4, //升级装备等级
    _DATAARRAY_ACTION_EMBEDGEM				= 5, //对装备嵌入宝石
    //---jinggy---2004-11-9---铸造系统---end

    //---jinggy---帮派系统---begin
    _DATAARRAY_ACTION_SETSYN_DRESS	= 50,	//设置帮派服装，一共四个等级的服装
    //---jinggy---帮派系统---end

    _DATAARRAY_SETITEMREBATE	= 60,	//设置物品的折扣


};
#define MAX_LEVEL_SUPERADDITION	9	//指追加属性的最高级别

class CMsgDataArray : public CNetMsg
{
public:
    CMsgDataArray();
    virtual ~CMsgDataArray();

public:		//接口函数
    BOOL	Create(int nAction, int nAmount, char setChar[]);
    BOOL	Create(int nAction, int nAmount, short setShort[]);
    BOOL	Create(int nAction, int nAmount, long setLong[]);

protected:
    int RateSuccForQuality(CItemPtr pEquipItem);
    int RateSuccForGhostLevel(CItemPtr pEquipItem);
    void SetSynDress(CUserPtr pUser);
public:
    bool Create(int nAction, int nAmount, unsigned long setULong[]);
    bool EmbedGemToEquip(CUserPtr pUser);
    bool UpEquipSuperAddition(CUserPtr pUser);
    bool UpEquipQuality(CUserPtr pUser);
    int RateSuccForEquipLevel(CItemPtr pEquipItem);
    bool UpEquipLevel(CUserPtr pUser);
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned char ucAction;
        unsigned char ucAmount;
        union
        {
            unsigned char ucChar[1];			// 变长
            char cChar[1];
            unsigned short usShort[1];
            short sShort[1];
            unsigned long dwLong[1];
            long nLong[1];
        };
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif
