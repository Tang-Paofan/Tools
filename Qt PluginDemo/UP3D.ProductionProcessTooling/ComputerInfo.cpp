#include "ComputerInfo.h"

#include <Windows.h>

#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QSysInfo>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include "SystemInfo.h"
#include <UP3D.DeviceDiagUI/StateItemWidget.h>

class ComputerInfo::Impl
{
public:
	Impl(ComputerInfo* owner);
	
	void updateComputerInfo();

	bool isNetworkConnected();

	std::set<StateWidget::State> stateSet_;
public:
	QWidget* infoWidget_;

	StateItemWidget* sitComputerInfoTitle_;
	StateItemWidget* sitOperationSystemState_;
	StateItemWidget* sitCPUState_;
	StateItemWidget* sitCPUCoreState_;
	StateItemWidget* sitRunningMemoryState_;
	StateItemWidget* sitGraphicsCardState_;
	StateItemWidget* sitGraphicsCardMemoryState_;
	StateItemWidget* sitGraphicsCardDriverVersionState_;
	StateItemWidget* sitHardDiskState_;
	StateItemWidget* sitScreenState_;
	StateItemWidget* sitNetworkState_;
private:
	void initUI();

	ComputerInfo* owner_;
};


ComputerInfo::Impl::Impl(ComputerInfo* owner)
{
	owner_ = owner;
	
	initUI();

	updateComputerInfo();
}

void ComputerInfo::Impl::updateComputerInfo()
{
	// 操作系统
	sitOperationSystemState_->setCurrentValue(QString::fromStdString(WindowsSystemInfo::getOSVersion()));
	sitOperationSystemState_->setState(StateWidget::State::S_CORRECT);
	stateSet_.insert(sitOperationSystemState_->getState());

	// CPU型号
	sitCPUState_->setCurrentValue(QString::fromStdString(WindowsSystemInfo::getProcessorModel())); 
	sitCPUState_->setState(StateWidget::State::S_CORRECT);
	stateSet_.insert(sitCPUState_->getState());

	// CPU核心数
	//sitCPUCoreState_

	// 运行内存
	int ms = qRound(WindowsSystemInfo::getMemorySize(true).total_phys);
	sitRunningMemoryState_->setCurrentValue(QString::number(ms) + "G");
	sitRunningMemoryState_->setState(StateWidget::State::S_CORRECT);
	if (ms < 8)
	{
		sitRunningMemoryState_->setState(StateWidget::State::S_ERROR);
		sitRunningMemoryState_->setInfoValue(u8"运行内存不满足要求");
	}
	stateSet_.insert(sitRunningMemoryState_->getState());

	// 显卡型号
	WindowsSystemInfo::GraphicsCardInfo gcInfo = WindowsSystemInfo::getGraphicsCardInfo();
	QString displayDevices = "";
	displayDevices = QString::fromStdString(gcInfo.graphicsCardModel_[0]);
	sitGraphicsCardState_->setCurrentValue(displayDevices);
	sitGraphicsCardState_->setState(StateWidget::State::S_CORRECT);
	QString gcdv = QString::fromStdString(gcInfo.graphicsCardModel_[0]).toLower();
	if (!gcdv.contains("nvidia"))
	{
		sitGraphicsCardState_->setState(StateWidget::State::S_ERROR);
		sitGraphicsCardState_->setInfoValue(u8"不支持该显卡型号");
	}
	else if(!gcdv.contains("rtx"))
	{
		sitGraphicsCardState_->setState(StateWidget::State::S_WARNING);
		sitGraphicsCardState_->setInfoValue(u8"显卡性能较低");
	}
	stateSet_.insert(sitGraphicsCardState_->getState());

	// 显卡内存
	sitGraphicsCardMemoryState_->setCurrentValue(QString::number(gcInfo.igraphicsCardMenmery_[0]) + "G");
	sitGraphicsCardMemoryState_->setState(StateWidget::State::S_CORRECT);
	if (gcInfo.igraphicsCardMenmery_[0] < 4)
	{
		sitGraphicsCardMemoryState_->setState(StateWidget::State::S_ERROR);
		sitGraphicsCardMemoryState_->setInfoValue(u8"显存大小不满足要求");
	}
	stateSet_.insert(sitGraphicsCardMemoryState_->getState());

	// 显卡驱动版本
	sitGraphicsCardDriverVersionState_->setCurrentValue(QString::fromStdString(WindowsSystemInfo::getGraphicsCardDriverVersion()));
	sitGraphicsCardDriverVersionState_->setState(StateWidget::State::S_CORRECT);
	if (sitGraphicsCardState_->isCorrectOrWarning())
	{
		//31.0.15.2698
		QStringList version = QString::fromStdString(WindowsSystemInfo::getGraphicsCardDriverVersion()).split(".");
		bool res = true;
		if (version[2].toInt() < 15)
		{
			res = false;
		}
		if (version[3].toInt() < 3700)
		{
			res = false;
		}
		if (!res)
		{
			sitGraphicsCardDriverVersionState_->setState(StateWidget::State::S_WARNING);
			sitGraphicsCardDriverVersionState_->setInfoValue(u8"显卡驱动版本过低\n推荐版本：31.0.15.3700以上");
		}
	}
	else
	{
		sitGraphicsCardDriverVersionState_->setState(StateWidget::State::S_ERROR);
		sitGraphicsCardDriverVersionState_->setInfoValue(u8"不支持该显卡型号");
	}
	stateSet_.insert(sitGraphicsCardDriverVersionState_->getState());


	// 硬盘类型
	QProcess process;
	QStringList args;
	args << "-NoProfile" << "-Command" << "Get-PhysicalDisk";
	process.start("powershell.exe", args);
	if (!process.waitForStarted()) {
		//qDebug() << "Failed to start PowerShell process.";
	}
	if (!process.waitForFinished()) {
		//qDebug() << "Failed to finish executing PowerShell command.";
	}
	QByteArray output = process.readAllStandardOutput();
	QString outputStr = QString::fromLocal8Bit(output);

	struct PhysicalDiskInfo {
		QString number;
		QString friendlyName;
		QString serialNumber;
		QString mediaType;
		bool canPool;
		QString operationalStatus;
		QString healthStatus;
		QString usage;
		QString size;
	};
	QStringList lines = outputStr.split("\n", Qt::SkipEmptyParts);
	QList<PhysicalDiskInfo> diskInfos;
	for (const QString& line : lines.mid(2)) { // 跳过表头
		QStringList fields = line.trimmed().split(QRegExp("\\s+"));
		if (fields.size() >= 9) {
			PhysicalDiskInfo info;
			info.number = fields.at(0);
			info.friendlyName = fields.at(1);
			info.serialNumber = fields.at(2);
			info.mediaType = fields.at(3);
			info.canPool = (fields.at(4).toLower() == "true");
			info.operationalStatus = fields.at(5);
			info.healthStatus = fields.at(6);
			info.usage = fields.at(7);
			info.size = fields.at(8);
			diskInfos.append(info);
		}
	}
	QString diskType;
	if (outputStr.contains("HDD"))
	{
		diskType += "HDD; ";
	}
	if (outputStr.contains("SSD"))
	{
		diskType += "SSD; ";
	}
	sitHardDiskState_->setCurrentValue(diskType);
	sitHardDiskState_->setState(StateWidget::State::S_CORRECT);
	stateSet_.insert(sitHardDiskState_->getState());

	// 屏幕分辨率
	QString screenState;
	for (auto it : gcInfo.graphicsCardDisplay_)
	{
		if (!screenState.isEmpty())
		{
			screenState += "\n";
		}
		screenState += QString::fromStdString(it);
	}
	sitScreenState_->setCurrentValue(screenState);
	sitScreenState_->setState(StateWidget::State::S_CORRECT);
	stateSet_.insert(sitScreenState_->getState());

	// 网络状态
	//bool internetState = isNetworkConnected();
	//sitNetworkState_->setCurrentValue(internetState ? u8"正常" : u8"异常");
}

bool ComputerInfo::Impl::isNetworkConnected()
{
	QNetworkAccessManager manager;
	QNetworkRequest request;
	request.setUrl(QUrl("https:www.baidu.com"));
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	QNetworkReply* reply = manager.get(request);

	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec(); // 等待网络请求完成

	bool isConnected = (reply->error() == QNetworkReply::NoError);
	reply->deleteLater();
	return isConnected;
}

void ComputerInfo::Impl::initUI()
{
	infoWidget_ = new QWidget();
	infoWidget_->setObjectName("Graph");
	QVBoxLayout* mainlay = new QVBoxLayout(infoWidget_);

	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_TITLE_CURR_INFO_BTN;
		para.objName = "ComputerInfoTitle";
		para.widgetTitle = u8"检测项";
		para.currValue = u8"当前信息";
		para.infoValue = u8"检测结果";
		sitComputerInfoTitle_ = new StateItemWidget(para);
		sitComputerInfoTitle_->setStyleSheet(QString("QLabel{font-weight:bold;}"));
		sitComputerInfoTitle_->getButton()->setCheckable(true);
		sitComputerInfoTitle_->getButton()->setChecked(false);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "OperationSystemState";
		para.widgetTitle = u8"操作系统：";
		para.infoValue = u8"检测通过";
		sitOperationSystemState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "CPUState";
		para.widgetTitle = u8"CPU型号：";
		para.infoValue = u8"检测通过";
		sitCPUState_ = new StateItemWidget(para);
	}
	//{
	//	StateItemWidget::StateItemPara para;
	//	para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
	//	para.objName = "CPUCoreState";
	//	para.widgetTitle = u8"CPU核心数：";
	//  para.infoValue = u8"检测通过";
	//	sitCPUCoreState_ = new StateItemWidget(para);
	//}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "RunningMemoryState";
		para.widgetTitle = u8"内存：";
		para.infoValue = u8"检测通过";
		sitRunningMemoryState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "GraphicsCardState";
		para.widgetTitle = u8"显卡型号：";
		para.infoValue = u8"检测通过";
		sitGraphicsCardState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "sitGraphicsCardMemoryState_";
		para.widgetTitle = u8"显卡内存：";
		para.infoValue = u8"检测通过";
		sitGraphicsCardMemoryState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "CameraNum";
		para.widgetTitle = u8"显卡驱动版本：";
		para.infoValue = u8"检测通过";
		sitGraphicsCardDriverVersionState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "HardDiskState";
		para.widgetTitle = u8"硬盘";
		para.infoValue = u8"检测通过";
		sitHardDiskState_ = new StateItemWidget(para);
	}
	{
		StateItemWidget::StateItemPara para;
		para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
		para.objName = "ScreenState";
		para.widgetTitle = u8"屏幕";
		para.infoValue = u8"检测通过";
		sitScreenState_ = new StateItemWidget(para);
	}
	//{
	//	StateItemWidget::StateItemPara para;
	//	para.format = StateItemFormat::SIF_ICON_TITLE_CURR_INFO_BTN;
	//	para.objName = "NetworkState";
	//	para.widgetTitle = u8"网络";
	//  para.infoValue = u8"检测通过";
	//	sitNetworkState_ = new StateItemWidget(para);
	//}

	mainlay->addWidget(sitComputerInfoTitle_);
	mainlay->addWidget(sitOperationSystemState_);
	mainlay->addWidget(sitCPUState_);
	//mainlay->addWidget(sitCPUCoreState_);
	mainlay->addWidget(sitRunningMemoryState_);
	mainlay->addWidget(sitGraphicsCardState_);
	mainlay->addWidget(sitGraphicsCardMemoryState_);
	mainlay->addWidget(sitGraphicsCardDriverVersionState_);
	mainlay->addWidget(sitHardDiskState_);
	mainlay->addWidget(sitScreenState_);
	//mainlay->addWidget(sitNetworkState_);
	mainlay->addStretch();
}

ComputerInfo::ComputerInfo()
{
	impl_ = std::make_unique<Impl>(this);
}

QWidget* ComputerInfo::getInfoWidget()
{
	return impl_->infoWidget_;
}

bool ComputerInfo::getComputerOverallStatus()
{
	auto it = impl_->stateSet_.find(StateWidget::State::S_ERROR);
	if (it != impl_->stateSet_.end()) {
		return false;
	}
	it = impl_->stateSet_.find(StateWidget::State::S_WARNING);
	if (it != impl_->stateSet_.end()) {
		return false;
	}
	return true;
}

void ComputerInfo::slotUpdateComputerInfo()
{
	// TODO
}

