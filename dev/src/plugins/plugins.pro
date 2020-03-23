#EzeCHieL configuration
include(../tucuxi.pri)

TEMPLATE = subdirs

#Add your plugin name here
SUBDIRS	+= newton gradient csp ga analytic rk4 taylor monte_carlo bayesian foce testcurves
# SUBDIRS += database repository
