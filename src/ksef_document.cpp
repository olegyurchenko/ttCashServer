/*----------------------------------------------------------------------------*/
/**
* @pkg ksef_document
*/
/**
* KsefDocument - class represebted KSEF DAT tag.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 18.01.2016 15:43:30<br>
* @pkgdoc ksef_document
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#include "ksef_document.h"
/*----------------------------------------------------------------------------*/
KsefDocument :: KsefDocument()
{
  mTime = QDateTime::currentDateTime();
}
/*----------------------------------------------------------------------------*/
KsefDocument :: ~KsefDocument()
{
}
/*----------------------------------------------------------------------------*/
bool KsefDocument :: assign(QDomElement& e)
{
  mDoc.clear();
  mDoc.appendChild(mDoc.importNode(e, true));
  if(e.tagName() != "DAT")
    return false;
  mSerial = e.attribute("ZN");
  mTax = e.attribute("TN");
  mFiscal = e.attribute("FN");
  mDi = e.attribute("DI").toInt();

  QDomElement ts = e.firstChildElement("TS");
  if(!ts.isNull())
  {
    //YYYYMMDDhhmmss
    QString t = ts.text();
    if(t.length() == 14)
    {
      int Y; int M; int D;
      int h, m, s;
      Y = t.left(4).toInt();
      M = t.mid(4, 2).toInt();
      D = t.mid(6, 2).toInt();
      h = t.mid(8, 2).toInt();
      m = t.mid(10, 2).toInt();
      s = t.mid(12, 2).toInt();

      mTime.setDate(QDate(Y, M, D));
      mTime.setTime(QTime(h, m, s, 0));
    }
  }

  if(mSerial.isEmpty())
    return false;
  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDocument :: parse(const QString& xml)
{
}
/*----------------------------------------------------------------------------*/
QString KsefDocument :: toXmlString() const
{
  return mDoc.toString(0).replace(QChar('\n'), " ");
}
/*----------------------------------------------------------------------------*/
