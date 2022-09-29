#ifndef MEASURE_H
#define MEASURE_H

#include <QObject>

#include "core/dal/amount.h"
#include "core/dal/entity.h"
#include "core/dal/corepatient.h"
#include "core/dal/coremeasure.h"
#include "core/dal/amount.h"
#include "core/interfaces/abstractrepository.h"
#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Drug;

class Measure : public Tucuxi::Gui::Core::CoreMeasure
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Measure)
    AUTO_PROPERTY(QString, sampleID)
    AUTO_PROPERTY(QDateTime, arrivalDate)

public:
    Q_INVOKABLE explicit Measure(Tucuxi::Gui::Core::AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Measure(Tucuxi::Gui::Core::AbstractRepository *repository, QDateTime moment, Tucuxi::Gui::Core::IdentifiableAmount* amount, QString getSdrug, int dbid = -1, QString sampleID = QString(), QDateTime arrivalDate = QDateTime(), bool enable = true);
};

//class MeasureList: public Tucuxi::Gui::Core::Entity {
//    Q_OBJECT
//    ADMIN_ENTITY_UTILS(MeasureList)
//    Q_PROPERTY(QList<QObject*> objlist READ getObjList NOTIFY objListChanged)
//public:
//    MeasureList(Tucuxi::Gui::Core::AbstractRepository* repository, QObject* parent = 0);
//    Q_INVOKABLE void append(Measure* target);
//    Q_INVOKABLE void add();
//    Q_INVOKABLE void remove(const int);
//    Q_INVOKABLE void clear();
//    Q_INVOKABLE int size() const;
//    Q_INVOKABLE bool isEmpty() const;
//    Q_INVOKABLE bool isValid();
//    Q_INVOKABLE Measure *first();
//    Q_INVOKABLE Measure *last();
//    Q_INVOKABLE QObject* objat(const int);
//    Q_INVOKABLE Measure* at(const int);
//    Q_INVOKABLE QList<Measure*>& getList();
//    Q_INVOKABLE QList<QObject*>& getObjList();
//signals:
//    Q_SIGNAL void objListChanged (QList<QObject*>& value);
//private:
//    QList<Measure*> _list;
//};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Measure*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Measure*>)


#endif // MEASURE_H
