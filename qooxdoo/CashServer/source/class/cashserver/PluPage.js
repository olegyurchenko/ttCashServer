qx.Class.define("cashserver.PluPage",
{
  extend : cashserver.SingleTablePage,

  construct : function()
  {
    this.base(arguments, this.tr("Products"), "resource/cashserver/product.png");
    this.setColumns([
      this.tr("Code"),
      this.tr("Name"),
      this.tr("Group"),
      this.tr("Dept"),
      this.tr("Tax group") + " 1",
      this.tr("Tax group") + " 2",
      this.tr("Quantity decimals")
    ]);
    this.setFields([
      "C",
      "NM",
      "GRP",
      "DPT",
      "TX",
      "TX2",
      "DEC"
    ]);
    this.setGetQuery("PLU");

  }

});
