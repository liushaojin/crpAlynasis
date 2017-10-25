#include "changetohl7.h"
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include "Parameter.h"

/*
send.at()
0--id
1--样本编号
2--姓名
3--生日
4--性别
5--门诊
6--科室
7--费别（自费）
8--样本结果编号
9--采样时间
10--检验时间
11--送检者
12--病症信息（感冒）
13--送检时间
14--血样模式（静脉血，末梢血）
15--审核时间

while(20--)
16--数据类型（NM）
17--项目编号
18--项目测值
19--单位
20--参考范围
21--结果标志（正常与否）
*/
#define  CR  0X0D
#define  EB  0X1C
#define  SB  0X0B
#define  ORXNUM  32*5
ChangetoHL7::ChangetoHL7( const QStringList & send)
{

    QString OBXnub;
    QStringList duanmessage ;
    int i ;
    QChar cr = CR ;
    QChar eb = EB ;
    QChar sb = SB ;
    QDateTime datetime =  QDateTime::currentDateTime();
    QString QstrDateTime = datetime.toString("yyyyMMddHHmmss");
    sendmessage = new QVector<QStringList> ;

    QString name = gParameter->getCompanyName_En();
    duanmessage.clear();
    sendmessage->clear();
    duanmessage.append(sb);
    //"^~\&"ARM下无法识别“\&”
    duanmessage.append("MSH|^~\&|" +name+ "|" +gParameter->getMachineName()+ "|||"+QstrDateTime+"||ORU^R01|"+send.at(0)+"|"+send.at(1)+"|2.3.1||||0|CHA|UTF-8|||");
    //duanmessage.append("MSH|^~/&|MyVision|MV100|||"+QstrDateTime+"||ORU^R01|"+send.at(0)+"|"+send.at(1)+"|2.3.1||||0|CHA|UTF-8|||");
    duanmessage.append(cr);
    duanmessage.append("PID|1|"+send.at(2)+"|||"+send.at(3)+"||"+send.at(4)+"|"+send.at(5)+"|||||||||||||||||||||||");
    duanmessage.append(cr);
    duanmessage.append("PV1|1|"+send.at(6)+"|"+send.at(7)+"|||||||||||||||||"+send.at(8)+"||||||||||||||||||||||||||||||||");
    duanmessage.append(cr);
    duanmessage.append("OBR|1||"+send.at(9)+"|||"+send.at(10)+"|"+send.at(11)+"|||"+send.at(12)+"|||"+send.at(13)+
                       "|"+send.at(14)+"|"+send.at(15)+"|||||||"+send.at(16)+"||HM||||||||" +name+ "|||||||||||||||");
    duanmessage.append(cr);
    sendmessage->append(duanmessage);
    duanmessage.clear();


    for(i=0 ;i<ORXNUM;i+=5)
    {
        OBXnub = QString::number(i/5+1, 10);

        duanmessage.append("OBX|"+OBXnub+"|"+send.at(i+17)+"|"+send.at(i+18)+"||"+send.at(i+19)+"|"+send.at(i+20)+"|"+send.at(i+21)+
                           "|||F|||||||");
        duanmessage.append(cr);
    }


    sendmessage->append(duanmessage);
    duanmessage.clear();
    duanmessage.append(eb);
    duanmessage.append(cr);

//    QByteArray mapbuffer;
//    tuduan.clear();

//    for( ;i<ORXNUM+4*5;i+=5)
//    {

//        mapbuffer.clear();
//        OBXnub = QString::number(i/5+1, 10);

//        QFile file(send.at(i+19).toUtf8());
//        file.open(QIODevice::ReadOnly);   //在Qt中，文件需要打开了才能进行操作，这点与java不同
//        mapbuffer=file.readAll();

//        mapbuffer = mapbuffer.toBase64();
//        mapbuffer = mapbuffer.toHex();

//        qDebug()<<send.at(i+19).toUtf8() << mapbuffer.size() ;
//        tuduan.append("OBX|");
//        //    for(int a=0;a<block.count();a++)
//        //    qDebug("%d:%c ",a,block.at(a));

//        tuduan.append(OBXnub.toUtf8());
//        tuduan.append(("|"+send.at(i+17)).toUtf8());
//        tuduan.append(("|"+send.at(i+18)).toUtf8());
//        tuduan.append("||");
//        tuduan.append(mapbuffer);
//        tuduan.append(("|"+send.at(i+20)).toUtf8());
//        tuduan.append(("|"+send.at(i+21)).toUtf8());
//        tuduan.append("|||F|||||||");
//        tuduan.append(duanmessage.at(1).toUtf8());


//    }
//    tuduan.append(duanmessage.at(0).toUtf8());
//    tuduan.append(duanmessage.at(1).toUtf8());
//    qDebug() << *sendmessage ;
}

ChangetoHL7::~ChangetoHL7()
{
    delete sendmessage;
}

