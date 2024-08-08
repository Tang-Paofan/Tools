#pragma once

#include "DeviceDiagInterface_Global.h"

#include <QtCore/QString>
#include <QtWidgets/QWidget>

#define Interface_iid "device_diag_interface"

class _EXPORT DeviceDiagInterface
{
public:
	virtual ~DeviceDiagInterface() {}

	/// @brief ��Ϣ���飨��ʾ��ʼ��UI��
	virtual QWidget* getDetailInfoWidget() = 0;

	/// @brief �첽����
	virtual void asyncInit() = 0;

	/// @brief ж�ز��
	virtual void unload() = 0;
};

Q_DECLARE_INTERFACE(DeviceDiagInterface, Interface_iid)