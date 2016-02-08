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
#ifndef WEB_REQUESTHANDLER_H_1452927863
#define WEB_REQUESTHANDLER_H_1452927863
/*----------------------------------------------------------------------------*/
#include "staticfilecontroller.h"
#include <QStringList>
#include <QMultiMap>

class QSettings;
class HttpSessionStore;
class User;
class QSqlQuery;
class LibKsef;
class WebRequestHandler : public StaticFileController
{
    Q_OBJECT
    Q_DISABLE_COPY(WebRequestHandler)
public:
    WebRequestHandler(QSettings* settings, QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);

protected:
    QStringList mDatabasePathList;
    HttpSessionStore* mSessionStore;
    LibKsef *mLibKsef;

    void ksefRequest(User& user, HttpRequest& request, HttpResponse& response);
    void storeRequest(User& user, HttpRequest& request, HttpResponse& response);
    void ksefDocRequest(User& user, HttpRequest& request, HttpResponse& response);

    bool query(const QMultiMap<QByteArray,QByteArray>& params, QSqlQuery &q, QVariant& v);
    QString loadSqlFromFile(const QString& fileName, bool *ok);

};

/*----------------------------------------------------------------------------*/
#endif /*WEB_REQUESTHANDLER_H_1452927863*/

