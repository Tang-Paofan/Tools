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

	/// @brief ����QSS
	static auto loadQss(const QString& path) -> void;

	/// @brief ��ʼ������
	static auto initFont(const QString & path) -> bool;

	/// @brief ��ʼ��ȫ�ֱ���
	static auto initTextCodec() -> void;

	/// @brief �첽��ʼ��
	static auto asyncInit() -> void;

	static auto unloadPlugin() -> void;
};