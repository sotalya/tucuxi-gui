#ifndef PARTIALREQUEST_H
#define PARTIALREQUEST_H

#include "core/dal/entity.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/measure.h"
#include "institute.h"


namespace Tucuxi {
namespace GuiCore {
    class ActiveSubstance;
}
}

class PartialRequest : public Tucuxi::GuiCore::Entity
{
    Q_OBJECT

    AUTO_PROPERTY(QString, requestId)
    AUTO_PROPERTY(QString, requestCpcl)
    AUTO_PROPERTY(Patient*, patient)
    AUTO_PROPERTY(Practician*, practician)
    AUTO_PROPERTY(Institute*, institute)
    AUTO_PROPERTY(Measure*, measure)
    AUTO_PROPERTY(Tucuxi::GuiCore::ActiveSubstance*, drug)
    AUTO_PROPERTY(QString, uncastedSourceSubstance)
    AUTO_PROPERTY(Measure*, sample)

public:
    Q_INVOKABLE PartialRequest(Tucuxi::GuiCore::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE PartialRequest(Tucuxi::GuiCore::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};
Q_DECLARE_METATYPE(PartialRequest*)
Q_DECLARE_METATYPE(QList<PartialRequest*>)

class PartialRequestSet : public QList<PartialRequest*>
{
public :
    PartialRequestSet(Tucuxi::GuiCore::AbstractRepository *repository, QObject* parent = 0) {}
    PartialRequestSet(Tucuxi::GuiCore::AbstractRepository *repository, QObject* parent, const PartialRequestSet* &other);
};
Q_DECLARE_METATYPE(PartialRequestSet*)

typedef PartialRequest* SharedPartialRequest;
typedef PartialRequestSet* SharedPartialRequestSet;

#endif // PARTIALREQUEST_H
