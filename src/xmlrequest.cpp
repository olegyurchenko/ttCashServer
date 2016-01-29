/*----------------------------------------------------------------------------*/
/**
* @pkg xmlrequest
*/
/**
* Class for handle XML request.
*
* started 16.01.2016 15:52:28<br>
* @pkgdoc xmlrequest
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#include "xmlrequest.h"
#include "xmlresponse.h"
/*----------------------------------------------------------------------------*/
XmlRequest :: XmlRequest()
{
}
/*----------------------------------------------------------------------------*/
XmlRequest :: ~XmlRequest()
{
}
/*----------------------------------------------------------------------------*/
bool XmlRequest :: parse(const QByteArray& src, XmlResponse& response)
{
  QString errMsg;
  int errorLine, errorColumn;
  if(!mDoc.setContent(src, &errMsg, &errorLine, &errorColumn))
  {
    QString msg = QString("XML ParseError %2:%3: %4").arg(errorLine).arg(errorColumn).arg(errMsg);
    response.setStatus(XmlResponse::ParserError, msg);
    qCritical("%s:`%s`", qPrintable(msg), qPrintable(src));
    return false;
  }

  QDomElement root = mDoc.firstChildElement("request");
  QDomElement e = root.firstChildElement();
  while(!e.isNull())
  {
    if(e.tagName() == "method")
      mMethod = e.text();
    if(e.tagName() == "id")
      response.setRequestId(e.text().toInt());
    if(e.tagName() == "data")
      mDataNode = e;
    e = e.nextSiblingElement();
  }


  return true;
}
/*----------------------------------------------------------------------------*/
void XmlRequest :: setAutentification(const QString& user, const QString& pass)
{
  mUserName = user;
  mPassword = pass;
}
/*----------------------------------------------------------------------------*/
