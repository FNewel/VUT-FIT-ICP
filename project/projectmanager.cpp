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

// save = true - keď sa vykoná nejaká vec
// save = false - keď klikne ctrlz, uloží kvôli redo
void ProjectManager::saveProjectNow(bool save)
{
    auto tempPath = QDir::currentPath() + "/temp";
    QString saveFile = "";

    // Check if temp file exist, else create one
    if (!QDir(tempPath).exists())
        QDir().mkdir(tempPath);

    // Save path for later use
    undoPath = tempPath;

    // Create path to file, if undo or operation is used
    if (save)
        saveFile = tempPath + "/temp_undo.json";
    else
        saveFile = tempPath + "/temp_redo.json";

    // Create save
    QByteArray jsonFile = createJson();

    QFile f(saveFile);
    f.open(QIODevice::WriteOnly);
    f.write(jsonFile);
    f.close();
}

void ProjectManager::newProject(int value)
{
    // Check if project is empty (clean)
    if(!class_scene->classes.empty()){

        if(value == 0){
            // Prompt to save project
            int reply = QMessageBox(QMessageBox::Information, "Save file", "Do you want to save this file?", QMessageBox::Yes|QMessageBox::No).exec();
            if (reply == 16384){    // Yes button clicked
                saveProject(0);
            }
        }
        else
            saveProject(value);

        // Delete everything    // TODO :(

        // Delete class diagram
        foreach(auto fClass, class_scene->classes){
            // Remove all attributes
            foreach(auto fAtt, fClass->attributes){
                fClass->attributes.remove(fClass->attributes.indexOf(fAtt));
                delete fAtt;
            }

            // Remove all methods
            foreach(auto fMeth, fClass->methods){
                fClass->methods.remove(fClass->methods.indexOf(fMeth));
                delete fMeth;
            }

            class_scene->classes.remove(class_scene->classes.indexOf(fClass));
            delete fClass;
        }

        // Remove all lines
        foreach(auto *line, lines){
            lines.remove(lines.indexOf(line));
            delete line->lineItem;
            delete line;
        }
    }
}

// flag = 0 - basic open project
// 1 - undo open
// 2 - redo open
void ProjectManager::openProject(int value)
{
    qDebug() << value;

    // Try create new empty project
    newProject(value);

    QString inputFilename = "";

    if (value == 0)
        inputFilename = QFileDialog::getOpenFileName();
    else if(value == 1)
        inputFilename = undoPath + "/temp_undo.json";
    else
        inputFilename = undoPath + "/temp_redo.json";

    // If no file is selected
    if(inputFilename.isEmpty())
        return;

    // Check if .json file is selected
    if(!inputFilename.endsWith(".json")){
        QMessageBox(QMessageBox::Warning, "Error!", "Wrong input file!", QMessageBox::Ok).exec();
        return;
    }

    // Save .json file path with name
    filename = inputFilename;

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
    QJsonArray cConnections = classDiagram.value("connections").toArray();

    // Iteration through all classes
    foreach(auto fClass, cClasses){
        QJsonObject tempClass = fClass.toObject();
        QString tempName = tempClass.value("name").toString();  // name -> str
        int tempSx = tempClass.value("position").toObject().value("x").toInt();
        int tempSy = tempClass.value("position").toObject().value("y").toInt();

        QJsonArray tempAtt = tempClass.value("attributes").toArray();   // attributes -> arr (name, type)
        QJsonArray tempMet = tempClass.value("methods").toArray();      // methods -> arr (name, type)


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

    // TODO: chýbaju lines a šípočky - done
    foreach(auto fConn, cConnections){
        auto sourcePos = class_scene->classes.at(fConn.toObject().value("source").toInt())->pos();
        auto targetPos = class_scene->classes.at(fConn.toObject().value("target").toInt())->pos();

        QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourcePos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        QApplication::sendEvent(class_scene->classes.at(fConn.toObject().value("source").toInt()), &event);
        QMouseEvent event2(QMouseEvent(QEvent::MouseButtonPress, targetPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        QApplication::sendEvent(class_scene->classes.at(fConn.toObject().value("target").toInt()), &event2);

        // Add source arrow
        class_scene->addLineArrow(0, class_scene->classes.at(fConn.toObject().value("source").toInt())->lineItems.last()->lineItem, fConn.toObject().value("source_arrow").toInt());
        // Add target arrow
        class_scene->addLineArrow(1, class_scene->classes.at(fConn.toObject().value("source").toInt())->lineItems.last()->lineItem, fConn.toObject().value("target_arrow").toInt());
    }

}

QByteArray ProjectManager::createJson()
{
    // Create json arrays to save data
    QJsonArray cData, cCon;

    foreach(auto fClass, class_scene->classes){
        QJsonArray attArr, metArr;

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

void ProjectManager::saveProject(int value)
{
    QString filen = "";

    if(value == 0){
        if (filename == "")
            saveProjectAs(true);
        filen = filename;
    }
    else if (value == 1)
        filen = undoPath + "/temp_redo.json";
    else
        filen = undoPath + "/temp_undo.json";

    QByteArray jsonFile = createJson();

    QFile f(filen);
    f.open(QIODevice::WriteOnly);
    f.write(jsonFile);
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

    saveProject(0);
}

void ProjectManager::undoAction()
{
    qDebug() << "Undo action";

    // Check if temp file exist, else return
    if (!QDir(undoPath).exists())
        return;
    auto saveFile = undoPath + "/temp_undo.json";
    if (!QFileInfo::exists(saveFile))
        return;

    openProject(1);
}

void ProjectManager::redoAction()
{
    qDebug() << "Redo action";

    // Check if temp file exist, else return
    if (!QDir(undoPath).exists())
        return;
    auto saveFile = undoPath + "/temp_redo.json";
    if (!QFileInfo::exists(saveFile))
        return;

    openProject(2);
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
            saveProject(0);
        }
    }

    QApplication::quit();
}
