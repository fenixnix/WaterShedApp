#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "vision/ncva.h"

using namespace cv;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionLoad_triggered();

  void on_btnBackground_clicked();

  void on_btnForeground_clicked();

  void on_btnErase_clicked();

  void on_btnClear_clicked();

  void on_actionSave_triggered();

  void on_btnProcess_clicked();

  void on_actionLoad_Mask_triggered();

  void on_sliderPenSize_valueChanged(int value);

private:
  Ui::MainWindow *ui;
  Segmentation seg;
};

#endif // MAINWINDOW_H
