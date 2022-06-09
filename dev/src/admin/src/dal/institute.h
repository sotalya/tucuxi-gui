#ifndef INSTITUTE_H
#define INSTITUTE_H

#include "core/dal/entity.h"
#include "location.h"
#include "phone.h"
#include "email.h"

#include <QImage>

class Institute : public ezechiel::GuiCore::Entity
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QImage, logo)
    AUTO_PROPERTY(Location*, location)
    AUTO_PROPERTY(int, location_id)

public:
    Q_INVOKABLE Institute(ezechiel::GuiCore::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Institute(ezechiel::GuiCore::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    QList<SharedPhone> phones() const;
    void setPhones(const QList<SharedPhone> &phones);

    QList<Email*> emails() const;
    void setEmails(const QList<Email*> &emails);

    SharedPhone phone(const PhoneType &type) const;
    Email* email(const Type &type) const;

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

private:
    QList<SharedPhone> _phones;
    QList<Email*> _emails;
};
Q_DECLARE_METATYPE(Institute*)
Q_DECLARE_METATYPE(QList<Institute*>)

class InstituteSet : public QList<Institute*>
{
public :
    InstituteSet(ezechiel::GuiCore::AbstractRepository *repository, QObject* parent = 0) {}
    InstituteSet(ezechiel::GuiCore::AbstractRepository *repository, QObject* parent, const InstituteSet* &other);
};
Q_DECLARE_METATYPE(InstituteSet*)

typedef Institute* SharedInstitute;
typedef InstituteSet* SharedInstituteSet;

#endif // INSTITUTE_H
