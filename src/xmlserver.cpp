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
#include "ksef_database.h"
#include "ksef_document.h"
/*----------------------------------------------------------------------------*/
XmlServer :: XmlServer(QSettings *settings, QObject *parent)
  : QObject(parent)
{
  ksefDatabase = new KsefDatabase(settings, this);
  if(ksefDatabase->isError())
  {
    qCritical("Error open KSEF database:%s", qPrintable(ksefDatabase->message()));
    mIsKsefError = true;
    mKsefMessage = QString("Error open KSEF database:%1").arg(ksefDatabase->message());
    delete ksefDatabase;
    ksefDatabase = NULL;
  }
  else
  {
    mIsKsefError = false;
  }
}
/*----------------------------------------------------------------------------*/
XmlServer :: ~XmlServer()
{
}
/*----------------------------------------------------------------------------*/
void XmlServer :: service(XmlRequest& request, XmlResponse& response)
{
  if(request.method() == "register")
  {
    methodRegister(request, response);
  }
  else
  if(request.method() == "upload")
  {
    methodUpload(request, response);
  }
  else
  if(request.method() == "download")
  {
    methodDownload(request, response);
  }
  else
  if(request.method() == "sync")
  {
    methodUpload(request, response);
    if(response.isOk())
      methodDownload(request, response);
  }
  else
  {
    response.setStatus(XmlResponse::InvalidMethod, QString("Invalid methos '%1'").arg(request.method()) );
  }
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodRegister(XmlRequest& request, XmlResponse& response)
{
  if(isKsefError() || ksefDatabase == NULL)
  {
    response.setStatus(XmlResponse::InternalError, ksefMessage());
    return;
  }

  KsefDocument doc;

  QDomElement data = request.data();
  QDomElement e = data.firstChildElement("RQ");
  e = e.firstChildElement("DAT");

  if(e.isNull() || !doc.assign(e))
  {
    response.setStatus(XmlResponse::InvalidQuery, "Invalid query");
  }



  if(!ksefDatabase->cashRegister(doc, response))
    response.setStatus(XmlResponse::InternalError, ksefDatabase->message());
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodUpload(XmlRequest& request, XmlResponse& response)
{
  if(isKsefError() || ksefDatabase == NULL)
  {
    response.setStatus(XmlResponse::InternalError, ksefMessage());
    return;
  }

  KsefDocument doc;

  QDomElement data = request.data();
  QDomElement e = data.firstChildElement("RQ");

  e = e.firstChildElement("DAT");

  while(!e.isNull() && !response.isError())
  {
    if(!doc.assign(e))
      response.setStatus(XmlResponse::InvalidQuery, "Invalid query");
    else
    if(!ksefDatabase->cashAddDoc(doc, response))
      response.setStatus(XmlResponse::InternalError, ksefDatabase->message());

    e = e.nextSiblingElement();
  }

}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodDownload(XmlRequest& request, XmlResponse& response)
{
}
/*----------------------------------------------------------------------------*/

