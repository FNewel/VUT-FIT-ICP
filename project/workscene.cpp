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
    /*
    testWidget *testWidgetObject = new testWidget();//TEST WIDGET
    QGraphicsProxyWidget* proxyWidget = this->addWidget(testWidgetObject);
    proxyWidget->setPos(local);*/

    ClassElement *classElement = new ClassElement();//TEST WIDGET
    QGraphicsProxyWidget* proxyWidget = this->addWidget(classElement);
    proxyWidget->setPos(local);
    classes.append(classElement);
    qDebug() << classes;

    //testWidgetObject->main_scene = this;
    //main_scene = this;

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
    delete line;
}

void WorkScene::addLineFirstArrow(QGraphicsItem *line)
{
    qDebug() << "TODO: Add first arrow";
}

void WorkScene::addLineSecondArrow(QGraphicsItem *line)
{
    qDebug() << "TODO: Add second arrow";
}
