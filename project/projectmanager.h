#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = nullptr);

    QString filename = "";
    QString undoPath = "";

    void saveProjectNow(bool save);

public slots:
    void newProject(int value);
    void openProject(int value);    // flag = 0 - basic open project / 1 - undo/redo / 2 - redo open
    void saveProject(int value);
    void saveProjectAs(bool save);
    void undoAction();
    void redoAction();
    void showDocs();
    void exitApp();
    QByteArray createJson();
};

#endif // PROJECTMANAGER_H
