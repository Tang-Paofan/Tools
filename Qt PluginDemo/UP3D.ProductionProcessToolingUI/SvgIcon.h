#ifndef __SVGICON_H__
#define __SVGICON_H__

#include "DeviceDiagUI_Global.h"
#include <QDomDocument>

class QString;
class QPixmap;

class DEVICEDIAGUI_EXPOT SvgIcon
{
public:
	explicit SvgIcon(const QString& _svgPath);

	void setSVGProperty(const QDomElement& elem, const QString& _tagName,const QString& _property, const QString& _value);
	QString getSVGProperty(const QString& _tagName, const QString& _property);

	/// @brief ��ȡxml�ı�
	const QDomDocument getDomDocument();
	/// @brief ��ȡSVGͼ��
	const QPixmap pixmap();
private:
	QDomDocument svgDoc_ = QDomDocument();
	QPixmap* pixmap_ = nullptr;
};

#endif // !__SVGICON_H__
