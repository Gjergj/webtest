import QtQuick 2.4
import QtQuick.Controls 1.4 as QQ1
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2 as QQ1Layout
import org.webtest.example 1.0

QQ1.TableView {
    property bool modelChanged: false
    id: tableView
    //backgroundVisible: false
    frameVisible: true
    sortIndicatorVisible: true
    //anchors.fill: parent
    //anchors.bottom: parent.bottom
    //anchors.right: parent.right
    //anchors.left: parent.left
    //anchors.bottom: parent.bottom
    anchors.fill: parent
    model: requestHeaderModel
    //Layout.minimumWidth: 50
    //Layout.minimumHeight: 50
    //Layout.preferredWidth: parent.width
    //Layout.preferredHeight: parent.height
    //Layout.fillHeight: true
    //Layout.fillWidth: true
    //selectionMode: SelectionMode.MultiSelection

    rowDelegate: Rectangle {
        height: 22

        SystemPalette {
            id: myPalette;
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
                    if( (requestHeaderModel.getData(requestHeaderModel.rowCount()-1,0) !== "") || (requestHeaderModel.getData(requestHeaderModel.rowCount()-1,1) !== "")){
                        requestHeaderWidget.addHeader("","");
                    }
                    mouse.accepted = false
                }
                else if (mouse.button === Qt.RightButton)
                {
                    contextMenu.popup();
                }
            }
        }
    }
    QQ1.Menu { id: contextMenu
        QQ1.MenuItem {
            visible: tableView.selection.count > 0
            text: qsTr('Delete')
            onTriggered: {
                var tmp = []
                tableView.selection.forEach( function(rowIndex) {
                    tmp.push(rowIndex);
                } );
                for(var i = tmp.length - 1; i >= 0; i--){
                    requestHeaderWidget.removeHeader(tmp[i]);
                }
                tableView.selection.clear()
            }
        }
        QQ1.MenuItem {
            visible: tableView.rowCount > 0
            text: qsTr('Delete All')
            onTriggered: {
                requestHeaderWidget.removeAllHeaders();
            }
        }
        QQ1.MenuItem {
            visible: tableView.selection.count > 0
            text: qsTr('Copy')
            onTriggered: {
                var tmp = []
                tableView.selection.forEach( function(rowIndex) {
                    tmp.push(rowIndex);
                } );
                requestHeaderWidget.copyHeadersToClipBoard(tmp);
                tableView.selection.clear()
            }
        }
        QQ1.MenuItem {
            visible: tableView.rowCount > 0
            text: qsTr('Copy All')
            onTriggered: {
                requestHeaderWidget.copyHeadersAllToClipBoard();
            }
        }
        QQ1.MenuItem {
            text: qsTr('Paste')
            onTriggered: {
                requestHeaderWidget.pasteHeadersFromClipBoard();
            }
        }
    }
    //    onRowCountChanged: {
    //        console.log("onRowCountChanged")
    //    }

    QQ1.TableViewColumn {
        id: headerName
        title: "Header Field"
        role: "headerName"


        delegate: Item {
            property string modelData: styleData.value
            Connections {
                target: modelData/*tableView.model*/
                onModelDataChanged:{
                    console.log("onRowsInserted")
                    console.log(styleData.value)
                    headersCombo.currentIndex = headersCombo.find(styleData.value);
                }
            }
            QQ1.ComboBox {
                property bool manualEditText: false //ndalon update te modeleve ne onModelDataChanged nese eshte modifikim manual i textit
                property bool modelDataEditText: false//ndalon update te modeleve ne onEditTextChanged nese eshte modifikim i modelit nga kodi
                property bool currentIndexChangedEdit: false//ndalon update te modeleve ne onEditTextChanged dhe onModelDatachanged nese eshte modifikim i modelit nga currentIndexChanged
                id: headersCombo
                anchors.fill: parent
                editable: true
                anchors.verticalCenter: parent.verticalCenter
                //currentIndex: -1
                model:  headerListModel

                onActivated: {
                    console.log("onActivated")
                    currentIndexChangedEdit = true;
                }
                onCurrentIndexChanged: {
                    console.log("onCurrentIndexChanged")
                    console.log(currentIndex)
                    console.log(styleData.value);
                    if(currentIndexChangedEdit){
                        requestHeaderModel.setData(styleData.row, styleData.column, currentText);
                    }
                    currentIndexChangedEdit = false
                }
                onEditTextChanged: {
                    console.log("onEditTextChanged")
                    console.log(currentIndex)
                    manualEditText = true;
                    if(!currentIndexChangedEdit)
                    {
                        if(currentIndex === 0){
                            headerListModel.setData(headerListModel.rowCount() - 1, styleData.column, headersCombo.editText)
                            requestHeaderModel.setData(requestHeaderModel.rowCount()-1, styleData.column, headersCombo.editText)
                        }
                        else if(currentIndex > 0){
                            console.log("else " + styleData.row)
                            headerListModel.setData(currentIndex, styleData.column, headersCombo.editText)
                            requestHeaderModel.setData(styleData.row, styleData.column, headersCombo.editText)
                        }else{

                        }
                    }
                    manualEditText = false;
                }
                onCurrentTextChanged: {

                }
                onFocusChanged: {
                }
            }

        }
    }

    QQ1.TableViewColumn {
        id: headerValueColumn
        title: "Value"
        role: "headerValue"
        delegate: Item {
            property string modelData: styleData.value
            Connections {
                target: modelData
                onModelDataChanged:{
                    headerValueEditor.text = styleData.value;
                }
            }
            QQ1.TextField{
                id: headerValueEditor
                anchors.fill: parent
                onTextChanged: {
                    //                        display.name = text;
                    //                        model.display = display
                }

                Connections {
                    target: headerValueEditor
                    onTextChanged: {
                        requestHeaderModel.setData(styleData.row, styleData.column, headerValueEditor.text)
                    }
                }
            }
        }
    }

    QQ1.TableViewColumn {
        id: vendorProductCodeColumn

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
    onClicked: {

    }
}
