#include "MV_ErrTable.h"
#include <QDebug>
#include <QElapsedTimer>


MV_ErrTable *gMV_ErrTable;
MV_ErrTable::MV_ErrTable(QObject *parent) : QObject(parent)
{
    //初始化普通错误表
    initNormalErrT();
    //是否初始化错误列表成功
    initErrTableFlag = OpenErrTable();
}

//是否初始化错误列表成功
int MV_ErrTable::get_initErrTableState() {return initErrTableFlag;}



//初始化普通错误表
void MV_ErrTable::initNormalErrT()
{
    m_NormalErrT.clear();

    //以下作测试用
    QList<QVariant> list;
    //缺少样本
    list.clear();
    list.append("0x41");
    list.append("0x41,0x41,0x41,0x41");
    list.append("缺少样本");
    m_NormalErrT.append(list);

    //缺水
    list.clear();
    list.append("0x43");
    list.append("0x41,0x41,0x41,0x41");
    list.append("缺水");
    m_NormalErrT.append(list);

    //缺试剂
    list.clear();
    list.append("0x45");
    list.append("0x41,0x41,0x41,0x41");
    list.append("缺试剂");
    m_NormalErrT.append(list);
}

//打开错误表
int MV_ErrTable::OpenErrTable()
{
//    QElapsedTimer timer;
//    timer.start();
    QString xlsFile;
    //错误表的绝对路径
    xlsFile = qApp->applicationDirPath() + "/ErrorTable/MV200ErrorTable.xlsx";
    if( !AppHelper::FileIsExist(xlsFile) )
    {
        qDebug() << "文件不存在。。。";
        return 0;
    }
    //qDebug() << "ErrTable Path:" << xlsFile;

    m_SeriousErrT.clear();
    //QScopedPointer类存储了一个指向动态分配对象的指针，一旦析构就会被删除
    QScopedPointer<ExcelBase> m_xls;
    if(m_xls.isNull())
        m_xls.reset(new ExcelBase);

    m_xls->open(xlsFile);
    m_xls->setCurrentSheet(1);
    m_xls->readAll(m_SeriousErrT);

    //
    if( m_SeriousErrT.count() == 0)
        return 0;

    qDebug()<< "错误表长度：" << m_SeriousErrT.count();
    //        for(int i = 0; i < m_SeriousErrT.count(); i ++)
    //        {
    //            qDebug() << m_SeriousErrT.at(i).count() << m_SeriousErrT.at(i);
    //        }
//qDebug()<<"init ErrTable times:"<<timer.elapsed()<<"ms";
    return 1;
}

//#include <QElapsedTimer>
//<严重错误表>查找id单元 no错误号对应的错误描述
QString MV_ErrTable::getSeriousErrDescribe(int id, QString no)
{
    //QElapsedTimer timer;
    //timer.start();

    //qDebug() << "错误id:" << id << ", 错误编号:" << no;
    QString detail = "";

    QString Error;
    QString errNo;
    bool ok; //是否转换成功
    int Tid; //对应在错误表中的ID
    for(int i = 1; i < m_SeriousErrT.count(); i ++)
    {
        //qDebug() << m_SeriousErrT.at(i).at(0).toString() << m_SeriousErrT.at(i).count();
        //qDebug() << id << m_SeriousErrT.at(i).at(0).toString()<< m_SeriousErrT.at(i).at(0).toByteArray();
        //先查找单元号
        Tid = m_SeriousErrT.at(i).at(0).toByteArray().toInt(&ok, 16);
        //qDebug() << ok << Tid << id;
        if( id == Tid )
        {
            //if( m_SeriousErrT.at(i).count() != 3 )
                //continue;

            errNo = "";
            Error = m_SeriousErrT.at(i).at(1).toString();
            for(int j = 0; j < Error.split(",").count(); j ++)
                errNo += QString::number( Error.split(",").at(j).toLatin1().toInt(&ok, 16), 16);

           // qDebug () << id << no << errNo;
            if( no == errNo)
            {
                detail = m_SeriousErrT.at(i).at(2).toString();
                break;  //匹配到对应的错误描述
            }
        }
    }
    //qDebug()<<"check ErrTable times:"<<timer.elapsed()<<"ms";
    return detail;
}

//<普通错误表>查找id单元 no错误号对应的错误描述
QString MV_ErrTable::getNormalErrDescribe(int id, QString no)
{
    //QElapsedTimer timer;
    //timer.start();

    //qDebug() << "错误id:" << id << ", 错误编号:" << no;
    QString detail = "";

    QString Error;
    QString errNo;
    bool ok; //是否转换成功
    int Tid; //对应在错误表中的ID
    for(int i = 1; i < m_NormalErrT.count(); i ++)
    {
        //qDebug() << m_NormalErrT.at(i).at(0).toString() << m_NormalErrT.at(i).count();
        //qDebug() << id << m_NormalErrT.at(i).at(0).toString()<< m_NormalErrT.at(i).at(0).toByteArray();
        //先查找单元号
        Tid = m_NormalErrT.at(i).at(0).toByteArray().toInt(&ok, 16);
        //qDebug() << ok << Tid << id;
        if( id == Tid )
        {
            if( m_NormalErrT.at(i).count() != 3 )
                continue;

            errNo = "";
            Error = m_NormalErrT.at(i).at(1).toString();
            for(int j = 0; j < Error.split(",").count(); j ++)
                errNo += QString::number( Error.split(",").at(j).toLatin1().toInt(&ok, 16), 16);

            if( no == errNo)
            {
                detail = m_NormalErrT.at(i).at(2).toString();
                break;  //匹配到对应的错误描述
            }
        }
    }
    //qDebug()<<"check ErrTable times:"<<timer.elapsed()<<"ms";
    return detail;
}


