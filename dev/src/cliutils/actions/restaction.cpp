//@@license@@

#include <QDomDocument>
#include <QNetworkReply>

#include "restaction.h"
#include "core/utils/xmlvalidator.h"
#include "core/definitionmanager.h"
#include "core/core.h"
#include "core/definitionmanager.h"
#include "apputils/src/drugmanager.h"
#include "core/pluginmanager.h"
#include "core/dal/drug/drug.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "apputils/src/appcore.h"
#include "admin/src/dal/measure.h"
#include "core/corefactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "core/corerepository.h"
#include "admin/src/adminfactory.h"
#include "climanager.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"
#include "rest/restrequestsclient.h"
#include "rest/network/networkaccessmanager.h"
#include "cli/rlutil.h"

#include "core/dal/drug/infusions.h"


namespace Tucuxi {
namespace cliutils {
//The action commands
const char *const RestAction::_ACTION_COMMAND = "rest";

//The curve name template
QString RestAction::name()
{
    return _ACTION_COMMAND;
}

QString RestAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Rest Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] %1 %2USERNAME %3PASSWORD COMMAND").arg(_ACTION_COMMAND, _CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Rest Command can be used to interact with a rest server. The COMMAND can be one of query_list {from} {to}, query_request {requestId} {patientId} {drugId}.") + "\n" + "\n" +
          "\n";

    return str;
}

RestAction::RestAction()
{

}

bool RestAction::run(const QStringList &args)
{
    QStringList _args = args;
    CliCommand _cmd(_args);
    QDomElement root;


    QTextStream informer(stdout);
    rlutil::setColor(rlutil::LIGHTBLUE);
    informer << "Performing rest action: " << _cmd.actionName << endl;
    rlutil::resetColor();

    RestRequestsClient * rctl = new RestRequestsClient(qApp);
    if (args.size() > 1) {
        QString cmdarg = _cmd.actionName;
        if (cmdarg == "query_list") {
            if (args.size() < 3) {
                LOG(QtWarningMsg, SYNTAXERROR, tr("Please set the 'from', 'to' and 'state' arguments."));
                return false;
            }
            QDateTime from = QDateTime::fromString(_cmd.actionArgs.first(),_dateTimeFormat);
            _cmd.actionArgs.pop_front();
            QDateTime to = QDateTime::fromString(_cmd.actionArgs.first(),_dateTimeFormat);
            _cmd.actionArgs.pop_front();
            _cmd.actionArgs.pop_front();
            if (from.isValid() && to.isValid()) {
                rctl->queryList(from, to, true);
                if (_cmd.actionArgs.size() > 1) {
                    _cmd.actionName = _cmd.actionArgs.first();
                    _cmd.actionArgs.pop_front();
                    connect(rctl, &RestRequestsClient::ackListFinished, this, [_cmd](){
                        CliCommand cmdl = _cmd;
                        bool _actionIsSuccess = CliManager::selectAction(&cmdl, true);
                        if (!_actionIsSuccess) {
                            LOG(QtFatalMsg, SYNTAXERROR, tr("Action could not be completed. Aborting."));
                        }
                    });
                }
            } else {
                LOG(QtWarningMsg, SYNTAXERROR, tr("Invalid date range requested, please set the 'from' and 'to' arguments."));
                return false;
            }
        } else if (cmdarg == "query_request") {
            bool ok = false;
            QString requestid = _cmd.actionArgs.first();
            _cmd.actionArgs.pop_front();
            int reqid = requestid.toInt(&ok);
            if (!ok) {
                LOG(QtWarningMsg, SYNTAXERROR, tr("Bad request id not parsed to int."));
                return false;
            }
            PartialRequest* request;
            ADMINREPO->getPartialRequestFromRequestId(requestid, request);
            if (!request) {
                LOG(QtWarningMsg, SYNTAXERROR, tr("Request from id %1 not found in local db.").arg(requestid));
                return false;
            }
//            QString patientid = _cmd.actionArgs.first();
//            _cmd.actionArgs.pop_front();
//            patientid.toInt(&ok);
//            if (!ok) {
//                LOG(QtWarningMsg, SYNTAXERROR, tr("Bad Patient id not parsed to int."));
//                return false;
//            }
//            QString drugid = _cmd.actionArgs.first();
//            _cmd.actionArgs.pop_front();
//            drugid.toInt(&ok);
//            if (!ok) {
//                LOG(QtWarningMsg, SYNTAXERROR, tr("Bad drug id not parsed to int."));
//                return false;
//            }

            rctl->queryRequest(requestid, request->patient()->externalId(), QString::number(request->drug()->id()));
            if (_cmd.actionArgs.size() > 1) {
                _cmd.actionName = _cmd.actionArgs.first();
                _cmd.actionArgs.pop_front();
                connect(rctl, &RestRequestsClient::ackRequestFinished, this, [_cmd](){
                    CliCommand cmdl = _cmd;
                    bool _actionIsSuccess = CliManager::selectAction(&cmdl, true);
                    if (!_actionIsSuccess) {
                        LOG(QtFatalMsg, SYNTAXERROR, tr("Action could not be completed. Aborting."));
                    }
                });
            }
        }
    } else {
        LOG(QtWarningMsg, SYNTAXERROR, tr("Not enough arguments."));
        return false;
    }


    return true;
}

} //namespace cliutils
} //namespace Tucuxi
