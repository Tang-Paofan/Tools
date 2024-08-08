#pragma once

#include <UP3D.ProductionProcessToolingInterface/DeviceDiagInterface.h>

#include <QtWidgets/QWidget>

class ExamplePlugin : public QObject, public DeviceDiagInterface
{
    Q_OBJECT
    Q_INTERFACES(DeviceDiagInterface)
	Q_PLUGIN_METADATA(IID Interface_iid)
public:
    /// �ڲ������ʱ������ҳ���ڵ���Ϣ��ӵ�������
    ExamplePlugin();
    ~ExamplePlugin();

    QWidget* getDetailInfoWidget() override;

    void asyncInit() override;

    void unload() override;
};
