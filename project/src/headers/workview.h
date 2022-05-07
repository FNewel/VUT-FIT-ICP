/**
 * UML Editor - ICP Project 2022
 * @file workview.h
 * @brief Header File for the WorkView Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef WORKVIEW_H
#define WORKVIEW_H

#include "workscene.h"

#include <QGraphicsView>
#include <QObject>

/**
* Work View Class.
* Class for Work Vies (sequence and class)
*/
class WorkView : public QGraphicsView
{
    Q_OBJECT
public:

    /**
    * Pointer to the currently displayed Work Scene in the view.
    */
    WorkScene *activeScene;

    /**
    *Consturctor for Work View.
    * @param parent Pointer to parent widget.
    */
    WorkView(QWidget *parent = nullptr);

protected slots:
    /**
    * Display custom context menu.
    * This slot is bound to a signal emitted from a pressed Right Mouse Button in an empty space of the currently displayed scene.
    */
    void workspaceViewCustomContextMenuRequested(const QPoint &pos);
};

#endif // WORKVIEW_H
