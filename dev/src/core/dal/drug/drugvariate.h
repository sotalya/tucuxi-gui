//@@license@@

#ifndef DRUGVARIATE_H
#define DRUGVARIATE_H

#include <QObject>

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"
#include "core/utils/units.h"
#include "core/dal/drug/operableamount.h"

//Q_MOC_INCLUDE("core/dal/drug/operation.h")
Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {

class OperationList;
class TranslatableString;

/// \brief Define the covariate types.
enum class CovariateType
{

    /// \brief Standard, no particular treatment of the value
    /// - if no patient variate exist -> use operation in drug model to generate a new value each time one or more
    ///                                  inputs of the operation are modified
    ///   if cannot apply operation   -> use default value
    ///   if >= 1 variate exists      -> if only one value -> use for the entire period
    ///                                  else              -> interpolate with function defined in
    ///                                                       CovariateDefinition, using first observed value for
    ///                                                       the interval between start and the first observation
    ///   \warning Look also at values outside the given period! The period itself limits the range of measures we are
    ///            interested in, but does not affect the available variates.
    Standard = 0,

    /// Age in years. Automatic calculation based on birth date, use default if not available, unit = years.
    AgeInYears,

    /// Age in days. Automatic calculation based on birth date, use default if not available, unit = days.
    AgeInDays,

    /// Age in weeks. Automatic calculation based on birth date, use default if not available, unit = weeks.
    AgeInWeeks,

    /// Age in months. Automatic calculation based on birth date, use default if not available, unit = months.
    AgeInMonths,

    /// \brief Sex of the person, based on his administrative data. 1 for male, 0 for female.
    /// Strangely, if Sex is at the second position, the test_covariateextractor.h tests will fail.
    Sex,

    /// \brief Automatic calculation of the dose, based on the intakes.
    /// The unit of the covariate will be used to convert the actual dose to the covariate.
    Dose
};


class DrugVariate : public Entity {

    Q_OBJECT

    Q_PROPERTY(QString unitString READ getUnitString WRITE setUnitString NOTIFY unitStringChanged)


    ENTITY_UTILS(DrugVariate)

    AUTO_PROPERTY_DECL(QString, covariateId, CovariateId)
//    AUTO_PROPERTY_DECL(QString, visualName, VisualName)
//    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    AUTO_PROPERTY_DECL(OperableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(Descriptor, descriptor, Descriptor)
    AUTO_PROPERTY_DECL(QString, description, Description)
    AUTO_PROPERTY_DECL(QMetaType::Type, type, Type)
    AUTO_PROPERTY_DECL(bool, automatic, Automatic)
    AUTO_PROPERTY_DECL(CovariateType, covariateType, CovariateType)

    //TODO change two next lines to #translatable
    AUTO_PROPERTY_DECL(TranslatableString*,visualNameTranslation,VisualNameTranslation)
    AUTO_PROPERTY_DECL(TranslatableString*, descriptionTranslation,DescriptionTranslation)
    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    // YTA: Not used yet, but soon in the future
    // For instance for the clearance of creatinin
//    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

protected:

    Q_INVOKABLE DrugVariate(AbstractRepository *repository = nullptr, QObject* parent = nullptr, const QString &id = "", const QDateTime &date = QDateTime());
    Q_INVOKABLE DrugVariate(AbstractRepository *repository, QObject* parent, const QString &id, const QDateTime &date, double value);

public:    

    Q_INVOKABLE QString typeToQString()
    {
        QString stringType;

        if(_type == QMetaType::Type::Double){
            stringType = "double";
        } else if(_type == QMetaType::Type::Int){
            stringType = "int";
        } else if (_type ==  QMetaType::Type::Bool){
            stringType = "bool";
        }else{
            stringType = "noType";
        }

        return stringType;
    }

    QString toString() const;
    void operator= (double value);
    bool operator== (const DrugVariate &) const;
    bool operator!= (const DrugVariate &) const;


    QString getUnitString() {return _quantity->unit().name();}
    void setUnitString(QString name) { _quantity->setUnit(name);}


signals:

    void unitStringChanged(QString);

private:

};

QML_POINTERLIST_CLASS_DECL(DrugVariateList, DrugVariate)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugVariate*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DrugVariate*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugVariateList*)

#endif // DRUGVARIATE_H
