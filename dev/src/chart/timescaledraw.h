#ifndef TIMESCALEDRAW_H
#define TIMESCALEDRAW_H

#include <qwt_scale_draw.h>

#include <QDateTime>

//! \brief Draw label as time
/*! \ingroup chart
   A base time corresponding to the start time is used, each value in double been used as second from this start time.
  */
class TimeScaleDraw: public QwtScaleDraw
{
public:
   //! \brief Constructor
   /*!
     */
   TimeScaleDraw();

   //! \brief Retrieve the text associated with this value on the X axis
   /*! The returned string depends on the chart settings (either relative or absolute)
     */
   virtual QwtText label(double v) const;
};

#endif // TIMESCALEDRAW_H
