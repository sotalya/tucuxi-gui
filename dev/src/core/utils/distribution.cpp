//@@license@@

#include "distribution.h"

//#include <QObject>

#include "core/utils/ezutils.h"

namespace ezechiel {
namespace core {


//Array of possible distributions
QList<Descriptor> descs;

//Fill the descriptor list
// Each value needs to be appened in the same order as the DistributionType enumeration
void createDescs()
{
   descs.append(Descriptor("uniform",Descriptor::tr("Uniform"),Descriptor::tr("Uniform distribution")));
   descs.append(Descriptor("logNormal",Descriptor::tr("Log Normal"),Descriptor::tr("Log normal distribution")));
   descs.append(Descriptor("normal",Descriptor::tr("Normal"),Descriptor::tr("Normal distribution")));
}


// Create the entries if not already done
QList<Descriptor> distribution::descriptions()
{
   if (descs.isEmpty())
      createDescs();
   return descs;
}

// Create the entries if not already done, and search for the right one on them
distribution::DistributionType distribution::fromString(QString str)
{
   if (descs.isEmpty())
      createDescs();

   //Simple search
   for (int i=0; i<descs.size(); i++)
      if (descs.at(i).id == str)
         return DistributionType(i);
   return Invalid;
}

//Simple lookup
QString distribution::toString (DistributionType type)
{
   if (descs.isEmpty())
      createDescs();

   if (type >= descs.size())
      return "invalid";
   return descs.at(type).id;
}

} // namespace core
} // namespace ezechiel

