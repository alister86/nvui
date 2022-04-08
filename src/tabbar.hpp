#ifndef NVUI_TABBAR_HPP
#define NVUI_TABBAR_HPP

#include "constants.hpp"
#include "utils.hpp"
#include <QApplication>
#include <QBitmap>
#include <QEvent>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPointF>
#include <QPushButton>
#include <QResizeEvent>
#include <QStyle>
#include <QTabBar>

/// MenuButtons for our TabBar that inherit from QPushButton
/// These have overridden the default hover events to provide
/// support for custom resizing as well as background changing
/// on hover. For more info, see the implementation
class MenuButton;

class Window;

/// Custom tabbar implementation that operates on the Window class.
class TabBar : public QWidget
{
  Q_OBJECT
public:
  TabBar(Window* window);
//public slots:
//  void minimize_maximize();
//  void colors_changed(QColor fg, QColor bg);
//  void win_state_changed(Qt::WindowStates state);
//signals:
//  void resize_move(QPointF p);

  QLabel* label;
  QHBoxLayout* layout;
  QTabBar* tabbar;
};
#endif
