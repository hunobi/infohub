#include <QApplication>
#include <ui/mainwindow.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a2a2a; border: none; margin: 2px;}");

    // create main window
    MainWindow window;
    window.show();

    return a.exec();
}
