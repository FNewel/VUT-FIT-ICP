#include "itemobject.h"
#include "ui_itemobject.h"
#include "classelement.h"
#include <QDebug>
#include "projectmanager.h"

ItemObject::ItemObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemObject)
{
    ui->setupUi(this);
}

ItemObject::~ItemObject()
{
    foreach(SeqMessage *message, seq_scene->messages){
        message->updateMessages();
    }
    delete ui;
}

void ItemObject::on_removeButton_clicked()
{
    class_scene->projectManager->saveProjectNow(true);  // SAVE

    auto *opFrame = ui->horizontalLayout;
    auto *popFrame = opFrame->parentWidget()->parentWidget()->parentWidget();
    auto *op_popFrame = opFrame->parentWidget()->parentWidget()->parentWidget()->parentWidget();

    popFrame->setMinimumHeight(popFrame->height()-30);
    popFrame->setMaximumHeight(popFrame->minimumHeight()+30);
    popFrame->resize(popFrame->width(), popFrame->minimumHeight());

    op_popFrame->setMinimumHeight(op_popFrame->height()-30);
    op_popFrame->setMaximumHeight(op_popFrame->minimumHeight()+30);
    op_popFrame->resize(op_popFrame->width(), op_popFrame->minimumHeight());

    // Remove attribute and method from list in ClassElement
    if (this->parent()->objectName() == "operationFrame_attribute"){
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                for(int i = 0; i < c_name->attributes.length(); i++){
                    if(c_name->attributes.at(i) == this){
                        c_name->attributes.remove(i);
                        break;
                    }
                }
            }
        }
    }
    else {
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                for(int i = 0; i < c_name->methods.length(); i++){
                    if(c_name->methods.at(i) == this){
                        c_name->methods.remove(i);
                        break;
                    }
                }
            }
        }
    }

    delete this;
}

void ItemObject::on_comboBox_currentIndexChanged(int index)
{
    if (this->parent()->objectName() == "operationFrame_attribute"){
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                foreach(auto *c_att, c_name->attributes){
                    if(c_att == this){
                        c_att->type = index;
                    }
                }
            }
        }
    }
    else {
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                foreach(auto *c_met, c_name->methods){
                    if(c_met == this){
                        c_met->type = index;
                    }
                }
            }
        }
    }
}

void ItemObject::on_lineEdit_textChanged(const QString &arg1)
{
    if (this->parent()->objectName() == "operationFrame_attribute"){
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                foreach(auto *c_att, c_name->attributes){
                    if(c_att == this){
                        c_att->value = arg1;
                    }
                }
            }
        }
    }
    else {
        // nájde ClassElement v zozname všetkých vytvorených klás, ktorý je rovnaký ako kliknutý element
        foreach(auto *c_name, class_scene->classes){
            if(c_name == this->parent()->parent()->parent()){
                foreach(auto *c_met, c_name->methods){
                    if(c_met == this){
                        c_met->value = arg1;
                    }
                }
            }
        }
    }
    foreach(SeqMessage *message, seq_scene->messages){
        message->updateMessages();
    }
}
