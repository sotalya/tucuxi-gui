/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef GENERICDBOBJECTS_H
#define GENERICDBOBJECTS_H

#include <QCoreApplication>
#include <QString>
#include <QDate>
#include <QList>
#include <QVariant>
#include <QMetaType>
#include <QObject>
#include <qdebug.h>
#include <QScriptEngine>
//#include "dbobjects/unit.h"
//#include "dbobjects/dbobjects.h
//#include "dbobjects/dbobjects.h
#include "plugin.h"

namespace Tucuxi {


//enum ParameterType {
//    //! Population or default parameters.
//    POPULATION,
//    //! Parameters adapted to the patient using its covariates.
//    APRIORI,
//    //! Parameters adapted to the patient using its measures.
//    APOSTERIORI,
//    //! Current parameters, entered or modified by the user.
//    MANUAL,
//    //! Temporary parameters, can be used for calculation, but should not be read.
//    TMP,
//    //! Last one, not used as a parameter type, have to be the last declared.
//    LAST_PT
//};



//class ErrorModel : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY errormodelPKNotify)
//    Q_PROPERTY(double additive READ additive WRITE setAdditive NOTIFY additiveChanged)
//    Q_PROPERTY(double proportional READ proportional WRITE setProportional NOTIFY proportionalChanged)
//    Q_PROPERTY(QString formula READ formula WRITE setFormula NOTIFY formulaChanged)

//    //foreign keys
////    Q_PROPERTY(QObject* drug READ drug WRITE setDrug NOTIFY drugChanged)
////    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)

//public:
//    Q_INVOKABLE ErrorModel() { setId(-1); }

//    Q_INVOKABLE ErrorModel(QObject* parent) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit errormodelPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setAdditive(double additive){ m_additive = additive; emit additiveChanged(additive);}
//    double additive() const{ return m_additive; }

//    void setProportional(double proportional){ m_proportional = proportional; emit proportionalChanged(proportional);}
//    double proportional() const{ return m_proportional; }

//    void setFormula(QString formula){ m_formula = formula; emit formulaChanged(formula);}
//    QString formula() const{ return m_formula; }

//    //foreign key relations
////    void setDrug_id(int drug_id){ m_id = drug_id; emit drugFKNotify(drug_id, this);}
////    int drug_id() const{ return m_drug_id; }

////    void setDrug(QObject* drug){ m_drug = drug; emit drugChanged(drug);}
////    QObject* drug() const{ return m_drug; }

//signals:
//    void errormodelPKNotify(int, ErrorModel*);
//    void additiveChanged(double);
//    void proportionalChanged(double);
//    void formulaChanged(QString);

//    //foreign keys
////    void drugFKNotify(int, ErrorModel*);
////    void drugChanged(QObject*);

//private:
//    int m_id;
//    double m_additive;
//    double m_proportional;
//    QString m_formula;

//    //foregin key relations
////    int m_drug_id;
////    QObject* m_drug;
//};


//class Drug : public QObject
//{
////    Q_DECLARE_TR_FUNCTIONS(Drug)
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY drugPKNotify)
//    Q_PROPERTY(float version READ version WRITE setVersion NOTIFY versionChanged)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
////    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
//    Q_PROPERTY(QString atc READ atc WRITE setAtc NOTIFY atcChanged)
//    Q_PROPERTY(QString modelname READ modelname WRITE setModelname NOTIFY modelnameChanged)
//    Q_PROPERTY(QString domain READ domain WRITE setDomain NOTIFY domainChanged)
//    Q_PROPERTY(QString study READ study WRITE setStudy NOTIFY studyChanged)
////    Q_PROPERTY(double molarmass READ molarmass WRITE setMolarmass NOTIFY molarmassChanged)

//    //foreign keys
//    Q_PROPERTY(int errormodel_id READ errormodel_id WRITE setErrormodel_id NOTIFY errormodelFKNotify)
//    Q_PROPERTY(ErrorModel* errormodel READ errormodel WRITE setErrormodel NOTIFY errormodelChanged)

//public:

//    Q_INVOKABLE Drug() : m_id(-1), m_version(1.0)
//      , m_name(""), m_atc(""), m_modelname(""), m_domain(""), m_study("")
//      , m_errormodel(0), m_errormodel_id(-1) { }

//    Q_INVOKABLE Drug(QObject* parent) : m_id(-1), m_version(1.0)
//      , m_name(""), m_atc(""), m_modelname(""), m_domain(""), m_study("")
//      , m_errormodel(0), m_errormodel_id(-1) { this->setParent(parent); setId(-1); }

//    Q_INVOKABLE Drug(ErrorModel& errormodel) : m_id(-1), m_version(1.0)
//      , m_name(""), m_atc(""), m_modelname(""), m_domain(""), m_study("")
//      , m_errormodel(0), m_errormodel_id(-1) { }

//    void setId(int id){ m_id = id; emit drugPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setVersion(float version){m_version = version; emit versionChanged(version);}
//    float version() const{ return m_version; }

//    void setName(QString name){m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

////    void setDescription(QString description){m_description = description; emit descriptionChanged(description);}
////    QString description() const{ return m_description; }

//    void setAtc(QString atc){m_atc = atc; emit atcChanged(atc);}
//    QString atc() const{ return m_atc; }

//    void setModelname(QString modelname){m_modelname = modelname; emit modelnameChanged(modelname);}
//    QString modelname() const{ return m_modelname; }

//    void setDomain(QString domain){m_domain = domain; emit domainChanged(domain);}
//    QString domain() const{ return m_domain; }

//    void setStudy(QString study){m_study = study; emit studyChanged(study);}
//    QString study() const{ return m_study; }

////    void setMolarmass(double molarmass){m_molarmass = molarmass; emit molarmassChanged(molarmass);}
////    double molarmass() const{ return m_molarmass; }

//    //foreign keys
//    void setErrormodel_id(int errormodel_id){ m_errormodel_id = errormodel_id; emit errormodelFKNotify(errormodel_id, this);}
//    int errormodel_id() const{ return m_errormodel_id; }

//    void setErrormodel(ErrorModel* errormodel){m_errormodel = errormodel; emit errormodelChanged(errormodel);}
//    ErrorModel* errormodel() const{ return m_errormodel; }

//signals:
//    void drugPKNotify(int, Drug*);
//    void versionChanged(float);
//    void nameChanged(QString);
////    void descriptionChanged(QString);
//    void atcChanged(QString);
//    void modelnameChanged(QString);
//    void domainChanged(QString);
//    void studyChanged(QString);
////    void molarmassChanged(double);

//    //foreign keys
//    void errormodelFKNotify(int, Drug*);
//    void errormodelChanged(ErrorModel*);

//private:

//    int m_id;
//    float m_version;
//    QString m_name;
////    QString m_description;
//    QString m_atc;
//    QString m_modelname;
//    QString m_domain;
//    QString m_study;
////    double m_molarmass;

//    //foreign keys
//    int m_errormodel_id;
//    ErrorModel* m_errormodel;
//};

////! \brief Define what a parameter is
///** \ingroup utils
//  A parameter is one of the possible options types given by the model's Model::getOptions() function.
//  A parameter have a intifier used exclusively in the model, a value and a variability related to this value.
//  The variability distribution type is used by the PercentileEngine in order to define the distribution in the variability.
//  \sa Distribution
//  */

//class Parameter : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
////    Q_PROPERTY(double additive READ additive WRITE setAdditive NOTIFY additiveChanged)
////    Q_PROPERTY(double proportional READ proportional WRITE setProportional NOTIFY proportionalChanged)
////    Q_PROPERTY(int distribution READ distribution WRITE setDistribution NOTIFY distributionChanged)
////    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
////    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)
////    Q_PROPERTY(double step READ step WRITE setStep NOTIFY stepChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

//public:
//    Q_INVOKABLE Parameter() { setId(-1); }

//    Q_INVOKABLE Parameter(QObject* parent) { setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit idChanged(id);}
//    int id() const{ return m_id; }

//    void setName(QString name){m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

////    void setDistribution(int distribution){ m_distribution = distribution; emit distributionChanged(distribution);}
////    int distribution() const{ return m_distribution; }

////    void setAdditive(double additive){m_additive = additive; emit additiveChanged(additive);}
////    double additive() const{ return m_additive; }

////    void setProportional(double proportional){m_proportional = proportional; emit proportionalChanged(proportional);}
////    double proportional() const{ return m_proportional; }

////    void setMin(double min){m_min = min; emit minChanged(min);}
////    double min() const{ return m_min; }

////    void setMax(double max){m_max = max; emit maxChanged(max);}
////    double max() const{ return m_max; }

////    void setStep(double step){m_step = step; emit stepChanged(step);}
////    double step() const{ return m_step; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }


//    //! To print.
//    QString toString () const;

//    //! Indicates if the parameter is variable or not
//    bool isVariable() const;

//    /** Returns the variance of the parameter
//     * @return the variance of the parameter
//     * Be careful with this function. If the standard deviation is
//     * different than 0, then the variability is considered as
//     * standard. If not, then the proportional variability is
//     * computed and returned.
//     */
//    double variance() const;

//    /** Returns the deviation of the parameter
//     * @return the deviation of the parameter
//     * Be careful with this function. If the standard deviation is
//     * different than 0, then the variability is considered as
//     * standard, and the standard deviation is returned. If not,
//     * then the proportional deviation is returned.
//     */
//    double deviation() const;

//signals:
//    void idChanged(int);
//    void nameChanged(QString);
////    void distributionChanged(int);
////    void additiveChanged(double);
////    void proportionalChanged(double);
////    void minChanged(double);
////    void maxChanged(double);
////    void stepChanged(double);
//    void unitChanged(QString);

//private:
//    int m_id;
//    QString m_name;
//    //! The parameter's value standard variability.
////    double m_additive;
//    //! The parameter's value proportional variability.
////    double m_proportional;
//    //! The parameter's variability distribution type.
////    int m_distribution;
////    //! Minimal value of this parameter.
////    double m_min;
////    //! Maximal value of this parameter.
////    double m_max;
////    //! Step between each value (for the UI).
////    double m_step;
//    //! Units of this parameter (handled through the units.h tools).
////    Unit unit;
//    QString m_unit;
//};

//class DrugParameter : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY drugParameterPKNotify)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

//    //foreign key
//    Q_PROPERTY(int parameter_id READ parameter_id WRITE setParameter_id NOTIFY parameterFKNotify)
//    Q_PROPERTY(Parameter* parameter READ parameter WRITE setParameter NOTIFY parameterChanged)
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//    Q_PROPERTY(int errormodel_id READ errormodel_id WRITE setErrormodel_id NOTIFY errormodelFKNotify)
//    Q_PROPERTY(ErrorModel* errormodel READ errormodel WRITE setErrormodel NOTIFY errormodelChanged)

//public:
//    Q_INVOKABLE DrugParameter() : m_drug(0), m_errormodel(0), m_parameter(0) { setId(-1); }

//    Q_INVOKABLE DrugParameter(QObject* parent) : m_drug(0), m_errormodel(0), m_parameter(0) { setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit drugParameterPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setValue(double value){m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//    //foreign key
//    void setParameter_id(int parameter_id){ m_parameter_id = parameter_id; emit parameterFKNotify(parameter_id, this);}
//    int parameter_id() const{ return m_parameter_id; }

//    void setParameter(Parameter* parameter){ m_parameter = parameter; emit parameterChanged(parameter);}
//    Parameter* parameter() const{ return m_parameter; }

//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//    void setErrormodel_id(int errormodel_id){ m_errormodel_id = errormodel_id; emit errormodelFKNotify(errormodel_id, this);}
//    int errormodel_id() const{ return m_errormodel_id; }

//    void setErrormodel(ErrorModel* errormodel){ m_errormodel = errormodel; emit errormodelChanged(errormodel);}
//    ErrorModel* errormodel() const{ return m_errormodel; }

//signals:
//    void drugParameterPKNotify(int, DrugParameter*);
//    void valueChanged(double);

//    //foreign keys
//    void parameterFKNotify(int, DrugParameter*);
//    void parameterChanged(Parameter*);
//    void drugFKNotify(int, DrugParameter*);
//    void drugChanged(Drug*);
//    void errormodelFKNotify(int, DrugParameter*);
//    void errormodelChanged(ErrorModel*);

//private:
//    int m_id;
//    double m_value;

//    //foreign key
//    int m_parameter_id;
//    Parameter* m_parameter;
//    int m_drug_id;
//    Drug* m_drug;
//    int m_errormodel_id;
//    ErrorModel* m_errormodel;
//};

//class DrugVariate : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY drugVariatePKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
//    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
////    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)

//    //foreign key relations
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)


//public:
//    Q_INVOKABLE DrugVariate() : m_drug(0) { setId(-1); }

//    Q_INVOKABLE DrugVariate(QObject* parent) : m_drug(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit drugVariatePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setUnit(QString unit){ m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setValue(QVariant value){ m_value = value; emit valueChanged(value);}
//    QVariant value() const{ return m_value; }

////    void setDate(QDateTime date){ m_date = date; emit dateChanged(date);}
////    QDateTime date() const{ return m_date; }

//    //foreign key relations
//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

////    void operator= (double value);
////    bool operator== (const Covariate &) const;
////    bool operator!= (const Covariate &) const;

//signals:
//    void drugVariatePKNotify(int, DrugVariate*);
//    void nameChanged(QString);
//    void unitChanged(QString);
//    void valueChanged(QVariant);
////    void dateChanged(QDateTime);

//    //foreign key relations
//    void drugFKNotify(int, DrugVariate*);
//    void drugChanged(Drug*);

//private:
//    int m_id;
//    QString m_name;
//    QString m_unit;
//    QVariant m_value;
////    QDateTime m_date;

//    //foregin key relations
//    int m_drug_id;
//    Drug* m_drug;
//};

//class Location : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY locationPKNotify)
//    Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged)
//    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
//    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
//    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
//    Q_PROPERTY(QString postcode READ postcode WRITE setPostcode NOTIFY postcodeChanged)
//public:
//    Q_INVOKABLE Location() { setId(-1); }

//    Q_INVOKABLE Location(QString country, QString state, QString address, QString city, QString postcode)
//        : m_country(country), m_state(state), m_address(address), m_city(city), m_postcode(postcode)
//    { setId(-1); }

//    Q_INVOKABLE Location(QObject* parent) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit locationPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setCountry(QString country){m_country = country; emit countryChanged(country);}
//    QString country() const{ return m_country; }

//    void setState(QString state){m_state = state; emit stateChanged(state);}
//    QString state() const{ return m_state; }

//    void setAddress(QString address){m_address = address; emit addressChanged(address);}
//    QString address() const{ return m_address; }

//    void setCity(QString city){m_city = city; emit cityChanged(city);}
//    QString city() const{ return m_city; }

//    void setPostcode(QString postcode){m_postcode = postcode; emit postcodeChanged(postcode);}
//    QString postcode() const{ return m_postcode; }

//signals:
//    void locationPKNotify(int, Location*);
//    void countryChanged(QString);
//    void stateChanged(QString);
//    void addressChanged(QString);
//    void cityChanged(QString);
//    void postcodeChanged(QString);

//public slots:
////    void on_Person_locationFKNotify(int fk, QObject* sender) {
////        if (fk == id()) { sender->setLocation(this); }
////    }

//private:
//    int m_id;
//    QString m_country;
//    QString m_state;
//    QString m_address;
//    QString m_city;
//    QString m_postcode;
//};

//class Institute : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY institutePKNotify)
//    Q_PROPERTY(QString external_id READ externalId WRITE setExternalId NOTIFY externalIdChanged)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QVariant logo READ logo WRITE setLogo NOTIFY logoChanged)

//    //foreign key relations
//    Q_PROPERTY(int location_id READ location_id WRITE setLocation_id NOTIFY locationFKNotify)
//    Q_PROPERTY(Location* location READ location WRITE setLocation NOTIFY locationChanged)

//public:
//    Q_INVOKABLE Institute() : m_location(0) { setId(-1); }

//    Q_INVOKABLE Institute(QObject* parent) : m_location(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit institutePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setExternalId(QString externalId){ m_externalId = externalId; emit externalIdChanged(externalId);}
//    QString externalId() const{ return m_externalId; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setLogo(QVariant logo){ m_logo = logo; emit logoChanged(logo);}
//    QVariant logo() const{ return m_logo; }

//    //foreign key relations
//    void setLocation_id(int location_id){ m_location_id = location_id; emit locationFKNotify(location_id, this);}
//    int location_id() const{ return m_location_id; }

//    void setLocation(Location* location){ m_location = location; emit locationChanged(location);}
//    Location* location() const{ return m_location; }

//signals:
//    void institutePKNotify(int, Institute*);
//    void externalIdChanged(QString);
//    void nameChanged(QString);
//    void logoChanged(QVariant);

//    //foreign key relations
//    void locationChanged(Location*);
//    void locationFKNotify(int, Institute*);

//private:
//    int m_id;
//    int m_location_id;
//    Location* m_location;
//    QString m_externalId;
//    QString m_name;
//    QVariant m_logo;
//};


//class Patient : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY patientPKNotify)
//    Q_PROPERTY(QString external_id READ externalId WRITE setExternalId NOTIFY externalIdChanged)
//    Q_PROPERTY(QString stay_number READ stayNumber WRITE setStayNumber NOTIFY stayNumberChanged)
//    Q_PROPERTY(bool stat_ok READ stat_ok WRITE setStat_ok NOTIFY stat_okChanged)
//    Q_PROPERTY(GenderType gender READ gender WRITE setGender NOTIFY genderChanged)
//    Q_PROPERTY(QDate birthday READ birthday WRITE setBirthday NOTIFY birthdayChanged)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString firstname READ firstname WRITE setFirstname NOTIFY firstnameChanged)
//    Q_PROPERTY(QString number READ number WRITE setNumber NOTIFY numberChanged)
//    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
//    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
//    Q_ENUMS(GenderType)

//    //foreign key relations
//    Q_PROPERTY(int location_id READ location_id WRITE setLocation_id NOTIFY locationFKNotify)
//    Q_PROPERTY(Location* location READ location WRITE setLocation NOTIFY locationChanged)

//public:
//    enum GenderType { Male, Female };

//    Q_INVOKABLE Patient() : m_location(0) { setId(-1); }

//    Q_INVOKABLE Patient(GenderType gender, QDate birthday, QString name, QString firstname)
//        : m_gender(gender), m_birthday(birthday), m_name(name), m_firstname(firstname), m_location(0) { setId(-1); }

//    Q_INVOKABLE Patient(QObject* parent) : m_location(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit patientPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setExternalId(QString externalId){ m_externalId = externalId; emit externalIdChanged(externalId);}
//    QString externalId() const{ return m_externalId; }

//    void setStayNumber(QString stayNumber){ m_stayNumber = stayNumber; emit stayNumberChanged(stayNumber);}
//    QString stayNumber() const{ return m_stayNumber; }

//    void setStat_ok(bool stat_ok){ m_stat_ok = stat_ok; emit stat_okChanged(stat_ok);}
//    bool stat_ok() const{ return m_stat_ok; }

//    void setGender(GenderType gender){ m_gender = gender; emit genderChanged(gender);}
//    GenderType gender() const{ return m_gender; }

//    void setBirthday(QDate birthday){ m_birthday = birthday; emit birthdayChanged(birthday);}
//    QDate birthday() const{ return m_birthday; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setFirstname(QString firstname){ m_firstname = firstname; emit firstnameChanged(firstname);}
//    QString firstname() const{ return m_firstname; }

//    void setType(QString type){m_type = type; emit typeChanged(type);}
//    QString type() const{ return m_type; }

//    void setNumber(QString number){m_number = number; emit numberChanged(number);}
//    QString number() const{ return m_number; }

//    void setEmail(QString email){m_email = email; emit emailChanged(email);}
//    QString email() const{ return m_email; }

//    //foreign key
//    void setLocation_id(int location_id){ m_location_id = location_id; emit locationFKNotify(location_id, this);}
//    int location_id() const{ return m_location_id; }

//    void setLocation(Location* location){ m_location = location; emit locationChanged(location); }
//    Location* location() const{ return m_location; }

//signals:
//    void patientPKNotify(int, Patient*);
//    void externalIdChanged(QString);
//    void stayNumberChanged(QString);
//    void stat_okChanged(bool);
//    void genderChanged(GenderType);
//    void birthdayChanged(QDate);
//    void nameChanged(QString);
//    void firstnameChanged(QString);
//    void typeChanged(QString);
//    void numberChanged(QString);
//    void emailChanged(QString);

//    //foreign key relations
//    void locationChanged(Location*);
//    void locationFKNotify(int, Patient*);

//private:
//    int m_id;
//    QString m_externalId;
//    QString m_stayNumber;
//    bool m_stat_ok;
//    GenderType m_gender;
//    QDate m_birthday;
//    QString m_name;
//    QString m_firstname;
//    QString m_type;
//    QString m_number;
//    QString m_email;

//    //foreign key relations
//    int m_location_id;
//    Location* m_location;
//};


//class Practician : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY practicianPKNotify)
//    Q_PROPERTY(QString external_id READ externalId WRITE setExternalId NOTIFY externalIdChanged)
//    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
//    Q_PROPERTY(GenderType gender READ gender WRITE setGender NOTIFY genderChanged)
//    Q_PROPERTY(QDate birthday READ birthday WRITE setBirthday NOTIFY birthdayChanged)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString firstname READ firstname WRITE setFirstname NOTIFY firstnameChanged)
//    Q_PROPERTY(QString number READ number WRITE setNumber NOTIFY numberChanged)
//    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
//    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
//    Q_ENUMS(GenderType)
////    Q_PROPERTY(QList<QString> emails READ emails WRITE setEmails NOTIFY emailsChanged)
////    Q_PROPERTY(QList<Phone*> phones READ phones WRITE setPhones NOTIFY phonesChanged)

//    //foreign key relations
//    Q_PROPERTY(int location_id READ location_id WRITE setLocation_id NOTIFY locationFKNotify)
//    Q_PROPERTY(Location* location READ location WRITE setLocation NOTIFY locationChanged)
//    Q_PROPERTY(int institute_id READ institute_id WRITE setInstitute_id NOTIFY instituteFKNotify)
//    Q_PROPERTY(Institute* institute READ institute WRITE setInstitute NOTIFY instituteChanged)
//public:
//    enum GenderType { Male, Female };

//    Q_INVOKABLE Practician() : m_location(0), m_institute(0) { setId(-1); }

//    Q_INVOKABLE Practician(GenderType gender, QDate birthday, QString name, QString firstname)
//        : m_gender(gender), m_birthday(birthday), m_name(name), m_firstname(firstname), m_location(0), m_institute(0) { setId(-1); }

//    Q_INVOKABLE Practician(QObject* parent) : m_location(0), m_institute(0), m_id(-1) { this->setParent(parent);}

//    void setId(int id){ m_id = id; emit practicianPKNotify(id, this); }
//    int id() const{ return m_id; }

//    void setExternalId(QString externalId){ m_externalId = externalId; emit externalIdChanged(externalId);}
//    QString externalId() const{ return m_externalId; }

//    void setTitle(QString title){ m_title = title; emit titleChanged(title);}
//    QString title() const{ return m_title; }

//    void setGender(GenderType gender){ m_gender = gender; emit genderChanged(gender);}
//    GenderType gender() const{ return m_gender; }

//    void setBirthday(QDate birthday){ m_birthday = birthday; emit birthdayChanged(birthday);}
//    QDate birthday() const{ return m_birthday; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setFirstname(QString firstname){ m_firstname = firstname; emit firstnameChanged(firstname);}
//    QString firstname() const{ return m_firstname; }

//    void setType(QString type){m_type = type; emit typeChanged(type);}
//    QString type() const{ return m_type; }

//    void setNumber(QString number){m_number = number; emit numberChanged(number);}
//    QString number() const{ return m_number; }

//    void setEmail(QString email){m_email = email; emit emailChanged(email);}
//    QString email() const{ return m_email; }

//    //foreign key relations
//    void setInstitute_id(int institute_id){ m_institute_id = institute_id; emit instituteFKNotify(institute_id, this);}
//    int institute_id() const{ return m_institute_id; }

//    void setInstitute(Institute* institute){ m_institute = institute; emit instituteChanged(institute);}
//    Institute* institute() const{ return m_institute; }

//    void setLocation_id(int location_id){ m_location_id = location_id; emit locationFKNotify(location_id, this);}
//    int location_id() const{ return m_location_id; }

//    void setLocation(Location* location){ m_location = location; emit locationChanged(location); }
//    Location* location() const{ return m_location; }

//signals:
//    void practicianPKNotify(int, Practician*);
//    void externalIdChanged(QString);
//    void titleChanged(QString);
//    void genderChanged(GenderType);
//    void birthdayChanged(QDate);
//    void nameChanged(QString);
//    void firstnameChanged(QString);
//    void typeChanged(QString);
//    void numberChanged(QString);
//    void emailChanged(QString);

//    //foreign key relations
//    void locationChanged(Location*);
//    void locationFKNotify(int, Practician*);
//    void instituteFKNotify(int, Practician*);
//    void instituteChanged(Institute*);

//private:
//    int m_id;
//    QString m_externalId;
//    QString m_title;
//    GenderType m_gender;
//    QDate m_birthday;
//    QString m_name;
//    QString m_firstname;
//    QString m_type;
//    QString m_number;
//    QString m_email;

//    //foreign key relations
//    int m_location_id;
//    Location* m_location;
//    int m_institute_id;
//    Institute* m_institute;
//};


//class PatientVariate : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY patientVariatePKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

//    //foreign key relations
//    Q_PROPERTY(int patient_id READ patient_id WRITE setPatient_id NOTIFY patientFKNotify)
//    Q_PROPERTY(Patient* patient READ patient WRITE setPatient NOTIFY patientChanged)


//public:
//    Q_INVOKABLE PatientVariate() : m_patient(0) { setId(-1); }

//    Q_INVOKABLE PatientVariate(QObject* parent) : m_patient(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit patientVariatePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setUnit(QString unit){ m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    //foreign key relations
//    void setPatient_id(int patient_id){ m_patient_id = patient_id; emit patientFKNotify(patient_id, this);}
//    int patient_id() const{ return m_patient_id; }

//    void setPatient(Patient* patient){ m_patient = patient; emit patientChanged(patient);}
//    Patient* patient() const{ return m_patient; }

////    void operator= (double value);
////    bool operator== (const Covariate &) const;
////    bool operator!= (const Covariate &) const;

//signals:
//    void patientVariatePKNotify(int, PatientVariate*);
//    void nameChanged(QString);
//    void unitChanged(QString);

//    //foreign key relations
//    void patientFKNotify(int, PatientVariate*);
//    void patientChanged(Patient*);

//private:
//    int m_id;
//    QString m_name;
//    QString m_unit;

//    //foregin key relations
//    int m_patient_id;
//    Patient* m_patient;
//};


//class VariateMeasure : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY variateMeasurePKNotify)
//    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)

//    //foreign keys
//    Q_PROPERTY(int patientvariate_id READ patientvariate_id WRITE setPatientvariate_id NOTIFY patientvariateFKNotify)
//    Q_PROPERTY(PatientVariate* patientvariate READ patientvariate WRITE setPatientvariate NOTIFY patientvariateChanged)

//public:

//    Q_INVOKABLE VariateMeasure() : m_patientvariate(0) { setId(-1); }

//    Q_INVOKABLE VariateMeasure(QObject* parent) : m_patientvariate(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit variateMeasurePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setValue(QVariant value){ m_value = value; emit valueChanged(value);}
//    QVariant value() const{ return m_value; }

//    void setDate(QDateTime date){ m_date = date; emit dateChanged(date);}
//    QDateTime date() const{ return m_date; }

//    void setPatientvariate_id(int patientvariate_id){ m_patientvariate_id = patientvariate_id; emit patientvariateFKNotify(patientvariate_id, this);}
//    int patientvariate_id() const{ return m_patientvariate_id; }

//    void setPatientvariate(PatientVariate* patientvariate){ m_patientvariate = patientvariate; emit patientvariateChanged(patientvariate);}
//    PatientVariate* patientvariate() const{ return m_patientvariate; }


//signals:
//    void variateMeasurePKNotify(int, VariateSharedMeasure);
//    void valueChanged(QVariant);
//    void dateChanged(QDateTime);

//    //foreign keys
//    void patientvariateFKNotify(int, VariateSharedMeasure);
//    void patientvariateChanged(PatientVariate*);

//private:
//    int m_id;
//    QVariant m_value;
//    QDateTime m_date;

//    //foregin keys
//    int m_patientvariate_id;
//    PatientVariate *m_patientvariate;
//};

//class Clinicals : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY clinicalsPKNotify)
//    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
//    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)

//    //foreign key relations
//    Q_PROPERTY(int patient_id READ patient_id WRITE setPatient_id NOTIFY patientFKNotify)
//    Q_PROPERTY(Patient* patient READ patient WRITE setPatient NOTIFY patientChanged)
//public:

//    Q_INVOKABLE Clinicals() : m_value(""), m_type(""), m_date(QDateTime()), m_patient_id(-1), m_patient(0) { setId(-1); }

//    Q_INVOKABLE Clinicals(QObject* parent) : m_value(""), m_type(""), m_date(QDateTime()), m_patient_id(-1), m_patient(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit clinicalsPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setValue(QString value){ m_value = value; emit valueChanged(value);}
//    QString value() const{ return m_value; }

//    void setType(QString type){ m_type = type; emit typeChanged(type);}
//    QString type() const{ return m_type; }

//    void setDate(QDateTime date){ m_date = date; emit dateChanged(date);}
//    QDateTime date() const{ return m_date; }

//    void operator= (QString value);
//    bool operator== (const Clinicals &) const;
//    bool operator!= (const Clinicals &) const;

//    //forieng key relations

//    void setPatient_id(int patient_id){ m_patient_id = patient_id; emit patientFKNotify(patient_id, this);}
//    int patient_id() const{ return m_patient_id; }

//    void setPatient(Patient* patient){ m_patient = patient; emit patientChanged(patient);}
//    Patient* patient() const{ return m_patient; }

//signals:
//    void clinicalsPKNotify(int, Clinicals*);
//    void valueChanged(QString);
//    void typeChanged(QString);
//    void dateChanged(QDateTime);

//    //foreign key relations
//    void patientFKNotify(int, Clinicals*);
//    void patientChanged(Patient*);

//private:
//    int m_id;
//    QString m_name;
//    QString m_value;
//    QString m_type;
//    QDateTime m_date;

//    //foreign key relations
//    int m_patient_id;
//    Patient* m_patient;
//};

//class ComputeEngine : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY enginePKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(float version READ version WRITE setVersion NOTIFY versionChanged)
//    Q_PROPERTY(int plugintype READ plugintype WRITE setPlugintype NOTIFY plugintypeChanged)
//public:
//    Q_INVOKABLE ComputeEngine () : m_name(""), m_version(0.0), m_plugintype(0) { setId(-1); }
//    Q_INVOKABLE ComputeEngine(QObject* parent) { this->setParent(parent); setId(-1); }
//    void setId(int id){ m_id = id; emit enginePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setName(QString name){m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setVersion(float version){m_version = version; emit versionChanged(version);}
//    float version() const{ return m_version; }

//    void setPlugintype(int plugintype){m_plugintype = plugintype; emit plugintypeChanged(plugintype);}
//    int plugintype() const{ return m_plugintype; }

//signals:
//    void enginePKNotify(int, ComputeEngine*);
//    void nameChanged(QString);
//    void versionChanged(float);
//    void plugintypeChanged(int);

//private:
//    int m_id;
//    QString m_name;
//    float m_version;
//    int m_plugintype;
//};


//class Curve : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY curvePKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(int patient_id READ patient_id WRITE setPatient_id NOTIFY patientFKNotify)
//    Q_PROPERTY(int ptype READ pType WRITE setPType NOTIFY pTypeChanged)
//    Q_PROPERTY(int ctype READ cType WRITE setCType NOTIFY cTypeChanged)
//    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
//    Q_PROPERTY(QDateTime first_take READ firsttake WRITE setFirsttake NOTIFY firsttakeChanged)

//    //Foreign key relations (always a QOBject*)
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(int model_id READ model_id WRITE setModel_id NOTIFY modelFKNotify)
//    Q_PROPERTY(int percentileengine_id READ percentileengine_id WRITE setpercentileengine_id NOTIFY percentileengine_FKNotify)
//    Q_PROPERTY(int postengine_id READ postengine_id WRITE setpostengine_id NOTIFY postengine_FKNotify)
//    Q_PROPERTY(int reverseengine_id READ reverseengine_id WRITE setreverseengine_id NOTIFY reverseengine_FKNotify)
//    Q_PROPERTY(Patient* patient READ patient WRITE setPatient NOTIFY patientChanged)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//    Q_PROPERTY(ComputeEngine* model READ model WRITE setModel NOTIFY modelChanged)
//    Q_PROPERTY(ComputeEngine* percentileengine READ percentileengine WRITE setpercentileengine NOTIFY percentileengineChanged)
//    Q_PROPERTY(ComputeEngine* postengine READ postengine WRITE setpostengine NOTIFY postengineChanged)
//    Q_PROPERTY(ComputeEngine* reverseengine READ reverseengine WRITE setreverseengine NOTIFY reverseengineChanged)
//public:
//    Q_INVOKABLE Curve() : m_drug_id(-1), m_model_id(-1), m_percentileengine_id(-1), m_postengine_id(-1), m_reverseengine_id(-1),
//        m_patient(0), m_drug(0), m_model(0), m_percentileengine(0), m_postengine(0), m_reverseengine(0)
//    { setId(-1); /*QMetaObject::connectSlotsByName(this);*/ }

//    Q_INVOKABLE Curve(QObject* parent) : m_drug_id(-1), m_model_id(-1), m_percentileengine_id(-1), m_postengine_id(-1), m_reverseengine_id(-1),
//        m_patient(0), m_drug(0), m_model(0), m_percentileengine(0), m_postengine(0), m_reverseengine(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit curvePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setPType(int pType){ m_pType = pType; emit pTypeChanged(pType);}
//    int pType() const{ return m_pType; }

//    void setCType(int cType){ m_cType = cType; emit cTypeChanged(cType);}
//    int cType() const{ return m_cType; }

//    void setName(QString name){ m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setComment(QString comment){ m_comment = comment; emit commentChanged(comment);}
//    QString comment() const{ return m_comment; }

//    void setFirsttake(QDateTime firsttake){ m_firsttake = firsttake; emit firsttakeChanged(firsttake);}
//    QDateTime firsttake() const{ return m_firsttake; }

//    //foreign key raltions
//    void setPatient_id(int patient_id){ m_patient_id = patient_id; emit patientFKNotify(patient_id, this);}
//    int patient_id() const{ return m_patient_id; }

//    void setPatient(Patient* patient){ m_patient = patient; emit patientChanged(patient);}
//    Patient* patient() const{ return m_patient; }

//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//    void setModel_id(int model_id){ m_model_id = model_id; emit modelFKNotify(model_id, this);}
//    int model_id() const{ return m_model_id; }

//    void setModel (ComputeEngine* model ){ m_model  = model ; emit modelChanged(model);}
//    ComputeEngine* model () const{ return m_model ; }

//    void setpercentileengine_id(int percentileengine_id){ m_percentileengine_id = percentileengine_id; emit percentileengine_FKNotify(percentileengine_id, this);}
//    int percentileengine_id() const{ return m_percentileengine_id; }

//    void setpercentileengine (ComputeEngine* percentileengine ){ m_percentileengine  = percentileengine ; emit percentileengineChanged(percentileengine);}
//    ComputeEngine* percentileengine () const{ return m_percentileengine ; }

//    void setpostengine_id(int postengine_id){ m_postengine_id = postengine_id; emit postengine_FKNotify(postengine_id, this);}
//    int postengine_id() const{ return m_postengine_id; }

//    void setpostengine (ComputeEngine* postengine ){ m_postengine  = postengine ; emit postengineChanged(postengine);}
//    ComputeEngine* postengine () const{ return m_postengine ; }

//    void setreverseengine_id(int reverseengine_id){ m_reverseengine_id = reverseengine_id; emit reverseengine_FKNotify(reverseengine_id, this);}
//    int reverseengine_id() const{ return m_reverseengine_id; }

//    void setreverseengine (ComputeEngine* reverseengine ){ m_reverseengine  = reverseengine ; emit reverseengineChanged(reverseengine);}
//    ComputeEngine* reverseengine () const{ return m_reverseengine ; }

//signals:
//    void curvePKNotify(int, Curve*);
//    void pTypeChanged(int);
//    void cTypeChanged(int);
//    void nameChanged(QString);
//    void commentChanged(QString);
//    void firsttakeChanged(QDateTime);

//    //foreign key relations
//    void patientFKNotify(int, Curve*);
//    void patientChanged(Patient*);
//    void drugFKNotify(int, Curve*);
//    void drugChanged(Drug*);
//    void modelFKNotify(int, Curve*);
//    void modelChanged(ComputeEngine*);
//    void percentileengine_FKNotify(int, Curve*);
//    void percentileengineChanged(ComputeEngine*);
//    void postengine_FKNotify(int, Curve*);
//    void postengineChanged(ComputeEngine*);
//    void reverseengine_FKNotify(int, Curve*);
//    void reverseengineChanged(ComputeEngine*);

////public slots:
////    void on_Patient_patientPKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setPatient(sender); } else { if (m_patient == sender) { setPatient(0); } }
////    }
////    void on_Drug_drugPKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setDrug(sender); } else { if (m_drug == sender) { setDrug(0); } }
////    }
////    void on_Model_modelPKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setModel(sender); } else { if (m_model == sender) { setModel(0); } }
////    }
////    void on_PercentileEngine_percentileenginePKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setpercentileengine(sender); } else { if (m_percentileengine == sender) { m_percentileengine = 0; } }
////    }
////    void on_PostEngine_postenginePKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setpostengine(sender); } else { if (m_postengine == sender) { setpostengine(0); } }
////    }
////    void on_ReverseEngine_reverseenginePKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setreverseengine(sender); } else { if (m_reverseengine == sender) { setreverseengine(0); } }
////    }

//private:
//    int m_id;
//    int m_pType;
//    int m_cType;
//    QString m_name;
//    QString m_comment;
//    QDateTime m_firsttake;

//    //foreign key relations
//    int m_patient_id;
//    Patient* m_patient;
//    int m_drug_id;
//    Drug* m_drug;
//    int m_model_id;
//    ComputeEngine* m_model;
//    int m_percentileengine_id;
//    ComputeEngine* m_percentileengine;
//    int m_postengine_id;
//    ComputeEngine* m_postengine;
//    int m_reverseengine_id;
//    ComputeEngine* m_reverseengine;
//};


//class Dosage* : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY dosagePKNotify)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(double tinf READ tinf WRITE setTinf NOTIFY tinfChanged)
//    Q_PROPERTY(double interval READ interval WRITE setInterval NOTIFY intervalChanged)
//    Q_PROPERTY(QDateTime starttime READ starttime WRITE setStarttime NOTIFY starttimeChanged)
//    Q_PROPERTY(QDateTime endtime READ endtime WRITE setEndtime NOTIFY endtimeChanged)
//    Q_PROPERTY(int route READ route WRITE setRoute NOTIFY routeChanged)

//    //Foreign key relations
//    Q_PROPERTY(int curve_id READ curve_id WRITE setCurve_id NOTIFY curveFKNotify)
//    Q_PROPERTY(Curve* curve READ curve WRITE setCurve NOTIFY curveChanged)
//public:
//    Q_INVOKABLE Dosage() : m_unit(""), m_value(0), m_tinf(0), m_interval(0), m_curve(0), m_curve_id(-1), m_route(0) { setId(-1);}

//    Q_INVOKABLE Dosage(QObject* parent) : m_unit(""), m_value(0), m_tinf(0), m_interval(0), m_curve(0), m_curve_id(-1), m_route(0) { this->setParent(parent); setId(-1);}

//    void setId(int id){ m_id = id; emit dosagePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setValue(double value){ m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//    void setTinf(double tinf){ m_tinf = tinf; emit tinfChanged(tinf);}
//    double tinf() const{ return m_tinf; }

//    void setInterval(double interval){ m_interval = interval; emit intervalChanged(interval);}
//    double interval() const{ return m_interval; }

//    void setStarttime(QDateTime starttime){ m_starttime = starttime; emit starttimeChanged(starttime);}
//    QDateTime starttime() const{ return m_starttime; }

//    void setEndtime(QDateTime endtime){ m_endtime = endtime; emit endtimeChanged(endtime);}
//    QDateTime endtime() const{ return m_endtime; }

//    void setRoute(int route){ m_route = route; emit routeChanged(route);}
//    int route() const{ return m_route; }

//    //foreign key relations
//    void setCurve_id(int curve_id){ m_curve_id = curve_id; emit curveFKNotify(curve_id, this);}
//    int curve_id() const{ return m_curve_id; }

//    void setCurve(Curve* curve){ m_curve = curve; emit curveChanged(curve); }
//    Curve* curve() const{ return m_curve; }

//signals:
//    void dosagePKNotify(int, Dosage*);
//    void unitChanged(QString);
//    void valueChanged(double);
//    void tinfChanged(double);
//    void intervalChanged(double);
//    void starttimeChanged(QDateTime);
//    void endtimeChanged(QDateTime);
//    void routeChanged(int);

//    //foreign key relations
//    void curveFKNotify(int, Dosage*);
//    void curveChanged(Curve*);

////Foreign key relations (always a QOBject*)
////public slots:
////    void on_Curve_curvePKNotify(int fk, QObject* sender) {
////        if (fk == m_id) { setCurve(sender); } else { if (m_curve == sender) { setCurve(0); } }
////    }

//private:
//    int m_id;
//    QDateTime m_starttime;
//    QDateTime m_endtime;
//    double m_tinf;
//    double m_value;
//    QString m_unit;
//    double m_interval;
//    int m_route;

//    //foriegn key relations
//    int m_curve_id;
//    Curve* m_curve;
//};


//class Measure : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY measurePKNotify)
//    Q_PROPERTY(QDateTime sampletime READ sampletime WRITE setSampletime NOTIFY sampletimeChanged)
//    Q_PROPERTY(QDateTime arrivaltime READ arrivaltime WRITE setArrivaltime NOTIFY arrivaltimeChanged)
//    Q_PROPERTY(QString sampleID READ sampleID WRITE setSampleID NOTIFY sampleIDChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
//    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

//    //Foreign key relations
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//    Q_PROPERTY(int patient_id READ patient_id WRITE setPatient_id NOTIFY patientFKNotify)
//    Q_PROPERTY(Patient* patient READ patient WRITE setPatient NOTIFY patientChanged)

//public:
//    Q_INVOKABLE Measure() : m_patient(0), m_drug(0), m_value(0), m_id(-1), m_patient_id(-1), m_drug_id(-1) {}

//    Q_INVOKABLE Measure(QObject* parent) : m_patient(0), m_drug(0), m_patient_id(-1), m_drug_id(-1), m_id(-1) { this->setParent(parent); }

//    Q_INVOKABLE Measure(QDateTime sampledate, double value, Drug* drug, QString& sampleID, QDateTime& arrivaltime)
//    : m_sampletime(sampledate), m_id(-1), m_value(value), m_drug(drug), m_patient_id(-1), m_drug_id(-1), m_sampleID(sampleID), m_arrivaltime(arrivaltime){}

//    void setId(int id){ m_id = id; emit measurePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setSampleID(QString sampleID){m_sampleID = sampleID; emit sampleIDChanged(sampleID);}
//    QString sampleID() const{ return m_sampleID; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setComment(QString comment){m_comment = comment; emit commentChanged(comment);}
//    QString comment() const{ return m_comment; }

//    void setSampletime(QDateTime sampletime){m_sampletime = sampletime; emit sampletimeChanged(sampletime);}
//    QDateTime sampletime() const{ return m_sampletime; }

//    void setArrivaltime(QDateTime arrivaltime){m_arrivaltime = arrivaltime; emit arrivaltimeChanged(arrivaltime);}
//    QDateTime arrivaltime() const{ return m_arrivaltime; }

//    //foreign key relations
//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//    void setPatient_id(int patient_id){ m_patient_id = patient_id; emit patientFKNotify(patient_id, this);}
//    int patient_id() const{ return m_patient_id; }

//    void setPatient(Patient* patient){ m_patient = patient; emit patientChanged(patient);}
//    Patient* patient() const{ return m_patient; }

//    void setValue(double value){ m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//signals:
//    void measurePKNotify(int, SharedMeasure);
//    void sampleIDChanged(QString);
//    void unitChanged(QString);
//    void commentChanged(QString);
//    void sampletimeChanged(QDateTime);
//    void arrivaltimeChanged(QDateTime);
//    void drugFKNotify(int, SharedMeasure);
//    void drugChanged(Drug*);
//    void patientFKNotify(int, SharedMeasure);
//    void patientChanged(Patient*);
//    void valueChanged(double);

//private:
//    int m_id;
//    QDateTime m_sampletime;
//    QDateTime m_arrivaltime;
//    QString m_sampleID;
//    QString m_unit;
//    QString m_comment;
//    double m_value;

//    //foreign key relations
//    int m_drug_id;
//    Drug* m_drug;
//    int m_patient_id;
//    Patient* m_patient;

//};


//class Option : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY optionPKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
//public:
//    Q_INVOKABLE Option () { setId(-1); }
//    Q_INVOKABLE Option(QObject* parent) { this->setParent(parent); setId(-1); }
//    void setId(int id){ m_id = id; emit optionPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setName(QString name){m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setValue(QVariant value){m_value = value; emit valueChanged(value);}
//    QVariant value() const{ return m_value; }

//signals:
//    void optionPKNotify(int, Option*);
//    void nameChanged(QString);
//    void valueChanged(QVariant);

//private:
//    QVariant m_value;
//    QString m_name;
//    int m_id;
//};


//class CurveOption : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY curveOptionPKNotify)
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

//    //foreign key relations
//    Q_PROPERTY(int engine_id READ engine_id WRITE setEngine_id NOTIFY engineFKNotify)
//    Q_PROPERTY(ComputeEngine* engine READ engine WRITE setEngine NOTIFY engineChanged)
//    Q_PROPERTY(int option_id READ option_id WRITE setOption_id NOTIFY optionFKNotify)
//    Q_PROPERTY(Option* option READ option WRITE setOption NOTIFY optionChanged)
//    Q_PROPERTY(int curve_id READ curve_id WRITE setCurve_id NOTIFY curveFKNotify)
//    Q_PROPERTY(Curve* curve READ curve WRITE setCurve NOTIFY curveChanged)
//public:
//    Q_INVOKABLE CurveOption () : m_name(""), m_value(0), m_engine_id(-1), m_option_id(-1), m_curve_id(-1),
//        m_engine(0), m_curve(0), m_option(0) { setId(-1); }
//    Q_INVOKABLE CurveOption(QObject* parent) : m_engine(0), m_curve(0), m_option(0) { this->setParent(parent); setId(-1); }
//    void setId(int id){ m_id = id; emit curveOptionPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setName(QString name){m_name = name; emit nameChanged(name);}
//    QString name() const{ return m_name; }

//    void setValue(QVariant value){m_value = value; emit valueChanged(value);}
//    QVariant value() const{ return m_value; }

//    //foreign key relations
//    void setEngine_id(int engine_id){ m_engine_id = engine_id; emit engineFKNotify(engine_id, this);}
//    int engine_id() const{ return m_engine_id; }

//    void setEngine(ComputeEngine* engine){ m_engine = engine; emit engineChanged(engine);}
//    ComputeEngine* engine() const{ return m_engine; }

//    void setOption_id(int option_id){ m_option_id = option_id; emit optionFKNotify(option_id, this);}
//    int option_id() const{ return m_option_id; }

//    void setOption(Option* option){ m_option = option; emit optionChanged(option);}
//    Option* option() const{ return m_option; }

//    void setCurve_id(int curve_id){ m_curve_id = curve_id; emit curveFKNotify(curve_id, this);}
//    int curve_id() const{ return m_curve_id; }

//    void setCurve(Curve* curve){ m_curve = curve; emit curveChanged(curve);}
//    Curve* curve() const{ return m_curve; }

//signals:
//    void curveOptionPKNotify(int, CurveOption*);
//    void nameChanged(QString);
//    void valueChanged(QVariant);

//    //foreign key relations
//    void engineFKNotify(int, CurveOption*);
//    void optionFKNotify(int, CurveOption*);
//    void curveFKNotify(int, CurveOption*);
//    void engineChanged(ComputeEngine*);
//    void optionChanged(Option*);
//    void curveChanged(Curve*);

//private:
//    QVariant m_value;
//    QString m_name;
//    int m_id;

//    //foreign key relations
//    int m_engine_id;
//    ComputeEngine* m_engine;
//    int m_curve_id;
//    Curve* m_curve;
//    int m_option_id;
//    Option* m_option;
//};


//class Report : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
////    Q_PROPERTY(int recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)
////    Q_PROPERTY(QString templateName READ templateName WRITE setTemplateName NOTIFY templateNameChanged)
////    Q_PROPERTY(QString templateLang READ templateLang WRITE setTemplateLang NOTIFY templateLangChanged)
////    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
////    Q_PROPERTY(QString reference READ reference WRITE setReference NOTIFY referenceChanged)
////    Q_PROPERTY(QString comedic READ comedic WRITE setComedic NOTIFY comedicChanged)
////    Q_PROPERTY(QString adverse READ adverse WRITE setAdverse NOTIFY adverseChanged)
////    Q_PROPERTY(QString answer READ answer WRITE setAnswer NOTIFY answerChanged)
////    Q_PROPERTY(QString comments READ comments WRITE setComments NOTIFY commentsChanged)
////    Q_PROPERTY(QString interpretation READ interpretation WRITE setInterpretation NOTIFY interpretationChanged)
////    Q_PROPERTY(QString posology READ posology WRITE setPosology NOTIFY posologyChanged)
////    Q_PROPERTY(QString monitoring READ monitoring WRITE setMonitoring NOTIFY monitoringChanged)
////    Q_PROPERTY(QString references READ references WRITE setReferences NOTIFY referencesChanged)
////    Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged)
////    Q_PROPERTY(bool distributed READ distributed WRITE setDistributed NOTIFY distributedChanged)
////    Q_PROPERTY(QDateTime crtDate READ crtDate WRITE setCrtDate NOTIFY crtDateChanged)
////    Q_PROPERTY(QDateTime date READ refDate WRITE setRefDate NOTIFY refDateChanged)
//    Q_PROPERTY(int Prediction READ prediction WRITE setPrediction NOTIFY predictionChanged)
////    Q_PROPERTY(QList<sections> layout READ layout WRITE setLayout NOTIFY layoutChanged)
////    Q_PROPERTY(QList<int> copyAt READ copyAt WRITE setCopyAt NOTIFY copyAtChanged)

//    //Foreign key relations
//    Q_PROPERTY(int curve_id READ curve_id WRITE setCurve_id NOTIFY curveFKNotify)
//    Q_PROPERTY(Curve* curve READ curve WRITE setCurve NOTIFY curveChanged)
//    Q_PROPERTY(int analyst_id READ analyst_id WRITE setAnalyst_id NOTIFY analystFKNotify)
//    Q_PROPERTY(Practician* analyst READ analyst WRITE setAnalyst NOTIFY analystChanged)
//    Q_PROPERTY(int applicant_id READ applicant_id WRITE setApplicant_id NOTIFY applicantFKNotify)
//    Q_PROPERTY(Practician* applicant READ applicant WRITE setApplicant NOTIFY applicantChanged)

//public:
//    //! The report's sections enumeration
//    enum sections {
//        INTERPRETATION,
//        ANALYST,
//        APPLICANT,
//        CHART,
//        MONITORING,
//        LOGO,
//        MEASURES,
//        PARAMETERS,
//        COVARIATES,
//        PATIENT,
//        RECIPIENT,
//        REFERENCES,
//        GENERAL,
//        LAST_SECTION   //Only used to get the total number of values in this enum
//    };

//    //Constructors
//    Q_INVOKABLE Report() : m_curve(0), m_applicant(0), m_analyst(0) { setId(-1); }
//    Q_INVOKABLE Report(QObject* parent) : m_curve(0), m_applicant(0), m_analyst(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit idChanged(id);}
//    int id() const{ return m_id; }

//    void setPrediction(int prediction){ m_prediction = prediction; emit predictionChanged(prediction);}
//    int prediction() const{ return m_prediction; }

////    void setRecipient(int recipient){ m_recipient = recipient; emit recipientChanged(recipient);}
////    int recipient() const{ return m_recipient; }

////    void setTemplateName(QString templateName){ m_templateName = templateName; emit templateNameChanged(templateName);}
////    QString templateName() const{ return m_templateName; }

////    void setTemplateLang(QString templateLang){ m_templateLang = templateLang; emit templateLangChanged(templateLang);}
////    QString templateLang() const{ return m_templateLang; }

////    void setTitle(QString title){ m_title = title; emit titleChanged(title);}
////    QString title() const{ return m_title; }

////    void setReference(QString reference){ m_reference = reference; emit referenceChanged(reference);}
////    QString reference() const{ return m_reference; }

////    void setComedic(QString comedic){ m_comedic = comedic; emit comedicChanged(comedic);}
////    QString comedic() const{ return m_comedic; }

////    void setAdverse(QString adverse){ m_adverse = adverse; emit adverseChanged(adverse);}
////    QString adverse() const{ return m_adverse; }

////    void setAnswer(QString answer){ m_answer = answer; emit answerChanged(answer);}
////    QString answer() const{ return m_answer; }

////    void setComments(QString comments){ m_comments = comments; emit commentsChanged(comments);}
////    QString comments() const{ return m_comments; }

////    void setInterpretation(QString interpretation){ m_interpretation = interpretation; emit interpretationChanged(interpretation);}
////    QString interpretation() const{ return m_interpretation; }

////    void setPosology(QString posology){ m_posology = posology; emit posologyChanged(posology);}
////    QString posology() const{ return m_posology; }

////    void setMonitoring(QString monitoring){ m_monitoring = monitoring; emit monitoringChanged(monitoring);}
////    QString monitoring() const{ return m_monitoring; }

////    void setImage(QVariant image){ m_image = image; emit imageChanged(image);}
////    QVariant image() const{ return m_image; }

////    void setReferences(QString references){ m_references = references; emit referencesChanged(references);}
////    QString references() const{ return m_references; }

////    void setDistributed(bool distributed){ m_distributed = distributed; emit distributedChanged(distributed);}
////    bool distributed() const{ return m_distributed; }

////    void setCrtDate(QDateTime crtDate){ m_crtDate = crtDate; emit crtDateChanged(crtDate);}
////    QDateTime crtDate() const{ return m_crtDate; }

////    void setRefDate(QDateTime refDate){ m_refDate = refDate; emit refDateChanged(refDate);}
////    QDateTime refDate() const{ return m_refDate; }

////    void setLayout(QList<sections> layout){ m_layout = layout; emit layoutChanged(layout);}
////    QList<sections> layout() const{ return m_layout; }

////    void setCopyAt(QList<int> copyAt){ m_copyAt = copyAt; emit copyAtChanged(copyAt);}
////    QList<int> copyAt() const{ return m_copyAt; }

//    //foreign key relations
//    void setAnalyst_id(int analyst_id){ m_analyst_id = analyst_id; emit analystFKNotify(analyst_id, this);}
//    int analyst_id() const{ return m_analyst_id; }

//    void setAnalyst(Practician* analyst){ m_analyst = analyst; emit analystChanged(analyst); }
//    Practician* analyst() const{ return m_analyst; }

//    void setApplicant_id(int applicant_id){ m_applicant_id = applicant_id; emit applicantFKNotify(applicant_id, this);}
//    int applicant_id() const{ return m_applicant_id; }

//    void setApplicant(Practician* applicant){ m_applicant = applicant; emit applicantChanged(applicant); }
//    Practician* applicant() const{ return m_applicant; }

//    void setCurve(Curve* curve){ m_curve = curve; emit curveChanged(curve);}
//    Curve* curve() const{ return m_curve; }

//    void setCurve_id(int curve_id){ m_curve_id = curve_id; emit curveFKNotify(curve_id, this);}
//    int curve_id() const{ return m_curve_id; }

//signals:
//    void idChanged(int);
//    void templateNameChanged(QString);
//    void templateLangChanged(QString);
//    void titleChanged(QString);
//    void referenceChanged(QString);
//    void comedicChanged(QString);
//    void adverseChanged(QString);
//    void answerChanged(QString);
//    void commentsChanged(QString);
//    void interpretationChanged(QString);
//    void posologyChanged(QString);
//    void monitoringChanged(QString);
//    void imageChanged(QVariant);
//    void referencesChanged(QString);
//    void distributedChanged(bool);
//    void crtDateChanged(QDateTime);
//    void refDateChanged(QDateTime);
//    void layoutChanged(QList<sections>);
//    void copyAtChanged(QList<int>);
//    void predictionChanged(int);

//    //foreign key relations
//    void applicantChanged(Practician*);
//    void applicantFKNotify(int, Report*);
//    void analystChanged(Practician*);
//    void analystFKNotify(int, Report*);
//    void curveChanged(Curve*);
//    void curveFKNotify(int, Report*);

//private:
//    QString m_templateName;
//    QString m_templateLang;

//    //Database IDs
//    int m_id;
////    int m_recipient;
//    int m_prediction;

//    //General section
//    QString m_title;
//    QString m_reference;

//    //Patient section
//    QString m_comedic;
//    QString m_adverse;
//    QString m_answer;
//    QString m_comments;

//    //Interpretation section
//    QString m_interpretation;
//    QString m_posology;

//    //Monitoring section
//    QString m_monitoring;

//    //Chart section
//    QVariant m_image;

//    //References section
//    QString m_references;

//    //Meta data
//    bool m_distributed;
//    QDateTime m_crtDate;
//    QDateTime m_refDate;
//    QList<sections> m_layout;
//    QList<int> m_copyAt;

//    //foreign key relations
//    int m_analyst_id;
//    Practician* m_analyst;
//    int m_applicant_id;
//    Practician* m_applicant;
//    int m_curve_id;
//    Curve* m_curve;
//};


//class Covariate : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY covariatePKNotify)

//    //foreign key relations
//    Q_PROPERTY(int patientvariate_id READ patientvariate_id WRITE setpatientvariate_id NOTIFY patientvariateFKNotify)
//    Q_PROPERTY(PatientVariate* patientvariate READ patientvariate WRITE setpatientvariate NOTIFY patientvariateChanged)
//    Q_PROPERTY(int drugvariate_id READ drugvariate_id WRITE setdrugvariate_id NOTIFY drugvariateFKNotify)
//    Q_PROPERTY(DrugVariate* drugvariate READ drugvariate WRITE setdrugvariate NOTIFY drugvariateChanged)
//    Q_PROPERTY(int curve_id READ curve_id WRITE setCurve_id NOTIFY curveFKNotify)
//    Q_PROPERTY(Curve* curve READ curve WRITE setCurve NOTIFY curveChanged)
//public:

//    Q_INVOKABLE Covariate() : m_curve_id(-1), m_patientvariate_id(-1), m_drugvariate_id(-1), m_curve(0), m_drugvariate(0), m_patientvariate(0) { setId(-1); }

//    Q_INVOKABLE Covariate(QObject* parent) : m_curve_id(-1), m_patientvariate_id(-1), m_drugvariate_id(-1), m_curve(0), m_drugvariate(0), m_patientvariate(0) { setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit covariatePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setpatientvariate_id(int patientvariate_id){ m_patientvariate_id = patientvariate_id; emit patientvariateFKNotify(patientvariate_id, this);}
//    int patientvariate_id() const{ return m_patientvariate_id; }

//    void setpatientvariate(PatientVariate* patientvariate){ m_patientvariate = patientvariate; emit patientvariateChanged(patientvariate);}
//    PatientVariate* patientvariate() const{ return m_patientvariate; }

//    void setdrugvariate_id(int drugvariate_id){ m_drugvariate_id = drugvariate_id; emit drugvariateFKNotify(drugvariate_id, this);}
//    int drugvariate_id() const{ return m_drugvariate_id; }

//    void setdrugvariate(DrugVariate* drugvariate){ m_drugvariate = drugvariate; emit drugvariateChanged(drugvariate);}
//    DrugVariate* drugvariate() const{ return m_drugvariate; }

//    void setCurve_id(int curve_id){ m_curve_id = curve_id; emit curveFKNotify(curve_id, this);}
//    int curve_id() const{ return m_curve_id; }

//    void setCurve(Curve* curve){ m_curve = curve; emit curveChanged(curve);}
//    Curve* curve() const{ return m_curve; }

//signals:
//    void covariatePKNotify(int, Covariate*);
//    void curveFKNotify(int, Covariate*);
//    Tucuxi::Curve* curveChanged(Curve*);
//    int patientvariateFKNotify(int, Covariate*);
//    Tucuxi::PatientVariate* patientvariateChanged(PatientVariate*);
//    int drugvariateFKNotify(int, Covariate*);
//    Tucuxi::DrugVariate* drugvariateChanged(DrugVariate*);

//private:
//    int m_id;
//    int m_drugvariate_id;
//    DrugVariate* m_drugvariate;
//    int m_patientvariate_id;
//    PatientVariate* m_patientvariate;
//    int m_curve_id;
//    Curve* m_curve;
//};

//class Correlation : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY correlationPKNotify)

//    //foreign key relations
//    Q_PROPERTY(int drugparameter1_id READ drugparameter1_id WRITE setdrugparameter1_id NOTIFY drugparameter1FKNotify)
//    Q_PROPERTY(DrugParameter* drugparameter1 READ drugparameter1 WRITE setdrugparameter1 NOTIFY drugparameter1Changed)
//    Q_PROPERTY(int drugparameter2_id READ drugparameter2_id WRITE setdrugparameter2_id NOTIFY drugparameter2FKNotify)
//    Q_PROPERTY(DrugParameter* drugparameter2 READ drugparameter2 WRITE setdrugparameter2 NOTIFY drugparameter2Changed)
//    Q_PROPERTY(double factor READ factor WRITE setFactor NOTIFY factorChanged)
//    //    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
////    Q_PROPERTY(QObject* drug READ drug WRITE setDrug NOTIFY drugChanged)
//public:

//    Q_INVOKABLE Correlation() : m_drugparameter1_id(-1), m_drugparameter2_id(-1), m_drugparameter1(0), m_drugparameter2(0), m_factor(0) { setId(-1); }

//    Q_INVOKABLE Correlation(QObject* parent) : m_drugparameter1_id(-1), m_drugparameter2_id(-1), m_drugparameter1(0), m_drugparameter2(0) { setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit correlationPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setdrugparameter1_id(int drugparameter1_id){ m_drugparameter1_id = drugparameter1_id; emit drugparameter1FKNotify(drugparameter1_id, this);}
//    int drugparameter1_id() const{ return m_drugparameter1_id; }

//    void setdrugparameter1(DrugParameter* drugparameter1){ m_drugparameter1 = drugparameter1; emit drugparameter1Changed(drugparameter1);}
//    DrugParameter* drugparameter1() const{ return m_drugparameter1; }

//    void setdrugparameter2_id(int drugparameter2_id){ m_drugparameter2_id = drugparameter2_id; emit drugparameter2FKNotify(drugparameter2_id, this);}
//    int drugparameter2_id() const{ return m_drugparameter2_id; }

//    void setdrugparameter2(DrugParameter* drugparameter2){ m_drugparameter2 = drugparameter2; emit drugparameter2Changed(drugparameter2);}
//    DrugParameter* drugparameter2() const{ return m_drugparameter2; }

//    void setFactor(double factor){ m_factor = factor; emit factorChanged(factor);}
//    double factor() const{ return m_factor; }
////    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
////    int drug_id() const{ return m_drug_id; }

////    void setDrug(QObject* drug){ m_drug = drug; emit drugChanged(drug);}
////    QObject* drug() const{ return m_drug; }

//signals:
//    void correlationPKNotify(int, Correlation*);
//    int drugparameter1FKNotify(int, Correlation*);
//    Tucuxi::DrugParameter* drugparameter1Changed(DrugParameter*);
//    int drugparameter2FKNotify(int, Correlation*);
//    Tucuxi::DrugParameter* drugparameter2Changed(DrugParameter*);
//    double factorChanged(double);
////    int drugFKNotify(int, Correlation*);
////    QObject* drugChanged(QObject*);

//private:
//    int m_id;
//    int m_drugparameter1_id;
//    DrugParameter* m_drugparameter1;
//    int m_drugparameter2_id;
//    DrugParameter* m_drugparameter2;
//    double m_factor;
////    int m_drug_id;
////    QObject* m_drug;
//};


/** A set of parameters.
 * \ingroup utils
 * This object can be seen like a list of parameters, but whith some parameter-specific functions and data, like for example the global additive and proportional variablities, or the covariances.
 */
//class ParameterSet
//{
//public:

//    //! Create a parameter set and define its type and model.
//    ParameterSet(ParameterType type = POPULATION, const QString &model = "");

//    //! Create a parameter set from another parameter set.
//    ParameterSet(const ParameterSet* other);

//    //! Parameter type of this set.
//    ParameterType type () const;

//    //! Set the parameter type of this set.
//    void setType (ParameterType type);

//    //! List of contained parameters ID.
//    QStringList pids () const;

//    //! Indicate if the parameter that corresponds to this ID exists in the set.
//    bool contains(const QString &pid) const;

//    //! Get the parameter that corresponds to this ID.
//    DrugParameter get(const QString &pid) const;

//    //! Set this parameter if it exists, add it otherwise
//    void set(const DrugParameter &parameter);

//    //! Copy the other set parameters into ourselves (remove the old ones)
//    void copy (const ParameterSet* other);

//    //! Remove all the parameters in this set, but keep the parameter type.
//    void clear ();

//    //! Number of parameters contained within.
//    int size () const;

//    /** Set this correlation if it exists, add it otherwise
//     * @param param1 The first parameter ID.
//     * @param param2 The second parameter ID.
//     * @param value The correlation value.
//     * @return True if successfully added, otherwise false.
//     */
//    bool setCorrelation(QString param1, QString param2, const double &value);

//    /** Get the value of the specified correlation, or 0 if it doesn't exist.
//     * @param param1 The first parameter ID.
//     * @param param2 The second parameter ID.
//     * @return The corresponding correlation value, or 0 if it doesn't exist.
//     */
//    double correlation(QString param1, QString param2) const;

//    //! Set the global additive and proportional variablities.
//    void setGlobal (double additive, double proportional);

//    //! The global additive variability.
//    double globalAdditive () const;

//    //! The global proportional variability.
//    double globalProportional () const;

//    //! Get this parameter set model ID
//    QString model() const;

//    //! Set this parameter set model ID
//    void setModel (const QString &model);

//    //! Access the parameter through its ID.
//    DrugParameter& operator[] (const QString &pid);

//    //! Access the constant parameter through its ID.
//    const DrugParameter operator[] (const QString &pid) const;

//private:
//    //The parameters type, eg. a posteriori
//    ParameterType _type;

//    //The paramaters list
//    QMap<QString,DrugParameter*> _parameters;

//    //The correlations list
//    QMap<QString,double> _correlations;

//    //The global additive and proportional variablities
//    double _proportional, _additive;

//    //The model's id
//    QString _model;
//};
//Q_DECLARE_METATYPE(Tucuxi::ParameterSet)


//class ValidDose : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY validdosePKNotify)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

//    //foreign key relations
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//public:

//    Q_INVOKABLE ValidDose() : m_drug(0) { setId(-1); }

//    Q_INVOKABLE ValidDose(QObject* parent) : m_drug(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit validdosePKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setValue(double value){m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//    //foreign key
//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//signals:
//    void validdosePKNotify(int, ValidDose*);
//    void valueChanged(double);
//    void unitChanged(QString);

//    //foreign keyu
//    void drugFKNotify(int, ValidDose*);
//    void drugChanged(Drug*);


//private:
//    int m_id;
//    double m_value;
//    QString m_unit;

//    //foreign key
//    int m_drug_id;
//    Drug* m_drug;
//};

//class ValidInterval : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

//    //foreign key relations
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//public:

//    Q_INVOKABLE ValidInterval() : m_drug(0) { setId(-1); }

//    Q_INVOKABLE ValidInterval(QObject* parent) : m_drug(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit idChanged(id);}
//    int id() const{ return m_id; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setValue(double value){m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//    //foreign key
//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//signals:
//    void idChanged(int);
//    void valueChanged(double);
//    void unitChanged(QString);

//    //foregin key
//    void drugFKNotify(int, ValidInterval*);
//    void drugChanged(Drug*);

//private:
//    int m_id;
//    double m_value;
//    QString m_unit;

//    //foreign key
//    int m_drug_id;
//    Drug* m_drug;
//};

//class ValidInfusion : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
//    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
//    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

//    //foreign key relations
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//public:

//    Q_INVOKABLE ValidInfusion() : m_drug(0) { setId(-1); }

//    Q_INVOKABLE ValidInfusion(QObject* parent) : m_drug(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit idChanged(id);}
//    int id() const{ return m_id; }

//    void setUnit(QString unit){m_unit = unit; emit unitChanged(unit);}
//    QString unit() const{ return m_unit; }

//    void setValue(double value){m_value = value; emit valueChanged(value);}
//    double value() const{ return m_value; }

//    //foreign key
//    void setDrug_id(int drug_id){ m_drug_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }

//signals:
//    void idChanged(int);
//    void valueChanged(double);
//    void unitChanged(QString);

//    //foregin key
//    void drugFKNotify(int, ValidInfusion*);
//    void drugChanged(Drug*);

//private:

//    int m_id;
//    double m_value;
//    QString m_unit;

//    //foregin key
//    int m_drug_id;
//    Drug* m_drug;
//};

//class Operation : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY operationPKNotify)
//    Q_PROPERTY(QString formula READ formula WRITE setFormula NOTIFY formulaChanged)

//    //foreign key
//    Q_PROPERTY(int drugparameter_id READ drugparameter_id WRITE setdrugparameter_id NOTIFY drugparameterFKNotify)
//    Q_PROPERTY(DrugParameter* drugparameter READ drugparameter WRITE setdrugparameter NOTIFY drugparameterChanged)
//public:

//    Q_INVOKABLE Operation() : m_drugparameter(0) { setId(-1); }

//    Q_INVOKABLE Operation(QObject* parent) : m_drugparameter(0) { this->setParent(parent); setId(-1); }

//    void setId(int id){ m_id = id; emit operationPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setFormula(QString formula){m_formula = formula; emit formulaChanged(formula);}
//    QString formula() const{ return m_formula; }

//    bool isValid() const;

//    //foriegn key
//    void setdrugparameter_id(int drugparameter_id){m_drugparameter_id = drugparameter_id; emit drugparameterFKNotify(drugparameter_id, this);}
//    int drugparameter_id() const{ return m_drugparameter_id; }

//    void setdrugparameter(class DrugParameter* drugparameter){ m_drugparameter = drugparameter; emit drugparameterChanged(drugparameter);}
//    DrugParameter* drugparameter() const{ return m_drugparameter; }


////    Operation(QString parameterId, QString formula);
////    QString parameterId() const;
////    void setParameterId(const QString &parameterId);
////    QString formula() const;
////    void setFormula(const QString &formula);

//signals:
//    void operationPKNotify(int, Operation*);
//    void formulaChanged(QString);

//    //foreign key
//    void drugparameterFKNotify(int, Operation*);
//    void drugparameterChanged(DrugParameter*);

//private:
//    int m_id;
//    QString m_formula;

//    //foreign key
//    int m_drugparameter_id;
//    DrugParameter* m_drugparameter;
//};


//class Target : public QObject {
////    Q_DECLARE_TR_FUNCTIONS(Target)
//    Q_OBJECT
//    Q_PROPERTY(int id READ id WRITE setId NOTIFY targetPKNotify)
//    Q_PROPERTY(double cmin READ cmin WRITE setCmin NOTIFY cminChanged)
//    Q_PROPERTY(double cmax READ cmax WRITE setCmax NOTIFY cmaxChanged)
//    Q_PROPERTY(double cbest READ cbest WRITE setCbest NOTIFY cbestChanged)
//    Q_PROPERTY(double tmin READ tmin WRITE setTmin NOTIFY tminChanged)
//    Q_PROPERTY(double tmax READ tmax WRITE setTmax NOTIFY tmaxChanged)
//    Q_PROPERTY(double tbest READ tbest WRITE setTbest NOTIFY tbestChanged)
//    Q_PROPERTY(TargetMethod::TargetType targettype READ targettype WRITE setTargettype NOTIFY targettypeChanged)
//    Q_ENUMS(TargetMethod::TargetType)

//    //foreign keys
//    Q_PROPERTY(Drug* drug READ drug WRITE setDrug NOTIFY drugChanged)
//    Q_PROPERTY(int drug_id READ drug_id WRITE setDrug_id NOTIFY drugFKNotify)

//public:

//    Q_INVOKABLE Target() : m_drug(0), m_drug_id(-1), m_cmin(0),
//        m_cmax(0), m_cbest(0), m_tmin(0),
//        m_tmax(0), m_tbest(0), m_id(-1),
//        m_targettype(TargetMethod::TargetType::MeanTarget) {}

//    Q_INVOKABLE Target(QObject* parent)
//        : m_drug(0), m_drug_id(-1), m_cmin(0),
//          m_cmax(0), m_cbest(0), m_tmin(0),
//          m_tmax(0), m_tbest(0), m_id(-1),
//          m_targettype(TargetMethod::TargetType::MeanTarget)
//    { this->setParent(parent);}

//    enum TargetMethod::TargetType {
//        ResidualTarget, //! Targeted residual concentration, or targetted concentration at the end of a complete cycle.
//        PeakTarget,     //! Targeted peak concentration. WARNING: It does not correspond to the maximum of a cycle, but the value at a specific time after the intake.
//        MeanTarget,     //! Targeted mean concentration, currently unused.
//        UnknownTarget   //! Unknown or invalid target.
//    };

//    void setId(int id){ m_id = id; emit targetPKNotify(id, this);}
//    int id() const{ return m_id; }

//    void setTargettype(TargetMethod::TargetType targettype){ m_targettype = targettype; emit targettypeChanged(targettype);}
//    TargetMethod::TargetType targettype() const{ return m_targettype; }

//    void setCmin(double cmin){ m_cmin = cmin; emit cminChanged(cmin);}
//    double cmin() const{ return m_cmin; }

//    void setCmax(double cmax){ m_cmax = cmax; emit cmaxChanged(cmax);}
//    double cmax() const{ return m_cmax; }

//    void setCbest(double cbest){ m_cbest = cbest; emit cbestChanged(cbest);}
//    double cbest() const{ return m_cbest; }

//    void setTmin(double tmin){ m_tmin = tmin; emit tminChanged(tmin);}
//    double tmin() const{ return m_tmin; }

//    void setTmax(double tmax){ m_tmax = tmax; emit tmaxChanged(tmax);}
//    double tmax() const{ return m_tmax; }

//    void setTbest(double tbest){ m_tbest = tbest; emit tbestChanged(tbest);}
//    double tbest() const{ return m_tbest; }

//    //foreign key relations
//    void setDrug_id(int drug_id){ m_id = drug_id; emit drugFKNotify(drug_id, this);}
//    int drug_id() const{ return m_drug_id; }

//    void setDrug(Drug* drug){ m_drug = drug; emit drugChanged(drug);}
//    Drug* drug() const{ return m_drug; }


//signals:
//    void targetPKNotify(int, Target*);
//    void targettypeChanged(TargetMethod::TargetType);
//    void cminChanged(double);
//    void cmaxChanged(double);
//    void cbestChanged(double);
//    void tminChanged(double);
//    void tmaxChanged(double);
//    void tbestChanged(double);

//    //foreign keys
//    void drugFKNotify(int, Target*);
//    void drugChanged(Drug*);

//private:
//    int m_id;
//    TargetMethod::TargetType m_targettype;
//    double m_cmin;
//    double m_cmax;
//    double m_cbest;
//    double m_tmin;
//    double m_tmax;
//    double m_tbest;

//    //foregin key relations
//    int m_drug_id;
//    Drug* m_drug;
//};


//! Define a ID as in a database
//typedef int int;

//! Invalid intifier, can be used as default value
//static const int -1 = -1;

//! A database account
/** \ingroup database
  An account is used by a database to connect to its storing method.
  A database backend (plugin) is able to give which of these data it needs through the DB::needs() function.
*/
//class DBAccount {
//public:
//   //! The database URL, may contain the protocol (http://,ssh://,sql://) and the port used (:80,:1234)
//   QString url;
//   //! The user name used for the connection (no link with the Tucuxi user)
//   QString username;
//   //! The password used for the connection (no link with an Tucuxi password), in plain text
//   QString password;
//   //! The backend to use for this account
//   QString backend;
//   //! Used to know is the account is currently valid or not
//   bool valid;
//};

//! \ingroup database
enum Gender {
   MALE,
   FEMALE
};

}

//Q_DECLARE_METATYPE(Tucuxi::ParameterType)

//Q_DECLARE_METATYPE(Tucuxi::ErrorModel*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::ErrorModel, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Drug*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Drug, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Parameter*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Parameter, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::DrugParameter*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::DrugParameter, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::DrugVariate*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::DrugVariate, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Location*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Location, Tucuxi::Location*)

//Q_DECLARE_METATYPE(Tucuxi::Institute*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Institute, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Patient*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Patient, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Practician*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Practician, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::PatientVariate*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::PatientVariate, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::VariateSharedMeasure)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::VariateMeasure, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Clinicals*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Clinicals, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::ComputeEngine*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::ComputeEngine, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Curve*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Curve, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Dosage*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Dosage, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::SharedMeasure)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Measure, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Option*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Option, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::CurveOption*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::CurveOption, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Report*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Report, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Covariate*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Covariate, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Correlation*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Correlation, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::ValidDose*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::ValidDose, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::ValidInterval*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::ValidInterval, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::ValidInfusion*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::ValidInfusion, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Operation*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Operation, QObject*)

//Q_DECLARE_METATYPE(Tucuxi::Target*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(Tucuxi::Target, QObject*)
#endif // GENERICDBOBJECTS_H
