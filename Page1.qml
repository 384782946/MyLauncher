import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Page {
    background: Rectangle{
        color: "transparent"
    }

    GridView {
        model: 20
        anchors.fill: parent
        anchors.margins: 60


        delegate: Item{
            width: 200
            height: 200

            Rectangle{

                anchors.centerIn: parent
                width: 80
                height: 80
                color: "#aa3a4c5b"
                radius: 4

//                FastBlur {
//                    id: blur
//                    anchors.fill: parent
//                    source: parent
//                    radius: 32
//                }

                Text {
                    id: name
                    text: qsTr("text")
                    color: "white"
                    font.pixelSize: 18
                    anchors.top: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }


        }
    }
}
