#pragma once

#include <QString>

class StringTool
{
public:
	template <typename, typename = std::void_t<>>
	struct _HasToString : std::false_type
	{
	};

	template <typename T>
	struct _HasToString<T, std::void_t<decltype(std::declval<T>()._to_string())>> : std::true_type
	{
	};

	template <typename T>
	static std::enable_if_t<_HasToString<T>::value, T> to(const QString& str)
	{
		return T::_from_string(str.toStdString().c_str());
	};

	template <typename T>
	static std::enable_if_t<!_HasToString<T>::value, T> to(const QString& str)
	{
		return str;
	};

	template <>
	static int to<int>(const QString& str)
	{
		return str.toInt();
	};

	template <>
	static float to<float>(const QString& str)
	{
		return str.toFloat();
	};

	template <>
	static bool to<bool>(const QString& str)
	{
		return str == "true";
	};

	template <typename T>
	static QString from(std::enable_if_t<_HasToString<T>::value, T>&& data)
	{
		return data._to_string();
	};

	template <typename T>
	static QString from(std::enable_if_t<!_HasToString<T>::value, T>&& data)
	{
		return data;
	};

	template <>
	static QString from<int>(int&& data)
	{
		return QString::number(data);
	};

	template <>
	static QString from<float>(float&& data)
	{
		return QString::number(data);
	};

	template <>
	static QString from<bool>(bool&& data)
	{
		return data ? QString("true") : QString("false");
	};
};