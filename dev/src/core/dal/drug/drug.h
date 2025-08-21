/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef DRUG_H
#define DRUG_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"
#include "core/dal/amount.h"

Q_MOC_INCLUDE("core/dal/drug/adme.h")
Q_MOC_INCLUDE("core/dal/drug/concentrations.h")
Q_MOC_INCLUDE("core/dal/drug/doses.h")
Q_MOC_INCLUDE("core/dal/drug/drugvariate.h")
Q_MOC_INCLUDE("core/dal/drug/errormodel.h")
Q_MOC_INCLUDE("core/dal/drug/halflife.h")
Q_MOC_INCLUDE("core/dal/drug/infusions.h")
Q_MOC_INCLUDE("core/dal/drug/intervals.h")
Q_MOC_INCLUDE("core/dal/drug/metadata.h")
Q_MOC_INCLUDE("core/dal/drug/parameters.h")
Q_MOC_INCLUDE("core/dal/drug/results.h")
Q_MOC_INCLUDE("core/dal/drug/standardtreatment.h")
Q_MOC_INCLUDE("core/dal/drug/target.h")
Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugXmlDescriptor;
class ValidDoses;
class ValidIntervals;
class ValidInfusions;
class Target;
class TargetList;
class ADME;
class Halflife;
class Concentrations;
class Results;
class ValidDoses;
class Operation;
class DrugVariate;
class DrugVariateList;
class ParameterSet;
class MetaData;
class ErrorModel;
class TranslatableString;
class StandardTreatment;

class Analyte : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Analyte)

    AUTO_PROPERTY_DECL(QString, analyteId, AnalyteId)
    AUTO_PROPERTY_DECL(TranslatableString*, name, Name)
    AMOUNT_PROPERTY_DECL(Amount, molarMass, MolarMass)

    protected:

        Q_INVOKABLE Analyte(AbstractRepository *repository, QObject* parent);
};

QML_POINTERLIST_CLASS_DECL(AnalyteList, Analyte)

class ActiveSubstance : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ActiveSubstance)

    AUTO_PROPERTY_DECL(QString, substanceId, SubstanceId)
    AUTO_PROPERTY_DECL(TranslatableString*, name, Name)
    TRANSLATABLE_STRING_PROPERTY_DECL(description, Description)
    AUTO_PROPERTY_DECL(QStringList, atc, Atc)
    AMOUNT_PROPERTY_DECL(Amount, molarMass, MolarMass)
    AUTO_PROPERTY_DECL(QStringList, brands, Brands)
    AUTO_PROPERTY_DECL(AnalyteList*, analytes, Analytes)

protected:

    Q_INVOKABLE ActiveSubstance(AbstractRepository *repository, QObject* parent);

};

QML_POINTERLIST_CLASS_DECL(ActiveSubstanceList, ActiveSubstance)


class LightActiveSubstance : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(LightActiveSubstance)

    AUTO_PROPERTY_DECL(QString, substanceId, SubstanceId)
    AUTO_PROPERTY_DECL(TranslatableString*, name, Name)

    ~LightActiveSubstance() {
        int i = 0;
        i++;
    }

protected:

    Q_INVOKABLE LightActiveSubstance(AbstractRepository *repository, QObject* parent);

};

QML_POINTERLIST_CLASS_DECL(LightActiveSubstanceList, LightActiveSubstance)




class DrugModel : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugModel)

    AUTO_PROPERTY_DECL(ActiveSubstance*, activeSubstance, ActiveSubstance)
    TRANSLATABLE_STRING_PROPERTY_DECL(domainName, DomainName)
    AUTO_PROPERTY_DECL(QString, drugModelId, DrugModelId)
//    AUTO_PROPERTY_DECL(QStringList, atc, Atc)
    AUTO_PROPERTY_DECL(QString, indication, Indication)
    AUTO_PROPERTY_DECL(QString, population, Population)
//    TRANSLATABLE_STRING_PROPERTY_DECL(name, Name)
    AUTO_PROPERTY_DECL(TranslatableString*, name, Name)
    TRANSLATABLE_STRING_PROPERTY_DECL(studyName, StudyName)
    TRANSLATABLE_STRING_PROPERTY_DECL(descriptions, Descriptions)
    AUTO_PROPERTY_DECL(ADME*, adme, Adme)
    AUTO_PROPERTY_DECL(Halflife*, halflife, Halflife)
    AUTO_PROPERTY_DECL(Concentrations*, concentrations, Concentrations)
    AUTO_PROPERTY_DECL(Results*, results, Results)
    AUTO_PROPERTY_DECL(StandardTreatment*, standardTreatment, StandardTreatment)
    AUTO_PROPERTY_DECL(ValidDoses*, doses, Doses)
    AUTO_PROPERTY_DECL(ValidInfusions*, infusions, Infusions)
    AUTO_PROPERTY_DECL(ValidIntervals*, intervals, Intervals)
    AUTO_PROPERTY_DECL(DrugVariateList*, covariates, Covariates)
//    AUTO_PROPERTY_DECL(QStringList, brands, Brands)
    AUTO_PROPERTY_DECL(TargetList*, targets, Targets)
    AMOUNT_PROPERTY_DECL(Amount, molarMass, MolarMass)
    AUTO_PROPERTY_DECL(ParameterSet*, parameters, Parameters)
    AUTO_PROPERTY_DECL(ErrorModel*, errorModel, ErrorModel)
    AUTO_PROPERTY_DECL(MetaData*,metaData,MetaData)
    AUTO_PROPERTY_DECL(QString, pkModelId, PkModelId)
    AUTO_PROPERTY_DECL(QString, studyAuthors, StudyAuthors)
    TRANSLATABLE_STRING_PROPERTY_DECL(headComments, HeadComments)
    TRANSLATABLE_STRING_PROPERTY_DECL(conversionsComments, ConversionsComments)
    TRANSLATABLE_STRING_PROPERTY_DECL(generalComments, GeneralComments)

    AUTO_PROPERTY_DECL(QString, filePath, FilePath)
    AUTO_PROPERTY_DECL(QString, defaultUnit, DefaultUnit)

protected:
    Q_INVOKABLE DrugModel(AbstractRepository *repository, const Descriptor &descriptor);

public: // YTA: To be checked and put protected
    Q_INVOKABLE DrugModel(AbstractRepository *repository, const DrugXmlDescriptor *descriptor);

     // YTA: Needed by ApputilsFactory which is outside core. Should be solved...
    Q_INVOKABLE DrugModel(AbstractRepository *repository = nullptr, QObject* parent = 0);

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:


    bool withPna() const;
    Descriptor descriptor() const;
    void setDescriptor(const Descriptor &descriptor);

    QString description() const;
    void setDescription(const QString &description);

//    QList<DrugVariate*> covariates() const;
//    void setCovariates(const QList<DrugVariate*> &covariates);

//    ParameterSet* parameters() const;
//    void setParameters(ParameterSet* parameters);

    std::string getSignName() { return signName; }
    std::string getSignOgranization() { return signOrganization; }
    std::string getSignLocality() { return signLocality; }

signals:
    void modelidChanged(QString);
//    void parametersChanged(ParameterSet*);

private:
    Descriptor _descriptor;
    QString _modelId;
//    QList<DrugVariate*> _covariates;
//    ParameterSet* _parameters;

    std::string signName = "Unknown";
    std::string signOrganization = "Unknown";
    std::string signLocality = "Unknown";

};

QML_POINTERLIST_CLASS_DECL(DrugModelList, DrugModel)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ActiveSubstance*)

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugModel*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DrugModel*>)

#endif // DRUG_H
