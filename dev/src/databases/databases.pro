#EzeCHieL configuration
include(../tucuxi.pri)

TEMPLATE = subdirs

#Add your plugin name here
#SUBDIRS	+= localdb
# SUBDIRS	+= genericlocaldb
SUBDIRS += ephemeraldb
# SUBDIRS += mysqldb
