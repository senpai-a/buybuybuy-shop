#include "hash.h"

QString hash(QString s){
    return QString(
                QCryptographicHash::hash(
                    s.toLatin1(),QCryptographicHash::Sha3_512
                ).toHex()
            );
}
