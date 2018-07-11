
#include "SqliteHelper.h"
#include <string>
#include <locale.h>

SqliteHelper::SqliteHelper(void) :
	sqlite_db_(NULL)
	, is_close_(true)
{
}


SqliteHelper::~SqliteHelper(void)
{
	CloseDB();
	//SaveFile(m_DBPath.c_str());
}
// 打开数据库
int SqliteHelper::OpenDB( bool bIsMemory)
{
	if (!is_close_) return 0;
	//
	int res = 0;
	if (bIsMemory)
	{
		res = sqlite3_open(":memory:", &sqlite_db_);
		res = loadOrSaveDb(sqlite_db_, m_DBPath.c_str(), 0);
	}
	else
	{
		res = sqlite3_open(m_DBPath.c_str(), &sqlite_db_);
	}
	is_close_ = false;
	if (res)
	{
		sqlite3_close(sqlite_db_);
		is_close_ = true;
	}
	return res;
}

// 关闭数据库
int SqliteHelper::CloseDB()
{
	int res = 0;
	if (!is_close_)
	{
		res = sqlite3_close(sqlite_db_);
		is_close_ = true;
	}
	return res;
}
//
int SqliteHelper::SaveFile(const char* path)
{
	int rc;
	rc = loadOrSaveDb(sqlite_db_, path, 1);
	return rc;
}

int SqliteHelper::loadOrSaveDb(sqlite3 *pInMemeory, const char *zFilename, int isSave)
{
	int rc;
	sqlite3 *pFile;
	sqlite3_backup *pBackup;
	sqlite3 *pTo;
	sqlite3 *pFrom;
	rc = sqlite3_open(zFilename, &pFile);
	if (rc == SQLITE_OK)
	{
		pFrom = (isSave ? pInMemeory : pFile);
		pTo = (isSave ? pFile : pInMemeory);

		pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
		if (pBackup)
		{
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
		}
		rc = sqlite3_errcode(pTo);
	}
	(void)sqlite3_close(pFile);
	return rc;
}
// 创建一张表
int SqliteHelper::CreateTable(const char* table_name_and_field)
{
	std::string create_table_state = "create table ";
	create_table_state += table_name_and_field;
	create_table_state += ";";
	int res = SqlStateExec(create_table_state.c_str());
	if (res != SQLITE_OK)
	{
		return -1;
	}
	return 0;
}
// 删除一张表
int SqliteHelper::DropTable(const char *table_name)
{
	std::string sql_state = "drop table ";
	sql_state += table_name;
	sql_state += ";";
	int res = SqlStateExec(sql_state.c_str());
	if (res != SQLITE_OK)
	{
		return -1;
	}
	return 0;
}
// 查询操作
int SqliteHelper::Select(const char *select_state)
{
	return -1;
}
// 插入操作
int SqliteHelper::Insert(const char *insert_state)
{
	int res = SqlStateExec(insert_state);
	if (res != SQLITE_OK)
	{
		return -1;
	}
	//CommitTransaction();
	return 0;
}
// 删除操作
int SqliteHelper::Delete(const char *delete_state)
{
	int res = SqlStateExec(delete_state);
	if (res != SQLITE_OK)
	{
		return -1;
	}
	return 0;
}
// 更新操作
int SqliteHelper::Update(const char *update_state)
{
	int res = SqlStateExec(update_state);
	if (res != SQLITE_OK)
	{
		return -1;
	}

	return 0;
}

int SqliteHelper::CallBackFunc(void *not_used, int element_count, char **element, char **col_name)
{
	return 0;
}

int SqliteHelper::SqlStateExec(const char *sql_state)
{
	return sqlite3_exec(sqlite_db_, sql_state, CallBackFunc, 0, &err_msg_);
}

// 执行sql语句 Lizhensheng 2015/10/20 为了传入回调接口
int SqliteHelper::SqlStateExec(const char *sql_state, int(*callback)(void*, int, char**, char**))
{
	return sqlite3_exec(sqlite_db_, sql_state, callback, 0, &err_msg_);
}

int SqliteHelper::SqlMergeDataFile(const char* sMergeFilePath)
{
	int iRes = 0;
	std::string sql_Attach = "ATTACH DATABASE '";
	sql_Attach += sMergeFilePath;
	sql_Attach += "' As 'TEMPB'";
	iRes = sqlite3_exec(sqlite_db_, sql_Attach.c_str(), NULL, NULL, NULL);
	iRes = sqlite3_exec(sqlite_db_, "insert into main.model_tree select  * from TEMPB.model_tree", NULL, NULL, NULL);
	iRes = sqlite3_exec(sqlite_db_, "insert into main.model_property select  * from TEMPB.model_property", NULL, NULL, NULL);
	iRes = sqlite3_exec(sqlite_db_, "insert into main.objecttype select  * from TEMPB.objecttype", NULL, NULL, NULL);
	sql_Attach = "DETACH DATABASE 'TEMPB';";
	iRes = sqlite3_exec(sqlite_db_, sql_Attach.c_str(), NULL, NULL, NULL);
	return iRes;
}

// 是否关闭
bool SqliteHelper::GetIsClose()
{
	return is_close_;
}


int SqliteHelper::BeginTransaction()
{
	int res = sqlite3_exec(sqlite_db_, "begin transaction;", CallBackFunc, 0, &err_msg_);
	return res;
}

int SqliteHelper::CommitTransaction()
{
	int res = sqlite3_exec(sqlite_db_, "commit transaction;", CallBackFunc, 0, &err_msg_);
	return res;
}
