//@@license@@

#ifndef DOSAGE_H
#define DOSAGE_H

#include <QJsonObject>

#include "core/dal/drug/operableamount.h"
#include "core/dal/entity.h"
#include "core/dal/drug/adme.h"
#include "core/utils/duration.h"
#include "core/utils/ezutils.h"

Q_MOC_INCLUDE("core/dal/uncastedvalue.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class UncastedValueList;

class AbstractDosage
{

public:
    ~AbstractDosage() {}
};


/** YTA: This class could allow to create dosage histories like
 * Every day of the week except on the week-end
 * */
class DosageList : public Entity
{
    Q_OBJECT

public:

protected:
    QList<AbstractDosage*> list;
};

class Dosage : public Entity, AbstractDosage
{
    Q_OBJECT

    ENTITY_UTILS(Dosage)

    Q_PROPERTY(double dbinterval READ getDbinterval WRITE setDbinterval NOTIFY dbintervalChanged)
    Q_PROPERTY(double dbtinf READ getDbtinf WRITE setDbtinf NOTIFY dbtinfChanged)
    Q_PROPERTY(Admin* route READ getRoute WRITE setRoute NOTIFY routeChanged)
    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(double, stepQuantity, StepQuantity)
    AUTO_PROPERTY_DECL(QDateTime, applied, Applied)
    AUTO_PROPERTY_DECL(QDateTime, endtime, EndTime)
    AUTO_PROPERTY_DECL(QDateTime, steadyStateLastDoseDate, SteadyStateLastDoseDate)
    AUTO_PROPERTY_DECL(Duration, tinf, Tinf)
    AUTO_PROPERTY_DECL(Duration, interval, Interval)
    AUTO_PROPERTY_DECL(bool, hasEndDate, HasEndDate)
    AUTO_PROPERTY_DECL(bool, isAtSteadyState, IsAtSteadyState)
    AUTO_PROPERTY_DECL(UncastedValueList*, uncastedValues, UncastedValues)

public:

    Q_INVOKABLE Dosage(AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Dosage(AbstractRepository *repository, QObject * parent, Dosage*& other);

public:

    Admin* getRoute() const;
    void setRoute(Admin* route);

    double getDbinterval() const;
    void setDbinterval(double dbinterval);

    double getDbtinf() const;
    void setDbtinf(double dbtinf);

    //! Convert the dosage unit
    void convert(const Unit &unit);
    //! Check if the dosage is valid
//    bool isValid();
    //! Check on all the sub-items.
    bool operator== ( Dosage* & other );
    //! Re-use the == operator.
    bool operator!= ( Dosage* & other );

    QString toString();

    bool getNextIntake(const QDateTime& fromTime, QDateTime &nextTime);

signals:
    void routeChanged(Admin*);
    void intervalChanged(double);
    void dbintervalChanged(double);
    void dbtinfChanged(double);
private:
    Admin* _route;
};

class AdjustmentDosage : public Dosage
{
    Q_OBJECT
    ENTITY_UTILS(AdjustmentDosage)
    AUTO_PROPERTY_DECL(QString, type, Type)
public:
    Q_INVOKABLE AdjustmentDosage(AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE AdjustmentDosage(AbstractRepository *repository, QObject * parent, Dosage*& other);
};

class DosageHistory : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(DosageHistory)
    typedef bool (*comparator_t)(const Dosage* , const Dosage*);
    Q_PROPERTY(QList<QObject*> objlist READ getObjList NOTIFY objListChanged)
public :
    DosageHistory(AbstractRepository *repository = nullptr, QObject* parent = nullptr);
    DosageHistory(AbstractRepository *repository, QObject* parent, const DosageHistory* &other);
    Q_INVOKABLE QList<Dosage*>& getList();
    void setList(QList<Dosage*>);
    Q_INVOKABLE Dosage* at(int i) const;
    Q_INVOKABLE void add();
    Q_INVOKABLE void update();
    Q_INVOKABLE void remove(const int);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QObject* objat(int i) const;
    Q_INVOKABLE Dosage* first() const;
    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE;
    Q_INVOKABLE Dosage* last() const;
    Q_INVOKABLE void append(Dosage* target);
    Q_INVOKABLE void sort(comparator_t cmpFunc) {
        std::sort(_list.begin(), _list.end(), cmpFunc);
        emit objListChanged(&getObjList());
    }
    Q_INVOKABLE int size() const;
    Q_INVOKABLE QList<QObject*>& getObjList() { return *reinterpret_cast<QList<QObject*>*>(&_list); }
    QDateTime firsttake() {
        if (!getList().empty()) {
            return getList().first()->getApplied();
        } else {
            return QDateTime();
        }
    }

    Q_INVOKABLE bool isEmpty() const;
    void setFirsttake(QDateTime firsttake) {_firsttake = firsttake;}

    QDateTime endTime();

    DosageHistory* clone();

    /** This function returns the dosage applied at the given time, or an invalid dosage
     * if there is none. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The dosage applied at the given time.
     */
    Dosage* dosage(const Duration &duration) ;

    //! Returns the dosage applied at the given date.
    /** This function returns the dosage applied at the given date, or an invalid dosage
     * if there is none.
     * @param duration The date.
     * @return The dosage applied at the given date.
     */
    Dosage* dosage(const QDateTime &date) ;

    //! Returns the last dosage.
    /** Returns the last dosage, or an invalid dosage if there is none.
     * @return The last dosage.
     */
    Dosage* lastDosage() const;

    //! Returns the next intake time after the given time
    bool getNextIntake(const QDateTime& fromTime, QDateTime &nextTime);

signals:
    void objListChanged (QList<QObject*>* objList);

private:
    QList<Dosage*> _list;
    QDateTime _firsttake;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Dosage*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Dosage*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DosageHistory*)
//Q_DECLARE_METATYPE(QList<QObject*>)

#endif // DOSAGE_H
