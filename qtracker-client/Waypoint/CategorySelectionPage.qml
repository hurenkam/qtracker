import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Category"
    options: options

    signal categorySelected(int index, string name)

    VisualItemModel {
        id: options

        CategoryList {
            id: lst
            title: "List"
            onClicked: root.categorySelected(index,text)
        }
    }

    onCategorySelected: console.log("CategoryMenu.onCategorySelected",index,name)
}
