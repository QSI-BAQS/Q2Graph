#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphvertex.h"

#include <QMainWindow>

class GraphFrame;

QT_BEGIN_NAMESPACE
class QGraphicsView;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget * parent = nullptr);
   ~MainWindow();

private:
   Ui::MainWindow * ui {};
   GraphFrame * graphframe {};

};
#endif // MAINWINDOW_H
