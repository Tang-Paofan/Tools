#pragma once

#include "DeviceDiagUI_Global.h"

#include <QtWidgets/QWidget>

class QStandardItem;

class DEVICEDIAGUI_EXPOT DeviceDiagnostics : public QWidget
{
	Q_OBJECT
public:
	/// @brief ��ӽڵ�
	/// @param nodeName �ڵ�����
	/// @param nodeDetailWidget �ýڵ�����ҳ
	/// @param parentNodeName ��������
	void addNode(const QString& nodeName, QWidget* nodeDetailWidget, const QString& parentNodeName);

	/// չ�����нڵ�
	void expandAll();

	/// @brief Ĭ��ѡ�е�һ��
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