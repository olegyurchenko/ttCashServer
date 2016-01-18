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
#include "ksef_document.h"
#include "xmlresponse.h"
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
bool KsefDatabase :: cashRegister(const KsefDocument& doc, XmlResponse& response)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  int id = cashId(doc.serial());
  if(isError())
    return false;

  if(id < 0)
  {
    q.prepare("insert into CR(ZN) values(?)");
    q.addBindValue(doc.serial());
    if(!q.exec())
    {
      error(q);
      return false;
    }
  }
  //!!!!!!!!!!!!!!!!!!!
  //TODO: Send not 0 DI !!!
  //!!!!!!!!!!!!!!!!!!!
  int maxDi = 0;
  q.prepare("select max(DI) from TAG_DAT where (1=1)\n"
            "and ZN=?\n"
            "and FN=?\n"
            "and TN=?\n"
            );
  q.addBindValue(doc.serial());
  q.addBindValue(doc.fiscal());
  q.addBindValue(doc.tax());
  if(!q.exec())
  {
    error(q);
    return false;
  }
  if(q.first())
    maxDi = q.value(0).toInt();

  QDomElement e = response.doc().createElement("DI");
  e.appendChild(response.doc().createTextNode(QString::number(maxDi)));
  response.data().appendChild(e);

  return true;
}
/*----------------------------------------------------------------------------*/
int KsefDatabase :: cashId(const QString& serial)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  q.prepare("select CR_ID from CR where ZN = ?");
  q.addBindValue(serial);
  if(!q.exec())
  {
    error(q);
    return -2;
  }

  if(!q.first())
    return -1;

  return q.value(0).toInt();
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: cashAddDoc(const KsefDocument& doc, XmlResponse &response)
{
  int id = cashId(doc.serial());
  if(isError())
    return false;

  if(id < 0 && cashRegister(doc, response))
    id = cashId(doc.serial());

  if(id < 0)
    return false;

  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  q.prepare("select DAT_ID from TAG_DAT where (1=1)\n"
            "and DI=?\n"
            "and ZN=?\n"
            "and FN=?\n"
            "and TN=?\n"
            "and TS=?\n"
            );
  q.addBindValue(doc.di());
  q.addBindValue(doc.serial());
  q.addBindValue(doc.fiscal());
  q.addBindValue(doc.tax());
  q.addBindValue(doc.time());
  if(!q.exec())
  {
    error(q);
    return false;
  }

  if(!q.first())
  {
    q.prepare("insert into TAG_DAT(CR_ID, DI,ZN,FN,TN,TS,EXT,XML) values(?,?,?,?,?,?,?,?)");
    q.addBindValue(id);
    q.addBindValue(doc.di());
    q.addBindValue(doc.serial());
    q.addBindValue(doc.fiscal());
    q.addBindValue(doc.tax());
    q.addBindValue(doc.time());
    q.addBindValue(0);
    q.addBindValue(doc.toXmlString());
  }

  if(!q.exec())
  {
    error(q);
    return false;
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

