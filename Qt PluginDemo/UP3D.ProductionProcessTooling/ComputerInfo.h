#pragma once

#include <QtWidgets/QWidget>
#include <QStandardItem>



class ComputerInfo : public QObject
{
	Q_OBJECT
public:
	ComputerInfo();

	QWidget* getInfoWidget();

	bool getComputerOverallStatus();

public slots:

	void slotUpdateComputerInfo();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};
