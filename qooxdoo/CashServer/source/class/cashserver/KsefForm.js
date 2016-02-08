qx.Class.define("cashserver.KsefForm",
{
  extend : qx.ui.tabview.Page,

  members: {
    _started : false,
    _crStore : null,
    _crDelegate : null,
    _crList : null,
    _ksefStore : null,
    _ksefDelegate : null,
    _ksefList : null,
    _ksefDocLabel : null,

    queryStart : function() {
      this.debug("queryStart");
      if(this._started)
        return;

      if(this._crStore.getUrl() == null)
        this._crStore.setUrl("/ksef");
      else
      {
        if(this._crStore.getModel() == null)
          this._crStore.reload();
      }

      if(this._ksefStore.getUrl() == null)
      {
        var query = {query : "ksefGetDoc"};
        this._ksefDelegate.setRequestData(query);
        this._ksefStore.setUrl("/ksef");
      }

    },
    __none : null
  },

  construct : function()
  {
    this.base(arguments, this.tr("KSEF"), "resource/cashserver/ksef.png");

    var layout = new qx.ui.layout.Grid(0, 0);
    //layout.setColumnFlex(1, 1);
    layout.setColumnWidth(0, 200);
    layout.setColumnWidth(1, 400);

    layout.setRowFlex(0, 1);
    layout.setRowFlex(1, 1);
    this.setLayout(layout);

    //-- CR panel
    var left_container = new qx.ui.container.Composite()
    this.add(left_container, {row: 0, column: 0});

    var left_layout = new qx.ui.layout.VBox();
    left_layout.setSpacing(4); // apply spacing
    left_container.setLayout(left_layout);

    this._crList = new qx.ui.form.List();
    //this._crList.setWidth(80);
    left_container.add(this._crList);
    this._crList.setSelectionMode("multi");

    var crController = new qx.data.controller.List(null, this._crList);
    crController.setLabelPath('name');
    crController.setLabelOptions({'convertthiser': function(value, model)
    {
      //this.debug(value, model);
      return model.getId() + ':' + value;
    }});

    crController.setIconPath("id");
    crController.setIconOptions({
        converter : function(data) {
          return "resource/cashserver/cr.png";
        }
      });
    this._crDelegate = new cashserver.StoreDelegate();
    this._crDelegate.setRequestData( { query : "getCR", rc : 100 } );

    this._crStore = new qx.data.store.Json(null, this._crDelegate);
    this._crStore.bind('model', crController, 'model');

    //-- Bills panel
    var right_container = new qx.ui.container.Composite()
    this.add(right_container, {row: 0, column: 1});

    var right_layout = new qx.ui.layout.VBox();
    right_layout.setSpacing(4); // apply spacing
    right_container.setLayout(right_layout);

    this._ksefList = new qx.ui.form.List();
    //this._ksefList.setWidth(120);
    right_container.add(this._ksefList);

    var ksefController = new qx.data.controller.List(null, this._ksefList);
    var delegate = {
      configureItem : function(item) {
        item.setRich(true);
        },
      createItem : null,
      bindItem : null
      };
    ksefController.setDelegate(delegate);
    ksefController.setLabelPath('time');
    ksefController.setLabelOptions({'converter': function(value, model)
    {
      var type = qx.locale.Manager.tr("bill");
      if(model.getType() >= 100)
        type = qx.locale.Manager.tr("report");
      
      function parseDate(str)
      {
        var parts = str.split(" ");
        var dateparts = parts[0].split("-");
        var timeparts = parts[1].split(":");
        return new Date(dateparts[0], dateparts[1]-1, dateparts[2], timeparts[0], timeparts[1], timeparts[2]);
      }
      
      var time = qx.util.format.DateFormat.getDateTimeInstance().format(parseDate(value));
      
//      return "<table  width=\"400\"><tr><td width=\"20%\">" + model.getId() + "</td><td width=\"40%\">" + value + "</td><td  width=\"40%\">" + model.getType() + "</tr></tr></table>";
      return "<table width=\"380\"><tr><td width=\"60%\">" + time + "</td><td  width=\"40%\">" + type + "</tr></tr></table>";
    }});

    ksefController.setIconPath("type");
    ksefController.setIconOptions({
        converter : function(data) {
          if(data >= 100)
            return "resource/cashserver/report.png";
          return "resource/cashserver/bill.png";
        }
      });
    this._ksefDelegate = new cashserver.StoreDelegate();

    this._ksefStore = new qx.data.store.Json(null, this._ksefDelegate);
    this._ksefStore.bind('model', ksefController, 'model');

    this._crList.addListener("changeSelection", function(e) {
          var selection = crController.getSelection();
          var query = {};
          query["query"] = "ksefGetDoc";

          for(var i = 0; i < selection.length; i++)
          {
            query["param" + i] = selection.getItem(i).getId();
          }

          this._ksefDelegate.setRequestData(query);
          this._ksefStore.setUrl(null); //For reload
          this._ksefStore.setUrl("/ksef");
        }, this);


    //Date select
    var dateBox = new qx.ui.groupbox.CheckGroupBox(this.tr("By date"));
    dateBox.setLayout(new qx.ui.layout.HBox());
    dateBox.setValue(false);
    left_container.add(dateBox);

    var dateField = new qx.ui.form.DateField();
    dateField.setValue(new Date());
    dateField.addListener("changeValue", function(e) {
      this.debug("Change Value: " + e.getData());
    });
    dateBox.add(dateField);

    //Bill label
    var box = new qx.ui.groupbox.GroupBox(this.tr("Document"), "resource/cashserver/bill.png");
    right_container.add(box);

    var layout = new qx.ui.layout.VBox();
    layout.setSpacing(4); // apply spacing
    box.setLayout(layout);

    var scroll = new qx.ui.container.Scroll()
    box.add(scroll);

    this._ksefDocLabel = new qx.ui.basic.Label();
    this._ksefDocLabel.set( {
      rich : true
      } );

    scroll.add(this._ksefDocLabel);

    this._ksefList.addListener("changeSelection", function(e) {
          var selection = ksefController.getSelection();
          this._ksefDocLabel.setValue("");
          if(selection.length)
          {
            var request = new qx.io.request.Xhr('/ksef/doc');
            var db = cashserver.Database.getInstance();
            var auth = new qx.io.request.authentication.Basic(db.username, db.password);
            var requestData = {};
            requestData["id"] = selection.getItem(0).getId();

            request.setRequestData(requestData);
            request.setMethod("GET");
            request.setAuthentication(auth);
            request.setAsync(true);

            request.addListener("fail", function(e) {
              var req = e.getTarget();
              var response = req.getResponse();
              cashserver.MessageBox.critical(this.tr("Server request error"), "fail "+ response + ". Code: " + req.getStatus());
            }, this);

            request.bind("response", this._ksefDocLabel, "value");
            request.send();
         }

        }, this);


  }


});

