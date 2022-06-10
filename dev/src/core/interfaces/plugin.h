//@@license@@

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
//#include "curvemanager.h"
#include "core/utils/version.h"
#include "core/utils/ezutils.h"
//#include "core/interfaces/optionable.h"

namespace Tucuxi {
namespace GuiCore {



//class DescriptorBase {
//public:
//    DescriptorBase() {}
//    virtual ~DescriptorBase() {}
//    DescriptorBase(const DescriptorBase& db) {
//        this->id = db.id;
//        this->name = db.name;
//        this->description = db.description;
//    }

//    DescriptorBase(QString id, QString name, QString description);
//    //! A name
//    QString name;
//    //! A description corresponding to the name
//    QString description;
//    //! An ID
//    QString id;

//    //! Return a string representation of this object
//    QString toString ();

//    QString getName () const;
//    QString getDescription () const;

//    virtual bool operator ==(const DescriptorBase &other);

//    //! Returns true if the ID, name and descriptions are not empty.
//    bool isValid() const;


//};

//template <typename T>
//class Descriptor : public DescriptorBase {
//    Q_DECLARE_TR_FUNCTIONS(Descriptor)
//public:
//    typedef T type;

//    Descriptor() {}
//    virtual ~Descriptor() {}
//    Descriptor(DescriptorBase db) : DescriptorBase(db) {}

//    Descriptor(QString id, QString name, QString description) : DescriptorBase(id, name, description) {}

//    template <typename C>
//    bool compareType() {
//        return std::is_same<C, type>();
//    }

//    // Compare two descriptor.
//    // Only the non-empty fields are compared.
//    // If all the fields are empty in one but not on both, returns false.

////    bool operator== (const Descriptor& other);
//};

//template <>
//class Descriptor<class Drug> : public DescriptorBase {
//public:
//    typedef Drug type;

//    Descriptor() {}
//    virtual ~Descriptor() {}
//    Descriptor(DescriptorBase db) : DescriptorBase(db) {}

//    Descriptor(QString id, QString name, QString description) : DescriptorBase(id, name, description) {}

//    template <typename C>
//    bool compareType() {
//        return std::is_same<C, type>();
//    }

//    Descriptor(const QString &drugId) {
//        id = drugId;
//    }

//    QString file() const {
//        return _filePath;
//    }

//    void setFile(const QString &filePath) {
//        _filePath = filePath;
//    }

//    QString modelId() const {
//        return _modelId;
//    }

//    void setModelId(const QString &modelId) {
//        _modelId = modelId;
//    }

//    QString atc() const {
//        return _atc;
//    }

//    void setAtc(const QString &atc) {
//        _atc = atc;
//    }

//    QString domainName() const {
//        return _domain;
//    }

//    void setDomainName(const QString &domainName) {
//        _domain = domainName;
//    }

//    QString studyName() const {
//        return _study;
//    }

//    void setStudyName(const QString &studyName) {
//        _study = studyName;
//    }

//private:
//    QString _filePath;
//    QString _modelId;
//    QString _atc;
//    QString _domain;
//    QString _study;
//};



class PluginBase : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(PluginType plugintype READ plugintype WRITE setPlugintype NOTIFY plugintypeChanged)
public:    

    enum PluginType {
        model, postengine, percentileengine, reverseengine, database
    };
    Q_ENUM(PluginType)

    void setId(int id){ m_id = id; emit idChanged(id);}
    int id() const{ return m_id; }

    QString version() {return m_version.toString();}
    void setVersion(const QString &version) {m_version.fromString(version);}

    void setName(QString name){m_name = name; emit nameChanged(name);}
    QString name() const{ return m_name; }

    void setPlugintype(PluginType plugintype){m_plugintype = plugintype; emit plugintypeChanged(plugintype);}
    PluginType plugintype() const{ return m_plugintype; }

//    virtual int priority (const QString&) const = 0;

    virtual void visit(class CurveManager&) = 0;
//    virtual Version version () = 0;

//    virtual QString errorString () = 0;

//    virtual void setCurveManager (class CurveManager *model) = 0;

//    virtual class CurveManager *CurveManager () = 0;
signals:
    void idChanged(int);
    void versionChanged(Version);
    void nameChanged(QString);
    void plugintypeChanged(PluginType);

private:
    int m_id;
    Version m_version;
    QString m_name;
    PluginType m_plugintype;

};

template <typename T>
PluginBase::PluginType pluginTypeLookup() {
    class Model;
    class PostEngine;
    class PercentileEngine;
    class ReverseEngine;
    class Database;
    if (std::is_same<T, Model>()) {
        return PluginBase::PluginType::model;
    }
    if (std::is_same<T, PostEngine>()) {
        return PluginBase::PluginType::postengine;
    }
    if (std::is_same<T, PercentileEngine>()) {
        return PluginBase::PluginType::percentileengine;
    }
    if (std::is_same<T, ReverseEngine>()) {
        return PluginBase::PluginType::reverseengine;
    }
    if (std::is_same<T, Database>()) {
        return PluginBase::PluginType::database;
    }
}

template<class T>
class Plugin : public PluginBase//, public Optionable
{    

public:

    virtual ~Plugin () {}

//    virtual T* clone () = 0;

    virtual Descriptor descriptor () = 0;

};

} // namespace core
} // namespace Tucuxi

#endif // PLUGIN_H
