#include "drugtohtml.h"

#include "core/dal/drug/drug.h"
#include "core/dal/drug/translatablestring.h"
#include "core/dal/drug/adme.h"

DrugToHtml::DrugToHtml()
{

}

QString DrugToHtml::drugToHtml(const ezechiel::core::DrugModel *drug)
{
    if (drug) {
        QString desc;

        desc += "<table border=0>";
        desc += "<tr><td></td><td></td></tr>";
        desc += "<tr><td><b>Drug name :</b></td><td><b>" + drug->getName()->value() + "</b></td></tr>";
        desc += "<tr><td>ATC:  </td><td>";
        ezechiel::core::ActiveSubstance *substance = drug->getActiveSubstance();
        for(int i=0;i<substance->getAtc().size(); i++) {
            desc += substance->getAtc().at(i);
            if (i != substance->getAtc().size() - 1)
                desc += ", ";
        }
        desc += "</td></tr>";
        /*desc += "<h5>Brands: ";
        for(int i=0;i<substance->getBrands().size(); i++) {
            desc += substance->getBrands().at(i);
            if (i != substance->getBrands().size() - 1)
                desc += ", ";
        }
        desc += "</h5>";*/
        desc += "<tr><td></td><td></td></tr>";
        desc += "<tr><td><b>Domain:   </b></td><td><b>" + drug->getDomainName()+ "</b></td></tr>";
        desc += "<tr><td>Study:   </td><td>" + drug->getStudyName() + "</td></tr>";
        desc += "<tr><td>Authors:   </td><td>" + drug->getStudyAuthors() + "</td></tr>";
//        desc += "<h5>Absorption, distribution, metabolism, and excretion</h5>";
        desc += "<tr><td></td><td></td></tr>";

        desc += "<tr><td>Model: </td><td>" + drug->getDrugModelId() + "</td></tr>";
        if (drug->getAdme()->getIntakes()->size() == 0)
            desc += "<tr><td>Route: </td><td>Undefined</td></tr>";
        else if (drug->getAdme()->getIntakes()->size() == 1)
            desc += "<tr><td>Route: </td><td>" + drug->getAdme()->getIntakes()->at(0)->getDescription() + "</td></tr>";
        else {
            desc += "<tr><td>Route: </td><td>";
            for(int i=0;i<drug->getAdme()->getIntakes()->size(); i++) {
                desc += drug->getAdme()->getIntakes()->at(i)->getDescription();
                if (i != drug->getAdme()->getIntakes()->size() - 1)
                    desc += ", ";
            }
            desc += "</td></tr>";
        }
        // desc += "<h5>Pk Model: " + drug->getPkModelId() + "</h5>";
        if (drug->getAdme()->getIntakes()->size() == 0)
            desc += "<tr><td>Absorption: </td><td>Undefined</td></tr>";
        else if (drug->getAdme()->getIntakes()->size() == 1)
            desc += "<tr><td>Absorption: </td><td>" + drug->getAdme()->getIntakes()->at(0)->getLabelString() + "</td></tr>";
        else {
            desc += "<tr><td>Absorption: </td><td>";
            for(int i=0;i<drug->getAdme()->getIntakes()->size(); i++) {
                desc += drug->getAdme()->getIntakes()->at(i)->getLabelString();
                if (i != drug->getAdme()->getIntakes()->size() - 1)
                    desc += ", ";
            }
            desc += "</td></tr>";
        }
        //desc += "<tr><td>Distribution: </td><td>" + drug->getAdme()->getDistribution() + " compartments</td></tr>";
        desc += "<tr><td>Distribution: </td><td>" + drug->getAdme()->getDistribution() + "</td></tr>";
        desc += "<tr><td>Elimination: </td><td>" + drug->getAdme()->getElimination() + "</td></tr>";
        desc += "</table>";

        return desc;
    }


    if (drug) {
        QString desc;

        desc += "<h1>" + drug->getName()->value() + "</h1>";
        // desc += "<h5>Model: " + drug->getPkModelId() + "</h5>";
        desc += "<h5>Model: " + drug->getDrugModelId() + "</h5>";
        desc += "<h5>ATC: ";
        ezechiel::core::ActiveSubstance *substance = drug->getActiveSubstance();
        for(int i=0;i<substance->getAtc().size(); i++) {
            desc += substance->getAtc().at(i);
            if (i != substance->getAtc().size() - 1)
                desc += ", ";
        }
        desc += "</h5>";
        /*desc += "<h5>Brands: ";
        for(int i=0;i<substance->getBrands().size(); i++) {
            desc += substance->getBrands().at(i);
            if (i != substance->getBrands().size() - 1)
                desc += ", ";
        }
        desc += "</h5>";*/
        desc += "<h5>Domain: " + drug->getDomainName()+ "</h5>";
        desc += "<h5>Study: " + drug->getStudyName() + "</h5>";
        desc += "<h5>Authors: " + drug->getStudyAuthors() + "</h5>";
        desc += "<h5>Absorption, distribution, metabolism, and excretion</h5>";
        desc += "<table border=0>";
        if (drug->getAdme()->getIntakes()->size() == 0)
            desc += "<tr><td>Route: </td><td>Undefined</td></tr>";
        else if (drug->getAdme()->getIntakes()->size() == 1)
            desc += "<tr><td>Route: </td><td>" + drug->getAdme()->getIntakes()->at(0)->getDescription() + "</td></tr>";
        else {
            desc += "<tr><td>Route: </td><td>";
            for(int i=0;i<drug->getAdme()->getIntakes()->size(); i++) {
                desc += drug->getAdme()->getIntakes()->at(i)->getDescription();
                if (i != drug->getAdme()->getIntakes()->size() - 1)
                    desc += ", ";
            }
            desc += "</td></tr>";
        }
        if (drug->getAdme()->getIntakes()->size() == 0)
            desc += "<tr><td>Absorption: </td><td>Undefined</td></tr>";
        else if (drug->getAdme()->getIntakes()->size() == 1)
            desc += "<tr><td>Absorption: </td><td>" + drug->getAdme()->getIntakes()->at(0)->getLabelString() + "</td></tr>";
        else {
            desc += "<tr><td>Absorption: </td><td>";
            for(int i=0;i<drug->getAdme()->getIntakes()->size(); i++) {
                desc += drug->getAdme()->getIntakes()->at(i)->getLabelString();
                if (i != drug->getAdme()->getIntakes()->size() - 1)
                    desc += ", ";
            }
            desc += "</td></tr>";
        }
        //desc += "<tr><td>Distribution: </td><td>" + drug->getAdme()->getDistribution() + " compartments</td></tr>";
        desc += "<tr><td>Distribution: </td><td>" + drug->getAdme()->getDistribution() + "</td></tr>";
        desc += "<tr><td>Elimination: </td><td>" + drug->getAdme()->getElimination() + "</td></tr>";
        desc += "</table>";

        return desc;
    }
    return QString("No drug selected");
}


QString DrugToHtml::activeSubstanceToHtml(const ezechiel::core::ActiveSubstance *drug)
{
    if (drug) {
        QString desc;

        desc += "<h1>" + drug->getName()->value() + "</h1>";
        desc += "<h5>ATC: ";
        for(int i=0;i<drug->getAtc().size(); i++) {
            desc += drug->getAtc().at(i);
            if (i != drug->getAtc().size() - 1)
                desc += ", ";
        }
        desc += "</h5>";
        desc += "<h5>Brands: ";
        for(int i=0;i<drug->getBrands().size(); i++) {
            desc += drug->getBrands().at(i);
            if (i != drug->getBrands().size() - 1)
                desc += ", ";
        }
        desc += "</h5>";

        desc += "<h5>Internal unique Id: ";
        desc += drug->getSubstanceId();
        desc += "</h5>";

        return desc;
    }
    return QString("No active substance selected");
}