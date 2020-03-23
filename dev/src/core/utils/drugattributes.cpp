/* Declaration of the tags attributes encountered in the drugs XML files.
 * Only the attributes used in more the one parser are declared. */

//Model attributes
DECLARE_DRUG_ATTRIBUTE(Version, version)

//Dosage attributes
DECLARE_DRUG_ATTRIBUTE(DefaultDose,     default)
DECLARE_DRUG_ATTRIBUTE(DefaultInterval, default)
DECLARE_DRUG_ATTRIBUTE(DefaultInfusion, default)
DECLARE_DRUG_ATTRIBUTE(AnyDose,     any)
DECLARE_DRUG_ATTRIBUTE(AnyInterval, any)
DECLARE_DRUG_ATTRIBUTE(AnyInfusion, any)

//Shared attributes
DECLARE_DRUG_ATTRIBUTE(Language, lang)
DECLARE_DRUG_ATTRIBUTE(Unit, unit)
