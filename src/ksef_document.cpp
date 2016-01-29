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
#include "ksef_document.h"
/*----------------------------------------------------------------------------*/
KsefDocument :: KsefDocument()
{
  mTime = QDateTime::currentDateTime();
}
/*----------------------------------------------------------------------------*/
KsefDocument :: ~KsefDocument()
{
}
/*----------------------------------------------------------------------------*/
QDateTime KsefDocument :: ts2time(const QString& src)
{
  QDate d = QDate(src.mid(0, 4).toInt(), src.mid(4, 2).toInt(), src.mid(6, 2).toInt());
  QTime t = QTime(src.mid(8, 2).toInt(), src.mid(10, 2).toInt(), src.mid(12, 2).toInt());
  return QDateTime(d, t);
}
/*----------------------------------------------------------------------------*/
bool KsefDocument :: assign(QDomElement& e, bool deep)
{
  /*
  QDomNode parent = e.parentNode();
  while(!parent.parentNode().isDocument() && !parent.isNull())
    parent = parent.parentNode();
  */

  //if(mDoc.isNull() || !parent.isDocument() || parent.toDocument() != mDoc)
  {
    mDoc.clear();
    mDoc.appendChild(mDoc.importNode(e, true));
  }
  if(e.tagName() != "DAT")
    return false;
  mSerial = e.attribute("ZN");
  mTax = e.attribute("TN");
  mFiscal = e.attribute("FN");
  mDi = e.attribute("DI").toInt();

  QDomElement ts = e.firstChildElement("TS");
  if(!ts.isNull())
  {
    mTime = ts2time(ts.text());
  }

  if(mSerial.isEmpty())
    return false;

  if(deep)
  {
    QDomElement c = e.firstChildElement();
    while(!c.isNull())
    {
      if(c.tagName() == "C")
      {
        int t = c.attribute("T", "-1").toInt();
        if(t >= 0)
        {
          KsefBill bill((KsefBill::Type) t);
          if(bill.assign(c))
            mBills.append(bill);
        }
      }

      if(c.tagName() == "Z")
      {
        KsefBill bill(KsefBill::Report);
        if(bill.assign(c))
          mBills.append(bill);
      }

      c = c.nextSiblingElement();
    }
  }

  return true;
}
/*----------------------------------------------------------------------------*/
bool KsefDocument :: parse(const QString& xml)
{
  mDoc.clear();
  if(!mDoc.setContent(xml))
    return false;
  QDomElement e = mDoc.firstChildElement("DAT");
  return assign(e, true);
}
/*----------------------------------------------------------------------------*/
QString KsefDocument :: toXmlString() const
{
  return mDoc.toString(0).replace(QChar('\n'), " ");
}
/*----------------------------------------------------------------------------*/
//XmlBill
/*----------------------------------------------------------------------------*/
KsefBill :: KsefBill(Type t)
{
  mType = t;
  switch(t)
  {
  case Sale:
  case Payout:
  case Service:
  case Deliver:
  case Report:
    break;
  default:
    mType = Unknown;
    break;
  }
}
/*----------------------------------------------------------------------------*/
KsefBill :: ~KsefBill()
{
}
/*----------------------------------------------------------------------------*/
bool KsefBill :: assign(QDomElement& element)
{
  QDomElement e = element.firstChildElement();
  while(!e.isNull())
  {
    if(type() == Report)
    {
      if(e.tagName() == "TXS")
      {
        KsefBillItem item(KsefBillItem::ZTax);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "M")
      {
        KsefBillItem item(KsefBillItem::ZPayment);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "IO")
      {
        KsefBillItem item(KsefBillItem::ZReceive);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "F")
      {
        KsefBillItem item(KsefBillItem::ZFuel);
        if(item.assign(e))
          mItems.append(item);
        QDomElement ee = e.firstChildElement("SF");
        while(!ee.isNull())
        {
          KsefBillItem item(KsefBillItem::ZTank);
          if(item.assign(ee))
            mItems.append(item);
          QDomElement eee = ee.firstChildElement("NPS");
          while(!eee.isNull())
          {
            eee.setAttribute("NR", ee.attribute("NR"));
            KsefBillItem item(KsefBillItem::ZPump);
            if(item.assign(eee))
              mItems.append(item);
            eee = eee.nextSiblingElement("NPS");
          }
          ee = ee.nextSiblingElement("SF");
        }

        ee = e.firstChildElement("SMI");
        while(!ee.isNull())
        {
          ee.setAttribute("C", e.attribute("C"));
          KsefBillItem item(KsefBillItem::ZFPayment);
          if(item.assign(ee))
            mItems.append(item);
          ee = ee.nextSiblingElement("SMI");
        }
      }
    }
    else
    {
      if(e.tagName() == "P")
      {
        KsefBillItem item(KsefBillItem::Sale);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "PP")
      {
        KsefBillItem item(KsefBillItem::SaleFuel);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "D" || e.tagName() == "M")
      {
        KsefBillItem item(KsefBillItem::Discount);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "I" || e.tagName() == "O")
      {
        KsefBillItem item(KsefBillItem::Receive);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "L")
      {
        KsefBillItem item(KsefBillItem::Comment);
        if(item.assign(e))
          mItems.append(item);
      }
      if(e.tagName() == "E")
      {
        KsefBillItem item(KsefBillItem::Sum);
        if(item.assign(e))
          mItems.append(item);

        QDomElement ee = e.firstChildElement("TX");
        while(!ee.isNull())
        {
          KsefBillItem item(KsefBillItem::Tax);
          if(item.assign(ee))
            mItems.append(item);

          ee = ee.nextSiblingElement("TX");
        }
      }
      if(e.tagName() == "IF")
      {
        KsefBillItem item(KsefBillItem::Deliver);
        if(item.assign(e))
          mItems.append(item);
      }
    }
    e = e.nextSiblingElement();
  }
  return true;
}
/*----------------------------------------------------------------------------*/
//KsefBillItem
/*----------------------------------------------------------------------------*/
KsefBillItem :: KsefBillItem(Type t)
{
  create(t);
}
/*----------------------------------------------------------------------------*/
KsefBillItem :: KsefBillItem(const KsefBillItem& src)
{
  create(src.type());
  assign(src);
}
/*----------------------------------------------------------------------------*/
KsefBillItem& KsefBillItem :: operator =(const KsefBillItem& src)
{
  assign(src);
  return *this;
}
/*----------------------------------------------------------------------------*/
KsefBillItem :: ~KsefBillItem()
{
  destroy();
}
/*----------------------------------------------------------------------------*/
void KsefBillItem :: create(Type t)
{
  mType = t;
  switch(t)
  {
  case Sale:
    mData.saleData = new SaleData;
    break;
  case Comment:
    mData.commentData = new CommentData;
    break;
  case Discount:
    mData.discountData = new DiscountData;
    break;
  case Sum:
    mData.sumData = new SumData;
    break;
  case Tax:
    mData.taxData = new TaxData;
    break;
  case SaleFuel:
    mData.saleFuelData = new SaleFuelData;
    break;
  case Payment:
    mData.paymentData = new PaymentData;
    break;
  case Receive:
    mData.receiveData = new ReseiveData;
    break;
  case Deliver:
    mData.deliverData = new DeliverData;
    break;
  case PumpCheck:
    mData.pumpCheckData = new PumpCheckData;
    break;
  case ZTax:
    mData.zTaxData = new ZTaxData;
    break;
  case ZReceive:
    mData.zReseiveData = new ZReseiveData;
    break;
  case ZBillCount:
    mData.zBillCountData = new ZBillCountData;
    break;
  case ZPayment:
    mData.zPaymentData = new ZPaymentData;
    break;
  case ZFuel:
    mData.zFuelData = new ZFuelData;
    break;
  case ZTank:
    mData.zTankData = new ZTankData;
    break;
  case ZFPayment:
    mData.zFPaymentData = new ZFPaymentData;
    break;
  case ZPump:
    mData.zPumpData = new ZPumpData;
    break;
  default:
    mData.p = NULL;
    mType = Unknown;
    break;
  }
}
/*----------------------------------------------------------------------------*/
void KsefBillItem :: destroy()
{
  switch(type())
  {
  case Sale:
    delete mData.saleData;
    break;
  case Comment:
    delete mData.commentData;
    break;
  case Discount:
    delete mData.discountData;
    break;
  case Sum:
    delete mData.sumData;
    break;
  case Tax:
    delete mData.taxData;
    break;
  case SaleFuel:
    delete mData.saleFuelData;
    break;
  case Payment:
    delete mData.paymentData;
    break;
  case Receive:
    delete mData.receiveData;
    break;
  case Deliver:
    delete mData.deliverData;
    break;
  case PumpCheck:
    delete mData.pumpCheckData;
    break;
  case ZTax:
    delete mData.zTaxData;
    break;
  case ZReceive:
    delete mData.zReseiveData;
    break;
  case ZBillCount:
    delete mData.zBillCountData;
    break;
  case ZPayment:
    delete mData.zPaymentData;
    break;
  case ZFuel:
    delete mData.zFuelData;
    break;
  case ZTank:
    delete mData.zTankData;
    break;
  case ZFPayment:
    delete mData.zFPaymentData;
    break;
  case ZPump:
    delete mData.zPumpData;
    break;
  default:
    break;
  }
  mData.p = NULL;
  mType = Unknown;
}
/*----------------------------------------------------------------------------*/
void KsefBillItem :: assign(const KsefBillItem& src)
{
  if(type() != src.type())
  {
    destroy();
    create(src.type());
  }
  mN = src.N();
  switch(type())
  {
  case Sale:
    *mData.saleData = *src.saleData();
    break;
  case Comment:
    *mData.commentData = *src.commentData();
    break;
  case Discount:
    *mData.discountData = *src.discountData();
    break;
  case Sum:
    *mData.sumData = *src.sumData();
    break;
  case Tax:
    *mData.taxData = *src.taxData();
    break;
  case SaleFuel:
    *mData.saleFuelData = *src.saleFuelData();
    break;
  case Payment:
    *mData.paymentData = *src.paymentData();
    break;
  case Receive:
    *mData.receiveData = *src.reseiveData();
    break;
  case Deliver:
    *mData.deliverData = *src.deliverData();
    break;
  case PumpCheck:
    *mData.pumpCheckData = *src.pumpCheckData();
    break;
  case ZTax:
    *mData.zTaxData = *src.zTaxData();
    break;
  case ZReceive:
    *mData.zReseiveData = *src.zReseiveData();
    break;
  case ZBillCount:
    *mData.zBillCountData = *src.zBillCountData();
    break;
  case ZPayment:
    *mData.zPaymentData = *src.zPaymentData();
    break;
  case ZFuel:
    *mData.zFuelData = *src.zFuelData();
    break;
  case ZTank:
    *mData.zTankData = *src.zTankData();
    break;
  case ZFPayment:
    *mData.zFPaymentData = *src.zFPaymentData();
    break;
  case ZPump:
    *mData.zPumpData = *src.zPumpData();
    break;
  default:
    break;
  }
}
/*----------------------------------------------------------------------------*/
bool KsefBillItem :: assign(QDomElement& element)
{
  mN = element.attribute("N").toInt();
  switch(type())
  {
  case Sale:
  {
    SaleData *data = saleData();
    data->C = element.attribute("C").toInt();
    data->CD = element.attribute("CD");
    data->NM = element.attribute("NM");
    data->PRC = element.attribute("PRC").toInt();
    data->Q = element.attribute("Q").toInt();
    data->RT = element.attribute("RT").toInt();
    data->SM = element.attribute("SM").toInt();
    data->TX = element.attribute("TX").toInt();
    data->TX2 = element.attribute("TX2").toInt();
    break;
  }
  case Comment:
  {
    CommentData *data = commentData();
    data->TXT = element.text();
    break;
  }
  case Discount:
  {
    DiscountData *data = discountData();
    data->PR = element.attribute("PR").toInt();
    data->S = element.tagName() == "M";
    data->SM = element.attribute("SM").toInt();
    data->ST = element.attribute("ST").toInt();
    data->TR = element.attribute("TR").toInt();
    data->TY = element.attribute("TY").toInt();
    break;
  }
  case Sum:
  {
    SumData *data = sumData();
    data->CS = element.attribute("CS").toInt();
    data->NO = element.attribute("NO").toInt();
    data->SE = element.attribute("SE").toInt();
    data->SM = element.attribute("SM").toInt();
    //YYYYMMDDhhmmss
    QString t = element.attribute("TS");
    if(t.length() == 14)
    {
      int Y; int M; int D;
      int h, m, s;
      Y = t.left(4).toInt();
      M = t.mid(4, 2).toInt();
      D = t.mid(6, 2).toInt();
      h = t.mid(8, 2).toInt();
      m = t.mid(10, 2).toInt();
      s = t.mid(12, 2).toInt();

      data->TS.setDate(QDate(Y, M, D));
      data->TS.setTime(QTime(h, m, s, 0));
    }
    break;
  }
  case Tax:
  {
    TaxData *data = taxData();
    data->DTPR = element.attribute("DTPR").toInt();
    data->DTSM = element.attribute("DTSM").toInt();
    data->TX = element.attribute("TX").toInt();
    data->TXAL = element.attribute("TXAL").toInt();
    data->TXPR = element.attribute("TXPR").toInt();
    data->TXSM = element.attribute("TXSM").toInt();
    data->TXTY = element.attribute("TXTY").toInt();
    break;
  }
  case SaleFuel:
  {
    SaleFuelData *data = saleFuelData();
    data->C = element.attribute("C").toInt();
    data->CD = element.attribute("CD");
    data->KRK = element.attribute("KRK").toInt();
    data->NM = element.attribute("NM");
    data->OV = element.attribute("OV");
    data->PRC = element.attribute("PRC").toInt();
    data->PRK = element.attribute("PRK").toInt();
    data->Q = element.attribute("Q").toInt();
    data->SM = element.attribute("SM").toInt();
    data->TX = element.attribute("TX").toInt();
    data->TX2 = element.attribute("TX2").toInt();
    break;
  }
  case Payment:
  {
    PaymentData *data = paymentData();
    data->NM = element.attribute("NM");
    data->RM = element.attribute("RM").toInt();
    data->RRN = element.attribute("RRN");
    data->SM = element.attribute("SM").toInt();
    data->T = element.attribute("T").toInt();
    break;
  }
  case Receive:
  {
    ReseiveData *data = reseiveData();
    data->O = element.text() == "O";
    data->SM = element.attribute("SM").toInt();
    break;
  }
  case Deliver:
  {
    DeliverData *data = deliverData();
    data->C = element.attribute("C").toInt();
    data->NM = element.attribute("NM");
    data->NR = element.attribute("NR").toInt();
    data->OV = element.attribute("OV");
    data->Q = element.attribute("Q").toInt();
    break;
  }
  case PumpCheck:
  {
    PumpCheckData *data = pumpCheckData();
    data->C = element.attribute("C").toInt();
    data->KRK = element.attribute("KRK").toInt();
    data->NM = element.attribute("NM");
    data->OV = element.attribute("OV");
    data->PRK = element.attribute("PRK").toInt();
    data->Q = element.attribute("Q").toInt();
    break;
  }
  case ZTax:
  {
    ZTaxData *data = zTaxData();
    data->DTI = element.attribute("DTI").toInt();
    data->DTO = element.attribute("DTO").toInt();
    data->DTPR = element.attribute("DTPR").toInt();
    data->SMI = element.attribute("SMI").toInt();
    data->SMO = element.attribute("SMO").toInt();
    data->TX = element.attribute("TX").toInt();
    data->TXAL = element.attribute("TXAL").toInt();
    data->TXI = element.attribute("TXI").toInt();
    data->TXO = element.attribute("TXO").toInt();
    data->TXPR = element.attribute("TXPR").toInt();
    data->TXTY = element.attribute("TXTY").toInt();
    //YYYYMMDD
    QString t = element.attribute("TS");
    if(t.length() == 8)
    {
      int Y; int M; int D;
      Y = t.left(4).toInt();
      M = t.mid(4, 2).toInt();
      D = t.mid(6, 2).toInt();

      data->TS = QDate(Y, M, D);
    }
    break;
  }
  case ZReceive:
  {
    ZReseiveData *data = zReseiveData();
    data->SMI = element.attribute("SMI").toInt();
    data->SMO = element.attribute("SMO").toInt();
    break;
  }
  case ZBillCount:
  {
    ZBillCountData *data  = zBillCountData();
    data->NI = element.attribute("NI").toInt();
    data->NO = element.attribute("NO").toInt();
    break;
  }
  case ZPayment:
  {
    ZPaymentData *data = zPaymentData();
    data->NM = element.attribute("NM");
    data->SMI = element.attribute("SMI").toInt();
    data->SMO = element.attribute("SMO").toInt();
    data->T = element.attribute("T").toInt();
    break;
  }
  case ZFuel:
  {
    ZFuelData *data = zFuelData();
    data->C = element.attribute("C").toInt();
    data->IF = element.attribute("IF").toInt();
    data->NM = element.attribute("NM");
    data->OF = element.attribute("OF").toInt();
    data->OFP = element.attribute("OFP").toInt();
    data->SF = element.attribute("SF").toInt();
    data->SMI = element.attribute("SMI").toInt();
    break;
  }
  case ZTank:
  {
    ZTankData *data = zTankData();
    data->C = element.attribute("C").toInt();
    data->NR = element.attribute("NR").toInt();
    data->SF = element.attribute("SF").toInt();
    break;
  }
  case ZFPayment:
  {
    ZFPaymentData *data = zFPaymentData();
    data->C = element.attribute("C").toInt();
    data->NM = element.attribute("NM");
    data->OF = element.attribute("OF").toInt();
    data->SM = element.attribute("SM").toInt();
    data->T = element.attribute("T").toInt();
    break;
  }
  case ZPump:
  {
    ZPumpData *data = zPumpData();
    data->NP = element.attribute("NP").toInt();
    data->NR = element.attribute("NR").toInt();
    break;
  }
  default:
    return false;
  }
  return true;
}
/*----------------------------------------------------------------------------*/
