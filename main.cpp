#include <QApplication>
#include <QIcon>

#include "./include/mainWindow.h"

#include "./include/sup.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  
        
    //DrawField window;
    //SettingPanel window;
    MainWindow window;

    window.setWindowTitle("lab2");
    window.resize(600, 400);
    window.setWindowIcon(QIcon("./imgsrc/ico.png"));

    window.show();

    return app.exec();
}