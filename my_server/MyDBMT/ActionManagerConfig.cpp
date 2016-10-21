
#include "ActionManagerConfig.h"
#include "logfile.h"
#include <fstream>

static CActionManagerConfig* m_Instance = nullptr;
CActionManagerConfig::CActionManagerConfig(void)
{
	m_mapAction.clear();
}


CActionManagerConfig::~CActionManagerConfig(void)
{
}
void CActionManagerConfig::Release()
{
	if(m_Instance != nullptr)
	{
		delete m_Instance;
	}
}
bool CActionManagerConfig::LoadScripte(const char* pSzFile)
{
	std::ifstream f(pSzFile);
	if(!f)
	{
		return false;
	}
	char szBuff[1024];
	while(!f.eof())
	{
		f.getline(szBuff,sizeof(szBuff));
		if(strlen(szBuff) == 0)continue;
		//◊¢ Õ
		if(szBuff[0] == '/' && szBuff[1] == '/')
		{
			continue;
		}
		tagAction action = this->LoadActionData(szBuff);
		if(action.id == 0) //∂¡»° ß∞‹
		{
			LOGMSG("LoadScripte error!id ==0 %s",pSzFile);
			return false;
		}
		std::map<unsigned int,tagAction>::iterator it = m_mapAction.find(action.id);
		
		if(it != m_mapAction.end())//÷ÿ∏¥
		{
			LOGMSG("’“µΩ÷ÿ∏¥key_id %s id:%d",pSzFile,action.id);
			return false;
		}
		m_mapAction[action.id] = action;
	}
	f.close();
	return true;

}
tagAction* CActionManagerConfig::GetActionData(unsigned int id)
{
	std::map<unsigned int,tagAction>::iterator it = m_mapAction.find(id);
	if(it == m_mapAction.end())
	{
		LOGMSG("GetActionData error! id=%d",id);
		return nullptr;
	}
	return &it->second;
}
tagAction CActionManagerConfig::LoadActionData(const char* szBuff)
{
	tagAction action;
	const char tabKeyCode = 9;
	std::string str = szBuff;
	int nPos = 0,nFindPos = 0,nIndex =0;
	while(true)
	{
		nFindPos = str.find(tabKeyCode,nPos);
		if(nFindPos == std::string::npos && nIndex == 6)
		{
			break;
		}
		if(nIndex == 5)
		{
			nFindPos = str.length();
		}
		std::string sub = str.substr(nPos,nFindPos - nPos);
		switch(nIndex)
		{
			case 0: //id
				{
					action.id = atoi(sub.c_str());
					break;
				}
			case 1: //id_next
				{
					action.id_next = atoi(sub.c_str());
					break;
				}
			case 2: //id_next_fail
				{
					action.id_next_fail = atoi(sub.c_str());
					break;
				}
			case 3: //type
				{

					action.type = atoi(sub.c_str());
					break;
				}
			case 4: //data
				{

					action.data = atoi(sub.c_str());
					break;
				}
			case 5: //param
				{
					if(sub.length() > sizeof(action.szParam))
					{
						return action;
					}
					strcpy(action.szParam,sub.c_str());
					break;
				}
		}
		nIndex ++;
		nPos = nFindPos + 1;
	}
	return action;
}

bool CActionManagerConfig::ReLoad()
{
	m_mapAction.clear();
	return this->Load();
}
bool CActionManagerConfig::Load()
{
	const char pszFile[] = "scripte/scripte.txt";
	std::ifstream f(pszFile);
	if(!f)
	{
		return false;
	}

	char szBuff[1024];
	while(!f.eof())
	{
		f.getline(szBuff,sizeof(szBuff));
		if(!this->LoadScripte(szBuff))
		{
			return false;
		}
	}
	f.close();
	return true;
}

CActionManagerConfig* CActionManagerConfig::GetInstance()
{
	if(!m_Instance)
	{

		m_Instance = new CActionManagerConfig();
	}
	return m_Instance;
}