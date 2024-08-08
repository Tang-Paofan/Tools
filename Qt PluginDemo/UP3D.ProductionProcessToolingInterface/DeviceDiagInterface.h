#pragma once

#include "DeviceDiagInterface_Global.h"

#include <QtCore/QString>
#include <QtWidgets/QWidget>

#define Interface_iid "device_diag_interface"

class _EXPORT DeviceDiagInterface
{
public:
	virtual ~DeviceDiagInterface() {}

	/// @brief 信息详情（显示初始化UI）
	virtual QWidget* getDetailInfoWidget() = 0;

	/// @brief 异步加载
	virtual void asyncInit() = 0;

	/// @brief 卸载插件
	virtual void unload() = 0;
};

Q_DECLARE_INTERFACE(DeviceDiagInterface, Interface_iid)