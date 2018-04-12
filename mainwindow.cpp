#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

Point prevPt;
Point prevWinPt;
Point prevWinMousePt;
Point winCurPos;
Mat src;
Mat segMask;
Mat showImg;
int pen = 250;
int penSize = 10;
float scale = 1;

void ShowImg(){
  Mat drawMask;
  threshold(segMask,drawMask,1,255,THRESH_BINARY);
  Mat showTmp = src.clone();
  segMask.copyTo(showTmp,drawMask);
  resize(showTmp,showImg,Size(src.cols*scale,src.rows*scale));
  imshow("Image", showImg);
  //imshow("Mask",segMask);
}

static void onMouse( int event, int x, int y, int flags, void* )
{
  //    if( x < 0 || x >= img.cols || y < 0 || y >= img.rows )
  //        return;

  int rx = x/scale;
  int ry = y/scale;

  if(event == EVENT_MOUSEWHEEL){
      if (getMouseWheelDelta(flags)>0)
        scale *= (float)1.1;
      else
        scale *= (float)0.9;

      ShowImg();
    }

  if(event == EVENT_MBUTTONDOWN){
      auto gPos = QCursor::pos();
      prevWinMousePt.x = gPos.x();
      prevWinMousePt.y = gPos.y();
    }

  if(event == EVENT_MBUTTONUP){
      prevWinPt.x = winCurPos.x;
      prevWinPt.y = winCurPos.y;
    }

  if(event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_MBUTTON)){
      auto gPos = QCursor::pos();
      Point vec;
      vec.x = gPos.x() - prevWinMousePt.x;
      vec.y = gPos.y() - prevWinMousePt.y;

      winCurPos.x = prevWinPt.x + vec.x;
      winCurPos.y = prevWinPt.y + vec.y;

      moveWindow("Image",winCurPos.x,winCurPos.y);

    }

  if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
    prevPt = Point(-1,-1);
  else if( event == EVENT_LBUTTONDOWN ){
      prevPt = Point(rx,ry);
      line( segMask, prevPt, prevPt, Scalar::all(pen), penSize, 8, 0 );
    }

  if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
      Point pt(rx, ry);

      if( prevPt.x < 0 )
        prevPt = pt;

      line( segMask, prevPt, pt, Scalar::all(pen), penSize, 8, 0 );
      prevPt = pt;
      waitKey(1);
      ShowImg();
    }
}

void MainWindow::on_actionLoad_triggered()
{
  QString fileName = QFileDialog::getOpenFileName();
  src = imread(fileName.toStdString());
  if(src.empty()){
      qDebug()<<"Couldn`t open image";
      return;
    }
  segMask = Mat(src.rows,src.cols,src.type(),Scalar::all(0));
  imshow("Image", src);
  moveWindow("Image",0,0);
  //imshow("Mask",segMask);
  setMouseCallback("Image",onMouse,0);
}

void MainWindow::on_btnBackground_clicked()
{
  pen = 120;
}

void MainWindow::on_btnForeground_clicked()
{
  pen = 250;
}

void MainWindow::on_btnErase_clicked()
{
  pen = 0;
}

void MainWindow::on_btnClear_clicked()
{
  segMask = 0;
}

void MainWindow::on_actionSave_triggered()
{
  QString fileName = QFileDialog::getSaveFileName();
  imwrite(fileName.toStdString(),segMask);
}

void MainWindow::on_btnProcess_clicked()
{
  QString fileName = QFileDialog::getOpenFileName();
  Mat gimg = imread(fileName.toStdString(),0);
  Mat cimg = imread(fileName.toStdString());
  Mat mask = imread("mask.bmp",0);
  seg.LoadMask(mask);
  seg.Process(gimg);
  Mat res = seg.MaskedImg(cimg,Scalar::all(255));
  Mat showTmp;
  cv::resize(res,showTmp,Size(res.cols*scale,res.rows*scale));
  imshow("Result", showTmp);
  cv::imwrite("res.bmp",res);
}

void MainWindow::on_actionLoad_Mask_triggered()
{
  segMask = NQtCva::GetImage();
}

void MainWindow::on_sliderPenSize_valueChanged(int value)
{
  penSize = value;
}
