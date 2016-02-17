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
#include "store_database.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include "xmlrequest.h"
#include "xmlresponse.h"
/*----------------------------------------------------------------------------*/
#define CONNECTION_NAME "STORE"
static int m_unique = 0;
/*----------------------------------------------------------------------------*/
StoreDatabase :: StoreDatabase(QSettings *settings, QObject *parent)
  :QObject(parent)
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

  mFileName = dir.filePath("store.db");
  qDebug("StoreDatabase: fileName=%s, maxSize=%lld",qPrintable(mFileName), mMaxSize);


  QSqlDatabase m_database = QSqlDatabase::addDatabase("QSQLITE", mConnectionName);
  m_database.setDatabaseName(mFileName);
  if(!m_database.open())
  {
    mIsError = true;
    mMessage = m_database.lastError().text();
    return;
  }

  create();
}
/*----------------------------------------------------------------------------*/
StoreDatabase :: ~StoreDatabase()
{
  QSqlDatabase::removeDatabase(mConnectionName);
}
/*----------------------------------------------------------------------------*/
QSqlQuery StoreDatabase :: sqlQuery()
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  return q;
}
/*----------------------------------------------------------------------------*/
bool StoreDatabase :: error(const QSqlQuery &q)
{
  if(q.lastError().isValid())
  {
    QVariantMap m = q.boundValues();

    mMessage = QString("%1:\n'%2'").arg(q.lastError().text()).arg(q.lastQuery());

    if(!m.isEmpty())
      mMessage += "\nBound values:\n";

    for(QVariantMap::Iterator it = m.begin(); it != m.end(); ++it)
      mMessage += QString("%1 = %2").arg(it.key()).arg(it.value().toString());

    qCritical("StoreDatabase:%s", qPrintable(mMessage));
    mIsError = true;
    return true;
  }
  mIsError = false;
  return false;
}
/*----------------------------------------------------------------------------*/
bool StoreDatabase :: create()
{

  QFile f(":/sql/create_cash_stor.sql");
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    mMessage = "Cannot open resource file";
    mIsError = true;
    return false;
  }

  QTextStream in(&f);
  QString line;

  QStringList sqlSource;
  //Read && remove comments # && --
  while (!in.atEnd())
  {
    line = in.readLine();
    QStringList lst = line.split('#', QString::KeepEmptyParts);
    if(!lst.isEmpty() && !lst.at(0).isEmpty())
    {
      const QString &s = lst.at(0);
      sqlSource.append(s.mid(0, s.indexOf("--")));
    }
  }

  QString sql = sqlSource.join(QChar(' '));
  //sql.remove('\r');

  QSqlQuery q(QSqlDatabase::database(mConnectionName));

  QStringList lst = sql.split("+++", QString::SkipEmptyParts);
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
bool StoreDatabase :: download(XmlRequest &request, XmlResponse& response)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));

  QVariantMap newRev = revisionsGet();
  QDomElement rev = response.doc().createElement("REV");
  for(QVariantMap::Iterator it = newRev.begin(); it != newRev.end(); ++it)
  {
    QDomElement e = response.doc().createElement(it.key());
    e.setAttribute("REV", it.value().toInt());
    rev.appendChild(e);
  }
  response.data().appendChild(rev);


  QDomElement getTag = request.data().firstChildElement("RQ").firstChildElement("GET");
  int recordCount = getTag.attribute("RC", "100").toInt();
  if(!getTag.isNull())
  {
    QDomElement e = getTag.firstChildElement();
    while(!e.isNull() && recordCount > 0)
    {
      int revision = e.attribute("REV", "0").toInt();
      if(e.tagName() == "PLU")
      {
        q.prepare("select REV,DEL,C,NM,GRP,DPT,TX,TX2,\"DEC\" from PLU where REV > ? order by REV, C LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "PLU", &recordCount))
          return false;
      }
      if(e.tagName() == "PRC")
      {
        q.prepare("select REV,DEL,C,PRC from PRC where REV > ? order by REV, C LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "PRC", &recordCount))
          return false;
      }
      if(e.tagName() == "GRP")
      {
        q.prepare("select REV,DEL,C,NM from GRP where REV > ? order by REV, C LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "GRP", &recordCount))
          return false;
      }
      if(e.tagName() == "DPT")
      {
        q.prepare("select REV,DEL,C,NM from DPT where REV > ? order by REV, C LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "DPT", &recordCount))
          return false;
      }
      if(e.tagName() == "BAR")
      {
        q.prepare("select REV,DEL,CD,C from BAR where REV > ? order by REV, ROWID LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "BAR", &recordCount))
          return false;
      }
      if(e.tagName() == "EMT")
      {
        q.prepare("select REV,DEL,C,F,NM from EMT where REV > ? order by REV, C LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "EMT", &recordCount))
          return false;
      }
      if(e.tagName() == "CNT")
      {
        q.prepare("select REV,DEL,PAN,NM,EMT,PF,F,PLU,BAL,PRC from CNT where REV > ? order by REV, ROWID LIMIT ?");
        q.addBindValue(revision);
        q.addBindValue(recordCount);
        if(!downloadTable(q, response, "CNT", &recordCount))
          return false;
      }
      e = e.nextSiblingElement();
    }
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool StoreDatabase :: downloadTable(QSqlQuery &q, XmlResponse& response, const QString& tableName, int *recordCount)
{
  //REV field always first !!!
  //DEL field always second !!!

  if(!q.exec())
  {
    error(q);
    return false;
  }

  while(q.next() && *recordCount > 0)
  {
    QSqlRecord rec = q.record();
    int count = rec.count();
    int rev = rec.field(0).value().toInt();
    int del = rec.field(1).value().toInt();

    QDomElement item = response.doc().createElement("ITEM");
    item.setAttribute("REV", rev);
    QDomElement e = response.doc().createElement(tableName);
    if(del)
      e.setAttribute("DEL", "1");
    for(int i = 2; i < count; i++)
    {
      QDomElement ee = response.doc().createElement(rec.fieldName(i));
      ee.appendChild(response.doc().createTextNode(rec.field(i).value().toString()));
      e.appendChild(ee);
    }

    item.appendChild(e);
    response.data().appendChild(item);
    -- *recordCount;
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool StoreDatabase :: update(XmlRequest &request, XmlResponse &response)
{
  QDomElement set = request.data().firstChildElement("RQ").firstChildElement("SET");

  QVariantMap oldRev = revisionsGet();
  QDomElement e = set.firstChildElement();
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  bool ok = q.exec("BEGIN TRANSACTION");
  while(!e.isNull() && ok)
  {
    bool del = e.attribute("DEL", "0").toInt();

    if(e.tagName() == "GRP")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "C")
          v["C"] = ee.text().toInt();
        if(ee.tagName() == "NM" && !del)
          v["NM"] = ee.text();
        ee = ee.nextSiblingElement();
      }
      ok = updateItem("GRP", "C", v);
    }
    if(e.tagName() == "DPT")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "C")
          v["C"] = ee.text().toInt();
        if(ee.tagName() == "NM" && !del)
          v["NM"] = ee.text();
        ee = ee.nextSiblingElement();
      }
      ok = updateItem("DPT", "C", v);
    }
    if(e.tagName() == "PLU")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "C")
          v["C"] = ee.text().toInt();
        if(ee.tagName() == "NM" && !del)
          v["NM"] = ee.text();
        if(ee.tagName() == "GRP" && !del)
          v["GRP"] = ee.text().toInt();
        if(ee.tagName() == "DPT" && !del)
          v["DPT"] = ee.text().toInt();
        if(ee.tagName() == "TX" && !del)
          v["TX"] = ee.text().toInt();
        if(ee.tagName() == "TX2" && !del)
          v["TX2"] = ee.text().toInt();
        if(ee.tagName() == "DEC" && !del)
          v["DEC"] = ee.text().toInt();

        ee = ee.nextSiblingElement();
      }
      ok = updateItem("PLU", "C", v);
    }
    if(e.tagName() == "PRC")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();

      while(!ee.isNull())
      {
        if(ee.tagName() == "C")
          v["C"] = ee.text().toInt();
        if(ee.tagName() == "PRC" && !del)
          v["PRC"] = ee.text().toDouble();

        ee = ee.nextSiblingElement();
      }
      ok = updateItem("PRC", "C", v);
    }
    if(e.tagName() == "BAR")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "CD")
          v["CD"] = ee.text();
        if(ee.tagName() == "C" && !del)
          v["C"] = ee.text().toInt();
        ee = ee.nextSiblingElement();
      }
      ok = updateItem("BAR", "CD", v);
    }
    if(e.tagName() == "EMT")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "C")
          v["C"] = ee.text().toInt();
        if(ee.tagName() == "F" && !del)
          v["F"] = ee.text().toInt();
        if(ee.tagName() == "NM" && !del)
          v["NM"] = ee.text();
        ee = ee.nextSiblingElement();
      }
      ok = updateItem("EMT", "C", v);
    }
    if(e.tagName() == "CNT")
    {
      QDomElement ee = e.firstChildElement();
      QVariantMap v;
      v["DEL"] = e.attribute("DEL", "0").toInt();
      while(!ee.isNull())
      {
        if(ee.tagName() == "PAN")
          v["PAN"] = ee.text();
        if(ee.tagName() == "NM" && !del)
          v["NM"] = ee.text();
        if(ee.tagName() == "EMT" && !del)
          v["EMT"] = ee.text().toInt();
        if(ee.tagName() == "PF" && !del)
          v["PF"] = ee.text().toInt();
        if(ee.tagName() == "F" && !del)
          v["F"] = ee.text().toInt();
        if(ee.tagName() == "PLU" && !del)
          v["PLU"] = ee.text().toInt();
        if(ee.tagName() == "BAL" && !del)
          v["BAL"] = ee.text().toDouble();
        if(ee.tagName() == "PRC" && !del)
          v["PRC"] = ee.text().toDouble();
        ee = ee.nextSiblingElement();
      }
      ok = updateItem("CNT", "PAN", v);
    }
    e = e.nextSiblingElement();
  }

  if(ok)
    q.exec("COMMIT");
  else
    q.exec("ROLLBACK");


  QDomElement rev = response.doc().createElement("OLD");
  for(QVariantMap::Iterator it = oldRev.begin(); it != oldRev.end(); ++it)
  {
    QDomElement e = response.doc().createElement(it.key());
    e.setAttribute("REV", it.value().toInt());
    rev.appendChild(e);
  }
  response.data().appendChild(rev);

  QVariantMap newRev = revisionsGet();
  rev = response.doc().createElement("REV");
  for(QVariantMap::Iterator it = newRev.begin(); it != newRev.end(); ++it)
  {
    QDomElement e = response.doc().createElement(it.key());
    e.setAttribute("REV", it.value().toInt());
    rev.appendChild(e);
  }
  response.data().appendChild(rev);

  return ok;
}
/*----------------------------------------------------------------------------*/
bool StoreDatabase :: updateItem(const QString& tableName, const QString& primaryKey,  const QVariantMap &v)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));

  q.prepare(QString("select \"%1\" from \"%2\" where \"%3\" = ?").arg(primaryKey).arg(tableName).arg(primaryKey));
  q.addBindValue(v[primaryKey]);

  if(!q.exec())
  {
    error(q);
    return false;
  }

  if(q.first())
  {
    QString sql = QString("update \"%1\" set ").arg(tableName);

    bool comma = false;
    for(QVariantMap::ConstIterator it = v.begin(); it != v.end(); ++it)
    {
      if(it.key() != primaryKey)
      {
        if(comma)
          sql += ",";
        comma = true;
        sql += QString("\"%1\" = ? ").arg(it.key());
      }
    }

    sql += QString("where \"%1\" = ?").arg(primaryKey);
    q.prepare(sql);
    for(QVariantMap::ConstIterator it = v.begin(); it != v.end(); ++it)
    {
      if(it.key() != primaryKey)
      {
        q.addBindValue(it.value());
      }
    }
    q.addBindValue(v[primaryKey]);
  }
  else
  {
    QString sql = QString("insert into \"%1\"(").arg(tableName);
    for(QVariantMap::ConstIterator it = v.begin(); it != v.end(); ++it)
    {
      if(it != v.begin())
        sql += ",";
      sql += QString("\"%1\"").arg(it.key());
    }

    sql += ") values(";
    for(QVariantMap::ConstIterator it = v.begin(); it != v.end(); ++it)
    {
      if(it != v.begin())
        sql += ",";
      sql += "?";
    }
    sql += ")";
    q.prepare(sql);
    for(QVariantMap::ConstIterator it = v.begin(); it != v.end(); ++it)
    {
      q.addBindValue(it.value());
    }
  }

  if(!q.exec())
  {
    error(q);
    return false;
  }
  return true;
}
/*----------------------------------------------------------------------------*/
QVariantMap StoreDatabase :: revisionsGet()
{
  QVariantMap m;
  QStringList tables, entries;
  tables << "PLU_REV"
         << "PRC_REV"
         << "GRP_REV"
         << "DPT_REV"
         << "BAR_REV"
         << "EMT_REV"
         << "CNT_REV";

  entries << "PLU"
          << "PRC"
          << "GRP"
          << "DPT"
          << "BAR"
          << "EMT"
          << "CNT";

  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  for(int i = 0; i < tables.size(); i++)
  {
    q.prepare(QString("select max(ROWID) from %1").arg(tables.at(i)));
    if(q.exec() & q.first())
      m[entries.at(i)] = q.value(0);
  }
  return m;
}
/*----------------------------------------------------------------------------*/

