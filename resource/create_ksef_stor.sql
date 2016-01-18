#----------------------------------------------------------------------------
##
# SQL to create KSEF storage.
#
# (C) T&T team, Kiev, Ukraine 2016.<br>
# started 18.01.2016  8:50:18<br>
##
#----------------------------------------------------------------------------
#==Таблица ЭККА==
CREATE TABLE IF NOT EXISTS CR (
CR_ID     INTEGER PRIMARY KEY AUTOINCREMENT,  # Код ЭККА
ZN        VARCHAR(16) # Заводской номер
);

#==Таблица документов TAG_DAT==
CREATE TABLE IF NOT EXISTS TAG_DAT(
DAT_ID INTEGER PRIMARY KEY AUTOINCREMENT, # Код документа
CR_ID  INTEGER, # Код ЭККА
DI     INTEGER, # Код документа в хранилище КСЕФ
ZN     VARCHAR(16), # Заводской номер
FN     VARCHAR(16), # фискальный номер
TN     VARCHAR(16), # ИНН
TS     DATETIME, # Время создания документа
EXT    INTEGER, # Признак обработки сохраненного XML
XML    BLOB     # Полный XML в кодировке UTF-8
);

#== Таблица чеков TAG_C ==
CREATE TABLE IF NOT EXISTS TAG_C(
C_ID   INTEGER PRIMARY KEY AUTOINCREMENT, # Код чека
DAT_ID INTEGER, # Код документа
T      INTEGER  # Тип чека: 0-чек продажи, 1-чек выплаты, 2-служебный чек, 3-чек приема топлива, 100-Z-отчет
);

#==Таблица позиций чков/отчетов ITEMS==
CREATE TABLE IF NOT EXISTS ITEMS(
ITEM_ID   INTEGER PRIMARY KEY AUTOINCREMENT, # Код позиции
C_ID      INTEGER, # Код чека
DAT_ID    INTEGER, # Код документа
N         INTEGER, # Порядковый номер операции в чеке
T         INTEGER  # Код типа операции
);

#Коды типа операаций:
#Для чека
#0 - Запис «Продаж/виплата товару»:
#1 - Запис «Коментар»:
#2 - Запис «Знижка/націнка»:
#3 - Запис «Сума чека»:
#4 - Начисление налогов
#5 - Запис «Продаж/повернення палива»:
#6 - Запис «Оплата»:
#7 - Запис службового внесення або службової видачі:
#8 - Запис приймання пального:
#9 - Запис повірки ПРК:

#Для отчета
#100 - Запис інформації по податковим ставкам
#101 - Запис інформації по внесенню та винесенню грошей (готівка)
#102 - Кол. чеков оплат/выплат
#103 - Запис інформації оберту по типу оплати
#104 - Запис інформації по типу палива
#105 - Запис інформації по резервуару
#106 - Запис інформації по резервуару


#==Таблица продаж/отмен товаров TAG_P==
CREATE TABLE IF NOT EXISTS TAG_P(
P_ID      INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
N         INTEGER, # Порядковый номер операции в чеке
CNC       BOOL,    # Признак отмены операции
С         INTEGER, # Код товара
CD        VARCHAR(16), # Штрихкод товара
NM        VARCHAR(32), # Название товара
SM        INTEGER, #  Сумма операции (копейки)
Q         INTEGER, #  Количество (в 0.001 единицах)
PRC       INTEGER, #  Цена товара (копейки)
TX        INTEGER, #  Обозначение налога
TX2       INTEGER, #  Обозначение 2-го налога
RT        INTEGER  #  Тип выплаты
);

#==Таблица скидок/наценок TAG_D==
CREATE TABLE IF NOT EXISTS TAG_D(
D_ID      INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
N         INTEGER, # Порядковый номер операции в чеке
CNC       BOOL,    # Признак отмены операции
S         BOOL,    # Признак наценки
TR        INTEGER, # Тип использования: 0-позиция, 1-промеж.сумма
TY        INTEGER, # Тип 0 - сумма, 1 - проценты
ST        INTEGER, # Сумма, на кот насчитывается (копейки)
PR        INTEGER, # Процент (в 0.01%)
SM        INTEGER  # Сумма операции (копейки)
);

#==Таблица вносов/выноса денег TAG_IO==
CREATE TABLE IF NOT EXISTS TAG_IO(
IO_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
O         BOOL,    # Признак выноса
SM        INTEGER  # Сумма операции (копейки)
);

#==Таблица оплат чеков TAG_M==
CREATE TABLE IF NOT EXISTS TAG_M(
M_ID      INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
N         INTEGER, # Порядковый номер операции в чеке
CNC       BOOL,    # Признак отмены операции
T         INTEGER, # Код дормы оплаты. 0-Наличные
NM        VARCHAR(16), # Название формы оплаты
SM        INTEGER, # Сумма операции (копейки)
RM        INTEGER, # Сумма здачи (копейки)
RRN       VARCHAR(32) # Код транзакции от платежного терминала
);

#==Таблица комментариев в чеках TAG_L==
CREATE TABLE IF NOT EXISTS TAG_L(
M_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID  INTEGER, # Код позиции
N        INTEGER, # Порядковый номер операции в чеке
TXT      VARCHAR(16) # Текст комментария
);

#==Таблица сумм чекоа TAG_E==
CREATE TABLE IF NOT EXISTS TAG_E(
E_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID  INTEGER, # Код позиции
N        INTEGER, # Порядковый номер операции в чеке
"NO"     INTEGER, # Номер фискального чека
SM       INTEGER, # Общая сумма чека (копейки)
SE       INTEGER, # Общая сумма чека без учета налогов (копейки)
CS       INTEGER, # Код кассира
TS       DATETIME # Время создания документа
);

#==Таблица налогообложения в чеке TAG_TX==
CREATE TABLE IF NOT EXISTS TAG_TX(
TX_ID    INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID  INTEGER, # Код позиции
TX       INTEGER, # Обозначение налога
TXPR     INTEGER, # Ставка в процентах (в 0.01%)
TXSM     INTEGER, # Сумма налога (копейки)
DTPR     INTEGER, # Ставка доп. сбора в процентах (в 0.01%)
DTSM     INTEGER, # Сумма доп. сбора  (копейки)
TXTY     BOOL,    # Признак налога, не включенного в стоимость
TXAL     INT      # Алгоритм начисления сбора
);

#=Таблица продаж/отмен топлива TAG_PP==
CREATE TABLE IF NOT EXISTS TAG_PP(
PP_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
N         INTEGER, # Порядковый номер операции в чеке
CNC       BOOL,    # Признак отмены операции
С         INTEGER, # Код товара
CD        VARCHAR(16), # Штрихкод товара
NM        VARCHAR(32), # Название товара
OV        VARCHAR(16), # Единица изменения
PRK       INTEGER,     # Номер ТРК
KRK       INTEGER,     # Номер крана
SM        INTEGER, # Сумма операции (копейки)
Q         INTEGER, #  Количество (в 0.001 единицах)
PRC       INTEGER, #  Цена товара (копейки)
TX        INTEGER, #  Обозначение налога
TX2       INTEGER  #  Обозначение 2-го налога
);

#=Таблица поверочных отпусков TAG_PV==
CREATE TABLE IF NOT EXISTS TAG_PV(
PV_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
С         INTEGER, # Код товара
NM        VARCHAR(32), # Название товара
OV        VARCHAR(16), # Единица изменения
PRK       INTEGER, # Номер ТРК
KRK       INTEGER, # Номер крана
Q         INTEGER  # Количество (в 0.001 единицах)
);

#==Таблица приходов топлива TAG_IF==
CREATE TABLE IF NOT EXISTS TAG_IF(
IF_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
С         INTEGER, # Код товара
NM        VARCHAR(32), # Название товара
OV        VARCHAR(16), # Единица изменения
NR        INTEGER,  # Номер резервуара
Q         INTEGER  #  Количество (в 0.001 единицах)
);

#==Таблица информации по налоговым группам (Z-отчет) TAG_TXSZ==
CREATE TABLE IF NOT EXISTS TAG_TXSZ(
TXSZ_ID  INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID  INTEGER, # Код позиции
TX       INTEGER, # Обозначение налога
TS       DATE,    # Дата программирования налога
TXPR     INTEGER, # Ставка в процентах (в 0.01%)
TXI      INTEGER, # Сумма налога по продажам(копейки)
TXO      INTEGER, # Сумма налога по выплатам (копейки)
DTPR     INTEGER, # Ставка доп. сбора в процентах (в 0.01%)
DTI      INTEGER, # Сумма доп. сбора по продажам (копейки)
DTO      INTEGER, # Сумма доп. сбора по выплатам (копейки)
TXTY     BOOL,    # Признак налога, не включенного в стоимость
TXAL     INTEGER, # Алгоритм начисления сбора
SMI      INTEGER, # Оборот по продажам (копейки)
SMO      INTEGER  # Оборот по выплатам (копейки)
);

#==Таблица информации по оплатам (Z-отчет) TAG_MZ==
CREATE TABLE IF NOT EXISTS TAG_MZ(
MZ_ID    INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID  INTEGER, # Код позиции
T        INTEGER, # Код дормы оплаты. 0-Наличные
NM       VARCHAR(16), # Название формы оплаты
SMI      INTEGER, # Сумма продаж (копейки)
SMO      INTEGER  # Сумма выплат (копейки)
);

#==Таблица сумм вносов/выноса денег (Z-отчет) TAG_IOZ==
CREATE TABLE IF NOT EXISTS TAG_IOZ(
IOZ_ID    INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
SMI       INTEGER, #  Сумма вносов (копейки)
SMO       INTEGER  #  Сумма выносов (копейки)
);

#==Таблица количаства чеков продаж/выплат (Z-отчет) TAG_NCZ==
CREATE TABLE IF NOT EXISTS TAG_NCZ(
NCZ_ID    INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
NI        INTEGER, # Кол. чеков оплат
"NO"      INTEGER  # Кол. чеков выплат
);

#==Таблица суммарной информации по типам топлива (Z-отчет) TAG_FZ==
CREATE TABLE IF NOT EXISTS TAG_FZ(
FZ_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
С         INTEGER, # Код товара
NM        VARCHAR(32), # Название товара
"IF"      INTEGER, #  Количество принятого топлива в мл
"OF"      INTEGER, #  Количество отпущенного топлива в мл
OFP       INTEGER, #  Количество отпущенного при поверке ТРК топлива в мл
SMI       INTEGER, #  Сумма принятых денег(копейки)
SF        INTEGER  #  Остаток топлива в мл
);

#==Таблица суммарной информации по продажам формам оплат типаов топлива (Z-отчет)
CREATE TABLE IF NOT EXISTS TAG_FZ(
FZ_ID     INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
С         INTEGER, # Код товара
T         INTEGER, # Код дормы оплаты. 0-Наличные
NM        VARCHAR(16), # Название формы оплаты
SM        INTEGER, # Сумма продаж (копейки)
"OF"      INTEGER  # Отпущено (мл)
);

#==Таблица суммарной информации по резервуарам (Z-отчет) TAG_SMIZ==
CREATE TABLE IF NOT EXISTS TAG_SMIZ(
SMIZ_ID   INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
NR        INTEGER, # Номер резервуара
С         INTEGER, # Код товара
SF        INTEGER  # Остаток топлива в мл
);

#==Таблица ТРК (Z-отчет) TAG_NPSZ==
CREATE TABLE IF NOT EXISTS TAG_NPSZ(
NPSZ_NPSZ INTEGER PRIMARY KEY AUTOINCREMENT, # Код
ITEM_ID   INTEGER, # Код позиции
NP        INTEGER, # Номер ТРК
NR        INTEGER  # Номер резервуара
);

