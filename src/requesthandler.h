#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "httprequesthandler.h"
#include <QStringList>

/**
  The request handler receives incoming HTTP requests and generates responses.
*/

class WebRequestHandler;
class CashRequestHandler;
class RequestHandler : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestHandler)
public:

    /**
      Constructor.
      @param parent Parent object
    */
    RequestHandler(QSettings* webSettings, QSettings* cashSettings, QObject* parent=0);

    /**
      Process an incoming HTTP request.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(HttpRequest& request, HttpResponse& response);

protected:
    QStringList mCashPathList;
    WebRequestHandler *mWebRequestHandler;
    CashRequestHandler *mCashRequestHandler;
};

#endif // REQUESTHANDLER_H
