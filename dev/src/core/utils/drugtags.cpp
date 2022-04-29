//@@license@@

/* Declaration of the tags encountered in the drugs XML files.
 * Only the tags that needs a translated name or that are used
 * in more the one parser are declared. The macro below is used
 * as follows DECLARE_DRUG_TAG(Enum, Tag, Name for translation) */

//Model tags
DECLARE_DRUG_TAG(Root, model, Model)
DECLARE_DRUG_TAG(Drug, drug, Drug)

//Head tags
DECLARE_DRUG_TAG(Head, head, Description)
DECLARE_DRUG_TAG(ATC, atc, ATC)
DECLARE_DRUG_TAG(DrugId, drugid, Drug ID)
DECLARE_DRUG_TAG(DrugModelId, drugmodelid, Drug model ID)
DECLARE_DRUG_TAG(PkModelId, pkmodelid, Pk Model ID)
DECLARE_DRUG_TAG(DrugNames, drugnames, Drug translated names)
DECLARE_DRUG_TAG(DomainNames, domainnames, Domain translated names)
DECLARE_DRUG_TAG(StudyNames, studynames, Study translated names)
DECLARE_DRUG_TAG(Brands, brands, Brands)
DECLARE_DRUG_TAG(Brand, brand, Brand)

//ADME tags
DECLARE_DRUG_TAG(ADME, adme, ADME)
DECLARE_DRUG_TAG(Intake, intake, Intake)
DECLARE_DRUG_TAG(Distribution, distribution, Distribution)
DECLARE_DRUG_TAG(Elimination, elimination, Elimination)

//Half-life tags
DECLARE_DRUG_TAG(Halflife, halflife, Half-life)
DECLARE_DRUG_TAG(HalflifeUnit, unit, Unit)
DECLARE_DRUG_TAG(HalflifeValue, value, Value)
DECLARE_DRUG_TAG(HalflifeMultiplier, multiplier, Steady-state multiplier)

//Conversions tags
DECLARE_DRUG_TAG(Conversions, conversions, Conversions)
DECLARE_DRUG_TAG(Units, units, Default units)
DECLARE_DRUG_TAG(ConcUnit, concentration, Concentrations default unit)
DECLARE_DRUG_TAG(ResultUnit, result, Results default unit)
DECLARE_DRUG_TAG(Factors, factors, Conversions factors)
DECLARE_DRUG_TAG(ConcFact, concentration, Dose to concentration factor)
DECLARE_DRUG_TAG(ResultFact, result, Concentration to result factor)
DECLARE_DRUG_TAG(MolarMass, molarmass, Molar mass)

//Dosage tags
DECLARE_DRUG_TAG(Dosages, dosages, Dosages)
DECLARE_DRUG_TAG(Doses, doses, Doses)
DECLARE_DRUG_TAG(Dose, dose, Dose)
DECLARE_DRUG_TAG(Intervals, intervals, Intervals)
DECLARE_DRUG_TAG(Interval, interval, Interval)
DECLARE_DRUG_TAG(Infusions, infusions, Infusions)
DECLARE_DRUG_TAG(Infusion, infusion, Infusion)

//Target tags
DECLARE_DRUG_TAG(Targets, targets, Targets)
DECLARE_DRUG_TAG(Target, target, Target)
DECLARE_DRUG_TAG(TargetType, type, Target type)
DECLARE_DRUG_TAG(TargetConc, concentrations, Concentration targets)
DECLARE_DRUG_TAG(TargetTimes, times, Time targets)
DECLARE_DRUG_TAG(ConcMin, min, Minimum concentration)
DECLARE_DRUG_TAG(ConcMax, max, Maximum concentration)
DECLARE_DRUG_TAG(ConcBest, best, Targeted concentration)
DECLARE_DRUG_TAG(TimeMin, min, Minimum time)
DECLARE_DRUG_TAG(TimeMax, max, Maximum time)
DECLARE_DRUG_TAG(TimeBest, best, Targeted time)
DECLARE_DRUG_TAG(TargetValue, value, Value)
DECLARE_DRUG_TAG(TargetFormula, formula, Formula)

//Covariate tags
DECLARE_DRUG_TAG(Covariates, covariates, Covariates)
DECLARE_DRUG_TAG(Covariate, covariate, Covariate)
DECLARE_DRUG_TAG(CovariateId, id, Covariate ID)
DECLARE_DRUG_TAG(CovariateUnit, unit, Unit)
DECLARE_DRUG_TAG(CovariateType, type, Type)
DECLARE_DRUG_TAG(CovariateDefault, defaultvalue, Default value)

//Error model tags
DECLARE_DRUG_TAG(ErrorModel, errormodel, Error model)
DECLARE_DRUG_TAG(ErrorAdditive, additive, Additive error)
DECLARE_DRUG_TAG(ErrorProportional, proportional, Proportional error)

//Parameter tags
DECLARE_DRUG_TAG(Parameters, parameters, Parameters)
DECLARE_DRUG_TAG(Parameter, parameter, Parameter)
DECLARE_DRUG_TAG(ParameterId, id, Parameter ID)
DECLARE_DRUG_TAG(ParameterUnit, unit, Unit)
DECLARE_DRUG_TAG(ParameterValue, value, Value)
DECLARE_DRUG_TAG(ParameterMin, min, Minimum)
DECLARE_DRUG_TAG(ParameterMax, max, Maximum)
DECLARE_DRUG_TAG(ParameterStep, step, Step)
DECLARE_DRUG_TAG(BSV, bsv, BSV)
DECLARE_DRUG_TAG(BSVAdditive, additive, BSV Additive)
DECLARE_DRUG_TAG(BSVProportional, proportional, BSV Proportional)

//Correlation tags
DECLARE_DRUG_TAG(Correlations, correlations, Correlations)
DECLARE_DRUG_TAG(Correlation, correlation, Correlation)
DECLARE_DRUG_TAG(CorrelationParam1, param1, First parameter)
DECLARE_DRUG_TAG(CorrelationParam2, param2, Second parameter)
DECLARE_DRUG_TAG(CorrelationValue, value, Value)

//Operation tags
DECLARE_DRUG_TAG(Operations, operations, Operations)
DECLARE_DRUG_TAG(Operation, operation, Operation)
DECLARE_DRUG_TAG(OperationParam, parameter, Parameter)
DECLARE_DRUG_TAG(OperationFormula, formula, Formula)

//History tag
DECLARE_DRUG_TAG(History, history, History)
DECLARE_DRUG_TAG(Creator, creator, Creator)
DECLARE_DRUG_TAG(Revisions, revisions, revisions)
DECLARE_DRUG_TAG(Revision, revision, revision)
DECLARE_DRUG_TAG(AuthorName, name, Name)
DECLARE_DRUG_TAG(AuthorInstitution, institution, Institution)
DECLARE_DRUG_TAG(AuthorEmail, email, Email)
DECLARE_DRUG_TAG(AuthorDate, date, Date)

//Comment tags
DECLARE_DRUG_TAG(Comment, comment, Comment)
DECLARE_DRUG_TAG(Texts, texts, Comments)
DECLARE_DRUG_TAG(Biblio, biblio, Bibliography)
DECLARE_DRUG_TAG(BiblioItem, item, Comment)

//Shared tags
DECLARE_DRUG_TAG(Names, names, Translated names)
DECLARE_DRUG_TAG(Name, name, Translated name)
DECLARE_DRUG_TAG(Descriptions, descs, Translated descriptions)
DECLARE_DRUG_TAG(Description, desc, Translated description)
