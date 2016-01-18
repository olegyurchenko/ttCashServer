/*----------------------------------------------------------------------------*/
/**
* @pkg ksef_database
*/
/**
* KSEF storage class.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 18.01.2016 10:36:04<br>
* @pkgdoc ksef_database
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef KSEF_DATABASE_H_1453106164
#define KSEF_DATABASE_H_1453106164
/*----------------------------------------------------------------------------*/
#include <QSettings>
#include <QThread>

class QSqlQuery;
class KsefDocument;
class XmlResponse;
class KsefDatabase : public QThread
{
  Q_OBJECT

public:
  KsefDatabase(QSettings *settings, QObject *parent = NULL);
  virtual ~KsefDatabase();
  bool isError() const {return mIsError;}
  QString message() {return mMessage;}
  bool cashRegister(const KsefDocument& doc, XmlResponse &response);
  bool cashAddDoc(const KsefDocument& doc, XmlResponse &response);
protected:
//TODO: Check size
  qlonglong mMaxSize;
  QString mFileName;
  QString mPath;
  QString mConnectionName;
  bool mIsError;
  QString mMessage;
  bool error(const QSqlQuery &q);
  bool create();
  virtual void run();
  int cashId(const QString& serial);
};

/*----------------------------------------------------------------------------*/
#endif /*KSEF_DATABASE_H_1453106164*/

