qx.Class.define("cashserver.PricePage",
{
  extend : cashserver.SingleTablePage,

  construct : function()
  {
    this.base(arguments, this.tr("Product prices"), "resource/cashserver/price.png");
    this.setColumns([
      this.tr("Code"),
      this.tr("Price")
    ]);
    this.setFields([
      "C",
      "PRC"
    ]);
    this.setGetQuery("PRC");

  }

});
