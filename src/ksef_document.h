/*----------------------------------------------------------------------------*/
/**
* @pkg ksef_document
*/
/**
* KsefDocument - class represebted KSEF DAT tag.
*
* (C) T&T, Kiev, Ukraine 2016.<br>
* started 18.01.2016 15:43:30<br>
* @pkgdoc ksef_document
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
#ifndef KSEF_DOCUMENT_H_1453124610
#define KSEF_DOCUMENT_H_1453124610
/*----------------------------------------------------------------------------*/
#include <QDomDocument>
#include <QDateTime>
#include <QList>

class KsefBill;
class KsefDocument //Tag DAT representation
{
protected:
  QString mSerial;
  QString mTax;
  QString mFiscal;
  QDateTime mTime;
  QDomDocument mDoc;
  QList<KsefBill> mBills;
  int mDi;
public:
  KsefDocument();
  virtual ~KsefDocument();

  bool assign(QDomElement& element, bool deep = false);
  bool parse(const QString&);

  QString toXmlString() const;

  QString serial() const {return mSerial;}
  QString tax() const {return mTax;}
  QString fiscal() const {return mFiscal;}
  QDateTime time() const {return mTime;}
  int di() const {return mDi;}
  const QList<KsefBill> &bills() const {return mBills;}
};
/*----------------------------------------------------------------------------*/
class KsefBillItem;
class KsefBill //Tag C representation
{
public:
  typedef enum
  { //0-чек продажи, 1-чек выплаты, 2-служебный чек, 3-чек приема топлива, 100-Z-отчет
    Unknown = -1,
    Sale = 0,
    Payout = 1,
    Service = 2,
    Deliver = 3,
    Report = 100
  } Type;
protected:
  QList<KsefBillItem> mItems;
  Type mType;
public:
  KsefBill(Type t = Unknown);
  virtual ~KsefBill();
  bool assign(QDomElement& element);
  Type type() const {return mType;}
  const QList<KsefBillItem> &items() const {return mItems;}
};
/*----------------------------------------------------------------------------*/
class KsefBillItem
{
public:
  typedef enum
  {
    Unknown = -1,
    Sale = 0,//0 - Запис «Продаж/виплата товару»:
    Comment = 1, //1 - Запис «Коментар»:
    Discount = 2, //2 - Запис «Знижка/націнка»:
    Sum = 3,//3 - Запис «Сума чека»:
    Tax = 4,//4 - Начисление налогов
    SaleFuel = 5, //5 - Запис «Продаж/повернення палива»:
    Payment = 6, //6 - Запис «Оплата»:
    Receive = 7, //7 - Запис службового внесення або службової видачі:
    Deliver = 8, //8 - Запис приймання пального:
    PumpCheck = 9, //9 - Запис повірки ПРК:

    //Для отчета
    ZTax = 100, //100 - Запис інформації по податковим ставкам
    ZReceive = 101, //101 - Запис інформації по внесенню та винесенню грошей (готівка)
    ZBillCount = 102, //102 - Кол. чеков оплат/выплат
    ZPayment = 103, //103 - Запис інформації оберту по типу оплати
    ZFuel = 104, //104 - Запис інформації по типу палива
    ZTank = 105, //105 - Запис інформації по резервуару
    ZFPayment = 106, //106 - Запис інформації по типам сплат за паливо
    ZPump = 107 //ТРК

  } Type;

  typedef struct
  {
    QString TXT;
  } CommentData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    QString CD; //VARCHAR(16), # Штрихкод товара
    QString NM; //VARCHAR(32), # Название товара
    int SM; //INTEGER, #  Сумма операции (копейки)
    int Q; //INTEGER, #  Количество (в 0.001 единицах)
    int PRC; //INTEGER, #  Цена товара (копейки)
    int TX; //INTEGER, #  Обозначение налога
    int TX2; //INTEGER, #  Обозначение 2-го налога
    int RT; //INTEGER  #  Тип выплаты
  } SaleData;

  typedef struct
  {
    bool S; //BOOL,    # Признак наценки
    int TR; //INTEGER, # Тип использования: 0-позиция, 1-промеж.сумма
    int TY; //INTEGER, # Тип 0 - сумма, 1 - проценты
    int ST; //INTEGER, # Сумма, на кот насчитывается (копейки)
    int PR; //INTEGER, # Процент (в 0.01%)
    int SM; //INTEGER  # Сумма операции (копейки)
  } DiscountData;

  typedef struct
  {
    int NO; //INTEGER, # Номер фискального чека
    int SM; //INTEGER, # Общая сумма чека (копейки)
    int SE; //INTEGER, # Общая сумма чека без учета налогов (копейки)
    int CS; //INTEGER, # Код кассира
    QDateTime TS; //DATETIME # Время создания документа
  } SumData;

  typedef struct
  {
    int TX; //INTEGER, # Обозначение налога
    int TXPR; //INTEGER, # Ставка в процентах (в 0.01%)
    int TXSM; //INTEGER, # Сумма налога (копейки)
    int DTPR; //INTEGER, # Ставка доп. сбора в процентах (в 0.01%)
    int DTSM; //INTEGER, # Сумма доп. сбора  (копейки)
    bool TXTY; //BOOL,    # Признак налога, не включенного в стоимость
    int TXAL; //INT      # Алгоритм начисления сбора
  } TaxData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    QString CD; //VARCHAR(16), # Штрихкод товара
    QString NM; //VARCHAR(32), # Название товара
    QString OV; //VARCHAR(16), # Единица изменения
    int PRK;  //INTEGER,     # Номер ТРК
    int KRK; //INTEGER,     # Номер крана
    int SM; //INTEGER, # Сумма операции (копейки)
    int Q; //INTEGER, #  Количество (в 0.001 единицах)
    int PRC; //INTEGER, #  Цена товара (копейки)
    int TX; //INTEGER, #  Обозначение налога
    int TX2; //INTEGER  #  Обозначение 2-го налога
  } SaleFuelData;

  typedef struct
  {
    int T; //INTEGER, # Код дормы оплаты. 0-Наличные
    QString NM; //VARCHAR(16), # Название формы оплаты
    int SM; //INTEGER, # Сумма операции (копейки)
    int RM; //INTEGER, # Сумма здачи (копейки)
    QString RRN; //VARCHAR(32) # Код транзакции от платежного терминала
  } PaymentData;

  typedef struct
  {
    bool O; //BOOL,    # Признак выноса
    int SM; //INTEGER  # Сумма операции (копейки)
  } ReseiveData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    QString NM; //VARCHAR(32), # Название товара
    QString OV; //VARCHAR(16), # Единица изменения
    int NR; //INTEGER,  # Номер резервуара
    int Q; //INTEGER  #  Количество (в 0.001 единицах)
  } DeliverData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    QString NM; //VARCHAR(32), # Название товара
    QString OV; //VARCHAR(16), # Единица изменения
    int PRK; //INTEGER, # Номер ТРК
    int KRK; //INTEGER, # Номер крана
    int Q; //INTEGER  # Количество (в 0.001 единицах)
  } PumpCheckData;

  typedef struct
  {
    int TX; //INTEGER, # Обозначение налога
    QDate TS; //DATE,    # Дата программирования налога
    int TXPR; //INTEGER, # Ставка в процентах (в 0.01%)
    int TXI; //INTEGER, # Сумма налога по продажам(копейки)
    int TXO; //INTEGER, # Сумма налога по выплатам (копейки)
    int DTPR; //INTEGER, # Ставка доп. сбора в процентах (в 0.01%)
    int DTI; //INTEGER, # Сумма доп. сбора по продажам (копейки)
    int DTO; //INTEGER, # Сумма доп. сбора по выплатам (копейки)
    int TXTY; //BOOL,    # Признак налога, не включенного в стоимость
    int TXAL; //INTEGER, # Алгоритм начисления сбора
    int SMI; //INTEGER, # Оборот по продажам (копейки)
    int SMO; //INTEGER  # Оборот по выплатам (копейки)
  } ZTaxData;

  typedef struct
  {
    int SMI; //INTEGER, #  Сумма вносов (копейки)
    int SMO; //INTEGER  #  Сумма выносов (копейки)
  } ZReseiveData;

  typedef struct
  {
    int NI; //INTEGER, # Кол. чеков оплат
    int NO; //INTEGER  # Кол. чеков выплат
  } ZBillCountData;

  typedef struct
  {
    int T; //INTEGER, # Код дормы оплаты. 0-Наличные
    QString NM; //VARCHAR(16), # Название формы оплаты
    int SMI; //INTEGER, # Сумма продаж (копейки)
    int SMO; //INTEGER  # Сумма выплат (копейки)
  } ZPaymentData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    QString NM; //VARCHAR(32), # Название товара
    int IF; //INTEGER, #  Количество принятого топлива в мл
    int OF; //INTEGER, #  Количество отпущенного топлива в мл
    int OFP; //INTEGER, #  Количество отпущенного при поверке ТРК топлива в мл
    int SMI; //INTEGER, #  Сумма принятых денег(копейки)
    int SF; //INTEGER  #  Остаток топлива в мл
  } ZFuelData;

  typedef struct
  {
    int NR; //INTEGER, # Номер резервуара
    int C; //INTEGER, # Код товара
    int SF; //INTEGER  # Остаток топлива в мл
  } ZTankData;

  typedef struct
  {
    int C; //INTEGER, # Код товара
    int T; //INTEGER, # Код дормы оплаты. 0-Наличные
    QString NM; //VARCHAR(16), # Название формы оплаты
    int SM; //INTEGER, # Сумма продаж (копейки)
    int OF; //INTEGER  # Отпущено (мл)
  } ZFPaymentData;

  typedef struct
  {
    int NP; //INTEGER, # Номер ТРК
    int NR; //INTEGER  # Номер резервуара
  } ZPumpData;

  typedef union
  {
    void *p;
    CommentData *commentData;
    SaleData *saleData;
    DiscountData *discountData;
    SumData *sumData;
    TaxData *taxData;
    SaleFuelData *saleFuelData;
    PaymentData *paymentData;
    ReseiveData *receiveData;
    DeliverData *deliverData;
    PumpCheckData *pumpCheckData;
    ZTaxData *zTaxData;
    ZReseiveData *zReseiveData;
    ZBillCountData *zBillCountData;
    ZPaymentData *zPaymentData;
    ZFuelData *zFuelData;
    ZTankData *zTankData;
    ZFPaymentData *zFPaymentData;
    ZPumpData *zPumpData;
  } Data;

protected:
  Type mType;
  int mN; //Порядковый номер в чеке
  Data mData;

  void create(Type t);
  void destroy();
  void assign(const KsefBillItem& src);

public:
  KsefBillItem(Type t);
  KsefBillItem(const KsefBillItem& src);
  KsefBillItem& operator =(const KsefBillItem& src);
  virtual ~KsefBillItem();

  Type type() const {return mType;}
  int N() const {return mN;}
  bool assign(QDomElement& element);

  CommentData* commentData() {return type() == Comment ? mData.commentData : NULL;}
  SaleData* saleData() {return type() == Sale ? mData.saleData : NULL;}
  DiscountData* discountData() {return type() == Discount ? mData.discountData : NULL;}
  SumData* sumData() {return type() == Sum ? mData.sumData : NULL;}
  TaxData* taxData() {return type() == Tax ? mData.taxData : NULL;}
  SaleFuelData* saleFuelData() {return type() == SaleFuel ? mData.saleFuelData : NULL;}
  PaymentData* paymentData() {return type() == Payment ? mData.paymentData : NULL;}
  ReseiveData* reseiveData() {return type() == Receive ? mData.receiveData : NULL;}
  DeliverData* deliverData() {return type() == Deliver ? mData.deliverData : NULL;}
  PumpCheckData* pumpCheckData() {return type() == PumpCheck ? mData.pumpCheckData : NULL;}
  ZTaxData* zTaxData() {return type() == ZTax ? mData.zTaxData : NULL;}
  ZReseiveData* zReseiveData() {return type() == ZReceive ? mData.zReseiveData : NULL;}
  ZBillCountData* zBillCountData() {return type() == ZBillCount ? mData.zBillCountData : NULL;}
  ZPaymentData* zPaymentData() {return type() == ZPayment ? mData.zPaymentData : NULL;}
  ZFuelData* zFuelData() {return type() == ZFuel ? mData.zFuelData : NULL;}
  ZTankData* zTankData() {return type() == ZTank ? mData.zTankData : NULL;}
  ZFPaymentData* zFPaymentData() {return type() == ZFPayment ? mData.zFPaymentData : NULL;}
  ZPumpData* zPumpData() {return type() == ZPump ? mData.zPumpData : NULL;}

  //const version
  const CommentData* commentData() const {return type() == Comment ? mData.commentData : NULL;}
  const SaleData* saleData() const {return type() == Sale ? mData.saleData : NULL;}
  const DiscountData* discountData() const {return type() == Discount ? mData.discountData : NULL;}
  const SumData* sumData() const {return type() == Sum ? mData.sumData : NULL;}
  const TaxData* taxData() const {return type() == Tax ? mData.taxData : NULL;}
  const SaleFuelData* saleFuelData() const {return type() == SaleFuel ? mData.saleFuelData : NULL;}
  const PaymentData* paymentData() const {return type() == Payment ? mData.paymentData : NULL;}
  const ReseiveData* reseiveData() const {return type() == Receive ? mData.receiveData : NULL;}
  const DeliverData* deliverData() const {return type() == Deliver ? mData.deliverData : NULL;}
  const PumpCheckData* pumpCheckData() const {return type() == PumpCheck ? mData.pumpCheckData : NULL;}
  const ZTaxData* zTaxData() const {return type() == ZTax ? mData.zTaxData : NULL;}
  const ZReseiveData* zReseiveData() const {return type() == ZReceive ? mData.zReseiveData : NULL;}
  const ZBillCountData* zBillCountData() const {return type() == ZBillCount ? mData.zBillCountData : NULL;}
  const ZPaymentData* zPaymentData() const {return type() == ZPayment ? mData.zPaymentData : NULL;}
  const ZFuelData* zFuelData() const {return type() == ZFuel ? mData.zFuelData : NULL;}
  const ZTankData* zTankData() const {return type() == ZTank ? mData.zTankData : NULL;}
  const ZFPaymentData* zFPaymentData() const {return type() == ZFPayment ? mData.zFPaymentData : NULL;}
  const ZPumpData* zPumpData() const {return type() == ZPump ? mData.zPumpData : NULL;}
};

#endif /*KSEF_DOCUMENT_H_1453124610*/

