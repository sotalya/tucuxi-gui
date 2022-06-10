//@@license@@

#ifndef LOCALDB_H
#define LOCALDB_H

#include <QSqlDatabase>

#include "genericdatabase.h"
#include "repository.h"
#include "pluginfactory.h"
#include "genericuser.h"

#include <QObject>
#include <QMetaProperty>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Tucuxi {


class GenericRepositoryFactory : public Factory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "localdb")
    Q_INTERFACES(Tucuxi::Factory)
public:
    void init(Core *core);

    PluginBase* newPlugin (QString& id);

    DescriptorBase* descriptor ();

    PluginBase::PluginType ptype() {
        return PluginBase::PluginType::database;
    }

    int priority(const QString &id) const;

private:
    //Our ID is : ch.heig-vd.ezechiel.bayesian
    DescriptorBase desc;    
};



class GenericRepository : public Repository
{
    Q_OBJECT

protected:
    Database *database;
    User *currentUser;

public:
    explicit GenericRepository(QObject *parent = 0);

    //! \brief Get the plugin's name
    /*! DBPlugin will return "default" as name, nothing for the description,
       and ch.heig-vd.ezechiel.database.default for ID.
      */
    //virtual Descriptor getDescriptor ();
    virtual QString databaseName() const;

    //! \brief Initialise the plugin
    /*! The default behavior of this function is to set the Core instance in Core.
       Based on that, a plugin might or might not want to reimplement the function.
      */
    virtual bool init(Core *core);

    //! \brief Error string
    /*! Will always responds that DBPlugin cannot connect or disconnect as it is a dead-end.
      */
    virtual QString errorString ();

    //! \brief Connect to the backend.
    /*! Simply returns false.
      */
    virtual bool connect (DBAccount account);

    //! \brief Disconnect to the backend.
    /*! Simply returns false.
      */
    virtual bool disconnect ();

    //! \brief Returns the database needs, in this case, nothing.
    virtual void needs (bool &url, bool &user, bool &password);


    // The following methods represent a subset of the old database interface
    // I think some things are missing, for sure, but that presents the idea

    virtual Response save(Patient *patient);
    virtual Response save(Practician *practician);
    virtual Response save(Institute *institute);
    virtual Response save(Drug *drug);
    virtual Response save(MedicalTreatment *treatment);
    virtual Response save(TreatmentAnalysis *analysis);
    virtual Response save(ReportData *report);

    virtual Response deleteObject(Aggregate *aggregate);

    // We can us pointers to pointers so as to let the repository allocate the object
    virtual Response getPatientById(int id, Patient **patient);
    virtual Response getPracticianById(int id, Practician **practician);
    virtual Response getInstituteById(int id, Institute **institute);
    virtual Response getDrugById(int id, Drug **drug);
    virtual Response getMedicalTreatmentById(int id, MedicalTreatment **treatment);
    virtual Response getTreatmentAnalysisById(int id, TreatmentAnalysis **analysis);
    virtual Response getReportById(int id, ReportData **report);
    // returns a list of patients (not all their information, but only first name and last name
    virtual Response getPatientList(QString from,QString to, QList<ShortPatient> *list);

signals:

public slots:

};

}

#endif // LOCALDB_H
