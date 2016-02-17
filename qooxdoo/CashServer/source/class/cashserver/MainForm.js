/*----------------------------------------------------------------------------*/
/**
* @pkg MainForm
*/
/**
* Web UI main form.
*
* MainForm is e tabView with 3 page: 1-settings, 2-ksef, 3-store<br>
* (C) T&T team, Kiev, Ukraine 2016.<br>
* started 13.01.2016 11:02:38<br>
* @pkgdoc MainForm
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
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

      if(selection != this._settingsForm)
        this._settingsForm.saveSettings();

      if(selection === this._ksefForm)
      {
        this._ksefForm.queryStart();
      }

      if(selection === this._storeForm)
      {
        this._storeForm.queryStart();
      }


    }, this);
  }

});
/*----------------------------------------------------------------------------*/
