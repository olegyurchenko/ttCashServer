/*----------------------------------------------------------------------------*/
/**
* @pkg ksef_lib
*/
/**
* Class helper to work with ksef.dll.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 08.02.2016  9:45:38<br>
* @pkgdoc ksef_lib
* @author oleg
* @version 0.01 
*/
/*----------------------------------------------------------------------------*/
#ifndef KSEF_LIB_H_1454917538
#define KSEF_LIB_H_1454917538
/*----------------------------------------------------------------------------*/
#include <QLibrary>

#ifndef KSEF_API
  #ifdef WIN32
  #if defined(__BORLANDC__)
    #define KSEF_API_DECL __stdcall
  #else
    #define KSEF_API_DECL __cdecl
  #endif

  #define DLL_KSEF_API_EXPORT __declspec(dllexport) KSEF_API_DECL
    #define DLL_KSEF_API_IMPORT __declspec(dllimport) KSEF_API_DECL
  #else /*WIN32*/
    #define KSEF_API_DECL
    #define DLL_KSEF_API_EXPORT
    #define DLL_KSEF_API_IMPORT
  #endif /*WIN32*/

  #if !defined(KSEF_API_STATIC) && !defined(KSEF_LATE_BINDING)
    #define KSEF_API DLL_KSEF_API_IMPORT
  #else /*KSEF_API_STATIC || PTS_LATE_BINDING*/
    #define KSEF_API KSEF_API_DECL
  #endif  /*KSEF_API_STATIC || PTS_LATE_BINDING*/
#endif /*KSEF_API*/

class LibKsef : public QLibrary
{
protected:
  typedef int KSEF_API (*ksef_mk_bill_t)(const char *xml, char **dst);
  typedef void KSEF_API (*ksef_free_t)(void *p);

  ksef_mk_bill_t _mk_html_bill;
  ksef_mk_bill_t _mk_txt_bill;
  ksef_free_t _free;

  QString mErrorMessage;

protected:
  bool init();

public:
  LibKsef(QObject *parent = 0);
  QString errorMessage() { return mErrorMessage;}


  bool mkTxtBill(const QString& xml, QString *dst);
  bool mkHtmlBill(const QString& xml, QString *dst);
};
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif /*KSEF_LIB_H_1454917538*/

