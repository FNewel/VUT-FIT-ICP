/**
 * UML Editor - ICP Project 2022
 * @file workview.h
 * @brief popis TODO
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef WORKVIEW_H
#define WORKVIEW_H

#include "workscene.h"

#include <QGraphicsView>
#include <QObject>


class WorkView : public QGraphicsView
{
    Q_OBJECT
public:

    WorkScene *activeScene;

    WorkView(QWidget *parent = nullptr);

protected slots:
    void workspaceViewCustomContextMenuRequested(const QPoint &pos);

signals:
    void spawnObject(int, int);
};

#endif // WORKVIEW_H
