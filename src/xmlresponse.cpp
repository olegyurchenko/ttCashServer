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
#include "xmlresponse.h"
#include <QDomDocument>
/*----------------------------------------------------------------------------*/
static int _unique;
static int unique()
{
  return ++_unique;
}
/*----------------------------------------------------------------------------*/
XmlResponse :: XmlResponse()
{
  mStatus = Ok;
  mDoc.appendChild(mDoc.createProcessingInstruction(
        "xml",
        "version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\""
        ));
  QDomElement root = mDoc.createElement("response");
  mDoc.appendChild(root);

  QDomElement e = mDoc.createElement("code");
  mCodeNode = mDoc.createTextNode(QString::number((int)mStatus));
  e.appendChild(mCodeNode);
  root.appendChild(e);

  e = mDoc.createElement("message");
  mMessageNode = mDoc.createTextNode(mMessage);
  e.appendChild(mMessageNode);
  root.appendChild(e);

  e = mDoc.createElement("request");
  mRequestNode = mDoc.createTextNode(QString::number(0));
  e.appendChild(mRequestNode);
  root.appendChild(e);

  e = mDoc.createElement("response");
  e.appendChild(mDoc.createTextNode(QString::number(unique())));
  root.appendChild(e);

  mDataNode = mDoc.createElement("data");
  root.appendChild(mDataNode);
}
/*----------------------------------------------------------------------------*/
XmlResponse :: ~XmlResponse()
{
}
/*----------------------------------------------------------------------------*/
void XmlResponse :: setStatus(Status s, const QString& m)
{
  mStatus = s;
  mMessage = m;
  mCodeNode.setData(QString::number((int) s));
  mMessageNode.setData(m);
}
/*----------------------------------------------------------------------------*/
void XmlResponse :: setMessage(const QString& m)
{
  mMessage = m;
  mMessageNode.setData(m);
}
/*----------------------------------------------------------------------------*/
QByteArray XmlResponse :: toByteArray()
{
  QByteArray s = mDoc.toByteArray(0);
  //qDebug("xml:%s", qPrintable(s));
  return s;
}
/*----------------------------------------------------------------------------*/
void XmlResponse :: setRequestId(int id)
{
  mRequestNode.setData(QString::number((int) id));
}
/*----------------------------------------------------------------------------*/

