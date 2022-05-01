#include "classelement.h"
#include "ui_classelement.h"
#include "itemobject.h"
#include "classlines.h"
#include "QDebug"
#include <QPointer>
#include <QLine>
#include <QPainter>
#include <QGraphicsLineItem>

bool isClicked = false;
QVector <ClassLines>lines;

ClassElement::ClassElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassElement)
{
    ui->setupUi(this);
}

ClassElement::~ClassElement()
{
    delete ui;
}

void ClassElement::linePosCheck()
{
    int sPx = lines.last().sourcePos.x();
    int sPy = lines.last().sourcePos.y();
    int tPx = lines.last().targetPos.x();
    int tPy = lines.last().targetPos.y();
    int offset = 15;

    if (abs(sPx - tPx) > abs(sPy - tPy)){
        // Source X
        if (sPx - tPx < 0)
            lines.last().sourcePos = QPoint(sPx + (lines.last().source->width()/2) + offset, sPy);
        else
            lines.last().sourcePos = QPoint(sPx - (lines.last().source->width()/2) - offset, sPy);

        //Source X
        if (tPx - sPx < 0)
            lines.last().targetPos = QPoint(tPx + (lines.last().target->width()/2) + offset, tPy);
        else
            lines.last().targetPos = QPoint(tPx - (lines.last().target->width()/2) - offset, tPy);
    }
    else {
        // Source Y
        if (sPy - tPy < 0)
            lines.last().sourcePos = QPoint(sPx, sPy + (lines.last().source->height()/2) + offset);
        else
            lines.last().sourcePos = QPoint(sPx, sPy - (lines.last().source->height()/2) - offset);

        // Target Y
        if (tPy - sPy < 0)
            lines.last().targetPos = QPoint(tPx, tPy + (lines.last().target->height()/2) + offset);
        else
            lines.last().targetPos = QPoint(tPx, tPy - (lines.last().target->height()/2) - offset);
    }
}

void ClassElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();

    if(event->buttons() & Qt::RightButton && !isClicked) {
        ClassLines line;
        line.source = ui->frame;
        line.sourcePos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));
        lines.append(line);
        isClicked = true;
    }
    else if (event->buttons() & Qt::RightButton && isClicked) {
        // TODO: asi nejaké checky čo vybral, úpravy súradníc a podobne
        lines.last().target = ui->frame;
        lines.last().targetPos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));

        isClicked = false;
        linePosCheck();

        // draw line from source to target
        auto line = main_scene->addLine(QLine(lines.last().sourcePos, lines.last().targetPos));
        line->setPen(QPen((Qt::black),3));
        line->setFlag(QGraphicsItem::ItemIsSelectable);     // TODO: dorobiť vymazávanie čiary
        line->setZValue(-1);
        line->setCursor(Qt::PointingHandCursor);

        qDebug() << line;
    }
}

void ClassElement::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));
    }
}

void ClassElement::on_operationAddButton_clicked()
{
    QFrame *opFrame = ui->operationFrame_operation;
    auto gridLayout = dynamic_cast<QGridLayout*>(opFrame->layout());
    auto *popFrame = opFrame->parentWidget();
    auto *op_popFrame = opFrame->parentWidget()->parentWidget();

    popFrame->setMinimumHeight(popFrame->height()+30);
    popFrame->setMaximumHeight(popFrame->minimumHeight()+30);
    popFrame->resize(popFrame->width(), popFrame->minimumHeight());

    op_popFrame->setMinimumHeight(op_popFrame->height()+30);
    op_popFrame->setMaximumHeight(op_popFrame->minimumHeight()+30);
    op_popFrame->resize(op_popFrame->width(), op_popFrame->minimumHeight());

    ItemObject *newItem = new ItemObject();

    gridLayout->addWidget(newItem);
}

void ClassElement::on_attributeAddButton_clicked()
{
    QFrame *opFrame = ui->operationFrame_attribute;
    auto gridLayout = dynamic_cast<QGridLayout*>(opFrame->layout());
    auto *popFrame = opFrame->parentWidget();
    auto *op_popFrame = opFrame->parentWidget()->parentWidget();

    popFrame->setMinimumHeight(popFrame->height()+30);
    popFrame->setMaximumHeight(popFrame->minimumHeight()+30);
    popFrame->resize(popFrame->width(), popFrame->minimumHeight());

    op_popFrame->setMinimumHeight(op_popFrame->height()+30);
    op_popFrame->setMaximumHeight(op_popFrame->minimumHeight()+30);
    op_popFrame->resize(op_popFrame->width(), op_popFrame->minimumHeight());

    ItemObject *newItem = new ItemObject();

    gridLayout->addWidget(newItem);
}

void ClassElement::on_pushButton_clicked()
{
    // TODO: tie čiary poriešiť
    delete this;
}
