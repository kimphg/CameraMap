#include "c_camera.h"

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
    reply = qnam->get(QNetworkRequest(QUrl("http://vnexpress.net")));
//    QEventLoop loop;
//    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec();
    int waitTime=0;
    while (!reply->isFinished())
    {
        waitTime++;
    };
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(reply);
    while (xmlReader.readNextStartElement())
    {
//        if(xmlReader.name()==XML_ELEM_NAME)
//        {
//           for (uint i=0;i<xmlReader.attributes().size();i++)
//           {
//               QXmlStreamAttribute attr = xml.attributes().at(i);
//               hashData.insert( attr.name().toString(),
//                                attr.value().toString());
//           }
//        }
//        if (xmlReader.tokenType() == QXmlStreamReader::Invalid)
//            xmlReader.readNext();
//        // readNextStartElement() leaves the stream in
//        // an invalid state at the end. A single readNext()
//        // will advance us to EndDocument.
//        if (xmlReader.hasError()) {
//            continue;
//        }
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
