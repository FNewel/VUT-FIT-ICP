#include "workscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include "testwidget.h"

WorkScene::WorkScene(QObject *parent) : QGraphicsScene(parent)
{

}

void WorkScene::spawnNewClass(const QPointF local)
{
    testWidget *testWidgetObject = new testWidget();//TEST WIDGET
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testWidgetObject);
    proxyWidget->setPos(local);
}

void WorkScene::spawnNewObject(const QPointF local)
{
    QLabel *testWidgetObject = new QLabel("TestObject"); //TEST QLABEL
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testWidgetObject);
    proxyWidget->setPos(local);
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
