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
#include <QFile>
#include <QDateTime>
#include <QDir>
/*----------------------------------------------------------------------------*/
CashRequestHandler :: CashRequestHandler(QSettings* settings, QObject* parent)
  : HttpRequestHandler(parent)
{
  mXmlServer = new XmlServer(settings, this);
  settings->endGroup();
  mLogName = settings->value("logging/xmlLog","xml.log").toString();
  mMaxFileSize = settings->value("logging/maxSize","104857600").toLongLong(); //100M
  if (QDir::isRelativePath(mLogName))
  {
    QFileInfo configFile(settings->fileName());
    mLogName = QFileInfo(configFile.absolutePath(),mLogName).absoluteFilePath();
  }
}
/*----------------------------------------------------------------------------*/
void CashRequestHandler :: service(HttpRequest& request, HttpResponse& response)
{
  XmlRequest xmlRequest;
  XmlResponse xmlResponse;

  //qDebug("request:%s", qPrintable(request.getBody()));
  saveXml(true, request.getBody());
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

  QByteArray content = xmlResponse.toByteArray();
  saveXml(false, content);
  response.setHeader("Content-Type", "text/xml; charset=utf8");
  //response.setHeader("Content-Length", content.length());
  response.write(content, true);
}
/*----------------------------------------------------------------------------*/
void CashRequestHandler :: saveXml(bool request, const QByteArray& content)
{
  if(!mMaxFileSize)
    return;
  QFile f(mLogName);
  if(f.size() >= mMaxFileSize)
    f.remove();
  if(f.open(QIODevice::WriteOnly  | QIODevice::Append | QIODevice::Text))
  {
    f.seek(f.size());
    QString h = QString("%1 %2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz")).arg(request ? ">>>" : "<<<");
    f.write(h.toLatin1());
    f.write(content);
    f.close();
  }
}
/*----------------------------------------------------------------------------*/

