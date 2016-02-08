/*----------------------------------------------------------------------------*/
/**
* @pkg database
*/
/**
* Module to collect data for cash server;
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 03.02.2016  9:02:39<br>
* @pkgdoc database
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef DATABASE_H_1454482959
#define DATABASE_H_1454482959
/*----------------------------------------------------------------------------*/
#include "ksef_database.h"
#include "store_database.h"
#include "users.h"
/*----------------------------------------------------------------------------*/
class Database : public QObject
{
  Q_OBJECT
protected:
  KsefDatabase *mKsefDatabase;
  StoreDatabase *mStoreDatabase;
  UserStrorage *mUserStorage;

  bool mIsKsefError;
  QString mKsefMessage;

  bool mIsStoreError;
  QString mStoreMessage;

public:
  Database(QSettings *settings, QObject *parent = NULL);
  ~Database();

  KsefDatabase *ksefDatabase() { return mKsefDatabase;}
  StoreDatabase *storeDatabase() { return mStoreDatabase; }
  UserStrorage *userStorage() { return mUserStorage; }

  bool isKsefError() const {return mIsKsefError;}
  QString ksefMessage() {return mKsefMessage;}
  bool isStoreError() const {return mIsStoreError;}
  QString storeMessage() {return mStoreMessage;}

};
/*----------------------------------------------------------------------------*/
extern Database *database;
/*----------------------------------------------------------------------------*/
#endif /*DATABASE_H_1454482959*/

