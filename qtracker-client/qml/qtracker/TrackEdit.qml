import QtQuick 1.0

Item {}

/* Obsolete

Page {
    id: root

    function confirm() {
        starttrack(nameinput.input,0);
        pageStack.home();
    }
    function cancel() {
        pageStack.pop();
    }
    function starttrack(trackname,interval) {
        console.log("starttrack()")
        var cmd = { "class": "server", "method": "trackstart", "args": [ trackname, interval ] }
        client.sendCommand(cmd);
    }

    MenuEntry {
        id: heading
        x: 0; y: 0
        width: parent.width
        height: 54
        text: "New Track"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
        rightButtonVisible: true
        rightButtonSource: "qrc:/images/visible.svg"
        onRightClicked: root.confirm();
    }

    MenuEntry {
        id: interval
        x: 0; y: 54
        width: parent.width
        height: parent.height -54

        InputField {
            id: nameinput
            x: 5; y:5;
            height: 50
            width: parent.width-10
            input: "trk-20110328-211000"
        }
        RadioBox {
            id: base
            x: 5; y:60
            width: landscape? parent.width/2 -10 : parent.width-10
            visible: true
            buttonheight: 45
            selected: 0
            radiobuttons: [ RadioButton { text:"All"}, RadioButton{ text:"Time based"}, RadioButton { text:"Distance based"} ]
        }
        RadioBox {
            id: time
            x: landscape? parent.width/2 + 5 : 5;
            y: landscape? 60 : base.height+70
            width: landscape? parent.width/2 -10 : parent.width-10
            buttonheight: 45
            visible: base.selected==1? true: false
            selected: 1
            radiobuttons: [ RadioButton { text:"5s"}, RadioButton{ text:"15s"}, RadioButton { text:"1m"}, RadioButton { text:"5m"}, RadioButton { text:"15m"} ]
        }
        RadioBox {
            id: distance
            x: landscape? parent.width/2 + 5 : 5;
            y: landscape? 60 : base.height+70
            width: landscape? parent.width/2 -10 : parent.width-10
            buttonheight: 45
            visible: base.selected==2? true: false
            selected: 1
            radiobuttons: [ RadioButton { text:"10m"}, RadioButton{ text:"30m"}, RadioButton { text:"100m"}, RadioButton { text:"300m"}, RadioButton { text:"1km"} ]
        }
    }
}

*/
