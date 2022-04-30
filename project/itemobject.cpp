#include "itemobject.h"
#include "ui_itemobject.h"
#include <QDebug>

ItemObject::ItemObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemObject)
{
    ui->setupUi(this);
}

ItemObject::~ItemObject()
{
    delete ui;
}

void ItemObject::on_removeButton_clicked()
{
    auto *opFrame = ui->horizontalLayout;
    auto *popFrame = opFrame->parentWidget()->parentWidget()->parentWidget();
    auto *op_popFrame = opFrame->parentWidget()->parentWidget()->parentWidget()->parentWidget();

    popFrame->setMinimumHeight(popFrame->height()-30);
    popFrame->setMaximumHeight(popFrame->minimumHeight()+30);
    popFrame->resize(popFrame->width(), popFrame->minimumHeight());

    op_popFrame->setMinimumHeight(op_popFrame->height()-30);
    op_popFrame->setMaximumHeight(op_popFrame->minimumHeight()+30);
    op_popFrame->resize(op_popFrame->width(), op_popFrame->minimumHeight());

    delete this;
}
