#ifndef C_CAMERA_H
#define C_CAMERA_H

#include <QtNetwork/QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QNetworkReply>
//extern CConfig *mConfig;
class CCamera
{
public:
    CCamera();
    double  mLat,mLon;
    double  mAzi,mElevation;
    void    requestAzi();
    void    requestElevation();
    QString mIP,mUserName,mPassword,mCamName;

    QString camName() const;
    void setCamName(const QString &camName);
};

#endif // C_CAMERA_H
