#include "projectmanager.h"
#include <QApplication>
#include <QDebug>
#include "classelement.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "workscene.h"
#include "ui_itemobject.h"

ProjectManager::ProjectManager(QObject *parent) : QObject(parent)
{

}

void ProjectManager::newProject()
{
    // Check if project is empty (clean)
    if(!class_scene->classes.empty()){
        // Prompt to save project
        int reply = QMessageBox(QMessageBox::Information, "Save file", "Do you want to save this file?", QMessageBox::Yes|QMessageBox::No).exec();
        if (reply == 16384){    // Yes button clicked
            saveProject();
        }

        // Delete everything    // TODO :(
    }
}

void ProjectManager::openProject()
{
    qDebug() << "Open Project";
    // Try create new empty project
    newProject();

    auto inputFilename =QFileDialog::getOpenFileName();

    // If no file is selected
    if(inputFilename.isEmpty())
        return;

    // Check if .json file is selected
    if(!inputFilename.endsWith(".json")){
        QMessageBox(QMessageBox::Warning, "Error!", "Wrong input file!", QMessageBox::Ok).exec();
        return;
    }

    // Load .json file
    QFile inFile(inputFilename);
    inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = inFile.readAll();
    inFile.close();

    // Try to parse .json file
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        QMessageBox(QMessageBox::Warning, "Error!", "Input file not well-formed!", QMessageBox::Ok).exec();
        return;
    }

    // Process .json file
    QJsonObject rootObj = doc.object();
    QJsonObject classDiagram = rootObj.value("class_diagram").toObject();
    QJsonArray cClasses = classDiagram.value("classes").toArray();

    // Iteration through all classes
    foreach(auto fClass, cClasses){
        QJsonObject tempClass = fClass.toObject();
        QString tempName = tempClass.value("name").toString();  // name -> str
        int tempSx = tempClass.value("position").toObject().value("x").toInt();
        int tempSy = tempClass.value("position").toObject().value("y").toInt();

        QJsonArray tempAtt = tempClass.value("attributes").toArray();   // attributes -> arr (name, type)
        QJsonArray tempMet = tempClass.value("methods").toArray();   // methods -> arr (name, type)


        // Create new Class element at position with proper name
        class_scene->spawnNewClass(QPointF(tempSx, tempSy));
        class_scene->classes.last()->ui->name_input->setText(tempName);

        // Add attributes to class with proper name and type
        foreach(auto fAtt, tempAtt){
            QMetaObject::invokeMethod(class_scene->classes.last()->ui->attributeAddButton, "clicked");
            class_scene->classes.last()->attributes.last()->ui->lineEdit->setText(fAtt.toObject().value("name").toString());
            class_scene->classes.last()->attributes.last()->ui->comboBox->setCurrentIndex(fAtt.toObject().value("type").toInt());
        }

        // Add methods to class with proper name and type
        foreach(auto fMet, tempMet){
            QMetaObject::invokeMethod(class_scene->classes.last()->ui->operationAddButton, "clicked");
            class_scene->classes.last()->methods.last()->ui->lineEdit->setText(fMet.toObject().value("name").toString());
            class_scene->classes.last()->methods.last()->ui->comboBox->setCurrentIndex(fMet.toObject().value("type").toInt());
        }
    }

    // TODO: chýbaju lines a šípočky
}

QByteArray ProjectManager::createJson()
{
    // Create json arrays to save data
    QJsonArray attArr, metArr, cData, cCon;


    foreach(auto fClass, class_scene->classes){
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
    // Check if project is empty (clean)
    if(!class_scene->classes.empty()){
        // Prompt to save project
        int reply = QMessageBox(QMessageBox::Information, "Save file", "Do you want to save this file?", QMessageBox::Yes|QMessageBox::No).exec();
        if (reply == 16384){    // Yes button clicked
            saveProject();
        }
    }

    QApplication::quit();
}
