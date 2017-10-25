#ifndef CHANGETOHL7_H
#define CHANGETOHL7_H

#include <QObject>

class ChangetoHL7
{
public:
    explicit ChangetoHL7( const QStringList & send);
    ~ChangetoHL7();

     QVector<QStringList> *sendmessage;
     QByteArray tuduan ;

private:


};

#endif // CHANGETOHL7_H
