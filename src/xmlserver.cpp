/*----------------------------------------------------------------------------*/
/**
* @pkg xmlserver
*/
/**
* Server to handle XML request and return XML response.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016 18:50:44<br>
* @pkgdoc xmlserver
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#include "xmlserver.h"
/*----------------------------------------------------------------------------*/
XmlServer :: XmlServer(QSettings *settings, QObject *parent)
  : QObject(parent)
{
}
/*----------------------------------------------------------------------------*/
XmlServer :: ~XmlServer()
{
}
/*----------------------------------------------------------------------------*/
void XmlServer :: service(XmlRequest& request, XmlResponse& response)
{
}
/*----------------------------------------------------------------------------*/

