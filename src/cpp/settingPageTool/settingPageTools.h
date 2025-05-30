#ifndef SETTINGPAGETOOLS_H
#define SETTINGPAGETOOLS_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QPointer>

#include "../utils/defutils.hpp"

#include "bingwallpaperhander.h"

namespace ADT {

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
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    explicit WallPaperModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(WallPaperInfo info);
    Q_INVOKABLE void insertRow(int row, WallPaperInfo info);
    Q_INVOKABLE void removeRow(const QString &code);
    Q_INVOKABLE void setInfo(const WallPaperInfo &info);
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void setCurrentIndex(int index);
    int currentIndex() {return m_currentIndex;}
    void clearData();

signals:
    void currentItemChanged(const QString &url);
    void currentIndexChanged(int index);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<WallPaperInfo> m_wallPaperInfo;
    int m_currentIndex = -1;
};

class SettingPageTools : public QObject
{
    Q_OBJECT
    SINGLETON(SettingPageTools)
    ~SettingPageTools();

private slots:
    void onBingWallPaperWorkFinish(const QString &url);
    void onRequestRefreshWallpaperList();

private:
    WallPaperModel *m_wallpaperModel;
    BingWallPaperHander *m_bingWallpaperHander;
};

#endif

} // namespace ADT
