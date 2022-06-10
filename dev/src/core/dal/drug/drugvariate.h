//@@license@@

#ifndef DRUGVARIATE_H
#define DRUGVARIATE_H

#include <QObject>

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"
#include "core/utils/units.h"
#include "core/dal/drug/operableamount.h"

namespace Tucuxi {
namespace GuiCore {

class OperationList;
class TranslatableString;

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

} // namespace core
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::GuiCore::DrugVariate*)
Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::DrugVariate*>)
Q_DECLARE_METATYPE(Tucuxi::GuiCore::DrugVariateList*)

#endif // DRUGVARIATE_H
