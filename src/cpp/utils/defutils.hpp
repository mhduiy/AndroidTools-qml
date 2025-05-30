#pragma once
#include <QMutex>

#define SINGLETON(className) \
public: \
    static className* instance(QObject *parent = nullptr) { \
        static QMutex mutex; \
        QMutexLocker locker(&mutex); \
        static className* instance = nullptr; \
        if (instance == nullptr) { \
            instance = new className(parent); \
    } \
        return instance; \
} \
private: \
explicit className(QObject *parent = nullptr);

#define DECLARE_PROPERTY(type, name) \
    Q_PROPERTY(type name READ name WRITE set##name NOTIFY name##Changed) \
    public: \
        type name() const { return m_##name; } \
        void set##name(type val) { \
            if (m_##name != val) { \
                m_##name = val; \
                emit name##Changed(val); \
            } \
        } \
    Q_SIGNALS: \
        void name##Changed(type val); \
    private: \
        type m_##name;

#define DECLARE_READONLY_PROPERTY(type, name) \
    Q_PROPERTY(type name READ name NOTIFY name##Changed) \
    public: \
        type name() const { return m_##name; } \
    Q_SIGNALS: \
        void name##Changed(type val); \
    private: \
        type m_##name;
