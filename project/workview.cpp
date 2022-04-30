#include "workview.h"
#include "workscene.h"
#include "classelement.h"
#include <QGraphicsView>
#include <QMenu>
#include <QCursor>
#include <QPointF>
#include <QObject>
#include <QDebug>


WorkView::WorkView(QWidget *parent) : QGraphicsView(parent)
{
    connect(this ,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(workspaceViewCustomContextMenuRequested(const QPoint &)));

}


void WorkView::workspaceViewCustomContextMenuRequested(const QPoint &pos)
{
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
}
