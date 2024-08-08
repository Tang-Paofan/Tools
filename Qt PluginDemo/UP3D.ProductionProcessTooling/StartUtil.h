#pragma once

#include <UP3D.ProductionProcessToolingUI/DeviceDiagnostics.h>

#include <QtWidgets/QSystemTrayIcon>
#include <QDebug>

class StartUtil: public QObject
{
	Q_OBJECT
public:
	static auto loadPlugin(DeviceDiagnostics* wgt) -> void;

	static auto initResources() -> void;

	/// @brief 加载QSS
	static auto loadQss(const QString& path) -> void;

	/// @brief 初始化字体
	static auto initFont(const QString & path) -> bool;

	/// @brief 初始化全局编码
	static auto initTextCodec() -> void;

	/// @brief 异步初始化
	static auto asyncInit() -> void;

	static auto unloadPlugin() -> void;
};