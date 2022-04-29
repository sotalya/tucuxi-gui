//@@license@@

#include "core/dal/drug/drug.h"
#include "core/drugxmldescriptor.h"
#include "core/core.h"
//#include "coremessagehandler.h"
#include "core/definitionmanager.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include "core/dal/drug/parameters.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/results.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/target.h"
#include "core/corefactory.h"
#include "core/dal/drug/translatablestring.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/standardtreatment.h"

namespace ezechiel {
namespace core {


AUTO_PROPERTY_IMPL(Analyte, QString, analyteId, AnalyteId)
AUTO_PROPERTY_IMPL(Analyte, TranslatableString*, name, Name)
AMOUNT_PROPERTY_IMPL(Analyte, Amount, molarMass, MolarMass)

QML_POINTERLIST_CLASS_IMPL(AnalyteList, Analyte)

Analyte::Analyte(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    _name = CoreFactory::createEntity<TranslatableString>(repository, this);

}





AUTO_PROPERTY_IMPL(ActiveSubstance, QString, substanceId, SubstanceId)
AUTO_PROPERTY_IMPL(ActiveSubstance, TranslatableString*, name, Name)
TRANSLATABLE_STRING_PROPERTY_IMPL(ActiveSubstance, description, Description)
AUTO_PROPERTY_IMPL(ActiveSubstance, QStringList, atc, Atc)
AMOUNT_PROPERTY_IMPL(ActiveSubstance, Amount, molarMass, MolarMass)
AUTO_PROPERTY_IMPL(ActiveSubstance, QStringList, brands, Brands)
AUTO_PROPERTY_IMPL(ActiveSubstance, AnalyteList*, analytes, Analytes)

QML_POINTERLIST_CLASS_IMPL(ActiveSubstanceList, ActiveSubstance)

AUTO_PROPERTY_IMPL(LightActiveSubstance, QString, substanceId, SubstanceId)
AUTO_PROPERTY_IMPL(LightActiveSubstance, TranslatableString*, name, Name)

QML_POINTERLIST_CLASS_IMPL(LightActiveSubstanceList, LightActiveSubstance)

LightActiveSubstance::LightActiveSubstance(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    _name = CoreFactory::createEntity<TranslatableString>(repository, this);
}

ActiveSubstance::ActiveSubstance(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    _description = CoreFactory::createEntity<TranslatableString>(repository,this);
    _name = CoreFactory::createEntity<TranslatableString>(repository, this);
    _analytes = CoreFactory::createEntity<AnalyteList>(repository, this);

}


AUTO_PROPERTY_IMPL(DrugModel, ActiveSubstance*, activeSubstance, ActiveSubstance)
TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, domainName,DomainName)
AUTO_PROPERTY_IMPL(DrugModel, QString, drugModelId, DrugModelId)
//AUTO_PROPERTY_IMPL(DrugModel, QStringList, atc, Atc)
AUTO_PROPERTY_IMPL(DrugModel, QString, indication, Indication)
AUTO_PROPERTY_IMPL(DrugModel, QString, population, Population)
//TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, name, Name)
AUTO_PROPERTY_IMPL(DrugModel, TranslatableString*, name, Name)
TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, studyName, StudyName)
TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, descriptions, Descriptions)
AUTO_PROPERTY_IMPL(DrugModel, ADME*, adme, Adme)
AUTO_PROPERTY_IMPL(DrugModel, Halflife*, halflife, Halflife)
AUTO_PROPERTY_IMPL(DrugModel, Concentrations*, concentrations, Concentrations)
AUTO_PROPERTY_IMPL(DrugModel, Results*, results, Results)
AUTO_PROPERTY_IMPL(DrugModel, StandardTreatment*, standardTreatment, StandardTreatment)
AUTO_PROPERTY_IMPL(DrugModel, ValidDoses*, doses, Doses)
AUTO_PROPERTY_IMPL(DrugModel, ValidInfusions*, infusions, Infusions)
AUTO_PROPERTY_IMPL(DrugModel, ValidIntervals*, intervals, Intervals)
AUTO_PROPERTY_IMPL(DrugModel, DrugVariateList*, covariates, Covariates)
//AUTO_PROPERTY_IMPL(DrugModel, QStringList, brands, Brands)
AUTO_PROPERTY_IMPL(DrugModel, TargetList*, targets, Targets)
// YTA: The operation should be in Parameter. To be removed very soon
AUTO_PROPERTY_IMPL(DrugModel, QList<Operation*>, operations, Operations)
AMOUNT_PROPERTY_IMPL(DrugModel, Amount, molarMass, MolarMass)
AUTO_PROPERTY_IMPL(DrugModel, ParameterSet*, parameters, Parameters)
AUTO_PROPERTY_IMPL(DrugModel, ErrorModel*, errorModel, ErrorModel)
AUTO_PROPERTY_IMPL(DrugModel, MetaData*, metaData, MetaData)
AUTO_PROPERTY_IMPL(DrugModel, QString, pkModelId, PkModelId)
AUTO_PROPERTY_IMPL(DrugModel, QString, studyAuthors, StudyAuthors)

TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, headComments, HeadComments)
TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, conversionsComments, ConversionsComments)
TRANSLATABLE_STRING_PROPERTY_IMPL(DrugModel, generalComments, GeneralComments)

//Constructor
DrugModel::DrugModel(AbstractRepository *repository, QObject* parent) : Entity(repository, parent),
  _doses(CoreFactory::createEntity<ValidDoses>(repository, this)),
  _intervals(CoreFactory::createEntity<ValidIntervals>(repository, this)),
  _infusions(CoreFactory::createEntity<ValidInfusions>(repository, this)),
  _adme(CoreFactory::createEntity<ADME>(repository, this)),
  _halflife(CoreFactory::createEntity<Halflife>(repository, this)),
  _concentrations(CoreFactory::createEntity<Concentrations>(repository, this)),
  _results(CoreFactory::createEntity<Results>(repository, this)),
  _covariates(CoreFactory::createEntity<DrugVariateList>(repository, this)),
  _standardTreatment(CoreFactory::createEntity<StandardTreatment>(repository, this)),
  _targets(CoreFactory::createEntity<TargetList>(repository, this)),
  _parameters(CoreFactory::createEntity<ParameterSet>(repository, this)),
  _errorModel(CoreFactory::createEntity<ErrorModel>(repository, this)),
  _name(CoreFactory::createEntity<TranslatableString>(repository, this)),
  _activeSubstance(CoreFactory::createEntity<ActiveSubstance>(repository, this)),
  _headComments(CoreFactory::createEntity<TranslatableString>(repository, this)),
  _conversionsComments(CoreFactory::createEntity<TranslatableString>(repository, this)),
  _generalComments(CoreFactory::createEntity<TranslatableString>(repository, this)),
  _metaData(0)
{

}

//Constructor
DrugModel::DrugModel(AbstractRepository *repository, const Descriptor &descriptor) :Entity(repository), _descriptor(descriptor),
  _doses(CoreFactory::createEntity<ValidDoses>(repository, this)),
  _intervals(CoreFactory::createEntity<ValidIntervals>(repository, this)),
  _infusions(CoreFactory::createEntity<ValidInfusions>(repository, this)),
  _adme(CoreFactory::createEntity<ADME>(repository, this)),
  _halflife(CoreFactory::createEntity<Halflife>(repository, this)),
  _concentrations(CoreFactory::createEntity<Concentrations>(repository, this)),
  _results(CoreFactory::createEntity<Results>(repository, this)),
  _covariates(CoreFactory::createEntity<DrugVariateList>(repository, this)),
  _targets(CoreFactory::createEntity<TargetList>(repository, this)),
  _parameters(CoreFactory::createEntity<ParameterSet>(repository, this)),
  _errorModel(CoreFactory::createEntity<ErrorModel>(repository, this)),
  _name(CoreFactory::createEntity<TranslatableString>(repository, this)),
  _metaData(0)
{

}

//Constructor
DrugModel::DrugModel(AbstractRepository *repository, const DrugXmlDescriptor *descriptor) : Entity(repository),
  _doses(CoreFactory::createEntity<ValidDoses>(repository, this)),
  _intervals(CoreFactory::createEntity<ValidIntervals>(repository, this)),
  _infusions(CoreFactory::createEntity<ValidInfusions>(repository, this)),
  _adme(CoreFactory::createEntity<ADME>(repository, this)),
  _halflife(CoreFactory::createEntity<Halflife>(repository, this)),
  _concentrations(CoreFactory::createEntity<Concentrations>(repository, this)),
  _results(CoreFactory::createEntity<Results>(repository, this)),
  _covariates(CoreFactory::createEntity<DrugVariateList>(repository, this)),
  _targets(CoreFactory::createEntity<TargetList>(repository, this)),
  _parameters(CoreFactory::createEntity<ParameterSet>(repository, this)),
  _errorModel(CoreFactory::createEntity<ErrorModel>(repository, this)),
  _metaData(0)
{
    //Check if the descriptor is valid
    if (!descriptor || !descriptor->isValid())
        return;

    Descriptor _desc;
    _desc.id = descriptor->drugId();
    setDescriptor(_desc);
    //Set the drug identifiers
//    QStringList atcs;
//    atcs << descriptor->atc();
//    setAtc(atcs);
//    setId(descriptor->id());
    setPkModelId(descriptor->pkModelId());

    //Set the drug name
//    setName(descriptor->name());
//    setDomainName(descriptor->domainName());
//    setStudyName(descriptor->studyName());

    //Set the drug description
    setDescription(descriptor->description());
}

//Returns true if contains a PNA covariate
bool DrugModel::withPna() const
{
//    foreach (DrugVariate* c, _covariates)
//        if (c->getName().compare("pna") == 0)
//            return true;

    return false;
}

//Returns the drug descriptor
Descriptor DrugModel::descriptor() const
{
    return _descriptor;
}

//Sets the drug descriptor
void DrugModel::setDescriptor(const Descriptor &descriptor)
{
    _descriptor = descriptor;
}

//Returns the drug description
QString DrugModel::description() const
{
    return _descriptor.description;
}

//Sets the drug description
void DrugModel::setDescription(const QString &description)
{
    _descriptor.description = description;
}

//QList<DrugVariate*> Drug::covariates() const
//{
//    return _covariates;
//}

//Set the default covariates
//void Drug::setCovariates(const QList<DrugVariate*> &covariates)
//{
//    foreach (DrugVariate* _dv, covariates) {
//        DrugVariate* covariate = _dv;
//        _covariates.append(covariate);
//    }
//}


//ParameterSet* Drug::parameters() const
//{
//    return _parameters;
//}


//Set the population parameters
//void Drug::setParameters(ParameterSet* parameters)
//{
//    ParameterSet* set = ezechiel::core::CoreFactory::cloneEntity<ParameterSet>(_repository, 0, parameters);
//    set->setModel(modelId());
//    set->setType(POPULATION);

//    _parameters = set;
//}

QML_POINTERLIST_CLASS_IMPL(DrugModelList, DrugModel)
} // namespace core
} // namespace ezechiel
