import QtQuick 2.4
import QtQuick.Controls 1.4 as QQ1
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2 as QQ1Layout
import org.webtest.example 1.0
Item {
    id: root
    property TextEdit bodyEdit
    QQ1Layout.ColumnLayout{
        anchors.fill: parent
        QQ1Layout.RowLayout{
            width: parent.width
            spacing: 10
            QQ1.ComboBox{
                property string httpMethod: requestHeaderWidget.httpMethod
                id: httpMethodCombo
                width: parent.width/4
                model: ListModel {
                    id: cbItems
                    ListElement { text: "PUT"; }
                    ListElement { text: "POST"; }
                    ListElement { text: "GET";  }
                    ListElement { text: "PATCH"; }
                    ListElement { text: "DELETE"; }
                }
                onCurrentIndexChanged: {
                    if( (currentText == "PUT") || (currentText == "POST") || (currentText == "PATCH") ){
                        tabviewRoot.enabled = true;
                    }else{
                        tabviewRoot.enabled = false;
                    }

                    requestHeaderWidget.httpMethod = currentText
                }
                onHttpMethodChanged: {
                    for(var i =0; i < cbItems.count; i++){
                        var method = cbItems.get(i);
                        if(method.text === requestHeaderWidget.httpMethod){
                            currentIndex = i;
                        }
                    }
                }

                Component.onCompleted: currentIndex = 2
            }

            QQ1.ComboBox{
                property string httpVersion: requestHeaderWidget.httpVersion
                id: httpVersionCombo
                width: parent.width/4
                model: ListModel {
                    id: httpVersionModel
                    ListElement { text: "HTTP 1.0"; }
                    ListElement { text: "HTTP 1.1"; }
                }
                onCurrentIndexChanged: {
                    requestHeaderWidget.httpVersion = currentText
                }
                Component.onCompleted: currentIndex = 1
                onHttpVersionChanged: {
                    for(var i =0; i < httpVersionModel.count; i++){
                        var method = httpVersionModel.get(i);
                        if(method.text === requestHeaderWidget.httpVersion){
                            currentIndex = i;
                        }
                    }
                }
            }
            QQ1.Button{
                text: "Send"
                onClicked: {
                    requestHeaderWidget.sendRequest();
                }
            }

        }
        QQ1.TextField{
            id: urlTextField
            QQ1Layout.Layout.fillWidth: true
            placeholderText: "Url"
            text: requestHeaderWidget.url
            onTextChanged: {
                requestHeaderWidget.url = text
//                queryStringModel.generateQueryStringFromRaw(requestHeaderWidget.getQueryStringFromUrl(text));
            }
        }
        QQ1.SplitView {
            //anchors.fill: parent
            //anchors.top: httpVersionCombo.bottom
            //anchors.bottom: parent.bottom
            QQ1Layout.Layout.fillWidth: true
            QQ1Layout.Layout.fillHeight: true
            orientation: Qt.Vertical
            onResizingChanged: {
                requestHeaderWidget.spliterSize = headersTabview.height;
            }

            QQ1.TabView{
                id: headersTabview
                height: requestHeaderWidget.spliterSize
                QQ1.Tab {
                    title: "Headers"
                    HeadersEditor{

                    }
                    //Component.onCompleted: headerListModel.rowCount() ;
                }
                QQ1.Tab {
                    title: "Query Editor"
                    id: queryEditorTab
                    QueryStringEditor{
                        id: queryEditorEditor
                        onValuesChanged: {
                            urlTextField.text = requestHeaderWidget.setQueryStringToUrl(urlTextField.text,queryStringModel.generateQueryString());
                        }
                    }
                }
            }
            Timer {
                id: timer
            }
            QQ1.TabView{
                id: tabviewRoot
                QQ1.Tab {
                    id: bodyText
                    title: "Raw Body"
                    BodyEditor{
                        id: bodyEditorId
                    }
                    function delay(delayTime, cb) {
                        timer.interval = delayTime;
                        timer.repeat = false;
                        timer.triggered.connect(cb);
                        timer.start();
                    }
                    onVisibleChanged: {
                        if(!bodyText.visible){
                            //workaround of https://bugs.kde.org/show_bug.cgi?id=375388
                            //https://bugreports.qt.io/browse/QTBUG-58811
                            delay(10, function() {
                                urlencodedBodyModel.generateUrlEncodedBodyFromRaw(requestHeaderWidget.bodyText);
                            });
                        }
                    }

                }
                QQ1.Tab {
                    function processValuesChanged(){
                        requestHeaderWidget.bodyText = urlencodedBodyModel.generateUrlEncodedBody();
                        var i = 0;
                        for(i = 0; i < requestHeaderModel.rowCount(); i++) {
                            var elemCur = requestHeaderModel.getData(i,0);//1 eshte kolona e headername
                            if("Content-Type" === elemCur) {
                                break
                            }
                        }
                        if(i === requestHeaderModel.rowCount()){
                            requestHeaderWidget.addHeader("Content-Type", "application/x-www-form-urlencoded")
//                            requestHeaderModel.setData(requestHeaderModel.rowCount()-1, 0, "Content-Type")
//                            requestHeaderModel.setData(requestHeaderModel.rowCount()-1, 1, "application/x-www-form-urlencoded")
                        }
                    }

                    title: "x-www-form-urlencoded"
                    id: urlEncodedBodyTab
                    UrlencodedBody{
                        id: urlEncodedBodyEditor
                        onValuesChanged: {
                            processValuesChanged()
                        }
                    }
                    onVisibleChanged: {
                        if(!urlEncodedBodyTab.visible){
                            processValuesChanged();
                        }
                    }
                }
            }
        }
    }
}
