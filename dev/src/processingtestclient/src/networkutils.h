/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H


#define DEFAULTPORT 58431
#define SERVERVERSION "0.1.0"

#define COMMAND_REQUEST  0
#define COMMAND_RESPONSE 1

#define blocksize_t quint32
#define PREPAREBLOCK(command)  \
     QByteArray block; \
     QDataStream out(&block, QIODevice::WriteOnly); \
     out.setVersion(QDataStream::Qt_4_0); \
     out << (blocksize_t)0; \
     out << (int)(command);
//	 cout << "command: " << command << "---";

#define ENDBLOCK \
     out.device()->seek(0); \
     out << (blocksize_t)(block.size()-sizeof(blocksize_t));
//	 cout << "size: " << (quint32)(block.size()-sizeof(blocksize_t)) << endl;

#define SENDBLOCK(socket) \
     socket->write(block);

#define CLIENTSENDBLOCK \
     return sendBlock(block);

#endif // NETWORKUTILS_H
