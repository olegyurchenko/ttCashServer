/*----------------------------------------------------------------------------*/
/**
* @pkg web_requesthandler
*/
/**
* Class to handle requests from browsers.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 16.01.2016  9:04:23<br>
* @pkgdoc web_requesthandler
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#include "web_requesthandler.h"
#include <httpsessionstore.h>
#include "database.h"
#include "json.h"
#include <stdio.h>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include "template.h"
#include "ksef_lib.h"
/*----------------------------------------------------------------------------*/
HttpSessionStore* sessionStore=0;

WebRequestHandler :: WebRequestHandler(QSettings* settings, QObject* parent)
  : StaticFileController(settings, parent)
{
  settings->endGroup();
  settings->beginGroup("sessions");
  mSessionStore= new HttpSessionStore(settings, this);


  mDatabasePathList << "/ksef" << "/ksef/doc" << "/store";
  mLibKsef = new LibKsef(this);
}
/*----------------------------------------------------------------------------*/
void WebRequestHandler :: service(HttpRequest& request, HttpResponse& response)
{
  HttpSession session = mSessionStore->getSession(request,response,true);
  QString path = request.getPath();
  if(mDatabasePathList.contains(path, Qt::CaseInsensitive))
  {
    User user;
    QByteArray auth = request.getHeader("Authorization");
    if(!auth.isEmpty())
    {
      QList<QByteArray> lst = auth.split(' ');
      int i = 0;//lst.indexOf("basic");
      if(i >= 0 && i < lst.size() - 1)
      {
        //qDebug("Auth: '%s' '%s", qPrintable(lst.at(i + 1)), qPrintable(QByteArray::fromBase64(lst.at(i + 1))));
        lst = QByteArray::fromBase64(lst.at(i + 1)).split(':');
        if(lst.size() >= 2)
        {
          user = database->userStorage()->user(lst.at(0), lst.at(1));
        }
      }
    }

    if(user.anonumous)
    {
      if(session.contains("userid"))
      {
        QByteArray hash = session.get("userid").toByteArray();
        user = database->userStorage()->userByHash(hash);
      }
    }

    if(!user.anonumous)
    {
      session.set("userid", user.hash);
    }

    if(path == "/ksef")
      ksefRequest(user, request, response);
    if(path == "/store")
      storeRequest(user, request, response);
    if(path == "/ksef/doc")
      ksefDocRequest(user, request, response);
  }
  else
    StaticFileController::service(request, response);
}
/*----------------------------------------------------------------------------*/
void WebRequestHandler :: ksefRequest(User& user, HttpRequest& request, HttpResponse& response)
{
  if(!user.ksef.read)
  {
    response.setHeader("Content-Type", "text/html; charset=utf8");
    response.setStatus(403,"forbidden");
    response.write("403 forbidden",true);
  }
  else
  {
    QSqlQuery q = database->ksefDatabase()->sqlQuery();
    QVariant result;
    query(request.getParameterMap(), q, result);

    response.setHeader("Content-Type", "application/json; charset=utf8");
    response.write(QtJson::serialize(result), true);
  }
}
/*----------------------------------------------------------------------------*/
void WebRequestHandler :: storeRequest(User& user, HttpRequest& request, HttpResponse& response)
{
  if(!user.store.read)
  {
    response.setHeader("Content-Type", "text/html; charset=utf8");
    response.setStatus(403,"forbidden");
    response.write("403 forbidden",true);
  }
  else
  {
    QSqlQuery q = database->storeDatabase()->sqlQuery();
    QVariant result;
    query(request.getParameterMap(), q, result);

    response.setHeader("Content-Type", "application/json; charset=utf8");
    response.write(QtJson::serialize(result), true);
  }
}
/*----------------------------------------------------------------------------*/
bool WebRequestHandler :: query(const QMultiMap<QByteArray,QByteArray>& params, QSqlQuery &q, QVariant& v)
{
  QMultiMap<QByteArray,QByteArray>::ConstIterator it;
  QString queryName;
  char buffer[256];
  QVariantList bindValues;


  it = params.find("query");
  if(it == params.end())
  {
    QtJson::insert(v, "status", -1);
    QtJson::insert(v, "message", "Query parameter not found");
    return false;
  }
  queryName = it.value();

  snprintf(buffer, sizeof(buffer) - 1, "param%d", bindValues.size());
  while((it = params.find(buffer)) != params.end())
  {
    bindValues.append(QString::fromUtf8(it.value()));
    //bindValues.append(2);
    snprintf(buffer, sizeof(buffer) - 1, "param%d", bindValues.size());
  }


  bool fileOk = false;
  QString sql = loadSqlFromFile(QString(":/sql/%1.sql").arg(queryName), &fileOk);
  if(!fileOk)
  {
    QtJson::insert(v, "status", -1);
    QtJson::insert(v, "message", QString("Query '%1' not found").arg(queryName));
    return false;
  }


  Template tmpl(sql, queryName);

  tmpl.setCondition("paramCount", !bindValues.isEmpty());
  tmpl.loop("param", bindValues.size());
  for(int i = 0; i < bindValues.size(); i++)
  {
    tmpl.setVariable(QString("param%1.comma").arg(i), i ? "," : "");
  }
  sql = tmpl;
  //sql = "select c.C_ID as id, d.TS as 'time', c.T as type from TAG_C c left join TAG_DAT d on d.DAT_ID = c.DAT_ID  where d.CR_ID in (  ?  )  order by c.C_ID desc limit 100";
  q.prepare(sql);
  for(int i = 0; i < bindValues.size(); i++)
  {
    QVariant value = bindValues.at(i);
    q.addBindValue(value);
  }

  if(!q.exec())
  {
    QtJson::insert(v, "status", -1);
    QtJson::insert(v, "message", QString("Query error: '%1'").arg(q.lastError().text()));
    return false;
  }
  QtJson::insert(v, "status", 0);
  QtJson::insert(v, "message", "");
  QtJson::setDateTimeFormat("yyyy-MM-dd hh:mm:ss");
  QtJson::setDateFormat("yyyy-MM-dd");
  QVariant data;
  if(q.first())
  {
    do
    {
      QSqlRecord rec = q.record();
      int count = rec.count();
      QVariant record;
      for(int i = 0; i < count; i++)
      {
        QtJson::insert(record, rec.fieldName(i), rec.field(i).value());
      }
      QtJson::append(data, record);
    } while(q.next());
  }
  QtJson::insert(v, "data", data);
  return true;
}
/*----------------------------------------------------------------------------*/
QString WebRequestHandler :: loadSqlFromFile(const QString& fileName, bool *ok)
{
  QFile f(fileName);
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    *ok = false;
    return QString();
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
  *ok = true;
  return sql;
}
/*----------------------------------------------------------------------------*/
void WebRequestHandler :: ksefDocRequest(User& user, HttpRequest& request, HttpResponse& response)
{
  QString content =
      "<html>"
      "<head>\n"
      "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\">\n"
      "</head>\n"
      "<body>\n";

  response.setHeader("Content-Type", "text/html; charset=utf8");
  if(!user.ksef.read)
  {
    response.setStatus(403,"forbidden");
    content += "forbidden";
  }
  else
  {
    QSqlQuery q = database->ksefDatabase()->sqlQuery();
    int id = QString :: fromUtf8(request.getParameter("id")).toInt();

    q.prepare("select xml from TAG_DAT where DAT_ID = ?");
    q.addBindValue(id);
    if(!q.exec())
    {
      content += QString("Query error: '%1'").arg(q.lastError().text());
    }
    else
    {
      if(q.first())
      {
        QString xml = QString("<KSEF>%1</KSEF>").arg(q.value(0).toString());
        QString bill;
        if(mLibKsef->mkHtmlBill(xml, &bill))
        {
          content += QString("<pre>%1</pre>\n").arg(bill);
        }
        else
        {
          content += QString("Error %1").arg(mLibKsef->errorString());
        }
      }
      else
      {
        response.setStatus(404,"not found");
        content += "404 not found";
      }
    }
  }
  content += "</body></html>";
  response.write(content.toUtf8(), true);

}
/*----------------------------------------------------------------------------*/

