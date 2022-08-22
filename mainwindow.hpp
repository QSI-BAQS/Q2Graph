#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#pragma once

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

   void openGraphDialog();
   void saveGraphDialog();

private:
   Ui::MainWindow * ui {};
   QMenu * fileMenu;
   GraphFrame * graphframe {};

   void createMenus();
};
#endif // MAINWINDOW_HPP
