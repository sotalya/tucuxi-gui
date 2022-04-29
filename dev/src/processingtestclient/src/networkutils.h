//@@license@@

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
