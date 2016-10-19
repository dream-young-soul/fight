
//**********************************************************
// 代码编辑器
//**********************************************************

// INI文件
// 仙剑修，2001.11.19


#ifndef INIFILE_H
#define INIFILE_H


#define INIWORDSIZE		512			//? INI 中使用的串的空间长度


class	CIniFile
{
public:
    CIniFile(const char* pFilename, const char* pSection);
    ~CIniFile();
public:
    void	SetSection(const char* pSect);
    bool	GetString(const char* pKey, char* bufString);
    int		GetInt(const char* pKey);
protected:
    char m_bufFilename[INIWORDSIZE];
    char m_bufSection[INIWORDSIZE];
};


#endif // INIFILE_H


/*	INI文件格式规范

[SECTION]

KEY1=STRING

KEY2=INT

; COMMENT


	说明：

	SECTION行：	必须为“标识符”，不区分大小写。([)必须顶行，([])中间不能有空白。(])后面可以带注释。

	KEY行：		KEY必须顶行，必须为“标识符”，不区分大小写。
				(=)前后不能有空白，否则会出错。
				STRING不能用(")括住，否则会被当着字符串的一部分。尾空会被删掉。
				INT为十进制数。
				STRING或INT后不能有注释。

	COMMENT：	(;)必须顶行，所有内容无意义。
	空行：		无意义。



//*/
