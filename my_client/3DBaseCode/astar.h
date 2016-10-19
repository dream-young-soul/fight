
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef PROGRAM_ASTAR
#define PROGRAM_ASTAR


#include "template.h"


//-----------------------------------------------------------------------------

const int MAXSEARCHSTEP		= 400;				//最长寻路步数

enum BODYSIZE
{
    BODILY0					= 0,					//不占格子
    BODILY1					= 1,					//占1个格子
    BODILY4					= 4,					//占4个格子
};

class STEPDATA 										//行路数据
{

private:
    int					nStepCount;							//记录步数
    char				cStepWay[ MAXSEARCHSTEP * 2 + 3 ];	//方向数组
    BODYSIZE			eBodySize;							//体型

public:
    STEPDATA( void ) : nStepCount( 0 ) {};
    STEPDATA( BODYSIZE bs ) : nStepCount( 0 ), eBodySize( bs ) {};

    int RemainSteps( void )							//剩余步数
    { return nStepCount; }
    void ClearStep( void )							//清除
    { nStepCount = 0; }
    void AddStep( int nC )							//附加步数
    { nStepCount += nC; };
    void SetStep( int nC )							//设置步数
    { nStepCount = nC; };
    char GetNextStep( void )						//取下一步
    { return cStepWay[ --nStepCount ]; }
    char* GetStepArray( void )						//得到步缓冲区
    { return cStepWay; }
    char* GetStepArrayTail( void )					//得到步缓冲区末尾地址
    { return cStepWay + nStepCount; }
    void SetBodySize( BODYSIZE eSize )				//设置体型尺寸
    { eBodySize = eSize; }
    BODYSIZE GetBodySize( void )					//得到体型尺寸
    { return eBodySize; }
};

//-----------------------------------------------------------------------------

#define ASTAR_UNSTAY		( ASTAR_NODE * )0xffffffffL	//不可停留
#define ASTAR_INSTAY		( ASTAR_NODE * )NULL		//可停留

const int ASTACKBUFFSIZE	= MAXSEARCHSTEP * 8 * 8 + 10;	//A*栈数量
const int ASTARBUFFSIZE		= MAXSEARCHSTEP * 8 + 10;		//A*结点缓冲区数量
const unsigned int ASTAR_HASMAN		= 0x00000001L;			//人停留标记

const char INSTAY			= 0;				//可停留
const char UNSTAY			= 1;				//不可停留

const char INOPEN			= 1;					//在OPEN链表中
const char INCLOSE			= 2;					//在CLOSE链表中
const char OTHERWISE		= 0;					//不在链表中

struct ASTAR_NODE
{
    int				nG, nH, nF;					//估价值
    int				nX, nY;						//坐标
    int				nNumber;					//数组编号
    ASTAR_NODE*		 pParent;					//父结点指针
    ASTAR_NODE*		 pChild[8];				//子结点指针
    ASTAR_NODE*		 pNextNode;				//链表指针
    char			cWhere, cDirect;			//位置、方向
};

class ASTAR_8WAY
{

private:
    class ASTAR_IDX
    {
    private:
        ASTAR_NODE*			 pAN;					//结点指针
        unsigned int		unCount;				//计数器、标记
    public:

        void SetUnstay( void )						//设置不可走
        { pAN = ASTAR_UNSTAY; };
        void SetStay( void )						//设置可走
        { pAN = ASTAR_INSTAY; };
        void SetStay( char cS )						//设置可走
        { pAN = ( cS == UNSTAY ) ? ASTAR_UNSTAY : ASTAR_INSTAY; };
        void SetHasman( void )						//设置有人
        { unCount |= ASTAR_HASMAN; };
        void SetNoman( void )						//设置没人
        { unCount = unCount & ( ~ASTAR_HASMAN ); };
        void SetCount( unsigned int unC )			//设置计数器
        { unCount = ( unCount & 0xf ) | unC; };
        bool IsAvail( unsigned int unC )			//计数器有效？
        { return ( unCount >= unC ); };
        bool IsUnstay( void )						//测试不可停留，不考虑人？
        { return ( pAN == ASTAR_UNSTAY ); };
        bool IsUnstayCM( void )						//测试不可停留，考虑人？
        { return ( ( pAN == ASTAR_UNSTAY ) || ( unCount & ASTAR_HASMAN ) ); };
        void SetNode( ASTAR_NODE* pN )				//设置结点指针
        { pAN = pN; };
        ASTAR_NODE* GetNode( void )				//得到结点指针
        { return pAN; };
        char GetStay( void )						//得到停留标记
        { return ( pAN == ASTAR_UNSTAY ); };
    };

    class CHAINASN_C 								//CLOSE链表
    {
    protected:
        ASTAR_NODE*			 pNode;

    public:
        CHAINASN_C( void ) : pNode( NULL ) {};

        bool IsEmpty( void )						//检查为空
        { return ( pNode == NULL ); };
        ASTAR_NODE* Get( void )					//取出
        {
            if ( pNode == NULL ) { return NULL; }
            ASTAR_NODE* pTemp = pNode;
            pNode = pNode->pNextNode;
            return pTemp;
        }
        void Insert( ASTAR_NODE* pN )				//插入
        { pN->pNextNode = pNode; pNode = pN; pNode->cWhere = INCLOSE; };
        void Clear( void )							//清除
        { pNode = NULL; };
    };

    class CHAINASN_O : public CHAINASN_C 			//OPEN链表
    {
    public:
        void Insert( ASTAR_NODE* );				//插入
    };


    int						nMapWidth;				//地图宽
    int						nMapHeight;				//地图高
    ASTAR_IDX* 				pAStarIdx;				//地图大小行走标记
    unsigned int			unPathCount;			//寻路计数器

    int						MAN_BODILY_OFFSET[ BODILY4 ];		//
    int						MAN_BODILY_AROUND[ 8 ];				//周围点相对中心点偏移
    int						LINEOFF[ 8 ];						//

    STACK< ASTAR_NODE*, ASTACKBUFFSIZE >		AstarStack;		//A*栈
    FILOMEMORY< ASTAR_NODE, ASTARBUFFSIZE >		AstarNode;		//A*结点堆

    CHAINASN_O				OpenChain;			//OPEN链表
    CHAINASN_C				CloseChain;			//CLOSE链表

    int					nEndX, nEndY;			//终点坐标
    ASTAR_IDX*			 pEndASI;				//终点地址
    STEPDATA*			 pStepPtr;				//行走物描述指针
    BODYSIZE			BSize;					//行走物体型

protected:
    ASTAR_NODE* ReturnBestNode( void );				//得到下一个结点
    int GenerateSuccessorsNoMan( ASTAR_NODE* );		//人不算障碍
    int GenerateSuccessorsHasMan( ASTAR_NODE* );		//人算障碍
    void FreeNodes( void );									//清寻路节点
    void PropagateDown( ASTAR_NODE* );

    inline void ClearAstarIdx( void );
    int CheckContinue( int, int, int&, int& );		//
    int CheckFront( int, int, int, int, int );			//
    char CalculateWay( ASTAR_NODE*, ASTAR_NODE* );
    char CalculateMirrWay( ASTAR_NODE*, ASTAR_NODE* );
    int Calculate( int nSX, int nSY, int nTX, int nTY )		//估计函数
    { return ( ( nTX - nSX ) * ( nTX - nSX ) + ( nTY - nSY ) * ( nTY - nSY ) ) << 2; }

    bool TestStayHasMan( ASTAR_IDX*, BODYSIZE );			//测试停留，考虑人
    bool TestStayNoMan( ASTAR_IDX*, BODYSIZE );			//测试停留，不考虑人

public:
    ASTAR_8WAY( void );
    ~ASTAR_8WAY( void ) { Release(); }

    int InitAStar( int, int );								//初始化地图尺寸
    void SetSafelyZone( void );								//设置安全带
    void Release( void );									//释放

    void SetAStarAtr( int nX, int nY, char cArt )			//设置行走属性
    { SetAStarAtr( nMapWidth * nY + nX, cArt ); }
    void SetAStarAtr( int nCount, char cArt )				//设置行走属性
    { pAStarIdx[ nCount ].SetStay( cArt ); };
    char GetAStarAtr( int nX, int nY )						//取停留标记
    { return GetAStarAtr( nMapWidth * nY + nX ); }
    char GetAStarAtr( int nC )								//取停留标记
    { return pAStarIdx[ nC ].GetStay(); };
    void SetManInAStarAtr( int nX, int nY, BODYSIZE cType )	//设置人占用
    { SetManInAStarAtr( nMapWidth * nY + nX, cType ); }
    void SetManInAStarAtr( int, BODYSIZE );					//设置人占用
    void ClrManInAStarAtr( int nX, int nY, BODYSIZE cType )	//清除人占用
    { ClrManInAStarAtr( nMapWidth * nY + nX, cType ); }
    void ClrManInAStarAtr( int, BODYSIZE );					//清除人占用

    int FindPath( int, int, int, int, STEPDATA*, int );	//寻路
};


//-------------------------参数------------------------------------------------

const int NO_MAN			= 0;				//不考虑人
const int HAS_MAN			= 1;				//考虑人

const int E_TO_S			= 0;				//反向搜索
const int S_TO_E			= 0x10;				//正向搜索

const int NOCHECKFRONT		= 0x1000;			//不检查起点
const int CONTINUESEARCH	= 0x2000;			//当前局部路径搜索

//-----------------------返回值------------------------------------------------

const int NOFOUND			= 1;				//没找到目标
const int NOPATH			= 2;				//没路
const int HAVEPATH			= 4;				//找到
const int ATTARGET			= 8;				//站在目标上

const int TARGETHASMAN		= 0x10;				//目标点有其他人
const int TARGETNOPATH		= 0x20;				//目标点不能停留(如被其它建筑覆盖)
const int TARGETCLOSE		= 0x40;				//目标封闭
const int SOURCECLOSE		= 0x80;				//源封闭
const int UNWALKABLE		= TARGETHASMAN | TARGETNOPATH | TARGETCLOSE | SOURCECLOSE;

const int NOENOUGHSTEP		= 0x200;			//剩余步长不够，不能局部搜索

const int RECORDPATH		= 0x1000;			//记录路径
//        CONTINUESEARCH	0x2000				//当前局部路径搜索
const int SHARELAST			= 0x4000;			//使用以前路径
const int MAXSTEP			= 0x8000;			//最大搜索

#endif
