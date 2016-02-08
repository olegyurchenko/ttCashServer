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
#include "database.h"
#include "ksef_document.h"
/*----------------------------------------------------------------------------*/
XmlServer :: XmlServer(QSettings *settings, QObject *parent)
  : QObject(parent)
{
  Q_UNUSED(settings);
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
  if(request.method() == "update")
  {
    methodUpdate(request, response);
  }
  else
  if(request.method() == "ksef_get")
  {
    methodKsefGet(request, response);
  }
  else
  {
    response.setStatus(XmlResponse::InvalidMethod, QString("Invalid method '%1'").arg(request.method()) );
  }
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodRegister(XmlRequest& request, XmlResponse& response)
{
  if(database->isKsefError() || database->ksefDatabase() == NULL)
  {
    response.setStatus(XmlResponse::InternalError, database->ksefMessage());
    return;
  }

  User user = database->userStorage()->user(request.userName(), request.password());
  if(!user.ksef.write)
  {
    response.setStatus(XmlResponse::accessDenied, QString("Access denied for user '%1'").arg(user.name));
    return;
  }

  KsefDocument doc;

  QDomElement data = request.data();
  QDomElement e = data.firstChildElement("RQ");
  e = e.firstChildElement("DAT");

  if(e.isNull() || !doc.assign(e))
  {
    response.setStatus(XmlResponse::InvalidQuery, "Invalid query");
    return;
  }



  if(!database->ksefDatabase()->cashRegister(doc, response))
    response.setStatus(XmlResponse::InternalError, database->ksefDatabase()->message());
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodUpload(XmlRequest& request, XmlResponse& response)
{
  if(database->isKsefError() || database->ksefDatabase() == NULL)
  {
    response.setStatus(XmlResponse::InternalError, database->ksefMessage());
    return;
  }

  User user = database->userStorage()->user(request.userName(), request.password());
  if(!user.ksef.write)
  {
    response.setStatus(XmlResponse::accessDenied, QString("Access denied for user '%1'").arg(user.name));
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
    if(!database->ksefDatabase()->cashAddDoc(doc, response))
      response.setStatus(XmlResponse::InternalError, database->ksefDatabase()->message());

    e = e.nextSiblingElement("DAT");
  }

}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodDownload(XmlRequest& request, XmlResponse& response)
{
  if(database->isStoreError() || database->storeDatabase() == NULL)
  {
    response.setStatus(XmlResponse::InternalError, database->storeMessage());
    return;
  }

  User user = database->userStorage()->user(request.userName(), request.password());
  if(!user.store.read)
  {
    response.setStatus(XmlResponse::accessDenied, QString("Access denied for user '%1'").arg(user.name));
    return;
  }

  if(!database->storeDatabase()->download(request, response))
    response.setStatus(XmlResponse::InternalError, database->storeDatabase()->message());
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodUpdate(XmlRequest& request, XmlResponse& response)
{
  if(database->isStoreError() || database->storeDatabase() == NULL)
  {
    response.setStatus(XmlResponse::InternalError, database->storeMessage());
    return;
  }

  User user = database->userStorage()->user(request.userName(), request.password());
  if(!user.store.write)
  {
    response.setStatus(XmlResponse::accessDenied, QString("Access denied for user '%1'").arg(user.name));
    return;
  }
  if(!database->storeDatabase()->update(request, response))
    response.setStatus(XmlResponse::InternalError, database->storeDatabase()->message());
}
/*----------------------------------------------------------------------------*/
void XmlServer :: methodKsefGet(XmlRequest& request, XmlResponse& response)
{
  if(database->isKsefError() || database->ksefDatabase() == NULL)
  {
    response.setStatus(XmlResponse::InternalError, database->ksefMessage());
    return;
  }

  User user = database->userStorage()->user(request.userName(), request.password());
  if(!user.ksef.read)
  {
    response.setStatus(XmlResponse::accessDenied, QString("Access denied for user '%1'").arg(user.name));
    return;
  }

  if(!database->ksefDatabase()->query(request, response))
    response.setStatus(XmlResponse::InternalError, database->ksefDatabase()->message());
}
/*----------------------------------------------------------------------------*/
