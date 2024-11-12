#include <QApplication>
#include <QLabel>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLabel label;
    QPixmap pixmap("/home/debian/00-Linux.png");
    label.setPixmap(pixmap);
    label.show();

    return a.exec();
}