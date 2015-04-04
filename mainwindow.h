#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStatusBar>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QAction>
#include <QSet>

#include "meshplot.h"
#include "mesh.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *);
private slots:
    void about();
    void setAntiAlias(bool aa);
    void setAxisEqual(bool ae);
    void setColorbar (bool sc);
    void setShowMesh (bool sm);
    void openMesh();
    void openSolution();
    void hilightDialog();
private:
    QLabel *statusLabel, *statusLabel2;
    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QAction *openMeshAction, *openSolutionAction;
    QAction *quitAction;
    QAction *antiaAction, *axiseqAction, *colorbarAction, *hilightAction, *showmeshAction;
    QAction *aboutAction;

    MeshPlot *meshplot;

    MeshFile *meshfile;
    Mesh     *mesh;

    QMap <int, QColor> hilightMap;
    QVector <double> solution;
    int numDoF;
};

#endif // MAINWINDOW_H
