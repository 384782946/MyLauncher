
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QSettings>
#include <QMutexLocker>
#include "application.h"
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <iostream>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    static QFile file;

    QMutexLocker locker(&mutex);
    if (!file.isOpen()) {

        QString logPath = QString("%1/%2/%3/log")
                .arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
                .arg(qApp->organizationName())
                .arg(qApp->applicationName());

        QDir logDir(logPath);
        if (!logDir.exists()) {
            logDir.mkpath(logPath);
        }

        file.setFileName(logPath + QString("/%1_app.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug");
        break;
    case QtWarningMsg:
        text = QString("Warning");
        break;
    case QtCriticalMsg:
        text = QString("Critical");
        break;
    case QtFatalMsg:
        text = QString("Fatal");
        break;
    case QtInfoMsg:
        text = QString("Info");
        break;
    }

    QString message = QString("[%1 File:%2,Line:%3 %4]    %5")
            .arg(text)
            .arg(QString::fromLocal8Bit(context.file))
            .arg(QString::number(context.line))
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(msg);

    QTextStream text_stream(&file);
    text_stream.setCodec("UTF-8");
    text_stream << message << "\r\n";
    file.flush();
}

Application::Application(int& argc, char *argv[]) :
    QGuiApplication(argc, argv) {
#ifndef QT_DEBUG
    qInstallMessageHandler(outputMessage);
#endif
}

Application::~Application() {
    qDebug() << QString::fromLocal8Bit("~Application 若异常退出请检查时候有残余的gomeplus进程。");
}

int Application::startApplication() {
    qDebug() << "\n\n\n\n\n"
             << QString("#").repeated(100)
             << "\nApplication::startApplication()";

    setQuitOnLastWindowClosed(false); //修复关闭窗口程序会退出的bug

    m_userSettings = new QSettings(organizationName(),applicationName(),this);

    m_qmlApplicationEngine = new QQmlApplicationEngine(this);

    /// 旨在解决某些机器在关闭登录窗口时无法退出程序的问题。
    connect(m_qmlApplicationEngine, &QQmlApplicationEngine::quit, [this]() {
        qDebug() << "before this->exit(0)";
        this->exit(0);
        qDebug() << "after this->exit(0)";
    });

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(closeApplication()));


    this->registQmlAndMetaType();
    this->initQmlApplicationEngine();

    return this->exec();
}

QVariant Application::getUserSetting(const QString &key, const QVariant &defaultValue)
{
    return m_userSettings->value(key, defaultValue);
}

void Application::setUserSetting(const QString &key, QVariant data)
{
    m_userSettings->setValue(key,data);
}

void Application::removeUserSetting(const QString &key)
{
    m_userSettings->remove(key);
}

void Application::closeApplication() {
    m_qmlApplicationEngine->quit();
    m_qmlApplicationEngine->deleteLater();
    m_qmlApplicationEngine = nullptr;

    qDebug() << "Application::closeApplication()";
}

void Application::initQmlApplicationEngine() {
    m_qmlApplicationEngine->addImportPath("qrc:/");
    m_qmlApplicationEngine->load("qrc:/main.qml");
}

void Application::registQmlAndMetaType() {
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<Functionnal>("Functionnal");
}

bool Application::event(QEvent *e) {
    switch (e->type()) {
    case QEvent::Close:
        break;
    case QEvent::FocusIn:
        break;
    case QEvent::ApplicationActivate:{
#ifdef Q_OS_MAC
        emit this->applicationActivate();
#endif
    }break;
    default:
        break;
    }
    return QCoreApplication::event(e);
}
