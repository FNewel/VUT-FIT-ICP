#include "workscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include "testwidget.h"
#include "classelement.h"

WorkScene::WorkScene(QObject *parent) : QGraphicsScene(parent)
{

}

void WorkScene::spawnNewClass(const QPointF local)
{
    ClassElement *classElementObject = new ClassElement();
    QGraphicsProxyWidget* proxyWidget2 = this->addWidget(classElementObject);
    proxyWidget2->setPos(local);

    //addRect(local.x(), local.y(), 10, 20);
}
