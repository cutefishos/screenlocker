#include "application.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.initialViewSetup();
    return app.exec();
}
