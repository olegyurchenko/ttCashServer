/*----------------------------------------------------------------------------*/
/**
* @pkg PluPage
*/
/**
* Page for edit products.
*
* Combine 2 table: PLU(products) & PRC(prices) <br>
* (C) T&T team, Kiev, Ukraine 2016.<br>
* started 13.01.2016 12:10:22<br>
* @pkgdoc PluPage
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
qx.Class.define("cashserver.PluPage",
{
  extend : cashserver.SingleTablePage,

  members :
  {
    ID_COLUMN : 0,
    PRC_COLUMN : 2,
    GRP_COLUMN : 3,
    DPT_COLUMN : 4,

    _modifiedData : {},

    /*------------------------------------------------------------------------*/
    queryStart : function()
    {
      if(this._dataReady)
        return;
      this._query(this._getQuery, this._onPluReady, this);
    },
    /*------------------------------------------------------------------------*/
    _onPluReady : function(data)
    {
      this._onDataReady(data);
      this._query("PRC", this._onPrcReady, this);
    },
    /*------------------------------------------------------------------------*/
    _onPrcReady : function(data)
    {
      var model = this._tableModel;
      var dataLen = data.length;
      var pluId = [];
      var rowCount = model.getRowCount();
      for(var i = 0; i < rowCount; i++)
        pluId.push(model.getValue(this.ID_COLUMN, i));
      //this.debug(pluId);
      this._updateActive = true; //Prevent onChange event
      for(var i = 0; i < dataLen; i++)
      {
        var prc = data[i].PRC;
        var row = pluId.indexOf(data[i].C);
        //this.debug(data[i].C, row, prc);
        model.setValue(this.PRC_COLUMN, row, prc);
      }
      this._updateActive = false;
    },
    /*------------------------------------------------------------------------*/
    _convertFromUi : function(data)
    {
      data[this.GRP_COLUMN] --;
      data[this.DPT_COLUMN] --;
      return data;
    },
    /*------------------------------------------------------------------------*/
    _convertToUi : function(data)
    {
      data[this.GRP_COLUMN] ++;
      data[this.DPT_COLUMN] ++;
      return data;
    },
    /*------------------------------------------------------------------------*/
    _prepareCommitData : function()
    {
      var data = [];
      for(var key in this._modifiedData)
      {
        var row = this._modifiedData[key];
        var plu = {};
        var prc = {};

        plu["C"] = row.C;
        plu["DEL"] = row.DEL;
        if(!row.DEL)
        {
          plu["NM"] = row.NM;
          plu["GRP"] = row.GRP;
          plu["DPT"] = row.DPT;
          plu["TX"] = row.TX;
          plu["TX2"] = row.TX2;
          plu["DEC"] = row.DEC;
        }
        plu["TABLE"] = "PLU";
        data.push(plu);

        prc["C"] = row.C;
        prc["DEL"] = row.DEL;
        if(!row.DEL)
        {
          prc["PRC"] = row.PRC;
        }
        prc["TABLE"] = "PRC";
        data.push(prc);
      }
      return data;
    },
    /*------------------------------------------------------------------------*/
    __none__ : null
  },
  /*------------------------------------------------------------------------*/
  /**Constructor*/
  //Combine 2 table: PLU & PRC. From PRC use one column PRC
  /*------------------------------------------------------------------------*/
  construct : function()
  {
    this.base(arguments, this.tr("Products"), "resource/cashserver/product.png");
    this.setColumns([
      this.tr("Code"),
      this.tr("Name"),
      this.tr("Price"),
      this.tr("Group"),
      this.tr("Dept"),
      this.tr("Tax group") + " 1",
      this.tr("Tax group") + " 2",
      this.tr("Quantity decimals")
    ]);
    this.setFields([
      "C",
      "NM",
      "PRC",
      "GRP",
      "DPT",
      "TX",
      "TX2",
      "DEC"
    ]);
    this.setGetQuery("PLU");
  }
});
/*----------------------------------------------------------------------------*/
