#ifndef UTILS_HPP
#define UTILS_HPP

#include <QMap>
#include <QThreadPool>
#include <QRunnable>
#include <functional>

static QMap<QString, QString> serializationInformation(const QString &info)
{
    QMap<QString, QString> res;
    QStringList l = info.split('\n');
    for (QString &s : l) {
        s = s.simplified();
        if (s.isEmpty()) {
            continue;
        }
        QStringList ll = s.split(':');
        res.insert(ll.value(0).simplified(), ll.value(1).simplified());
    }
    return res;
}

inline void asyncOperator(std::function<void()> func)
{
    class FnRunnable : public QRunnable
    {
    public:
        FnRunnable(std::function<void()> fn) : m_fn(std::move(fn)) {}
        void run() override { m_fn(); }
    private:
        std::function<void()> m_fn;
    };
    QThreadPool::globalInstance()->start(new FnRunnable(std::move(func)));
}

#endif
