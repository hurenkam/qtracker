import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal routeSelected(int index, int routeid, string name, real top, real left, real bottom, real right)

    function saveRoute(index, name, top, left, bottom, right) {
        console.log("saveRoute",index, name, top, left, bottom, right)
        if (index < 0) {
            database.append( {name: name, top: top, left: left, bottom: bottom, right: right} )
        } else {
            database.set(index, {name: name, top: top, left: left, bottom: bottom, right: right} )
        }
        database.refresh()
        root.update()
    }

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true; }
    }
/*
    Database {
        id: database
        table: "routes"
        onCountChanged: root.update()
        onDataChanged: root.update()

        Component.onCompleted: {
            console.log("RouteList.database.onCompleted")
            database.exec("CREATE TABLE IF NOT EXISTS routes (routeid INTEGER PRIMARY KEY, name TEXT, top REAL, left REAL, bottom REAL, right REAL);")
        }
    }
*/
    Routes {
        id: database
        onCountChanged: root.update()
        onDataChanged: root.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "RouteList"
        //OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        item = delegate.createObject(null)
        item.text = "<new>"
        item.button = true;
        content.append(item)
        console.log("routelist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("routelist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i).routeid + " " + database.get(i).name
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("RouteList.onClicked",index,text);
        var rte = database.get(index-1)
        routeSelected(index-1,rte.routeid,rte.name,rte.top,rte.left,rte.bottom,rte.right)
    }

    Component.onCompleted: {
        update()
    }
}
