#pragma once

#include "enum.hpp"

#include <bitset>

template <typename Enum>
constexpr Enum getMaxLoop(Enum target, size_t index)
{
	if (index >= Enum::_size())
	{
		return target;
	}
	if (Enum::_values()[index] > target)
	{
		return getMaxLoop<Enum>(Enum::_values()[index], index + 1);
	}
	return getMaxLoop<Enum>(target, index + 1);
}

template <typename Enum>
constexpr Enum getMax()
{
	return getMaxLoop<Enum>(Enum::_values()[0], 1);
}

template <typename Enum>
using EnumBitset = std::bitset<getMax<Enum>()._to_integral() + 1>;