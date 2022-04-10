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
#include <QFileInfo>
#include <QtCore/QStringBuilder>
#include "constants.hpp"
#include "window.hpp"

static const QColor mm_light = "#665c74";
static const QColor mm_dark = "#3d4148";
static const int bar_height = 32;

QString normal_bufname(QString name)
{
  if (name.isEmpty())
    return QString("No Name");
  int i = name.lastIndexOf("/");
  if (i < 0)
    i = name.lastIndexOf("\\");
  if (i >= 0)
    return name.right(name.length() - i - 1);
  return name;
}

TabBar::TabBar(Window* window)
  : QWidget(window)
  , win(window)
  , foreground("#3d4148")
  , background("#ffffff")
{
  setMouseTracking(true);
  setFocusPolicy(Qt::NoFocus);
  setFixedHeight(bar_height);
  setContentsMargins(10, 0, 0, 0);

  win->setMenuWidget(this);

  // application icon
  int icon_size = (int)((float)bar_height * 0.5f);
  auto app_icon = new QLabel(this);
  app_icon->setPixmap(QIcon(constants::appicon()).pixmap(icon_size, icon_size));
  app_icon->setMouseTracking(true);

  // buffer tab bar
  tabbar = new QTabBar(this);
  tabbar->setTabsClosable(true);
  tabbar->setMovable(true);
  tabbar->setMouseTracking(true);
  tabbar->setFocusPolicy(Qt::NoFocus);
  tabbar->setFixedHeight(bar_height - 5);
  //tabbar->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
  tabbar->setExpanding(false);

  auto tablayout = new QVBoxLayout();
  tablayout->addStretch();
  tablayout->addWidget(tabbar);

  // buttons
  min_btn = create_min_btn();
  max_btn = create_max_btn();
  close_btn = create_close_btn();

  // layout
  auto layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->addWidget(app_icon);
  layout->addLayout(tablayout);
  layout->addStretch();
  layout->addWidget(min_btn);
  layout->addWidget(max_btn);
  layout->addWidget(close_btn);

  // setup tabbar layout
  setLayout(layout);

  QObject::connect(min_btn, SIGNAL(clicked()), win, SLOT(showMinimized()));
  QObject::connect(max_btn, SIGNAL(clicked()), this, SLOT(minimize_maximize()));
  QObject::connect(close_btn, SIGNAL(clicked()), win, SLOT(close()));
  QObject::connect(tabbar, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_close_requested(int)));
  QObject::connect(tabbar, SIGNAL(currentChanged(int)), this, SLOT(on_current_changed(int)));
  QObject::connect(tabbar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tab_moved(int, int)));
}

QIcon TabBar::get_icon(QStyle::StandardPixmap sp) const
{
  auto sp_px = QApplication::style()->standardPixmap(sp);
  auto mask = sp_px.createMaskFromColor({"black"}, Qt::MaskOutColor);
  sp_px.fill(foreground);
  sp_px.setMask(mask);
  return QIcon(sp_px);
}

QPushButton* TabBar::create_menu_btn(const QIcon& icon) const
{
  auto btn = new QPushButton();
  btn->setFocusPolicy(Qt::NoFocus);
  btn->setFlat(true);
  btn->setAutoFillBackground(true);
  btn->setMouseTracking(true);
  btn->setIcon(icon);
  btn->setFixedSize(bar_height, bar_height);
  return btn;
}

QPushButton* TabBar::create_min_btn() const
{
  auto icon = get_icon(QStyle::SP_TitleBarMinButton);
  auto btn = create_menu_btn(icon);
  return btn;
}

QPushButton* TabBar::create_max_btn() const
{
  auto icon = get_icon(QStyle::SP_TitleBarMaxButton);
  auto btn = create_menu_btn(icon);
  return btn;
}

QPushButton* TabBar::create_close_btn() const
{
  auto icon = get_icon(QStyle::SP_TitleBarCloseButton);
  auto btn = create_menu_btn(icon);
  btn->setStyleSheet(
    "QPushButton:hover{ background-color: rgb(225,0,0); }"
    "QPushButton:pressed{ background-color: rgb(192,0,0); }"
  );
  return btn;
}

void TabBar::minimize_maximize()
{
  if (win->isMaximized())
  {
    win->showNormal();
  }
  else
  {
    win->maximize();
  }
}

void TabBar::win_state_changed(Qt::WindowStates states)
{
  // update max/normal icon
  if (win->isMaximized())
  {
    max_btn->setIcon(get_icon(QStyle::SP_TitleBarNormalButton));
  }
  else
  {
    max_btn->setIcon(get_icon(QStyle::SP_TitleBarMaxButton));
  }
}

void TabBar::mouseDoubleClickEvent(QMouseEvent* mevent)
{
  minimize_maximize();
}

void TabBar::buffer_enter(int bufn, QString name, QString path)
{
  name = normal_bufname(name);
  if (tabinfomap.find(bufn) == tabinfomap.end())
  {
    tab_create(bufn, name, path);
    tabinfomap[bufn] = { bufn, name, path };
  }
  tab_enter(bufn);
}

void TabBar::buffer_leave(int bufn, QString name, QString path)
{
  if (tabinfomap.find(bufn) != tabinfomap.end())
  {
    tab_leave(bufn);
  }
}

void TabBar::buffer_delete(int bufn, QString name, QString path)
{
  if (tabinfomap.find(bufn) != tabinfomap.end())
  {
    tab_delete(bufn);
    tabinfomap.erase(bufn);
  }
}

void TabBar::tab_create(int bufn, QString name, QString path)
{
  int index = tabbar->addTab(name);
  tabbar->setTabToolTip(index, path);
  tabbar->setTabData(index, QVariant(bufn));
}

void TabBar::tab_enter(int bufn)
{
  if (int i = get_tab_index(bufn); i >= 0)
  {
    if (tabbar->currentIndex() != i)
    {
      tabbar->setCurrentIndex(i);
    }
  }
}

void TabBar::tab_leave(int bufn)
{
}

void TabBar::tab_delete(int bufn)
{
  if (int i = get_tab_index(bufn); i >= 0)
  {
    tabbar->removeTab(i);
  }
}

int TabBar::get_tab_index(int bufn)
{
  for (int i = 0; i < tabbar->count(); ++i)
  {
    if (bufn == tabbar->tabData(i).toInt())
    {
      return i;
    }
  }
  return -1;
}

void TabBar::on_tab_close_requested(int index)
{
  int bufn = tabbar->tabData(index).toInt();
  printf("close requested: %d, %d\n", index, bufn);
  // close tab
  tabbar->removeTab(index);
  // nvim command
  char command[32];
  sprintf(command, "bd %d", index);
  // send command
  Nvim* nvim = win->get_current_editor()->get_nvim();
  nvim->command(command);
}

void TabBar::on_current_changed(int index)
{
  int bufn = tabbar->tabData(index).toInt();
  printf("current changed: %d, %d\n", index, bufn);
  // nvim command
  char command[32];
  sprintf(command, "b %d", index);
  // send command
  Nvim* nvim = win->get_current_editor()->get_nvim();
  nvim->command(command);
}

void TabBar::on_tab_moved(int from, int to)
{
  printf("moved: %d, %d\n", from, to);
}

