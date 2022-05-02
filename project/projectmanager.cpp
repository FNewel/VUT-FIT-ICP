#include "projectmanager.h"
#include <QApplication>
#include <QDebug>

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
