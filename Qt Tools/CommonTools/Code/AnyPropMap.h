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
	/// @brief �󶨶��������ָ��
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
	/// @brief �󶨶��������ָ��
	BindPropertiesPtr bind(BindObject obj, BindPropertiesPtr other)
	{
		auto prop = bind(obj);
		prop->setOther(other);
		return prop;
	};
	/// @brief �󶨶��������
	template <typename... Args>
	BindPropertiesPtr bind(BindObject obj, Args&&... args)
	{
		auto prop = bind(obj);
		prop->set(args...);
		return prop;
	};
	/// @brief ��������ƥ���Ӧ����
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

	/// @brief ����Ƿ���ȫ�߱���������
	/// @note �󲿷�ö��ֵ����Ψһ��, �������ͬ����ö��, ���󲿷����һ���᷵��false
	template <typename... Args>
	bool matchBoth(BindObject obj, Args&&... args)
	{
		return bind(obj)->matchBoth(args...);
	};
	/// @brief ����Ƿ���������������������, ��������ͬ���͵�����
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