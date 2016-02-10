qx.Class.define("cashserver.BarcodePage",
{
  extend : cashserver.SingleTablePage,

  construct : function()
  {
    this.base(arguments, this.tr("Barcodes"), "resource/cashserver/barcode.png");
    this.setColumns([
      this.tr("Barcode"),
      this.tr("Product")
    ]);
    this.setFields([
      "CD",
      "C"
    ]);
    this.setGetQuery("BAR");

  }

});
