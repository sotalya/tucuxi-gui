//@@license@@

#ifndef BSV_H
#define BSV_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace GuiCore {


class Bsv : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Bsv)

    //! The parameter's value standard variability
    AUTO_PROPERTY_DECL(double, standard, Standard)
    //! The parameter's value proportional variability
    AUTO_PROPERTY_DECL(double, proportional, Proportional)

protected:

    Q_INVOKABLE Bsv(AbstractRepository *repository, QObject *parent = nullptr) :
      Entity(repository, parent), _standard(0), _proportional(0) {}
    Q_INVOKABLE Bsv(AbstractRepository *repository, QObject * parent, double standard, double proportional)
        : Entity(repository, parent), _standard(standard), _proportional(proportional) {}
};

} // namespace core
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::GuiCore::Bsv*)
Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::Bsv*>)

#endif // BSV_H
