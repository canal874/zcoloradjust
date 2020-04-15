#include <stdio.h>
#include <stdlib.h>

#include <sl/slfiledialog.h>
#include <sl/slfileselector.h>

#include <qmainwindow.h>
#include <qwidget.h>
#include <qdirectpainter_qws.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qfileinfo.h>
#include <qdialog.h>
#include <qbutton.h>
#include <qtoolbar.h> 

#include <qpe/qcopenvelope_qws.h>

#include <qpe/global.h>

#include <qpe/applnk.h>
#include <qpe/qpeapplication.h>


#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

#define YOKO 0
#define TATE 1

#define XOFFSET 17

#define SAVEICON_X 1
#define SAVEICON_Y 1
#define SAVEICON_WIDTH 30
#define SAVEICON_HEIGHT 29

#define OPENICON_X 33
#define OPENICON_Y 1
#define OPENICON_WIDTH 30
#define OPENICON_HEIGHT 29


#define RESETICON_X 65
#define RESETICON_Y 1
#define RESETICON_WIDTH 30
#define RESETICON_HEIGHT 29

#define RESET2ICON_X 97
#define RESET2ICON_Y 1
#define RESET2ICON_WIDTH 30
#define RESET2ICON_HEIGHT 29

#define REDICON_X 129
#define REDICON_Y 1
#define REDICON_WIDTH 30
#define REDICON_HEIGHT 29

#define GREENICON_X 160
#define GREENICON_Y 1
#define GREENICON_WIDTH 30
#define GREENICON_HEIGHT 29

#define BLUEICON_X 191
#define BLUEICON_Y 1
#define BLUEICON_WIDTH 30
#define BLUEICON_HEIGHT 29

#define IMAGEICON_X 222
#define IMAGEICON_Y 1
#define IMAGEICON_WIDTH 30
#define IMAGEICON_HEIGHT 29

#define BALANCEICON_X 255
#define BALANCEICON_Y 1
#define BALANCEICON_WIDTH 30
#define BALANCEICON_HEIGHT 29


#define LEFT2ICON_X 297
#define LEFT2ICON_Y 1
#define LEFT2ICON_WIDTH 38
#define LEFT2ICON_HEIGHT 29

#define LEFT1ICON_X 338
#define LEFT1ICON_Y 1
#define LEFT1ICON_WIDTH 27
#define LEFT1ICON_HEIGHT 29

#define RIGHT1ICON_X 396
#define RIGHT1ICON_Y 1
#define RIGHT1ICON_WIDTH 27
#define RIGHT1ICON_HEIGHT 29

#define RIGHT2ICON_X 426
#define RIGHT2ICON_Y 1
#define RIGHT2ICON_WIDTH 38
#define RIGHT2ICON_HEIGHT 29

#define UPICON_X 371
#define UPICON_Y 1
#define UPICON_WIDTH 19
#define UPICON_HEIGHT 15

#define DOWNICON_X 371
#define DOWNICON_Y 16
#define DOWNICON_WIDTH 19
#define DOWNICON_HEIGHT 13


#define CLOSEICON_X 467
#define CLOSEICON_Y 9
#define CLOSEICON_WIDTH 11
#define CLOSEICON_HEIGHT 11


class ZColorMatching;
class Canvas;
class MyToolBar;

class ZColorMatching : public QMainWindow {
  Q_OBJECT

public:
  ZColorMatching();
  ~ZColorMatching();

  char canvasHeight;


  void myResize();
  virtual void keyPressEvent( QKeyEvent *e );
  virtual void keyReleaseEvent( QKeyEvent *e );
  virtual void closeEvent( QCloseEvent *e );
  virtual void resizeEvent(QResizeEvent *e);

  void openImage();

private:
  Canvas *canvas;
  void resize();

};


class Canvas : public QWidget {
  Q_OBJECT

public:
  Canvas(ZColorMatching *parent);
  ~Canvas();

  QImage *zcmImg;  
  QImage *bgImage;
  QImage *sampleImage;
  bool fileChangeFlag;
  uchar bgColor[4][16];
  uchar colorIndex;
  char windowDirection;
  MyToolBar *toolbar;
  uchar currentLevel;

  uchar colorMatrix[3][256];

  bool sampleImageExists;
  bool sampleVisible;

  unsigned int curveColor;

  uchar prevLevel;
  uchar prevColor;

  ZColorMatching *zcm;

  void rebuildColorMatrix();
  void save();
  void repaint_rect();

  virtual void paintEvent( QPaintEvent *e );
  virtual void mousePressEvent( QMouseEvent *e );
  virtual void mouseReleaseEvent( QMouseEvent *e );
  virtual void mouseMoveEvent( QMouseEvent *e );

protected:

private:
  int repaint_left;
  int repaint_top;
  int repaint_width;
  int repaint_height;

};

class MyToolBar : public QDialog {
  Q_OBJECT

public:
  MyToolBar(Canvas * parent=0, const char * name=0, bool modal=FALSE, WFlags f=0);
  ~MyToolBar();

  QImage toolbarImg;  
  QImage toolbar_normalImg;  
  QImage toolbar_reverseImg;

  bool saveBtnPressed;
  bool openBtnPressed;
  bool balanceBtnPressed;
  bool resetBtnPressed;
  bool resetBtn2Pressed;
  bool closeBtnPressed;

  bool left1BtnPressed;
  bool left2BtnPressed;
  bool right1BtnPressed;
  bool right2BtnPressed;
  bool upBtnPressed;
  bool downBtnPressed;


  void redrawToolBar();

  virtual void mousePressEvent( QMouseEvent *e );
  virtual void mouseReleaseEvent( QMouseEvent *e );
  virtual void mouseMoveEvent( QMouseEvent *e );
protected:

private:
  Canvas *canvas;

};
