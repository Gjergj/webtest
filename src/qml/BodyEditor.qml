import QtQuick 2.4
import QtQuick.Controls 1.4 as QQ1
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2 as QQ1Layout
import org.webtest.example 1.0
Flickable {
    id: flick
    anchors.fill: parent
    clip: true
    property alias bodyEditorText: bodyEditor.text
    contentWidth: bodyEditor.paintedWidth
    contentHeight: bodyEditor.paintedHeight
    Keys.onUpPressed: scrollBar.decrease()
    Keys.onDownPressed: scrollBar.increase()
    ScrollBar.horizontal: ScrollBar {
        id: hbar;
        active: vbar.active
        //                        parent: flick.parent
        //                        anchors.top: flick.top
        //                        anchors.left: flick.right
        //                        anchors.bottom: flick.bottom

    }
    ScrollBar.vertical: ScrollBar {
        id: vbar;
        active: hbar.active
        //                        parent: flick.parent
        //                        anchors.top: flick.top
        //                        anchors.left: flick.right
        //                        anchors.bottom: flick.bottom
    }
    function ensureVisible(r)
    {
        if (contentX >= r.x)
            contentX = r.x;
        else if (contentX+width <= r.x+r.width)
            contentX = r.x+r.width-width;
        if (contentY >= r.y)
            contentY = r.y;
        else if (contentY+height <= r.y+r.height)
            contentY = r.y+r.height-height;
    }
    TextEdit {
        id: bodyEditor
        Accessible.name: "document"
        enabled: true
        width: flick.width
        height: flick.height
        //focus: true
        //wrapMode: TextEdit.Wrap
        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
        selectByMouse: true
        mouseSelectionMode: TextEdit.SelectCharacters
        text: requestHeaderWidget.bodyText//document.text
        onTextChanged:{
            requestHeaderWidget.bodyText = text;
            requestHeaderWidget.updateContentLenght();
        }
        MouseArea {
            acceptedButtons: Qt.RightButton
            anchors.fill: parent
            onClicked: textEditorContextMenu.popup()
        }
        QQ1.Menu { id: textEditorContextMenu
            QQ1.MenuItem {
                text: qsTr("Copy")
                enabled: bodyEditor.selectedText
                onTriggered: bodyEditor.copy()
            }
            QQ1.MenuItem {
                text: qsTr("Cut")
                enabled: bodyEditor.selectedText
                onTriggered: bodyEditor.cut()
            }
            QQ1.MenuItem {
                text: qsTr("Paste")
                enabled: bodyEditor.canPaste
                onTriggered: bodyEditor.paste()
            }
            QQ1.MenuItem {
                text: qsTr("Send to Notepad")
                enabled: bodyEditor.selectedText
                onTriggered: requestHeaderWidget.sendToNotepadEditor(bodyEditor.selectedText)
            }
        }
        Component.onCompleted: bodyEdit = bodyEditor
    }
    Rectangle {
        color: "yellow"
        opacity: 0.2
        //border.color: "black"
        height: bodyEditor.cursorRectangle.height
        width: bodyEditor.width
        visible: bodyEditor.focus
        y: bodyEditor.cursorRectangle.y
    }

    //                    TextArea {
    //                        Accessible.name: "document"
    //                        id: textArea
    //                        frameVisible: false
    //                        width: parent.width
    //                        anchors.top: secondaryToolBar.bottom
    //                        anchors.bottom: parent.bottom
    //                        baseUrl: "qrc:/"
    //                        text: document.text
    //                        textFormat: Qt.RichText
    //                        Component.onCompleted: forceActiveFocus()
    //                    }

    DocumentHandler {
        id: document
        target: bodyEditor
        cursorPosition: bodyEditor.cursorPosition
        selectionStart: bodyEditor.selectionStart
        selectionEnd: bodyEditor.selectionEnd
        text:  requestHeaderWidget.bodyText

        //textColor: Qt.rgba(57, 228, 142)//colorDialog.color
        //Component.onCompleted: document.fileUrl = "qrc:/example.html"
        //                        onFontSizeChanged: {
        //                            fontSizeSpinBox.valueGuard = false
        //                            fontSizeSpinBox.value = document.fontSize
        //                            fontSizeSpinBox.valueGuard = true
        //                        }
        onFontFamilyChanged: {
            //                            var index = Qt.fontFamilies().indexOf(document.fontFamily)
            //                            if (index === -1) {
            //                                fontFamilyComboBox.currentIndex = 0
            //                                fontFamilyComboBox.special = true
            //                            } else {
            //                                fontFamilyComboBox.currentIndex = index
            //                                fontFamilyComboBox.special = false
            //                            }
        }
        onError: {
            //                            errorDialog.text = message
            //                            errorDialog.visible = true
        }
    }
}
