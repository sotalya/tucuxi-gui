set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(CORE_HEADER_FILES
    ../current_version.h

    dal/drug/adme.h
    dal/drug/bsv.h
    dal/drug/concentrations.h
    dal/drug/doses.h
    dal/drug/drug.h
    dal/drug/drugvariate.h
    dal/drug/editors.h
    dal/drug/errormodel.h
    dal/drug/halflife.h
    dal/drug/infusions.h
    dal/drug/intervals.h
    dal/drug/metadata.h
    dal/drug/operableamount.h
    dal/drug/operation.h
    dal/drug/parameters.h
    dal/drug/references.h
    dal/drug/results.h
    dal/drug/standardtreatment.h
    dal/drug/target.h
    dal/drug/translatablestring.h
    
    dal/adjustment.h
    dal/amount.h
    dal/chartdata.h
    dal/coremeasure.h
    dal/corepatient.h
    dal/covariate.h
    dal/dosage.h
    dal/drugresponseanalysis.h
    dal/drugtreatment.h
    dal/entity.h
    dal/factory.h
    dal/genericdbobjects.h
    dal/percentile.h
    dal/percentilepair.h
    dal/prediction.h
    dal/predictiondata.h
    dal/predictionresult.h
    dal/predictionspec.h
    dal/predictive.h
    dal/residual.h
    dal/uncastedvalue.h
    dal/variability.h
    
    dao/genericdao.h
    
    interfaces/abstractrepository.h
    interfaces/database.h
    interfaces/drugmodelengine.h
    interfaces/genericdatabase.h
    interfaces/optionable.h
    interfaces/plugin.h
    interfaces/pluginfactory.h
    interfaces/processinginterface.h
    interfaces/processingtraits.h
    
    utils/autoproperty.h
    utils/connect.h
    utils/convert.h
    utils/dataset.h
    utils/dbobjects.h
    utils/distribution.h
    utils/duration.h
    utils/errorenums.h
    utils/ezchoice.h
    utils/ezutils.h
    utils/ezxmlreader.h
    utils/genericuser.h
    utils/logging.h
    utils/tag.h
    utils/units.h
    utils/version.h
    utils/xmltranslator.h
    utils/xmlvalidator.h
    
    core.h
    corefactory.h
    corerepository.h
    definitionmanager.h
    distributor.h
    drugxmldescriptor.h
    errors_core.h
    errors_processing.h
    pluginmanager.h
    processingfactory.h
    settings.h
    stdparamnames.h
)

set(CORE_SOURCE_FILES
    dal/drug/adme.cpp
    dal/drug/bsv.cpp
    dal/drug/concentrations.cpp
    dal/drug/doses.cpp
    dal/drug/drug.cpp
    dal/drug/drugvariate.cpp
    dal/drug/editors.cpp
    dal/drug/errormodel.cpp
    dal/drug/halflife.cpp
    dal/drug/infusions.cpp
    dal/drug/intervals.cpp
    dal/drug/metadata.cpp
    dal/drug/operableamount.cpp
    dal/drug/operation.cpp
    dal/drug/parameters.cpp
    dal/drug/references.cpp
    dal/drug/results.cpp
    dal/drug/standardtreatment.cpp
    dal/drug/target.cpp
    dal/drug/translatablestring.cpp
    
    dal/adjustment.cpp
    dal/amount.cpp
    dal/chartdata.cpp
    dal/coremeasure.cpp
    dal/corepatient.cpp
    dal/covariate.cpp
    dal/dosage.cpp
    dal/drugresponseanalysis.cpp
    dal/drugtreatment.cpp
    dal/entity.cpp
    dal/percentile.cpp
    dal/percentilepair.cpp
    dal/prediction.cpp
    dal/predictiondata.cpp
    dal/predictionresult.cpp
    dal/predictionspec.cpp
    dal/predictive.cpp
    dal/residual.cpp
    dal/uncastedvalue.cpp
    dal/variability.cpp
    
    interfaces/optionable.cpp
    
    utils/convert.cpp
    utils/dataset.cpp
    utils/dbobjects.cpp
    utils/distribution.cpp
    utils/duration.cpp
#    utils/drugattributes.cpp
#    utils/drugtags.cpp
    utils/ezchoice.cpp
    utils/ezutils.cpp
    utils/ezxmlreader.cpp
    utils/logging.cpp
    utils/units.cpp
    utils/version.cpp
    utils/xmltranslator.cpp
    utils/xmlvalidator.cpp
    
    core.cpp
    corefactory.cpp
#    core_errors.cpp
    definitionmanager.cpp
    distributor.cpp
    drugxmldescriptor.cpp
    pluginmanager.cpp
    processingfactory.cpp
    settings.cpp
    stdparamnames.cpp
)

set(CORE_OTHER_FILES
    models/tucuxi_core.qmodel
)

set(CORE_RESOURCES_FILES
    resources/core.qrc
)

add_library(core 
                STATIC
                    ${CORE_HEADER_FILES}
                    ${CORE_SOURCE_FILES}
                    ${CORE_OTHER_FILES}
                    ${CORE_RESOURCES_FILES}
)

target_include_directories(core
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/dal
                    ${CMAKE_CURRENT_SOURCE_DIR}/dal/drugs
                    ${CMAKE_CURRENT_SOURCE_DIR}/dao
                    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
                    ${CMAKE_CURRENT_SOURCE_DIR}/utils
                    ${CMAKE_CURRENT_SOURCE_DIR}/xml
                PRIVATE
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

# Translations
set(translation_files
      trans/core_fr.ts
)
qt_add_translations(core TS_FILES ${translation_files})

target_link_libraries(core
                PRIVATE
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS core
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
