qx.Class.define("cashserver.StoreForm",
{
  extend : qx.ui.tabview.Page,

  members: {
    _tabView : null,
    _grpPage : null,
    _dptPage : null,
    _pluPage : null,
    _prcPage : null,
    _barPage : null,
    _emtPage : null,
    _cntPage : null,

    queryStart : function()
    {
      var selection = this._tabView.getSelection()[0];
      selection.queryStart();
    },

    __none : null

  },


  construct : function()
  {
    this.base(arguments, this.tr("Store"), "resource/cashserver/store.png");
    this.setLayout(new qx.ui.layout.VBox());

    var tabView = this._tabView = new qx.ui.tabview.TabView();

    this._grpPage = new cashserver.GroupPage();
    tabView.add(this._grpPage);

    this._dptPage = new cashserver.DeptPage();
    tabView.add(this._dptPage);

    this._pluPage = new cashserver.PluPage();
    tabView.add(this._pluPage);

    this._prcPage = new cashserver.PricePage();
    tabView.add(this._prcPage);

    this._barPage = new cashserver.BarcodePage();
    tabView.add(this._barPage);

    this._emtPage = new cashserver.EmitentPage();
    tabView.add(this._emtPage);

    this._cntPage = new cashserver.ContractPage();
    tabView.add(this._cntPage);

    this.add(tabView);

    tabView.addListener("changeSelection", function(e)
    {
      var selection = e.getData()[0];

      selection.queryStart();

    }, this);

  }

});

