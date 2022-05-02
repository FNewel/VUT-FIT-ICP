#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = nullptr);

public slots:
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void undoAction();
    void redoAction();
    void showDocs();
    void exitApp();
};

#endif // PROJECTMANAGER_H
