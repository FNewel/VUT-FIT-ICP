#include "workview.h"
#include "workscene.h"
#include "classelement.h"
#include <QGraphicsView>
#include <QMenu>
#include <QCursor>
#include <QPointF>
#include <QDebug>

WorkView::WorkView(QWidget *parent) : QGraphicsView(parent)
{
    connect(this ,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(workspaceViewCustomContextMenuRequested(const QPoint &)));

}


void WorkView::workspaceViewCustomContextMenuRequested(const QPoint &pos)
{
    if(this->itemAt(pos))
        return;

    QMenu menu(this);
    QAction *newClassAction = menu.addAction("New Class");
    connect(newClassAction, &QAction::triggered, [=](){ this->activeScene->spawnNewClass(mapToScene(pos));});
    menu.addAction("New Connection");
    menu.exec(this->mapToGlobal(pos));
}
