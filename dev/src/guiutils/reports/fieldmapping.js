var graphImageId = 1;

function enumerate(list, callback)
{
    list.size(function(size) {
        for (var i = 0; i < size; ++i) {
            list.objat(i, function(obj) {
                callback(obj);
            });
        }
    });
}

function getInfos(channel)
{
    var fields = {}

    var interp = channel.objects.interpretation;
    var treat = interp.drugResponseAnalysis.treatment;

    drugvariates = channel.objects.info.drugvariates;
    parameters = channel.objects.info.parameters;
    measures = channel.objects.info.measures;
    patientvariates = channel.objects.info.patientvariates;
    dosages = channel.objects.info.dosages;
    aPrioriParams = JSON.parse(channel.objects.info.aPrioriParameters);
    aPosterioriParams = JSON.parse(channel.objects.info.aPosterioriParameters);

    var date = new Date();    
    fields["creationDate"] = date;

    fields["patientId"] = treat.patient.externalId;
    fields["patientFirstName"] = treat.patient.person.firstname;
    fields["patientLastName"] = treat.patient.person.name;
    fields["patientAddress"] = treat.patient.person.location.address;    
    fields["patientDOB"] = treat.patient.person.birthday;
    var gender = "???";
    if (treat.patient.person.gender === 1) gender = "Male";
    if (treat.patient.person.gender === 0) gender = "Female";
    fields["patientGender"] = gender;

    fields["patientWeight"] = "-";
    fields["patientAge"] = "-";
    fields["patientGist"] = "-";

    fields["drugName"] = interp.drugResponseAnalysis.drugModel.activeSubstance.substanceId;
    
    fields["analystFirstName"] = interp.analyst.person.firstname;
    fields["analystLastName"] = interp.analyst.person.name;
    fields["instituteName"] = interp.analyst.institute.name;
    fields["instituteAddresse"] = interp.analyst.institute.location.address;
    
    fields["expectedness"] = interp.analysis.expectedness;
    fields["warning"] = interp.analysis.warning;
    fields["remonitoring"] = interp.analysis.remonitoring;
    fields["prediction"] = interp.analysis.prediction;
    fields["suitability"] = interp.analysis.suitability;
    
    fields["lastDosage"] = channel.objects.info.lastDosage;
    fields["lastDosageDate"] = channel.objects.info.lastDosageDate;
    fields["proposedPosology"] = channel.objects.info.posology.replace("\r\n", "<br>");
    fields["nextControl"] = channel.objects.info.nextControl;
    fields["validationDate"] = channel.objects.info.validationDate;

    fields["steadyStateAUC24"] = channel.objects.info.steadyStateAUC24;
    fields["steadyStateMin"] = channel.objects.info.steadyStateMin;
    fields["steadyStateMax"] = channel.objects.info.steadyStateMax;

    // Set the source of the graph's image
    graphImageId = graphImageId+1;
    fields["imageUrl"] = channel.objects.info.graphPath + "?" + graphImageId;

    return fields;
}


function getMeasures(channel, callback)
{
    var interp = channel.objects.interpretation;
    var measures = interp.drugResponseAnalysis.treatment.measures;
    if (measures) {
        enumerate(measures, function(obj) {
            var m = Object.create(null);
            m.sampleId = obj.sampleID;
            m.sampleDate = obj.moment;
            m.sampleValue = obj.concentration.dbvalue + obj.concentration.unitstring;
            callback(m);
        });
    }
}

function getParameters(channel, callback) 
{

    // I don't know why, but simply using aPrioriParams and aPosterioriParams does not work.
    // Maybe because it is not related to the channel.
    // So I am parsing again here.
    popParams1 = JSON.parse(channel.objects.info.popParameters);
    aPrioriParams1 = JSON.parse(channel.objects.info.aPrioriParameters);
    aPosterioriParams1 = JSON.parse(channel.objects.info.aPosterioriParameters);

    // Be careful that parameters Id should be the same.
    // There is an issue with Tucucore, as CL has been transformed to Cl...
    var interp = channel.objects.interpretation;
    var parameters = interp.drugResponseAnalysis.drugModel.parameters.parameters;
    if (parameters) {
        enumerate(parameters, function(obj) {
            var param = Object.create(null);
            param.name = obj.name;
            param.value = obj.quantity.dbvalue;
            param.pop = (obj.name in popParams1 ? popParams1[obj.name] : "");
            param.unit = obj.quantity.unitstring;
            // Modification related to the comment at the beginning of the function
//            param.apriori = (obj.name in aPrioriParams ? aPrioriParams[obj.name] : "");
//            param.aposteriori = (obj.name in aPosterioriParams ? aPosterioriParams[obj.name] : "");
            param.apriori = (obj.name in aPrioriParams1 ? aPrioriParams1[obj.name] : "");
            param.aposteriori = (obj.name in aPosterioriParams1 ? aPosterioriParams1[obj.name] : "");
            callback(param);
        });
    }
}

function getDrugVariates(channel, callback)
{
    var interp = channel.objects.interpretation;
    var covariates = interp.drugResponseAnalysis.drugModel.covariates;
    if (covariates) {
        enumerate(covariates, function(obj) {
            obj.visualNameTranslation.value("en", function(name) {
                var covariate = Object.create(null);
                covariate.id = obj.covariateId;
                covariate.name = name
                covariate.value = obj.quantity.dbvalue + " " + obj.quantity.unitstring;
                covariate.description = obj.description;
                callback(covariate);
            });
        });
    }
}


function hasDefaultVariates(channel, callback)
{
    return true;
    var interp = channel.objects.interpretation;
    var covariates = interp.drugResponseAnalysis.drugModel.covariates;
    var returnValue = false;
    if (covariates) {
        enumerate(covariates, function(obj) {
            var found = false;

            var patientCovariates = interp.drugResponseAnalysis.treatment.covariates;
            if (patientCovariates) {
                enumerate(patientCovariates, function(obj) {
                    found = true;
                });
            }

            if (!found) {
                returnValue = true;
            }

        });
    }

    returnValue = true;
    return returnValue;
}

function hasPatientVariates(channel)
{
    return true;
    var interp = channel.objects.interpretation;
    var covariates = interp.drugResponseAnalysis.treatment.covariates;
    var atLeastOne = false;
    if (covariates) {
        atLeastOne = covariates.size() > 0;
    }

    return atLeastOne;
}

function getPatientVariates(channel, callback)
{
    var interp = channel.objects.interpretation;    
    var covariates = interp.drugResponseAnalysis.treatment.covariates;
    if (covariates) {
        enumerate(covariates, function(obj) {
            var covariate = Object.create(null);
            covariate.id = obj.covariateId;
            covariate.name = obj.name;
            if (covariate.id === "sex")  {
                covariate.value = obj.quantity.dbvalue;
            }
            else {
                covariate.value = obj.quantity.dbvalue + " " + obj.quantity.unitstring;
            }
            callback(covariate);
        });
    }
}
