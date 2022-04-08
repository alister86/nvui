#include "tabbar.hpp"
#include "utils.hpp"
#include <cassert>
#include <QApplication>
#include <QObject>
#include <QPalette>
#include <iostream>
#include <QLayoutItem>
#include <stdexcept>
#include <string>
#include <utility>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QRect>
#include <QScreen>
#include <QSize>
#include <QWindow>
#include <QtCore/QStringBuilder>
#include "constants.hpp"
#include "window.hpp"

TabBar::TabBar(Window* window)
  : QWidget(window)
{
  setFixedHeight(50);
  window->setMenuWidget(this);

  tabbar = new QTabBar(this);
  tabbar->setTabsClosable(true);
  tabbar->setMovable(true);
  tabbar->addTab("file 0");
  tabbar->addTab("file 1");
  tabbar->addTab("file 2");
  tabbar->addTab("file 3");
  tabbar->addTab("file 4");

  layout = new QHBoxLayout();
  layout->addWidget(tabbar);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);

  setLayout(layout);
  setContentsMargins(40, 2, 100, 0);
}

