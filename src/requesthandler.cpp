/**
  @file
  @author Stefan Frings
*/

#include "requesthandler.h"
#include "filelogger.h"
#include "web_requesthandler.h"
#include "cash_requesthandler.h"

/** Logger class */
extern FileLogger* logger;

RequestHandler::RequestHandler(QSettings* webSettings, QSettings* cashSettings, QObject* parent)
    :HttpRequestHandler(parent)
{
  mCashPathList << "/register"
                << "/upload"
                << "/download"
                << "/sync"
                << "/update"
                << "/ksef_get";
  mWebRequestHandler = new WebRequestHandler(webSettings, this);
  mCashRequestHandler = new CashRequestHandler(cashSettings, this);
}

/*----------------------------------------------------------------------------*/
void RequestHandler::service(HttpRequest& request, HttpResponse& response)
{
  QString path = request.getPath();

  // Clear the log buffer
  if (logger)
  {
    logger->clear();
  }

  qWarning("%s:%s:%s", qPrintable(request.getPeerAddress().toString()),
         qPrintable(request.getMethod()),
         qPrintable(request.getPath())
         );

  if(request.getMethod() == "POST" && mCashPathList.contains(path, Qt::CaseInsensitive))
  {
    qDebug("RequestHandler: redirect to cashService\n");
    mCashRequestHandler->service(request, response);
  }
  else
  {
    qDebug("RequestHandler: redirect to webService");
    mWebRequestHandler->service(request, response);
  }
  qDebug("RequestHandler: finished request");
}
/*----------------------------------------------------------------------------*/
