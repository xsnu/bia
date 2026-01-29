#pragma once

// LIBRARIES

#include <Qt>
#include <QStyleHints>

#include <QShortcut>
#include <QKeySequence>

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QToolBar>
#include <QToolButton>
#include <QIcon>
#include <QTextEdit>
#include <QSize>

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QString>
#include <QFileDialog>
#include <QFile>

#include <QString>
#include <QPdfView>
#include <QPdfDocument>
#include <QPdfPageNavigator>

// OPEN DOCUMENT FUNCTION
void openFile(QMainWindow& win, QPdfView* pdf, QPdfDocument* doc, QLabel* pagecount) {
  QString filename = QFileDialog::getOpenFileName(&win, "Open File", "", "PDF Files (*.pdf)");
  if (!filename.isEmpty()) {
    doc->load(filename);
    pdf->setDocument(doc);
    pdf->setPageMode(QPdfView::PageMode::MultiPage);
    QPdfPageNavigator *nav = pdf->pageNavigator();

    QObject::connect(pdf->pageNavigator(), &QPdfPageNavigator::currentPageChanged, [pagecount, pdf, doc](int page) {
        pagecount->setText("Page " + QString::number(page + 1) + " of " + QString::number(doc->pageCount()) + " Pages");
    });

    pagecount->setText("Page " + QString::number(nav->currentPage() + 1) + " of " + QString::number(doc->pageCount()) + " Pages");
  }
}

// ABOUT FUNCTION
void aboutProgram(QMainWindow& win) {
  QDialog dialog(&win);
  dialog.setWindowTitle("About");

  QLabel *text = new QLabel("BIA - Simple Book Reader");
  text->setAlignment(Qt::AlignCenter);

  QPixmap image("icons/tuxbook.png");
  QLabel *imagel = new QLabel();
  imagel->setPixmap(image.scaled(250, 250, Qt::KeepAspectRatio));
  imagel->setAlignment(Qt::AlignCenter);

  QLabel *info = new QLabel("Version: 0.1\nAuthor: xsnu\nHomepage: https://github.com/suigarra/bia\nLicense: MIT");
  QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close);

  QVBoxLayout *layout = new QVBoxLayout(&dialog);
  layout->addWidget(text);
  layout->addWidget(imagel);
  layout->addWidget(info);
  layout->addWidget(button);

  QObject::connect(button, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  dialog.exec();
}

// ZOOM FUNCTION
double zfactor = 1;

void zoom(QPdfView* pdf, QLabel* zlabel, bool check) {
  if (check != false) {
    zfactor += 0.1;
  } else {
    zfactor -= 0.1;
  }

  // SET MAX & MIN ZOOM FACTOR
  if (zfactor < 0.1) {
    zfactor = 0.1;
  } else if (zfactor > 5) {
    zfactor = 5;
  }
  zlabel->setText(QString::number(zfactor * 100) + "%");
  pdf->setZoomFactor(zfactor);
}

// PREFERENCES FUNCTION
void preferences(QMainWindow& win) {
  QDialog dialog(&win);
  dialog.setWindowTitle("Preferences");

  QPushButton *darktheme = new QPushButton("Dark Theme");
  QPushButton *whitetheme = new QPushButton("White Theme");
  QPushButton *defaulttheme = new QPushButton("Default Theme");

  QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Ok);

  QVBoxLayout *layout = new QVBoxLayout(&dialog);
  layout->addWidget(darktheme);
  layout->addWidget(whitetheme);
  layout->addWidget(defaulttheme);
  layout->addWidget(button);

  QObject::connect(button, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  QObject::connect(darktheme, &QPushButton::clicked, [&]() {
    qApp->setStyleSheet("QWidget { background-color: #1b1b1b; color: white;}");
  });
  QObject::connect(whitetheme, &QPushButton::clicked, [&]() {
    qApp->setStyleSheet("QWidget { background-color: #ffffff; color: black;}");
  });
  QObject::connect(defaulttheme, &QPushButton::clicked, [&]() {
    qApp->setStyleSheet("");
  });

  dialog.exec();
}
