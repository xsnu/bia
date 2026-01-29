// BIA -- simple book reader

// Copyright (C) 2026 xsnu

// Author: xsnu
// Version: 0.1
// Keywords: productivity
// Homepage: https://github.com/xsnu/bia
// License: MIT

#include <iostream>
#include "utils.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow win;
  win.setWindowTitle("bia");

  QMenuBar *bar = win.menuBar();

  // FILE MENU
  QMenu *file = bar->addMenu("&File");

  QAction *open = file->addAction("&Open");
  open->setIcon(QIcon("icons/open.png"));
  QAction *close = file->addAction("&Close");
  close->setIcon(QIcon("icons/close.png"));

  file->addSeparator();

  QAction *about = file->addAction("&About");
  about->setIcon(QIcon("icons/about.png"));
  QAction *quit = file->addAction("&Quit");
  quit->setIcon(QIcon("icons/quit.png"));

  // EDIT MENU
  QMenu *edit = bar->addMenu("&Edit");

  QAction *pref = edit->addAction("&Preferences");
  pref->setIcon(QIcon("icons/preferences.png"));

  // TOOLBAR
  QToolBar *tools = new QToolBar(&win);

  QLabel *pagecount = new QLabel("Page 0 of 0 Pages");

  QToolButton *zfit = new QToolButton(&win);
  zfit->setIcon(QIcon("icons/fit.png"));
  zfit->setEnabled(false);
  QToolButton *zcus = new QToolButton(&win);
  zcus->setIcon(QIcon("icons/custom.png"));
  zcus->setEnabled(false);
  QToolButton *zoomin = new QToolButton(&win);
  zoomin->setIcon(QIcon("icons/zoomin.png"));
  zoomin->setEnabled(false);
  QToolButton *zoomout = new QToolButton(&win);
  zoomout->setIcon(QIcon("icons/zoomout.png"));
  zoomout->setEnabled(false);
  QLabel *zlabel = new QLabel("100%");

  tools->addWidget(zfit);
  tools->addWidget(zcus);

  tools->addSeparator();

  tools->addWidget(zoomin);
  tools->addWidget(zlabel);
  tools->addWidget(zoomout);

  tools->addSeparator();

  tools->addWidget(pagecount);

  // MAIN WIDGETS
  QWidget *widget = new QWidget(&win);

  QPdfView *pdf = new QPdfView(&win);
  QPdfDocument *doc = new QPdfDocument(&win);

  QVBoxLayout *layout = new QVBoxLayout(widget);
  layout->addWidget(tools);
  layout->addWidget(pdf);

  // BUTTONS
  QObject::connect(open, &QAction::triggered, [&]() {
    openFile(win, pdf, doc, pagecount);
    zfit->setEnabled(true);
    zcus->setEnabled(true);
    zoomin->setEnabled(true);
    zoomout->setEnabled(true);
  });
  QObject::connect(close, &QAction::triggered, [&]() {
    doc->close();
    pagecount->setText("Page 0 of 0 Pages");

    zfit->setEnabled(false); zcus->setEnabled(false);
    zoomin->setEnabled(false); zoomout->setEnabled(false);
  });
  QObject::connect(about, &QAction::triggered, [&]() {
    aboutProgram(win);
  });
  QObject::connect(zfit, &QToolButton::clicked, [&]() {
    pdf->setZoomMode(QPdfView::ZoomMode::FitToWidth);

    zoomin->setEnabled(false); zoomout->setEnabled(false);
  });
  QObject::connect(zcus, &QToolButton::clicked, [&]() {
    pdf->setZoomMode(QPdfView::ZoomMode::Custom);

    zoomin->setEnabled(true); zoomout->setEnabled(true);
  });
  QObject::connect(zoomin, &QToolButton::clicked, [&]() {
    zoom(pdf, zlabel, true); // TRUE MEANS ZOOM IN
  });
  QObject::connect(zoomout, &QToolButton::clicked, [&]() {
    zoom(pdf, zlabel, false); // FALSE MEANS ZOOM OUT
  });
  QObject::connect(quit, &QAction::triggered, &app, &QApplication::quit);

  QObject::connect(pref, &QAction::triggered, [&]() {
    preferences(win);
  });

  // SHORTCUTS
  QShortcut *szoomin = new QShortcut(QKeySequence("Ctrl+="), &win);
  QObject::connect(szoomin, &QShortcut::activated, [&pdf, zlabel]() {
    zoom(pdf, zlabel, true); // TRUE MEANS ZOOM IN
  });
  QShortcut *szoomout = new QShortcut(QKeySequence("Ctrl+-"), &win);
  QObject::connect(szoomout, &QShortcut::activated, [&pdf, zlabel]() {
    zoom(pdf, zlabel, false); // FALSE MEANS ZOOM OUT
  });

  win.setCentralWidget(widget);
  win.show();

  return app.exec();
}
