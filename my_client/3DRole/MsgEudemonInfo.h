
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgEudemonInfo.h: interface for the CMsgEudemonInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGEUDEMONINFO_H__2A75E484_1289_47CF_8E73_CCAA01C9A02B__INCLUDED_)
#define AFX_MSGEUDEMONINFO_H__2A75E484_1289_47CF_8E73_CCAA01C9A02B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMsgEudemonInfo  : public CNetMsg
{
public:
    CMsgEudemonInfo();
    virtual ~CMsgEudemonInfo();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;
        OBJID   id;
        DWORD   dwLookFace;

        //I64 i64Status;
        DWORD	dwStatus[2];
        OBJID	idOwner;			// 幻兽有效

        USHORT	usMaxLife;			// 当Player为Monster的时候，表示最大生命
        USHORT	usCurLife;			// 当Player为Monster的时候，表示当前生命


        USHORT	usPosX, usPosY;
        UCHAR	ucDir;
        UCHAR	ucPose;
        OBJID	idMonsterType;			// 怪物和幻兽有效

        // for str packer
        char szBuf[1];
    } MSG_Info;
    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGEUDEMONINFO_H__2A75E484_1289_47CF_8E73_CCAA01C9A02B__INCLUDED_)
