#pragma once
#ifndef __DEVICEDIAGUI_GLOBAL_H__
#define __DEVICEDIAGUI_GLOBAL_H__

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>

#include <memory>

#ifdef DEVICEDIAGUI_EXPOTS
#define DEVICEDIAGUI_EXPOT Q_DECL_EXPORT
#else
#define DEVICEDIAGUI_EXPOT Q_DECL_IMPORT
#endif

#endif // __COMMONUTILS_GLOBAL_H__
