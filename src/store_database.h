/*----------------------------------------------------------------------------*/
/**
* @pkg store_database
*/
/**
* Store database to download and upload items to/from cash registers.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 23.01.2016 16:42:47<br>
* @pkgdoc store_database
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef STORE_DATABASE_H_1453560167
#define STORE_DATABASE_H_1453560167
#include <QSettings>
#include <QObject>
/*----------------------------------------------------------------------------*/
class QSqlQuery;
class XmlRequest;
class XmlResponse;
class StoreDatabase : public QObject
{
  Q_OBJECT

public:
  StoreDatabase(QSettings *settings, QObject *parent = NULL);
  virtual ~StoreDatabase();
  bool isError() const {return mIsError;}
  QString message() {return mMessage;}

  bool download(XmlRequest &request, XmlResponse& response);
  bool update(XmlRequest &request, XmlResponse& response);

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
  bool downloadTable(QSqlQuery &q, XmlResponse& response, const QString& tableName, int *recordCount);
  bool updateItem(const QString& tableName, const QString& primaryKey,  const QVariantHash &v);
};

/*----------------------------------------------------------------------------*/
#endif /*STORE_DATABASE_H_1453560167*/

