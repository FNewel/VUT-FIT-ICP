#include "classelement.h"
#include "ui_classelement.h"
#include "itemobject.h"
#include "classlines.h"
#include "QDebug"
#include <QPointer>

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

void ClassElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();

    if(event->buttons() & Qt::RightButton && !isClicked) {
        ClassLines line;
        line.source = ui->frame;
        line.sourcePos = event->pos();
        lines.append(line);
        isClicked = true;
    }
    else if (event->buttons() & Qt::RightButton && isClicked) {
        // TODO: asi nejaké checky čo vybral, úpravy súradníc a podobne
        lines.last().target = ui->frame;
        lines.last().targetPos = event->pos();
        // TODO: nejaké vykreslenie line
        isClicked = false;

        qDebug() << "------------------";
        qDebug() << lines.last().source;
        qDebug() << lines.last().sourcePos;
        qDebug() << lines.last().target;
        qDebug() << lines.last().targetPos;
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
