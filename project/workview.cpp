#include "workview.h"
#include "workscene.h"
#include "classelement.h"
#include <QGraphicsView>
#include <QMenu>
#include <QCursor>
#include <QPointF>
#include <QObject>
#include <QDebug>
#include <QGraphicsLineItem>


WorkView::WorkView(QWidget *parent) : QGraphicsView(parent)
{
    connect(this ,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(workspaceViewCustomContextMenuRequested(const QPoint &)));

}


void WorkView::workspaceViewCustomContextMenuRequested(const QPoint &pos)
{
    this->setRenderHint(QPainter::Antialiasing);    // Better lines

    auto list = class_scene->selectedItems();
    if(!list.empty()){
        QMenu menu(this);

        QMenu* subFirst = menu.addMenu("First connection");
        QMenu* subSecond = menu.addMenu("Second conneciton");

        QAction *firstLineAss = subFirst->addAction("Association");
        connect(firstLineAss, &QAction::triggered, [=](){ this->activeScene->addLineArrow(0, list.first(), 0);});
        QAction *firstLineGen = subFirst->addAction("Generalization");
        connect(firstLineGen, &QAction::triggered, [=](){ this->activeScene->addLineArrow(0, list.first(), 1);});
        QAction *firstLineCom = subFirst->addAction("Composition");
        connect(firstLineCom, &QAction::triggered, [=](){ this->activeScene->addLineArrow(0, list.first(), 2);});
        QAction *firstLineAgg = subFirst->addAction("Aggregation");
        connect(firstLineAgg, &QAction::triggered, [=](){ this->activeScene->addLineArrow(0, list.first(), 3);});

        QAction *secondLineAss = subSecond->addAction("Association");
        connect(secondLineAss, &QAction::triggered, [=](){ this->activeScene->addLineArrow(1, list.first(), 0);});
        QAction *secondLineGen = subSecond->addAction("Generalization");
        connect(secondLineGen, &QAction::triggered, [=](){ this->activeScene->addLineArrow(1, list.first(), 1);});
        QAction *secondLineCom = subSecond->addAction("Composition");
        connect(secondLineCom, &QAction::triggered, [=](){ this->activeScene->addLineArrow(1, list.first(), 2);});
        QAction *secondLineAgg = subSecond->addAction("Aggregation");
        connect(secondLineAgg, &QAction::triggered, [=](){ this->activeScene->addLineArrow(1, list.first(), 3);});

        QAction *newLineActionDel = menu.addAction("Remove line");
        connect(newLineActionDel, &QAction::triggered, [=](){ this->activeScene->removeLine(list.first());});

        menu.exec(this->mapToGlobal(pos));
        return;
    }

    if(this->itemAt(pos))
        return;

    //Context menu for class diagram
    if (sender()->objectName() == "classView"){
        QMenu menu(this);
        QAction *newClassAction = menu.addAction("New Class");
        connect(newClassAction, &QAction::triggered, [=](){ this->activeScene->spawnNewClass(mapToScene(pos));});
        QAction *newTextAction = menu.addAction("New Text");
        connect(newTextAction, &QAction::triggered, [=](){ this->activeScene->spawnNewText(mapToScene(pos));});
        menu.exec(this->mapToGlobal(pos));
    }

    //Context menu for sequence diagram
    if (sender()->objectName() == "seqView"){
        QMenu menu(this);
        QAction *newObjectAction = menu.addAction("New Object");
        connect(newObjectAction, &QAction::triggered, [=](){ this->activeScene->spawnNewObject(mapToScene(pos));});
        QAction *newActivationAction = menu.addAction("New Activation");
        connect(newActivationAction, &QAction::triggered, [=](){ this->activeScene->spawnNewActivation(mapToScene(pos));});
        QAction *newTextAction = menu.addAction("New Text");
        connect(newTextAction, &QAction::triggered, [=](){ this->activeScene->spawnNewText(mapToScene(pos));});
        menu.exec(this->mapToGlobal(pos));
    }

    qDebug() << sender()->objectName();
}
