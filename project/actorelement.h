#ifndef ACTORELEMENT_H
#define ACTORELEMENT_H

#include "objectelement.h"

#include <QGraphicsLineItem>
#include <QWidget>

namespace Ui {
class ActorElement;
}

class ActorElement : public QWidget
{
    Q_OBJECT

public:
    explicit ActorElement(QWidget *parent = nullptr);
    ~ActorElement();
    QGraphicsLineItem * createLifeLine(ActorElement *actorPtr);
    //Dictionary of proxy widgets and message anchors
    QMap<QGraphicsProxyWidget*, MessageAnchor*> anchors;

    //Vector of proxies (keys) -> needed to find last added in dictionary
    QVector<QGraphicsProxyWidget *> proxyList;


private:
    Ui::ActorElement *ui;
    QPoint offset;
    QGraphicsLineItem * lifeLine = nullptr;



private slots:

    void increaseLifeLine();
    void decreaseLifeLine();
    void deleteActor();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // ACTORELEMENT_H
