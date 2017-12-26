#include "hash.h"

QString hash(QString s){
    return QString(
                QCryptographicHash::hash(
                    s.toUtf8(),QCryptographicHash::Sha3_512
                ).toHex()
            );
}
