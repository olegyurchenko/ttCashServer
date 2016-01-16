/*----------------------------------------------------------------------------*/
/**
* @pkg cash_requesthandler
*/
/**
* Class to handle requests from cash registers.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016  9:04:31<br>
* @pkgdoc cash_requesthandler
* @author oleg
* @version 0.01 
*/
/*----------------------------------------------------------------------------*/
#include "cash_requesthandler.h"
/*----------------------------------------------------------------------------*/
CashRequestHandler :: CashRequestHandler(QSettings* settings, QObject* parent)
  : HttpRequestHandler(parent)
{
}
/*----------------------------------------------------------------------------*/
void CashRequestHandler :: service(HttpRequest& request, HttpResponse& response)
{
}
/*----------------------------------------------------------------------------*/

