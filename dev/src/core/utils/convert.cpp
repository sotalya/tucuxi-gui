//@@license@@

#include "convert.h"

#include <QObject>
#include <QMetaType>
#include <QTime>
#include <QDate>
#include <QStringList>
#include <QApplication>

//#include "core_errors.h"
#include "core/core.h"
#include "core/utils/ezutils.h"
//#include "coremessagehandler.h"
#include "distribution.h"
#include "core/dal/drug/parameters.h"
#include "core/utils/units.h"
#include "ezchoice.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"
#include "core/dal/drug/bsv.h"

namespace ezechiel {
namespace core {


Convert::Convert()
{
}

/* If the type is a QVariant::Type, just call .toString()
   Else, handle each custom types
  */
QString Convert::toString(int type, QVariant value)
{
   //Custom or normal type?
   if (uint(type) < QVariant::UserType) {
      //Check the QVariant
      if (!value.isValid()) {
         LOG(QtWarningMsg, NOEZERROR, tr("The given QVariant is invalid (type=%1)").arg(QString::number(type)));
         return QString();
      }
      //Following the type, we may want to use a different output
      switch (type) {
         //For the time, we prefer our time format
         case QVariant::Time:
            return value.value<QTime>().toString(TIME_FORMAT);
         //The same for the date
         case QVariant::Date:
            return value.value<QDate>().toString(DATE_FORMAT);
         //Usual stuff
         default:
            //Check if convertible
            if (!value.convert(QVariant::String)) {
               LOG(QtWarningMsg, NOEZERROR, tr("The given QVariant is not convertible to a string (type=%1)").arg(QString::number(type)));
               return QString();
            }
            // All green
            return value.toString();
      }
   }
   // Custom type
   //Duration
   if (type == QMetaType::type("Duration")) {
      //Duration has its own .toString() function
      return value.value<Duration>().toString();
   //Parameter
   } else if (type == QMetaType::type("SharedParameter")) {
      SharedParameter p = value.value<SharedParameter>();
      // PID=value:standard:proportional:distribution
      return QString("%1=%2:%3:%4:%5:%6:%7:%8:%9")
            .arg(p->getName())
            .arg(p->getQuantity()->value())
            .arg(p->getBsv()->getStandard())
            .arg(p->getBsv()->getProportional())
//            .arg(distribution::toString(p->getDistribution()))
            .arg(p->getMin())
            .arg(p->getMax())
            .arg(p->getStep())
            .arg(p->getQuantity()->unit().name());
   //DescriptorList
   } else if (type == QMetaType::type("EzChoice")) {
      //Return the first value (selected)
      return value.value<EzChoice>().selectedId();
   //ParameterType
   } else if (type == QMetaType::type("ParameterType")) {
      switch(value.value<ParameterType>()) {
         case POPULATION : return "POPULATION";
         case APRIORI : return "APRIORI";
         case APOSTERIORI : return "APOSTERIORI";
         case MANUAL : return "MANUAL";
         default :
            LOG(QtWarningMsg, NOEZERROR, tr("A developper just forget to add this new parameter value in the Convert object. You can hit him if you like, or simply contact the development team."));
            return "";
      }
   //Unknown
   } else
      LOG(QtWarningMsg, NOEZERROR, tr("Unknown type")+" : "+QMetaType::typeName(type)+"("+QString::number(type)+")");
   return QString();
}

//Use the other toString with the given QVariant's type
QString Convert::toString (QVariant value)
{
   return toString(value.userType(),value);
}

/* For normal types, handles differently the times and dates, but for the most use a simple convert().
   If the convertion fails, switch between certain type to give a meaningfull error
   For custom type, use the specific type way of conversion.
  */
QVariant Convert::fromString(int type, QString str)
{
   QVariant result;
   QDateTime date;
   //Normal types
   if (uint(type) < QVariant::UserType) {
      //For certain types, do a special conversion
      switch (type) {
         //Times
         case QVariant::Time:
            //Use our own format
            result.setValue(QTime::fromString(str,TIME_FORMAT));
            if (!result.toTime().isValid()) {
               LOG(QtWarningMsg, NOEZERROR, tr("Invalid time given, expecting a value in the form")+" "+TIME_FORMAT+" "+tr("received")+" "+str);
               return QVariant();
            }
            break;
         //Dates
         case QVariant::Date:
            //Use our own format
            result.setValue(QDate::fromString(str,DATE_FORMAT));
            if (!result.toDate().isValid()) {
               LOG(QtWarningMsg, NOEZERROR, tr("Invalid date given, expecting a value in the form")+" "+DATE_FORMAT+" "+tr("received")+" "+str);
               return QVariant();
            }
            break;
         //DateTime
         case QVariant::DateTime:
            //Read only the time, set the day to today
            date = QDateTime::currentDateTime();
            date.setTime(QTime::fromString(str,TIME_FORMAT));
            if (!date.isValid()) {
               LOG(QtWarningMsg, NOEZERROR, tr("Invalid date and time given, expecting a value in the form")+" "+TIME_FORMAT+" "+tr("received")+" "+str);
               return QVariant();
            }
            result = QVariant::fromValue(date);
            break;
         //Simple conversion
         default:
            result.setValue(str);
            if (!result.convert(QVariant::Type(type))) {
                LOG(QtWarningMsg, NOEZERROR, tr("Impossible to convert '%1' to the type %2(%3)").arg(str,QVariant::typeToName(QVariant::Type(type)),QString::number(type)));
               return QVariant();
            }
      }
      //All green, return the result
      return result;
   }

   // Custom types
   //Duration
   if (type == QMetaType::type("Duration")) {
      //Duration, has its own fromString() function
      Duration d;
      if (!d.fromString(str)) {
         LOG(QtWarningMsg, NOEZERROR, tr("Invalid duration format, expecting something like 12, 12h, 12h30 , got : %1").arg(str));
         return QVariant();
      }
      //result.setValue(d);
      result = QVariant::fromValue(d);
   //Parameter
   } else if (type == QMetaType::type("Parameter")) {
      //Separate the PID from the values
      QStringList data = str.split("=");
      if (data.size() != 2) {
         LOG(QtWarningMsg, NOEZERROR, tr("Invalid parameter given, expecting a value in the format PID=value[:standard[:proportional[:distribution]]] . got : %1").arg(str));
         return QVariant();
      }
      //Manual, each value is separated with a ':'
      QStringList values = data.at(1).split(":");
      SharedParameter p = (ezechiel::core::CoreFactory::createEntity<Parameter>(ABSTRACTREPO, nullptr));
      p->setName(data.at(0));
      p->getQuantity()->setValue(values.at(0).toDouble());
      //The rest is optional
      p->getBsv()->setStandard((values.size() > 1) ? values.at(1).toDouble():0.0);
      p->getBsv()->setProportional((values.size() > 2) ? values.at(2).toDouble():0.0);
//      p->setDistribution(distribution::fromString((values.size() > 3) ? values.at(3):"logNormal"));
      p->setMin((values.size() > 4) ? values.at(4).toDouble():0.0);
      p->setMax((values.size() > 5) ? values.at(5).toDouble():0.0);
      p->setStep((values.size() > 6) ? values.at(6).toDouble():0.0);
      p->getQuantity()->setUnit((values.size() > 7) ? Unit(values.at(7)):Unit());
      return QVariant::fromValue(p);
   //DescriptorList
   } else if (type == QMetaType::type("EzChoice")) {
      //Return a choice with only one item and only the ID field filled, and selected
      EzChoice choice;
      choice.addChoice(Descriptor(str,"",""));
      choice.selectAt(0);
      return QVariant::fromValue(choice);
   //ParameterType
   } else if (type == QMetaType::type("ParameterType")) {
      //Simple switch like statement
      ParameterType pt;
      if (str == "POPULATION" || str == "population")
         pt = POPULATION;
      else if (str == "APRIORI" || str == "a-priori")
         pt = APRIORI;
      else if (str == "APOSTERIORI" || str == "a-posteriori")
         pt = APOSTERIORI;
      else if (str == "MANUAL" || str == "manual")
         pt = MANUAL;
      else {
         LOG(QtWarningMsg, NOEZERROR, tr("Invalide parameter type")+" : "+str);
         return QVariant();
      }
      return QVariant::fromValue(pt);
   //Unknow type
   } else { // return an empty QVariant
      LOG(QtWarningMsg, NOEZERROR, tr("Unknown type")+"="+tr("type"));
      return QVariant();
   }
   //All green
   return result;
}

} // namespace core
} // namespace ezechiel

