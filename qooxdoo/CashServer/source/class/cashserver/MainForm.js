qx.Class.define("cashserver.MainForm",
{
  extend : qx.ui.tabview.TabView,

  members: {
    _settingsForm : null,
    _ksefForm : null,
    _storeForm : null,

    retranslateUi : function() {
    }
  },

  construct : function()
  {
    this.base(arguments);

    //var layout = new qx.ui.layout.Grid(0, 0);
    //parent.setLayout(layout);

    //parent.add(this);

    this._settingsForm = new cashserver.SettingsForm();
    this.add(this._settingsForm);

    this._ksefForm = new cashserver.KsefForm();
    this.add(this._ksefForm);

    this._storeForm = new cashserver.StoreForm();
    this.add(this._storeForm);

    this._settingsForm.loadSettings();

    this.addListener("changeSelection", function(e)
    {
      var selection = e.getData()[0];

      //this.debug(selection, this._settingsForm, e.getData());
      if(selection != this._settingsForm)
        this._settingsForm.saveSettings();

      //this.debug(selection, this._ksefForm, selection === this._ksefForm, selection == this._ksefForm);
      if(selection === this._ksefForm)
      {
        //this.debug("Try call queryStart", this._ksefForm, this._ksefForm.queryStart);
        this._ksefForm.queryStart();
        //this.debug("End call queryStart");
      }


    }, this);
  }

});
