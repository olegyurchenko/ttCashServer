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
#ifndef KSEF_DOCUMENT_H_1453124610
#define KSEF_DOCUMENT_H_1453124610
/*----------------------------------------------------------------------------*/
#include <QDomDocument>
#include <QDateTime>
class KsefDocument
{
protected:
  QString mSerial;
  QString mTax;
  QString mFiscal;
  QDateTime mTime;
  QDomDocument mDoc;
  int mDi;
public:
  KsefDocument();
  virtual ~KsefDocument();

  bool assign(QDomElement& element);
  bool parse(const QString&);

  QString toXmlString() const;

  QString serial() const {return mSerial;}
  QString tax() const {return mTax;}
  QString fiscal() const {return mFiscal;}
  QDateTime time() const {return mTime;}
  int di() const {return mDi;}
};
/*----------------------------------------------------------------------------*/
#endif /*KSEF_DOCUMENT_H_1453124610*/

