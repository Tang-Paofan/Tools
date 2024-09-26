#pragma once

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QTextCodec>
#include <map>
#include <tuple>

#include "IProp.h"
#include "TupleTool.h"

template <typename... Args>
class AnyProp : public IProp
{
public:
	AnyProp(const std::tuple<Args...>& t)
		: mProps_(t)
	{
		resetGroup();
	}
	AnyProp() {};
	~AnyProp() = default;

	template <typename... Args>
	static AnyProp<Args...> make_properties(const std::tuple<Args...>& t)
	{
		return AnyProp<Args...>(t);
	}

public:
	template <typename Target>
	void set(Target t)
	{
		std::get<Target>(mProps_) = t;
	}

	template <typename Target, typename... Others>
	void set(Target t, Others... others)
	{
		set<Target>(t);
		set(others...);
	}

	template <typename Target>
	Target get()
	{
		return std::get<Target>(mProps_);
	}

	template <typename Target>
	void _exchange(AnyProp& tOther)
	{
		auto t = get<Target>();
		set(tOther.get<Target>());
		tOther.set(t);
	}

	// 遍历到最后时, 什么也不做
	template <std::size_t Index = 0, typename... TargetArgs>
	typename std::enable_if<Index == sizeof...(TargetArgs), void>::type _exchange(AnyProp& tOther)
	{
		// 最后一个参数什么也不做
	}

	/// @brief 遍历交换参数
	template <std::size_t Index = 0, typename... TargetArgs>
	typename std::enable_if < Index<sizeof...(TargetArgs), void>::type _exchange(AnyProp& tOther)
	{
		// 执行指定类型的交换
		_exchange<typename std::tuple_element<Index, std::tuple<TargetArgs...>>::type>(tOther);
		// 执行下一个类型的交换
		_exchange<Index + 1, TargetArgs...>(tOther);
	}

	/// @brief 执行指定类型的交换
	/// @tparam ...TargetArgs 要交换的类型
	/// @param tOther 要交换的对象
	template <typename... TargetArgs>
	void exchange(std::shared_ptr<AnyProp> tOther)
	{
		// 从头开始交换
		_exchange<0, TargetArgs...>(*tOther);
	}

	template <typename... TargetArgs>
	void exchange(AnyProp& tOther)
	{
		_exchange<0, TargetArgs...>(tOther);
	}

	/// @brief 执行全部类型的交换
	template <>
	void exchange(AnyProp& tOther)
	{
		// 从头开始交换
		_exchange<0, Args...>(tOther);
	}

	template <typename Target>
	bool match(Target t)
	{
		return t == std::get<Target>(mProps_);
	}

	template <typename Target>
	bool matchBoth(Target t)
	{
		return match<Target>(t);
	}

	template <typename Target, typename... Others>
	bool matchBoth(Target t, Others... others)
	{
		return match<Target>(t) && matchBoth(others...);
	}

	template <typename Target>
	bool matchAny(Target t)
	{
		return match<Target>(t);
	}

	template <typename Target, typename... Others>
	bool matchAny(Target t, Others... others)
	{
		return match<Target>(t) || matchAny(others...);
	}

public:
	void setOther(AnyProp other)
	{
		this->mProps_ = other.mProps_;
		this->mStrProps_ = other.mStrProps_;
	}
	void setOther(std::shared_ptr<AnyProp> other)
	{
		this->mProps_ = other->mProps_;
		this->mStrProps_ = other->mStrProps_;
	}

public:
	void setGroup(QString v) override
	{
		mStrPropGroup_ = v;
	}

	void resetGroup() override
	{
		mStrPropGroup_ = "StringData";
	}

public:
	void set(std::pair<QString, QString> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = v.second;
	}
	void set(QString t, QString v) override
	{
		mStrProps_[mStrPropGroup_][t] = v;
	}
	void set(std::pair<const char*, QString> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = v.second;
	}
	void set(const char* t, QString v) override
	{
		mStrProps_[mStrPropGroup_][t] = v;
	}
	void set(std::pair<QString, const char*> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = v.second;
	}
	void set(QString t, const char* v) override
	{
		mStrProps_[mStrPropGroup_][t] = v;
	}
	void set(std::pair<const char*, const char*> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = v.second;
	}
	void set(const char* t, const char* v) override
	{
		mStrProps_[mStrPropGroup_][t] = v;
	}
	void set(std::pair<QString, int> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	void set(QString t, int v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(std::pair<const char*, int> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	void set(const char* t, int v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(std::pair<QString, bool> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	void set(QString t, bool v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(std::pair<const char*, bool> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	void set(const char* t, bool v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(QString t, float v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(const char* t, float v) override
	{
		mStrProps_[mStrPropGroup_][t] = QString::number(v);
	}
	void set(std::pair<QString, float> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	void set(std::pair<const char*, float> v) override
	{
		mStrProps_[mStrPropGroup_][v.first] = QString::number(v.second);
	}
	bool reset(QString v) override
	{
		auto& tMap = mStrProps_[mStrPropGroup_];
		if (tMap.find(v) == tMap.end()) {
			return false;
		}
		tMap.erase(v);
		return true;
	}
	bool reset(const char* v) override
	{
		auto& tMap = mStrProps_[mStrPropGroup_];
		if (tMap.find(v) == tMap.end()) {
			return false;
		}
		tMap.erase(v);
		return true;
	}
	QString get(const QString& key) const override
	{
		if (!exist(key))
		{
			return "";
		}
		return mStrProps_.at(mStrPropGroup_).at(key);
	}

	bool exist(const QString& key) const override
	{
		if (mStrProps_.empty())
			return false;
		auto		tIter = mStrProps_.find(mStrPropGroup_);
		if (tIter == mStrProps_.end())
		{
			return false;
		}
		return tIter->second.find(key) != tIter->second.cend();
	}

public:
	void printAll() override
	{
		TupleTool::print(mProps_);
		for (const auto& [tGroup, tMap] : mStrProps_)
		{
			for (const auto& [tName, tValue] : tMap)
			{
				qDebug() << QString("%1.%2=%3").arg(tGroup).arg(tName).arg(tValue);
			}
		}
	}

	QString toString() override
	{
		QStringList temp;
		for (const auto& [tGroup, tMap] : mStrProps_)
		{
			for (const auto& [tName, tValue] : tMap)
			{
				temp << QString("%1.%2=%3").arg(tGroup).arg(tName).arg(tValue);
			}
		}
		return temp.join("\n");
	}

public:
	bool save(const QString& iniPath) override
	{
		QSettings ini(iniPath, QSettings::IniFormat);
		ini.setIniCodec(QTextCodec::codecForName("utf-8"));

		for (const auto& [tGroup, tMap] : mStrProps_)
		{
			ini.beginGroup(tGroup);
			for (const auto& [tName, tValue] : tMap)
			{
				ini.setValue(tName, tValue);
			}
			ini.endGroup();
		}
		return true;
	}

	bool load(const QString& iniPath) override
	{
		QSettings ini(iniPath, QSettings::IniFormat);
		ini.setIniCodec(QTextCodec::codecForName("utf-8"));

		for (const auto& tGroup : ini.childGroups())
		{
			ini.beginGroup(tGroup);
			for (const QString& key : ini.childKeys())
			{
				// 当key已经存在时，emplace不会修改该key的值
				mStrProps_[tGroup][key] = ini.value(key).toString();
			}
			ini.endGroup();
		}
		return true;
	};

public:
	AnyProp& operator=(const AnyProp& other)
	{
		setOther(other);
		return *this;
	}

public:
	std::tuple<Args...> mProps_;

public:
	QString mStrPropGroup_;

	std::map<QString, std::map<QString, QString>> mStrProps_;
};
