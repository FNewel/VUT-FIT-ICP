/**
 * UML Editor - ICP Project 2022
 * @file main.cpp
 * @brief popis TODO
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/editor_ui.h"
#include "headers/projectmanager.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
