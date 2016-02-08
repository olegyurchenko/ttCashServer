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
#include "database.h"
/*----------------------------------------------------------------------------*/
Database *database = NULL;

Database :: Database(QSettings *settings, QObject *parent)
  : QObject(parent)
{

  if(!settings->group().isEmpty())
    settings->endGroup();
  settings->beginGroup("server");

  mKsefDatabase = new KsefDatabase(settings, this);
  if(mKsefDatabase->isError())
  {
    qCritical("Error open KSEF database:%s", qPrintable(mKsefDatabase->message()));
    mIsKsefError = true;
    mKsefMessage = QString("Error open KSEF database:%1").arg(mKsefDatabase->message());
    delete mKsefDatabase;
    mKsefDatabase = NULL;
  }
  else
  {
    mIsKsefError = false;
  }

  mStoreDatabase = new StoreDatabase(settings, this);
  if(mStoreDatabase->isError())
  {
    mIsStoreError = true;
    mStoreMessage = QString("Error open Store database:%1").arg(mStoreDatabase->message());
    delete mStoreDatabase;
    mStoreDatabase = NULL;
  }
  else
  {
    mIsStoreError = false;
  }


  if(!settings->group().isEmpty())
    settings->endGroup();
  settings->beginGroup("access");
  mUserStorage = new UserStrorage(settings, this);

  database = this;
}
/*----------------------------------------------------------------------------*/
Database :: ~Database()
{
  database = NULL;
}
/*----------------------------------------------------------------------------*/
