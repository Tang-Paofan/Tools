#pragma once

#include <set>

#include <QList>

template <typename T>
class EnumList
{
public:
	EnumList(){};
	~EnumList(){};

public:
	QStringList toStringList() const
	{
		QStringList ret;
		for (const auto& item : mData_)
		{
			ret << item._to_string();
		}
		return ret;
	}

	QStringList toNumberStringList() const
	{
		QStringList ret;
		for (const auto& item : mData_)
		{
			ret << QString::number(item._to_integral());
		}
		return ret;
	}

	std::set<T> toStdSet() const
	{
		std::set<T> ret;
		for (const auto& item : mData_)
		{
			ret.insert(item);
		}
		return ret;
	}

	bool contains(const QString& data) const
	{
		for (auto& item : mData_)
		{
			if (item._to_string() == data)
			{
				return true;
			}
		}
		return false;
	};

	bool contains(const T& data) const
	{
		for (auto& item : mData_)
		{
			if (item == data)
			{
				return true;
			}
		}
		return false;
	};

	int size() const
	{
		return mData_.size();
	}

public:
	EnumList& operator=(const QStringList& other)
	{
		mData_.clear();
		for (const auto& item : other)
		{
			auto opt = T::_from_string_nothrow(item.toStdString().c_str());
			if (!opt)
			{
				opt = T::_from_integral_nothrow(item.toInt());
			}
			if (!opt)
			{
				opt = T::_from_index_nothrow(item.toInt());
			}
			if (opt)
			{
				mData_.push_back(opt.value());
			}
		}
		return *this;
	}

	EnumList& operator=(const std::vector<T>& other)
	{
		mData_.clear();
		for (const auto& item : other)
		{
			mData_ << (item);
		}
		return *this;
	}

	EnumList& operator<<(const T& other)
	{
		mData_ << other;
		return *this;
	}

	T& operator[](const int& index)
	{
		if (index < 0 || index >= mData_.size())
		{
			throw std::out_of_range("Index out of range");
		}
		return mData_[index];
	}

private:
	QList<T> mData_;
};
