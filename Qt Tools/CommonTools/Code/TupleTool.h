#pragma once

#include <string>
#include <tuple>

#include <QDebug>
#include <QStringList>

namespace TupleTool
{
// ������ڲ�������toString����, ��ʹ��toString
template <class T>
typename std::enable_if<std::is_same<decltype(std::declval<T&>().toString()), std::string>::value, std::string>::type toString(T& t)
{
	return t.toString();
}

// ���std::to_string����ת��, ��ôʹ�ø�ת��
template <class T>
typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type toString(T& t)
{
	return std::to_string(t);
}

template <size_t N = 0, typename... Args>
static typename std::enable_if<N == sizeof...(Args), void>::type print(std::tuple<Args...> t)
{
	// ��NΪ0ʱ, ˵���Ѿ���ȫ����, ����ʲôҲ����
	return;
}

// ʹ�������Ƶ�
template <size_t N = 0, typename... Args>
    static typename std::enable_if < N<sizeof...(Args), void>::type print(std::tuple<Args...> t)
{
	qDebug() << std::get<N>(t);
	print<N + 1>(t);
}
} // namespace TupleTool