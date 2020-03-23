#ifndef IDENTIFIERS_H
#define IDENTIFIERS_H

namespace ID {

    //General identifiers
    const char *const Title       = "title";
    const char *const Reference   = "reference";
    const char *const Language    = "language";
    const char *const Creation    = "creation";
    const char *const Publication = "publication";

    //Treatment identifiers
    namespace Treatment {
        const char *const Id   = "treatment.id";
        const char *const Name = "treatment.name";
        const char *const Drug = "treatment.drug";
        const char *const Type = "treatment.type";
    }

    //Concentrations identifiers
    namespace Concentrations {
        const char *const Count = "concentrations.count";

        namespace Concentration {
            const char *const Value = "concentrations.concentration%1.value";
            const char *const Unit  = "concentrations.concentration%1.unit";
            const char *const Date  = "concentrations.concentration%1.date";
        }
    }

    //Samples identifiers
    namespace Sample {
        const char *const Id       = "sample.id";
        const char *const External = "sample.external";
        const char *const Value    = "sample.value";
        const char *const Unit     = "sample.unit";
        const char *const Drug     = "sample.drug";
        const char *const Date     = "sample.date";
        const char *const Arrival  = "sample.arrival";
    }

    //Dosages identifiers
    namespace Dosage {
        const char *const Id     = "dosage.id";
        const char *const Intake = "dosage.intake";

        namespace Take {
            const char *const First = "dosage.take.first";
            const char *const Last  = "dosage.take.last";
        }

        namespace Dose {
            const char *const Value = "dosage.dose.value";
            const char *const Unit  = "dosage.dose.unit";
        }

        namespace Interval {
            const char *const Value = "dosage.interval.value";
            const char *const Unit  = "dosage.interval.unit";
        }

        namespace Infusion {
            const char *const Value = "dosage.infusion.value";
            const char *const Unit  = "dosage.infusion.unit";
        }
    }

    //Covariates identifiers
    namespace Covariates {
        const char *const Count = "covariates.count";

        namespace Covariate {
            const char *const Id          = "covariates.covariate%1.id";
            const char *const Identifier  = "covariates.covariate%1.identifier";
            const char *const Name        = "covariates.covariate%1.name";
            const char *const Description = "covariates.covariate%1.description";
            const char *const Value       = "covariates.covariate%1.value";
            const char *const Unit        = "covariates.covariate%1.unit";
            const char *const Type        = "covariates.covariate%1.type";
        }
    }

    namespace DrugVariates {
        const char *const Count = "drugvariates.count";

        namespace DrugVariate {
            const char *const Id          = "drugvariates.drugvariate%1.id";
            const char *const Identifier  = "drugvariates.drugvariate%1.identifier";
            const char *const Name        = "drugvariates.drugvariate%1.name";
            const char *const Description = "drugvariates.drugvariate%1.description";
            const char *const Value       = "drugvariates.drugvariate%1.value";
            const char *const Unit        = "drugvariates.drugvariate%1.unit";
            const char *const Type        = "drugvariates.drugvariate%1.type";
        }

    }

    //Targets identifiers
    namespace Targets {
        const char *const Count = "targets.count";

        namespace Target {
            const char *const Type  = "targets.target%1.type";
            const char *const Count = "targets.target%1.count";

            namespace Value {
                const char *const Unit = "targets.target%1.value%2.unit";
                const char *const Min  = "targets.target%1.value%2.min";
                const char *const Max  = "targets.target%1.value%2.max";
                const char *const Best = "targets.target%1.value%2.best";
            }
        }
    }

    //Parameters identifiers
    namespace Parameters {
        const char *const Count        = "parameters.count";
        const char *const Additive     = "parameters.additive";
        const char *const Proportional = "parameters.proportional";

        namespace Parameter {
            const char *const Identifier   = "parameters.parameter%1.identifier";
            const char *const Population   = "parameters.parameter%1.population";
            const char *const Reference    = "parameters.parameter%1.reference";
            const char *const Patient      = "parameters.parameter%1.patient";
            const char *const Unit         = "parameters.parameter%1.unit";
            const char *const Additive     = "parameters.parameter%1.additive";
            const char *const Proportional = "parameters.parameter%1.proportional";
        }
    }

    //Chart identifiers
    namespace Chart {
        const char *const Name     = "chart.name";
        const char *const Image    = "chart.image";
        const char *const Legend   = "chart.legend";
        const char *const Comments = "chart.comments";
    }

    //Patient identifiers
    namespace Patient {
        const char *const Id        = "patient.id";
        const char *const External  = "patient.external";
        const char *const Room      = "patient.room";
        const char *const Approval  = "patient.approval";
        const char *const Firstname = "patient.firstname";
        const char *const Lastname  = "patient.lastname";
        const char *const Gender    = "patient.gender";
        const char *const Birthdate = "patient.birthdate";
        const char *const Address   = "patient.address";
        const char *const Postcode  = "patient.postcode";
        const char *const City      = "patient.city";
        const char *const Country   = "patient.country";
        const char *const Phone     = "patient.phone";
        const char *const Fax       = "patient.fax";
        const char *const Email     = "patient.email";
    }

    //Institute identifiers
    namespace Institute {
        const char *const Id        = "institute.id";
        const char *const External  = "institute.external";
        const char *const Name      = "institute.name";
        const char *const Logo      = "institute.logo";
        const char *const Address   = "institute.address";
        const char *const Postcode  = "institute.postcode";
        const char *const City      = "institute.city";
        const char *const Country   = "institute.country";
        const char *const Phone     = "institute.phone";
        const char *const Fax       = "institute.fax";
        const char *const Email     = "institute.email";
        const char *const Website   = "institute.website";
    }

    //Analyst identifiers
    namespace Analyst {
        const char *const Id        = "analyst.id";
        const char *const External  = "analyst.external";
        const char *const Title     = "analyst.title";
        const char *const Firstname = "analyst.firstname";
        const char *const Lastname  = "analyst.lastname";
        const char *const Gender    = "analyst.gender";
        const char *const Birthdate = "analyst.birthdate";
        const char *const Institute = "analyst.institute";
        const char *const Address   = "analyst.address";
        const char *const Postcode  = "analyst.postcode";
        const char *const City      = "analyst.city";
        const char *const Country   = "analyst.country";
        const char *const Phone     = "analyst.phone";
        const char *const Fax       = "analyst.fax";
        const char *const Email     = "analyst.email";
    }

    //Recipient identifiers
    namespace Recipient {
        const char *const Id        = "recipient.id";
        const char *const External  = "recipient.external";
        const char *const Title     = "recipient.title";
        const char *const Firstname = "recipient.firstname";
        const char *const Lastname  = "recipient.lastname";
        const char *const Gender    = "recipient.gender";
        const char *const Birthdate = "recipient.birthdate";
        const char *const Institute = "recipient.institute";
        const char *const Address   = "recipient.address";
        const char *const Postcode  = "recipient.postcode";
        const char *const City      = "recipient.city";
        const char *const Country   = "recipient.country";
        const char *const Phone     = "recipient.phone";
        const char *const Fax       = "recipient.fax";
        const char *const Email     = "recipient.email";
    }

    //Team identifiers
    namespace Team {
        const char *const Count   = "team.count";
        const char *const Name    = "team.name";
        const char *const Phone   = "team.phone";
        const char *const Fax     = "team.fax";
        const char *const Email   = "team.email";
        const char *const Website = "team.website";

        namespace Member {
            const char *const Id        = "team.member%1.id";
            const char *const External  = "team.member%1.external";
            const char *const Title     = "team.member%1.title";
            const char *const Firstname = "team.member%1.firstname";
            const char *const Lastname  = "team.member%1.lastname";
            const char *const Gender    = "team.member%1.gender";
            const char *const Birthdate = "team.member%1.birthdate";
            const char *const Institute = "team.member%1.institute";
            const char *const Address   = "team.member%1.address";
            const char *const Postcode  = "team.member%1.postcode";
            const char *const City      = "team.member%1.city";
            const char *const Country   = "team.member%1.country";
            const char *const Phone     = "team.member%1.phone";
            const char *const Fax       = "team.member%1.fax";
            const char *const Email     = "team.member%1.email";
        }
    }
}

#endif // IDENTIFIERS_H
