#include "workscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include "classelement.h"
#include "objectelement.h"
#include <QDebug>
#include <QGraphicsLineItem>

#define SYNC_ARROW 0
#define ASYNC_ARROW 1
#define RETURN_ARROW 2

WorkScene *class_scene;
WorkScene *seq_scene;

WorkScene::WorkScene(QObject *parent) : QGraphicsScene(parent)
{
    if (class_scene == nullptr)
        class_scene = this;
    else
        seq_scene = this;
}

WorkScene::~WorkScene(){
    foreach(ClassElement *test, classes){
        delete test;
    }
}

void WorkScene::spawnNewClass(const QPointF local)
{
    ClassElement *classElement = new ClassElement();
    QGraphicsProxyWidget* proxyWidget = this->addWidget(classElement);
    proxyWidget->setPos(local);
    classes.append(classElement);
}

void WorkScene::spawnNewObject(const QPointF local)
{
    /*QLabel *testWidgetObject = new QLabel("TestObject"); //TEST QLABEL
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testWidgetObject);
    proxyWidget->setPos(local);*/

    ObjectElement *objectElement = new ObjectElement();//TEST WIDGET
    QGraphicsProxyWidget* proxyWidget = this->addWidget(objectElement);
    proxyWidget->setPos(local);
    objects.append(objectElement);
}

void WorkScene::spawnNewText(const QPointF local)
{
    QLabel *testText = new QLabel("TestText");//TEST QLABEL
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testText);
    proxyWidget->setPos(local);
}

void WorkScene::spawnNewActivation(const QPointF local)
{
    QLabel *testActivation = new QLabel("TestActivation");//TEST WLABEL
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testActivation);
    proxyWidget->setPos(local);
}

void WorkScene::removeLine(QGraphicsItem *line)
{
    for (int i=0; i < lines.size(); i++){
        if(line == lines.value(i)->lineItem){
            foreach(ClassElement *c_name, class_scene->classes){
                // Remove line from source list
                if(c_name == lines.at(i)->source){
                    for(int ids = 0; ids < c_name->lineItems.length(); ids++){
                        if(c_name->lineItems.at(ids)->lineItem == line){
                            c_name->lineItems.remove(ids);
                        }
                    }
                }
                // Remove line from target list
                if(c_name == lines.at(i)->target){
                    for(int idt = 0; idt < c_name->lineItems.length(); idt++){
                        if(c_name->lineItems.at(idt)->lineItem == line){
                            c_name->lineItems.remove(idt);
                        }
                    }
                }
            }

            // Remove line from list
            lines.remove(i);
        }
    }
    delete line;
}


// where = 0 - source, 1 - target
// type = 0 - association, 1- generalization, 2 - composition, 3 - aggregation
void WorkScene::addLineArrow(int where, QGraphicsItem *line, int type)  // TODO: doriešiť pohyb šípiek plsssss :(
{
    int posX = 0;
    int posY = 0;
    int sPx = 0;
    int sPy = 0;
    int tPx = 0;
    int tPy = 0;
    int lineNum = 0;
    int rotate = 0;

    for (int i = 0; i < lines.size(); i++){
        if (lines.value(i)->lineItem == nullptr)
            continue;

        if (lines.value(i)->lineItem == line){
            sPx = lines.value(i)->sourcePos.x();
            sPy = lines.value(i)->sourcePos.y();
            tPx = lines.value(i)->targetPos.x();
            tPy = lines.value(i)->targetPos.y();

            lineNum = i;
        }
    }

    // Check if only one arrow (connection) is used
    if (lines.value(lineNum)->sourceConnection != 0 || lines.value(lineNum)->targetConnection != 0)
        return;

    // Set arrow (connection) to proper side
    if (where == 0)
        lines.value(lineNum)->sourceConnection = type;
    else
        lines.value(lineNum)->targetConnection = type;


    int offset = 32;

    if ((abs(sPx - tPx) > abs(sPy - tPy)) && type != 0){
        if (where == 0){
            // Source X - left
            if (sPx - tPx < 0){
                //source x++
                posX = sPx + offset;
                posY = sPy;
                rotate = 180;
                lines.value(lineNum)->lineItem->setLine(posX, sPy, tPx, tPy);
            }
            else{
                //source x--
                posX = sPx - offset;
                posY = sPy;
                lines.value(lineNum)->lineItem->setLine(posX, sPy, tPx, tPy);
            }
            lines.value(lineNum)->sourcePos.setX(posX);
            lines.value(lineNum)->sourcePos.setY(posY);
        }
        else {
            //Target X - left
            if (tPx - sPx < 0){
                //target x--
                posX = tPx + offset;
                posY = tPy;
                rotate = 180;
                lines.value(lineNum)->lineItem->setLine(sPx, sPy, posX, tPy);
            }
            else{
                //target x++
                posX = tPx - offset;
                posY = tPy;
                //rotate = 180;
                lines.value(lineNum)->lineItem->setLine(sPx, sPy, posX, tPy);
            }
            lines.value(lineNum)->targetPos.setX(posX);
            lines.value(lineNum)->targetPos.setY(posY);
        }
    }
    else if (type != 0){
        if (where == 0) {
            // Source Y - top
            if (sPy - tPy < 0){
                //source y++
                posX = sPx;
                posY = sPy + offset;
                rotate = 270;
                lines.value(lineNum)->lineItem->setLine(sPx, posY, tPx, tPy);
            }
            else{
                //source y--
                posX = sPx;
                posY = sPy - offset;
                rotate = 90;
                lines.value(lineNum)->lineItem->setLine(sPx, posY, tPx, tPy);
            }
            lines.value(lineNum)->sourcePos.setX(posX);
            lines.value(lineNum)->sourcePos.setY(posY);
        }
        else {
            // Target Y - top
            if (tPy - sPy < 0){
                //target y--
                posX = tPx;
                posY = tPy + offset;
                rotate = 270;
                lines.value(lineNum)->lineItem->setLine(sPx, sPy, tPx, posY);
            }
            else{
                //target y++
                posX = tPx;
                posY = tPy - offset;
                rotate = 90;
                lines.value(lineNum)->lineItem->setLine(sPx, sPy, tPx, posY);
            }
            lines.value(lineNum)->targetPos.setX(posX);
            lines.value(lineNum)->targetPos.setY(posY);
        }
    }

    QPolygon Gen, Com, Agg;
    Gen << QPoint(0, 16) << QPoint(32, 0) << QPoint(0, -16);
    Com << QPoint(16, 16) << QPoint(32, 0) << QPoint(16, -16) << QPoint(0, 0);
    Agg << QPoint(16, 16) << QPoint(32, 0) << QPoint(16, -16) << QPoint(0, 0);

    if (type == 1){
        QGraphicsPolygonItem *GenPoi = addPolygon(Gen, QPen(Qt::black), QBrush(Qt::white));
        GenPoi->setPos(posX, posY);
        GenPoi->setRotation(rotate);
        GenPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else if (type == 2) {
        QGraphicsPolygonItem *ComPoi = addPolygon(Com, QPen(Qt::black), QBrush(Qt::black));
        ComPoi->setPos(posX, posY);
        ComPoi->setRotation(rotate);
        ComPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else if (type == 3) {
        QGraphicsPolygonItem *AggPoi = addPolygon(Agg, QPen(Qt::black), QBrush(Qt::white));
        AggPoi->setPos(posX, posY);
        AggPoi->setRotation(rotate);
        AggPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else {
        qDebug() << "UH OH";    // TODO: ak to nebude treba, vymazať
    }
}

void WorkScene::setArrow(QGraphicsItem *msgLine, int arrowType)
{
    SeqMessage *message = nullptr;
    for (int i=0; i < __gl__messages.size(); i++){
        if(msgLine == __gl__messages.value(i)->messageLine){
            message = __gl__messages.value(i);
            break;
        }
    }

    if(message){
        switch (arrowType) {
            case SYNC_ARROW:
                message->setArrow(SYNC_ARROW);
            case ASYNC_ARROW:
                message->setArrow(ASYNC_ARROW);
            case RETURN_ARROW:
                message->setArrow(RETURN_ARROW);
            default:
            return;
        }
    }

}


void WorkScene::removeMessage(QGraphicsItem *msgLine)
{
    for (int i=0; i < __gl__messages.size(); i++){
        if(msgLine == __gl__messages.value(i)->messageLine){
            delete __gl__messages.value(i);
            __gl__messages.remove(i);
            break;
        }
    }
}


