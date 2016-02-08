qx.Class.define("cashserver.MessageBox",
{
  extend : qx.ui.window.Window,
  members: {
    headerLabel : null,
    bodyLabel : null
  },

  statics :
  {
    critical : function(header, message) {
      var win = new cashserver.MessageBox("Critical");
      win.headerLabel.setValue("<h1>" + header + "</h>");
      win.bodyLabel.setValue("<p>" + message + "</p>");
      win.open();
    },

    information : function(header, message) {
    }
  },

  construct : function(caption)
  {
    this.base(arguments, caption);

    this.setContentPadding(10);

    //var layout = new qx.ui.layout.Grid(0, 0);
    var layout = new qx.ui.layout.VBox();
    layout.setSpacing(4); // apply spacing
    this.setLayout(layout);

    this.headerLabel = new qx.ui.basic.Label("<h1>Header</h>")
    this.headerLabel.setRich(true);
    this.add(this.headerLabel);

    this.bodyLabel = new qx.ui.basic.Label("<p>Message text</p>")
    this.add(this.bodyLabel);
    this.bodyLabel.setRich(true);

    //var layout = 
    
    var btn_container = new qx.ui.container.Composite()
    var btn_layout = new qx.ui.layout.Grid(2, 1);
    //layout.setRowFlex(0, 1); // make row 0 flexible
    btn_layout.setColumnFlex(0, 1);
    btn_layout.setColumnWidth(1, 80); // set with of column 1 to 200 pixel
    btn_container.setLayout(btn_layout);
    var okBtn = new qx.ui.form.Button("Ok");
    btn_container.add(okBtn, {row: 0, column: 1});
    this.add(btn_container);
    

    okBtn.addListener("execute", function() {
      this.close();
    }, this);


    this.addListenerOnce("resize", this.center, this);
    this.setAllowMaximize(false);
    this.setAllowMinimize(false);
    this.setModal(true);
  }
});

