#include "zcm.h"
#include <sl/slfiledialog.h>
#include <sl/slfileselector.h>
#include <qdir.h>
#include <qdatetime.h> 
#include <qregexp.h>
#include <qpe/qcopenvelope_qws.h>
#include <qpushbutton.h> 

ZColorMatching::ZColorMatching()
{
  // QDirectPainter dp(this) つまりトップレベルにあるQMainWindowの
  // フレームバッファには描けないぽいので，
  // canvasという子ウィジェットを作成してそこからフレームバッファを取得

  canvas = new Canvas(this);
  setCentralWidget(canvas);

}

ZColorMatching::~ZColorMatching() {
  delete canvas;
}


void ZColorMatching::myResize()
{
  resize();
}

void ZColorMatching::resize(){
  setFixedSize(QApplication::desktop()->size());
  showNormal();
  reparent( NULL, WType_TopLevel | WStyle_Customize | WStyle_NoBorderEx, QPoint(0, 0));

  showFullScreen();
}


void ZColorMatching::keyReleaseEvent( QKeyEvent *e )
{
}


void ZColorMatching::keyPressEvent( QKeyEvent *e )
{
  switch(e->key()){
  case 0x1004: // OK
    if(canvas->curveColor == 0xffffffff)
      canvas->curveColor = 0xff000000;
    else
      canvas->curveColor = 0xffffffff;

    canvas->repaint(QRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT,QRegion::Rectangle), false);

    break;

  case Qt::Key_Up:
    if(canvas->currentLevel < 15){
      canvas->prevLevel = canvas->currentLevel;
      canvas->currentLevel++;

      int tmpleft = (255 - canvas->bgColor[canvas->colorIndex][canvas->prevLevel])*2 + XOFFSET;
      int tmptop = (15 - canvas->prevLevel)*40 + 20;
      QRegion r = QRegion(tmpleft-4,tmptop-4,9,9,QRegion::Rectangle);
      canvas->repaint(r, false);
      tmpleft = (255 - canvas->bgColor[canvas->colorIndex][canvas->currentLevel])*2 + XOFFSET;
      tmptop = (15 - canvas->currentLevel)*40 + 20;
      r = QRegion(tmpleft-4,tmptop-4,9,9,QRegion::Rectangle);
      canvas->repaint(r, false);

    }
    break;

  case Qt::Key_Down:
    if(canvas->currentLevel > 1){
      canvas->prevLevel = canvas->currentLevel;
      canvas->currentLevel--;

      int tmpleft = (255 - canvas->bgColor[canvas->colorIndex][canvas->prevLevel])*2 + XOFFSET;
      int tmptop = (15 - canvas->prevLevel)*40 + 20;
      QRegion r = QRegion(tmpleft-4,tmptop-4,9,9,QRegion::Rectangle);
      canvas->repaint(r, false);
      tmpleft = (255 - canvas->bgColor[canvas->colorIndex][canvas->currentLevel])*2 + XOFFSET;
      tmptop = (15 - canvas->currentLevel)*40 + 20;
      r = QRegion(tmpleft-4,tmptop-4,9,9,QRegion::Rectangle);
      canvas->repaint(r, false);

    }
    break;

  case Qt::Key_Escape:
    close();
    myResize();
    break;

  case Qt::Key_Right:
    break;

  case Qt::Key_Left:
    break;

  default:
    break;
  }

}


void ZColorMatching::closeEvent( QCloseEvent *e )
{
  if(canvas->fileChangeFlag){
    if (QMessageBox::warning(this, "ZColorAdjust",
			     "Changes are not saved.\nExit anyway?\n",
			     "OK", "Cancel")){
      e->ignore();
      //      canvas->repaint();

    }
    else{
      e->accept();
    }
  }
  else{
    e->accept();
  }
}


void ZColorMatching::resizeEvent(QResizeEvent *e)
{
  // 全画面モードへ
  // showFullScreen()はコンストラクタに書いても無効
  
  if(QApplication::desktop()->width() == 640 && canvas->windowDirection == TATE){
    for(int y=0; y<640; y++){
      for(int x=0; x<480; x++){
	canvas->bgImage->setPixel(x,y, 0xff000000);
      }
    }
    for(int y=0; y<32; y++){
      for(int x=0; x<32; x++){
	canvas->bgImage->setPixel(304+x,260+y, canvas->zcmImg->pixel(x,y));
      }
    }

    QPixmap canvasPix = QPixmap();
    canvasPix.convertFromImage(*(canvas->bgImage),0);
    QPainter p;
    p.begin(&canvasPix );
    p.setPen( white );
    p.setFont(QFont("lcfont", 20));
    p.drawText(QRect(200,180, 320, 24),Qt::AlignLeft , "ZColorAdjust:");
    p.drawText(QRect(200,210, 320, 24),Qt::AlignLeft , "Please change to ViewStyle.");
    p.end();  
    canvas->setBackgroundPixmap(canvasPix);

    canvas->windowDirection = YOKO;
    setFixedSize(QApplication::desktop()->size());

    return;
  }
  else if(QApplication::desktop()->width() == 480 && canvas->windowDirection == YOKO){
    canvas->windowDirection = TATE;
    canvas->toolbar->show();

    for(int y=0; y<640; y++){
      for(int x=0; x<480; x++){
	canvas->bgImage->setPixel(x,y, 0xffffffff);
      }
    }
    QPixmap canvasPix = QPixmap();
    canvasPix.convertFromImage(*(canvas->bgImage),0);
    canvas->setBackgroundPixmap(canvasPix);

  }

  if ( size() == QApplication::desktop()->size() ) {
    return;
  }

  setFixedSize(QApplication::desktop()->size());

  reparent( NULL, WStyle_StaysOnTop | WStyle_Customize | WStyle_NoBorderEx, QPoint(0, 0));
  showNormal();
  showFullScreen();


}



void ZColorMatching::openImage()
{

  SlFileDialog sfd(FALSE, NULL, "Open file(png, jpg, bmp) ...", TRUE, WType_Modal);

  sfd.move(0,0);

  sfd.setDefaultFile("/home/zaurus/Documents/Image_Files/");

  if(!sfd.exec()){
    return;
  }
  
  QString filepath = sfd.getFilePath();
  QString filename = sfd.getFileName();


  if(filename.length() <= 4){
    QMessageBox::warning(this, "CloverPaint", "The file name is invalid.\n","OK");
    return;
  }

  QString ext;
  ext = filepath.right(4);

  if(ext == ".BMP" || ext == ".bmp"   ){

    QImage *tmpImage = new QImage();
    if(!tmpImage->load(filepath,"BMP")){
	if (!QMessageBox::warning(this, "CloverPaint", "Load error.\n","OK")){
	  delete tmpImage;
	  return;
	}
    }
    int tmpWidth = 0;
    int tmpHeight = 0;
    if(tmpImage->width()< 480)
      tmpWidth = tmpImage->width();
    else
      tmpWidth = 480;
    if(tmpImage->height()<640)
      tmpHeight = tmpImage->height();
    else
      tmpHeight = 640;
    
    if(tmpWidth != 480 || tmpHeight !=640){
      for(int y=0; y<640; y++){
	for(int x=0; x<480; x++){
	  canvas->sampleImage->setPixel(x,y,0xffffffff);
	}
      }
    }
    for(int y=0; y<tmpHeight; y++){
      for(int x=0; x<tmpWidth; x++){
	canvas->sampleImage->setPixel(x,y, tmpImage->pixel(x,y));
      }
    }
    delete tmpImage;

  }
  else if(ext == ".JPG" || ext == ".jpg"  ){

    QImage *tmpImage = new QImage();

    if(!tmpImage->load(filepath,"JPEG")){
	if (!QMessageBox::warning(this, "CloverPaint", "Load error.\n","OK")){

	  delete tmpImage;

	  return;
	}
    }
    int tmpWidth = 0;
    int tmpHeight = 0;
    if(tmpImage->width()< 480)
      tmpWidth = tmpImage->width();
    else
      tmpWidth = 480;
    if(tmpImage->height()<640)
      tmpHeight = tmpImage->height();
    else
      tmpHeight = 640;
    
    if(tmpWidth != 480 || tmpHeight !=640){
      for(int y=0; y<640; y++){
	for(int x=0; x<480; x++){
	  canvas->sampleImage->setPixel(x,y,0xffffffff);
	}
      }
    }
    for(int y=0; y<tmpHeight; y++){
      for(int x=0; x<tmpWidth; x++){
	canvas->sampleImage->setPixel(x,y, tmpImage->pixel(x,y));
      }
    }
    delete tmpImage;

  }
  else if(ext == ".PNG"|| ext == ".png" ){
    QImage *tmpImage = new QImage();

    if(!tmpImage->load(filepath,"PNG")){
	if (!QMessageBox::warning(this, "CloverPaint", "Load error.\n","OK")){

	  delete tmpImage;

	  return;
	}
    }
    int tmpWidth = 0;
    int tmpHeight = 0;
    if(tmpImage->width()< 480)
      tmpWidth = tmpImage->width();
    else
      tmpWidth = 480;
    if(tmpImage->height()<640)
      tmpHeight = tmpImage->height();
    else
      tmpHeight = 640;
    
    if(tmpWidth != 480 || tmpHeight !=640){
      for(int y=0; y<640; y++){
	for(int x=0; x<480; x++){
	  canvas->sampleImage->setPixel(x,y,0xffffffff);
	}
      }
    }
    for(int y=0; y<tmpHeight; y++){
      for(int x=0; x<tmpWidth; x++){
	canvas->sampleImage->setPixel(x,y, tmpImage->pixel(x,y));
      }
    }
    delete tmpImage;

  }
  else{
    if (QMessageBox::warning(this, "CloverPaint",
			     "Unsupported image format.\n(CloverPaint supports .cpf .png .jpg .bmp)",
			     "OK")){
      return;
    }
  }


  canvas->sampleImageExists = true;
  canvas->sampleVisible = true;


}
