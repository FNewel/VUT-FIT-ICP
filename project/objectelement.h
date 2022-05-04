#ifndef OBJECTELEMENT_H
#define OBJECTELEMENT_H

#include <QComboBox>
#include <QMap>
#include <QWidget>
#include "workscene.h"

class MessageAnchor;
class SeqMessage;

extern WorkScene *class_scene;
extern WorkScene *seq_scene;
extern bool __gl__msgClicked;
extern QVector <SeqMessage *>__gl__messages;



namespace Ui {
class ObjectElement;
}

class ObjectElement : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectElement(QWidget *parent = nullptr);
    QGraphicsLineItem * createLifeLine(ObjectElement *objectPtr);

    void updateClasses();
    ~ObjectElement();

    Ui::ObjectElement *ui; //ui is public in this case so that message names can find current class of object

private:
    QPoint offset;
    QGraphicsLineItem * lifeLine = nullptr;
    MessageAnchor *leftAnchor = nullptr;
    QGraphicsProxyWidget *leftAnchorProxy = nullptr;
    MessageAnchor *rightAnchor = nullptr;
    QGraphicsProxyWidget *rightAnchorProxy = nullptr;

    //Dictionary of proxy widgets and message anchors
    QMap<QGraphicsProxyWidget*, MessageAnchor*> anchors;

    //Vector of proxies (keys) -> needed to find last added in dictionary
    QVector<QGraphicsProxyWidget *> proxyList;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private slots:

    void increaseLifeLine();
    void decreaseLifeLine();
    void deleteObject();

    void on_comboBox_currentTextChanged(const QString &arg1);
};


//Message Anchor Class
namespace Ui {
class MessageAnchor;
}

class MessageAnchor : public QWidget
{
    Q_OBJECT

public:
    explicit MessageAnchor(QWidget *parent = nullptr);
    ~MessageAnchor();
    QGraphicsProxyWidget * proxy = nullptr;
    SeqMessage *message = nullptr;

private:
    Ui::MessageAnchor *ui;
    QGraphicsItem* destructionIcon = nullptr;



protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void moveEvent(QMoveEvent *event);
};



//Mssage Class
class SeqMessage : public QWidget
{
    Q_OBJECT
public:
    explicit SeqMessage(QWidget *parent = nullptr);
    ~SeqMessage();

    void addComboBox();
    void updateMessages();
    void updateCBoxLoc();
    void updateArrowHead();
    void setArrow(int arrowType);

    QGraphicsLineItem *messageLine = nullptr;

    QComboBox *messageName = nullptr;
    QGraphicsProxyWidget *messageNameProxy = nullptr;
    QGraphicsItem *arrowHeadProxy = nullptr;

    MessageAnchor *sourceAnchor = nullptr;
    MessageAnchor *destAnchor = nullptr;

    QPoint sourcePos;
    QPoint destPos;

signals:

};


#endif // OBJECTELEMENT_H
