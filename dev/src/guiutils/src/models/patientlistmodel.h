//@@license@@

#ifndef PATIENTLISTMODEL_H
#define PATIENTLISTMODEL_H

#include "abstractentitylistmodel.h"
#include "admin/src/dal/patient.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class PatientListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum PatientRoles {
        ExternalIdRole,
        StayNumberRole,
        StatOkRole,
        FirstNameRole,
        LastNameRole,
        BirthDateRole,
        GenderRole,
        AddressRole,
        PostcodeRole,
        CityRole,
        StateRole,
        CountryRole
    };
    Q_ENUM(PatientRoles)

    explicit PatientListModel(QObject *parent = nullptr);
    explicit PatientListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~PatientListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<SharedPatient> *patients);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities = Tucuxi::Gui::Core::SharedEntitySet());
    virtual Tucuxi::Gui::Core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<SharedPatient> *_patients;
    QHash<int, QByteArray> _roleNames;
};

}
}
}

#endif // PATIENTLISTMODEL_H
