qx.Class.define("cashserver.LoginWindow",
{
  extend : qx.ui.window.Window,

  events :
  {
    "login" : "qx.event.type.Data",
    "cancel"   : "qx.event.type.Event"
  },


  construct : function()
  {
    this.base(arguments, "Login");
    //this.setWidth(250);
    //this.setHeight(300);
    this.setContentPadding(10);

    var layout = new qx.ui.layout.Grid(0, 0);
    this.setLayout(layout);

    this.add(new qx.ui.basic.Label("User name"), {row: 0, column: 0});

    var userField = new qx.ui.form.TextField();
    this.add(userField, {row: 0, column: 1});

    this.add(new qx.ui.basic.Label("Password"), {row: 1, column: 0});
    var passField = new qx.ui.form.PasswordField();
    this.add(passField, {row: 1, column: 1});

    var cancelBtn = new qx.ui.form.Button("Cancel");
    this.add(cancelBtn, {row: 2, column: 0});

    cancelBtn.addListener("execute", function() {
      this.fireEvent("cancel");
    }, this);


    var loginBtn = new qx.ui.form.Button("Login");
    this.add(loginBtn, {row: 2, column: 1});
    loginBtn.setEnabled(false);

    loginBtn.addListener("execute", function() {
      this.fireEvent("login");
    }, this);

    userField.addListener("input", function(e)
        {
          var value = e.getData();
          loginBtn.setEnabled(value.length < 16 && value.length > 0);
        },
      this);

    this.addListenerOnce("resize", this.center, this);
    this.setAllowMaximize(false);
    this.setAllowMinimize(false);

  }
});
