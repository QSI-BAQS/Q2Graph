#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
   MainWindow(QWidget * parent= nullptr);
   ~MainWindow();

private:
   Ui::MainWindow * ui {};
   QMenu * fileMenu;
   GraphFrame * graphframe {};

   void createMenus();
};
#endif // MAINWINDOW_H
