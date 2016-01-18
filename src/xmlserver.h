/*----------------------------------------------------------------------------*/
/**
* @pkg xmlserver
*/
/**
* Server to handle XML request and return XML response.
*
* Long description of xmlserver.<br>
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016 18:50:44<br>
* @pkgdoc xmlserver
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef XMLSERVER_H_1452963044
#define XMLSERVER_H_1452963044
/*----------------------------------------------------------------------------*/
#include <QObject>
#include <QSettings>
#include "xmlrequest.h"
#include "xmlresponse.h"
/*----------------------------------------------------------------------------*/
class KsefDatabase;
class XmlServer : public QObject
{
  Q_OBJECT

public:
  XmlServer(QSettings *settings, QObject *parent = NULL);
  virtual ~XmlServer();
  void service(XmlRequest& request, XmlResponse& response);
protected:
  KsefDatabase *ksefDatabase;
};
/*----------------------------------------------------------------------------*/
#endif /*XMLSERVER_H_1452963044*/

