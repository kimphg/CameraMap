#include "c_camera.h"

#include <QEventLoop>

//#include <QEventLoop>

CCamera::CCamera()
{

    mIP = "192.168.100.100";
    mUserName = "service";
    mPassword = "12345678";
}

void CCamera::requestAzi()
{
    /*
    <rcp>
    <command>
    <hex>0x09a5</hex>
    <dec>2469</dec>
    </command>
    <type>P_OCTET</type>
    <direction>WRITE</direction>
    <num>1</num>
    <idstring/>
    <payload/>
    <cltid>0x0046</cltid>
    <sessionid>0x00000000</sessionid>
    <auth>2</auth>
    <protocol>TCP</protocol>
    <result>
    <len>8</len>
    <str>81 00 06 01 12 01 f3 d1</str>
    </result>
    </rcp>
    */
    QNetworkReply *reply;
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    /*reply = qnam->get(QNetworkRequest(QUrl("http://"+service+":12345678@"
    + mIP +"/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201")));
    //http://192.168.100.100/rcp.xml?command=0x09A5&type=P_OCTET&direction=WRITE&num=1&payload=0x810006011201*/
    reply = qnam->get(QNetworkRequest(QUrl("http://127.0.0.1/index.xml")));
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
                mAzi = !!
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

QString CCamera::camName() const
{
    return mCamName;
}

void CCamera::setCamName(const QString &camName)
{
    mCamName = camName;
}
