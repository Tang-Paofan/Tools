#pragma once

#include "DeviceDiagUI_Global.h"

#include <QtWidgets/QWidget>

class QStandardItem;

class DEVICEDIAGUI_EXPOT DeviceDiagnostics : public QWidget
{
	Q_OBJECT
public:
	/// @brief 添加节点
	/// @param nodeName 节点名称
	/// @param nodeDetailWidget 该节点详情页
	/// @param parentNodeName 父节名称
	void addNode(const QString& nodeName, QWidget* nodeDetailWidget, const QString& parentNodeName);

	/// 展开所有节点
	void expandAll();

	/// @brief 默认选中第一行
	void defaultSelectItem();

private slots:
	void slotTreeViewClicked(const QModelIndex &index);
	void slotCloseWidget();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);

public:
	DeviceDiagnostics(QWidget* parent = nullptr);
	~DeviceDiagnostics();

private:
	class PImpl;
	std::unique_ptr<PImpl> impl_;
};