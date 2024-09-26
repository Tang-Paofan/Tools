#pragma once

#include <QtCore/QString>

class IProp
{
public:
	IProp() {}
	virtual ~IProp() {}

public:
	virtual void setGroup(QString v) {};
	virtual void resetGroup() {};

public:
	virtual void set(std::pair<QString, QString> v) {};
	virtual void set(std::pair<const char*, QString> v) {};
	virtual void set(std::pair<QString, const char*> v) {};
	virtual void set(std::pair<const char*, const char*> v) {};

	virtual void set(QString t, QString v) {};
	virtual void set(QString t, const char* v) {};
	virtual void set(QString t, int v) {};
	virtual void set(QString t, bool v) {};
	virtual void set(QString t, float v) {};

	virtual void set(const char* t, QString v) {};
	virtual void set(const char* t, const char* v) {};
	virtual void set(const char* t, int v) {};
	virtual void set(const char* t, bool v) {};
	virtual void set(const char* t, float v) {};

	virtual void set(std::pair<QString, int> v) {};
	virtual void set(std::pair<QString, bool> v) {};
	virtual void set(std::pair<QString, float> v) {};

	virtual void set(std::pair<const char*, int> v) {};
	virtual void set(std::pair<const char*, bool> v) {};
	virtual void set(std::pair<const char*, float> v) {};

	virtual QString get(const QString& key) const {
		return "";
	};

	virtual bool reset(QString v) {
		return false;
	};
	virtual bool reset(const char* v) {
		return false;
	};

	virtual bool exist(const QString& key) const {
		return false;
	};

public:
	virtual void printAll() {};

	virtual QString toString() {
		return "";
	};

public:
	virtual bool save(const QString& iniPath) {
		return false;
	};

	virtual bool load(const QString& iniPath) {
		return false;
	};
};
