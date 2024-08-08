#include "SvgIcon.h"

#include <QtSvg/QSvgRenderer>
#include <QtCore/QFile>
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include <QGuiApplication>
#include <QScreen>

SvgIcon::SvgIcon(const QString& _svgPath)
{
	QFile tFile(_svgPath);
	tFile.open(QIODevice::ReadOnly);
	QByteArray tData = tFile.readAll();
	tFile.close();

	svgDoc_.setContent(tData);

	auto tDpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();

	QSvgRenderer* tSvgRender = new QSvgRenderer(_svgPath);
	pixmap_ = new QPixmap(getSVGProperty("svg", "width").toInt() * tDpi, getSVGProperty("svg", "height").toInt() * tDpi);
	pixmap_->fill(Qt::transparent);
	QPainter tPainter(pixmap_);
	tSvgRender->render(&tPainter);
}

void SvgIcon::setSVGProperty(const QDomElement& elem, const QString& _tagName, const QString& _property, const QString& _value)
{
	if (elem.tagName().compare(_tagName) == 0)
	{
		const_cast<QDomElement*>(&elem)->setAttribute(_property, _value);
		for (int i = 0; i < elem.childNodes().count(); i++)
		{
			if (!elem.childNodes().at(i).isElement())
			{
				continue;
			}
			setSVGProperty(elem.childNodes().at(i).toElement(), _tagName, _property, _value);
		}
	}
}

QString SvgIcon::getSVGProperty(const QString& _tagName, const QString& _property)
{
	const auto& elem = svgDoc_.documentElement();
	if (elem.tagName().compare(_tagName) == 0)
	{
		return const_cast<QDomElement*>(&elem)->attribute(_property);
	}

	return QString::number(-1);
}

const QDomDocument SvgIcon::getDomDocument()
{
	return svgDoc_;
}

const QPixmap SvgIcon::pixmap()
{
	return *pixmap_;
}
