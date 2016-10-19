
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////
// cell.h
///////////////////////////////////////////

#ifndef _CELL_H
#define _CELL_H

//-------------------------------------------------------------
// header files ...
#include "BaseFunc.h"
#include "Layer.h"
#include "MyHeap.h"
//-------------------------------------------------------------

const int	LAYER_TOP			= -1;				// 缺省删除顶层LAYER的标志

////////////////////////////////////////////////////////////////////////
const int	MASK_SEARCHED		= 0x80;				// searched flag
const int	MASK_ROLECOUNT		= 0x7F;				// count of role
const int	MASK_MASK			= 0x0001;			// layer mask
const int	MASK_ALTITUDE		= 0xFFFE;			// layer alt
const int	MAX_IDXLAYER		= 0xFFFF;			// 附加层CELL数量
const int	IDXLAYER_NONE		= USHORT(-1);
const int	DEFAULT_FLOORATTR	= 0;				// 缺省的地表属性
const int	ALTITUDE_LIMIT		= 32767;			// 极限高度(用于非法高度)

#pragma pack(push)
#pragma pack(1)
class CCell
{
private:
    //	DEQUE_LAYER m_setLayer;
    //	bool        m_bSearched;
    USHORT		m_idxLayer;							// <= 65534, -1: invalid
    SHORT		m_nAlt2Mask;						// alt*2 and m_bMasked
    UCHAR		m_nCountFlag;					// rule count and m_bSearched
public:
    CCell(CLayer* pLayer);
    ~CCell();
    static CCell*	CreateNew(CLayer* pLayer);
    bool	Create(int nAlt, DWORD dwMask);

    void    Destory();

public: //
    ULONG	Release()							{ delete this; return 0; }
public:
    int		GetFloorAttr(DEQUE2_LAYER& set2Layer);
    DWORD	GetFloorMask(DEQUE2_LAYER& set2Layer);
    int		GetFloorAlt(DEQUE2_LAYER& set2Layer);
    int		GetSurfaceAlt();
    void	FullMask()							{ m_nAlt2Mask |= MASK_MASK; }
    //...
    void	IncRole(int nVal = 1)				{ if ((m_nCountFlag & MASK_ROLECOUNT) + nVal <= MASK_ROLECOUNT) { m_nCountFlag = (m_nCountFlag & MASK_ROLECOUNT) + nVal; } }
    void	DecRole(int nVal = 1)				{ if ((m_nCountFlag & MASK_ROLECOUNT) >= nVal) { m_nCountFlag = (m_nCountFlag & MASK_ROLECOUNT) - nVal; } }
    int		GetRoleAmount()						{ return (m_nCountFlag & MASK_ROLECOUNT); }

public:
    int             GetLayerAmount(DEQUE2_LAYER& set2Layer);                                    // 得到层次数量
    CLayer*         Getlayer(DEQUE2_LAYER& set2Layer, int nIndex);   	//★ 不能取第0层                             // 取得层次

    void            AddLayer(DEQUE2_LAYER& set2Layer, CLayer* pLayer);                            // 添加层次
    bool            DelLayer(DEQUE2_LAYER& set2Layer, int nIndex = LAYER_TOP);     //★ 不能删除第0层                         // 删除层次

    //	BOOL            AddLogicLayer(CLayer* pLayer, CLayer* pBaseLayer);   // 添加逻辑层次
    //	BOOL            DelLogicLayer(int nIndex);                           // 删除逻辑层次

    BOOL            BeSearched() {return (m_nCountFlag & MASK_SEARCHED) != 0;}
    void            SetSearchFlag(bool bSearched) {if (bSearched) { m_nCountFlag |= MASK_SEARCHED; } else { m_nCountFlag &= (~MASK_SEARCHED); }}

protected:
    MYHEAP_DECLARATION(s_heap)
};
#pragma pack(pop)






//-------------------------------------------------------------



#endif
