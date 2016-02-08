qx.Class.define("cashserver.Database", {
  extend: qx.core.Object,
  type : "singleton",
  members: {
    username : null,
    password : null
  },

  construct : function()
  {
    this.base(arguments);
  }

});
