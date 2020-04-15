#include "zcm.h"
#include <math.h>

#include "zcmimage.h"

Canvas::Canvas(ZColorMatching *parent)
  :QWidget(parent)
{
  zcm = parent;

  //オフスクリーン作成(フレームバッファにあわせて必ず縦長で)
  bgImage = new QImage(SCREEN_WIDTH,
		       SCREEN_HEIGHT,
		       32,
		       0,
		       QImage::IgnoreEndian);


  sampleImage = new QImage(SCREEN_WIDTH,
		       SCREEN_HEIGHT,
		       32,
		       0,
		       QImage::IgnoreEndian);


  sampleImageExists = false;
  sampleVisible = false;

  fileChangeFlag = false;

  prevColor = 255;
  prevLevel = 15;
  currentLevel = 15;

  repaint_left = 0;
  repaint_top = 0;
  repaint_width = 0;
  repaint_height = 0;


  colorIndex = 1;

  uchar col = 0;
  char level = 0;

  for(int y=0; y<SCREEN_HEIGHT; y++){
    for(int x=0; x<SCREEN_WIDTH; x++){
      level = (int)(SCREEN_HEIGHT-1-y)/40 ;
      col = level * 16 + 15;
      bgColor[0][level] = col;
      bgColor[1][level] = col;
      bgColor[2][level] = col;
      bgColor[3][level] = col;
    }
  }

  QImage *tmpImg = new QImage(zcmimage);
  zcmImg = new QImage(tmpImg->convertDepth(32));
  delete tmpImg;



  //設定の読み込み
  if(QFile::exists("/home/zaurus/Settings/ZColorMatching.conf")){
    uchar tmpColor[3][256];

    QFile f( "/home/zaurus/Settings/ZColorMatching.conf" );

    f.open( IO_ReadOnly );
    QDataStream s( &f );
    for(int i=0; i<3; i++){
      for(int j=0; j<256; j++){
	s >> (Q_INT8 &)(tmpColor[i][j]);
      }
    }
    f.close();

    for(int i=0; i<3; i++){
      for(int j=0; j<16; j++){
	bgColor[i+1][j] = tmpColor[i][j*16+15];
      }
    }
    f.remove();
    save();
  }

  if(QFile::exists("/home/zaurus/Settings/ZColorAdjust.conf")){
    uchar tmpColor[3][256];

    QFile f( "/home/zaurus/Settings/ZColorAdjust.conf" );

    f.open( IO_ReadOnly );
    QDataStream s( &f );
    for(int i=0; i<3; i++){
      for(int j=0; j<256; j++){
	s >> (Q_INT8 &)(tmpColor[i][j]);
      }
    }
    f.close();

    for(int i=0; i<3; i++){
      for(int j=0; j<16; j++){
	bgColor[i+1][j] = tmpColor[i][j*16+15];

	//fprintf(stderr, "%d,",tmpColor[i][j*16+15]);

      }
    }

  }


  if(QApplication::desktop()->width() == 640){
    windowDirection = YOKO;
    for(int y=0; y<640; y++){
      for(int x=0; x<480; x++){
	bgImage->setPixel(x,y, 0xff000000);
      }
    }

    for(int y=0; y<32; y++){
      for(int x=0; x<32; x++){
	bgImage->setPixel(304+x,260+y, zcmImg->pixel(x,y));
      }
    }

    QPixmap canvasPix = QPixmap();
    canvasPix.convertFromImage(*(bgImage),0);
    QPainter p;
    p.begin(&canvasPix );
    p.setPen( white );
    p.setFont(QFont("lcfont", 20));
    p.drawText(QRect(200,180, 320, 24),Qt::AlignLeft , "ZColorAdjust:");
    p.drawText(QRect(200,210, 320, 24),Qt::AlignLeft , "Please change to ViewStyle.");
    p.end();  
    setBackgroundPixmap(canvasPix);

  }
  else{
    windowDirection = TATE;
  }

  toolbar = new MyToolBar(this, "toolbar", false);
  if(windowDirection == TATE){
    toolbar->show();
    toolbar->redrawToolBar();
  }
  else
    toolbar->hide();



  rebuildColorMatrix();


  curveColor = 0xffffffff;
   
  repaint();


}

Canvas::~Canvas()
{
  delete bgImage;
  delete sampleImage;
  delete toolbar;
  delete zcmImg;
}



void Canvas::paintEvent( QPaintEvent * )
{

  QDirectPainter dp(this);
  unsigned short *frameBuffer = (unsigned short *)dp.frameBuffer();
  unsigned int tmpColor;


  if(windowDirection ==YOKO){
    return;
  }


  //フレームバッファは常に縦640x横480で取得される
  //dp.numRects()は常に1と考えていい

  int pointX_default = 0;
  int pointX = 0;

  for(int y=0; y<SCREEN_HEIGHT; y++){
    pointX_default = -1;
    pointX = -1;

    char level = (int)(SCREEN_HEIGHT-1-y)/40;

    // default
    uchar col_default = bgColor[0][level];
    if((SCREEN_HEIGHT-1-y)%40 >= 20){
      pointX_default = (255 - (bgColor[0][level+1] - (bgColor[0][level+1] - bgColor[0][level])*(40-(SCREEN_HEIGHT-20-1-y)%40)/40))*2 + XOFFSET;
    }
    else if((SCREEN_HEIGHT-1-y)%40 >= 0){
      pointX_default = (255 - (col_default - (bgColor[0][level] - bgColor[0][level-1])*(40-(SCREEN_HEIGHT-20-1-y)%40)/40))*2 + XOFFSET;
    }


    int amari_default = (y+20)%40;

    // current 
    uchar col = bgColor[colorIndex][level];
    if((SCREEN_HEIGHT-1-y)%40 >= 20){
      pointX = (255 - (bgColor[colorIndex][level+1] - (bgColor[colorIndex][level+1] - bgColor[colorIndex][level])*(40-(SCREEN_HEIGHT-20-1-y)%40)/40))*2 + XOFFSET;
    }
    else if((SCREEN_HEIGHT-1-y)%40 >= 0){
      pointX = (255 - (col - (bgColor[colorIndex][level] - bgColor[colorIndex][level-1])*(40-(SCREEN_HEIGHT-20-1-y)%40)/40))*2 + XOFFSET;
    }


    int amari = (y+20)%40;


    for(int x=0; x<SCREEN_WIDTH; x++){

	if(level == currentLevel){
	  if((amari == 37 || amari == 38 || amari == 39 || amari%40 == 0 || amari%40 == 1 || amari == 2 || amari == 3 ) && SCREEN_WIDTH > (255 - col)*2 &&  x == (255 - col)*2 + XOFFSET){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,curveColor);
	    }
	    else{
	      bgImage->setPixel(x,y,0xffffffff);
	    }
	  }
	  else if((amari%40 == 0) && SCREEN_WIDTH > (255 - col)*2 &&  (x == (255 - col)*2 + XOFFSET-3 || x == (255 - col)*2 + XOFFSET-2 || x == (255 - col)*2 + XOFFSET-1 || x == (255 - col)*2 + XOFFSET+1 || x == (255 - col)*2 + XOFFSET+2 || x == (255 - col)*2 + XOFFSET+3)){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,curveColor);
	    }
	    else{
	      bgImage->setPixel(x,y,0xffffffff);
	    }
	  }
	  else if( (y>20 && y<580) && x == pointX){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,curveColor);
	    }
	    else{
	      bgImage->setPixel(x,y,0xffc0c0c0);
	    }
	  }
	  else if((amari_default == 39 || amari_default%40 == 0 || amari_default%40 == 1) && SCREEN_WIDTH > (255 - col_default)*2 &&  x == (255 - col_default)*2 + XOFFSET){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,0xff808080);
	    }
	    else{
	      bgImage->setPixel(x,y,0xff808080);
	    }
	  }
	  else if( (y>20 && y<580) && x == pointX_default){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,0xff808080);
	    }
	    else{
	      bgImage->setPixel(x,y,0xff606060);
	    }
	  }
	  else{
	    if(sampleVisible){
	      bgImage->setPixel(x,y,sampleImage->pixel(x,y));
	    }
	    else{
	      if(colorIndex == 1){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 16)&0xffff0000);
	      }
	      else if(colorIndex == 2){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 8)&0xff00ff00);
	      }
	      else if(colorIndex == 3){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 0)&0xff0000ff);
	      }
	    }
	  }
	}
	else{
	  if((amari == 39 || amari%40 == 0 || amari%40 == 1) && SCREEN_WIDTH > (255 - col)*2 && (x == (255 - col)*2 + XOFFSET)){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,curveColor);
	    }	    
	    else{
	      bgImage->setPixel(x,y,0xffffffff);
	    }
	  }
	  else if( (y>20 && y<580) && x == pointX){
	    if(sampleVisible){
	      bgImage->setPixel(x,y,curveColor);
	    }
	    else{
	      bgImage->setPixel(x,y,0xffc0c0c0);
	    }
	  }
	  else if((amari_default == 39 || amari_default%40 == 0 || amari_default%40 == 1) && SCREEN_WIDTH > (255 - col_default)*2 &&  x == (255 - col_default)*2 + XOFFSET){
	    bgImage->setPixel(x,y,0xff808080);
	  }
	  else if( (y>20 && y<580) && x == pointX_default){
	    bgImage->setPixel(x,y,0xff606060);
	  }
	  else{
	    if(sampleVisible){
	      bgImage->setPixel(x,y,sampleImage->pixel(x,y));
	    }
	    else{
	      if(colorIndex == 1){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 16)&0xffff0000);
	      }
	      else if(colorIndex == 2){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 8)&0xff00ff00);
	      }
	      else if(colorIndex == 3){
		bgImage->setPixel(x,y,(bgColor[colorIndex][level] << 0)&0xff0000ff);
	      }
	    }
	  }
	}
      }

    }




    for(int i=0; i<dp.numRects(); i++){
      QRect r = dp.rect(i);
      for(int y=r.top(); y<=r.bottom(); y++){
	for(int x=r.left(); x<=r.right(); x++){
	  //lineStep()はcharで数えるため2で割る
	  //pixel()は24bitカラーを返すので16bitカラーへ変換

	  tmpColor = bgImage->pixel(x,y);

	  if(sampleVisible){
		frameBuffer[y * 480 + x] = 
		  (( colorMatrix[0][((bgImage->pixel(x,y) & 0x00ff0000)>>16) & 0x000000ff] << 8) & 0xf800 )
		  | (( colorMatrix[1][((bgImage->pixel(x,y) & 0x0000ff00)>>8) & 0x000000ff] << 3) & 0x07e0)
		  | (( colorMatrix[2][((bgImage->pixel(x,y) & 0x000000ff)) & 0x000000ff] >> 3) & 0x001f);
	  }
	  else{
	    frameBuffer[y * 480 + x] = 
	      (((tmpColor & 0x00ff0000) >> 8) & 0xf800)
	      | (((tmpColor & 0x0000ff00) >> 5) & 0x07e0)
	      | (((tmpColor & 0x000000ff) >> 3) & 0x001f);
	  }

	  }
	}
    }


}


void Canvas::mousePressEvent( QMouseEvent *e )
{

  if(e->y() < 600){
    fileChangeFlag = true;
    prevLevel = currentLevel;
    currentLevel = (int)(SCREEN_HEIGHT-1-e->y())/40;
  }

  int tmpleft = (255 - bgColor[colorIndex][prevLevel])*2 + XOFFSET;
  int tmptop = (15 - prevLevel)*40 + 20;
  QRegion r = QRegion(tmpleft-4,tmptop-4,9,9,QRegion::Rectangle);
  repaint(r, false);

  prevColor = bgColor[colorIndex][currentLevel];
  if(e->x() > XOFFSET && e->x() < SCREEN_WIDTH){
    bgColor[colorIndex][currentLevel] = 255 - (e->x() - XOFFSET)/2;
  }

  rebuildColorMatrix();

  repaint_rect();
}


void Canvas::mouseReleaseEvent( QMouseEvent *e )
{
  //  if(e->y() < 600){
  //    currentLevel = (int)(SCREEN_HEIGHT-1-e->y())/40 ;
  //  }

  //  repaint_rect();


}


void Canvas::mouseMoveEvent( QMouseEvent *e )
{
  // if(e->y() < 600){
  // currentLevel = (int)(SCREEN_HEIGHT-1-e->y())/40 ;
  //}
  prevColor = bgColor[colorIndex][currentLevel];
  if(e->x() > XOFFSET && e->x() < SCREEN_WIDTH){
    bgColor[colorIndex][currentLevel] = 255 - (e->x() - XOFFSET)/2;
  }
  repaint_rect();

  rebuildColorMatrix();
}

void Canvas::rebuildColorMatrix(){

  for(int i=0; i<3; i++){
    for(int j=0; j<15; j++){
      colorMatrix[i][j] = j;
    }
  }

  for(int i=0; i<3; i++){
    for(int j=15; j<255; j++){
      int level = (j-15)/16;
      int amari = (j-15)%16;
      colorMatrix[i][j] = bgColor[i+1][level] + (bgColor[i+1][level+1] - bgColor[i+1][level])*amari/16;
    }
    colorMatrix[i][255] = bgColor[i+1][15];
  }

}


void Canvas::save(){

  fileChangeFlag = false;

  uchar tmpColor[3][256];

  for(int i=0; i<3; i++){
    for(int j=0; j<15; j++){
      tmpColor[i][j] = j;
    }
  }

  for(int i=0; i<3; i++){
    for(int j=15; j<255; j++){
      int level = (j-15)/16;
      int amari = (j-15)%16;
      tmpColor[i][j] = 	bgColor[i+1][level] + (bgColor[i+1][level+1] - bgColor[i+1][level])*amari/16;
    }
    tmpColor[i][255] = bgColor[i+1][15];
  }

  QFile f( "/home/zaurus/Settings/ZColorAdjust.conf" );

  f.open( IO_WriteOnly );
  QDataStream s( &f );
  
  for(int i=0; i<3; i++){
    for(int j=0; j<256; j++){
      s << (Q_INT8)tmpColor[i][j];
      //      fprintf(stderr, "%d,",tmpColor[i][j]);
    }
  }
  f.close();



}

void Canvas::repaint_rect()
{

  if(sampleVisible){
    QRegion r = QRegion(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,QRegion::Rectangle);
    repaint(r, false);
    return;
  }

  if(currentLevel == 15){
    int x2 = (255 - bgColor[colorIndex][currentLevel])*2 + XOFFSET;
    int x3 = (255 - bgColor[colorIndex][currentLevel-1])*2 + XOFFSET;
    int x4 = (255 - prevColor)*2 + XOFFSET;
    int max = ( x2 > x3 ) ? x2 : x3;
    if( x4 > max ) max = x4;
    int min = ( x2 < x3 ) ? x2 : x3;
    if( x4 < min ) min = x4;

    repaint_left = min;
    repaint_width = max - min +1;
    
    repaint_top = 0;
    repaint_height = 80;

  }
  else if(currentLevel == 0){
    int x1 = (255 - bgColor[colorIndex][currentLevel+1])*2 + XOFFSET;
    int x2 = (255 - bgColor[colorIndex][currentLevel])*2 + XOFFSET;
    int x4 = (255 - prevColor)*2 + XOFFSET;
    int max = ( x1 > x2 ) ? x1 : x2;
    if( x4 > max ) max = x4;
    int min = ( x1 < x2 ) ? x1 : x2;
    if( x4 < min ) min = x4;

    repaint_left = min;
    repaint_width = max - min +1;
    
    repaint_top = (15 - currentLevel -1)*40;
    repaint_height = 80;
  }
  else{
    int x1 = (255 - bgColor[colorIndex][currentLevel+1])*2 + XOFFSET;
    int x2 = (255 - bgColor[colorIndex][currentLevel])*2 + XOFFSET;
    int x3 = (255 - bgColor[colorIndex][currentLevel-1])*2 + XOFFSET;
    int x4 = (255 - prevColor)*2 + XOFFSET;
    int max = ( x1 > x2 ) ? x1 : x2;
    int max2 = ( x3 > x4 ) ? x3 : x4;
    if( max2 > max ) max = max2;
    int min = ( x1 < x2 ) ? x1 : x2;
    int min2 = ( x3 < x4 ) ? x3 : x4;
    if( min2 < min ) min = min2;

    repaint_left = min;
    repaint_width = max - min +1;
    
    repaint_top = (15 - currentLevel -1)*40;
    repaint_height = 120;
  }

  QRegion r = QRegion(0,(15-currentLevel)*40,SCREEN_WIDTH,40,QRegion::Rectangle);
  repaint(r, false);

  r = QRegion(repaint_left-3,repaint_top,repaint_width+6,repaint_height,QRegion::Rectangle);
  repaint(r, false);


}
