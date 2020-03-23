include (../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.svmpred

#Plugin dest
PLUGIN_DEST = ../../dist/plugins

#Database files
DB_TRAIN_FILE = complete_train.txt
DB_TEST_FILE  = complete_test.txt

#Models ID
DEFINES += SVMPRED_ID=\\\"$${PLUGIN_ID}\\\" \
           SVMPRED1_ID=\\\"$${PLUGIN_ID}.svm1\\\" \
           DB_TRAIN_PATH=\\\"plugins/svm/$${DB_TRAIN_FILE}\\\" \
           DB_TEST_PATH=\\\"plugins/svm/$${DB_TEST_FILE}\\\"

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

QMAKE_CFLAGS += -std=c99

TARGET      = svm_predict
TEMPLATE    = lib
DESTDIR     = $${PLUGIN_DEST}

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils

HEADERS	+= svm_predict.h

SOURCES += svm_predict.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_SRC}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core
for(dep, DEPENDANCIES) {
    LIBNAME = ../../dist/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

include (../../gsl.pri)

#Install the plugin database files
DB_DEST = $${PLUGIN_DEST}/svm
svm_train.path  = $${DB_DEST}
svm_train.files = db/$${DB_TRAIN_FILE}
svm_test.path  = $${DB_DEST}
svm_test.files = db/$${DB_TEST_FILE}
INSTALLS += svm_train svm_test

include(../../translation.pri)
