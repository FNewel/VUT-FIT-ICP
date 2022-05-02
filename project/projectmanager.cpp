#include "projectmanager.h"
#include <QApplication>
#include <QDebug>
#include "classelement.h"

ProjectManager::ProjectManager(QObject *parent) : QObject(parent)
{

}

void ProjectManager::newProject()
{
    qDebug() << "New Project";
}

void ProjectManager::openProject()
{
    qDebug() << "Open Project";
}

void ProjectManager::saveProject()
{
    qDebug() << "Save Project";
    qDebug() << "------------";
    foreach(auto test, class_scene->classes){
        qDebug() << test << " | " << test->name;

        foreach(auto test2, test->attributes){
            qDebug() << "+" << test2;
            qDebug() << "  |" << test2->type;
            qDebug() << "  |" << test2->value;

        }
        qDebug() << "-----";

        foreach(auto test3, test->methods){
            qDebug() << "+" << test3;
            qDebug() << "  |" << test3->type;
            qDebug() << "  |" << test3->value;
        }
    }
}

void ProjectManager::saveProjectAs()
{
    qDebug() << "Save Project As";
}

void ProjectManager::undoAction()
{
    qDebug() << "Undo action";
}

void ProjectManager::redoAction()
{
    qDebug() << "Redo action";
}

void ProjectManager::showDocs()
{
    qDebug() << "Show Documentation";
}

void ProjectManager::exitApp()
{
    QApplication::quit();
}
