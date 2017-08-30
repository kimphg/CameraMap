#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPaintEvent>
#include <QPainter>
#include <QUdpSocket>
QString mPath;
bool camBlinking = false;
CConfig *mConfig= new CConfig;
QList<CCamera*> cameraList;
QTimer *pUpdateTimer;

QUdpSocket *udpSocket;
QThread* mThread ;
MainWindow::MainWindow(QWidget *parent) :dxMap(0),dyMap(0),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //init UI elements
    ui->setupUi(this);
    ui->frame->setHidden(true);
    this->setGeometry(200,200,800,800);
    //Load initial setting from config file
    LoadSettings();
    // init Map Object
    map = new CMap(this);
    map->setCenterPos(mLat,mLon);
    map->setPath(mPath);
    //pUpdateTimer = new QTimer();
    //connect(pUpdateTimer, SIGNAL(timeout()), this, SLOT(updateCameras()));
    //pUpdateTimer->start(1000);
    isPressed = false;
    initCameras();
    mThread = new QThread(this);
    QTimer* timer = new QTimer(0); //parent must be null
    QTimer *pResetTimer = new QTimer(0);
    timer->setInterval(1000);
    timer->start();
    pResetTimer->setInterval(900000);
    pResetTimer->start();
    timer->moveToThread(mThread);
    pResetTimer->moveToThread(mThread);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCameras()));
    connect(pResetTimer, SIGNAL(timeout()), this, SLOT(resetCameras()));
    connect(this, SIGNAL(destroyed()), mThread, SLOT(quit()));
    mThread->start();
    resetCameras();

}
void MainWindow::resetCameras()
{
    system("reset.bat");
}
void MainWindow::updateCameras()
{
    camBlinking = (!camBlinking);
    foreach (CCamera *cam, cameraList)
    {
        cam->requestAzi();
        cam->requestElevation();
        update();
    }
}
int MainWindow::lon2x(double lon)
{
    double refLat = mLat*0.00872664625997f;
    return  ( width()/2.0 + dxMap + ((lon - mLon) * 105.0*cos(refLat))*mScale);
}
int MainWindow::lat2y(double lat)
{
    return (height()/2.0 + dyMap - ((lat - mLat) * 111.31949079327357)*mScale);
}

void MainWindow::initCameras()
{
    CCamera *cam1 = new CCamera(this);
    cam1->setCamName("Camera 1");
    cam1->setIP("192.168.100.100");
    cam1->setAziNorth(mConfig->getDouble("AziNorth1",0));
    cam1->setLat(21.111230);
    cam1->setLon(105.322770);
    cam1->setHeight(mConfig->getDouble("CamHeight1",0.03));
    cam1->setSkipAzi(mConfig->getDouble("SkipAzi1",0));
    cam1->setSkipAziSize(mConfig->getDouble("SkipAziSize1",30));
    cameraList.push_back(cam1);
    mConfig->setValue(cam1->camName(),cam1->toString());
    CCamera *cam2 = new CCamera(this);
    cam2->setCamName("Camera 2");
    cam2->setIP("192.168.100.101");
    cam2->setAziNorth(mConfig->getDouble("AziNorth2",0));
    cam2->setLat(21.125846);
    cam2->setLon(105.322995);
    cam2->setHeight(mConfig->getDouble("CamHeight2",0.04));
    cam2->setSkipAzi(mConfig->getDouble("SkipAzi2",0));
    cam2->setSkipAziSize(mConfig->getDouble("SkipAziSize2",30));
    cameraList.push_back(cam2);
    mConfig->setValue(cam2->camName(),cam2->toString());
    CCamera *cam3 = new CCamera(this);
    cam3->setCamName("Camera 3");
    cam3->setIP("192.168.100.102");
    cam3->setAziNorth(mConfig->getDouble("AziNorth3",0));
    cam3->setLat(21.107606);
    cam3->setLon(105.3304);
    cam3->setHeight(mConfig->getDouble("CamHeight3",0.04));
    cam3->setSkipAzi(mConfig->getDouble("SkipAzi3",0));
    cam3->setSkipAziSize(mConfig->getDouble("SkipAziSize3",30));
    cameraList.push_back(cam3);
    mConfig->setValue(cam3->camName(),cam3->toString());

}
void MainWindow::LoadSettings()
{
    mScale = mConfig->getDouble("mScale",500);
    mPath = mConfig->getString("mPath","C:/Mapdata/");
    mLat = mConfig->getDouble("mLat",21.117);
    mLon = mConfig->getDouble("mLon",105.325);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete mConfig;
    mThread->deleteLater();
}
void MainWindow::drawMap(QPainter *p)
{
    ui->label_scale->setText("OSM scale factor:" + QString::number(map->getScaleRatio()));
    QPixmap pix = map->getImage(mScale);
    p->drawPixmap((width()/2.0-pix.width()/2.0)+dxMap,
                  (height()/2.0-pix.height()/2.0)+dyMap,
                  pix.width(),pix.height(),pix
                  );
    p->setPen(QPen(Qt::white,2));

    // draw the reference 1km line in top left of the map
    if(this->mScale<width()/2)
    {
        p->drawLine(30,10,30+this->mScale,10);
        p->drawText(rect(), Qt::AlignTop|Qt::TextWordWrap,"1 km");
    }
    else if(this->mScale<width())
    {
        p->drawLine(30,10,30+this->mScale/2,10);
        p->drawText(rect(), Qt::AlignTop|Qt::TextWordWrap,"500m");
    }
    else if(this->mScale/5<width()/2)
    {
        p->drawLine(30,10,30+this->mScale/5,10);
        p->drawText(rect(), Qt::AlignTop|Qt::TextWordWrap,"200m");
    }
    else if(this->mScale/10<width()/2)
    {
        p->drawLine(30,10,30+this->mScale/10,10);
        p->drawText(rect(), Qt::AlignTop|Qt::TextWordWrap,"100m");
    }
    else if(this->mScale/20<width()/2)
    {
        p->drawLine(30,10,30+this->mScale/20,10);
        p->drawText(rect(), Qt::AlignTop|Qt::TextWordWrap,"50m");
    }
    p->drawText(rect().adjusted(10,20,0,0), Qt::AlignTop|Qt::TextWordWrap,QString::number(mLat,'f',4));
    p->drawText(rect().adjusted(10,30,0,0), Qt::AlignTop|Qt::TextWordWrap,QString::number(mLon,'f',4));
    // draw the crosshair mark in the center
    int scrCtx = width()/2;
    int scrCty = height()/2;
    drawCrossHairMark(scrCtx,scrCty,p);
}

void MainWindow::drawCameras(QPainter *p)
{

    foreach (CCamera *cam, cameraList) {
        // vẽ vòng tròn tại vị trí của camera
        int cameraX = lon2x(cam->lon());
        int cameraY = lat2y(cam->lat());
        p->setPen(QPen(QColor(255,255,0),2));
        p->drawEllipse(cameraX-8,cameraY-8,16,16);
        p->drawText(cameraX-5,cameraY+15,100,100,0,cam->camName());
        p->drawText(cameraX-5,cameraY+25,100,100,0,"Phương vị: "+QString::number(cam->azi(),'f',1));
        p->drawText(cameraX-5,cameraY+35,100,100,0,"Góc tà: "+QString::number(cam->elevation(),'f',1));
        // gạch chéo nếu camera ko online
        if(!cam->getIsOnline())
        {
            p->drawLine(cameraX-6,cameraY-6,cameraX+6,cameraY+6);
        }
        //vẽ khung rẻ quạt quan sát
        bool isAlarm = cam->alarm()||(!cam->getIsScaning());
        if(camBlinking){
            if(isAlarm)
            {
                p->setPen(QPen(QColor(255,0,0),2));

            }
        }
        int rangeMin =0.5*mScale,rangeMax = 0.8*mScale,rangeCenter;
        double eleMin = cam->elevation()-8.0;
        double eleMax = cam->elevation()+8.0;
        if(eleMin<0)
            rangeMin= cam->getHeight()/tan(-eleMin/DEG2RAD)*mScale;
        else
            rangeMin=5;
        if(eleMax<0)
            rangeMax= cam->getHeight()/tan(-eleMax/DEG2RAD)*mScale;
        else
            rangeMax=5;
        if(cam->elevation()<0)
            rangeCenter= cam->getHeight()/tan(-cam->elevation()/DEG2RAD)*mScale;
        else
            rangeCenter = 5;
        double azi = cam->azi()-8;
        p->drawLine(cameraX+rangeMin*sin(azi/DEG2RAD),cameraY-rangeMin*cos(azi/DEG2RAD),cameraX+rangeMax*sin(azi/DEG2RAD),cameraY-rangeMax*cos(azi/DEG2RAD));
        for(;azi<cam->azi()+8;azi+=1)
        {
            p->drawPoint(cameraX+rangeMin*sin(azi/DEG2RAD),cameraY-rangeMin*cos(azi/DEG2RAD));
            p->drawPoint(cameraX+rangeMax*sin(azi/DEG2RAD),cameraY-rangeMax*cos(azi/DEG2RAD));
        }
        p->drawLine(cameraX+rangeMin*sin(azi/DEG2RAD),cameraY-rangeMin*cos(azi/DEG2RAD),cameraX+rangeMax*sin(azi/DEG2RAD),cameraY-rangeMax*cos(azi/DEG2RAD));
        if(isAlarm)
        {
            QPoint center(cameraX+rangeCenter*sin(cam->azi()/DEG2RAD),cameraY-rangeCenter*cos(cam->azi()/DEG2RAD));
            drawCrossHairMark(center.x(),center.y(),p);
            QPoint scrCenter(width()/2,height()/2);
            double lat,lon;
            map->ConvKmToWGS((center.x()-scrCenter.x())/mScale,(scrCenter.y()-center.y())/mScale,&lat,&lon);

            p->drawText(center.x()-5,center.y()+15,100,100,0,"KĐ: "+QString::number(lat,'f',4));
            p->drawText(center.x()-5,center.y()+25,100,100,0,"VĐ: "+QString::number(lon,'f',4));
        }

    }
}
void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::LocalHost, 8888);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(buffer.data(),buffer.size());

    }
}
void MainWindow::processUdpData(QByteArray buffer)
{
    QString str = QString::fromLatin1(buffer.data());
    QStringList strList = str.split(';');
    if(strList.size()>2)
    {
        if(strList.at(0)=="alarm")
        {
            int camIndex = strList.at(2).toInt();
            bool status  = strList.at(3).toInt();
            Q_ASSERT(camIndex>0);
            Q_ASSERT(camIndex<4);
            cameraList.at(camIndex)->setAlarm(status);
        }
    }
}
void MainWindow::paintEvent(QPaintEvent * e)
{
    e=e;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    drawMap(&p);
    drawCameras(&p);
}
void MainWindow::drawCrossHairMark(int x,int y,QPainter* p)
{
    p->drawLine(x-10,y,x-5,y);
    p->drawLine(x+10,y,x+5,y);
    p->drawLine(x,y-10,x,y-5);
    p->drawLine(x,y+10,x,y+5);
}
void MainWindow::resizeEvent(QResizeEvent *)
{
    map->setImgSize(width(), height());

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    //    if (e->key() != Qt::Key_Z )
    //    {
    //        return;
    //    }
    //    else{
    //        int zoom = (int) map->getScaleRatio();
    //        switch(zoom)
    //        {
    //        case 14: map->setScaleRatio(15);
    //            break;
    //        case 15: map->setScaleRatio(16);
    //            break;
    //        case 16: map->setScaleRatio(14);
    //            break;
    //        default: map->setScaleRatio(15);
    //            break;
    //        }

    //        map->invalidate();
    //        map->UpdateImage();
    //    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

    if(isPressed&&(e->buttons() & Qt::LeftButton)) {
        dxMap = e->x()-pressPos.x();
        dyMap = e->y()-pressPos.y();
        update();
    }
    else
    {
        //show cursor lat-lon
        short   x = this->mapFromGlobal(QCursor::pos()).x() - width()/2;
        short   y = this->mapFromGlobal(QCursor::pos()).y() - height()/2;
        double lat,lon;
        map->ConvKmToWGS(x/mScale,-y/mScale,&lon,&lat);
        ui->lineEdit_cursor_lat->setText(QString::number(lat));
        ui->lineEdit_cursor_lon->setText(QString::number(lon));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{

    if(e->button() != Qt::LeftButton)
    {
        return;
    }
    isPressed = true;
    pressPos = e->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{

    // change center lat-lon of the map

    map->ConvKmToWGS(-(double)dxMap/mScale,(double)dyMap/mScale,&mLon,&mLat);
    map->setCenterPos(mLat,mLon);
    ui->lineEdit_lat->setText(QString::number(mLat,'g',10));
    ui->lineEdit_lon->setText(QString::number(mLon,'g',10));
    dxMap = 0;
    dyMap = 0;
    update();
}


void MainWindow::on_lineEdit_returnPressed()
{
    map->setPath(ui->lineEdit->text());
    update();
}
void MainWindow::wheelEvent(QWheelEvent* event)
{
    if(event->delta()>0)mScale*=1.2;
    if(event->delta()<0)mScale/=1.2;
    if(mScale>8000)mScale = 8000;
    if(mScale<1)mScale = 1;
    update();
    repaint();
}
