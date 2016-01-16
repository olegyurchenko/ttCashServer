/*----------------------------------------------------------------------------*/
/**
* @pkg xmlresponse
*/
/**
* Class for handle XML response.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016 15:52:37<br>
* @pkgdoc xmlresponse
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef XMLRESPONSE_H_1452952357
#define XMLRESPONSE_H_1452952357
/*----------------------------------------------------------------------------*/
#include <QByteArray>
#include <QString>
#include <QDomDocument>

class QDomDocument;
class XmlResponse
{
public:
  typedef enum
  {
    Ok = 0,
    ParserError = 1,
    InternalError = 100,
    UnknownError = -1
  } Status;
protected:
  QDomDocument mDoc;
  Status mStatus;
  QString mMessage;
  QDomElement mDataNode;
  QDomText mCodeNode;
  QDomText mMessageNode;
  QDomText mRequestNode;
public:
  XmlResponse();
  virtual ~XmlResponse();
  QByteArray toByteArray();

  QDomDocument& doc() {return mDoc;}
  QDomElement& data() {return mDataNode;}

  Status status() {return mStatus;}
  void setStatus(Status s, const QString& m = QString());
  QString message() {return mMessage;}
  void setMessage(const QString& m);
  bool isOk() {return mStatus == Ok;}
  void setRequestId(int id);
};

/*----------------------------------------------------------------------------*/
#endif /*XMLRESPONSE_H_1452952357*/

