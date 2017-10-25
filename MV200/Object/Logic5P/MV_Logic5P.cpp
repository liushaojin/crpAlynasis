#include "MV_Logic5P.h"
#include <math.h>
#include "stdio.h"
#include <qdebug.h>

MV_Logic5P::MV_Logic5P(QObject *parent) : QObject(parent)
{

}

/*************************************************************************************************************************************
'功能：Logic5P定标方法计算反应度函数(R=R0+K*1/(1+exp(-(a+b*lnC+c*C)))；可以关系式：令：Rr(i)=R(i)-R0，e(i)=exp(-(a+b*lnC(i)))；则：f(i)=K/(1+e(i)))
'参数：
'   fk：定标参数k
'   fa：定标参数a
'   fb：定标参数b
'   fc：定标参数c
'   fR0：参数R0
'   fInC：待求反应幅度的浓度
'   fRetResult：返回结果
'   sRetErrStr：本函数发生错误时，返回错误描述内容
'返回：能正确求解：true；否则：false
**************************************************************************************************************************************/
static char GetResponseVal_Logic5P(double fK, double fa, double fb, double fc, double fr0,  double fInC, double *fRetResult)
{
    double fTemp0, fTemp1;

    if(fInC <=0)
        return 0;
    fTemp0 = fa + fb * log(fInC) + fc * fInC; //a+b*lnC+c*C
    fTemp0 = -fTemp0;         //-(a+b*lnC+c*C)
    fTemp1 = 1 + exp(fTemp0); //1+exp(-(a+b*lnC+c*C))
    fTemp0 = fK / fTemp1;
    *fRetResult = fr0 + fTemp0;

    return 1;
}

/******************************************************************************************************
'功能：Logic5P定标的浓度计算，用二分法解一元二次多项式方程(方程(R=R0+K*1/(1+exp(-(a+b*lnC+c*C)))；
可以关系式：令：Rr(i)=R(i)-R0，e(i)=exp(-(a+b*lnC(i)))；则：f(i)=K/(1+e(i))))
'参数：
'   lMaxLoopNum：最大迭代次数
'   fTol1：参数fTol1(容限1)
'   fTol2：参数fTol2(容限1)
'   fLimta：参数fa(区间下限值)
'   fLimtb：参数fb(区间上限值)
'   fk：定标参数k
'   fa：定标参数a
'   fb：定标参数b
'   fc：定标参数c
'   fR0：参数R0
'   fInR：待求浓度的反应幅度
'   fOutC：返回结果浓度fOutC
'   sRetErrStr：本函数发生错误时，返回错误描述内容
'返回：能正确解方程：true；否则：false
*******************************************************************************************************/
double  MV_Logic5P::Consistence_Logic5P(double fLimta, double fLimtb, double fR0, double fK, double fa, double fb, double fc, double fInR)
{
    return DoHalfDiv_Logic5P(fLimta, fLimtb, fR0, fK, fa, fb, fc, fInR);
}

/******************************************************************************************************
'功能：Logic5P定标的浓度计算，用二分法解一元二次多项式方程(方程(R=R0+K*1/(1+exp(-(a+b*lnC+c*C)))；
可以关系式：令：Rr(i)=R(i)-R0，e(i)=exp(-(a+b*lnC(i)))；则：f(i)=K/(1+e(i))))
'参数：
'   lMaxLoopNum：最大迭代次数
'   fTol1：参数fTol1(容限1)
'   fTol2：参数fTol2(容限1)
'   fLimta：参数fa(区间下限值)
'   fLimtb：参数fb(区间上限值)
'   fk：定标参数k
'   fa：定标参数a
'   fb：定标参数b
'   fc：定标参数c
'   fR0：参数R0
'   fInR：待求浓度的反应幅度
'   fOutC：返回结果浓度fOutC
'   sRetErrStr：本函数发生错误时，返回错误描述内容
'返回：能正确解方程：true；否则：false
*******************************************************************************************************/
double  MV_Logic5P::DoHalfDiv_Logic5P(double fLimta, double fLimtb, double fR0, double fK, double fa, double fb, double fc, double fInR)
{
    long lMaxLoopNum=10000;
    long n;
    double fOutC;
    double fTol1=0.01;
    double fTol2=0;
    double a = fLimta;
    double b = fLimtb;
    double p;
    double fp;
    double fp1;
    double fDeltaX;

    for(n=0; n<=lMaxLoopNum; n++)//到最大迭代次数
    {
        p = (a + b) / 2;
         if(GetResponseVal_Logic5P(fK, fa, fb, fc, fR0, p, &fp) == 0)
        {
            return 0;
        }

        fp = fp - fInR;

        fDeltaX = (b - a) / 2;
        if(fabs(fp) < fTol1 || fDeltaX < fTol2)
        {
            fOutC = p;
            break;
        }
        if(GetResponseVal_Logic5P(fK, fa, fb, fc, fR0, b, &fp1) == 0)
        {
            return 0;
        }

        fp1 = fp1 - fInR;
        if(fp < 0 && fp1 < 0)
        {
            fOutC = b;
            break;
        }
        if(fp*fp1 < 0)
        {
            a = p;
        }
        else
        {
            b = p;
        }
    }
    if(n > lMaxLoopNum)
    {
        fOutC=0;
    }
    return fOutC;
}
