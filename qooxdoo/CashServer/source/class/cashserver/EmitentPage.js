qx.Class.define("cashserver.EmitentPage",
{
  extend : cashserver.SingleTablePage,

  construct : function()
  {
    this.base(arguments, this.tr("Emitents"), "resource/cashserver/emitent.png");
    this.setColumns([
      this.tr("Code"),
      this.tr("Flags"),
      this.tr("Name")
    ]);
    this.setFields([
      "C",
      "F",
      "NM"
    ]);
    this.setGetQuery("EMT");

  }

});
