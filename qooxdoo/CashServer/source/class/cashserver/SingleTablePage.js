/*----------------------------------------------------------------------------*/
/**
* @pkg SingleTablePage
*/
/**
* Base for all store pages.
*
* (C) T&T team, Kiev, Ukraine 2016.<br>
* started 13.02.2016 11:50:39<br>
* @pkgdoc SingleTablePage
* @author oleg
* @version 0.01
*/
/*----------------------------------------------------------------------------*/
qx.Class.define("cashserver.SingleTablePage",
{
  extend : qx.ui.tabview.Page,

  members: {
    _table : null,
    _tableModel : null,
    _commitCommand : null,
    _rollbackCommand : null,
    _fields : [],
    _keyField : 0,
    _modifiedData : {},
    _columns : [],
    _getQuery : "???",
    _updateActive : false,
    _dataReady : false,

    /*------------------------------------------------------------------------*/
    _getFirstSelectedRow : function()
    {
      var selection = [];
      this._table.getSelectionModel().iterateSelection(function(ind)
      {
        selection.push(ind);
      });
      if(selection.length)
        return selection[0];
      return null;
    },
    /*------------------------------------------------------------------------*/
    _onAddRow : function()
    {
      var row = this._getFirstSelectedRow();
      this.debug(row);
      if(row == null)
        row = 0;
      var index = this._tableModel.getValue(this._keyField, row);
      var rowCount = this._tableModel.getRowCount();
      for(row ++; row < rowCount; row ++)
      {
        var nIndex = this._tableModel.getValue(this._keyField, row);
        if(nIndex > index + 1)
          break;
        index = nIndex;
      }

      var rows = [[]];
      rows[0][this._keyField] = index + 1;
      this.debug(rows, row);

      this._updateActive = true; //Prevent onChange event
      this._tableModel.addRows(rows, row);
      this._updateActive = false; //Prevent onChange event

      this._table.scrollCellVisible(this._keyField, row);
      this._table.getSelectionModel().setSelectionInterval(row, row);

      var pane = this._table.getPaneScroller(0).getTablePane();
      pane.activate();
      this._table.setFocusedCell(this._keyField, row);
    },
    /*------------------------------------------------------------------------*/
    _onDelRow : function()
    {
      var selection = [];
      var delta = 0;
      this._table.getSelectionModel().iterateSelection(function(ind)
      {
        selection.push(ind);
      });

      var model = this._tableModel;
      var fields = this._fields;
      var fieldsLen = fields.length;
      for(var i = 0; i < selection.length; i ++)
      {
        var row = selection[i] - delta;
        var data = [];
        for(var col = 0; col < fieldsLen; col ++)
          data.push(model.getValue(col, row));

        data = this._convertFromUi(data);
        var key = data[this._keyField];
        var record = {};
        record[fields[this._keyField]] = key;
        record["DEL"] = 1;
        this._modifiedData[key] = record;
        this._commitCommand.setEnabled(true);
        this._rollbackCommand.setEnabled(true);

        model.removeRows(row, 1);
        delta ++;
      }
      this._table.resetSelection();
      this._table.resetCellFocus();
    },
    /*------------------------------------------------------------------------*/
    _convertFromUi : function(data)
    {
      return data;
    },
    /*------------------------------------------------------------------------*/
    _convertToUi : function(data)
    {
      return data;
    },
    /*------------------------------------------------------------------------*/
    setFields : function(fields)
    {
      this._fields = fields;
    },
    /*------------------------------------------------------------------------*/
    setColumns : function(columns)
    {
      this._columns = columns;
      this._tableModel.setColumns(columns);
    },
    /*------------------------------------------------------------------------*/
    setGetQuery : function(query)
    {
      this._getQuery = query;
    },
    /*------------------------------------------------------------------------*/
    _query : function(tableName, onDataReady, self)
    {
      var req = new qx.io.request.Xhr();
      req.setUrl("/store");
      var db = cashserver.Database.getInstance();
      var auth = new qx.io.request.authentication.Basic(db.username, db.password);

      req.setRequestData({ query : tableName });
      req.setMethod("GET");
      req.setAuthentication(auth);
      req.setAsync(true);
      req.addListener("success", function(e) {
        var req = e.getTarget();
        var response = req.getResponse();

        if(response.status)
        {
          cashserver.MessageBox.critical(this.tr("Server request error"), response.message);
          return;
        }

        if(typeof onDataReady == "function")
        {
          onDataReady.call(self||this, response.data);
        }
      }, this);

      //TODO: Handle parser error
      req.addListener("fail", function(e) {
      var req = e.getTarget();
      var response = req.getResponse();
        cashserver.MessageBox.critical(this.tr("Server request error"), "fail "+ response + ". Code: " + req.getStatus());
      }, this);

      req.send();
    },
    /*------------------------------------------------------------------------*/
    queryStart : function()
    {

      if(this._dataReady)
        return;
      this._query(this._getQuery, this._onDataReady, this);
    },
    /*------------------------------------------------------------------------*/
    _onDataReady : function(data)
    {
      this._dataReady = true;
      var rows = [];
      var fields = this._fields;
      var fieldsLen = fields.length;
      var dataLen = data.length;
      //this.debug(fields);
      for(var row = 0; row < dataLen; row ++)
      {
        var cols = [];
        //this.debug(data.data[row]);
        for(var col = 0; col < fieldsLen; col ++)
          cols.push(data[row][fields[col]]);
        rows.push(this._convertToUi(cols));
      }
      //this.debug(rows);
      this._tableModel.setData(rows);

      if(!this._updateActive)
      {
        var table = this._table = new qx.ui.table.Table(this._tableModel);

        table.set({
          width: 600,
          height: 400,
          decorator : null
        });
        this.add(table);
        table.getSelectionModel().setSelectionMode(qx.ui.table.selection.Model.MULTIPLE_INTERVAL_SELECTION);

        for(var col = 0; col < fieldsLen; col ++)
          this._tableModel.setColumnEditable(col, true);

        this._tableModel.addListener("dataChanged",   function(event)
        {
          if ( !(event instanceof qx.event.type.Data) || this._updateActive)
          {
            return;
          }
          var changedData = event.getData();
          this._onDataChanged(changedData);
        },
        this);
      } //this._updateActive

      this._updateActive = false;
      this._commitCommand.setEnabled(false);
      this._rollbackCommand.setEnabled(false);
    },
    /*------------------------------------------------------------------------*/
    _prepareModifiedRecord : function(data)
    {
      var fields = this._fields;
      var fieldsLen = fields.length;
      var value = {};
      for(var col = 0; col < fieldsLen; col ++)
      {
        value[fields[col]] = data[col];
      }
      value["DEL"] = 0;
      return value;
    },
    /*------------------------------------------------------------------------*/
    _onDataChanged : function(changedData)
    {
      // get changed data
      var model = this._tableModel;
      var fields = this._fields;
      var fieldsLen = fields.length;
      var data = [];
      for(var col = 0; col < fieldsLen; col ++)
      {
        data.push(model.getValue(col, changedData.firstRow));
      }
      data = this._convertFromUi(data);
      var key = data[this._keyField];
      this._modifiedData[key] = this._prepareModifiedRecord(data);
      this._commitCommand.setEnabled(true);
      this._rollbackCommand.setEnabled(true);
      this.debug("Modified data:", key, this._modifiedData[key]);
    },
    /*------------------------------------------------------------------------*/
    reload : function()
    {
      this._dataReady = false;
      this._updateActive = true;
      this._tableModel.setData([]);
      this._modifiedData = {};
      this.queryStart();
    },
    /*------------------------------------------------------------------------*/
    _prepareCommitData : function()
    {
      var data = [];
      for(var key in this._modifiedData)
      {
        var row = this._modifiedData[key];
        row["TABLE"] = this._getQuery;
        data.push(row);
      }
      return data;
    },
    /*------------------------------------------------------------------------*/
    _onCommit : function()
    {
      var req = new qx.io.request.Xhr();
      req.setUrl("/store");
      var db = cashserver.Database.getInstance();
      var auth = new qx.io.request.authentication.Basic(db.username, db.password);

//      req.setRequestData({ query : this._getQuery });
      req.setMethod("POST");
      req.setAuthentication(auth);
      req.setAsync(true);
      req.setRequestHeader("Content-Type", "application/json; charset=utf8");
      req.setRequestData(this._prepareCommitData());

      req.addListener("success", function(e) {
        var req = e.getTarget();
        var data = req.getResponse();
        if(data.status)
        {
          cashserver.MessageBox.critical(this.tr("Server request error"), data.message);
          return;
        }

        this._commitCommand.setEnabled(false);
        this._rollbackCommand.setEnabled(false);
        this._modifiedData = {};
      }, this);

      req.addListener("fail", function(e) {
      var req = e.getTarget();
      var response = req.getResponse();
        cashserver.MessageBox.critical(this.tr("Server request error"), "fail "+ response + ". Code: " + req.getStatus());
      }, this);

      req.send();

    },

    __none__ : null
  }, //members

  /*------------------------------------------------------------------------*/
  /**Constructor*/
  /*------------------------------------------------------------------------*/
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

    // table will be created after data loaded

  },

  /*------------------------------------------------------------------------*/
  /**Destructor*/
  /*------------------------------------------------------------------------*/
   destruct : function()
   {
      this._disposeObjects("_tableModel");
   }

});
/*----------------------------------------------------------------------------*/
