/**
 * UML Editor - ICP Project 2022
 * @file workview.cpp
 * @brief Source File for the WorkView Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/workview.h"
#include "headers/workscene.h"
#include "headers/classelement.h"
#include "headers/objectelement.h"

#include <QGraphicsView>
#include <QMenu>
#include <QCursor>
#include <QPointF>
#include <QObject>
#include <QGraphicsLineItem>


#define SYNC_ARROW 0
#define ASYNC_ARROW 1
#define RETURN_ARROW 2


WorkView::WorkView(QWidget *parent) : QGraphicsView(parent)
{
    //Set up custom context menu tag on creation
    connect(this ,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(workspaceViewCustomContextMenuRequested(const QPoint &)));
    this->setRenderHint(QPainter::Antialiasing);    // Better lines
}


void WorkView::workspaceViewCustomContextMenuRequested(const QPoint &pos)
{
    auto list = class_scene->selectedItems();
    //Custom menu for lines in the class diagram
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

    //Context menu for selected message
    if (!seq_scene->selectedItems().isEmpty()){
        auto message = seq_scene->selectedItems().last();
        if(message != nullptr){
            QMenu menu(this);

            QAction *syncArrowAction = menu.addAction("Sync Arrow");
            connect(syncArrowAction, &QAction::triggered, [=](){ this->activeScene->setArrow(message, SYNC_ARROW);});
            QAction *asyncArrowAction = menu.addAction("Async Arrow");
            connect(asyncArrowAction, &QAction::triggered, [=](){ this->activeScene->setArrow(message, ASYNC_ARROW);});
            QAction *returnArrowAction = menu.addAction("Return Arrow");
            connect(returnArrowAction, &QAction::triggered, [=](){ this->activeScene->setArrow(message, RETURN_ARROW);});
            QAction *removeMessageAction = menu.addAction("Remove Message");
            connect(removeMessageAction, &QAction::triggered, [=](){ this->activeScene->removeMessage(message);});

            menu.exec(this->mapToGlobal(pos));
        }
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
        QAction *newActorAction = menu.addAction("New Actor");
        connect(newActorAction, &QAction::triggered, [=](){ this->activeScene->spawnNewActor(mapToScene(pos));});
        QAction *newObjectAction = menu.addAction("New Object");
        connect(newObjectAction, &QAction::triggered, [=](){ this->activeScene->spawnNewObject(mapToScene(pos));});
        QAction *newTextAction = menu.addAction("New Text");
        connect(newTextAction, &QAction::triggered, [=](){ this->activeScene->spawnNewText(mapToScene(pos));});
        menu.exec(this->mapToGlobal(pos));
    }
}

