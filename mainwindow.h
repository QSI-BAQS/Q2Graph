#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphvertex.h"

#include <QLabel>
#include <QMainWindow>

class GraphFrame;

QT_BEGIN_NAMESPACE
class QAction;
namespace Ui { class MainWindow; }
class QGraphicsView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget * parent = nullptr);
   ~MainWindow();

   //QLabel * clabel {};

private:
   Ui::MainWindow * ui {};
   GraphFrame * graphframe {};

};
#endif // MAINWINDOW_H
