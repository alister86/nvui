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

struct TabInfo
{
  int bufn;
  QString name;
  QString path;
};

/// Custom tabbar implementation that operates on the Window class.
class TabBar : public QWidget
{
  Q_OBJECT

public:
  TabBar(Window* window);

public slots:
  void minimize_maximize();
  void win_state_changed(Qt::WindowStates state);
  void on_tab_close_requested(int index);
  void on_current_changed(int index);
  void on_tab_moved(int from, int to);

public:
  void buffer_enter(int bufn, QString name, QString path);
  void buffer_leave(int bufn, QString name, QString path);
  void buffer_delete(int bufn, QString name, QString path);

protected:
  void mouseDoubleClickEvent(QMouseEvent* mevent);

private:
  QIcon get_icon(QStyle::StandardPixmap sp) const;
  QPushButton* create_menu_btn(const QIcon& icon) const;
  QPushButton* create_min_btn() const;
  QPushButton* create_max_btn() const;
  QPushButton* create_close_btn() const;
  void tab_create(int bufn, QString name, QString path);
  void tab_enter(int bufn);
  void tab_leave(int bufn);
  void tab_delete(int bufn);
  int get_tab_index(int bufn);

  QColor foreground;
  QColor background;
  Window* win;
  QPushButton* close_btn;
  QPushButton* max_btn;
  QPushButton* min_btn;
  QTabBar* tabbar;
  std::unordered_map<int, TabInfo> tabinfomap;
};
#endif
