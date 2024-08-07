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
	/// @brief ��ȡSVGͼ��
	const QPixmap& pixmap();

	/// @brief ���ɫ�����޸�
	void setFill(const QString &targetName, const QString &color);

private:
	class Pimpl;
	std::unique_ptr<Pimpl> impl_;
};

#endif // !__UISVGICON_H__
