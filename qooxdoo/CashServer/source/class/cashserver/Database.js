/*----------------------------------------------------------------------------*/
/**
* @pkg Database
*/
/**
* Class - singletone for store WebUI data.
*
* (C) T&T team, Kiev, Ukraine 2016.<br>
* started 13.01.2016 11:17:33<br>
* @pkgdoc Database
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
qx.Class.define("cashserver.Database", {
  extend: qx.core.Object,
  type : "singleton",
  members: {
    username : null,
    password : null,
    language : null,

    /*--------------------------------------------------------------------------*/
    /**Load data from cookie */
    /*--------------------------------------------------------------------------*/
    load : function()
    {
      this.language = qx.bom.Cookie.get("language");
      this.username = qx.bom.Cookie.get("user");
      this.password = qx.bom.Cookie.get("pass");
    },

    /*--------------------------------------------------------------------------*/
    /**Save data to cookie */
    /*--------------------------------------------------------------------------*/
    save : function()
    {
      qx.bom.Cookie.set("language", qx.locale.Manager.getInstance().getLanguage(), 30);
      qx.bom.Cookie.set("user", this.username, 30);
      qx.bom.Cookie.set("pass", this.password, 30);
    },

    /*--------------------------------------------------------------------------*/
    /**Delete data from cookie */
    /*--------------------------------------------------------------------------*/
    del : function()
    {
      qx.bom.Cookie.del("language");
      qx.bom.Cookie.del("user");
      qx.bom.Cookie.del("pass");
    },

    __none__ : null
  },

  construct : function()
  {
    this.base(arguments);
  }

});
/*----------------------------------------------------------------------------*/
