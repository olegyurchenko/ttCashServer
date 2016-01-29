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
#include "xmlrequest.h"
/*----------------------------------------------------------------------------*/
#define CONNECTION_NAME "KSEF"
static int m_unique = 0;
/*----------------------------------------------------------------------------*/
class _KsefFilter
{
public:
  QDateTime dateTime;
  int di;
  int billNumber;
  _KsefFilter() : di(-1),billNumber(-1) {}
};

class KsefFilter
{
public:
  KsefFilter() : billCount(-1), docType(0) {}
  _KsefFilter from;
  _KsefFilter to;
  int billCount;
  int docType;
  QStringList crList;
};
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

  mFileName = dir.filePath("ksef.db");
  qDebug("KsefDatabase: fileName=%s, maxSize=%lld",qPrintable(mFileName), mMaxSize);


  //QFileInfo fi(mFileName);
  //bool exists = fi.exists();

  QSqlDatabase m_database = QSqlDatabase::addDatabase("QSQLITE", mConnectionName);
  m_database.setDatabaseName(mFileName);
  if(!m_database.open())
  {
    mIsError = true;
    mMessage = m_database.lastError().text();
    return;
  }

  create();
  mTerminated = false;
  if(!isError())
    start();
}
/*----------------------------------------------------------------------------*/
KsefDatabase :: ~KsefDatabase()
{

  mTerminated = true;
  wait(1000);
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
    lock();
    q.prepare("insert into CR(ZN,REG) values(?,?)");
    q.addBindValue(doc.serial());
    q.addBindValue(QDateTime::currentDateTime());
    if(!q.exec())
    {
      unlock();
      error(q);
      return false;
    }
    unlock();
  }
  //!!!!!!!!!!!!!!!!!!!
  //TODO: Send not 0 DI !!!
  //!!!!!!!!!!!!!!!!!!!
  int maxDi = 0;
  lock();
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
    unlock();
    error(q);
    return false;
  }
  unlock();
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
  lock();
  if(!q.exec())
  {
    unlock();
    error(q);
    return -2;
  }
  unlock();
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
  lock();
  if(!q.exec())
  {
    unlock();
    error(q);
    return false;
  }
  unlock();

  if(!q.first())
  {
    q.prepare("insert into TAG_DAT(CR_ID,DI,ZN,FN,TN,TS,UP,EXT,XML) values(?,?,?,?,?,?,?,?,?)");
    q.addBindValue(id);
    q.addBindValue(doc.di());
    q.addBindValue(doc.serial());
    q.addBindValue(doc.fiscal());
    q.addBindValue(doc.tax());
    q.addBindValue(doc.time());
    q.addBindValue(QDateTime::currentDateTime());
    q.addBindValue(0);
    q.addBindValue(doc.toXmlString());
    lock();
    if(!q.exec())
    {
      unlock();
      error(q);
      return false;
    }
    unlock();
  }

  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: query(XmlRequest& request, XmlResponse& response)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  QDomElement data = request.data();
  QDomElement e = data.firstChildElement("RQ");

  e = e.firstChildElement();

  while(!e.isNull() && !response.isError())
  {
    if(e.tagName() == "CR_GET")
    {
      if(!crResponse(response))
        return false;
    }
    if(e.tagName() == "DOC_GET")
    {
      KsefFilter f;

      f.billCount = e.attribute("RC", "-1").toInt();
      f.docType = e.attribute("TY", "0").toInt();

      QDomElement ee = e.firstChildElement("F");
      if(!ee.isNull())
      {
        if(ee.attributes().contains("TS"))
          f.from.dateTime = KsefDocument::ts2time(ee.attribute("TS"));
        f.from.billNumber = ee.attribute("DN", "-1").toInt();
        f.from.di = ee.attribute("DI", "-1").toInt();
      }
      ee = e.firstChildElement("T");
      if(!ee.isNull())
      {
        if(ee.attributes().contains("TS"))
          f.to.dateTime = KsefDocument::ts2time(ee.attribute("TS"));
        f.to.billNumber = ee.attribute("DN", "-1").toInt();
        f.to.di = ee.attribute("DI", "-1").toInt();
      }
      ee = e.firstChildElement("CR");
      while(!ee.isNull())
      {
        QString zn = ee.attribute("ZN", "");
        if(!zn.isEmpty())
          f.crList.append(zn);
        ee = ee.nextSiblingElement("CR");
      }


      QVariantList values;
      QString sql;

      sql = "select XML from TAG_DAT ";
      if(f.docType > 0)
      {
        sql += ",TAG_C where TAG_DAT.DAT_ID = TAG_C.DAT_ID ";
        if(f.docType > 1)
          sql += "and TAG_C.T>=100 ";
        else
          sql += "and TAG_C.T<100 ";
      }
      else
        sql += "where (1=1) ";

      if(f.from.di >= 0)
      {
        sql += "and TAG_DAT.DI >= ? ";
        values.append(f.from.di);
      }

      if(f.to.di >= 0)
      {
        sql += "and TAG_DAT.DI <= ? ";
        values.append(f.to.di);
      }

      if(f.from.dateTime.isValid())
      {
        sql += "and TAG_DAT.TS >= ? ";
        values.append(f.from.dateTime);
      }

      if(f.to.dateTime.isValid())
      {
        sql += "and TAG_DAT.TS <= ? ";
        values.append(f.to.dateTime);
      }

      if(!f.crList.isEmpty())
      {
        sql += "and TAG_DAT.ZN in (";
        for(int i = 0; i < f.crList.size(); i++)
        {
          if(i)
            sql += ",";
          sql += "?";
          values.append(f.crList.at(i));
        }
        sql += ") ";
      }

      if(f.billCount > 0)
      {
        sql += "LIMIT ?";
        values.append(f.billCount);
      }

      qWarning("SQL='%s'", qPrintable(sql));
      q.prepare(sql);
      foreach (QVariant v, values)
      {
        q.addBindValue(v);
      }
      if(!q.exec())
        return !error(q);

      if(!datResponse(q, response))
        return false;
    }


    e = e.nextSiblingElement();
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: crResponse(XmlResponse& response)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));
  if(!q.exec("select ZN from CR"))
    return error(q);
  while (q.next())
  {
    QDomElement e = response.doc().createElement("CR");
    e.setAttribute("ZN", q.value(0).toString());
    response.data().appendChild(e);
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: datResponse(QSqlQuery &q, XmlResponse& response)
{
  while(q.next())
  {
    QDomDocument doc;

    if(doc.setContent(q.value(0).toString()))
      response.data().appendChild(response.doc().importNode(doc.firstChildElement("DAT"), true));
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: error(const QSqlQuery &q)
{
  if(q.lastError().isValid())
  {
    mMessage = QString("%1:\n'%2'").arg(q.lastError().text()).arg(q.lastQuery());
    qCritical("KsefDatabase:%s", qPrintable(mMessage));
    mIsError = true;
    return true;
  }
  mIsError = false;
  return false;
}
/*----------------------------------------------------------------------------*/
void KsefDatabase :: logError(const QSqlQuery &q)
{
  if(q.lastError().isValid())
  {
    qCritical("%s:\n%s", qPrintable(q.lastError().text()), qPrintable(q.lastQuery()));
  }
}
/*----------------------------------------------------------------------------*/
void KsefDatabase :: run()
{
  while(!mTerminated)
  {
    QSqlQuery q(QSqlDatabase::database(mConnectionName));
    lock();
    if(!q.exec("select DAT_ID,XML from TAG_DAT where EXT=0 LIMIT 1"))
    {
      unlock();
      logError(q);
      qCritical("Worker ABORTING");
      return;
    }
    unlock();

    if(q.first())
    {
      int datId = q.value(0).toInt();
      QString xml = q.value(1).toString();
      KsefDocument doc;
      lock();
      bool ok = q.exec("begin transaction");
      if(ok)
        do {

        if(!doc.parse(xml))
        {
          q.prepare("update TAG_DAT set EXT = -1 where DAT_ID = ?");
          q.addBindValue(datId);
          if(!(ok = q.exec()))
          {
            logError(q);
          }
          break;
        }

        if(!q.exec("select max(C_ID) from TAG_C") || !q.first())
        {
          logError(q);
          break;
        }

        int cId = q.value(0).toInt() + 1;
        int billCount = doc.bills().count();

        for(int i= 0; ok && i < billCount; i++)
        {
          q.prepare("insert into TAG_C(C_ID,DAT_ID,T) values(?,?,?)");
          q.addBindValue(cId);
          q.addBindValue(datId);
          q.addBindValue((int)doc.bills().at(i).type());

          if(!q.exec())
          {
            logError(q);
            ok = false;
            break;
          }

          int itemCont = doc.bills().at(i).items().count();
          for(int j = 0; j < itemCont && ok; j++)
          {
            ok = addItem(datId, cId, doc.bills().at(i).items().at(j));
          }
          cId ++;
        }

        if(!ok)
          break;

        q.prepare("update TAG_DAT set EXT = 1 where DAT_ID = ?");
        q.addBindValue(datId);
        if(!(ok = q.exec()))
        {
          logError(q);
        }
      } while(0);

      if(ok)
        q.exec("commit");
      if(!ok)
        q.exec("rollback");
      unlock();

      if(!ok)
      {
        qCritical("Worker ABORTING");
        return;
      }
    }
    msleep(100);
  }
}
/*----------------------------------------------------------------------------*/
bool KsefDatabase :: addItem(int datId, int cId, const KsefBillItem& item)
{
  QSqlQuery q(QSqlDatabase::database(mConnectionName));

  q.exec("select max(ITEM_ID) from ITEMS");
  if(!q.exec() || !q.first())
  {
    logError(q);
    return false;
  }
  int itemId = q.value(0).toInt() + 1;

  q.prepare("insert into ITEMS(ITEM_ID,C_ID,DAT_ID,N,T) values(?,?,?,?,?)");
  q.addBindValue(itemId);
  q.addBindValue(cId);
  q.addBindValue(datId);
  q.addBindValue(item.N());
  q.addBindValue(item.type());
  if(!q.exec())
  {
    logError(q);
    return false;
  }

  switch(item.type())
  {
  case KsefBillItem::Sale:
  {
    const KsefBillItem::SaleData *data = item.saleData();

    q.prepare("insert into TAG_P(ITEM_ID,N,C,CD,NM,PRC,Q,RT,SM,TX,TX2) values(?,?,?,?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->C);
    q.addBindValue(data->CD);
    q.addBindValue(data->NM);
    q.addBindValue(data->PRC);
    q.addBindValue(data->Q);
    q.addBindValue(data->RT);
    q.addBindValue(data->SM);
    q.addBindValue(data->TX);
    q.addBindValue(data->TX2);
    break;
  }
  case KsefBillItem::Comment:
  {
    const KsefBillItem::CommentData *data = item.commentData();

    q.prepare("insert into TAG_L(ITEM_ID,N,TXT) values(?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->TXT);
    break;
  }
  case KsefBillItem::Discount:
  {
    const KsefBillItem::DiscountData *data = item.discountData();

    q.prepare("insert into TAG_D(ITEM_ID,N,PR,S,SM,ST,TR,TY) values(?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->PR);
    q.addBindValue(data->S);
    q.addBindValue(data->SM);
    q.addBindValue(data->ST);
    q.addBindValue(data->TR);
    q.addBindValue(data->TY);
    break;
  }
  case KsefBillItem::Sum:
  {
    const KsefBillItem::SumData *data = item.sumData();

    q.prepare("insert into TAG_E(ITEM_ID,N,CS,\"NO\",SE,SM,TS) values(?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->CS);
    q.addBindValue(data->NO);
    q.addBindValue(data->SE);
    q.addBindValue(data->SM);
    q.addBindValue(data->TS);

    break;
  }
  case KsefBillItem::Tax:
  {
    const KsefBillItem::TaxData *data = item.taxData();

    q.prepare("insert into TAG_TX(ITEM_ID,DTPR,DTSM,TX,TXAL,TXPR,TXSM,TXTY) values(?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->DTPR);
    q.addBindValue(data->DTSM);
    q.addBindValue(data->TX);
    q.addBindValue(data->TXAL);
    q.addBindValue(data->TXPR);
    q.addBindValue(data->TXSM);
    q.addBindValue(data->TXTY);
    break;
  }
  case KsefBillItem::SaleFuel:
  {
    const KsefBillItem::SaleFuelData *data = item.saleFuelData();

    q.prepare("insert into TAG_PP(ITEM_ID,N,C,CD,KRK,NM,OV,PRC,PRK,Q,SM,TX,TX2) values(?,?,?,?,?,?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->C);
    q.addBindValue(data->CD);
    q.addBindValue(data->KRK);
    q.addBindValue(data->NM);
    q.addBindValue(data->OV);
    q.addBindValue(data->PRC);
    q.addBindValue(data->PRK);
    q.addBindValue(data->Q);
    q.addBindValue(data->SM);
    q.addBindValue(data->TX);
    q.addBindValue(data->TX2);
    break;
  }
  case KsefBillItem::Payment:
  {
    const KsefBillItem::PaymentData *data = item.paymentData();

    q.prepare("insert into TAG_M(ITEM_ID,N,NM,RM,RRN,SM,T) values(?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->NM);
    q.addBindValue(data->RM);
    q.addBindValue(data->RRN);
    q.addBindValue(data->SM);
    q.addBindValue(data->T);
    break;
  }
  case KsefBillItem::Receive:
  {
    const KsefBillItem::ReseiveData *data = item.reseiveData();

    q.prepare("insert into TAG_IO(ITEM_ID,N,O,SM) values(?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->O);
    q.addBindValue(data->SM);
    break;
  }
  case KsefBillItem::Deliver:
  {
    const KsefBillItem::DeliverData *data = item.deliverData();

    q.prepare("insert into TAG_IF(ITEM_ID,N,C,NM,NR,OV,Q) values(?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->C);
    q.addBindValue(data->NM);
    q.addBindValue(data->NR);
    q.addBindValue(data->OV);
    q.addBindValue(data->Q);
    break;
  }
  case KsefBillItem::PumpCheck:
  {
    const KsefBillItem::PumpCheckData *data = item.pumpCheckData();

    q.prepare("insert into TAG_PV(ITEM_ID,N,C,KRK,NM,OV,PRK,Q) values(?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(item.N());
    q.addBindValue(data->C);
    q.addBindValue(data->KRK);
    q.addBindValue(data->NM);
    q.addBindValue(data->OV);
    q.addBindValue(data->PRK);
    q.addBindValue(data->Q);
    break;
  }
  case KsefBillItem::ZTax:
  {
    const KsefBillItem::ZTaxData *data = item.zTaxData();

    q.prepare("insert into TAG_TXSZ(ITEM_ID,DTI,DTO,DTPR,SMI,SMO,TX,TXAL,TXI,TXO,TXPR,TXTY,TS) "
              "values(?,?,?,?,?,?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->DTI);
    q.addBindValue(data->DTO);
    q.addBindValue(data->DTPR);
    q.addBindValue(data->SMI);
    q.addBindValue(data->SMO);
    q.addBindValue(data->TX);
    q.addBindValue(data->TXAL);
    q.addBindValue(data->TXI);
    q.addBindValue( data->TXO);
    q.addBindValue(data->TXPR);
    q.addBindValue(data->TXTY);
    q.addBindValue(data->TS);
    break;
  }
  case KsefBillItem::ZReceive:
  {
    const KsefBillItem::ZReseiveData *data = item.zReseiveData();

    q.prepare("insert into TAG_IOZ(ITEM_ID,SMI,SMO) values(?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->SMI);
    q.addBindValue(data->SMO);
    break;
  }
  case KsefBillItem::ZBillCount:
  {
    const KsefBillItem::ZBillCountData *data  = item.zBillCountData();

    q.prepare("insert into TAG_NCZ(ITEM_ID,NI,NO) values(?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->NI);
    q.addBindValue(data->NO);
    break;
  }
  case KsefBillItem::ZPayment:
  {
    const KsefBillItem::ZPaymentData *data = item.zPaymentData();

    q.prepare("insert into TAG_MZ(ITEM_ID,NM,SMI,SMO,T) values(?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->NM);
    q.addBindValue(data->SMI);
    q.addBindValue(data->SMO);
    q.addBindValue(data->T);
    break;
  }
  case KsefBillItem::ZFuel:
  {
    const KsefBillItem::ZFuelData *data = item.zFuelData();

    q.prepare("insert into TAG_FZ(ITEM_ID,C,IF,NM,OF,OFP,SF,SMI) values(?,?,?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->C);
    q.addBindValue(data->IF);
    q.addBindValue(data->NM);
    q.addBindValue(data->OF);
    q.addBindValue(data->OFP);
    q.addBindValue(data->SF);
    q.addBindValue(data->SMI);
    break;
  }
  case KsefBillItem::ZTank:
  {
    const KsefBillItem::ZTankData *data = item.zTankData();

    q.prepare("insert into TAG_SFZ(ITEM_ID,C,NR,SF) values(?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->C);
    q.addBindValue(data->NR);
    q.addBindValue(data->SF);
    break;
  }

  case KsefBillItem::ZFPayment:
  {
    const KsefBillItem::ZFPaymentData *data = item.zFPaymentData();

    q.prepare("insert into TAG_SMIZ(ITEM_ID,C,NM,OF,SM,T) values(?,?,?,?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->C);
    q.addBindValue(data->NM);
    q.addBindValue(data->OF);
    q.addBindValue(data->SM);
    q.addBindValue(data->T);
    break;
  }
  case KsefBillItem::ZPump:
  {
    const KsefBillItem::ZPumpData *data = item.zPumpData();

    q.prepare("insert into TAG_NPSZ(ITEM_ID,NP,NR) values(?,?,?)");

    q.addBindValue(itemId);
    q.addBindValue(data->NP);
    q.addBindValue(data->NR);
    break;
  }
  default:
    return false;
  }
  if(!q.exec())
  {
    logError(q);
    return false;
  }
  return true;
}
/*----------------------------------------------------------------------------*/

