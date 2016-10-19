
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************


#ifndef FISH_HEADER_KATTY
#define FISH_HEADER_KATTY

#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include "MapObj.h"
#include "airbubble.h"
#include <deque>
using namespace std;


enum {FISH_NONE, FISH_CREATE, FISH_DESTROY};

const int MAX_ANIMATIONS	= 8;
const int FISH_NUM			= 8;
const int FISH_FRAME		= 10;


typedef struct
{
    int x, y;
    int xv, yv;
    int width[MAX_ANIMATIONS], height[MAX_ANIMATIONS];
    int curr_frame;     // current animation frame
    int num_frames;     // total number of animation frames
    int curr_animation;
    int anim_index;
} INFO, *PINFO;

class CFish: public CMapObj
{
public:
    CFish() { m_iStatus = FISH_NONE;}
    ~CFish() {Destroy();}
    void Reset();
    void Destroy();
    int	 GetObjType   () {return MAP_COVER;}
    void Show(void* pInfo);
    BOOL Create(int IsDolphin);

    void GetPos(CMyPos& posCell) ;
    void GetWorldPos(int& iWorldX, int& iWorldY) {iWorldX = m_info.x; iWorldY = m_info.y;}
    void GetBase(CMySize& infoSize)   {infoSize.iWidth = 2; infoSize.iHeight = 2;}
    void Process	(void* pInfo);
    BOOL SetPicture( int nType);
    static void AddFish(int nNum);

private:
    void	Fish_AI(void);

    void	Move_Fish();
    int* 	m_pCurPattern;
    int		m_iIp;
    int		m_iCount;
    int		m_iPatternIndex;
    INFO	m_info;
    void	Set_Animation(int iopcode);
    int		m_nOPcode;
    int		m_nOPrand;
    int		m_iStatus;
    int		m_iPatterIndex;
    int		m_iScreenX;
    int		m_iScreenY;
    int		m_bIsDolphin;
    char	m_szAniTitle[MAX_ANIMATIONS][100];
    BOOL	m_bOnTop;

};


#endif
