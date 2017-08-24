#include "c_camera.h"

#include <QEventLoop>
#include <QtGui>

//#include <QEventLoop>

CCamera::CCamera(QMainWindow *parent)
{
    mParent = parent;
    mIP = "192.168.100.100";
    mUserName = "service";
    mPassword = "12345678";
    mAzi = 0;
    mAziNorth = 0;
    mElevation = -25;
    mElevHorizon = -15;
    mAlarm = false;
    mHeight = 0.03;
    isScaning = false;
    isOnline = false;
    delayScan = false;
    isNetworkRequestAwaiting  =0;
    mSkipAzi = 0;
    mSkipAziSize = 20;
}

bool CCamera::CheckLastReply()
{/*
    if(!reply)
        return true;
    if(!reply->isFinished())
    {
        reply->abort();
        return true;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(reply);
    xmlReader.readNext();
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        if (xmlReader.isStartElement())
        {
            QString name = xmlReader.name().toString();
            if (name == "str" )
            {
                //name = name;
                //QMessageBox::information(this,name,xmlReader.readElementText());
                name = xmlReader.readElementText();
                QStringList strList = name.split(' ');
                bool ok;
                double aziRad =  strList.at(6).toInt(&ok,16)*255 + strList.at(7).toInt(&ok,16);

                mAziRad = aziRad/ 10000.0 ;//aziRad/3.1415926535*180.0;
                if(mAziRad<0)mAziRad = 0;
                if(mAziRad>=(3.1415926535*2.0))mAziRad = 0;
            }
            if (xmlReader.hasError())
            {break;}
        }
        xmlReader.readNext();

    }*/
    return true;
}
int curEle = 1;
void CCamera::changeElevation()
{
    if(curEle==1)
    {
        scanningElevation = -12;
        curEle=2;
    }
    else if(curEle==2)
    {
        scanningElevation = -19;
        curEle=3;
    }
    else if(curEle==3)
    {
        scanningElevation = -26;
        curEle=1;
    }
    setElevation(scanningElevation);
    setElevation(scanningElevation);
    setElevation(scanningElevation);
}
void CCamera::requestAzi()
{
    if(isNetworkRequestAwaiting>5)
    {
        isOnline = false;
        return;
    }
    if(delayScan&&(!isScaning))
    {
        StartScan(3);
        changeElevation();
        delayScan = false;
    }
    isNetworkRequestAwaiting +=1;
    QNetworkAccessManager *qnam = new QNetworkAccessManager(mParent);
    QEventLoop loop;
    double oldAzi = mAzi;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP
                                           +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201")));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(mParent, SIGNAL(destroyed()), &loop, SLOT(quit()));
    loop.exec();
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(reply);
    xmlReader.readNext();
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        if (xmlReader.isStartElement())
        {
            QString name = xmlReader.name().toString();
            if (name == "str" )
            {
                //name = name;
                //QMessageBox::information(this,name,xmlReader.readElementText());
                name = xmlReader.readElementText();
                QStringList strList = name.split(' ');
                if ((strList.at(4)!="12")||strList.size()<8)break;
                bool ok;
                double aziRad =  strList.at(6).toInt(&ok,16)*255 + strList.at(7).toInt(&ok,16);
                mAzi = 90-(aziRad/ 10000.0)*DEG2RAD ;//aziRad/3.1415926535*180.0;
                mAzi+= mAziNorth;
                if(mAzi<0)mAzi +=360;
                if(mAzi>=360)mAzi -=360;
                if(qAbs(mAzi-oldAzi)<0.3)isScaning = false;
                else    isScaning=true;
                isOnline = true;
            }
            if (xmlReader.hasError())
            {break;}
        }
        xmlReader.readNext();

    }
    reply->deleteLater();
    qnam->deleteLater();
    isNetworkRequestAwaiting = 0;
    checkSkipAzi();
}

void CCamera::requestElevation()
{
    if(isNetworkRequestAwaiting>5)
    {
        isOnline = false;
        return;
    }
    isNetworkRequestAwaiting +=1;
    QNetworkAccessManager *qnam = new QNetworkAccessManager(mParent);
    QEventLoop loop;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP
                                           +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011301")));

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(mParent, SIGNAL(destroyed()), &loop, SLOT(quit()));
    loop.exec();
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(reply);
    xmlReader.readNext();
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        if (xmlReader.isStartElement())
        {
            QString name = xmlReader.name().toString();
            if (name == "str" )
            {
                //name = name;
                //QMessageBox::information(this,name,xmlReader.readElementText());
                name = xmlReader.readElementText();
                QStringList strList = name.split(' ');
                if ((strList.at(4)!="13")||strList.size()<8)break;
                bool ok;
                double aziRad =  strList.at(6).toInt(&ok,16)*255 + strList.at(7).toInt(&ok,16);

                mElevation =  (aziRad/ 10000.0 )*DEG2RAD - 180.0 ;//aziRad/3.1415926535*180.0;
                mElevation += mElevHorizon;
                if(mElevation<-90)mElevation +=360;
                if(mElevation>90)mElevation -=360;
            }

        }
        xmlReader.readNext();

    }
    qnam->deleteLater();
    reply->deleteLater();
    isNetworkRequestAwaiting = 0;
}

double CCamera::elevation() const
{
    return mElevation;
}

void CCamera::setElevation(double elevation)
{
    while(elevation<0)elevation+=360;
    while(elevation>360)elevation-=360;
    mElevation = elevation;
    double tilt = (mElevation - mElevHorizon)+180;
    while(tilt<0) tilt+=360;
    while(tilt>360) tilt-=360;
    tilt = tilt/DEG2RAD*10000.0;
    QString value = QString::number(int(tilt), 16);
    while(value.size()<4)value = "0" + value;
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    QEventLoop loop;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
            + mUserName + ":" + mPassword + "@" + mIP
            +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011303"
            +value)));

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    qnam->deleteLater();
    reply->deleteLater();
}

bool CCamera::getIsScaning() const
{
    return isScaning;
}

double CCamera::getHeight() const
{
    return mHeight;
}

bool CCamera::getIsOnline() const
{
    return isOnline;
}

void CCamera::setHeight(double height)
{
    mHeight = height;
}

double CCamera::getSkipAzi() const
{
    return mSkipAzi;
}

void CCamera::setSkipAzi(double skipAzi)
{
    mSkipAzi = skipAzi;
}

double CCamera::getSkipAziSize() const
{
    return mSkipAziSize;
}

void CCamera::setSkipAziSize(double skipAziSize)
{
    mSkipAziSize = skipAziSize;
}

void CCamera::checkSkipAzi()
{
    if(qAbs(mSkipAzi-mAzi)<1)
    {
        setAzi( mSkipAzi-mSkipAziSize);
        delayScan = true;

    }
}
void CCamera::StartScan(int rate)
{
    if(rate>9||rate<0)return;
    QNetworkAccessManager*qnam = new QNetworkAccessManager();
    QNetworkReply* reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP+
                                           "/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x8000060110850"
                                           +QString::number(rate)+"0000")));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    qnam->deleteLater();
    reply->deleteLater();
}
QString CCamera::iP() const
{
    return mIP;
}

void CCamera::setIP(const QString &iP)
{
    mIP = iP;
}

double CCamera::lat() const
{
    return mLat;
}

void CCamera::setLat(double lat)
{
    mLat = lat;
}

double CCamera::lon() const
{
    return mLon;
}

void CCamera::setLon(double lon)
{
    mLon = lon;
}

double CCamera::azi() const
{
    return (mAzi);
}

void CCamera::setAzi(double azi)
{
    while(azi<0)azi+=360;
    while(azi>360)azi-=360;
    mAzi = azi;
    double pan = 90-(mAzi - mAziNorth);
    while(pan<0)pan+=360;
    while(pan>360)pan-=360;
    pan = pan/DEG2RAD*10000.0;
    QString value = QString::number( int(pan), 16 );
    while(value.size()<4)value = "0" + value;
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    QEventLoop loop;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
            +mUserName+":"+mPassword+"@"+ mIP
            +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011203"
            +value)));

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    qnam->deleteLater();
    reply->deleteLater();
}

bool CCamera::alarm() const
{
    return mAlarm;
}

void CCamera::setAlarm(bool alarm)
{
    mAlarm = alarm;
}

double CCamera::aziNorth() const
{
    return mAziNorth;
}

void CCamera::setAziNorth(double aziNorth)
{
    mAziNorth = aziNorth;
}

QString CCamera::password() const
{
    return mPassword;
}

void CCamera::setPassword(const QString &password)
{
    mPassword = password;
}

QString CCamera::userName() const
{
    return mUserName;
}

void CCamera::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString CCamera::camName() const
{
    return mCamName;
}

void CCamera::setCamName(const QString &camName)
{
    mCamName = camName;
}
