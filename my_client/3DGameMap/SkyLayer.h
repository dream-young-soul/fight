
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef HEADER_SKY_KATTY
#define HEADER_SKY_KATTY
#include "maplayer.h"
#include "weather.h"
#include "airbubble.h"

class CSkyLayer : public CMapLayer
{
public:
    CSkyLayer() {}
    ~CSkyLayer() {Destroy();}
    void Destroy();
    void Show(CMyPos posShow);
    void Process(void* pInfo);
    int  GetType() {return LAYER_SKY;}
    static CSkyLayer* CreateNew();

    void CreateFireWork_Char(int nScreenX, int nScreenY, char* szLetter , int nType, DWORD dwDelayTime) {m_objCharFirework.Create(nScreenX, nScreenY, szLetter, nType, dwDelayTime);}
    void CreateFireWork(int nScreenX, int nScreenY,  int nRadius , int nColor, DWORD dwDelayTime = 0) {m_objfirework.Create(nScreenX, nScreenY, 1, nRadius, 0, nColor, dwDelayTime);}
    void CreateAirBubble(int nBubbleNum) { m_objAirBubble.Create(nBubbleNum, 0);}


    BOOL CreateWeather(int nType, int nWindDir, int nLevel, DWORD dwColor = 0x00ffffff) {return m_objWeather.CreateEnv(nType, nWindDir, nLevel, dwColor);}
    void SetWeather(int nWindDir, int nLevel) {m_objWeather.SetEnv(nWindDir, nLevel);}
    void StopWeather() {m_objWeather.StopEnv();}
    void DestroyWeather() {m_objWeather.Destroy();}
    void PauseWeather() {m_objWeather.Pause();}
    void ContinueWeather() {m_objWeather.Continue();}
    int  GetWeathType() {return m_objWeather.m_nType;}

    void AddObj(CMapObj* obj);
    void ClearObj();



private:
    CWeather		m_objWeather;
    Firework		m_objfirework;
    CFireWorkChar	m_objCharFirework;
    CAirBubbleSys	m_objAirBubble;
    DEQUE_MAPOBJ	m_dequeMapObj;
};


#endif
