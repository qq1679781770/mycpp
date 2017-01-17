#pragma once
#include<iostream>
#include<string>
#include<functional>
#include"rapidjson/include/rapidjson/document.h"
#include"rapidjson/include/rapidjson/stringbuffer.h"
#include"rapidjson/include/rapidjson/writer.h"
#include"sqlite/sqlite3.h"
#include<unordered_map>
#include<boost/variant.hpp>
typedef boost::variant<double, int, uint32_t, sqlite3_int64, char*, const char*, std::string, nullptr_t>SqliteValue;
using JsonBuilder = rapidjson::Writer<rapidjson::StringBuffer>;
template<int...>
struct IndexTuple {};
template<int N,int...Indexes>
struct MakeIndexes:MakeIndexes<N-1,N-1,Indexes...>{};
template<int...Indexes>
struct MakeIndexes<0, Indexes...>
{
	typedef IndexTuple<Indexes...>type;
};
class NonCopyable
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable(NonCopyable const&) = delete;
	NonCopyable& operator=(NonCopyable const&) = delete;
};
class SmartDB :NonCopyable
{
public:
	SmartDB():m_jsonBuilder(m_buf){}
	explicit SmartDB(const std::string& filename):m_jsonBuilder(m_buf)
	{
		Open(filename);
	}
	/*
	**打开数据库
	*/
	bool Open(const std::string& filename)
	{
		m_code = sqlite3_open(filename.data(), &m_dbHandle);
		return (SQLITE_OK == m_code);
	}
	/*
	**释放资源，关闭数据库
	*/
	bool Close()
	{
		if (m_dbHandle == nullptr)
		{
			return true;
		}
		sqlite3_finalize(m_statement);
		m_code = CloseDBHandle();
		m_statement = nullptr;
		m_dbHandle = nullptr;
		return SQLITE_OK == m_code;
	}
	int GetLastErrorCode()
	{
		return m_code;
	}
private:
	sqlite3* m_dbHandle;
	sqlite3_stmt* m_statement;
	int m_code;//记录最近一次错误码
	static std::unordered_map<int, std::function<SqliteValue(sqlite3_stmt*, int)>>m_valmap;
private:
	int CloseDBHandle()
	{
		int code = sqlite3_close(m_dbHandle);
		while (code == SQLITE_BUSY)
		{
			sqlite3_stmt* stmt = sqlite3_next_stmt(m_dbHandle, NULL);
			if (stmt == nullptr)
			{
				break;
			}
			code = sqlite3_finalize(stmt);
			if (code == SQLITE_OK)
			{
				code = sqlite3_close(m_dbHandle);
			}
		}
		return code;
	}
private:
	/*
	**统一参数绑定
	*/
	int BindParams(sqlite3_stmt* stmt, int current)
	{
		return SQLITE_OK;
	}
	template<typename T,typename...Args>
	int BindParams(sqlite3_stmt* stmt, int current, T&& first, Args...args)
	{
		BindValue(stmt, current, first);
		if (m_code != SQLITE_OK)
		{
			return m_code;
		}
		BindParams(stmt, current + 1, args...);
		return m_code;
	}
	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value>::type BindValue(sqlite3_stmt* stmt, int current, T t)
	{
		m_code = sqlite3_bind_double(stmt, current, std::forward<T>(t));
	}
	template<typename T>
	typename std::enable_if<std::is_integral<T>::value>::type BindValue(sqlite3_stmt* stmt, int current, T t)
	{
		BindIntValue(stmt, current, t);
	}
	template<typename T>
	typename std::enable_if<std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value>::type BindIntValue(sqlite3_stmt* stmt, int current, T t)
	{
		m_code = sqlite3_bind_int64(stmt, current, std::forward<T>(t));
	}
	template<typename T>
	typename std::enable_if<!std::is_same<T, int64_t>::value&&!std::is_same<T, uint64_t>::value>::type BindIntValue(sqlite3_stmt* stmt, int current, T t)
	{
		m_code = sqlite3_bind_int(stmt, current, std::forward<T>(t));
	}
	template<typename T>
	typename std::enable_if<std::is_same<T, std::string>::value>::type BindValue(sqlite3_stmt* stmt, int current, const T& t)
	{
		m_code = sqlite3_bind_text(stmt, current, t.data(), t.length(), SQLITE_TRANSIENT);
	}
	template<typename T>
	typename std::enable_if<std::is_same<T, char*>::value || std::is_same<T, const char*>::value>::type BindValue(sqlite3_stmt* stmt, int current, T t)
	{
		m_code = sqlite3_bind_text(stmt, current, t, strlen(t) + 1, SQLITE_TRANSIENT);
	}
	/*template<typename T>
	typename std::enable_if<std::is_same<T, blob>::value>::type BindValue(sqlite3_stmt* stmt, int current,const T& t)
	{
		m_code = sqlite3_bind_blob(stmt, current, t.pBuf, t.size, SQLITE_TRANSIENT);
	}*/
	template<typename T>
	typename std::enable_if<std::is_same<nullptr_t, T>::value>::type BindValue(sqlite3_stmt* stmt, int current,const T& t)
	{
		m_code = sqlite3_bind_null(stmt, current);
	}
/*实现excecute*/
public:
	/*
	** 不带占位符
	*/
	bool Excecute(const std::string& sqlstr)
	{
		m_code = sqlite3_exec(m_dbHandle, sqlstr.data(), nullptr, nullptr, nullptr);
		return SQLITE_OK == m_code;
	}
	/*
	**带占位符
	*/
	template<typename...Args>
	bool Excecute(const std::string& sqlstr, Args&&...args)
	{
		if (!Prepare(sqlstr))
		{
			return false;
		}
		return ExcecuteArgs(std::forward<Args>(args)...);
	}
	/*
	**解析和保存sql语句
	*/
	bool Prepare(const std::string& sqlstr)
	{
		m_code = sqlite3_prepare_v2(m_dbHandle, sqlstr.data(), -1, &m_statement, nullptr);
		if (m_code != SQLITE_OK)
		{
			return false;
		}
		return true;
	}
	template<typename...Args>
	bool ExcecuteArgs(Args&&...args)
	{
		if (SQLITE_OK != BindParams(m_statement, 1, std::forward<Args>(args)...))
		{
			return false;
		}
		m_code = sqlite3_step(m_statement);
		sqlite3_reset(m_statement);
		return m_code == SQLITE_OK;
	}
	/*
	** 实现excecutescalar
	*/
	template<typename R=sqlite_int64,typename...Args>
	R ExcecuteScalar(const std::string& sqlstr, Args&&...args)
	{
		if (!Prepare(sqlstr))
		{
			return GetErrorVal<R>();
		}
		if (SQLITE_OK != BindParams(m_statement, 1, std::forward<Args>(args)...))
		{
			return GetErrorVal<R>();
		}
		m_code = sqlite3_step(m_statement);
		if (m_code != SQLITE_OK)
		{
			return GetErrorVal<R>();
		}
		SqliteValue val = GetValue(m_statement, 0);
		R result = std::get<R>(val);
		sqlite3_reset(m_statement);
		return result;
	}
	/*取列的值*/
	SqliteValue GetValue(sqlite3_stmt* stmt, int index)
	{
		int type = sqlite3_column_type(stmt, index);
		auto it = m_valmap.find(type);
		if (it == m_valmap.end())
		{
			throw;
		}
		return it->second(stmt, index);
	}
	/*返回无效值*/
	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type GetErrorVal()
	{
		return T(-9999);
	}
	/*实现ExcecuteTuple*/
	template<class Tuple>
	bool ExcecuteTuple(const std::string& sqlstr, Tuple&& t)
	{
		if (!Prepare(sqlstr))
		{
			return false;
		}
		return ExcecuteTuple(MakeIndexes<std::tuple_size<Tuple>::value>::type(), std::forward<Tuple>(t));
	}
	template<int...Indexes,class Tuple>
	bool ExcecuteTuple(IndexTuple<Indexes...>&& in, Tuple&& t)
	{
		if (SQLITE_OK != BindParams(m_statement, 1, get<Indexes>(std::forward<Tuple>(t))...))
		{
			return false;
		}
		m_code = sqlite3_step(m_statement);
		sqlite3_reset(m_statement);
		return m_code = SQLITE_OK;
	}
	/*实现json接口*/
	bool ExcecuteJson(const std::string& sqlstr, const char* json)
	{
		rapidjson::Document doc;
		doc.Parse<0>(json);
		if (doc.HasParseError())
		{
			std::cout << doc.GetParseError() << std::endl;
			return false;
		}
		if (!Prepare(sqlstr))
		{
			return false;
		}
		return JsonTransaction(doc);
	}
	bool JsonTransaction(const rapidjson::Document& doc)
	{
		Begin();
		for (size_t i = 0, size = doc.Size(); i < size; ++i)
		{
			if (!ExcecuteJson(doc[i]))
			{
				RollBack();
				break;
			}
		}
		if (m_code != SQLITE_OK)
		{
			return false;
		}
		Commit();
		return true;
	}
	bool ExcecuteJson(const rapidjson::Value& val)
	{
		auto p = val.GetKeyPtr();
		for (size_t i = 0, size = val.GetSize(); i < size; ++i)
		{
			const char* key = val.GetKey(p++);
			auto& t = val[key];
			BindJsonValue(t, i + 1);
		}
		m_code = sqlite3_step(m_statement);
		sqlite3_reset(m_statement);
		return SQLITE_OK == m_code;
	}
	void BindJsonValue(const rapidjson::Value& t, int index)
	{
		auto type = t.GetType();
		if (type == rapidjson::kNullType)
		{
			m_code = sqlite3_bind_null(m_statement, index);
		}
		else if (type == rapidjson::kStringType)
		{
			m_code = sqlite3_bind_text(m_statement, index, t.GetString(), -1, SQLITE_STATIC);
		}
		else if (type = rapidjson::kNumberType)
		{
			BindNumber(t, index);
		}
		else
		{
			throw std::invalid_argument("can't find this type");
		}
	}
	void BindNumber(const rapidjson::Value& t, int index)
	{
		if (t.IsInt() || t.IsUint())
			m_code = sqlite3_bind_int(m_statement, index, t.GetInt());
		else if (t.IsInt64() || t.IsUint64())
			m_code = sqlite3_bind_int64(m_statement, index, t.GetInt64());
		else
			m_code = sqlite3_bind_double(m_statement, index, t.GetDouble());
	}
	/*事务接口*/
	bool Begin()
	{
		return Excecute("BEGIN");
	}
	bool RollBack()
	{
		return Excecute("ROLLBACK");
	}
	bool Commit()
	{
		return Excecute("COMMIT");
	}
private:
	JsonBuilder m_jsonBuilder;
	rapidjson::StringBuffer m_buf;
	static std::unordered_map<int, std::function<void(sqlite3_stmt* stmt, int index, JsonBuilder&)>>m_builderMap;
public:
	/*
	**json查询接口
	*/
	//
	std::shared_ptr<rapidjson::Document> Query(const std::string& sqlstr)
	{
		if (!Prepare(sqlstr))
		{
			return nullptr;
		}
		auto doc = std::make_shared<rapidjson::Document>();
		m_buf.Clear();
		BuildJsonObject();
		doc->Parse<0>(m_buf.GetString());
		return doc;
	}
	//创建json对象
	void BuildJsonObject()
	{
		int colCount = sqlite3_column_count(m_statement);
		m_jsonBuilder.StartArray();
		while (true)
		{
			m_code = sqlite3_step(m_statement);
			if (m_code == SQLITE_DONE)
			{
				break;
			}
			BuildJsonArray(colCount);
		}
		m_jsonBuilder.EndArray();
		sqlite3_reset(m_statement);
	}
	//创建json对象列表
	void BuildJsonArray(int colCount)
	{
		m_jsonBuilder.StartObject();
		for (int i = 0; i < colCount; ++i)
		{
			char* name = (char*)sqlite3_column_name(m_statement, i);
			m_jsonBuilder.String(name);
			BuildJsonValue(m_statement, i);
		}
		m_jsonBuilder.EndObject();
	}
	//创建json值
	void BuildJsonValue(sqlite3_stmt* stmt, int index)
	{
		int type = sqlite3_column_type(stmt, index);
		auto it = m_builderMap.find(type);
		if (it == m_builderMap.end())
		{
			throw;
		}
		it->second(stmt, index, m_jsonBuilder);
	}
};
std::unordered_map<int,std::function<SqliteValue(sqlite3_stmt*,int)>>SmartDB::m_valmap=
{
	{std::make_pair(SQLITE_INTEGER,[](sqlite3_stmt* stmt,int index) {return sqlite3_column_int64(stmt,index); })},
	{ std::make_pair(SQLITE_FLOAT,[](sqlite3_stmt* stmt,int index) {return sqlite3_column_double(stmt,index); }) },
	{ std::make_pair(SQLITE_TEXT,[](sqlite3_stmt* stmt,int index) {return std::string((const char*)sqlite3_column_text(stmt,index)); }) },
	{std::make_pair(SQLITE_NULL,[](sqlite3_stmt* stmt,int index) {return nullptr; })}
};
std::unordered_map<int,std::function<void(sqlite3_stmt* stmt,int index,JsonBuilder&)>>SmartDB::m_builderMap
{
	{std::make_pair(SQLITE_INTEGER,[](sqlite3_stmt* stmt,int index,JsonBuilder& builder) {builder.Int64(sqlite3_column_int64(stmt,index)); })},
	{ std::make_pair(SQLITE_FLOAT,[](sqlite3_stmt* stmt,int index,JsonBuilder& builder) {builder.Double(sqlite3_column_double(stmt,index)); }) },
	{ std::make_pair(SQLITE_TEXT,[](sqlite3_stmt* stmt,int index,JsonBuilder& builder) {builder.String((const char*)(sqlite3_column_text(stmt,index))); }) },
	{std::make_pair(SQLITE_NULL,[](sqlite3_stmt* stmt,int index,JsonBuilder& builder) {builder.Null(); })}
};