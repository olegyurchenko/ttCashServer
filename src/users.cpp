/*----------------------------------------------------------------------------*/
/**
* @pkg users
*/
/**
* Users for server access.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 25.01.2016 12:02:31<br>
* @pkgdoc users
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#include "users.h"
#include <QStringList>
#include <QCryptographicHash>
/*----------------------------------------------------------------------------*/
UserStrorage :: UserStrorage(QSettings *settings, QObject *parent)
  : QObject(parent)
{
  foreach(QString name, settings->childKeys())
  {
    QString val = settings->value(name, QString(",0,0,0,0")).toString();
    QStringList lst = val.split(';', QString::KeepEmptyParts);
    User u;
    u.name = name;
    if(lst.size() >= 1)
      u.pass = lst.at(0);
    if(lst.size() >= 2)
      u.ksef.read = lst.at(1).toInt();
    if(lst.size() >= 3)
      u.ksef.write = lst.at(2).toInt();
    if(lst.size() >= 4)
      u.store.read = lst.at(3).toInt();
    if(lst.size() >= 5)
      u.store.write = lst.at(4).toInt();

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(u.name.toUtf8());
    hash.addData(u.pass.toUtf8());
    u.hash = hash.result();
    mUserMap[name] = u;
  }
}
/*----------------------------------------------------------------------------*/
UserStrorage :: ~UserStrorage()
{
}
/*----------------------------------------------------------------------------*/
User UserStrorage :: user(const QString& name, const QString& pass) const
{
  QMap<QString, User>::ConstIterator it = mUserMap.find(name);
  User u;
  if(it != mUserMap.end())
  {
    u = it.value();
    u.anonumous = false;
  }

  if(!u.name.isEmpty() && u.pass != pass)
    u = User();

  if(u.name.isEmpty() && (it = mUserMap.find("anonymous")) != mUserMap.end())
    u = it.value();

  if(u.name.isEmpty())
    u.name = "anonymous";

  u.pass = "";
  return u;
}
/*----------------------------------------------------------------------------*/
User UserStrorage :: userByHash(const QByteArray& hash) const
{
  User u;
  QMap<QString, User>::ConstIterator it;
  for(it = mUserMap.begin(); it != mUserMap.end(); ++it)
  {
    if(it.value().hash == hash)
    {
      u = it.value();
      u.anonumous = false;
      break;
    }
  }

  if(u.name.isEmpty() && (it = mUserMap.find("anonymous")) != mUserMap.end())
    u = it.value();

  if(u.name.isEmpty())
    u.name = "anonymous";

  u.pass = "";
  return u;

}
/*----------------------------------------------------------------------------*/
