#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>
#include <QQuickView>
#include "authenticator.h"

struct org_kde_ksld;

class Application : public QGuiApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);
    ~Application();

    void initialViewSetup();

public slots:
    void desktopResized();
    void onScreenAdded(QScreen *screen);

private slots:
    void getFocus();
    void markViewsAsVisible(QQuickView *view);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWindow *getActiveScreen();
    void shareEvent(QEvent *e, QQuickView *from);
    void screenGeometryChanged(QScreen *screen, const QRect &geo);

private:
    Authenticator *m_authenticator;
    QList<QQuickView *> m_views;

    org_kde_ksld *m_ksldInterface = nullptr;
    bool m_testing = true;
};

#endif // APPLICATION_H
