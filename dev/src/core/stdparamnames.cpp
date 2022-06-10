//@@license@@

#include "stdparamnames.h"


namespace Tucuxi {
namespace Gui {
namespace Core {



QList<AbstractStdParamNameHandler*> StdParamNames::handlersList;

void StdParamNames::addStdParamNameHandler(AbstractStdParamNameHandler *handler)
{
    handlersList << handler;
}


QString StdParamNames::getName(const QString &id)
{
    if (id.compare("CL")==0)
        return getNameCL();
    if (id.compare("V")==0)
        return getNameV();
    if (id.compare("KA")==0)
        return getNameKA();
    if (id.compare("F")==0)
        return getNameF();
    if (id.compare("V1")==0)
        return getNameV1();
    if (id.compare("V2")==0)
        return getNameV2();
    if (id.compare("V3")==0)
        return getNameV3();
    if (id.compare("Q")==0)
        return getNameQ();
    if (id.compare("Q2")==0)
        return getNameQ2();
    if (id.compare("Q3")==0)
        return getNameQ3();
    if (id.compare("VM")==0)
        return getNameVM();
    if (id.compare("KM")==0)
        return getNameKM();
    if (id.compare("K12")==0)
        return getNameK12();
    if (id.compare("K21")==0)
        return getNameK21();
    if (id.compare("K13")==0)
        return getNameK13();
    if (id.compare("K31")==0)
        return getNameK31();
    if (id.compare("Ke")==0)
        return getNameKe();
    if (id.compare("A")==0)
        return getNameA();
    if (id.compare("Ga1")==0)
        return getNameGA1();
    if (id.compare("Gb1")==0)
        return getNameGB1();
    if (id.compare("Ga2")==0)
        return getDescGA2();
    if (id.compare("Gb2")==0)
        return getDescGB2();
    if (id.compare("R")==0)
        return getDescR();
    for(int i=0;i<handlersList.size();i++)
    {
        QString name = handlersList[i]->getName(id);
        if (!name.isEmpty())
            return name;
    }
    return "";
}

QString StdParamNames::getDesc(const QString &id)
{
    if (id.compare("CL")==0)
        return getDescCL();
    if (id.compare("V")==0)
        return getDescV();
    if (id.compare("KA")==0)
        return getDescKA();
    if (id.compare("F")==0)
        return getDescF();
    if (id.compare("V1")==0)
        return getDescV1();
    if (id.compare("V2")==0)
        return getDescV2();
    if (id.compare("V3")==0)
        return getDescV3();
    if (id.compare("Q")==0)
        return getDescQ();
    if (id.compare("Q2")==0)
        return getDescQ2();
    if (id.compare("Q3")==0)
        return getDescQ3();
    if (id.compare("VM")==0)
        return getDescVM();
    if (id.compare("KM")==0)
        return getDescKM();
    if (id.compare("K12")==0)
        return getDescK12();
    if (id.compare("K21")==0)
        return getDescK21();
    if (id.compare("K13")==0)
        return getDescK13();
    if (id.compare("K31")==0)
        return getDescK31();
    if (id.compare("Ke")==0)
        return getDescKe();
    if (id.compare("A")==0)
        return getDescA();
    if (id.compare("Ga1")==0)
        return getDescGA1();
    if (id.compare("Gb1")==0)
        return getDescGB1();
    if (id.compare("Ga2")==0)
        return getDescGA2();
    if (id.compare("Gb2")==0)
        return getDescGB2();
    if (id.compare("R")==0)
        return getDescR();
    for(int i=0;i<handlersList.size();i++)
    {
        QString dest = handlersList[i]->getDesc(id);
        if (!dest.isEmpty())
            return dest;
    }
    return "";
}


QString StdParamNames::getNameCL(){return tr("Clearance");}
QString StdParamNames::getDescCL(){return tr("Ratio of the drug's elimination rate from the body over its circulating concentration, [L/h]");}

QString StdParamNames::getNameV(){return tr("Volume of distribution");}
QString StdParamNames::getDescV(){return tr("Ratio of the drug's amount present in the body over its circulating concentration, [L]");}

QString StdParamNames::getNameKA(){return tr("Absorption rate constant");}
QString StdParamNames::getDescKA(){return tr("Relative rate constant of the drug's absorption into the body, [1/h]");}

QString StdParamNames::getNameF(){return tr("Bioavailability");}
QString StdParamNames::getDescF(){return tr("Fraction of the drug's administered dose that reaches unchanged the systemic circulation, [%]");}

QString StdParamNames::getNameV1(){return tr("Central Volume of distribution");}
QString StdParamNames::getDescV1(){return tr("Volume into which the drug distributes initially without delay after its delivery into the circulation, [L]");}
QString StdParamNames::getNameV2(){return tr("Peripheral volume of distribution, second compartment");}
QString StdParamNames::getDescV2(){return tr("Volume into which a drug is considered to distribute secondly with retardation, from and back to the central compartment, [L]");}
QString StdParamNames::getNameV3(){return tr("Peripheral volume of distribution, third compartment");}
QString StdParamNames::getDescV3(){return tr("Volume into which a drug is considered to distribute thirdly with further retardation, from and back to the central compartment, [L]");}
QString StdParamNames::getNameQ(){return tr("Inter-compartmental clearance");}
QString StdParamNames::getDescQ(){return tr("Ratio of the drug's distribution rate between the central compartment and the peripheral compartments over its circulating concentration , [L/h]");}
QString StdParamNames::getNameQ2(){return tr("Inter-compartmental clearance 1-2");}
QString StdParamNames::getDescQ2(){return tr("Ratio of the drug's distribution rate between compartments 1 and 2 over its circulating concentration, [L/h]");}
QString StdParamNames::getNameQ3(){return tr("Inter-compartmental clearance 1-3");}
QString StdParamNames::getDescQ3(){return tr("Ratio of the drug's distribution rate between compartment 1 and 3 over its circulating concentration, [L/h]");}
QString StdParamNames::getNameVM(){return tr("Maximum rate of elimination");}
QString StdParamNames::getDescVM(){return tr("Maximum rate of elimination of the drug from the circulation through a saturable pathway, [mg/h]");}
QString StdParamNames::getNameKM(){return tr("Michaelis-Menten constant");}
QString StdParamNames::getDescKM(){return tr("Substrate concentration at which a saturable elimination rate reaches half of its maximum value, [mg/L]");}
QString StdParamNames::getNameK12(){return tr("Inter-compartmental absorption rate 1-2");}
QString StdParamNames::getDescK12(){return tr("Relative rate constant of the drug's absorption between compartment 1 and 2,  [1/h]");}
QString StdParamNames::getNameK21(){return tr("Inter-compartmental absorption rate 2-1");}
QString StdParamNames::getDescK21(){return tr("Relative rate constant of the drug's absorption between compartment 2 and 1,  [1/h]");}
QString StdParamNames::getNameK13(){return tr("Inter-compartmental absorption rate 1-3");}
QString StdParamNames::getDescK13(){return tr("Relative rate constant of the drug's absorption between compartment 1 and 3,  [1/h]");}
QString StdParamNames::getNameK31(){return tr("Inter-compartmental absorption rate 3-1");}
QString StdParamNames::getDescK31(){return tr("Relative rate constant of the drug's absorption between compartment 3 and 1,  [1/h]");}
QString StdParamNames::getNameKe(){return tr("Elimination rate constant");}
QString StdParamNames::getDescKe(){return tr("Relative rate constant of the drug's elimination from the body, [1/h]");}
QString StdParamNames::getNameGA1(){return tr("Gamma function parameter a1 [-]");}
QString StdParamNames::getDescGA1(){return tr("Gamma function parameter a1 [-]");}
QString StdParamNames::getNameGB1(){return tr("Gamma function parameter b1 [-]");}
QString StdParamNames::getDescGB1(){return tr("Gamma function parameter b1 [-]");}
QString StdParamNames::getNameGA2(){return tr("Gamma function parameter a2 [-]");}
QString StdParamNames::getDescGA2(){return tr("Gamma function parameter a2 [-]");}
QString StdParamNames::getNameGB2(){return tr("Gamma function parameter b2 [-]");}
QString StdParamNames::getDescGB2(){return tr("Gamma function parameter b2 [-]");}
QString StdParamNames::getNameA(){return tr("Gamma model parameter A [-]");}
QString StdParamNames::getDescA(){return tr("Gamma model parameter A [-]");}
QString StdParamNames::getNameR(){return tr("Gamma model parameter R [-]");}
QString StdParamNames::getDescR(){return tr("Gamma model parameter R [-]");}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

