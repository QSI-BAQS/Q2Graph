#include "graphframe.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QtWidgets>


// public:
MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   createMenus();

   graphframe= new GraphFrame(this);
   ui->graphicsView->setScene(graphframe);

   ui->graphicsView->setMouseTracking(true);
   setWindowTitle("Q2Graph");
}

MainWindow::~MainWindow()
{
   delete ui;
}


// public:
void MainWindow::openGraphDialog() {
   QString graphopenfile= QFileDialog::getOpenFileName(this, tr("Open File"));
   graphframe->openGraph(graphopenfile);
}

void MainWindow::saveGraphDialog() {
   QString graphsavefile= QFileDialog::getSaveFileName(this, tr("Save File"));
   graphframe->saveGraph(graphsavefile);
}


// private:
void MainWindow::createMenus() {
   fileMenu= menuBar()->addMenu(tr("&File"));
   fileMenu->addAction(tr("&Open"), this, [this](){
      openGraphDialog();
   });
   fileMenu->addAction(tr("&Save"), this, [this](){
      saveGraphDialog();
   });
   fileMenu->addAction(tr("E&xit"), this, [this](){
      QWidget::close();
   });
}
