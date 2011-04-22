import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Page {

    id: root
    imageSource: "qrc:/images/options-bg.png"

    signal categorySelected(int index, string name)

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }

    OptionHeader {
        id: hdr
        x: 0; y:0; width: parent.width; height: 50
        text: "Category"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    CategoryList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        anchors.bottom: parent.bottom;
        title: "List"
        onClicked: categorySelected(index,text)
    }

    onCategorySelected: console.log("CategoryMenu.onCategorySelected",index,name)
}
