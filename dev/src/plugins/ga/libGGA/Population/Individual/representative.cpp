/**
 * @file representative.cpp
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 02.2012
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
 * @class Representative
 * @brief A representative or cooperator is a population entity selected
 * to be a cooperator at the disposale of the genetic algortihm (e,g Fuzzy Coco)
 *
 */
#include "representative.h"

/**
 * @brief Representative::Representative
 * Constructs a reprensentative with a certain genome lenght.
 *
 * @param lenght The genome lenght
 */
Representative::Representative(quint32 lenght) : PopEntity(lenght)
{
}

/**
 * @brief Representative::Representative
 * Constructs a representative with a specific genotype.
 *
 * @param genotype The genotype to be applied to the representative
 */
Representative::Representative(Genotype *genotype) : PopEntity(genotype)
{
}

/**
 * @brief Representative::Representative
 * Copy a representative.
 *
 * @param representative The representative to be copied
 */
Representative::Representative(Representative *representative) : PopEntity(representative)
{
}
