#include "editor_ui.h"
#include "ui_editor_ui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->workspace);
}

MainWindow::~MainWindow()
{
    delete ui;
}

