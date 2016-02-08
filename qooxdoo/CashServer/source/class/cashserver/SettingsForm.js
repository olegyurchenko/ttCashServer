qx.Class.define("cashserver.SettingsForm",
{
  extend : qx.ui.tabview.Page,

  members: {
    _nameLabel : null,
    _passLabel : null,
    _nameEdit : null,
    _passEdit : null,
    _rememberCheck : null,
    _langLabel : null,
    _langModel : null,
    _langCombo : null,
    _langController : null,
    _locales : ["en", "ru", "uk"],
    _isModified : true,

    loadSettings : function() {
      var lang = qx.bom.Cookie.get("language");
      var user = qx.bom.Cookie.get("user");
      var pass = qx.bom.Cookie.get("pass");

      this._nameEdit.setValue(user);
      this._passEdit.setValue(pass);

      var index = this._locales.indexOf(lang);
      if(index >= 0 && index < this._locales.length)
        qx.locale.Manager.getInstance().setLocale(this._locales[index]);

      this._rememberCheck.setValue(lang != null);
      this._isModified = false;
    },

    saveSettings : function() {

      var db = cashserver.Database.getInstance();
      db.username = this._nameEdit.getValue();
      db.password = this._passEdit.getValue();


      if(!this._isModified)
        return;

      if(!this._rememberCheck.getValue())
      {
        qx.bom.Cookie.del("language");
        qx.bom.Cookie.del("user");
        qx.bom.Cookie.del("pass");
      }
      else
      {
        qx.bom.Cookie.set("language", qx.locale.Manager.getInstance().getLanguage(), 30);
        qx.bom.Cookie.set("user", this._nameEdit.getValue(), 30);
        qx.bom.Cookie.set("pass", this._passEdit.getValue(), 30);
      }

      this._isModified = false;
    },

    __none : null
  },

  construct : function()
  {
    this.base(arguments, this.tr("Settings"), "resource/cashserver/settings.png");



    var layout = new qx.ui.layout.Grid(0, 0);
    this.setLayout(layout);

    this.setPadding(10);

    this._nameLabel = new qx.ui.basic.Label(this.tr("User name"));
    this.add(this._nameLabel, {row: 0, column: 0});

    this._nameEdit = new qx.ui.form.TextField();
    this.add(this._nameEdit, {row: 0, column: 1});

    this._passLabel = new qx.ui.basic.Label(this.tr("Password"));
    this.add(this._passLabel, {row: 1, column: 0});

    this._passEdit = new qx.ui.form.PasswordField();
    this.add(this._passEdit, {row: 1, column: 1});


    this._langLabel = new qx.ui.basic.Label(this.tr("Interface language"));
    this.add(this._langLabel, {row: 2, column: 0});

    this._langModel = new qx.data.Array([]);
    this._langModel.push(this.tr("English"));
    this._langModel.push(this.tr("Russian"));
    this._langModel.push(this.tr("Ukrainian"));
    this._langCombo = new qx.ui.form.SelectBox();
    this._langController = new qx.data.controller.List(this._langModel, this._langCombo);

    this.add(this._langCombo, {row:2, column:1});

    this._rememberCheck = new qx.ui.form.CheckBox(this.tr("remember me"));
    this.add(this._rememberCheck, {row:3, column:1});
    //this._rememberCheck.setValue(true);


    var currentLocale = qx.locale.Manager.getInstance().getLanguage();
    var index = this._locales.indexOf(currentLocale);
    if(index >= 0 && index < this._locales.length)
    {
      this._langController.getSelection().setItem(0, this._langModel.getItem(index));
    }


    this._langController.addListener("changeSelection", function()
      {
        var selection = this._langController.getSelection().getItem(0);
        var index = this._langModel.indexOf(selection);
        if(index >= 0 && index < this._locales.length)
          qx.locale.Manager.getInstance().setLocale(this._locales[index]);
        this._isModified = true;
      },
      this);

    this._nameEdit.addListener("input", function()
        {
        this._isModified = true;
        },
      this);

    this._passEdit.addListener("input", function()
        {
        this._isModified = true;
        },
      this);

    this._rememberCheck.addListener("changeValue", function()
        {
        this._isModified = true;
        },
      this);


    qx.locale.Manager.getInstance().addListener("changeLocale", function(){
        var currentLocale = qx.locale.Manager.getInstance().getLanguage();
        var index = this._locales.indexOf(currentLocale);
        if(index >= 0 && index < this._locales.length)
        {
          this._langController.getSelection().setItem(0, this._langModel.getItem(index));
        }
      }, this);

  }
});


