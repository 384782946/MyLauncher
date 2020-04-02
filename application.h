#pragma once

#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <functional>

class WindowController;
class ImageProvider;
class QSettings;

typedef std::function<void()> Functionnal;


class Application : public QGuiApplication{
    Q_OBJECT

signals:
    void applicationActivate();
public:
    explicit Application(int& argc, char *argv[]);
    ~Application();

    /**
     * @brief initQmlApplicationEngine
     * 初始化QML引擎
     */
    void initQmlApplicationEngine();


    /**
     * @brief registQmlAndMetaType
     * 注册QML和meta类型
     */
    void registQmlAndMetaType();

    /**
     * @brief startApplication
     * @return
     *        -<em>0</em> 程序正常退出
     */
    int startApplication();

    /**
     * @brief runOnMainThread
     * @param func
     * 在主线程中执行代码段
     */
    void runOnMainThread(Functionnal func) {
        QMetaObject::invokeMethod(this,"_runOnMainThread",Qt::QueuedConnection,Q_ARG(Functionnal,func));
    }


    /**
     * @brief getUserSetting
     * @param key
     * @return 获得用户设置中key对应的值
     */
    QVariant getUserSetting(const QString& key, const QVariant &defaultValue = QVariant());

    /**
     * @brief setUserSetting
     * @param key
     * @param data
     * 配置本地用户设置
     */
    void setUserSetting(const QString&key,QVariant data);

    /**
     * @brief removeUserSetting
     * @param key
     * 移除本地用户设置
     */
    void removeUserSetting(const QString& key);

protected:
    bool event(QEvent *e) override;

    Q_INVOKABLE void _runOnMainThread(Functionnal func) {
        func();
    }

private:
    /**
     * @brief initTornadoConfig
     * @return
     * 初始化tornado配置文件
     */
    int initTornadoConfig();

    void dealChatCmd(const QString &param);
public slots:

    /**
     * @brief closeApplication
     * 应用程序即将关闭时调用的接口
     */
    void closeApplication();

private:
    QQmlApplicationEngine *m_qmlApplicationEngine = nullptr; ///< QML引擎
    QSettings* m_userSettings = nullptr; ///< 用户设置
};
