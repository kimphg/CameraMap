#include "c_camera.h"

#include <QEventLoop>
#include <QtGui>

//#include <QEventLoop>

CCamera::CCamera()
{

    mIP = "192.168.100.100";
    mUserName = "service";
    mPassword = "12345678";
    mAzi = 0;
    mElevation = -25;
    mAlarm = false;
    mHeight = 0.03;
    isScaning = false;
    isOnline = false;
    isNetworkRequestAwaiting  =0;
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
void CCamera::requestAzi(QMainWindow *parent)
{
    if(isNetworkRequestAwaiting>5)
    {
        isOnline = false;
        return;
    }
    isNetworkRequestAwaiting +=1;
    QNetworkAccessManager *qnam = new QNetworkAccessManager(parent);
    QEventLoop loop;
    double oldAzi = mAzi;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP
                                           +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201")));

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(parent, SIGNAL(destroyed()), &loop, SLOT(quit()));
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
                mAzi = 360-(aziRad/ 10000.0)*DEG2RAD+90 ;//aziRad/3.1415926535*180.0;
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
    qnam->deleteLater();
    isNetworkRequestAwaiting = 0;
}

void CCamera::requestElevation(QMainWindow *parent)
{
    if(isNetworkRequestAwaiting>5)
    {
        isOnline = false;
        return;
    }
    isNetworkRequestAwaiting +=1;
    QNetworkAccessManager *qnam = new QNetworkAccessManager(parent);
    QEventLoop loop;
    QNetworkReply *reply ;
    reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP
                                           +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011301")));

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(parent, SIGNAL(destroyed()), &loop, SLOT(quit()));
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

                mElevation =  (aziRad/ 10000.0 - 3.1415926535)*DEG2RAD - 15 ;//aziRad/3.1415926535*180.0;
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
    mElevation = elevation;
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
    return (mAzi+mAziNorth);
}

void CCamera::setAzi(double azi)
{
    mAzi = azi;
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
