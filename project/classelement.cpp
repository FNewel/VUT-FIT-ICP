#include "classelement.h"

bool isClicked = false;
QVector <ClassLines*>lines;

ClassElement::ClassElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassElement)
{
    ui->setupUi(this);
}

ClassElement::~ClassElement()
{
    //Remove pointer to this class element on destruction
    class_scene->classes.removeOne(this);
    delete ui;
}

void ClassElement::linePosCheck(ClassLines *cLines, bool update)
{
    int sPx = cLines->source->pos().x() + cLines->source->width()/2;
    int sPy = cLines->source->pos().y() + cLines->source->height()/2;
    int tPx = cLines->target->pos().x() + cLines->target->width()/2;
    int tPy = cLines->target->pos().y() + cLines->target->height()/2;
    int offset = 15;

    if (abs(sPx - tPx) > abs(sPy - tPy)){
        // Source X
        if (sPx - tPx < 0)
            cLines->sourcePos = QPoint(sPx + (cLines->source->width()/2) + offset, sPy);
        else
            cLines->sourcePos = QPoint(sPx - (cLines->source->width()/2) - offset, sPy);

        //Target X
        if (tPx - sPx < 0)
            cLines->targetPos = QPoint(tPx + (cLines->target->width()/2) + offset, tPy);
        else
            cLines->targetPos = QPoint(tPx - (cLines->target->width()/2) - offset, tPy);
    }
    else {
        // Source Y
        if (sPy - tPy < 0)
            cLines->sourcePos = QPoint(sPx, sPy + (cLines->source->height()/2) + offset);
        else
            cLines->sourcePos = QPoint(sPx, sPy - (cLines->source->height()/2) - offset);

        // Target Y
        if (tPy - sPy < 0)
            cLines->targetPos = QPoint(tPx, tPy + (cLines->target->height()/2) + offset);
        else
            cLines->targetPos = QPoint(tPx, tPy - (cLines->target->height()/2) - offset);
    }

    if(update){
        //qDebug() << "-> " << sPx << "|" << sPy << "|" << tPx << "|" << tPy;
        //qDebug() << "*> " << lines.last()->source->pos().x() << "|" << lines.last()->source->pos().y() ;


        foreach(auto line, cLines->lineItems){
            line->setLine(cLines->sourcePos.x(), cLines->sourcePos.y(), cLines->targetPos.x(), cLines->targetPos.y());
            qDebug() << cLines->lineItems.size();
        }
    }

}

void ClassElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();

    if(event->buttons() & Qt::RightButton && !isClicked) {
        ClassLines *line = new ClassLines();
        line->source = this;
        line->sourcePos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));
        lines.append(line);
        isClicked = true;

        //TODO--------
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        // uloží do line, ClassLines
        foreach(ClassElement *c_name, class_scene->classes){
            // qDebug() << "[.....]" << c_name << "|" << ui->frame << "|" << this;

            if(c_name == this){
                c_name->line = line;
            }
        }

    }
    else if (event->buttons() & Qt::RightButton && isClicked) {
        // TODO: asi nejaké checky čo vybral, úpravy súradníc a podobne
        lines.last()->target = this;
        lines.last()->targetPos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));

        isClicked = false;
        linePosCheck(lines.last(), false);

        // draw line from source to target
        auto line = class_scene->addLine(QLine(lines.last()->sourcePos, lines.last()->targetPos));
        line->setPen(QPen((Qt::black),3));
        line->setFlag(QGraphicsItem::ItemIsSelectable);
        line->setZValue(-1);
        line->setCursor(Qt::PointingHandCursor);

        lines.last()->lineItems.append(line);

        //TODO--------
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        // uloží do line, ClassLines
        foreach(ClassElement *c_name, class_scene->classes){
            // qDebug() << "[.....]" << c_name << "|" << ui->frame << "|" << this;

            if(c_name == this){
                c_name->line = lines.last();
            }
        }
    }
}

void ClassElement::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));
        // TODO --- Line update

        if (!lines.empty()){    // && lines.last()->lineItem != nullptr
            // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
            foreach(ClassElement *c_name, class_scene->classes){
                if(c_name == this){
                    linePosCheck(c_name->line, true);
                }
            }
        }
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
    this->deleteLater(); //Using this instead of delete solves crashing on some machines
}
