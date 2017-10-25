#ifndef MV_LOGIC5P_H
#define MV_LOGIC5P_H

#include <QObject>

//**************************************************************
//******           计算基础函数部分                       ******
//**************************************************************
#define		MaxLoopNum	(1000)
#define		Tol1		(0.01)
#define		Tol2		(0.000000001)
#define		Coe			(1.0)
#define  	ABSOLUT_ZERO  (0.0000000000001) ///*绝对0的定义*/
#define		UBound_Double(p) (sizeof(p)/8)

class MV_Logic5P : public QObject
{
    Q_OBJECT
public:
    explicit MV_Logic5P(QObject *parent = 0);

    //计算浓度
    double  Consistence_Logic5P(double fLimta, double fLimtb, double fR0, double fK, double fa, double fb, double fc, double fInR);

private:
    double  DoHalfDiv_Logic5P(double fLimta, double fLimtb, double fR0,double fK, double fa, double fb, double fc, double fInR);



signals:

public slots:

};

#endif // MV_LOGIC5P_H
