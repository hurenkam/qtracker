import QtQuick 1.0

XmlListModel {
    id: root
    query: "/methodResponse/params"
    property string url: "http://127.0.0.1:8280/RPC2"
    property string user: ""
    property string passwd: ""

    function rpcRequest(request,handler) {
        var http = new XMLHttpRequest()

        http.open("POST",root.url,true)
        http.setRequestHeader("Content-type", "text/xml")
        http.setRequestHeader("Content-length", request.length)
        //http.setRequestHeader("Authorization", "Basic " + Qt.btoa(root.user+":"+root.passwd))
        http.setRequestHeader("Connection", "close")
        http.onreadystatechange = function() {
            if(http.readyState == 4 && http.status == 200) {
                //console.log("XmlRpc::rpcRequest.onreadystatechange()")
                handler(http.responseText)
            }
        }
        http.send(request)
    }

    function callHandler(response) {
        xml = response
    }

    function call(cmd,params) {
        //console.log("XmlRpc.call(",cmd,params,")")
        var request = ""
        request += "<?xml version='1.0'?>"
        request += "<methodCall>"
        request += "<methodName>" + cmd + "</methodName>"
        request += "<params>"
        for (var i=0; i<params.length; i++) {
            request += "<param><value>"
            if (typeof(params[i])=="string") {
                request += "<string>" + params[i] + "</string>"
            }
            if (typeof(params[i])=="number") {
                request += "<int>" + params[i] + "</int>"
            }
            request += "</value></param>"
        }
        request += "</params>"
        request += "</methodCall>"
        rpcRequest(request,callHandler)
    }
}
