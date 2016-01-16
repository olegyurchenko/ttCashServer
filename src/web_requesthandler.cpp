/*----------------------------------------------------------------------------*/
/**
* @pkg web_requesthandler
*/
/**
* Class to handle requests from browsers.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016  9:04:23<br>
* @pkgdoc web_requesthandler
* @author oleg
* @version 0.01 
*/
/*----------------------------------------------------------------------------*/
#include "web_requesthandler.h"
/*----------------------------------------------------------------------------*/
WebRequestHandler :: WebRequestHandler(QSettings* settings, QObject* parent)
  : StaticFileController(settings, parent)
{
}
/*----------------------------------------------------------------------------*/
void WebRequestHandler :: service(HttpRequest& request, HttpResponse& response)
{
  //TODO
  StaticFileController::service(request, response);
}
/*----------------------------------------------------------------------------*/

