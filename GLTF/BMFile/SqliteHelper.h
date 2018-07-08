
#ifndef _SQLITEHELPER_H
#define _SQLITEHELPER_H
#include <sqlite3.h>
#include <iostream>
class SqliteHelper
{
public:
	SqliteHelper(void);
	~SqliteHelper(void);
	//////////////////////////////////////////////////////////////////////////
	//此包装类有很多问题不想改了---JGB
	//////////////////////////////////////////////////////////////////////////
	// 打开数据库
	int OpenDB( bool bIsMemory);
	// 关闭数据库
	int CloseDB();
	// 创建一张表
	int CreateTable(const char* table_name_and_field);
	// 删除一张表
	int DropTable(const char *table_name);
	// 查询操作
	int Select(const char *select_state);
	// 插入操作
	int Insert(const char *insert_state);
	// 删除操作
	int Delete(const char *delete_state);
	// 更新操作
	int Update(const char *update_state);
	//保存内存数据库
	int SaveFile(const char* path);
	// 执行sql语句
	int SqlStateExec(const char *sql_state);
	// 执行sql语句 Lizhensheng 2015/10/20 为了传入回调接口
	int SqlStateExec(const char *sql_state, int(*callback)(void*, int, char**, char**));
	//合并数据库
	int SqlMergeDataFile(const char* sMergeFilePath);
	// 是否关闭
	bool GetIsClose();
	sqlite3* GetSQLite(){ return sqlite_db_; }

	//int SqliteHelper::CommitTransaction()
	int CommitTransaction();
	int BeginTransaction();
	//
	std::string m_DBPath;
private:
	sqlite3 *sqlite_db_;// 数据库的指针
	char* err_msg_;		// 错误信息
	bool is_close_;		// 关闭数据的标识

protected:
	// 主要用在selece操作中的显示数据元素
	static int CallBackFunc(void *NotUsed, int argc, char **argv, char **azColName);
	//
	int loadOrSaveDb(sqlite3 *pInMemeory, const char *zFilename, int isSave);

};


#endif // _SQLITEHELPER_H


