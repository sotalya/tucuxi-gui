#Use of Qt plugins
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)

#Please, no GUI
set(QT_DONT_USE_QTGUI 1)

#Please, use QtXmlPatterns
SET(QT_USE_QTXMLPATTERNS TRUE)

#Qt part
include(${LOCAL_QT_USE_FILE})

#Source dirs
include_directories(${CMAKE_CURRENT_BINARY_DIR} ./ ../ utils interfaces parsers)

#Sources
set(CORE_HDRS
   stdparamnames.h
   definitionmanager.h
   pluginmanager.h
   drugmanager.h
   druglister.h
   drugxmldescriptor.h
   drugxmltree.h
   config.h
   core_errors.h
   dbplugin.h
   security.h
   curvemodel.h
   distributor.h
   database.h
   core.h
   ../current_version.h
   interfaces/modelfactory.h
   interfaces/postenginefactory.h
   interfaces/postengine.h
   interfaces/reverseenginefactory.h
   interfaces/reverseengine.h
   interfaces/dbinterface.h
   interfaces/percentileengine.h
   interfaces/model.h
   interfaces/optionable.h
   interfaces/percentileenginefactory.h
   interfaces/coremessagehandler.h
   utils/version.h
   utils/ezxmlreader.h
   utils/duration.h
   utils/ezutils.h
   utils/medical.h
   utils/dataset.h
   utils/units.h
   utils/ezchoice.h
   utils/parameters.h
   utils/distribution.h
   utils/convert.h
   utils/dbobjects.h
   utils/drug.h
   utils/tag.h
   utils/xmlvalidator.h
   utils/xmltranslator.h
   utils/clinical.h
   parsers/domparser.h
   parsers/drugheadparser.h
   parsers/dispositionheadparser.h
   parsers/intakeheadparser.h
   parsers/comethheadparser.h
   parsers/drugparser.h
   parsers/dispositionparser.h
   parsers/comethparser.h
   parsers/intakeparser.h
   parsers/dosagesparser.h
   parsers/dosageparser.h
   parsers/intervalsparser.h
   parsers/targetparser.h
   parsers/targetsparser.h
   parsers/covariateparser.h
   parsers/covariatesparser.h
   parsers/errormodelparser.h
   parsers/parametersparser.h
   parsers/parameterparser.h
   parsers/correlationparser.h
   parsers/correlationsparser.h
   parsers/operationparser.h
   parsers/operationsparser.h
   parsers/commentparser.h
   parsers/historyparser.h
   parsers/authorparser.h
   parsers/revisionsparser.h
   parsers/dispositionsparser.h
   parsers/intakesparser.h
   parsers/comethsparser.h
   parsers/abstractmodelparser.h
   parsers/abstractheadparser.h
   parsers/modelparser.h
   parsers/headparser.h
)
set(CORE_SRCS 
   stdparamnames.cpp
   distributor.cpp
   core.cpp
   database.cpp
   dbplugin.cpp
   curvemodel.cpp
   config.cpp
   definitionmanager.cpp
   pluginmanager.cpp
   drugmanager.cpp
   druglister.cpp
   drugxmldescriptor.cpp
   drugxmltree.cpp
   interfaces/optionable.cpp
   utils/covariate.cpp
   utils/units.cpp
   utils/duration.cpp
   utils/dataset.cpp
   utils/convert.cpp
   utils/ezxmlreader.cpp
   utils/ezchoice.cpp
   utils/version.cpp
   utils/parameters.cpp
   utils/distribution.cpp
   utils/medical.cpp
   utils/ezutils.cpp
   utils/dbobjects.cpp
   utils/drug.cpp
   utils/xmlvalidator.cpp
   utils/xmltranslator.cpp
   utils/clinical.cpp
   parsers/domparser.cpp
   parsers/drugheadparser.cpp
   parsers/dispositionheadparser.cpp
   parsers/intakeheadparser.cpp
   parsers/comethheadparser.cpp
   parsers/drugparser.cpp
   parsers/dispositionparser.cpp
   parsers/comethparser.cpp
   parsers/intakeparser.cpp
   parsers/dosagesparser.cpp
   parsers/dosageparser.cpp
   parsers/intervalsparser.cpp
   parsers/targetparser.cpp
   parsers/targetsparser.cpp
   parsers/covariateparser.cpp
   parsers/covariatesparser.cpp
   parsers/errormodelparser.cpp
   parsers/parametersparser.cpp
   parsers/parameterparser.cpp
   parsers/correlationparser.cpp
   parsers/correlationsparser.cpp
   parsers/operationparser.cpp
   parsers/operationsparser.cpp
   parsers/commentparser.cpp
   parsers/historyparser.cpp
   parsers/authorparser.cpp
   parsers/revisionsparser.cpp
   parsers/dispositionsparser.cpp
   parsers/intakesparser.cpp
   parsers/comethsparser.cpp
   parsers/abstractmodelparser.cpp
   parsers/abstractheadparser.cpp
   parsers/modelparser.cpp
   parsers/headparser.cpp
)

#Find the commit version in GIT
find_program(GIT git)
if (GIT)
   execute_process (	COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} git rev-parse HEAD
                     OUTPUT_VARIABLE GIT_COMMIT
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
else (GIT)
   set(GIT_COMMIT ${PROJECT_VERSION})
   message(WARNING "No GIT program found, using the version instead")
endif (GIT)

#Modifying the core.h file to add definitions
set(APPDATA \"${INSTALL_PREFIX}${APPDATA_INSTALL}\")
set(BUILD_ID \"${GIT_COMMIT}\")
configure_file(core-cmake.h.in core-cmake.h)

#Preprocess the core_errors.h file to find the real tr()
add_custom_command(	OUTPUT ${CMAKE_BINARY_DIR}/core_errors.h
 							COMMAND gcc -E -CC -D PREPROC_ONLY ${CMAKE_CURRENT_LIST_DIR}/core_errors.h -o ${CMAKE_BINARY_DIR}/core_errors.h
							)
#Translations
file(GLOB CORE_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(CORE_QM core ${CMAKE_BINARY_DIR}/core_errors.h ${CORE_SRCS} ${CORE_TS})
#Moc precompiler
QT4_WRAP_CPP(CORE_MOC ${CORE_HDRS})
#Library creation
add_library(ezechiel-core ${CORE_SRCS} ${CORE_MOC} ${CORE_QM})
target_link_libraries(ezechiel-core ${QT_LIBRARIES})
#Install
if (${BUILD_SHARED_LIBS})
	install(TARGETS ezechiel-core DESTINATION ${LIBS_INSTALL})
endif (${BUILD_SHARED_LIBS})

#Definition XML file
install(FILES xml/definitions.xml DESTINATION ${APPDATA_INSTALL})

# Resources
QT4_ADD_RESOURCES(CORE_RCC resources/core.qrc)

