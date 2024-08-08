#pragma once

#include <UP3D.ProductionProcessToolingInterface/DeviceDiagInterface.h>

#include <QtWidgets/QWidget>

class ExamplePlugin : public QObject, public DeviceDiagInterface
{
    Q_OBJECT
    Q_INTERFACES(DeviceDiagInterface)
	Q_PLUGIN_METADATA(IID Interface_iid)
public:
    /// 在插件构造时将详情页及节点信息添加到主窗口
    ExamplePlugin();
    ~ExamplePlugin();

    QWidget* getDetailInfoWidget() override;

    void asyncInit() override;

    void unload() override;
};
