#ifndef COVARIATE_H
#define COVARIATE_H

#include <QString>
#include <QMetaType>
#include <QDateTime>
#include <QMetaType>

#include "core/utils/ezutils.h"
#include "core/utils/units.h"
//#include "dbobjects.h"
#include "drug/operableamount.h"

#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

class UncastedValueList;

class PatientVariate : public Entity {
    Q_OBJECT

    ENTITY_UTILS(PatientVariate)

    Q_PROPERTY(QString covariateId READ getCovariateId WRITE setCovariateId NOTIFY covariateIdChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    //! Now only used for storing the birthdate
    AUTO_PROPERTY_DECL(QString, valueAsString, ValueAsString)
    AUTO_PROPERTY_DECL(OperableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(QString, description, Description)
    AUTO_PROPERTY_DECL(Descriptor, descriptor, Descriptor)
    AUTO_PROPERTY_DECL(QMetaType::Type, type, Type)
    //! To indicate if it was automatically calculated or forced by the user
    AUTO_PROPERTY_DECL(bool, forced, Forced)

    AUTO_PROPERTY_DECL(UncastedValueList*, uncastedValues, UncastedValues)

protected:

    Q_INVOKABLE PatientVariate(AbstractRepository *repository = 0, QObject *parent = nullptr, const QString &id = "", const QDateTime &date = QDateTime());
    Q_INVOKABLE PatientVariate(AbstractRepository *repository, QObject *parent, const QString &id, const QDateTime &date, double value);

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

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

    QString getCovariateId() const {return _covariateId;}
    bool setCovariateId(const QString covariateId);
    QString getName() const {return _name;}
    bool setName(const QString name);

    QString toString() const;
    bool isValid () const;
    void operator= (double value);
    bool operator== (const PatientVariate &) const;
    bool operator!= (const PatientVariate &) const;

signals:
    void nameChanged(QString);
    void covariateIdChanged(QString);

private:
    QString _name;
    QString _covariateId;
    bool _valid;
};

QML_POINTERLIST_CLASS_DECL(PatientVariateList, PatientVariate)

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::PatientVariate*)
Q_DECLARE_METATYPE(QList<ezechiel::core::PatientVariate*>)
Q_DECLARE_METATYPE(ezechiel::core::PatientVariateList*)

#endif // COVARIATE_H
