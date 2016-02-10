qx.Class.define("cashserver.SingleTablePage",
{
  extend : qx.ui.tabview.Page,

  members: {
    _table : null,
    _tableModel : null,
    _onAddRow : null,
    _onDelRow : null,
    _onCommit : null,
    _commitCommand : null,
    _rollbackCommand : null,
    _fields : [],
    _keyField : 0,
    _convertFromUi : function(data)
    {
      return data;
    },
    _convertToUi : function(data)
    {
      return data;
    },
    
    _modifiedData : {},
    
    setFields : function(fields)
    {
      this._fields = fields;
    },

    _columns : [],
    setColumns : function(columns)
    {
      this._columns = columns;
      this._tableModel.setColumns(columns);
    },
    _getQuery : "???",
    setGetQuery : function(query)
    {
      this._getQuery = query;
    },

    _updateActive : false,
    _dataReady : false,
    queryStart : function()
    {

      if(this._dataReady)
        return;
      var req = new qx.io.request.Xhr();
      req.setUrl("/store");
      var db = cashserver.Database.getInstance();
      var auth = new qx.io.request.authentication.Basic(db.username, db.password);

      req.setRequestData({ query : this._getQuery });
      req.setMethod("GET");
      req.setAuthentication(auth);
      req.setAsync(true);
      req.addListener("success", function(e) {
        var req = e.getTarget();
        var response = req.getResponse();
        this._onDataReady(response);
      }, this);
      //TODO: Handle parser error
      req.addListener("fail", function(e) {
      var req = e.getTarget();
      var response = req.getResponse();
        cashserver.MessageBox.critical(this.tr("Server request error"), "fail "+ response + ". Code: " + req.getStatus());
      }, this);

      req.send();
   },

    _onDataReady : function(data)
    {
      if(data.status)
      {
        cashserver.MessageBox.critical(this.tr("Server request error"), data.message);
        return;
      }
      this._dataReady = true;
      var rows = [];
      var fields = this._fields;
      var fieldsLen = fields.length;
      var dataLen = data.data.length;
      //this.debug(fields);
      for(var row = 0; row < dataLen; row ++)
      {
        var cols = [];
        //this.debug(data.data[row]);
        for(var col = 0; col < fieldsLen; col ++)
          cols.push(data.data[row][fields[col]]);
        rows.push(this._convertToUi(cols));
      }
      //this.debug(rows);
      this._tableModel.setData(rows);
      
      if(!this._updateActive)
      {
        var table = new qx.ui.table.Table(this._tableModel);
  
        table.set({
          width: 600,
          height: 400,
          decorator : null
        });
        this.add(table);
        
        for(var col = 0; col < fieldsLen; col ++)
          this._tableModel.setColumnEditable(col, true);
        
        this._tableModel.addListener("dataChanged",   function(event)
        {
          if ( !(event instanceof qx.event.type.Data) || this._updateActive)
          {
            return;
          }
          var changedData = event.getData();
  
          // get changed data
          var model = this._tableModel;
          var key = model.getValue(this._keyField, changedData.firstRow);
          var fields = this._fields;
          var fieldsLen = fields.length;
          var data = [];
          for(var col = 0; col < fieldsLen; col ++)
          {
            data.push(model.getValue(col, changedData.firstRow));
          } 
          data = this._convertFromUi(data);
          var value = {};
          for(var col = 0; col < fieldsLen; col ++)
          {
            value[fields[col]] = data[col];
          } 

          this._modifiedData[key] = value;
          this.debug(key, value);
          this._commitCommand.setEnabled(true);
          this._rollbackCommand.setEnabled(true);
        },
        this);
      } //this._updateActive

      this._updateActive = false;
      this._commitCommand.setEnabled(false);
      this._rollbackCommand.setEnabled(false);
    },
    
    reload : function()
    {
      this._dataReady = false;
      this._updateActive = true;
      this._tableModel.setData([]);
      this._modifiedData = {};
      this.queryStart();
    },

    __none : null
  },

  construct : function(label, icon)
  {
    this.base(arguments);

    if(label != null)
      this.setLabel(label);
    if(icon != null)
      this.setIcon(icon);

    this.setLayout(new qx.ui.layout.VBox());

    var bar = new qx.ui.toolbar.ToolBar();
    this.add(bar);

    var button, part, checkBox;

    part = new qx.ui.toolbar.Part();
    bar.add(part);

    var command = new qx.ui.command.Command("F2");
    //command.setLabel(this.tr("New record"));
    command.setIcon("resource/cashserver/table_row_insert.png");
    command.setToolTipText(this.tr("Add new record to table") +  " (F2)");
    command.addListener("execute", function() {
      if(typeof this._onAddRow == "function")
        this._onAddRow();
    }, this);
      
    button = new qx.ui.toolbar.Button();
    button.setCommand(command);
    part.add(button);

    command = new qx.ui.command.Command("F3");
    //command.setLabel(this.tr("Delete record"));
    command.setIcon("resource/cashserver/table_row_delete.png");
    command.setToolTipText(this.tr("Delete selected record") +  " (F3)");
    command.addListener("execute", function() {
      if(typeof this._onDelRow == "function")
        this._onDelRow();
    }, this);

    button = new qx.ui.toolbar.Button();
    button.setCommand(command);
    part.add(button);

    part = new qx.ui.toolbar.Part();
    bar.add(part);

    command = this._commitCommand = new qx.ui.command.Command("CTRL+S");
    //command.setLabel(this.tr("Commit"));
    command.setIcon("resource/cashserver/db_commit.png");
    command.setToolTipText(this.tr("Apply changes") +  " (Ctrl+S)");
    command.addListener("execute", function() {
      if(typeof this._onCommit == "function")
        this._onCommit();
    }, this);

    button = new qx.ui.toolbar.Button();
    button.setCommand(command);
    part.add(button);

    command = this._rollbackCommand = new qx.ui.command.Command("CTRL+R");
    //command.setLabel(this.tr("Rollback"));
    command.setIcon("resource/cashserver/db_rollback.png");
    command.setToolTipText(this.tr("Skip changes") + " (Ctrl+R)");
    command.setEnabled(false);
    command.addListener("execute", function() {
      this.reload();
    }, this);

    button = new qx.ui.toolbar.Button();
    button.setCommand(command);
    part.add(button);


    var tableModel = this._tableModel = new qx.ui.table.model.Simple();


  // table

  },

 destruct : function()
 {
    this._disposeObjects("_tableModel");
 }

});
