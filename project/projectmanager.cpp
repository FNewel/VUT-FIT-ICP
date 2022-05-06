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
#include "ui_actorelement.h"
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

    //Class Diagram
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

    //Sequence Diagram
    QJsonObject seqDiagram = rootObj.value("seq_diagram").toObject();
    QJsonArray sObjects = seqDiagram.value("objects").toArray();
    QJsonArray sActors = seqDiagram.value("actors").toArray();
    QJsonArray sMessages = seqDiagram.value("messages").toArray();
    QJsonArray sActivations = seqDiagram.value("activations").toArray();

    foreach(auto sObject, sObjects){
        QJsonObject tempObject = sObject.toObject();
        QJsonArray tempDestructions = tempObject.value("destructions").toArray();
        QString tempName = tempObject.value("name").toString();  // name -> str
        QString tempClassName = tempObject.value("class").toString();  // class -> str
        int tempSx = tempObject.value("position").toObject().value("x").toInt();
        int tempSy = tempObject.value("position").toObject().value("y").toInt();
        int anchors = tempObject.value("anchors").toInt();


        // Create new Object element at position with proper name
        seq_scene->spawnNewObject(QPointF(tempSx, tempSy));
        seq_scene->objects.last()->ui->lineEdit->setText(tempName);

        //Make sure Class Still Exists (the name was not changed in json)
        bool class_ok = false;
        int classIndex = 0;
        foreach(ClassElement *classElement, class_scene->classes){
            if(classElement->name == tempClassName){
                seq_scene->objects.last()->ui->comboBox->setCurrentIndex(classIndex);
                class_ok = true;
            }
            classIndex++;
        }

        if(!class_ok){
            seq_scene->objects.last()->ui->comboBox->setCurrentText("///MISSING CLASS///");
        }


        // Add Anchors
        for(int i = 0; i < anchors; i++){
            QMetaObject::invokeMethod(seq_scene->objects.last()->ui->plusLineButton, "clicked");
        }

        foreach(auto sDestruction, tempDestructions){
            int anchor_id = sDestruction.toInt();
            QPoint anchorPos = QPoint(seq_scene->objects.last()->pos().x()/2,
                                      seq_scene->objects.last()->pos().y()+seq_scene->objects.last()->height()+(anchor_id+1)*50);
            QMouseEvent event(QMouseEvent(QEvent::MouseButtonDblClick, anchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            //DoubleClick
            QApplication::sendEvent(seq_scene->objects.last()->anchors[seq_scene->objects.last()->proxyList[anchor_id]],&event);
        }



    }

    foreach(auto sActor, sActors){
        QJsonObject tempActor = sActor.toObject();
        QJsonArray tempDestructions = tempActor.value("destructions").toArray();
        QString tempName = tempActor.value("name").toString();  // name -> str
        int tempSx = tempActor.value("position").toObject().value("x").toInt();
        int tempSy = tempActor.value("position").toObject().value("y").toInt();
        int anchors = tempActor.value("anchors").toInt();


        // Create new Actor element at position with proper name
        seq_scene->spawnNewActor(QPointF(tempSx, tempSy));
        seq_scene->actors.last()->ui->lineEdit->setText(tempName);

        // Add Anchors

        for(int i = 0; i < anchors; i++){

            QMetaObject::invokeMethod(seq_scene->actors.last()->ui->plusLineButton, "clicked");

        }

        //Add destruction Icons
        foreach(auto sDestruction, tempDestructions){
            int anchor_id = sDestruction.toInt();

            QPoint anchorPos = QPoint(seq_scene->actors.last()->pos().x()/2,
                                      seq_scene->actors.last()->pos().y()+seq_scene->actors.last()->height()+(anchor_id+1)*50);
            QMouseEvent event(QMouseEvent(QEvent::MouseButtonDblClick, anchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            //DoubleClick
            QApplication::sendEvent(seq_scene->actors.last()->anchors[seq_scene->actors.last()->proxyList[anchor_id]],&event);
        }

    }

    //Activations
    foreach(auto sActivation, sActivations){
        QJsonObject tempActivation = sActivation.toObject();
        int item = tempActivation.value("item").toInt();
        int item_type = tempActivation.value("item_type").toInt();
        int source = tempActivation.value("source").toInt();
        int target = tempActivation.value("target").toInt();

        if(item_type == 0){
            ObjectElement *srcObjectPtr = nullptr;
            srcObjectPtr = seq_scene->objects[item];

            //First Click
            QPoint sourceAnchorPos = srcObjectPtr->anchors[srcObjectPtr->proxyList[source]]->pos() + QPoint(8,8);

            QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            QApplication::sendEvent(srcObjectPtr->anchors[srcObjectPtr->proxyList[source]],&event);

            //Second Click
            QPoint destAnchorPos = srcObjectPtr->anchors[srcObjectPtr->proxyList[target]]->pos() + QPoint(8,8);

            event = (QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            QApplication::sendEvent(srcObjectPtr->anchors[srcObjectPtr->proxyList[target]],&event);


        }else if(item_type == 1){
            ActorElement *srcActorPtr = nullptr;
            srcActorPtr = seq_scene->actors[item];

            //First Click
            QPoint sourceAnchorPos = srcActorPtr->anchors[srcActorPtr->proxyList[source]]->pos() + QPoint(8,8);

            QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            QApplication::sendEvent(srcActorPtr->anchors[srcActorPtr->proxyList[source]],&event);

            //Second Click
            QPoint destAnchorPos = srcActorPtr->anchors[srcActorPtr->proxyList[target]]->pos() + QPoint(8,8);

            event = (QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
            QApplication::sendEvent(srcActorPtr->anchors[srcActorPtr->proxyList[target]],&event);

        }
    }

    //Messages
    foreach(auto sMessage, sMessages){

        QJsonObject tempMessage = sMessage.toObject();
        int target_class = tempMessage.value("target_class").toInt(); //TODO possibly not needed
        int target_method = tempMessage.value("id_method").toInt();
        int msg_type = tempMessage.value("msg_type").toInt();
        int src_object_id = tempMessage.value("source").toObject().value("id").toInt();
        int src_object_type = tempMessage.value("source").toObject().value("type").toInt();
        int src_position = tempMessage.value("source").toObject().value("position").toInt();
        int dst_object_id = tempMessage.value("target").toObject().value("id").toInt();
        int dst_object_type = tempMessage.value("target").toObject().value("type").toInt();
        int dst_position = tempMessage.value("target").toObject().value("position").toInt();



        // Create new Message from source to destination, set type


        //Get position of source anchor + click
        QPoint sourceAnchorPos;
        if(src_object_type == 0){
            ObjectElement *srcObjectPtr = nullptr;
            srcObjectPtr = seq_scene->objects[src_object_id];
            if(src_position >= 0){
                sourceAnchorPos = srcObjectPtr->anchors[srcObjectPtr->proxyList[src_position]]->pos() + QPoint(8,8);

                QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                QApplication::sendEvent(srcObjectPtr->anchors[srcObjectPtr->proxyList[src_position]],&event);
            }else if (src_position > -3){
                if(src_position == -1){

                    sourceAnchorPos = srcObjectPtr->leftAnchor->pos()+QPoint(8,8);

                    QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                    QApplication::sendEvent(srcObjectPtr->leftAnchor,&event);
                }else{

                    sourceAnchorPos = srcObjectPtr->rightAnchor->pos()+QPoint(8,8);

                    QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                    QApplication::sendEvent(srcObjectPtr->rightAnchor,&event);
                }
            }else{
                qDebug() << "Not supposed to happen";
            }

        }else if(src_object_type == 1){
            ActorElement *srcActorPtr = nullptr;
            srcActorPtr = seq_scene->actors[src_object_id];
            if(src_position >= 0){
                sourceAnchorPos = srcActorPtr->anchors[srcActorPtr->proxyList[src_position]]->pos() + QPoint(8,8);

                QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, sourceAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                QApplication::sendEvent(srcActorPtr->anchors[srcActorPtr->proxyList[src_position]],&event);
            }
        }

        //Get position of destination anchor + click
        QPoint destAnchorPos;
        if(dst_object_type == 0){
            ObjectElement *dstObjectPtr = nullptr;
            dstObjectPtr = seq_scene->objects[dst_object_id];
            if(dst_position >= 0){
                destAnchorPos = dstObjectPtr->anchors[dstObjectPtr->proxyList[dst_position]]->pos() + QPoint(8,8);

                QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                QApplication::sendEvent(dstObjectPtr->anchors[dstObjectPtr->proxyList[dst_position]],&event);
            }else if (dst_position > -3){
                if(dst_position == -1){

                    destAnchorPos = dstObjectPtr->leftAnchor->pos()+QPoint(8,8);

                    QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                    QApplication::sendEvent(dstObjectPtr->leftAnchor,&event);
                }else{
                    destAnchorPos = dstObjectPtr->rightAnchor->pos()+QPoint(8,8);

                    QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                    QApplication::sendEvent(dstObjectPtr->rightAnchor,&event);
                }
            }else{
                qDebug() << "Not supposed to happen";
            }
        }else if(dst_object_type == 1){
            ActorElement *dstActorPtr = nullptr;
            dstActorPtr = seq_scene->actors[dst_object_id];
            if(dst_position >= 0){
                destAnchorPos = dstActorPtr->anchors[dstActorPtr->proxyList[dst_position]]->pos() + QPoint(8,8);
                QMouseEvent event(QMouseEvent(QEvent::MouseButtonPress, destAnchorPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                QApplication::sendEvent(dstActorPtr->anchors[dstActorPtr->proxyList[dst_position]],&event);
            }
        }

        //Curretnly created message
        SeqMessage *createdMsg = seq_scene->messages.last();

        //Set message type
        createdMsg->setArrow(msg_type);

        //if target is an object, set method
        if(dst_object_type == 0){
            ClassElement *targetClass = nullptr;
            ObjectElement *dstObjectPtr = seq_scene->objects[dst_object_id];
            QString className = dstObjectPtr->ui->comboBox->currentText();

            foreach(ClassElement *classElement, class_scene->classes){
                if (classElement->name == className){
                    targetClass = classElement;
                }
            }
            //Check if class still exists
            if(targetClass){
                if(targetClass->methods.count() > target_method){
                    createdMsg->messageName->setCurrentIndex(target_method);
                }else{
                    createdMsg->messageName->setCurrentIndex(-1); //If class was found, but does not have enough methods, no index of method
                }

            }else{
                createdMsg->messageName->setCurrentIndex(-1); //If no class was found, no index of method
            }
        }




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


    //Clean unfinished activations and messages from vectors
    if(!seq_scene->activations.empty() && seq_scene->activations.last()->destAnchor == nullptr){
        delete seq_scene->activations.last();
        seq_scene->actClicked = false;
    }
    if(!seq_scene->messages.empty() && seq_scene->messages.last()->destAnchor == nullptr){
        delete seq_scene->messages.last();
        seq_scene->msgClicked = false;
    }

    QJsonArray sObjects, sActors, sMessages, sActivations;
    //Each Object
    foreach(ObjectElement* fObject, seq_scene->objects){

        QJsonArray oDestructions;
        QVector<int> destructions;
        int anchorAmount = 0;
        //For each anchor add to counter
        //For each desturction anchor, append to list of destructions
        foreach(QGraphicsProxyWidget *anchorProxy, fObject->proxyList){
            if(fObject->anchors[anchorProxy]->destructionIcon)
                destructions.append(anchorAmount);
            anchorAmount++;
        }

        foreach(int destruction, destructions){
              oDestructions.append(destruction);
        }

        // Append classes to array (with attributes and methods)
        sObjects.append(QJsonObject({
            {"name", fObject->name},
            {"class", fObject->ui->comboBox->currentText()},
            {"position", QJsonObject({{"x", fObject->pos().x()},
                                      {"y", fObject->pos().y()}})},
            {"anchors", anchorAmount},
            {"destructions", oDestructions}
        }));
    }

    foreach(ActorElement* fActor, seq_scene->actors){

        QJsonArray oDestructions;
        QVector<int> destructions;
        int anchorAmount = 0;
        //For each anchor add to counter
        //For each desturction anchor, append to list of destructions
        foreach(MessageAnchor *anchor, fActor->anchors){
            if(anchor->destructionIcon)
                destructions.append(anchorAmount);
            anchorAmount++;
        }

        foreach(int destruction, destructions){
              oDestructions.append(destruction);
        }

        // Append classes to array (with attributes and methods)
        sActors.append(QJsonObject({
            {"name", fActor->name},
            {"position", QJsonObject({{"x", fActor->pos().x()},
                                      {"y", fActor->pos().y()}})},
            {"anchors", anchorAmount},
            {"destructions", oDestructions}
        }));
    }

    foreach(SeqMessage* fMessage, seq_scene->messages){

        //If no class was found, -1 will be saved and therefore the message will have no method as well
        int classIndex = -1;
        int methodIndex = -1;

        //These should always exist so if somehow a -1 is saved there is an error with the code
        int srcObjectIndex = -1;
        int dstObjectIndex = -1;

        //Type of source/dest (object = 0, actor = 1)
        int srcType = -1;
        int dstType = -1;




        //These should also always exist but -1 and -2 are valid (left and right anchor of objects) so -3 signifies an error
        int srcAnchorIndex = -3;
        int dstAnchorIndex = -3;


        //Type of message (0 = Sync, 1 = Async, 2 = Return)
        int messageType = fMessage->messageType;

        //Only looking for destination classes if destination anchor belongs to an object
        //Actors do not have classes or methods
        //Also sets destination anchorIndex, objectIndex and type
        if(QString::fromUtf8(fMessage->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(fMessage->destAnchor->parent());
            QString className = objectElement->ui->comboBox->currentText();
            ClassElement* classPtr = nullptr;
            ItemObject *methodPtr = nullptr;

            dstObjectIndex = seq_scene->objects.indexOf(objectElement);
            dstType = 0;


            foreach(ClassElement *classElement, class_scene->classes){
                if(classElement->name == className){
                    classPtr = classElement;
                }
            }
            if(classPtr){
                classIndex = class_scene->classes.indexOf(classPtr); //If class was found, save index
                //Look for method ID
                foreach(ItemObject* method, classPtr->methods){
                    if(method->value == fMessage->messageName->currentText()){
                        methodPtr = method;
                    }
                    if(methodPtr){
                        methodIndex = class_scene->classes.at(class_scene->classes.indexOf(classPtr))->methods.indexOf(methodPtr);
                    }
                }
            }

            if(objectElement->leftAnchor == fMessage->destAnchor){
                dstAnchorIndex = -1;
            }else if(objectElement->rightAnchor == fMessage->destAnchor){
                dstAnchorIndex = -2;
            }else{
                MessageAnchor *anchorPtr = nullptr;
                foreach(MessageAnchor* anchor, objectElement->anchors){
                    if(anchor->proxy == fMessage->destAnchor->proxy)
                        anchorPtr = anchor;
                }
                if(anchorPtr){
                    dstAnchorIndex = objectElement->proxyList.indexOf(anchorPtr->proxy);
                }

            }


        }else if(QString::fromUtf8(fMessage->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(fMessage->destAnchor->parent());

            dstObjectIndex = seq_scene->actors.indexOf(actorElement);
            dstType = 1;

            MessageAnchor *anchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == fMessage->destAnchor->proxy)
                    anchorPtr = anchor;
            }
            if(anchorPtr){

                dstAnchorIndex = actorElement->proxyList.indexOf(anchorPtr->proxy);

            }
        }
        //Similiar to Dest lookup but only for destination item in json
        if(QString::fromUtf8(fMessage->sourceAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(fMessage->sourceAnchor->parent());

            srcObjectIndex = seq_scene->objects.indexOf(objectElement);
            srcType = 0;


            if(objectElement->leftAnchor == fMessage->sourceAnchor){
                srcAnchorIndex = -1;
            }else if(objectElement->rightAnchor == fMessage->sourceAnchor){
                srcAnchorIndex = -2;
            }else{
                MessageAnchor *anchorPtr = nullptr;
                foreach(MessageAnchor* anchor, objectElement->anchors){
                    if(anchor->proxy == fMessage->sourceAnchor->proxy)
                        anchorPtr = anchor;
                }
                if(anchorPtr){

                    srcAnchorIndex = objectElement->proxyList.indexOf(anchorPtr->proxy);
                }

            }


        }else if(QString::fromUtf8(fMessage->sourceAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(fMessage->sourceAnchor->parent());

            srcObjectIndex = seq_scene->actors.indexOf(actorElement);
            srcType = 1;

            MessageAnchor *anchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == fMessage->sourceAnchor->proxy)
                    anchorPtr = anchor;
            }
            if(anchorPtr){
                srcAnchorIndex = actorElement->proxyList.indexOf(anchorPtr->proxy);
            }

        }
        // Append classes to array (with attributes and methods)
        sMessages.append(QJsonObject({
            {"target_class", classIndex},
            {"id_method", methodIndex},
            {"msg_type", messageType},
            {"source", QJsonObject({{"id", srcObjectIndex},
                                    {"position", srcAnchorIndex},
                                    {"type", srcType}})},
            {"target", QJsonObject({{"id", dstObjectIndex},
                                    {"position", dstAnchorIndex},
                                    {"type", dstType}})}
        }));
    }

    foreach(ActivationElement *fActivation, seq_scene->activations){

        int objectIndex = -1; //Index of Actor/object
        int objectType = -1; // Actor/object
        int srcAnchor = -1; //Source anchor
        int dstAnchor = -1; //Destination anchor


        if(QString::fromUtf8(fActivation->destAnchor->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *objectElement = dynamic_cast<ObjectElement*>(fActivation->destAnchor->parent());

            objectIndex = seq_scene->objects.indexOf(objectElement);
            objectType = 0;

            MessageAnchor *sourceAnchorPtr = nullptr;
            MessageAnchor *destAnchorPtr = nullptr;
            foreach(MessageAnchor* anchor, objectElement->anchors){
                if(anchor->proxy == fActivation->sourceAnchor->proxy)
                    sourceAnchorPtr = anchor;
                if(anchor->proxy == fActivation->destAnchor->proxy)
                    destAnchorPtr = anchor;
            }
            if(sourceAnchorPtr && destAnchorPtr){

                srcAnchor = objectElement->proxyList.indexOf(sourceAnchorPtr->proxy);
                dstAnchor =  objectElement->proxyList.indexOf(destAnchorPtr->proxy);
            }

        }else if(QString::fromUtf8(fActivation->destAnchor->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *actorElement = dynamic_cast<ActorElement*>(fActivation->destAnchor->parent());
            objectIndex = seq_scene->actors.indexOf(actorElement);
            objectType = 1;

            MessageAnchor *sourceAnchorPtr = nullptr;
            MessageAnchor *destAnchorPtr = nullptr;
            foreach(MessageAnchor* anchor, actorElement->anchors){
                if(anchor->proxy == fActivation->sourceAnchor->proxy)
                    sourceAnchorPtr = anchor;
                if(anchor->proxy == fActivation->destAnchor->proxy)
                    destAnchorPtr = anchor;
            }
            if(sourceAnchorPtr && destAnchorPtr){

                srcAnchor = actorElement->proxyList.indexOf(sourceAnchorPtr->proxy);
                dstAnchor =  actorElement->proxyList.indexOf(destAnchorPtr->proxy);
            }
        }

        sActivations.append(QJsonObject({
            {"item", objectIndex},
            {"item_type", objectType},
            {"source", srcAnchor},
            {"target", dstAnchor}
        }));

    }



    QJsonObject jSequence{
        {"objects", sObjects},
        {"actors", sActors},
        {"messages", sMessages},
        {"activations", sActivations}
    };
    // Create main json with class and sequence diagrams
    QJsonObject mJson{
        {"class_diagram", jClasses},
        {"seq_diagram", jSequence}
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
    QVector<int> destructions;
    qDebug() << "Redo action";

    //Flush unfinished items from Activation and Message Vectors
    qDebug() << "Redo action";
    if(!seq_scene->activations.empty()){
        if(seq_scene->activations.last()->destAnchor == nullptr){
            delete seq_scene->activations.last();
            seq_scene->activations.removeLast();
            seq_scene->actClicked = false;

        }
    }
    qDebug() << "Redo action";
    if(!seq_scene->messages.empty()){
        if(seq_scene->messages.last()->destAnchor == nullptr){
            delete seq_scene->messages.last();
            seq_scene->messages.removeLast();
            seq_scene->msgClicked = false;

        }
    }




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
    qDebug() << "Anchors(amount):";

    foreach(ObjectElement* object, seq_scene->objects){
        int j = 0;
        foreach(MessageAnchor *anchor, object->anchors){
            if(anchor->destructionIcon)
                destructions.append(j);
            j++;
        }
        qDebug() << i << ": " << j;
        foreach(int destruction, destructions){
              qDebug() << i << "Dest: " << destruction;
        }
        i++;
    }
    i = 0;



    ///Actors////
    destructions.clear();
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
            if(anchor->destructionIcon)
                destructions.append(j);
            j++;
        }
        qDebug() << i << ": " << j;
        foreach(int destruction, destructions){
              qDebug() << i << "Dest: " << destruction;
        }
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
