qx.Class.define("cashserver.ContractPage",
{
  extend : cashserver.SingleTablePage,

  construct : function()
  {
    this.base(arguments, this.tr("Contracts"), "resource/cashserver/contract.png");
    this.setColumns([
      this.tr("Number"),
      this.tr("Name"),
      this.tr("Emitent"),
      this.tr("Pay form"),
      this.tr("Flags"),
      this.tr("Product"),
      this.tr("Balance"),
      this.tr("Price")
    ]);

    this.setFields([
      "PAN",
      "NM",
      "EMT",
      "PF",
      "F",
      "PLU",
      "BAL",
      "PRC"
    ]);
    this.setGetQuery("CNT");

  }

});
