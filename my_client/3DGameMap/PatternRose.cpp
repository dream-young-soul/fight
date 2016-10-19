
//**********************************************************
// 代码编辑器
//**********************************************************

#include "basefunc.h"
#include "3dgamemap.h"
#include "PatternRose.h"
#include <math.h>
#include "nd_bitmap.h"
#include "skylayer.h"
#include "skyrose.h"
#include "gamedataset.h"


#define HZK24c "hzk24k"
#define HZK24t "hzk24t"


/*
            aaaaaaaaa   -6
	  aaaa     	 aaaa
     aaaaa              aa
   aaa
  aaaa
   aaaa
 aaaa                  0
   aaaa                1
      aaaa             2
	      aaaa         3
		      aaaa
			      aaaa
				      a
*/
int ArrayRose1[] = { -5, -6,  -6, -6,  -7, -6,  -8, -6 , -9, -6 , -10, -6,  -11, -6,  -12, -6,  -13, -6,    5, -6,  6, -6,  7, -6,  8, -6 , 9, -6 , 10, -6,  11, -6,  12, -6,  13, -6,
                     -2, -5, -3, -5,  -4, -5, -5, -5,                 -13, -5, -14, -5, -15, -5, -16, -5,                         2, -5, 3, -5,  4, -5, 5, -5,                 13, -5, 14, -5, 15, -5, 16, -5,
                     0, -4  , -1, -4,                                     -16, -4,  -17, -4,  -18, -4,  -19, -4 ,  -20, -4,        0, -4  , 1, -4,                                     16, -4,  17, -4,  18, -4,  19, -4 ,  20, -4,
                     -20, -3, -21, -3, -22, -3,                             20, -3, 21, -3, 22, -3,
                     -20, -2, -21, -2, -22, -2, -23, -2,                        20, -2, 21, -2, 22, -2, 23, -2,
                     -19, -1, -20, -1, -21, -1, -22, -1,                        19, -1, 20, -1, 21, -1, 22, -1,
                     -18, 0,  -19, 0,  -20, 0,  -21, 0,                             18, 0,  19, 0,  20, 0,  21, 0,
                     -16, 1,  -17, 1,  -18, 1,  -19, 1,                              16, 1,  17, 1,  18, 1,  19, 1,
                     -13, 2,  -14, 2, -15, 2,  -16, 2,                                  13, 2,  14, 2, 15, 2,  16, 2,
                     -9, 3,  -10, 3,  -11, 3,  -12, 3,                                        9, 3,  10, 3,  11, 3,  12, 3,
                     -5, 4,  -6, 4,  -7, 4,   -8, 4,                                           5, 4,  6, 4,  7, 4,  8, 4,
                     -1, 5,  -2, 5,  -3, 5,   -4, 5,                                            1, 5,  2, 5,  3, 5,  4, 5,
                     0, 6,   0, 0
                   };// null heart




/*

             aaaaaaaaa   -6
		  aaaa     	 aaaa
      aaaaa     aa       aa
    aaa    aa       aa
   aaaa        aa        aa
    aaaa              aa
	 aaaa  aa  aa   aa  aa 0
	   aaaa       aa        1
	      aaaa    aa   aaa 2
		      aaaa   aa    3
			      aaaa  aa
				      aaaa
					      a*/


int ArrayRose2[] = { -5, -6,  -6, -6,  -7, -6,  -8, -6 , -9, -6 , -10, -6,  -11, -6,  -12, -6,  -13, -6,                    5, -6,  6, -6,  7, -6,  8, -6 , 9, -6 , 10, -6,  11, -6,  12, -6,  13, -6,
                     -2, -5, -3, -5,  -4, -5, -5, -5,                 -13, -5, -14, -5, -15, -5, -16, -5,                                         2, -5, 3, -5,  4, -5, 5, -5,                 13, -5, 14, -5, 15, -5, 16, -5,
                     0, -4, -1, -4,             -10, -4, -9, -4 ,                       -16, -4,  -17, -4,  -18, -4,  -19, -4 ,  -20, -4,              0, -4  , 1, -4,             10, -4, 9, -4 ,         16, -4,  17, -4,  18, -4,  19, -4 ,  20, -4,
                     -20, -3, -21, -3, -22, -3,     -15, -3, -12, -3,   -6, -3, -5, -3 ,                                                                 20, -3, 21, -3, 22, -3,     15, -3, 12, -3,   6, -3, 5, -3 ,
                     -20, -2, -21, -2, -22, -2, -23, -2,   -9, -2, -8, -2,   -1, -2, 0, -2,                                                               20, -2, 21, -2, 22, -2, 23, -2,   9, -2, 8, -2,   1, -2, 0, -2,
                     -19, -1, -20, -1, -21, -1, -22, -1,   -4, -1, -3, -1,                                                                              19, -1, 20, -1, 21, -1, 22, -1,   4, -1, 3, -1,
                     -18, 0,  -19, 0,  -20, 0,  -21, 0,    -15, 0,  -14, 0,  -9, 0, -8, 0,  -6, 0, -5, 0, -2, 0, -1, 0,                                          18, 0,  19, 0,  20, 0,  21, 0,    15, 0,  14, 0,  9, 0, 8, 0,  6, 0, 5, 0, 2, 0, 1, 0,
                     -16, 1,  -17, 1,  -18, 1,  -19, 1,      - 10, 1, -9, 1,                                                                                10, 1, 9, 1,            16, 1,  17, 1, 18, 1,  19, 1,
                     -13, 2,  -14, 2, -15, 2,  -16, 2,      -11, 2, -10, 2,   -6, 2, -5, 2 , -4, 2,                                                             13, 2,  14, 2, 15, 2,  16, 2,      11, 2, 10, 2,   6, 2, 5, 2 , 4, 2,
                     -9, 3,  -10, 3,  -11, 3,  -12, 3,      -8, 3, -7, 3,                                                                                     9, 3,  10, 3,  11, 3,  12, 3,     8, 3, 7, 3,
                     -5, 4,  -6, 4,  -7, 4,   -8, 4,        -5, 4, -4, 4,                                                5, 4,  6, 4,  7, 4,   8, 4,        5, 4, 4, 4,
                     -1, 5,  -2, 5,  -3, 5,   -4, 5,    0, 5,                                                         1, 5,  2, 5,  3, 5,   4, 5,   0, 5,
                     0, 6,   0, 0
                   };


/*
   aaa
 aaaaaaa
aaaaaaa   0
  aaaaaa
     aaa
	   a


*/
int ArrayRose3[] = { -2, -2,   -3, -2,   -4, -2,     2, -2, 3, -2, 4, -2,
                     0, -1,  -1, -1,  -2, -1,  -3, -1,  -4, -1,  -5, -1,  -6, -1,     1, -1, 2, -1, 3, -1 , 4, -1, 5, -1, 6, -1,
                     -1, 0,  -2, 0,   -3, 0,   -4, 0,  -5, 0,  -6, 0,  -7, 0,       1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0,
                     0, 1,   -1, 1,   -2, 1 ,   -3, 1,   -4, 1,   -5, 1,              1, 1, 2, 1, 3, 1, 4, 1, 5, 1,
                     0, 2,   -1, 2,    -2, 2,                              1, 2, 2, 2,
                     0, 3, 0, 0
                   };// small heart
/*
 a
 aa         aaa
 aaa     aaaaaaaa
 aaaa aaaaaaaaaaaa
 aaaaaaaaaaaaaaa0 aa
 aaaa aaaaaaaaaaaaa
 aaa     aaaaaaaa
 aa         aaa
 a
 */

int ArrayRose4[] = {1, -4,
                    1, -3,  2, -3,                           12, -3,  13, -3,  14, -3,
                    1, -2,  2, -2,  3, -2,                   9, -2,  10, -2,  11, -2,  12, -2,   13, -2,  14, -2,  15, -2,  16, -2,
                    1, -1,  2, -1,  3, -1,  4, -1,            6, -1, 7, -1, 8, -1,  9, -1,  10, -1,  11, -1,  12, -1,   13, -1,  14, -1,  15, -1,  16, -1, 17, -1,
                    1, 0,	2, 0,	3, 0,	4, 0,	5, 0,	6, 0,	7, 0,	8, 0,	9, 0,	10, 0,	11, 0,	12, 0,	13, 0,	14, 0, 15, 0,           18, 0, 19, 0,
                    1, 4,
                    1, 3,  2, 3,                           12, 3,  13, 3,  14, 3,
                    1, 2,  2, 2,  3, 2,                   9, 2,  10, 2,  11, 2,  12, 2,   13, 2,  14, 2,  15, 2,  16, 2,
                    1, 1,  2, 1,  3, 1,  4, 1,            6, 1, 7, 1, 8, 1,  9, 1,  10, 1,  11, 1,  12, 1,   13, 1,  14, 1,  15, 1,  16, 1, 17, 1,
                    0, 0
                   };//fish


/*
 a
 aa         aaa
 a a     aaa   aa
 a  a aaa        a
 a  aaa        a   a
 a  a aaa        a
 a a     aaa   aa
 aa         aaa
 a
 */



int ArrayRose5[] = {1, -4,
                    1, -3,  2, -3,                           12, -3,  13, -3,  14, -3,
                    1, -2,    3, -2,                   9, -2,  10, -2,  11, -2,         15, -2,  16, -2,
                    1, -1,     4, -1,            6, -1, 7, -1, 8, -1,      17, -1,
                    1, 0, 	4, 0,	5, 0,	6, 0, 	 15, 0,           19, 0,
                    1, 4,
                    1, 3,  2, 3,                           12, 3,  13, 3,  14, 3,
                    1, 2,    3, 2,                   9, 2,  10, 2,  11, 2,         15, 2,  16, 2,
                    1, 1,     4, 1,            6, 1, 7, 1, 8, 1,      17, 1,
                    0, 0
                   };//fish

BOOL CPatternRose::Create(int iWorldX, int iWorldY)
{
    m_iStatus				= PATTERNROSE_CREATE;
    m_nShowWorldPoint.x		= iWorldX;
    m_nShowWorldPoint.y		= iWorldY;
    m_nOrigWorldPoint.x		= iWorldX;
    m_nOrigWorldPoint.y		= iWorldY;
    m_nRiseCount			= 0;
    m_nLoopFrame			= 0;
    m_nCurX					= rand() % 10;
    m_iYMoved				= 150 + rand() % 350;
    m_bAniIndex				= 0;
    m_iAlpha				= 255;
    m_dwFrameTime			= 0;
    m_nLastFrame			= -1;
    return 1;
}


void CPatternRose::Destroy()
{
    m_iStatus = PATTERNROSE_NONE;
}

void  CPatternRose::Show()
{
    if (m_bAniIndex > 8)
    {
        return;
    }
    int nScreenX, nScreenY;
    g_objGameMap.World2Screen( m_nShowWorldPoint.x , m_nShowWorldPoint.y, nScreenX, nScreenY);
    int iAniIndex = m_bAniIndex;
    if (iAniIndex == 0)
        if (m_nCurX == 0)
        {
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[2]);
            if (pAni)
            {
                pAni->Show(m_nLoopFrame, nScreenX + 20, nScreenY + 20, m_iAlpha);
            }
        }
        else if (m_nCurX == 1)
        {
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[3]);
            if (pAni)
            {
                pAni->Show(m_nLoopFrame, nScreenX + 10, nScreenY + 10, m_iAlpha);
            }
        }
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[iAniIndex]);
    if (pAni)
        if (pAni->Show(m_nLoopFrame, nScreenX, nScreenY, m_iAlpha))
        {
            if (iAniIndex == 1)
            {
                m_nLoopFrame = 0;
            }
        }
        else
        {
            DWORD  dwNow = TimeGet();
            DWORD nInterval = 150;
            if ( PATTERNROSE_RISE == m_iStatus)
            {
                nInterval = 250;
            }
            if ((dwNow - m_dwFrameTime) > nInterval )
            {
                m_dwFrameTime = dwNow;
                m_nLoopFrame++;
            }
        }
}


void CPatternRose::Process()
{
    if (m_iStatus == PATTERNROSE_NONE)
    {
        return;
    }
    if (m_iStatus == PATTERNROSE_CREATE)
    {
        if (m_iYMoved -- < 0)
        {
            m_iStatus = PATTERNROSE_RISE;
            m_iCircleDir = rand() % 8 - 4 ;
            m_nRiseCount = 0;
            m_nRiseStep =  rand() % 8 + 3;
            m_iYMoved = 0;
            if (rand() % 2)
            {
                CSkyRose* rose = new CSkyRose;
                rose->Create(this->m_szAniTitle, m_nShowWorldPoint.x , m_nShowWorldPoint.y, m_iCircleDir, m_nRiseStep);
                CSkyLayer* p = g_objGameMap.GetSkyLayer();
                p->AddObj(rose);
            }
        }
        m_iAlpha = 255  ;
        m_bAniIndex = 0 ;
        Show();
    }
    if (m_iStatus == PATTERNROSE_RISE)
    {
        m_iYMoved += m_nRiseStep;
        m_nShowWorldPoint.y = m_nOrigWorldPoint.y - m_iYMoved;
        m_nCurX	+= m_iCircleDir;
        m_nShowWorldPoint.x = m_nOrigWorldPoint.x + m_nCurX;
        if (m_iYMoved > 600)
        {
            m_iStatus    = PATTERNROSE_FALL;
            m_nRiseCount = rand() % 250;
            m_nCurX	 	 = 0;
            m_nOrigWorldPoint.x = m_nShowWorldPoint.x ;
            m_nOrigWorldPoint.y = m_nShowWorldPoint.y ;
        }
        m_iAlpha = 255;
        m_bAniIndex = 1;
        //	Show();
    }
    if (m_iStatus ==  PATTERNROSE_FALL)
    {
        if (m_nCurX ++ > m_nRiseCount)
        {
            m_iStatus = ROSE_DESTROY;
            m_bAniIndex = 10;
        }
        else
        {
            m_nShowWorldPoint.y = m_nOrigWorldPoint.y + m_nCurX * 2;
            m_iAlpha = 255 - m_nCurX  ;
            m_bAniIndex = 1;
            //	Show( );
        }
    }
}

//============================================================================================
//
//   PatternRoseSys
//
//=============================================================================================



BOOL  PatternRose::GetPoint(int* nArray, char* zw, int X, int Y)
{
    DWORD n = 0;
    int nInterval = 8;
    int iArrayIndex = 0;
    while(n != strlen(zw))
    {
        int i = 0, j = 0, l = 0;
        int xx = 0;
        int yy = 0;
        FILE* fp;
        int  num = 0;
        if ((zw[n] & 0x80) == 0)
        {
            char* wm;
            wm = (char*)malloc(72);
            if (zw[n] == ' ')
                {X = X + 24 * nInterval / 2 + 10 ; yy = 0; n++;}
            else
            {
                X += -15;
                fp = fopen(HZK24t, "rb++");
                if (fp == NULL)
                {
                    return FALSE;
                }
                num = 155 + zw[n];
                fseek(fp, (72)*num, 0);
                fread(wm, 72, 1, fp);
                fclose(fp);
                for(i = 0; i < 24; i++)
                {
                    for(j = 0; j < 3; j++)
                    {
                        for(l = 1; l <= 8; l++)
                            if (test(wm[i*3+j], l) == 1)
                            {
                                *(nArray + iArrayIndex * 2) = X + xx;
                                *(nArray + iArrayIndex * 2 + 1) = Y + yy;
                                if (++iArrayIndex >= 10000)
                                {
                                    return 0;
                                }
                                yy += nInterval;
                            }
                            else
                            {
                                yy += nInterval;
                            }
                    }
                    X += nInterval ;
                    yy = 0;
                }
                n += 1;
                X += -15;
            }
        }
        else
        {
            BYTE* wm;
            wm = (BYTE*)malloc(72);
            fp = fopen(HZK24c, "rb++");
            if (fp == NULL)
            {
                return FALSE;
            }
            BYTE bChar = zw[n];
            bChar = bChar - 175;
            BYTE bChar1 = zw[n+1];
            bChar1 = bChar1 - 160;
            num = (bChar - 1) * 94  + bChar1 - 1;
            fseek(fp, (72)*num, 0);
            fread(wm, 72, 1, fp);
            fclose(fp);
            for(i = 0; i < 24; i++)
            {
                for(j = 0; j < 3; j++)
                {
                    for(l = 1; l <= 8; l++)
                        if (test(wm[i*3+j], l) == 1)
                        {
                            *(nArray + iArrayIndex * 2) = X + xx;
                            *(nArray + iArrayIndex * 2 + 1) = Y + yy;
                            if (++iArrayIndex >= 10000)
                            {
                                return 0;
                            }
                            yy += nInterval;
                        }
                        else
                        {
                            yy += nInterval;
                        }
                }
                X += nInterval;
                yy = 0;
            }
            n += 2;
            X += 10;
            yy = 0;
        }
    }
    *(nArray + iArrayIndex * 2) = 0;
    *(nArray + iArrayIndex * 2 + 1) = 0;
    return TRUE;
}

BOOL PatternRose::Create(int iWorldX, int iWorldY, int nColor, char* szChar)
{
    Destroy();
    int i;
    if (nColor <= 3)
    {
        for(i = 0; i < MAX_PatternRoseANI; i++)
        {
            char szTitle[100];
            if (i < 2)
            {
                sprintf(szTitle, "rose%d_ani%d" , nColor, i);
            }
            if (i == 2)
            {
                strcpy(szTitle, "paint");
            }
            if (i == 3)
            {
                strcpy(szTitle, "grass");
            }
            strcpy(m_szFileName[i], szTitle);
        }
        strcpy(m_szFileName[MAX_PatternRoseANI] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 1] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 2] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 3] , "");
    }
    else
    {
        int icolor = 1;
        if (nColor > 4)
        {
            icolor = 4;
        }
        for(i = 0; i < 3; i++)
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 0);
        }
        icolor = 1;
        if (nColor > 4)
        {
            icolor = 4;
        }
        for(i = 3; i < 6 ; i++) //载入飘的动画
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 1);
        }
        for(i = 0; i < 2; i++)
        {
            char szTitle[100];
            if (i == 0)
            {
                strcpy(szTitle, "paint");
            }
            else
            {
                strcpy(szTitle, "grass");
            }
            if (nColor > 4)
            {
                strcat(szTitle, "1");
            }
            strcpy(m_szFileName[6+i], szTitle);
        }
    }
    m_nStatus = PATTERNROSE_CREATE;
    int* ArrayRose = (int*)malloc(sizeof(int) * 10000);
    GetPoint(ArrayRose, szChar, iWorldX,  iWorldY);
    for(i = 1; i <= 10000; i++)
    {
        if (ArrayRose[i*2] == 0 && ArrayRose[i*2+1] == 0)// the end of array
        {
            break;
        }
        int nOrigWorldPointX =  ArrayRose[i* 2]  + rand() % 6;
        int nOrigWorldPointY =  ArrayRose[i * 2 + 1]  + rand() % 6;
        CPatternRose*   pObjPatternRose	= new CPatternRose;
        if (nColor <= 3)
        {
            pObjPatternRose->SetPicture(m_szFileName, 0, 1, 2, 3, nColor);
        }
        else
        {
            int nflower = rand() % 3;
            pObjPatternRose->SetPicture(m_szFileName, nflower, nflower + 3, 6, 7, nflower);
        }
        pObjPatternRose->Create (nOrigWorldPointX , nOrigWorldPointY);
        m_dequePatternRose.push_back(pObjPatternRose);
    }
    free(ArrayRose);
    return 1;
}

int* PatternRose::GetBmpPoint(int index, char* szpFileName)
{
    FILE* P = NULL;
    P = fopen(szpFileName, "rb");
    if (!P)
    {
        return 0;
    }
    fseek(P, index * 700 * 2 * sizeof(int), SEEK_SET);
    memset(&m_nPoint[0], 0, 700 * 2 * 4);
    if (!feof(P))
    {
        fread(&m_nPoint[0], 4, 700 * 2, P);
    }
    else
    {
        fclose(P);
        return 0;
    }
    fclose(P);
    return &m_nPoint[0];
}

BOOL PatternRose::Create(int iWorldX, int iWorldY, int nType, int nColor)
{
    Destroy();
    int i;
    if (nColor <= 3)
    {
        for(i = 0; i < MAX_PatternRoseANI; i++)
        {
            char szTitle[100];
            if (i < 2)
            {
                sprintf(szTitle, "rose%d_ani%d" , nColor, i);
            }
            if (i == 2)
            {
                strcpy(szTitle, "paint");
            }
            if (i == 3)
            {
                strcpy(szTitle, "grass");
            }
            strcpy(m_szFileName[i], szTitle);
        }
        strcpy(m_szFileName[MAX_PatternRoseANI] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 1] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 2] , "");
        strcpy(m_szFileName[MAX_PatternRoseANI + 3] , "");
    }
    else
    {
        int icolor = 1;
        if (nColor > 4)
        {
            icolor = 4;
        }
        for(i = 0; i < 3; i++)
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 0);
        }
        icolor = 1;
        if (nColor > 4)
        {
            icolor = 4;
        }
        for(i = 3; i < 6 ; i++) //载入飘的动画
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 1);
        }
        for(i = 0; i < 2; i++)
        {
            char szTitle[100];
            if (i == 0)
            {
                strcpy(szTitle, "paint");
            }
            else
            {
                strcpy(szTitle, "grass");
            }
            if (nColor > 4)
            {
                strcat(szTitle, "1");
            }
            strcpy(m_szFileName[6+i], szTitle);
        }
    }
    m_nStatus = PATTERNROSE_CREATE;
    int* ArrayRose;
    switch(nType)
    {
    case 0:
    case 1:
        ArrayRose = &ArrayRose1[0];
        break;
    case 2:
        ArrayRose = &ArrayRose2[0];
        break;
    case 3:
        ArrayRose = &ArrayRose3[0];
        break;
    case 4:
        ArrayRose = &ArrayRose4[0];
        break;
    case 5:
        ArrayRose = &ArrayRose5[0];
        break;
    default:
        ArrayRose = GetBmpPoint(nType - 6, "./data/weather/Rose/patternRose.dat");
        if (!ArrayRose)
        {
            ArrayRose = &ArrayRose1[0];
        }
        break;
    }
    for(i = 0; i <= 700; i++)
    {
        if (ArrayRose[i*2] == 0 && ArrayRose[i*2+1] == 0)// the end of array
        {
            break;
        }
        int nOffsetX, nOffsetY;
        if (nType <= 5)
        {
            nOffsetX = rand() % 6;
            nOffsetY = rand() % 6;
        }
        else
        {
            nOffsetX = 0;
            nOffsetY = 0;
        }
        int nOrigWorldPointX = iWorldX + ArrayRose[i* 2] * 8 + nOffsetX;
        int nOrigWorldPointY = iWorldY + ArrayRose[i * 2 + 1] * 12 + nOffsetY;
        CPatternRose*   pObjPatternRose	= new CPatternRose;
        if (nColor <= 3)
        {
            pObjPatternRose->SetPicture(m_szFileName, 0, 1, 2, 3, nColor);
        }
        else
        {
            int nflower = rand() % 3;
            pObjPatternRose->SetPicture(m_szFileName, nflower, nflower + 3, 6, 7, nflower);
        }
        pObjPatternRose->Create (nOrigWorldPointX , nOrigWorldPointY);
        m_dequePatternRose.push_back(pObjPatternRose);
    }
    return 1;
}


void PatternRose::Destroy()
{
    if (m_nStatus == PATTERNROSE_NONE || m_nStatus == PATTERNROSE_DESTROY)
    {
        return;
    }
    int i;
    for(i = 0 ; i < (int)m_dequePatternRose.size(); i++)
    {
        SAFE_DELETE(m_dequePatternRose[i]);
    }
    m_dequePatternRose.clear();
    m_nStatus = PATTERNROSE_DESTROY;
}


void PatternRose::Process()
{
    int i;
    int iFlag = 1;
    for(i = 0; i <(int) m_dequePatternRose.size(); i++)
    {
        m_dequePatternRose[i]->Process();
        if (m_dequePatternRose[i]->m_iStatus != ROSE_DESTROY)//PATTERNROSE_DESTROY
        {
            iFlag = 0;
        }
    }
    if (iFlag)
    {
        Destroy();
    }
}

//==========================================================================
//
//   PatternRose INTERFACE
//==========================================================================
BOOL CPatternRoseSys::Create(int iWorldX, int iWorldY, int nColor, char* szChar)
{
    int nsl = strlen(szChar);
    nsl = nsl * 24 * 8 + (nsl - 2) / 2 * 15;
    iWorldX -= nsl / 4;
    iWorldY -= 48 * 2;
    CMyPos pos ;
    g_objGameMap.GetViewPos(pos);
    if ( abs(  pos.x + 300 - iWorldX) < 1500 && abs( pos.y + 200 - iWorldY) < 800)
    {
        PatternRose* pObjPatternRose = new PatternRose;
        if (pObjPatternRose)
        {
            pObjPatternRose->Create(iWorldX, iWorldY, nColor, szChar);
            m_DequePatternRose.push_back(pObjPatternRose);
            return 1;
        }
    }
    return 0;
}

BOOL CPatternRoseSys::Create(int iWorldX, int iWorldY, int nType, int nColor)
{
    switch(nType)
    {
    case 1:
    case 2:
    case 3:
        iWorldX -= 30;
        iWorldY -= 20;
        break;
    case 4:
    case 5:
        iWorldX -= 80;
        iWorldY -= 30;
        break;
    default:
        iWorldX -= 20;
        iWorldY -= 20;
        break;
    }
    CMyPos pos ;
    g_objGameMap.GetViewPos(pos);
    if ( abs(  pos.x + 600 - iWorldX) <  2000 && abs( pos.y + 300 - iWorldY) < 1200)
    {
        PatternRose* pObjPatternRose = new PatternRose;
        if (pObjPatternRose)
        {
            pObjPatternRose->Create(iWorldX, iWorldY, nType, nColor);
            m_DequePatternRose.push_back(pObjPatternRose);
            return 1;
        }
    }
    return 0;
}

void CPatternRoseSys::Destroy()
{
    int i;
    for(i = 0; i < (int)m_DequePatternRose.size(); i++)
    {
        SAFE_DELETE(m_DequePatternRose[i]);
    }
    m_DequePatternRose.clear();
}
void CPatternRoseSys::Show()
{
    int i, iDestroyFlag = 1;
    for(i = 0; i < (int)m_DequePatternRose.size(); i++)
    {
        m_DequePatternRose[i]->Process();
        if (m_DequePatternRose[i]->m_nStatus != PATTERNROSE_DESTROY)
        {
            iDestroyFlag = 0;
        }
    }
    if (iDestroyFlag)
    {
        for(i = 0; i < (int)m_DequePatternRose.size(); i++)
        {
            SAFE_DELETE( m_DequePatternRose[i]);
        }
        m_DequePatternRose.clear();
    }
}

