#include "editor_ui.h"
#include "ui_editor_ui.h"
#include "workscene.h"
#include "workview.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QDebug>

#define MAX_WORKSPACE_SCALE 10



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->tabWidget);

    ui->classView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->seqView->setContextMenuPolicy(Qt::CustomContextMenu);

    InitWorkView(ui->classView);
    InitWorkView(ui->seqView);




}


MainWindow::~MainWindow()
{
    delete ui;
}

//Create new work scene
void MainWindow::InitWorkView(WorkView *workspaceView)
{
    auto *workspaceScene = new WorkScene(workspaceView);
    workspaceView->setScene(workspaceScene);
    //Maximum Workspace Size
    workspaceScene->setSceneRect(0,0,workspaceView->width()*MAX_WORKSPACE_SCALE, workspaceView->height()*MAX_WORKSPACE_SCALE);
    workspaceView->activeScene = workspaceScene;
}

