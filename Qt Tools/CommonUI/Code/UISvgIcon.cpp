#include "UISvgIcon.h"

#include <QtCore/QFile>
#include <QtCore/QPointer>

#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

#include <QtSvg/QSvgRenderer>

#include <QtXml/QDomDocument>

#include <QApplication>

class UISvgIcon::Pimpl
{
public:
	Pimpl(const QString &svgPath);
	~Pimpl();

public:
	void setSVGProperty(QDomElement &&elem, const QString &tagName, const QString &property, const QString &value);
	QString SVGProperty(QDomElement &&elem, const QString &tagName, const QString &property);

public:
	QScopedPointer<QDomDocument> mSvgDoc_;
	QScopedPointer<QSvgRenderer> pSvgRender_;
	QScopedPointer<QPixmap> pPixmap_;

	QString mSvgPath_ = "";
};

UISvgIcon::Pimpl::Pimpl(const QString &svgPath)
{
	mSvgPath_ = svgPath;

	mSvgDoc_.reset(new QDomDocument());
	QFile tFile(mSvgPath_);
	tFile.open(QIODevice::ReadOnly | QFile::Text);
	mSvgDoc_->setContent(&tFile);
	tFile.close();

	pSvgRender_.reset(new QSvgRenderer(mSvgDoc_->toByteArray()));

	// TODO dpi
	auto tDpi = QApplication::primaryScreen()->logicalDotsPerInch();

	pPixmap_.reset(new QPixmap(SVGProperty(mSvgDoc_->documentElement(), "svg", "width").toInt() * tDpi,
	                           SVGProperty(mSvgDoc_->documentElement(), "svg", "height").toInt() * tDpi));

	pPixmap_->fill(Qt::transparent);
	QPainter tPainter(pPixmap_.get());
	pSvgRender_->render(&tPainter);
}

UISvgIcon::Pimpl::~Pimpl()
{
}

void UISvgIcon::Pimpl::setSVGProperty(QDomElement &&elem, const QString &tagName, const QString &property, const QString &value)
{
	if (elem.tagName().compare(tagName) == 0)
	{
		elem.setAttribute(property, value);
	}

	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (elem.childNodes().at(i).isElement())
		{
			setSVGProperty(elem.childNodes().at(i).toElement(), tagName, property, value);
		}
	}
}

QString UISvgIcon::Pimpl::SVGProperty(QDomElement &&elem, const QString &tagName, const QString &property)
{
	if (elem.tagName().compare(tagName) == 0)
	{
		return elem.attribute(property);
	}

	QString tPropertyStr;
	for (int i = 0; i < elem.childNodes().count(); i++)
	{
		if (elem.childNodes().at(i).isElement())
		{
			tPropertyStr = SVGProperty(elem.childNodes().at(i).toElement(), tagName, property);
			if (!tPropertyStr.isEmpty())
			{
				return tPropertyStr;
			}
		}
	}

	return tPropertyStr;
}

UISvgIcon::UISvgIcon(const QString &svgPath)
    : impl_(std::make_unique<Pimpl>(svgPath))
{
}

UISvgIcon::~UISvgIcon()
{
}

const QPixmap& UISvgIcon::pixmap()
{
	return *impl_->pPixmap_;
}

void UISvgIcon::setFill(const QString &targetName, const QString &color)
{
	impl_->setSVGProperty(impl_->mSvgDoc_->documentElement(), targetName, "fill", color);

	QPainter tPainter(impl_->pPixmap_.get());
	impl_->pSvgRender_->load(impl_->mSvgDoc_->toByteArray());
	impl_->pSvgRender_->render(&tPainter);
}