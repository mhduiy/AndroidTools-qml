#ifndef UTILS_HPP
#define UTILS_HPP

#include <QMap>

QMap<QString, QString> serializationInformation(const QString &info)
{
    QMap<QString, QString> res;
    QStringList l = info.split('\n');
    for (QString &s : l) {
        s = s.simplified();
        if(s.isEmpty()) {   //过滤空行
            continue;
        }
        QStringList ll = s.split(':');
        res.insert(ll.value(0).simplified(), ll.value(1).simplified());
    }
    return res;
}

#endif
