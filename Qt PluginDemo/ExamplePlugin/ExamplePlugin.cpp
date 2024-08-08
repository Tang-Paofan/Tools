#include "ExamplePlugin.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include <QtCore/QDebug>
ExamplePlugin::ExamplePlugin()
{
}

ExamplePlugin::~ExamplePlugin()
{

}

void ExamplePlugin::asyncInit()
{
}

void ExamplePlugin::unload()
{
}

QWidget* ExamplePlugin::getDetailInfoWidget()
{
    QWidget* infoWidget_ = new QWidget();
    infoWidget_->setFixedSize(500, 100);
    infoWidget_->setStyleSheet(QString("background-color: red;"));
    QHBoxLayout * lay = new QHBoxLayout();
    infoWidget_->setLayout(lay);

    QLabel* label = new QLabel("UP3DDDExamplePlugin");
    lay->addWidget(label);
    qDebug() << "getMainInfoWidget 1";
    return infoWidget_;
}
