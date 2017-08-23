#ifndef C_CAMERA_H
#define C_CAMERA_H
#define DEG2RAD 57.295779513
#include <QtNetwork/QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QNetworkReply>
#include <QMainWindow>
//extern CConfig *mConfig;
class CCamera
{
public:
    CCamera();
    void    requestAzi(QMainWindow *parent);
    void    requestElevation();
    QString camName() const;
    void setCamName(const QString &camName);
    QString userName() const;
    void setUserName(const QString &userName);
    QString password() const;
    void setPassword(const QString &password);
    QString iP() const;
    void setIP(const QString &iP);
    double lat() const;
    void setLat(double lat);

    double lon() const;
    void setLon(double lon);

    double azi() const;
    void setAzi(double azi);

    bool alarm() const;
    void setAlarm(bool alarm);

    double aziNorth() const;
    void setAziNorth(double aziNorth);

    void requestElevation(QMainWindow *parent);
    double elevation() const;
    void setElevation(double elevation);

    bool getIsScaning() const;

    double getHeight() const;

    bool getIsOnline() const;

    void setHeight(double height);

private:
    //QNetworkReply *reply;
    double  mLat,mLon;
    double mHeight;
    bool mAlarm;
    double mAzi,mElevation,mAziNorth;
    bool isScaning,isOnline;
    int isNetworkRequestAwaiting;
    QString mIP,mUserName,mPassword,mCamName;
    bool CheckLastReply();
};

#endif // C_CAMERA_H
