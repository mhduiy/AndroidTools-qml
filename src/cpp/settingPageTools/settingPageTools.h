#ifndef SETTINGPAGETOOLS_H
#define SETTINGPAGETOOLS_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QPointer>

#include "../utils/singleton.hpp"

#include "bingwallpaperhander.h"

enum WallPaperModelRoles {
    UrlRole = Qt::UserRole + 1,
    TitleRole,
    LoadingRole,
};

struct WallPaperInfo
{
    QString url;
    QString title;
    bool isLoading = true;
    WallPaperInfo(const QString &_url = "", const QString &_title = "", const bool _loading = false)
        : url(_url), title(_title), isLoading(_loading) {}
};

class WallPaperModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WallPaperModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(WallPaperInfo info);
    Q_INVOKABLE void removeRow(const QString &code);
    Q_INVOKABLE void setInfo(const WallPaperInfo &info);
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void setCurrentIndex(int index);

signals:
    void currentItemChanged(const QString &url);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<WallPaperInfo> m_wallPaperInfo;
};

class SettingPageTools : public QObject
{
    Q_OBJECT
    SINGLETON(SettingPageTools)
    ~SettingPageTools();

private slots:
    void onBingWallPaperWorkFinish(const QString &url);

private:
    WallPaperModel *m_wallpaperModel;
    BingWallPaperHander *m_bingWallpaperHander;
};

#endif
