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


class Window;

/// Custom tabbar implementation that operates on the Window class.
class TabBar : public QWidget
{
  Q_OBJECT

public:
  TabBar(Window* window);

public slots:
  void minimize_maximize();
  void win_state_changed(Qt::WindowStates state);

//  void colors_changed(QColor fg, QColor bg);
//signals:
//  void resize_move(QPointF p);

protected:
  void mouseDoubleClickEvent(QMouseEvent* mevent);

private:
  QIcon get_icon(QStyle::StandardPixmap sp) const;
  QPushButton* create_menu_btn(const QIcon& icon) const;
  QPushButton* create_min_btn() const;
  QPushButton* create_max_btn() const;
  QPushButton* create_close_btn() const;

  QColor foreground;
  QColor background;
  Window* win;
  QPushButton* close_btn;
  QPushButton* max_btn;
  QPushButton* min_btn;
  QTabBar* tabbar;
};
#endif
