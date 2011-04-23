import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Track" : "Resume Track"
    options: trkoptions
    property int index: -1

    VisualItemModel {
        id: trkoptions

        OptionList {
            id: trkbox
            title: "Track Options"
            items: trkitems

            DynamicItemModel {
                id: trkitems
                name: "trkitems"

                OptionInputItem { id: trkname; text: "Name:";   value: "Home" }
            }
        }

        OptionList {
            id: intervaloptions
            title: "Recording Options"
            items: intervalitems

            DynamicItemModel {
                id: intervalitems
                name: "intervalitems"

                OptionRadioButton { id: btnall;  text: "All";      }
                OptionRadioButton { id: btntime; text: "Time";     }
                OptionRadioButton { id: btndist; text: "Distance"; ticked: true }
            }
            onClicked: {
                console.log("Interval.onClicked",index)
                for (var i=0; i<intervalitems.count(); i++) {
                    intervalitems.get(i).ticked = (index == i)
                }
            }
        }

        OptionList {
            id: timeoptions
            title: "Time Interval"
            items: timeitems
            visible: btntime.ticked

            DynamicItemModel {
                id: timeitems
                name: "timeitems"

                OptionRadioButton { text: " 5 seconds";  }
                OptionRadioButton { text: "15 seconds";  ticked: true }
                OptionRadioButton { text: " 1 minute";   }
                OptionRadioButton { text: " 5 minutes";  }
                OptionRadioButton { text: "15 minutes";  }
                OptionRadioButton { text: " 1 hour";     }
            }
            onClicked: {
                console.log("Time.onClicked",index)
                for (var i=0; i<timeitems.count(); i++) {
                    timeitems.get(i).ticked = (index == i)
                }
            }
            onVisibleChanged: root.layoutPage();
        }

        OptionList {
            id: distoptions
            title: "Distance Interval"
            items: distitems
            visible: btndist.ticked

            DynamicItemModel {
                id: distitems
                name: "distitems"

                OptionRadioButton { text: " 10 meters";    }
                OptionRadioButton { text: " 30 meters";    ticked: true }
                OptionRadioButton { text: "100 meters";    }
                OptionRadioButton { text: "300 meters";    }
                OptionRadioButton { text: "  1 kilometer"; }
            }
            onClicked: {
                console.log("Distance.onClicked",index)
                for (var i=0; i<distitems.count(); i++) {
                    distitems.get(i).ticked = (index == i)
                }
            }
            onVisibleChanged: root.layoutPage();
        }

        OptionList {
            id: confirm
            items: confirmitems

            DynamicItemModel {
                id: confirmitems
                name: "confirmitems"
                OptionTextItem {
                    text: "Confirm";
                    button: true;
                    buttonsource: "visible.svg";
                    onClicked: pageStack.pop()
                }
            }
        }
    }
}

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
        rightButtonSource: "visible.svg"
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
