#include "zcm.h"
#include "toolbarimg.h"
#include "toolbarimg_reverse.h"



MyToolBar::MyToolBar(Canvas * parent, const char * name, bool modal, WFlags f)
  :QDialog(parent, name, modal, f)
{
  QPainter p;
  QString tmpStr = "   ";

  canvas = parent;

  reparent( parent, QPoint(0, SCREEN_HEIGHT-30));
    resize(480, 30);

  //ツールバー画像の読み込み
  QImage tmpImg = QImage(toolbarimg);
  //xpmからの読みこみはインデックスカラーになるため変換
  toolbarImg = tmpImg.convertDepth(32);
  toolbar_normalImg = toolbarImg.copy();

  //ツールバー画像(裏)の読み込み
  tmpImg = QImage(toolbarimg_reverse);
  toolbar_reverseImg = tmpImg.convertDepth(32);

  //ツールバー画像の表示
  QPixmap toolbarPix = QPixmap();
  toolbarPix.convertFromImage(toolbarImg, 0);
  setBackgroundPixmap(toolbarPix);

  saveBtnPressed = false;
  openBtnPressed = false;
  balanceBtnPressed = false;
  resetBtnPressed = false;
  resetBtn2Pressed = false;
  closeBtnPressed = false;
  left1BtnPressed = false;
  left2BtnPressed = false;
  right1BtnPressed = false;
  right2BtnPressed = false;
  upBtnPressed = false;
  downBtnPressed = false;

}

MyToolBar::~MyToolBar()
{
}

void MyToolBar::redrawToolBar(){

  for(int y=IMAGEICON_Y; y<IMAGEICON_Y+IMAGEICON_HEIGHT; y++){
    for(int x=IMAGEICON_X; x<IMAGEICON_X+IMAGEICON_WIDTH; x++){
      if(canvas->sampleVisible){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
      else{
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }

  if(canvas->colorIndex == 1){
    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  else if(canvas->colorIndex == 2){
    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  else if(canvas->colorIndex == 3){
    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
  }

  QPixmap toolbarPix = QPixmap();
  toolbarPix.convertFromImage(toolbarImg, 0);
  setBackgroundPixmap(toolbarPix);


}


void MyToolBar::mousePressEvent( QMouseEvent *e )
{

  if(e->x()>=SAVEICON_X && e->x()< SAVEICON_X + SAVEICON_WIDTH){
    saveBtnPressed = true;
    for(int y=SAVEICON_Y; y<SAVEICON_Y+SAVEICON_HEIGHT; y++){
      for(int x=SAVEICON_X; x<SAVEICON_X+SAVEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    canvas->save();
  }
  else if(e->x()>=OPENICON_X && e->x()< OPENICON_X + OPENICON_WIDTH){
    openBtnPressed = true;
    for(int y=OPENICON_Y; y<OPENICON_Y+OPENICON_HEIGHT; y++){
      for(int x=OPENICON_X; x<OPENICON_X+OPENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
  }
  else if(e->x()>=BALANCEICON_X && e->x()< BALANCEICON_X + BALANCEICON_WIDTH){
    canvas->fileChangeFlag = true;
    balanceBtnPressed = true;
    for(int y=BALANCEICON_Y; y<BALANCEICON_Y+BALANCEICON_HEIGHT; y++){
      for(int x=BALANCEICON_X; x<BALANCEICON_X+BALANCEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }

  }
  else if(e->x()>=RESETICON_X && e->x()< RESETICON_X + RESETICON_WIDTH){
    resetBtnPressed = true;

    for(int y=RESETICON_Y; y<RESETICON_Y+RESETICON_HEIGHT; y++){
      for(int x=RESETICON_X; x<RESETICON_X+RESETICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }


  }
  else if(e->x()>=RESET2ICON_X && e->x()< RESET2ICON_X + RESET2ICON_WIDTH){
    resetBtn2Pressed = true;

    for(int y=RESET2ICON_Y; y<RESET2ICON_Y+RESET2ICON_HEIGHT; y++){
      for(int x=RESET2ICON_X; x<RESET2ICON_X+RESET2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
  }
  else if(e->x()>=CLOSEICON_X && e->x()< CLOSEICON_X + CLOSEICON_WIDTH){
    closeBtnPressed = true;

    for(int y=CLOSEICON_Y; y<CLOSEICON_Y+CLOSEICON_HEIGHT; y++){
      for(int x=CLOSEICON_X; x<CLOSEICON_X+CLOSEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
  }

  else if(e->x()>=LEFT1ICON_X && e->x()< LEFT1ICON_X + LEFT1ICON_WIDTH){
    left1BtnPressed = true;
    canvas->fileChangeFlag = true;

    if( canvas->bgColor[canvas->colorIndex][canvas->currentLevel] <= 254 ){
      canvas->prevColor = canvas->bgColor[canvas->colorIndex][canvas->currentLevel];
      canvas->bgColor[canvas->colorIndex][canvas->currentLevel] += 1;
    }
    for(int y=LEFT1ICON_Y; y<LEFT1ICON_Y+LEFT1ICON_HEIGHT; y++){
      for(int x=LEFT1ICON_X; x<LEFT1ICON_X+LEFT1ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }

    canvas->rebuildColorMatrix();
    canvas->repaint_rect();
  }
  else if(e->x()>=LEFT2ICON_X && e->x()< LEFT2ICON_X + LEFT2ICON_WIDTH){
    canvas->fileChangeFlag = true;
    left2BtnPressed = true;
    if( canvas->bgColor[canvas->colorIndex][canvas->currentLevel] <= 251 ){
      canvas->prevColor = canvas->bgColor[canvas->colorIndex][canvas->currentLevel];
      canvas->bgColor[canvas->colorIndex][canvas->currentLevel] += 4;
    }
    for(int y=LEFT2ICON_Y; y<LEFT2ICON_Y+LEFT2ICON_HEIGHT; y++){
      for(int x=LEFT2ICON_X; x<LEFT2ICON_X+LEFT2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    canvas->rebuildColorMatrix();
    canvas->repaint_rect();
  }
  else if(e->x()>=RIGHT1ICON_X && e->x()< RIGHT1ICON_X + RIGHT1ICON_WIDTH){
    right1BtnPressed = true;
    canvas->fileChangeFlag = true;
    if( canvas->bgColor[canvas->colorIndex][canvas->currentLevel] > 0 ){
      canvas->prevColor = canvas->bgColor[canvas->colorIndex][canvas->currentLevel];
      canvas->bgColor[canvas->colorIndex][canvas->currentLevel] -= 1;
    }
    for(int y=RIGHT1ICON_Y; y<RIGHT1ICON_Y+RIGHT1ICON_HEIGHT; y++){
      for(int x=RIGHT1ICON_X; x<RIGHT1ICON_X+RIGHT1ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    canvas->rebuildColorMatrix();
    canvas->repaint_rect();
  }
  else if(e->x()>=RIGHT2ICON_X && e->x()< RIGHT2ICON_X + RIGHT2ICON_WIDTH){
    right2BtnPressed = true;
    canvas->fileChangeFlag = true;
    if( canvas->bgColor[canvas->colorIndex][canvas->currentLevel] >= 4 ){
      canvas->prevColor = canvas->bgColor[canvas->colorIndex][canvas->currentLevel];
      canvas->bgColor[canvas->colorIndex][canvas->currentLevel] -= 4;
    }
    for(int y=RIGHT2ICON_Y; y<RIGHT2ICON_Y+RIGHT2ICON_HEIGHT; y++){
      for(int x=RIGHT2ICON_X; x<RIGHT2ICON_X+RIGHT2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    canvas->rebuildColorMatrix();
    canvas->repaint_rect();
  }
  else if(e->x()>=UPICON_X && e->x()< UPICON_X + UPICON_WIDTH && e->y() < UPICON_Y+UPICON_HEIGHT){
    upBtnPressed = true;

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

    for(int y=UPICON_Y; y<UPICON_Y+UPICON_HEIGHT; y++){
      for(int x=UPICON_X; x<UPICON_X+UPICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }

  }
  else if(e->x()>=DOWNICON_X && e->x()< DOWNICON_X + DOWNICON_WIDTH && e->y() > DOWNICON_Y){
    downBtnPressed = true;

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

    for(int y=DOWNICON_Y; y<DOWNICON_Y+DOWNICON_HEIGHT; y++){
      for(int x=DOWNICON_X; x<DOWNICON_X+DOWNICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }

  }


  if(e->x()>=IMAGEICON_X && e->x()< IMAGEICON_X + IMAGEICON_WIDTH){
    if(canvas->sampleVisible){
      canvas->sampleVisible = ! canvas->sampleVisible;      
    }
    else{
      if(canvas->sampleImageExists){
	canvas->sampleVisible = ! canvas->sampleVisible;      
      }
      else{
	return;
      }
    }

    for(int y=IMAGEICON_Y; y<IMAGEICON_Y+IMAGEICON_HEIGHT; y++){
      for(int x=IMAGEICON_X; x<IMAGEICON_X+IMAGEICON_WIDTH; x++){
	if(canvas->sampleVisible){
	  toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
	}
	else{
	  toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
	}
      }
    }
    canvas->repaint();
  }

  if(e->x()>=REDICON_X && e->x()< REDICON_X + REDICON_WIDTH){
    canvas->colorIndex = 1;

    canvas->prevColor = 255;
    canvas->prevLevel = 15;
    canvas->currentLevel = 15;

    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    canvas->repaint();
  }
  else if(e->x()>=GREENICON_X && e->x()< GREENICON_X + GREENICON_WIDTH){
    canvas->colorIndex = 2;

    canvas->prevColor = 255;
    canvas->prevLevel = 15;
    canvas->currentLevel = 15;


    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    canvas->repaint();
  }
  else if(e->x()>=BLUEICON_X && e->x()< BLUEICON_X + BLUEICON_WIDTH){
    canvas->colorIndex = 3;

    canvas->prevColor = 255;
    canvas->prevLevel = 15;
    canvas->currentLevel = 15;


    for(int y=REDICON_Y; y<REDICON_Y+REDICON_HEIGHT; y++){
      for(int x=REDICON_X; x<REDICON_X+REDICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=GREENICON_Y; y<GREENICON_Y+GREENICON_HEIGHT; y++){
      for(int x=GREENICON_X; x<GREENICON_X+GREENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
    for(int y=BLUEICON_Y; y<BLUEICON_Y+BLUEICON_HEIGHT; y++){
      for(int x=BLUEICON_X; x<BLUEICON_X+BLUEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
    }
    canvas->repaint();
  }


  QPixmap toolbarPix = QPixmap();
  toolbarPix.convertFromImage(toolbarImg, 0);
  setBackgroundPixmap(toolbarPix);


}


void MyToolBar::mouseMoveEvent( QMouseEvent *e )
{
}


void MyToolBar::mouseReleaseEvent( QMouseEvent *e )
{
  if(saveBtnPressed){
    QMessageBox::about(this, "ZColorAdjust", "Succeeded.");
    canvas->zcm->myResize();

    for(int y=SAVEICON_Y; y<SAVEICON_Y+SAVEICON_HEIGHT; y++){
      for(int x=SAVEICON_X; x<SAVEICON_X+SAVEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }

  }
  else if(openBtnPressed){
    canvas->zcm->openImage();
    canvas->zcm->myResize();
    canvas->repaint();

    for(int y=OPENICON_Y; y<OPENICON_Y+OPENICON_HEIGHT; y++){
      for(int x=OPENICON_X; x<OPENICON_X+OPENICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }

  }
  else if(balanceBtnPressed){
    for(int y=BALANCEICON_Y; y<BALANCEICON_Y+BALANCEICON_HEIGHT; y++){
      for(int x=BALANCEICON_X; x<BALANCEICON_X+BALANCEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }

    for(int i=0; i<3; i++){
      for(int j=0; j<16; j++){
	canvas->bgColor[i+1][j] = canvas->bgColor[canvas->colorIndex][j];
      }
    }

    canvas->rebuildColorMatrix();

    canvas->repaint();    


  }
  else if(resetBtnPressed){
    if (QMessageBox::warning(this, "ZColorAdjust",
			     "Reset current colors?\n",
			     "OK", "Cancel")){
    }
    else{
      canvas->fileChangeFlag = true;
      for(int y=0; y<SCREEN_HEIGHT; y++){
	for(int x=0; x<SCREEN_WIDTH; x++){
	  int level = (int)(SCREEN_HEIGHT-1-y)/40 ;
	  uchar col = level * 16 + 15;
	  canvas->bgColor[canvas->colorIndex][level] = col;
	}
      }
    }
    canvas->zcm->myResize();
    canvas->repaint();

    for(int y=RESETICON_Y; y<RESETICON_Y+RESETICON_HEIGHT; y++){
      for(int x=RESETICON_X; x<RESETICON_X+RESETICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }

    canvas->rebuildColorMatrix();
  }
  else if(resetBtn2Pressed){
    if (QMessageBox::warning(this, "ZColorAdjust",
			     "Reset current colors?\n",
			     "OK", "Cancel")){
    }
    else{
      canvas->fileChangeFlag = true;
	canvas->bgColor[canvas->colorIndex][0] = 15;
	canvas->bgColor[canvas->colorIndex][1] = 21;
	canvas->bgColor[canvas->colorIndex][2] = 28;
	canvas->bgColor[canvas->colorIndex][3] = 36;
	canvas->bgColor[canvas->colorIndex][4] = 44;
	canvas->bgColor[canvas->colorIndex][5] = 52;
	canvas->bgColor[canvas->colorIndex][6] = 60;
	canvas->bgColor[canvas->colorIndex][7] = 68;
	canvas->bgColor[canvas->colorIndex][8] = 76;
	canvas->bgColor[canvas->colorIndex][9] = 86;
	canvas->bgColor[canvas->colorIndex][10] = 95;
	canvas->bgColor[canvas->colorIndex][11] = 109;
	canvas->bgColor[canvas->colorIndex][12] = 124;
	canvas->bgColor[canvas->colorIndex][13] = 145;
	canvas->bgColor[canvas->colorIndex][14] = 186;
	canvas->bgColor[canvas->colorIndex][15] = 254;
    }
    canvas->zcm->myResize();
    canvas->repaint();

    for(int y=RESET2ICON_Y; y<RESET2ICON_Y+RESET2ICON_HEIGHT; y++){
      for(int x=RESET2ICON_X; x<RESET2ICON_X+RESET2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }

    canvas->rebuildColorMatrix();
  }

  if(closeBtnPressed){
    canvas->zcm->close();

    canvas->zcm->myResize();
    canvas->repaint();

    for(int y=CLOSEICON_Y; y<CLOSEICON_Y+CLOSEICON_HEIGHT; y++){
      for(int x=CLOSEICON_X; x<CLOSEICON_X+CLOSEICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }

  if(left1BtnPressed){
    for(int y=LEFT1ICON_Y; y<LEFT1ICON_Y+LEFT1ICON_HEIGHT; y++){
      for(int x=LEFT1ICON_X; x<LEFT1ICON_X+LEFT1ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  if(left2BtnPressed){
    for(int y=LEFT2ICON_Y; y<LEFT2ICON_Y+LEFT2ICON_HEIGHT; y++){
      for(int x=LEFT2ICON_X; x<LEFT2ICON_X+LEFT2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  if(right1BtnPressed){
    for(int y=RIGHT1ICON_Y; y<RIGHT1ICON_Y+RIGHT1ICON_HEIGHT; y++){
      for(int x=RIGHT1ICON_X; x<RIGHT1ICON_X+RIGHT1ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  if(right2BtnPressed){
    for(int y=RIGHT2ICON_Y; y<RIGHT2ICON_Y+RIGHT2ICON_HEIGHT; y++){
      for(int x=RIGHT2ICON_X; x<RIGHT2ICON_X+RIGHT2ICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  if(upBtnPressed){
    for(int y=UPICON_Y; y<UPICON_Y+UPICON_HEIGHT; y++){
      for(int x=UPICON_X; x<UPICON_X+UPICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }
  if(downBtnPressed){
    for(int y=DOWNICON_Y; y<DOWNICON_Y+DOWNICON_HEIGHT; y++){
      for(int x=DOWNICON_X; x<DOWNICON_X+DOWNICON_WIDTH; x++){
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }


  for(int y=IMAGEICON_Y; y<IMAGEICON_Y+IMAGEICON_HEIGHT; y++){
    for(int x=IMAGEICON_X; x<IMAGEICON_X+IMAGEICON_WIDTH; x++){
      if(canvas->sampleVisible){
	toolbarImg.setPixel(x,y,toolbar_reverseImg.pixel(x,y));
      }
      else{
	toolbarImg.setPixel(x,y,toolbar_normalImg.pixel(x,y));
      }
    }
  }


  openBtnPressed = false;
  saveBtnPressed = false;
  balanceBtnPressed = false;
  resetBtnPressed = false;
  resetBtn2Pressed = false;

  closeBtnPressed = false;
  left1BtnPressed = false;
  left2BtnPressed = false;
  right1BtnPressed = false;
  right2BtnPressed = false;
  upBtnPressed = false;
  downBtnPressed = false;

  QPixmap toolbarPix = QPixmap();
  toolbarPix.convertFromImage(toolbarImg, 0);
  setBackgroundPixmap(toolbarPix);

}

