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
#include "ksef_lib.h"
#include "fm_errors.h"
/*----------------------------------------------------------------------------*/
LibKsef :: LibKsef(QObject *parent)
  : QLibrary(parent)
{
  _mk_txt_bill = NULL;
  _mk_html_bill = NULL;
  _free = NULL;
}
/*----------------------------------------------------------------------------*/
bool LibKsef :: init()
{
  QString ext = ".so";
  QString path = "./";
#ifdef WIN32
  ext = ".dll";
  path = "";
#endif
  if(!isLoaded())
  {
    setFileName(QString("%1ksef%2").arg(path).arg(ext));
    if(!load())
    {
      setFileName(QString("%1libksef%2").arg(path).arg(ext));
      if(!load())
        return false;
    }

    _mk_txt_bill = (ksef_mk_bill_t) resolve("_ksef_mk_bill");
    if(_mk_txt_bill == NULL)
      _mk_txt_bill = (ksef_mk_bill_t) resolve("ksef_mk_bill");
    if(_mk_txt_bill == NULL)
      return false;

    _mk_html_bill = (ksef_mk_bill_t) resolve("_ksef_mk_html");
    if(_mk_html_bill == NULL)
      _mk_html_bill = (ksef_mk_bill_t) resolve("ksef_mk_html");
    if(_mk_html_bill == NULL)
      return false;


    _free = (ksef_free_t)resolve("_ksef_free");
    if(_free == NULL)
      _free = (ksef_free_t)resolve("ksef_free");
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool LibKsef :: mkTxtBill(const QString& xml, QString *dst)
{
  if(!init())
    return false;

  if(_mk_txt_bill == NULL)
    return false;

  char *bill = NULL;
  int r = _mk_txt_bill(xml.toUtf8().constData(), &bill);
  if(r != 0)
  {
    mErrorMessage = fm_str_error(r);
    if(_free != NULL && bill != NULL)
      _free(bill);
    return false;
  }
  *dst = QString::fromUtf8(bill);
  if(_free != NULL && bill != NULL)
    _free(bill);
  return true;
}
/*----------------------------------------------------------------------------*/
bool LibKsef :: mkHtmlBill(const QString& xml, QString *dst)
{
  if(!init())
    return false;

  if(_mk_html_bill == NULL)
    return false;

  char *bill = NULL;
  int r = _mk_html_bill(xml.toUtf8().constData(), &bill);
  if(r != 0)
  {
    mErrorMessage = fm_str_error(r);
    if(_free != NULL && bill != NULL)
      _free(bill);
    return false;
  }
  *dst = QString::fromUtf8(bill);
  if(_free != NULL && bill != NULL)
    _free(bill);
  return true;
}
/*----------------------------------------------------------------------------*/

