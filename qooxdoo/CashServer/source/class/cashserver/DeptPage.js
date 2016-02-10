qx.Class.define("cashserver.DeptPage",
{
  extend : cashserver.SingleTablePage,
  members: {
    _convertFromUi : function(data)
    {
      data[0] --;
      return data;
    },
    _convertToUi : function(data)
    {
      data[0] ++;
      return data;
    },
    __none : null
  },

  construct : function()
  {
    this.base(arguments, this.tr("Departments"), "resource/cashserver/department.png");
    this.setColumns([
      this.tr("Code"),
      this.tr("Name")
    ]);
    this.setFields([
      "C",
      "NM"
    ]);
    this.setGetQuery("DPT");

  }

});
