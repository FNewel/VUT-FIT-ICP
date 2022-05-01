#ifndef WORKVIEW_H
#define WORKVIEW_H

#include <QGraphicsView>
#include <QObject>

#include <workscene.h>

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
