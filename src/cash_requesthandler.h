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
#ifndef CASH_REQUESTHANDLER_H_1452927871
#define CASH_REQUESTHANDLER_H_1452927871
/*----------------------------------------------------------------------------*/
#include "httprequesthandler.h"
#include "xmlserver.h"

class QSettings;
class CashRequestHandler : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(CashRequestHandler)
public:
    CashRequestHandler(QSettings* settings, QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
protected:
  XmlServer *mXmlServer;
};
/*----------------------------------------------------------------------------*/
#endif /*CASH_REQUESTHANDLER_H_1452927871*/

