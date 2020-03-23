#ifndef PLOTPICKER_H
#define PLOTPICKER_H

#include <qwt_plot_picker.h>
#include <qwt_plot_canvas.h>

//! \brief Manages the events on plot
/*! \ingroup chart
   Overrides the QwtPlotPicker class  in order to show correct values when moving the mouse on the plot
  */
class PlotPicker : public QwtPlotPicker
{
    Q_OBJECT

public:
    //! \brief Constructor
    /*!
     @param xAxis X axis of the picker
     @param yAxis Y axis of the picker
     @param rubberBand Rubberband style
     @param trackerMode Tracker mode
     @param canvas Plot canvas to observe, also the parent object
      */
    explicit PlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QwtPlotCanvas * canvas);

protected:
    //! \brief Get the tracker text
    /*!
      Is called everytime the mouse is moved on the plot to show a tracker text
      @param point The position of the cursor
      @return The text to show
      */
    QwtText trackerTextF(const QPointF& point) const;

    //! \brief Double click event
    /*!
      Is called when a double click occured on the plot
      @param event The mouse event object
      */
    void widgetMouseDoubleClickEvent(QMouseEvent* event);

    //! \brief Event launched when the mouse is moved within the plot
    void widgetMouseMoveEvent(QMouseEvent* event);

    //! \brief Event launched when the mouse is pressed within the plot
    void widgetMousePressEvent(QMouseEvent* event);

    //! \brief Event launched when the mouse is released within the plot
    void widgetMouseReleaseEvent(QMouseEvent* event);

    void widgetLeaveEvent(QEvent* event);
    void widgetEnterEvent(QEvent* event);

    void widgetWheelEvent(QWheelEvent* event);

signals:

    //! \brief Double click signal
    /*!
      Is emitted when a double click occured on the plot
      @param pos The position of the click on the plot
      */
    void on_double_clicked(const QPointF &pos);

    void on_clicked(const QPointF &pos, QMouseEvent *event);

    //! \brief Emitted when the mouse has move within the plot
    void on_mouse_moved(const QPointF &pos);

    void on_mouse_entered();
    void on_mouse_leaved();

    void on_mouse_wheeled(bool up);

public slots:

private:

    QPoint position;
};

#endif // PLOTPICKER_H
