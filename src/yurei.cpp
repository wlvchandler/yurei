#include <QApplication>
#include <mainwindow.h>
#include <SDL.h>
#include <j16/j16.h>
#include <j16/runtime.h>

#if defined(_WIN32) || defined(_WIN64)
int SDL_main(int argc, char* argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
    std::cout << "HELLO" << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}