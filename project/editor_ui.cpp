#include "editor_ui.h"
#include "projectmanager.h"
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

    this->projectManager = new ProjectManager();

    bindSignals(projectManager);

    this->setCentralWidget(ui->tabWidget);

    ui->classView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->seqView->setContextMenuPolicy(Qt::CustomContextMenu);

    InitWorkView(ui->classView);
    InitWorkView(ui->seqView);

    //Set Class Diagram tab as the default
    ui->tabWidget->setCurrentIndex(0);

}


MainWindow::~MainWindow()
{
    delete this->projectManager;
    delete ui;
}

void MainWindow::bindSignals(ProjectManager *projectManager){
    connect(ui->actionNew_Project, &QAction::triggered, projectManager, &ProjectManager::newProject);
    connect(ui->actionOpen_Project , &QAction::triggered, projectManager, &ProjectManager::openProject);
    connect(ui->actionSave , &QAction::triggered, projectManager, &ProjectManager::saveProject);
    connect(ui->actionSave_As, &QAction::triggered, projectManager, &ProjectManager::saveProjectAs);
    connect(ui->actionUndo, &QAction::triggered, projectManager, &ProjectManager::undoAction);
    connect(ui->actionRedo, &QAction::triggered, projectManager, &ProjectManager::redoAction);
    connect(ui->actionExit, &QAction::triggered, projectManager, &ProjectManager::exitApp);
    connect(ui->actionDocumentation, &QAction::triggered, projectManager, &ProjectManager::showDocs);
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

