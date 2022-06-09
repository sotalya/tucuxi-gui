//@@license@@

#include <QList>

#include "sniffaction.h"

#include "listaction.h"

#include "core/pluginmanager.h"
#include "apputils/src/drugmanager.h"
#include "core/utils/ezutils.h"
#include "apputils/src/appcore.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"
#include "rest/restrequestsclient.h"
#include "rest/network/networkaccessmanager.h"
#include "cli/rlutil.h"
#include "admin/src/dal/partialrequest.h"
#include "core/dal/drug/drug.h"
#include "admin/src/stdadminrepository.h"

using namespace ezechiel::GuiCore;

namespace ezechiel {
namespace cliutils {

QString SniffAction::name()
{
    return "sniff";
}

QString SniffAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Sniffing Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] sniff") + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Sniff Command sniffs the requests the server can offer and stores them locally.") + "\n";

    return str;
}

//Depanding on the first parameter, fetch the wanted data
bool SniffAction::run(const QStringList &args)
{


    QTextStream informer(stdout);
    rlutil::setColor(rlutil::LIGHTBLUE);
    informer << "Performing sniff action" << endl;
    rlutil::resetColor();

    rctl = new RestRequestsClient(qApp);
    QDateTime from = QDateTime::currentDateTime().addMonths(-12);
    QDateTime to = QDateTime::currentDateTime();
    rctl->queryList(from, to, true);


    connect(rctl, &RestRequestsClient::ackRequestFinished, this, [this](){


        std::cout << "Yepee. Got a request" << std::endl;
        if (!requestsList.isEmpty()) {
            PartialRequest *request = requestsList.at(0);
            requestsList.removeAt(0);
            this->rctl->queryRequest(request->requestId(), request->patient()->externalId(), request->drug()->getSubstanceId());
        }
    });

    connect(rctl, &RestRequestsClient::ackListFinished, this, [this](){
        std::cout << "Yepee. Got a list" << std::endl;
        ADMINREPO->getPartialRequestsList(requestsList);

        if (!requestsList.isEmpty()) {
            PartialRequest *request = requestsList.at(0);
            requestsList.removeAt(0);
            ActiveSubstance *substance = request->drug();

            //this->rctl->queryRequest(request->requestId(),"","");
            this->rctl->queryRequest(request->requestId(), request->patient()->externalId(), request->drug()->getSubstanceId());
        }
    });


/*} else if (cmdarg == "query_request") {
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
*/

    return true;
}

void SniffAction::listOptions(const QString &arg)
{
}

} //namespace cliutils
} //namespace ezechiel
