#pragma once

#include <string>
#include <tuple>

#include <QDebug>
#include <QStringList>

namespace TupleTool
{
// 如果类内部定义了toString函数, 则使用toString
template <class T>
typename std::enable_if<std::is_same<decltype(std::declval<T&>().toString()), std::string>::value, std::string>::type toString(T& t)
{
	return t.toString();
}

// 如果std::to_string可以转换, 那么使用该转换
template <class T>
typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type toString(T& t)
{
	return std::to_string(t);
}

template <size_t N = 0, typename... Args>
static typename std::enable_if<N == sizeof...(Args), void>::type print(std::tuple<Args...> t)
{
	// 当N为0时, 说明已经完全遍历, 所以什么也不做
	return;
}

// 使用类型推导
template <size_t N = 0, typename... Args>
    static typename std::enable_if < N<sizeof...(Args), void>::type print(std::tuple<Args...> t)
{
	qDebug() << std::get<N>(t);
	print<N + 1>(t);
}
} // namespace TupleTool