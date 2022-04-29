#include "workscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include "testwidget.h"

WorkScene::WorkScene(QObject *parent) : QGraphicsScene(parent)
{

}

void WorkScene::spawnNewClass(const QPointF local)
{
    testWidget *testWidgetObject = new testWidget();
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testWidgetObject);
    proxyWidget->setPos(local);
    //addRect(local.x(), local.y(), 10, 20);
}
