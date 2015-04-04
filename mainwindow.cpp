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

#include "mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    statusLabel = new QLabel("Status");
    statusLabel2 = new QLabel("--");

    statusBar()->addWidget(statusLabel, 1);
    statusBar()->addWidget(statusLabel2);

    openMeshAction = new QAction(tr("Open &Mesh"), this);
    openMeshAction->setShortcut(tr("Ctrl+M"));
    connect(openMeshAction, SIGNAL(triggered()), SLOT(openMesh()));

    openSolutionAction = new QAction(tr("Open &Solution"), this);
    openSolutionAction->setShortcut(tr("Ctrl+S"));
    connect(openSolutionAction, SIGNAL(triggered()), SLOT(openSolution()));

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openMeshAction);
    fileMenu->addAction(openSolutionAction);
    fileMenu->addAction(quitAction);
//    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    optionsMenu = menuBar()->addMenu(tr("&Options"));

    showmeshAction = new QAction(tr("Show Mesh"), this);
    showmeshAction->setCheckable(true);
    showmeshAction->setChecked(true);
    optionsMenu->addAction(showmeshAction);
    connect(showmeshAction, SIGNAL(toggled(bool)), this, SLOT(setShowMesh(bool)));


    antiaAction = new QAction(tr("&Antialiasing"), this);
    antiaAction->setCheckable(true);
    antiaAction->setChecked(true);
    optionsMenu->addAction(antiaAction);
    connect(antiaAction, SIGNAL(toggled(bool)), this, SLOT(setAntiAlias(bool)));

    axiseqAction = new QAction(tr("Force Axis Equal"), this);
    axiseqAction->setCheckable(true);
    axiseqAction->setChecked(true);
    optionsMenu->addAction(axiseqAction);
    connect(axiseqAction, SIGNAL(toggled(bool)), this, SLOT(setAxisEqual(bool)));

    colorbarAction = new QAction(tr("Show Colorbar"), this);
    colorbarAction->setCheckable(true);
    colorbarAction->setChecked(true);
    optionsMenu->addAction(colorbarAction);
    connect(colorbarAction, SIGNAL(toggled(bool)), this, SLOT(setColorbar(bool)));

    hilightAction = new QAction(tr("Hilight Region"), this);
    hilightAction->setShortcut(tr("Ctrl+H"));
    optionsMenu->addAction(hilightAction);
    connect(hilightAction, SIGNAL(triggered()), SLOT(hilightDialog()));


    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    meshplot = new MeshPlot;
    meshplot->setFocus();
    meshplot->setFocusPolicy(Qt::StrongFocus);
    this->setCentralWidget(meshplot);
    resize(800, 600);
    meshfile = 0;
    mesh = 0;

    numDoF = 0;
}

void
MainWindow::about() {
    QMessageBox::about(this, tr("About"),
                       tr("<h2>SimpleFE Postprocessor 0.1</h2>"
                          "<p>Copyright &copy; 2015 Ville Räisänen vsr@vsr.name</p>"
                          "<p>See COPYING for license information</p>"));
}

void
MainWindow::setShowMesh(bool sm) {
    if (meshplot->visdata.numLayers > 0) {
        meshplot->visdata.layers[0]->linesVisible = sm;
        meshplot->refreshPixmap();
    }
}

void
MainWindow::setAntiAlias(bool aa) {
    meshplot->setAntiAliasing(aa);
    meshplot->refreshPixmap();
}

void
MainWindow::setAxisEqual(bool ae) {
    meshplot->setAxisEqual(ae);
    if (ae) {
        meshplot->fixAspectRatio();
        meshplot->refreshPixmap();
    }
}

void
MainWindow::setColorbar(bool sc) {
    meshplot->setShowColorbar(sc);
    meshplot->refreshPixmap();
}

void
MainWindow::hilightDialog() {
    bool ok;
    int phys = QInputDialog::getInt(this, tr("Hilight Region"),
                                    tr("Hilight Region"),
                                    1, 1, 1e9, 1, &ok);
    if (ok && meshplot->visdata.numLayers > 0 && mesh) {
        QColor hilightColor(160, 160, 160);

        if (hilightMap.contains(phys)) {
            hilightMap.remove(phys);
        } else {
            hilightMap[phys] = hilightColor;
        }
        meshplot->visdata.layers[0]->polygons.clear();
        meshplot->visdata.layers[0]->numPolygons = 0;
        meshplot->visdata.layers[0]->colorPhysicals(mesh, hilightMap);
        meshplot->refreshPixmap();
    }
}

void
MainWindow::openMesh() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Mesh File"),
                                                    ".", tr("Mesh Files (*.msh)"));

    QFileInfo info(filename);
    if (!info.exists()) {
        return;
    }

    if (mesh) {
        hilightMap.clear();
        delete meshfile;
        delete mesh;
        meshplot->clearData();
        meshfile = new MeshFile(qPrintable(filename));
        mesh = new Mesh(meshfile);
    } else {
        meshfile = new MeshFile(qPrintable(filename));
        mesh = new Mesh(meshfile);
    }

    QString qs;
    QString file = QFileInfo(filename).fileName();
    qs = qs.sprintf("%s, %d Nodes, %d Triangles", qPrintable(file),
                          mesh->num_nodes, mesh->num_triangles);
    statusLabel->setText(qs);

    meshplot->loadMesh(mesh);
    meshplot->show();
    meshplot->fixAspectRatio();
    meshplot->refreshPixmap();
}

void
MainWindow::openSolution() {
    if (!mesh) {
        QMessageBox::critical(this, tr("No Mesh File Open!"),
                              tr("Cannot load solution\n"
                                 "No mesh file open."), QMessageBox::Ok, 0);
    } else {
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Solution File"),
                                                    ".", tr("Solution Files *"));
        QFileInfo info(filename);
        if (!info.exists()) {
            return;
        }

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        double numDoF_tmp = 0;
        QVector <double> solution_tmp;
        solution_tmp.reserve(mesh->num_nodes);
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            line.remove(line.length()-1, 1);
            solution_tmp.push_back(line.toDouble());
            numDoF_tmp++;
        }
        bool ok;
        int numContours = QInputDialog::getInt(this, tr("Number of Contour Lines"),
                                               tr("Number of Contour Lines"),
                                               20, 1, 1000, 1, &ok);
        if (ok) {
            if (numDoF_tmp == mesh->num_nodes) {
                if (meshplot->visdata.numLayers >= 2) {
                    for (int ind_layer = 1; ind_layer<meshplot->visdata.numLayers; ind_layer++) {
                        meshplot->visdata.layers.remove(ind_layer);
                    }
                    meshplot->visdata.numLayers = 1;
                }
                numDoF = numDoF_tmp;
                solution = solution_tmp;

                Layer *contourLayer = new Layer;
                contourLayer->createContours(mesh, solution, numContours);
                meshplot->visdata.layers.push_back(contourLayer);
                meshplot->visdata.numLayers++;
                meshplot->refreshPixmap();
            } else {
                QMessageBox::critical(this, tr("Invalid File!"),
                                      tr("Number of lines doesn't match with the\n"
                                         "number of mesh nodes."), QMessageBox::Ok, 0);
            }
        }
    }
}

void
MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << event->key();
}

MainWindow::~MainWindow() {
    if (meshfile) {
        delete meshfile;
        meshfile = 0;
    }
    if (mesh) {
        delete mesh;
        mesh = 0;
    }
}
