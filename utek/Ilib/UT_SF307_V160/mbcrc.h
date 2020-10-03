#ifndef MBCRC
#define MBCRC

#include <QObject>
#include <QByteArray>

typedef  unsigned short USHORT;
typedef  unsigned char  UCHAR;

//USHORT usMBCRC16(UCHAR * pucFrame, USHORT usLen);
USHORT usMBCRC16(QByteArray &msg);

#endif // MBCRC
