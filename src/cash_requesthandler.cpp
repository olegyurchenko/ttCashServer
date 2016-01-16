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
#include "xmlrequest.h"
#include "xmlresponse.h"
/*----------------------------------------------------------------------------*/
CashRequestHandler :: CashRequestHandler(QSettings* settings, QObject* parent)
  : HttpRequestHandler(parent)
{
  mXmlServer = new XmlServer(settings, this);
}
/*----------------------------------------------------------------------------*/
void CashRequestHandler :: service(HttpRequest& request, HttpResponse& response)
{
  XmlRequest xmlRequest;
  XmlResponse xmlResponse;


  if(xmlRequest.parse(request.getBody(), xmlResponse))
  {
    QByteArray auth = request.getHeader("Authorization");
    if(!auth.isEmpty())
    {
      QList<QByteArray> lst = auth.split(' ');
      int i = 0;//lst.indexOf("basic");
      if(i >= 0 && i < lst.size() - 1)
      {
        //qDebug("Auth: '%s' '%s", qPrintable(lst.at(i + 1)), qPrintable(QByteArray::fromBase64(lst.at(i + 1))));
        lst = QByteArray::fromBase64(lst.at(i + 1)).split(':');
        if(lst.size() >= 2)
          xmlRequest.setAutentification(lst.at(0), lst.at(1));

      }
    }
    mXmlServer->service(xmlRequest, xmlResponse);
  }

  response.setHeader("Content-Type", "text/xml; charset=utf8");
  response.write(xmlResponse.toByteArray(), true);
}
/*----------------------------------------------------------------------------*/

