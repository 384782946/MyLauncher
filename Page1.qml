import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Page {
    background: Rectangle{
        color: "transparent"
    }

    ListModel{
        id:_model
    }

    GridView {
        anchors.centerIn: parent
        width: parent.width * 0.9
        height: parent.height * 0.8
        model: _model
        cellWidth:140
        cellHeight:140

        DropArea{
            anchors.fill: parent

            onEntered: {
                if (drag.urls.length > 10) {
                    drag.accepted = false
                } else {
                    drag.accepted = true
                }
            }

            onDropped: {
                if (drop.hasUrls) {
                    var u = drop.urls[0]
                    for(var i in drop.urls) {
                        var filePath = drop.urls[i].toString()
                        console.info("selected file:",filePath)
                        var obj = {
                            path: filePath,
                            name: filePath
                        }

                        _model.append(obj)
                    }
                }
            }
        }

        delegate: Item{
            width: 120
            height: 120

            MouseArea{
                id:_mouseArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onEntered: {
                    _bg.visible = true
                }

                onExited: {
                    _bg.visible = false
                }

                onClicked: {
                    Qt.openUrlExternally(name)
                }
            }

            Rectangle{
                id: _bg
                anchors.fill: parent
                color: "#000000"
                opacity: 0.08
                visible: false
            }

            Rectangle{
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -8
                width: 80
                height: 80
                color: "#66000000"
                radius: 4

                TextEdit {
                    id: _name
                    text: name
                    color: "white"
                    font.family: "微软雅黑"
                    font.pixelSize: 17
                    width: parent.width
                    anchors.top: parent.bottom
                    anchors.topMargin: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: true
                    //elide: Text.ElideMiddle

                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked: {
                            _name.readOnly = false
                            _name.forceActiveFocus()
                            _name.selectAll()
                        }
                    }

                    Keys.onReturnPressed: {
                        _name.readOnly = true
                    }
                }
            }
        }
    }
}
