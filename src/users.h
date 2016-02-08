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
#ifndef USERS_H_1453716151
#define USERS_H_1453716151
/*----------------------------------------------------------------------------*/
#include <QString>
#include <QSettings>
#include <QObject>
#include <QMap>

class Access
{
public:
  Access() : read(false), write(false) {}
  bool read;
  bool write;
};

class User
{
public:
  User() : anonumous(true) {}
  QString name;
  QString pass;
  Access ksef;
  Access store;
  bool anonumous;
  QByteArray hash;
};

class UserStrorage : public QObject
{
  Q_OBJECT
protected:
  QMap<QString, User> mUserMap;
public:
  UserStrorage(QSettings *settings, QObject *parent = NULL);
  virtual ~UserStrorage();

  User user(const QString& name, const QString& pass) const;
  User userByHash(const QByteArray& hash) const;

};

/*----------------------------------------------------------------------------*/
#endif /*USERS_H_1453716151*/

