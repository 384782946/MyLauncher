﻿import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 330
    height: 500
    title: qsTr("Hello World")
    //flags: Qt.FramelessWindowHint

    Image{
        anchors.fill: parent
        source: "qrc:/images/eMEvOt1ClEs.jpg"
        fillMode: Image.PreserveAspectCrop
    }

    TextEdit{
        anchors{
            top:parent.top
            topMargin: 10
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 0 // tabBar.currentIndex

        background: null


        Page1 {
        }
    }

//    PageIndicator {
//         id: indicator

//         count: swipeView.count
//         currentIndex: swipeView.currentIndex

//         anchors.bottom: swipeView.bottom
//         anchors.horizontalCenter: parent.horizontalCenter
//     }

//    header: TabBar {
//        id: tabBar
//        currentIndex: swipeView.currentIndex
//        background: null

//        TabButton {
//            text: qsTr("应用")
//            height: parent.height
//            font.pixelSize: 18
//            background: Rectangle{
//                color: tabBar.currentIndex == 0?"#44ffffff":"#66000000"
//            }
//        }
//        TabButton {
//            text: qsTr("文件夹")
//            height: parent.height
//            font.pixelSize: 18
//            background: Rectangle{
//                color: tabBar.currentIndex == 1?"#44ffffff":"#66000000"
//            }
//        }
//    }
}
