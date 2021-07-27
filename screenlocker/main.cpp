#include "application.h"
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    // Translations
    QLocale locale;
    QString qmFilePath = QString("%1/%2.qm").arg("/usr/share/cutefish-screenlocker/translations/").arg(locale.name());
    if (QFile::exists(qmFilePath)) {
        QTranslator *translator = new QTranslator(app.instance());
        if (translator->load(qmFilePath)) {
            app.installTranslator(translator);
        } else {
            translator->deleteLater();
        }
    }

    app.setQuitOnLastWindowClosed(false);
    app.initialViewSetup();
    return app.exec();
}
