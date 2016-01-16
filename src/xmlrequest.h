/*----------------------------------------------------------------------------*/
/**
* @pkg xmlrequest
*/
/**
* Class for handle XML request.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016 15:52:28<br>
* @pkgdoc xmlrequest
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef XMLREQUEST_H_1452952348
#define XMLREQUEST_H_1452952348
/*----------------------------------------------------------------------------*/
#include <QByteArray>
#include <QDomDocument>
#include <QString>

class XmlResponse;
class QDomDocument;
class XmlRequest
{
protected:
  QDomDocument mDoc;
  QDomElement mDataNode;
  QString mMethod;
  QString mUserName;
  QString mPassword;
public:
  XmlRequest();
  virtual ~XmlRequest();
  bool parse(const QByteArray& src, XmlResponse& response);
  QDomElement& data() {return mDataNode;}
  QString method() {return mMethod;}
  void setAutentification(const QString& user, const QString& pass);
  QString userName() {return mUserName;}
  QString password() {return mPassword;}
};

/*----------------------------------------------------------------------------*/
#endif /*XMLREQUEST_H_1452952348*/

