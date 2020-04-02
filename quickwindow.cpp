#include <QQuickItem>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QDesktopWidget>

#include "quickwindow.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#include <Dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#endif

namespace GomeplusUI {
    namespace QMLType {
        
        QuickWindow::QuickWindow(QWindow *parent) : QQuickWindow(parent)
        {
            setFlags(Qt::Window|Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint);
            
#ifdef Q_OS_WIN
            BOOL  bCompEnabled = FALSE;
            DwmIsCompositionEnabled(&bCompEnabled);
            setColor(bCompEnabled ? Qt::transparent : Qt::white);
            setAeroSupport ( bCompEnabled ? true : false);
#endif

#ifdef Q_OS_MAC
            quickWindowHelper = new QQuickWindowHelper(this);
            if (m_edgeResizeEnabled) {
                quickWindowHelper->activateOn(this);
            }
            connect(this, &QuickWindow::edgeResizeEnabledChanged, this, [&]{
                if (m_edgeResizeEnabled) {
                    quickWindowHelper->activateOn(this);
                } else {
                    quickWindowHelper->removeFrom(this);
                }
            });
#endif

            connect(this, &QQuickWindow::visibleChanged, [this]() {
                if (!this->isVisible() && this->destoryOnInvisible()) {
                    this->deleteLater();
                }
            });

            connect(this,&QWindow::screenChanged,this,[=](QScreen* screen){
                auto rect = screen->geometry();
                setScreenSize(rect.size());
            });
        }
        
        QuickWindow::~QuickWindow() {

        }
        
        void QuickWindow::moveToScreenCenter() {
            QRect rect = this->screen()->geometry();
            setScreenSize(rect.size());
            if (parent()) {
                rect = parent()->screen()->geometry();
            }

            this->setX((rect.width() - this->width()) / 2 + rect.x());
            this->setY((rect.height() - this->height()) / 2 + rect.y());
        }
        
        void QuickWindow::setTitleBar(QQuickItem *titleBar) {
            m_pTitleBar = titleBar;
        }

#ifdef Q_OS_WIN
        bool QuickWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
            /// return true; 表示Qt处理事件，不交给Window系统处理。
            /// return false; 表示Qt不处理事件，交给Window系统处理。
            if (!m_nativeEventEnabled) {
                return false;
            }
            
            const MSG *msg = static_cast<MSG*>(message);
            switch (msg->message) {
            case WM_DWMCOMPOSITIONCHANGED: {
                BOOL  bCompEnabled = FALSE;
                DwmIsCompositionEnabled(&bCompEnabled);
                setColor(bCompEnabled ? Qt::transparent : Qt::white);
                setAeroSupport ( bCompEnabled ? true : false);
                
                update();
            }break;
            case WM_CLOSE:{
                emit customClose();
                return true;
            }break;
            case WM_NCLBUTTONDBLCLK:
                /// 为了防止主窗口标题栏双击后全屏显示。
                if (this->flags() & Qt::WindowMaximizeButtonHint) {
                    if (this->visibility() == QWindow::Maximized) {
                        this->showNormal();
                    } else if (this->visibility() == QWindow::Windowed) {
                        this->showMaximized();
                    }
                    return true;
                }
                break;
            case WM_NCHITTEST:{

                qreal ratio = this->devicePixelRatio();
                const int HIT_BORDER = 8;

                int nativeX = (int)(short)LOWORD(msg->lParam);
                int nativeY = (int)(short)HIWORD(msg->lParam);

                QRect screenRect = this->screen()->geometry();
                int offsetX = screenRect.x();
                int offsetY = screenRect.y();

                int xPos = ((nativeX-offsetX)/ratio) - (this->x()-offsetX);
                int yPos = ((nativeY-offsetY)/ratio) - (this->y()-offsetY);
                if (m_edgeResizeEnabled) {
                    if (xPos > 0 && xPos < HIT_BORDER * 2 && yPos > 0 && yPos < HIT_BORDER * 2 ) {
                        *result = HTTOPLEFT;
                        return true;
                    }
                    if (xPos > (this->width() - HIT_BORDER * 2 ) && xPos < (this->width() - 0) && yPos > 0 && yPos < HIT_BORDER * 2 ) {
                        *result = HTTOPRIGHT;
                        return true;
                    }
                    if (xPos > 0 && xPos < HIT_BORDER * 2  && yPos > (this->height() - HIT_BORDER * 2 ) && yPos < (this->height() - 0)) {
                        *result = HTBOTTOMLEFT;
                        return true;
                    }
                    if (xPos > (this->width() - HIT_BORDER * 2 ) && xPos < (this->width() - 0) && yPos > (this->height() - HIT_BORDER * 2 ) && yPos < (this->height() - 0)) {
                        *result = HTBOTTOMRIGHT;
                        return true;
                    }
                    if (xPos > 0 && xPos < HIT_BORDER) {
                        *result = HTLEFT;
                        return true;
                    }
                    if (xPos > (this->width() - HIT_BORDER) && xPos < (this->width() - 0)) {
                        *result = HTRIGHT;
                        return true;
                    }
                    if (yPos > 0 && yPos < HIT_BORDER) {
                        *result = HTTOP;
                        return true;
                    }
                    if (yPos > (this->height() - HIT_BORDER) && yPos < (this->height() - 0)) {
                        *result = HTBOTTOM;
                        return true;
                    }
                }
                
                if (m_pTitleBar && m_pTitleBar->contains(QPointF(xPos, yPos))) {
                    auto cood = m_pTitleBar->mapFromGlobal(this->mapToGlobal(QPoint(xPos, yPos)));
                    if (m_pTitleBar->childAt(cood.x(), cood.y())) {
                        return false;
                    } else {
                        *result = HTCAPTION;
                        return true;
                    }
                }
                
                return false;
            }
                break;
            case WM_NCCALCSIZE:
                return true;
            case WM_NCLBUTTONDOWN:{
                qreal ratio = this->devicePixelRatio();
                const int HIT_BORDER = 8;

                int nativeX = (int)(short)LOWORD(msg->lParam);
                int nativeY = (int)(short)HIWORD(msg->lParam);

                QRect screenRect = this->screen()->geometry();
                int offsetX = screenRect.x();
                int offsetY = screenRect.y();

                int xPos = ((nativeX-offsetX)/ratio) - (this->x()-offsetX);
                int yPos = ((nativeY-offsetY)/ratio) - (this->y()-offsetY);
                
                if (m_pTitleBar && m_pTitleBar->contains(QPointF(xPos, yPos))) {
                    /// 代码放开之后拖动titleBar导致程序失去焦点，点击一次后才能获取焦点。
//                    QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonPress,QPointF(xPos,yPos),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
//                    qApp->postEvent(this,event);
                }
                return false;
            }
                break;
            default:
                break;
            }
            return QQuickWindow::nativeEvent(eventType,message,result);
        }
#endif
        
        bool QuickWindow::isNativeEventEnabled() {
            return m_nativeEventEnabled;
        }
        
        void QuickWindow::setNativeEventEnabled(bool b) {
            if (m_nativeEventEnabled != b) {
                m_nativeEventEnabled = b;
                emit this->nativeEventEnabledChanged();
            }
        }
        
        bool QuickWindow::isEdgeResizeEnabled() {
            return m_edgeResizeEnabled;
        }
        
        void QuickWindow::setEdgeResizeEnabled(bool b) {
            if (m_edgeResizeEnabled != b) {
                m_edgeResizeEnabled = b;
                emit this->edgeResizeEnabledChanged();
            }
        }
    }
}
