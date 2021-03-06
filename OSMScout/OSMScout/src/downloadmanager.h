#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtGlobal>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>
#include <QtGui>

#include "downloadmanagerHTTP.h"




class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = 0);

signals:
    void addLine(QString qsLine);
    void progress(int nPercentage);
    void downloadComplete();

public slots:
    void download(QUrl url, QString downloadPath);
    void pause();
    void resume();
    QString md5sum(const QString &fileName);
    bool checkmd5sum(const QString path, const QString & fileName);

private slots:
    void localAddLine(QString qsLine);
    void localProgress(int nPercentage);
    void localDownloadComplete();

private:
    QUrl _URL;
    DownloadManagerHTTP *_pHTTP;
};




#endif // DOWNLOADMANAGER_H
