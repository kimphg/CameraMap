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
    mAlarm = false;

}

void CCamera::requestAzi()
{

    QNetworkReply *reply;
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    reply = qnam->get(QNetworkRequest(QUrl("http://"
                                           +mUserName+":"+mPassword+"@"+ mIP
                                           +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201")));
    //http://192.168.100.100/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201*/
    //reply = qnam->get(QNetworkRequest(QUrl("http://127.0.0.1/index.xml")));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
//    int waitTime=0;
//    while (!reply->isFinished())
//    {
//        waitTime++;
//        if(waitTime>1000)break;
//    };
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(reply);
    xmlReader.readNext();
    while (!xmlReader.isEndDocument())
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
                double aziRad = strList.at(6).toInt(&ok,16)*255 + strList.at(7).toInt(&ok,16);
                aziRad/= 10000.0 ;
                mAzi = aziRad/3.1415926535*180;
                if(mAzi<0)mAzi = 0;
                if(mAzi>=360)mAzi = 0;
            }
        }
        xmlReader.readNext();

    }
    if (xmlReader.hasError())
    {
        return;
        //std::cout << "XML error: " << xmlReader.errorString().data() << std::endl;
    }

}

void CCamera::requestElevation()
{

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

    return mAzi;
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
