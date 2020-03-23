/**
  * @file   genotype.h
  * @author Yvan Da Silva <yvan.dasilva_at_heig-vd.ch>
  * @author ReDS (Reconfigurable and embedded digital systems) <www.reds.ch>
  * @author HEIG-VD (Haute école d'ingénierie et de gestion) <www.heig-vd.ch>
  * @date   06.2012
  * @section LICENSE
  *
  * This application is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 3.0 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * @class Genotype
  *
  * @brief  A genotype is a serie of bits, the container used is a QBitArray.
  * A genotype is used to describe an entitiy.
  */
#ifndef GENOTYPE_H
#define GENOTYPE_H
#include <Qt>
#include <QBitArray>
#include <QString>
#include <QDebug>

#include <memory>

using namespace std;
class Genotype
{
public:
    Genotype(quint32 length);
    Genotype(QBitArray *data);
    Genotype(Genotype *genotype);
    ~Genotype();

    Genotype *getCopy();

    quint32 getLength();
    QBitArray *getData();
    QBitArray *getDataCopy();
    void setData(QBitArray *data);
private:
    quint32 length;
    QBitArray *data;

};

#endif // GENOTYPE_H
