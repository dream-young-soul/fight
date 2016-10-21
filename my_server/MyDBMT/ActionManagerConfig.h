#ifndef _ACTIONMANAGERCONFIG_H_
#define _ACTIONMANAGERCONFIG_H_
/****************************/
//之前的脚本是集体放在mysql cq_action数据库，导致查找不方便，现在归纳到txt，可以分开脚本，以便清晰对应查找脚本函数
//@author 后天
//@time 2016.10.20
/***************************/
#include <map>
struct tagAction
{
	unsigned int id;
	unsigned int id_next;
	unsigned int id_next_fail;
	unsigned int type;
	unsigned int data;
	char szParam[128];
	tagAction()
	{
		id = id_next = id_next_fail = type = data = 0;
		memset(szParam,0,sizeof(szParam));
	}

	unsigned int GetID(){return id;}
	unsigned int GetKey(){return id;}
	unsigned int GetInt(int idx)
	{
		switch(idx)
		{

		case 0://ACTIONDATA_ID_
			{
				return id;
			}
		case 1://ACTIONDATA_IDNEXT
			{
				return id_next;
			}
		case 2://ACTIONDATA_IDNEXTFAIL
			{
				return id_next_fail;
			}
		case 3: //ACTIONDATA_TYPE
			{
				return type;
			}
		case 4: //ACTIONDATA_DATA
			{
				return data;
			}
		}
		return 0;
	}

	const char* GetStr(int idx)
	{
		//ACTIONDATA_PARAM
		return szParam;
	}
};
class CActionManagerConfig
{
public:
	CActionManagerConfig(void);
	~CActionManagerConfig(void);
	static CActionManagerConfig* GetInstance();
	void Release();
public:
	bool Load();
	bool LoadScripte(const char* pSzFile);
	bool ReLoad();
	tagAction* GetActionData(unsigned int id);
private:
	std::map<unsigned int,tagAction> m_mapAction;

	tagAction LoadActionData(const char* szBuff);
	
};

#endif