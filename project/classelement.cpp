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
    //Update objects
    foreach(auto object, seq_scene->objects){
        object->updateClasses();
    }
    delete ui;
}

void ClassElement::linePosCheck(ClassLines *cLine, bool update)
{
    if(cLine->target == nullptr)
        return;

    int sPx = cLine->source->pos().x() + cLine->source->width()/2;
    int sPy = cLine->source->pos().y() + cLine->source->height()/2;
    int tPx = cLine->target->pos().x() + cLine->target->width()/2;
    int tPy = cLine->target->pos().y() + cLine->target->height()/2;
    int offset = 8;

    if (abs(sPx - tPx) > abs(sPy - tPy)){
        // Source X
        if (sPx - tPx < 0)
            cLine->sourcePos = QPoint(sPx + (cLine->source->width()/2) + offset, sPy);
        else
            cLine->sourcePos = QPoint(sPx - (cLine->source->width()/2) - offset, sPy);

        //Target X
        if (tPx - sPx < 0)
            cLine->targetPos = QPoint(tPx + (cLine->target->width()/2) + offset, tPy);
        else
            cLine->targetPos = QPoint(tPx - (cLine->target->width()/2) - offset, tPy);
    }
    else {
        // Source Y
        if (sPy - tPy < 0)
            cLine->sourcePos = QPoint(sPx, sPy + (cLine->source->height()/2) + offset);
        else
            cLine->sourcePos = QPoint(sPx, sPy - (cLine->source->height()/2) - offset);

        // Target Y
        if (tPy - sPy < 0)
            cLine->targetPos = QPoint(tPx, tPy + (cLine->target->height()/2) + offset);
        else
            cLine->targetPos = QPoint(tPx, tPy - (cLine->target->height()/2) - offset);
    }

    if(update){
        cLine->lineItem->setLine(cLine->sourcePos.x(), cLine->sourcePos.y(), cLine->targetPos.x(), cLine->targetPos.y());
    }
}

void ClassElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();

    // First object
    if(event->buttons() & Qt::RightButton && !isClicked) {
        ClassLines *line = new ClassLines();
        line->source = this;
        line->sourcePos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));
        lines.append(line);
        isClicked = true;

        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        // uloží do line, ClassLines
        foreach(ClassElement *c_name, class_scene->classes){
            if(c_name == this){
                c_name->lineItems.append(line);
            }
        }

    }
    // Second object
    else if (event->buttons() & Qt::RightButton && isClicked) {

        // Check if target is same as source
        // if so, remove it and delete
        if(lines.last()->source == this){
            lines.last()->source->lineItems.removeLast();
            delete lines.last();
            lines.removeLast();
            isClicked = false;
            return;
        }

        // Check if trying to create identical line
        // if so, remove it and delete
        foreach(auto linee, lines){
            if((linee->source == lines.last()->source && linee->target == this) || (linee->target == lines.last()->source && linee->source == this)){
                lines.last()->source->lineItems.removeLast();
                delete lines.last();
                lines.removeLast();
                isClicked = false;
                return;
            }
        }


        lines.last()->target = this;
        lines.last()->targetPos = QPoint(this->pos().x()+(this->width()/2), this->pos().y()+(this->height()/2));

        isClicked = false;
        linePosCheck(lines.last(), false);

        // draw line from source to target
        auto line = class_scene->addLine(QLine(lines.last()->sourcePos, lines.last()->targetPos));
        lines.last()->lineItem = line;
        line->setPen(QPen((Qt::black),3));
        line->setFlag(QGraphicsItem::ItemIsSelectable);
        line->setZValue(-1);
        line->setCursor(Qt::PointingHandCursor);

        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        // uloží do line, ClassLines
        foreach(ClassElement *c_name, class_scene->classes){
            if(c_name == this){
                c_name->lineItems.append(lines.last());
            }
        }
    }
}

void ClassElement::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));

        if (!lines.empty()){
            // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
            foreach(ClassElement *c_name, class_scene->classes){
                if(c_name == this){
                    foreach(auto *line, c_name->lineItems)
                        linePosCheck(line, true);
                }
            }
        }
    }
}

// Add method
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

    // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
    foreach(ClassElement *c_name, class_scene->classes){
        if(c_name == this){
            c_name->methods.append(newItem);
        }
    }

    gridLayout->addWidget(newItem);
}

// Add attribute
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

    // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
    foreach(ClassElement *c_name, class_scene->classes){
        if(c_name == this){
            c_name->attributes.append(newItem);
        }
    }

    gridLayout->addWidget(newItem);
}

// remove class
void ClassElement::on_pushButton_clicked()
{
    // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
    foreach(ClassElement *c_name, class_scene->classes){
        if(c_name == this){
            foreach(auto *line, c_name->lineItems){
                // Remove line from list in source element
                for(int iSrc = 0; iSrc < line->source->lineItems.count(); iSrc++){
                    if (line == line->source->lineItems.at(iSrc))
                        line->source->lineItems.remove(iSrc);
                }
                // Remove line from list in target element
                for(int iTrg = 0; iTrg < line->target->lineItems.count(); iTrg++){
                    if (line == line->target->lineItems.at(iTrg))
                        line->target->lineItems.remove(iTrg);
                }
                // Remove line and line class
                delete line->lineItem;
                delete line;
                lines.remove(lines.indexOf(line));
            }
        }
    }


    this->deleteLater(); //Using this instead of delete solves crashing on some machines
}

void ClassElement::on_name_input_textChanged(const QString &arg1)
{
    foreach(auto element, class_scene->classes){
        if(element == this){
            element->name = arg1;
        }
    }


    foreach(auto object, seq_scene->objects){
        object->updateClasses();
    }
}
