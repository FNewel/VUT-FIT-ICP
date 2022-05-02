#include "workscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include "testwidget.h"
#include "classelement.h"
#include "objectelement.h"
#include <QDebug>
#include <QGraphicsLineItem>

WorkScene *class_scene;
WorkScene *seq_scene;

WorkScene::WorkScene(QObject *parent) : QGraphicsScene(parent)
{
    if (class_scene == nullptr)
        class_scene = this;
    else
        seq_scene = this;
}

void WorkScene::spawnNewClass(const QPointF local)
{
    ClassElement *classElement = new ClassElement();//TEST WIDGET
    QGraphicsProxyWidget* proxyWidget = this->addWidget(classElement);
    proxyWidget->setPos(local);
    classes.append(classElement);
    qDebug() << classes;
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
    qDebug() << objects;
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
            lines.remove(i);
        }
    }
    delete line;
}


// where = 0 - source, 1 - target
// type = 0 - generalization, 1 - composition, 2 - aggregation, 3 - association
void WorkScene::addLineArrow(int where, QGraphicsItem *line, int type)
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
            qDebug() << "ty si tá pravá";
            sPx = lines.value(i)->sourcePos.x();
            sPy = lines.value(i)->sourcePos.y();
            tPx = lines.value(i)->targetPos.x();
            tPy = lines.value(i)->targetPos.y();

            lineNum = i;
        }
    }

    int offset = 32;

    if ((abs(sPx - tPx) > abs(sPy - tPy)) && type != 3){
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
    else if (type != 3){
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

    if (type == 0){
        QGraphicsPolygonItem *GenPoi = addPolygon(Gen, QPen(Qt::black), QBrush(Qt::white));
        GenPoi->setPos(posX, posY);
        GenPoi->setRotation(rotate);
        GenPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else if (type == 1) {
        QGraphicsPolygonItem *ComPoi = addPolygon(Com, QPen(Qt::black), QBrush(Qt::black));
        ComPoi->setPos(posX, posY);
        ComPoi->setRotation(rotate);
        ComPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else if (type == 2) {
        QGraphicsPolygonItem *AggPoi = addPolygon(Agg, QPen(Qt::black), QBrush(Qt::white));
        AggPoi->setPos(posX, posY);
        AggPoi->setRotation(rotate);
        AggPoi->setParentItem(lines.value(lineNum)->lineItem);
    }
    else {
        qDebug() << "UH OH";
    }
}

