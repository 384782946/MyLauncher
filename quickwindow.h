#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "qquickwindowhelper.h"

namespace GomeplusUI {
    namespace QMLType {
        class QuickWindow : public QQuickWindow {
            Q_OBJECT
        public:
            Q_PROPERTY(QSize screenSize READ screenSize WRITE setScreenSize NOTIFY screenSizeChanged)
            Q_PROPERTY(bool nativeEventEnabled MEMBER m_nativeEventEnabled READ isNativeEventEnabled WRITE setNativeEventEnabled NOTIFY nativeEventEnabledChanged) 
            Q_PROPERTY(bool edgeResizeEnabled MEMBER m_edgeResizeEnabled READ isEdgeResizeEnabled WRITE setEdgeResizeEnabled NOTIFY edgeResizeEnabledChanged) 
            Q_PROPERTY(double scale MEMBER m_scale READ scale CONSTANT)
            Q_PROPERTY(bool aeroSupport READ aeroSupport WRITE setAeroSupport NOTIFY aeroSupportChanged)
            Q_PROPERTY(bool destoryOnInvisible READ destoryOnInvisible WRITE setDestoryOnInvisible NOTIFY destoryOnInvisibleChanged)
            
            explicit QuickWindow(QWindow *parent = 0);
            virtual ~QuickWindow();

            Q_INVOKABLE void setTitleBar(QQuickItem *titleBar);

            Q_INVOKABLE void moveToScreenCenter();
            
            bool isNativeEventEnabled();
            void setNativeEventEnabled(bool);
            
            bool isEdgeResizeEnabled();
            void setEdgeResizeEnabled(bool);
            
            double scale() { return m_scale; }
            
            void initScale();

            bool aeroSupport() const
            {
                return m_aeroSupport;
            }
            
            bool destoryOnInvisible() const
            {
                return m_destoryOnInvisible;
            }
            
            QSize screenSize() const
            {
                return m_screenSize;
            }

        signals:
            void nativeEventEnabledChanged();
            void edgeResizeEnabledChanged();
            void customClose();
            void aeroSupportChanged(bool aeroSupport);
            
            void destoryOnInvisibleChanged(bool destoryOnInvisible);

            void screenSizeChanged(QSize screenSize);

        public slots:
            void setAeroSupport(bool aeroSupport)
            {
                if (m_aeroSupport == aeroSupport)
                    return;
                
                m_aeroSupport = aeroSupport;
                emit aeroSupportChanged(m_aeroSupport);
            }
            
            void setDestoryOnInvisible(bool destoryOnInvisible)
            {
                if (m_destoryOnInvisible == destoryOnInvisible)
                    return;
                
                m_destoryOnInvisible = destoryOnInvisible;
                emit destoryOnInvisibleChanged(m_destoryOnInvisible);
            }

            void setScreenSize(QSize screenSize)
            {
                if (m_screenSize == screenSize)
                    return;

                m_screenSize = screenSize;
                emit screenSizeChanged(m_screenSize);
            }

#ifdef Q_OS_WIN
        protected:
            virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif
        private:
            QQuickItem *m_pTitleBar = nullptr;
            
            QPoint m_pressPoint;
            bool m_bPress = false;
            
            bool m_nativeEventEnabled = true;
            bool m_edgeResizeEnabled = true;
            double m_scale = 1;
        private:
            QQuickWindowHelper* quickWindowHelper;
            bool m_aeroSupport = true;
            bool m_destoryOnInvisible = false;
            QSize m_screenSize;
        };
    }
}
