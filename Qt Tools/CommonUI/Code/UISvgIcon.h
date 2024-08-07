#ifndef __UISVGICON_H__
#define __UISVGICON_H__

class QString;
class QPixmap;

class UISvgIcon
{
public:
	explicit UISvgIcon(const QString& svgPath);
	~UISvgIcon();

public:
	/// @brief 获取SVG图像
	const QPixmap& pixmap();

	/// @brief 填充色属性修改
	void setFill(const QString &targetName, const QString &color);

private:
	class Pimpl;
	std::unique_ptr<Pimpl> impl_;
};

#endif // !__UISVGICON_H__
