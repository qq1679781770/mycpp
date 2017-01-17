#pragma once
#include<string>
#include"rapidjson\include\rapidjson\document.h"
#include"rapidjson\include\rapidjson\writer.h"
#include"rapidjson\include\rapidjson\stringbuffer.h"
class JsonCpp
{
	typedef rapidjson::Writer<rapidjson::StringBuffer>JsonWriter;
public:
	JsonCpp():m_writer(m_buf){}
	~ JsonCpp(){}
	void StartArray()
	{
		m_writer.StartArray();
	}
	void EndArray()
	{
		m_writer.EndArray();
	}
	void StartObject()
	{
		m_writer.StartObject();
	}
	void EndObject()
	{
		m_writer.EndObject();
	}
	const char* GetString()const
	{
		return m_buf.GetString();
	}
	template<typename T>
	void WriteJson(std::string& key, T&& value)
	{
		m_writer.String(key.c_str());
		WriteValue(std::forward<T>(value));
	}
	template<typename T>
	void WriteJson(const char* key, T&& value)
	{
		m_writer.String(key);
		WriteValue(std::forward<T>(value));
	}
private:
	rapidjson::StringBuffer m_buf;
	JsonWriter m_writer;
	rapidjson::Document m_doc;
private:
	template<typename V>
	typename std::enable_if<std::is_same<V, int>::value>::type WriteValue(V value)
	{
		m_writer.Int(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, unsigned int>::value>::type WriteValue(V value)
	{
		m_writer.Uint(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, int64_t>::value>::type WriteValue(V value)
	{
		m_writer.Int64(value);
	}
	template<typename V>
	typename std::enable_if<std::is_floating_point<V>::value>::type WriteValue(V value)
	{
		m_writer.Double(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V,bool>::value>::type WriteValue(V value)
	{
		m_writer.Bool(value);
	}
	template<typename V>
	typename std::enable_if<std::is_pointer<V>::value>::type WriteValue(V value)
	{
		m_writer.String(value);
	}
	template<typename V>
	typename std::enable_if<std::is_array<V>::value>::type WriteValue(V value)
	{
		m_writer.String(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V,std::nullptr_t>::value>::type WriteValue(V value)
	{
		m_writer.Null();
	}
};