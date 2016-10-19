
//**********************************************************
// 代码编辑器
//**********************************************************

// INI文件
// 仙剑修，2001.11.19

// 只读配置文件接口
// 用于读取格式化的字符串和整数。
// 具体实例参考工程中的INI文件和本文后的说明

#ifndef INIFILE_H
#define INIFILE_H


// 所有的KEY串和数据串长不能大于512。
#define INIWORDSIZE		512			// INI 中使用的串的空间长度
#define	INIDATASIZE		8192		//	INI 中的数据串长度

class	CIniFile
{
public:
    // pFilename：要读取的文件名
    // pSection: 要读取的区域。例：在INI文件中的[SECTION]部分，pSection为 "SECTION" 。
    CIniFile(const char* pFilename, const char* pSection);
    ~CIniFile();
public:
    // 重新设计要读取的区域。
    // pSect：区域名的串
    void	SetSection(const char* pSect);

    // 从指定区域中读取一个“串”数据
    // pKey:	“=”号前的KEY串。
    // bufString:	预先分配的串空间。必须够大，否则只能读入部分串。
    // nBufSize：	bufString所指向的空间的尺寸。
    // return:		有KEY串时，返回true。否则返回false
    bool	GetString(char* bufString, const char* pKey, int nBufSize);

    // 从指定区域中读取一个整数
    // pKey:	“=”号前的KEY串
    // return :	取出的整数。(无KEY时返回0)
    int		GetInt(const char* pKey);

    // 以下为内部属性 ////////////////////////////////////////////////////////////
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
				(=)前后可以有空白。
				STRING不能用(")括住，否则会被当着字符串的一部分。尾空会被删掉。
				INT为十进制数。
				STRING或INT后不能有注释。

	COMMENT：	(;)必须顶行，所有内容无意义。
	空行：		无意义。



//*/
