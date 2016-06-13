#ifndef __JSONVALUEADAPTER_H__
#define __JSONVALUEADAPTER_H__

#include "jsonrpc/json/value.h"
#include <string>
#include <sstream>
#include <stdlib.h>

struct JsonValueAdapter
{
	JsonValueAdapter(const Json::Value& value)
		: m_value(value)
	{
	}

	~JsonValueAdapter()
	{
		m_value = m_value;
	}

	operator const char * ()
	{
		if (m_value.isConvertibleTo(Json::stringValue))
		{
			m_string = m_value.asString();
			return m_string.c_str();
		}
		else
		{
			if (m_value.isInt())
			{
				std::stringstream strstream;
				strstream << m_value.asInt();
				m_string = strstream.str();
				return m_string.c_str();
			}
			else if (m_value.isUInt())
			{
				std::stringstream strstream;
				strstream << m_value.asUInt();
				m_string = strstream.str();
				return m_string.c_str();
			}
			else if (m_value.isDouble())
			{
				std::stringstream strstream;
				strstream << m_value.asDouble();
				m_string = strstream.str();
				return m_string.c_str();
			}
		}
		return nullptr;
	}

	operator const std::string ()
	{
		if (m_value.isConvertibleTo(Json::stringValue))
		{
			return m_value.asString();
		}
		else
		{
			if (m_value.isInt())
			{
				std::stringstream strstream;
				strstream << m_value.asInt();
				return strstream.str();
			}
			else if (m_value.isUInt())
			{
				std::stringstream strstream;
				strstream << m_value.asUInt();
				return strstream.str();

			}
			else if (m_value.isDouble())
			{
				std::stringstream strstream;
				strstream << m_value.asDouble();
				return strstream.str();
			}
		}
		return std::string();
	}

	operator int ()
	{
		if (m_value.isConvertibleTo(Json::intValue))
		{
			return m_value.asInt();
		}
		else
		{
			if (m_value.isString())
			{
				return atoi(m_value.asCString());
			}
		}
		return 0;
	}

	operator unsigned ()
	{
		if (m_value.isConvertibleTo(Json::uintValue))
		{
			return m_value.asUInt();
		}
		else
		{
			if (m_value.isString())
			{
				return atoi(m_value.asCString());
			}
		}
		return 0;
	}

	operator double ()
	{
		if (m_value.isConvertibleTo(Json::realValue))
		{
			return m_value.asDouble();
		}
		else
		{
			if (m_value.isString())
			{
				return atof(m_value.asCString());
			}
		}
		return 0;
	}

	operator bool ()
	{
		if (m_value.isConvertibleTo(Json::booleanValue))
		{
			return m_value.asBool();
		}
		if (m_value.isString())
		{	
			std::string str = m_value.asString();
			return str == "true" || str == "1" || str == "TRUE";
		}
		return false;
	}

	Json::Value m_value;
	std::string m_string;
};

#endif
