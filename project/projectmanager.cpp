#include "projectmanager.h"
#include <QApplication>
#include <QDebug>
#include "classelement.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

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

QByteArray ProjectManager::createJson()
{
    // Create json arrays to save data
    QJsonArray attArr, metArr, cData, cCon;


    foreach(auto fClass, class_scene->classes){
        // tu mám klásu

        // Append attributes to array
        foreach(auto fAtt, fClass->attributes){
            attArr.append(QJsonObject({
                {"name", fAtt->value},
                {"type", fAtt->type}
            }));
        }

        // Append methods to array
        foreach(auto fMeth, fClass->methods){
            metArr.append(QJsonObject({
                  {"name", fMeth->value},
                  {"type", fMeth->type}
            }));
        }

        // Append classes to array (with attributes and methods)
        cData.append(QJsonObject({
            {"name", fClass->name},
            {"attributes", attArr},
            {"methods", metArr},
            {"position", QJsonObject({{"x", fClass->pos().x()},
                                      {"y", fClass->pos().y()}})}
        }));
    }

    // Append connections to array
    foreach(auto line, lines){
        cCon.append(QJsonObject({
            {"source", class_scene->classes.indexOf(line->source, 0)},
            {"target", class_scene->classes.indexOf(line->target, 0)},
            {"source_arrow", line->sourceConnection},
            {"target_arrow", line->targetConnection}
        }));
    }

    // Create main class object (with all informations)
    QJsonObject jClasses{
        {"classes", cData},
        {"connections", cCon}
    };

    // Create main json with class and sequence diagrams
    QJsonObject mJson{
        {"class_diagram", jClasses},
        {"seq_diagram", "wow such empty"}
    };

    QJsonDocument doc;
    doc.setObject(mJson);

    return doc.toJson();
}

void ProjectManager::saveProject()
{
    if (filename == "")
        saveProjectAs(false);

    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    f.write(createJson());
    f.close();
}

void ProjectManager::saveProjectAs(bool save)
{
    auto inputFilename = QFileDialog::getSaveFileName();
    if(!inputFilename.endsWith(".json"))
        filename = inputFilename + ".json";
    else
        filename = inputFilename;

    if(save)
        return;

    saveProject();
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
