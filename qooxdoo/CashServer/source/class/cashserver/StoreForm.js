qx.Class.define("cashserver.StoreForm",
{
  extend : qx.ui.tabview.Page,

  construct : function()
  {
    this.base(arguments, this.tr("Store"), "resource/cashserver/store.png");

    var layout = new qx.ui.layout.Grid(0, 0);
    this.setLayout(layout);

    this.add(new qx.ui.basic.Label(this.tr("Store")), {row: 0, column: 0});

  }

});

