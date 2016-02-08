qx.Class.define("cashserver.StoreDelegate",
{
  extend: qx.core.Object,
  implement: [qx.data.store.IStoreDelegate, qx.data.marshal.IMarshalerDelegate],
  include: [qx.locale.MTranslation],

  members:
  {
    _requestData : null,
    setRequestData : function(data){this._requestData = data;},

    configureRequest: function(req) {
        var db = cashserver.Database.getInstance();
        var auth = new qx.io.request.authentication.Basic(db.username, db.password);

        if(this._requestData != null)
        req.setRequestData(this._requestData);
        req.setMethod("GET");
        req.setAuthentication(auth);
        req.setAsync(true);

        //TODO: Handle parser error
        req.addListener("fail", function(e) {
        var req = e.getTarget();
        var response = req.getResponse();
        cashserver.MessageBox.critical(this.tr("Server request error"), "fail "+ response + ". Code: " + req.getStatus());
      }, this);
    },

    manipulateData: function(data) {
        if(data.status)
        {
          cashserver.MessageBox.critical(this.tr("Server request error"), data.message);
          return null;
        }
        return data.data;
      },

    getPropertyMapping : function(property, properties) {
        //this.debug(property, properties);
        return property;
      },

    ignore : function(properties, parentProperty, depth) {
        return false;
      }
    },

  construct : function()
  {
    this.base(arguments);
  }
});
