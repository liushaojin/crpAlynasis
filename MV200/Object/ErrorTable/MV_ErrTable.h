#ifndef MV_ERRTABLE_H
#define MV_ERRTABLE_H

#include <QObject>
#include "AppHelper.h"
#include "UI/Widget/ExcelBase/ExcelBase.h"

class MV_ErrTable : public QObject
{
    Q_OBJECT
public:
    explicit MV_ErrTable(QObject *parent = 0);

    int get_initErrTableState(); //是否初始化错误列表成功

    QString getSeriousErrDescribe(int id, QString no); //<严重错误表>查找id单元 no错误号对应的错误描述
    QString getNormalErrDescribe(int id, QString no);  //<普通错误表>查找id单元 no错误号对应的错误描述

private:
    QList< QList<QVariant> > m_SeriousErrT; //严重错误表（停机）
    QList< QList<QVariant> > m_NormalErrT;  //普通错误表（不停机，如缺水、缺样本、缺试剂等）


    void initNormalErrT();   //初始化普通错误表
    int OpenErrTable();      //打开错误表
    int initErrTableFlag;    //是否初始化错误列表成功

signals:

public slots:
};
extern MV_ErrTable *gMV_ErrTable;
#endif // MV_ERRTABLE_H
