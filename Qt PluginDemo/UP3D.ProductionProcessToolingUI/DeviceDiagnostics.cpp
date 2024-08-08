#include "DeviceDiagnostics.h"
#include "SvgIcon.h"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtGui/QMouseEvent>
#include <QtGui/QScreen>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QRect>

#include <windows.h>
#include <windowsx.h>

const QString gCloseBtnSvgPath = ":/Resource/Icon/CloseBtn.svg";
const QString gComputerInfoBtnSvgPath = ":/Resource/Icon/CamputerInfoIcon.svg";
const QString gDeviceInfoBtnSvgPath = ":/Resource/Icon/DeviceInfoIcon.svg";
const QString gCommonSvgPath = ":/Resource/Icon/CamputerInfoIcon.svg";

const int WidgetWidth = 1024;
const int WidgetHeight = 1024;

class DeviceDiagnostics::PImpl
{
public:
	PImpl(DeviceDiagnostics* _owner);
	~PImpl() = default;

	void initUI();

	void initTreeRoot();

public:
	/// @brief view
	QTreeView* nodeListWidget_;
	/// @brief model
	QStandardItemModel* nodeModel_;
	/// @brief detail of widget
	QHBoxLayout* detailedInfoWidgetLayout_;

public:
	/// @brief item model, detail if widget
	std::map<QStandardItem*, QWidget*> detailedInfoWidgetMap_;
	/// @brief plugin name, item model
	std::map<QString, QStandardItem*> treeNodeInfoMap_;

public:
	QToolButton* closeButton_ = nullptr;

	QPoint dragPosition_;
private:
	DeviceDiagnostics* owner_ = nullptr;
};

DeviceDiagnostics::PImpl::PImpl(DeviceDiagnostics* _owner)
    : owner_(_owner)
{
	initUI();
	initTreeRoot();
}

void DeviceDiagnostics::PImpl::initUI()
{
	// 顶级窗口
	// 顶级窗口无法设置阴影
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(10, 10, 10, 10);

	// 次顶级窗口
	QVBoxLayout* subMainLayout = new QVBoxLayout;
	subMainLayout->setContentsMargins(20, 5, 20, 20);

	// 头部
	QHBoxLayout* headerLayout = new QHBoxLayout;

	QLabel* titleLabel = new QLabel(u8"UP3D 口扫工装系统");

	SvgIcon* tCloseIcon = new SvgIcon(gCloseBtnSvgPath);
	closeButton_ = new QToolButton;
	closeButton_->setIcon(tCloseIcon->pixmap());
	closeButton_->setCursor(QCursor(Qt::PointingHandCursor));

	headerLayout->addWidget(titleLabel);
	headerLayout->addWidget(closeButton_);

	QWidget* headerWidget = new QWidget;
	headerWidget->setObjectName("HeaderWidget");
	headerWidget->setLayout(headerLayout);


	// 主体部分
	QSplitter* splitter = new QSplitter(Qt::Horizontal);

	nodeListWidget_ = new QTreeView;
	nodeListWidget_->setFocusPolicy(Qt::NoFocus);
	nodeListWidget_->setHeaderHidden(true);
	nodeListWidget_->viewport()->installEventFilter(owner_);
	nodeListWidget_->setSelectionMode(QTreeView::SingleSelection);
	nodeListWidget_->setIndentation(0);

	QWidget* detailedInfoWidget = new QWidget;
	detailedInfoWidget->setObjectName("DetailedInfoWidget");
	detailedInfoWidgetLayout_ = new QHBoxLayout(detailedInfoWidget);
	detailedInfoWidgetLayout_->setContentsMargins(5, 0, 0, 0);

	splitter->addWidget(nodeListWidget_);
	splitter->addWidget(detailedInfoWidget);
	splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	subMainLayout->addWidget(headerWidget);
	subMainLayout->addWidget(splitter);

	QWidget* subMainWidget = new QWidget;
	subMainWidget->setLayout(subMainLayout);
	subMainWidget->setObjectName("SubWidget");

	// 次顶级窗口阴影
	QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
	shadowEffect->setColor(QColor("#F2F2F2"));
	shadowEffect->setBlurRadius(10);
	shadowEffect->setOffset(0, 0);
	subMainWidget->setGraphicsEffect(shadowEffect);

	mainLayout->addWidget(subMainWidget);

	owner_->setLayout(mainLayout);
}

void DeviceDiagnostics::PImpl::initTreeRoot()
{
	nodeModel_ = new QStandardItemModel(nodeListWidget_);
	nodeListWidget_->setModel(nodeModel_);
	nodeListWidget_->setEditTriggers(QAbstractItemView::NoEditTriggers);

	auto tCalibrationItem = new QStandardItem(u8"标定工装");
	auto tCommonSvg = new SvgIcon(gCommonSvgPath);
	tCalibrationItem->setIcon(tCommonSvg->pixmap());
	treeNodeInfoMap_.emplace(u8"标定工装", tCalibrationItem);

	nodeModel_->appendRow(tCalibrationItem);
	nodeListWidget_->expandAll();
}

void DeviceDiagnostics::slotCloseWidget()
{
	close();
}

void DeviceDiagnostics::mousePressEvent(QMouseEvent* event)
{
	if (event->button() & Qt::LeftButton) {
		impl_->dragPosition_ = event->pos();
		event->accept();
	}
}

void DeviceDiagnostics::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		move(event->globalPos() - impl_->dragPosition_);
		event->accept();
	}
}

bool DeviceDiagnostics::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* tMouseEvent = static_cast<QMouseEvent*>(event);
		impl_->dragPosition_ = tMouseEvent->pos();
		// TODO 将QTreeView坐标系转为窗口坐标系
		impl_->dragPosition_.ry() += 70;
		impl_->dragPosition_.rx() += 15;
		tMouseEvent->accept();
		return true;
	}
	else if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent* tMouseEvent = static_cast<QMouseEvent*>(event);
		if (tMouseEvent->buttons() & Qt::LeftButton)
		{
			move(tMouseEvent->globalPos() - impl_->dragPosition_);
			tMouseEvent->accept();
			return true;
		}
	}

	return QWidget::eventFilter(obj, event);
}

DeviceDiagnostics::DeviceDiagnostics(QWidget *parent)
    : QWidget(parent),
	impl_(std::make_unique<PImpl>(this))
{
	auto tScreenRect = QApplication::primaryScreen()->geometry();
	setGeometry((tScreenRect.width() - WidgetWidth) / 2, (tScreenRect.height() - WidgetHeight) / 2, WidgetWidth, WidgetHeight);
	setObjectName("Widget");
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	QObject::connect(impl_->closeButton_, &QToolButton::clicked, this, &DeviceDiagnostics::slotCloseWidget);
	QObject::connect(impl_->nodeListWidget_, &QTreeView::clicked, this, &DeviceDiagnostics::slotTreeViewClicked);
}

DeviceDiagnostics::~DeviceDiagnostics()
{
}

void DeviceDiagnostics::addNode(const QString& nodeName, QWidget* nodeDetailWidget, const QString& parentNodeName)
{
	// NOTE 特殊处理,root和node一致时,直接与根关联
	auto tParent = impl_->treeNodeInfoMap_.find(parentNodeName);
	if (tParent != impl_->treeNodeInfoMap_.end() && tParent->first == nodeName)
	{
		impl_->detailedInfoWidgetMap_[impl_->treeNodeInfoMap_[nodeName]] = nodeDetailWidget;
		impl_->detailedInfoWidgetLayout_->addWidget(nodeDetailWidget);
		nodeDetailWidget->setVisible(false);
		return;
	}

	QStandardItem* item = new QStandardItem(nodeName);

	if (!nodeName.isEmpty())
	{
		SvgIcon* icon = new SvgIcon(":/Resource/Icon/EmptyItemIcon.svg");
		item->setIcon(QIcon(icon->pixmap()));
	}

	impl_->detailedInfoWidgetMap_[item] = nodeDetailWidget;
	impl_->detailedInfoWidgetLayout_->addWidget(nodeDetailWidget);
	nodeDetailWidget->setVisible(false);

	auto parent = impl_->treeNodeInfoMap_.find(parentNodeName);
	if (parent != impl_->treeNodeInfoMap_.end())
	{
		parent->second->appendRow(item);
	}
	else
	{
		impl_->nodeModel_->appendRow(item);
	}

	impl_->treeNodeInfoMap_.emplace(nodeName, item);
}

void DeviceDiagnostics::expandAll()
{
	impl_->nodeListWidget_->expandAll();
}

void DeviceDiagnostics::defaultSelectItem()
{
	auto tIndex = impl_->treeNodeInfoMap_.cbegin()->second->index();
	impl_->nodeListWidget_->setCurrentIndex(tIndex);
	slotTreeViewClicked(tIndex);
}

void DeviceDiagnostics::slotTreeViewClicked(const QModelIndex &index)
{
	for (const auto& it : impl_->detailedInfoWidgetMap_)
	{
		if (it.first->text() == index.data().toString())
			it.second->setVisible(true);
		else
			it.second->setVisible(false);
	}
}