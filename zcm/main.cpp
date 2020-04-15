#include <qpe/qpeapplication.h>
#include <qmainwindow.h>

#include "zcm.h"

int main(int argc, char **argv) {

  QPEApplication a(argc, argv);

  ZColorMatching zcm;
  a.showMainWidget(&zcm);

  return a.exec();
}

