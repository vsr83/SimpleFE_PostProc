/* Qt tool for the visualization of FE meshes and scalar data
   with contour curves.
   Copyright (C) 2015 Ville Räisänen <vsr at vsr.name>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
