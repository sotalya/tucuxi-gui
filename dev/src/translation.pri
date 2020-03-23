
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

#Create the translation QM files
trans.input = TRANSLATIONS
trans.output = $${OUT_PWD}/../$${DIST_DIR}/$${TRANS_DIR}/${QMAKE_FILE_BASE}.qm
trans.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm $${OUT_PWD}/../$${DIST_DIR}/$${TRANS_DIR}/${QMAKE_FILE_BASE}.qm
trans.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += trans

#Install the translation QM files - Not needed now because we compile qm files in dist folder
#movetrans.path  = $${OUT_PWD}/../$${DIST_DIR}/$${TRANS_DIR}
#movetrans.files = $${OUT_PWD}/$${TRANS_DIR}/*.qm
#movetrans.CONFIG += no_check_exist
#INSTALLS += movetrans
