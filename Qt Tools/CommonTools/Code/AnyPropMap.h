#pragma once

#include <map>
#include <memory>

template <typename BindObject, typename BindProperties>
class AnyPropMap
{
public:
	using BindPropertiesPtr = std::shared_ptr<BindProperties>;

public:
	AnyPropMap()
		: mPropMap_() {};
	~AnyPropMap() {};

public:
	/// @brief 绑定对象和属性指针
	BindPropertiesPtr bind(BindObject obj)
	{
		auto it = mPropMap_.find(obj);
		if (it != mPropMap_.end())
		{
			return it->second;
		}
		BindPropertiesPtr temp = std::make_shared<BindProperties>();
		mPropMap_.emplace(obj, temp);
		return temp;
	};
	/// @brief 绑定对象和属性指针
	BindPropertiesPtr bind(BindObject obj, BindPropertiesPtr other)
	{
		auto prop = bind(obj);
		prop->setOther(other);
		return prop;
	};
	/// @brief 绑定对象和属性
	template <typename... Args>
	BindPropertiesPtr bind(BindObject obj, Args&&... args)
	{
		auto prop = bind(obj);
		prop->set(args...);
		return prop;
	};
	/// @brief 从属性中匹配对应对象
	template <typename... Args>
	BindObject findBoth(Args&&... args)
	{
		for (auto& it : mPropMap_)
		{
			if (it.second->matchBoth(args...))
			{
				return it.first;
			}
		}
		return BindObject();
	};
	template <typename... Args>
	std::vector<BindObject> findBothAll(Args&&... args)
	{
		std::vector<BindObject> tResult;
		for (auto& it : mPropMap_)
		{
			if (it.second->matchBoth(args...))
			{
				tResult.push_back(it.first);
			}
		}
		return tResult;
	};
	template <typename... Args>
	BindObject findAny(Args&&... args)
	{
		for (auto& it : mPropMap_)
		{
			if (it.second->matchAny(args...))
			{
				return it.first;
			}
		}
		return BindObject();
	};
	template <typename... Args>
	std::vector<BindObject> findAnyAll(Args&&... args)
	{
		std::vector<BindObject> tResult;
		for (auto& it : mPropMap_)
		{
			if (it.second->matchAny(args...))
			{
				tResult.push_back(it.first);
			}
		}
		return tResult;
	};

	/// @brief 检查是否完全具备给定属性
	/// @note 大部分枚举值都是唯一的, 如果输入同类型枚举, 绝大部分情况一定会返回false
	template <typename... Args>
	bool matchBoth(BindObject obj, Args&&... args)
	{
		return bind(obj)->matchBoth(args...);
	};
	/// @brief 检查是否存在任意符合条件的属性, 可以是相同类型的属性
	template <typename... Args>
	bool matchAny(BindObject obj, Args&&... args)
	{
		return bind(obj)->matchAny(args...);
	};

	template <typename PropertyType>
	PropertyType value(BindObject obj)
	{
		return bind(obj)->get<PropertyType>();
	};

	QString value(BindObject obj, const char* key)
	{
		return bind(obj)->get(key);
	};

	bool exist(BindObject obj)
	{
		auto it = mPropMap_.find(obj);
		if (it != mPropMap_.end())
		{
			return true;
		}
		return false;
	};
	bool remove(BindObject obj)
	{
		auto it = mPropMap_.find(obj);
		if (it != mPropMap_.end())
		{
			mPropMap_.erase(it);
			return true;
		}
		return false;
	};
	bool clear()
	{
		mPropMap_.clear();
		return true;
	};

protected:
	std::map<BindObject, BindPropertiesPtr> mPropMap_;
};