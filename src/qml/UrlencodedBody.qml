import QtQuick 2.4
import QtQuick.Controls 1.4 as QQ1
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2 as QQ1Layout
import org.webtest.example 1.0


QQ1.TableView {
    signal valuesChanged();
    property bool modelChanged: false
    id: tableView
    frameVisible: true
    sortIndicatorVisible: true
    QQ1Layout.Layout.fillHeight: true
    QQ1Layout.Layout.fillWidth: true
    //anchors.fill: parent
    model: urlencodedBodyModel
    headerDelegate: Rectangle {
        height: textItem.implicitHeight * 1.2
        width: textItem.implicitWidth
        color: "lightsteelblue"
        Text {
            visible: styleData.column !== 3
            id: textItem
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.value
            renderType: Text.NativeRendering
        }
        Text{
            property bool myPressed: styleData.pressed
            text: "Bulk Edit"
            visible: styleData.column === 3
            font.underline: true
            renderType: Text.NativeRendering
            anchors.verticalCenter: parent.verticalCenter
            onMyPressedChanged: {
                console.log("show bulk edit")
            }
        }

        //styleData.column
        Rectangle {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            anchors.topMargin: 1
            width: 3
            color: "#ccc"
        }

    }
    rowDelegate: Rectangle {
        height: 20
        SystemPalette {
            id: myPalette
            colorGroup: SystemPalette.Active
        }
        color: {
            var baseColor = styleData.alternate?myPalette.alternateBase:myPalette.base
            return styleData.selected?myPalette.highlight:baseColor
        }
        MouseArea {
            propagateComposedEvents: true
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.LeftButton)
                {
                    if((urlencodedBodyModel.getData(urlencodedBodyModel.rowCount() - 1,1) !== "") || (urlencodedBodyModel.getData(urlencodedBodyModel.rowCount() - 1,2) !== "")){
                        urlencodedBodyModel.insertKeyValue("","");
                    }
                    mouse.accepted = false
                }
                else if (mouse.button === Qt.RightButton)
                {
                    contextMenuKeyValue.popup();
                }
            }
        }
    }
    QQ1.Menu {
        id: contextMenuKeyValue
        QQ1.MenuItem {
            text: qsTr('Delete')
            onTriggered: {
                var tmp = []
                tableView.selection.forEach( function(rowIndex) {
                    tmp.push(rowIndex);
                } );
                for(var i = tmp.length - 1; i >= 0; i--){
                    urlencodedBodyModel.removeHeader(tmp[i]);
                }
                tableView.selection.clear()
            }
        }
        QQ1.MenuItem {
            text: qsTr('Delete All')
            onTriggered: {
                urlencodedBodyModel.removeRows(0, urlencodedBodyModel.rowCount());
            }
        }
        QQ1.MenuItem {
            text: qsTr('Copy All')
            onTriggered: {
                urlencodedBodyModel.copyKeyValueToClipboard();
            }
        }
    }
    QQ1.TableViewColumn {
        id: selectColumn

        title: "Select"
        width: 40
        delegate: Item{
            anchors.fill: parent
            QQ1.CheckBox {
                anchors.centerIn: parent
                checked: styleData.value
                enabled: true
                onClicked: {
                    if(checked)
                        tableView.selection.select(styleData.row,styleData.row)
                    else{
                        tableView.selection.deselect(styleData.row,styleData.row)
                    }
                }
            }
        }
    }
    QQ1.TableViewColumn {
        id: key
        title: "Key"
        role: "key"
        delegate: Item {
            QQ1.TextField {
                property variant modelData: styleData.value
                id: keyEdit
                anchors.fill: parent
                text: styleData.value
                anchors.verticalCenter: parent.verticalCenter
                //currentIndex: -1
                onTextChanged: {
                    urlencodedBodyModel.setData(styleData.row, styleData.column, keyEdit.text)
                    tableView.valuesChanged();
                }
                onFocusChanged: {
                }

                onModelDataChanged: {    }
            }
        }
    }

    QQ1.TableViewColumn {
        id: valueColumn
        title: "Value"
        role: "value"
        delegate: Item {
            QQ1.TextField{
                id: valueEditor
                anchors.fill: parent
                text: styleData.value
                onTextChanged: {
                    //                        display.name = text;
                    //                        model.display = display
                }
                Connections {
                    target: valueEditor
                    onTextChanged: {
                        urlencodedBodyModel.setData(styleData.row, styleData.column, valueEditor.text)
                        tableView.valuesChanged();
                    }
                }
            }
        }
    }

    QQ1.TableViewColumn {
        delegate: Item{
            anchors.fill: parent
        }
    }
    onClicked: {
        console.log("value ")
    }
}
