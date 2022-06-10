//@@license@@

#include "core/dal/residual.h"

namespace Tucuxi {
namespace GuiCore {

//Residual::Residual(QObject *parent) : QObject(parent)
//{

//}

Residual::Residual(double value) : value(value){
}

Residual::Residual() : value(0.0){
}

//----------------------- RESIDUALS --------------------------

Residuals::Residuals()
{

}

Residuals::Residuals(int size) : residuals(size)
{
}

const Residual &Residuals::at(int i)
{
    //Resize the vector if position do not exist
    if(i+1 > residuals.size())
        residuals.resize(i+1);

    return residuals.at(i);
}

Residual &Residuals::operator[](int i)
{
    //Resize the vector if position do not exist
    if(i+1 > residuals.size())
        residuals.resize(i+1);

    return residuals[i];
}

int Residuals::size() const
{
    return residuals.size();
}

void Residuals::clear()
{
    residuals.clear();
}

} // namespace core
} // namespace Tucuxi
