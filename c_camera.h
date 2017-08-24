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
    CCamera(QMainWindow *parent);
    void    requestAzi();
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

    double elevation() const;
    void setElevation(double elevation);

    bool getIsScaning() const;

    double getHeight() const;

    bool getIsOnline() const;

    void setHeight(double height);

    double getSkipAzi() const;
    void setSkipAzi(double skipAzi);

    double getSkipAziSize() const;
    void setSkipAziSize(double skipAziSize);
    void checkSkipAzi();
    void StartScan(int rate);
private:
    //QNetworkReply *reply;
    QMainWindow *mParent;
    double  mLat,mLon;
    double mHeight;
    bool mAlarm;
    bool delayScan;
    double scanningElevation;
    double mAzi,mElevation,mAziNorth;
    bool isScaning,isOnline;
    int isNetworkRequestAwaiting;
    double mElevHorizon;
    double mSkipAzi,mSkipAziSize;
    QString mIP,mUserName,mPassword,mCamName;
    bool CheckLastReply();

    void changeElevation();
};

#endif // C_CAMERA_H
