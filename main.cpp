#include <QApplication>

#include "taskmanager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TaskManager window;
    window.show();

    return app.exec();
}
