/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


//Determine the current Tucuxi version
#define TUCUXI_VERSION "0.2.11"

//Determine the current database version
#define DATABASE_VERSION "0.2.7"

//Determine the current XML editor version
#define EDITOR_VERSION "0.0.3"

//Determine the current XML drug files version
#define DRUG_VERSION "0.6"

//Determine the current XML definition file version
#define DEFINITIONS_VERSION "0.2"

//Determine the current XML import file version
#define IMPORT_VERSION "0.2"

//Determine the current XML export file version
#define EXPORT_VERSION "0.2"

/* This number is used to know on which channel this
 * build should be by default. The channel can be changed
 * through the configuration, but the default value is
 * used upon installation. Possible values: Dev, Stable, Beta. */
#define RELEASE_TYPE Distributor::Dev
