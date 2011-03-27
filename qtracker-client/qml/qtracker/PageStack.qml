import QtQuick 1.0
import "qrc:/js/pages.js" as JS

Rectangle {
    id: root
    color: "black"
    width: parent.width
    height: parent.height

    //property list<Item> pages
    property Item currentPage: null

    function push(page) {
        JS.push(page);
        page.pageStack = root;
        selectPage();
    }

    function pop() {
        if (JS.length()<1) return;
        JS.pop();
        selectPage();
    }

    function selectPage() {
        console.log("selectPage")
        if (root.currentPage)
            root.currentPage.deActivate();

        root.currentPage = null;
        if (JS.length()<1) return;

        root.currentPage = JS.bottom();
        root.currentPage.activate(root);
        console.log("currentPage",root.currentPage.id);
    }

    Component.onCompleted: {
        console.log("PageStack:onCompleted()")
        selectPage();
    }
}
