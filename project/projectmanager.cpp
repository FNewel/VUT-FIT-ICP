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
#include "ui_objectelement.h"
#include "actorelement.h"

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
    qDebug() << undoPath;

    if (save)
        saveProject(2);
    else
        saveProject(1);
}

void ProjectManager::newProject(int value)
{
    // Check if project is empty (clean)
    if(!class_scene->classes.empty()){

        if(value == 0){
            // Prompt to save project
            int reply = QMessageBox(QMessageBox::Information, "Save file", "Do you want to save this file?", QMessageBox::Yes|QMessageBox::No).exec();
            if (reply == 16384) // Yes button clicked
                saveProject(0);
        }
        else
            saveProject(value);

        // Delete class diagram
        foreach(auto *fClass, class_scene->classes){
            qDebug() << "oiiiii";
            // Remove all attributes
            foreach(auto *fAtt, fClass->attributes){
                fClass->attributes.remove(fClass->attributes.indexOf(fAtt));
                delete fAtt;
            }

            // Remove all methods
            foreach(auto *fMeth, fClass->methods){
                fClass->methods.remove(fClass->methods.indexOf(fMeth));
                delete fMeth;
            }

            class_scene->classes.remove(class_scene->classes.indexOf(fClass));
            qDebug() << fClass;
            fClass->ui->name_input->setText("ahoj");
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
    if (value == 0)
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
    int i = 0;
    qDebug() << "Redo action";
    ///Objects:///
    //Vector of objects
    qDebug() << "Objects Vector:" << seq_scene->objects;
    //Names of objects
    qDebug() << "Object Names:";
    foreach(ObjectElement* object, seq_scene->objects){
        qDebug() << i << ": " << object->name;
        i++;
    }
    i = 0;
    //ClassNames Of Objects
    qDebug() << "Class Names:";
    foreach(ObjectElement* object, seq_scene->objects){

            qDebug() << i << ": " << object->ui->comboBox->currentText();
            i++;
        }
    i = 0;
    //Position
    qDebug() << "Positions:";
    foreach(ObjectElement* object, seq_scene->objects){
            qDebug() << i << ": " << object->pos();
            i++;
        }
    i = 0;
    //Anchors - Amount
    qDebug() << "Anchors:";
    foreach(ObjectElement* object, seq_scene->objects){
        int j = 0;
        foreach(MessageAnchor *anchor, object->anchors){
                j++;
        }
        qDebug() << i << ": " << j;
            i++;
    }
    i = 0;

    ///Actors////
    //Vector of actors
    qDebug() << "Positions Vector:" << seq_scene->actors;
    //Names of actors
    qDebug() << "Actor Names:";
    foreach(ActorElement* actor, seq_scene->actors){
        qDebug() << i << ": " << actor->name;
        i++;
    }
    i = 0;
    //Position
    qDebug() << "Positions:";
    foreach(ActorElement* actor, seq_scene->actors){
            qDebug() << i << ": " << actor->pos();
            i++;
        }
    i = 0;
    //Anchors - Amount
    qDebug() << "Anchors:";
    foreach(ActorElement* actor, seq_scene->actors){
        int j = 0;
        foreach(MessageAnchor *anchor, actor->anchors){
                j++;
        }
        qDebug() << i << ": " << j;
            i++;
    }
    i = 0;

    ///Messages///
    //vector
    qDebug() << "Messages Vector:" << seq_scene->messages;
    //target classes
    qDebug() << "Target Classes (indexes in class vector):";
    foreach(SeqMessage* msg, seq_scene->messages){

        //Tu treba cast lebo message moze ist aj actorovi aj objektu
        if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(msg->destAnchor->parent());
            QString className = objectElement->ui->comboBox->currentText();
            ClassElement* classPtr = nullptr;

            foreach(ClassElement *classElement, class_scene->classes){
                if(classElement->name == className){
                    classPtr = classElement;
                }
            }
            if(classPtr){
                qDebug() << i << ": " << class_scene->classes.indexOf(classPtr);
            }else{
                qDebug() << i << ": " << ""; //Class may be empty
            }


        }else if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            qDebug() << i << ": " << ""; //Actor nema target class
        }
        i++;
    }
    i = 0;
    //method id
    qDebug() << "Methods (indexes in class->methods vector):";
    foreach(SeqMessage* msg, seq_scene->messages){

        //Tu treba cast lebo message moze ist aj actorovi aj objektu
        if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(msg->destAnchor->parent());

            QString className = objectElement->ui->comboBox->currentText();
            ClassElement* classPtr = nullptr;

            foreach(ClassElement *classElement, class_scene->classes){
                if(classElement->name == className){
                    classPtr = classElement;
                }
            }

            ItemObject *methodPtr = nullptr;
            if(classPtr){
                foreach(ItemObject* method, classPtr->methods){
                    if(method->value == msg->messageName->currentText()){
                        methodPtr = method;
                    }
                }
                if(methodPtr){
                    qDebug() << i << ": " << class_scene->classes.at(class_scene->classes.indexOf(classPtr))->methods.indexOf(methodPtr);
                }else{
                    qDebug() << i << ": " << ""; //Class exists but no method was selected
                }

            }else{
                qDebug() << i << ": " << ""; //Class may be empty, therefore message can be empty
            }

        }else if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            qDebug() << i << ": " << ""; //Actor nema target class a teda ani message
        }
        i++;
    }
    i = 0;

    //Source -> object Id + whuch anchor (-1 left, -2 right)
    qDebug() << "Source Anchor (object + number)";
    foreach(SeqMessage* msg, seq_scene->messages){

        //Tu treba cast lebo message moze ist aj actorovi aj objektu
        if(QString::fromUtf8(msg->sourceAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(msg->sourceAnchor->parent());

            qDebug() << i << ": " << "Object ID" << ": " <<seq_scene->objects.indexOf(objectElement);

            if(objectElement->leftAnchor == msg->sourceAnchor){
                qDebug() << i << ": " << "Anchor ID" << ": " << "-1";
            }else if(objectElement->rightAnchor == msg->sourceAnchor){
                qDebug() << i << ": " << "Anchor ID" << ": " << "-2";
            }else{
                MessageAnchor *anchorPtr = nullptr;
                foreach(MessageAnchor* anchor, objectElement->anchors){
                    if(anchor->proxy == msg->sourceAnchor->proxy)
                        anchorPtr = anchor;
                }
                if(anchorPtr){

                    qDebug() << i << ": " << "Anchor ID" << ": " << objectElement->proxyList.indexOf(anchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
                }

            }




        }else if(QString::fromUtf8(msg->sourceAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(msg->sourceAnchor->parent());
            qDebug() << i << ": " << "Actor ID" << ": " <<seq_scene->actors.indexOf(actorElement);

            MessageAnchor *anchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == msg->sourceAnchor->proxy)
                    anchorPtr = anchor;
            }
            if(anchorPtr){

                qDebug() << i << ": " << "Anchor ID" << ": " << actorElement->proxyList.indexOf(anchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
            }

        }
        i++;
    }
    i = 0;
    //Destination -> object Id + which anchor (-1 left, -2 right)
    qDebug() << "Destination Anchor (object + number)";
    foreach(SeqMessage* msg, seq_scene->messages){

        //Tu treba cast lebo message moze ist aj actorovi aj objektu
        if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(msg->destAnchor->parent());

            qDebug() << i << ": " << "Object ID" << ": " <<seq_scene->objects.indexOf(objectElement);

            if(objectElement->leftAnchor == msg->destAnchor){
                qDebug() << i << ": " << "Anchor ID" << ": " << "-1";
            }else if(objectElement->rightAnchor == msg->destAnchor){
                qDebug() << i << ": " << "Anchor ID" << ": " << "-2";
            }else{
                MessageAnchor *anchorPtr = nullptr;
                foreach(MessageAnchor* anchor, objectElement->anchors){
                    if(anchor->proxy == msg->destAnchor->proxy)
                        anchorPtr = anchor;
                }
                if(anchorPtr){

                    qDebug() << i << ": " << "Anchor ID" << ": " << objectElement->proxyList.indexOf(anchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
                }

            }




        }else if(QString::fromUtf8(msg->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(msg->destAnchor->parent());
            qDebug() << i << ": " << "Actor ID" << ": " <<seq_scene->actors.indexOf(actorElement);

            MessageAnchor *anchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == msg->destAnchor->proxy)
                    anchorPtr = anchor;
            }
            if(anchorPtr){

                qDebug() << i << ": " << "Anchor ID" << ": " << actorElement->proxyList.indexOf(anchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
            }

        }
        i++;
    }
    i = 0;

    ///Activations///
    //Item -> object Id + whuch anchor
    qDebug() << "Item (Object/Actor) + source and dest anchor";
    foreach(ActivationElement* act, seq_scene->activations){
        if(QString::fromUtf8(act->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(act->destAnchor->parent());

            qDebug() << i << ": " << "Object ID" << ": " <<seq_scene->objects.indexOf(objectElement);

            MessageAnchor *sourceAnchorPtr = nullptr;
            MessageAnchor *destAnchorPtr = nullptr;
            foreach(MessageAnchor* anchor, objectElement->anchors){
                if(anchor->proxy == act->sourceAnchor->proxy)
                    sourceAnchorPtr = anchor;
                if(anchor->proxy == act->destAnchor->proxy)
                    destAnchorPtr = anchor;
            }
            if(sourceAnchorPtr && destAnchorPtr){

                qDebug() << i << ": " << "Source Anchor ID" << ": " << objectElement->proxyList.indexOf(sourceAnchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
                qDebug() << i << ": " << "Dest Anchor ID" << ": " << objectElement->proxyList.indexOf(destAnchorPtr->proxy);
            }

        }else if(QString::fromUtf8(act->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(act->destAnchor->parent());
            qDebug() << i << ": " << "Actor ID" << ": " <<seq_scene->actors.indexOf(actorElement);

            MessageAnchor *sourceAnchorPtr = nullptr;
            MessageAnchor *destAnchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == act->sourceAnchor->proxy)
                    sourceAnchorPtr = anchor;
                if(anchor->proxy == act->destAnchor->proxy)
                    destAnchorPtr = anchor;
            }
            if(sourceAnchorPtr && destAnchorPtr){

                qDebug() << i << ": " << "Source Anchor ID" << ": " << actorElement->proxyList.indexOf(sourceAnchorPtr->proxy); //Toto zrobime spolu lebo tu je pouzity QMAP kde sa na seba viazu anchors a ich widgety
                qDebug() << i << ": " << "Dest Anchor ID" << ": " << actorElement->proxyList.indexOf(destAnchorPtr->proxy);
            }
            i++;
        }
    }

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
