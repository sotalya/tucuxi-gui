include (../tucuxi.pri)

TARGET      = tucuxi-gui-core
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}
# DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

# To solve issues with namespaces
# QMAKE_CXXFLAGS += -fpermissive

#Removing the QtGui module (the core must NOT use any gui element)
QT -= gui

DEPENDPATH  += ./ interfaces utils parsers dal

INCLUDEPATH += ..

HEADERS	+= \
    ../current_version.h \
    dal/drug/parameters.h \
    dal/drug/drug.h \
    dal/drug/adme.h \
    dal/drug/halflife.h \
    dal/drug/concentrations.h \
    dal/drug/results.h \
    dal/drug/doses.h \
    dal/drug/intervals.h \
    dal/drug/infusions.h \
    dal/drug/target.h \
    dal/drug/drugvariate.h \
    dal/drug/bsv.h \
    dal/drug/errormodel.h \
    dal/uncastedvalue.h \
    dal/covariate.h \
    dal/genericdbobjects.h \
    dal/entity.h \
    dal/amount.h \
    dal/dosage.h \
    dal/percentile.h \
    dal/residual.h \
    dal/drugtreatment.h \
    dal/drugresponseanalysis.h \
    dal/prediction.h \
    dal/predictionspec.h \
    dal/corepatient.h \
    dal/coremeasure.h \
    interfaces/genericdatabase.h \
    interfaces/optionable.h \
    interfaces/plugin.h \
    interfaces/pluginfactory.h \
    interfaces/processinginterface.h \
    utils/convert.h \
    utils/dataset.h \
    utils/dbobjects.h \
    utils/distribution.h \
    utils/duration.h \
    utils/errorenums.h \
    utils/ezutils.h \
    utils/ezchoice.h \
    utils/ezxmlreader.h \
    utils/genericuser.h \
    utils/tag.h \
    utils/units.h \
    utils/version.h \
    utils/xmlvalidator.h \
    utils/xmltranslator.h \
    core.h \
    definitionmanager.h \
    distributor.h \
    pluginmanager.h \
    stdparamnames.h \
    settings.h \
    dao/genericdao.h \
    interfaces/drugmodelengine.h \
    drugxmldescriptor.h \
    interfaces/abstractrepository.h \
    corefactory.h \
    corerepository.h \
    interfaces/processingtraits.h \
    utils/logging.h \
    errors_core.h \
    interfaces/database.h \
    utils/autoproperty.h \
    dal/drug/operation.h \
    dal/factory.h \
    dal/drug/metadata.h \
    dal/drug/editors.h \
    dal/drug/references.h \
    dal/drug/operableamount.h \
    dal/chartdata.h \
    dal/predictionresult.h \
    dal/predictive.h \
    dal/predictiondata.h \
    dal/percentilepair.h \
    dal/adjustment.h \
    utils/connect.h \
    dal/variability.h \
    dal/drug/translatablestring.h \
    dal/drug/standardtreatment.h \
    processingfactory.h \
    errors_processing.h

SOURCES	+= \
    dal/drug/target.cpp \
    dal/drug/drug.cpp \
    dal/drug/parameters.cpp \
    dal/drug/drugvariate.cpp \
    dal/uncastedvalue.cpp \
    dal/covariate.cpp \
    dal/amount.cpp \
    dal/dosage.cpp \
    dal/percentile.cpp \
    dal/residual.cpp \
    dal/drugresponseanalysis.cpp \
    dal/prediction.cpp \
    dal/drugtreatment.cpp \
    dal/coremeasure.cpp \
    dal/corepatient.cpp \
    dal/entity.cpp \
    interfaces/optionable.cpp \
    utils/convert.cpp \
    utils/dataset.cpp \
    utils/dbobjects.cpp \
    utils/distribution.cpp \
#    utils/drugattributes.cpp \
#    utils/drugtags.cpp \
    utils/duration.cpp \
    utils/ezchoice.cpp \
    utils/ezutils.cpp \
    utils/ezxmlreader.cpp \
    utils/units.cpp \
    utils/version.cpp \
    utils/xmltranslator.cpp \
    utils/xmlvalidator.cpp \
    core.cpp \
#    core_errors.cpp \
    definitionmanager.cpp \
    distributor.cpp \
    pluginmanager.cpp \
    stdparamnames.cpp \
    settings.cpp \
    corefactory.cpp \
    drugxmldescriptor.cpp \
    dal/drug/adme.cpp \
    dal/drug/halflife.cpp \
    dal/drug/concentrations.cpp \
    dal/drug/doses.cpp \
    dal/drug/intervals.cpp \
    dal/drug/infusions.cpp \
    dal/drug/results.cpp \
    dal/drug/bsv.cpp \
    dal/predictionspec.cpp \
    dal/drug/errormodel.cpp \
    dal/drug/operation.cpp \
    dal/drug/metadata.cpp \
    dal/drug/editors.cpp \
    dal/drug/references.cpp \
    dal/drug/operableamount.cpp \
    dal/chartdata.cpp \
    dal/predictionresult.cpp \
    dal/predictive.cpp \
    dal/predictiondata.cpp \
    dal/percentilepair.cpp \
    dal/adjustment.cpp \
    dal/variability.cpp \
    dal/drug/translatablestring.cpp \
    utils/logging.cpp \
    dal/drug/standardtreatment.cpp \
    processingfactory.cpp

OTHER_FILES += models/tucuxi_core.qmodel

RESOURCES += \
    resources/core.qrc

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/core_fr.ts

#Translation configuration
include(../translation.pri)

#Third party libraries headers
include(../boost.pri)

DISTFILES += \
    models/tucuxi_core2.qmodel
