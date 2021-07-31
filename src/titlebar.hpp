#ifndef NVUI_TITLEBAR_HPP
#define NVUI_TITLEBAR_HPP

#include <QLayout>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QPointF>
#include <QEvent>
#include <QLabel>
#include <QPushButton>

/// MenuButtons for our TitleBar that inherit from QPushButton
/// These have overridden the default hover events to provide
/// support for custom resizing as well as background changing
/// on hover. For more info, see the implementation
class MenuButton;

class Window;

/// Custom titlebar implementation that operates on the Window class.
class TitleBar : public QWidget
{
  Q_OBJECT
public:
  TitleBar(QString text, Window* window);
  /**
   * Sets the right-side text to text.
   */
  void set_right_text(QString text);
  /**
   * Sets the left text.
   */
  void set_left_text(QString text);
  /**
   * Sets the middle text.
   */
  void set_middle_text(QString text);
  /**
   * Sets foreground to color if foreground is true,
   * otherwise sets background to color.
   */
  void set_color(QColor color, bool is_foreground = true);
  /**
   * Sets the foreground and background color to bg and fg respectively.
   */
  void set_color(QColor fg, QColor bg);
  /**
   * Set the separator string, which is what separates left_text and
   * right_text
   */
  void set_separator(QString new_sep);
  /**
   * Update the icons based on the status of the attached window (win).
   */
  void update_maxicon();
  /**
   * Hide the titlebar
   */
  inline void hide()
  {
    if (titlebar_widget->isHidden()) return;
    titlebar_widget->hide();
  }
  /**
   * Show the titlebar
   */
  inline void show()
  {
    if (titlebar_widget->isVisible()) return;
    titlebar_widget->show();
  }
  inline void set_font_family(QString new_fam)
  {
    title_font.setFamily(new_fam);
    label->setFont(title_font);
  }
  inline void set_font_size(double new_size)
  {
    title_font.setPointSizeF(new_size);
    label->setFont(title_font);
  }
  inline QWidget* widget() const
  {
    return titlebar_widget;
  }
private:
  /**
   * Updates the titlebar with new colors.
   * This should only be called after the colors are updated
     (not when new text is set).
   */
  void update_titlebar();
  /**
   * Updates the titlebar text to the current values of
   * left_text, separator, and right_text
   */
  void update_text();
  QString separator;
  bool maximized;
  QColor foreground;
  QColor background;
  QIcon close_icon;
  QIcon max_icon;
  QIcon min_icon;
  MenuButton* close_btn;
  MenuButton* max_btn;
  MenuButton* min_btn;
  //QPushButton* close_btn;
  //QPushButton* max_btn;
  //QPushButton* min_btn;
  Window* win;
  QFont title_font;
  QString left_text;
  QString right_text;
  QString middle_text;
  QLabel* label;
  QHBoxLayout* layout;
  QWidget* titlebar_widget;
public slots:
  void minimize_maximize();
  void colors_changed(QColor fg, QColor bg);
  void win_state_changed(Qt::WindowStates state);
signals:
  void resize_move(QPointF p);
};
#endif
