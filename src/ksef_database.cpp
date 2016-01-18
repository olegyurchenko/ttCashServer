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
#include "ksef_database.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
/*----------------------------------------------------------------------------*/
#define CONNECTION_NAME "KSEF"
static int m_unique = 0;
/*----------------------------------------------------------------------------*/
KsefDatabase :: KsefDatabase(QSettings *settings, QObject *parent)
  : QThread(parent)
{
  mIsError = false;
  mMaxSize=settings->value("maxSize", 4294967296LL).toLongLong();
  mPath = settings->value("storage",".").toString();
  if (QDir::isRelativePath(mPath))
  {
    QFileInfo configFile(settings->fileName());
    mPath=QFileInfo(configFile.absolutePath(),mPath).absoluteFilePath();
  }

  QDir dir(mPath);
  if(!dir.exists())
  {
    dir.mkpath(mPath);
  }

  mConnectionName = QString("%1_%2").arg(CONNECTION_NAME).arg(++m_unique);

  mFileName = dir.filePath("ksef.sqlite");
  qDebug("KsefDatabase: fileName=%s, maxSize=%lld",qPrintable(mFileName), mMaxSize);


  QFileInfo fi(mFileName);
  bool exists = fi.exists();

  QSqlDatabase m_database = QSqlDatabase::addDatabase("QSQLITE", mConnectionName);
  m_database.setDatabaseName(mFileName);
  if(!m_database.open())
  {
    mIsError = true;
    mMessage = m_database.lastError().text();
    return;
  }

  if(!exists)
  {
    create();
  }
  else
  {
    QSqlQuery q(m_database);

    if(!q.exec("select count(*) from CR"))
      create();
  }

  if(!isError())
    start();
}
/*----------------------------------------------------------------------------*/
KsefDatabase :: ~KsefDatabase()
{
  QSqlDatabase::removeDatabase(mConnectionName);
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: create()
{

  QFile f(":/sql/create_ksef_stor.sql");
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    mMessage = "Cannot open resource file";
    mIsError = true;
    return false;
  }

  QTextStream in(&f);
  QString line;

  QStringList sqlSource;
  //Read && remove comments #
  while (!in.atEnd())
  {
    line = in.readLine();
    QStringList lst = line.split('#', QString::KeepEmptyParts);
    if(!lst.isEmpty() && !lst.at(0).isEmpty())
      sqlSource.append(lst.at(0));
  }

  QString sql = sqlSource.join(QChar(' '));
  sql.remove('\r');

  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  QStringList lst = sql.split(';', QString::SkipEmptyParts);
  QString s;
  QRegExp r("[A-Za-z]");

  foreach(s, lst)
  {
    if(s.indexOf(r) == -1)
      continue;

    if(!q.exec(s))
    {
      error(q);
      return false;
    }
  }
  return true;
}
/*----------------------------------------------------------------------------*/
void KsefDatabase :: run()
{
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: error(const QSqlQuery &q)
{
  if(q.lastError().isValid())
  {
    mMessage = QString("%1:\n'%2'").arg(q.lastError().text()).arg(q.lastQuery());
    mIsError = true;
    return true;
  }
  mIsError = false;
  return false;
}
/*----------------------------------------------------------------------------*/

