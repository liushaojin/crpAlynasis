#include "Thermometer.h"
#include <QPainter>
#include <qdebug.h>

Thermometer::Thermometer(QWidget *parent) : QWidget(parent)
{
    m_value = 0;       //默认值
    m_minValue = 0;    //最小温度
    m_maxValue = 50;   //最大温度

    m_thresholdUp = -20;
    m_thresholdDown = 50;
    m_thresholdFlag = false;
    m_units = QString::fromUtf8("°C", -1);
    m_scaleMajor = 10;
    m_scaleMinor = 5;
    m_borderColor = QColor(127,230,247); //
    m_foreground = Qt::white;
    m_background = QColor(127,230,247);
    m_liquidColor = Qt::red;

    this->setFixedSize(30, 60);
}

void Thermometer::paintEvent(QPaintEvent *)
{
    double scale = 250.0;
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width / 2, height / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width, height);
    painter.scale(side / scale, side / scale);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);
    int nSteps = m_scaleMajor * m_scaleMinor;
    double step = 400.0 / nSteps;

    /* 绘制液体容器圆 */
    painter.save();
    painter.setBrush(m_foreground);
    painter.drawEllipse(-50, 230, 100, -100);
    painter.restore();

    /* 绘制液体容器柱 */
    painter.save();
    painter.setBrush(m_foreground);
    painter.drawRect(-18, 210, 45, -440);
    painter.restore();

    /* 绘制刻度值 */
    painter.save();
    painter.setPen(Qt::black);
    double tmpVal;
    step = 400.0 / m_scaleMajor;
    for (int i = 0, y = 185.0; i <= m_scaleMajor; i++, y -= step)
    {
        tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;
        painter.drawText(50, y, QString( "%1" ).arg(tmpVal));
    }
    painter.restore();

    /* 绘制液体圆 */
    painter.save();
    painter.setBrush(m_liquidColor);
    painter.drawEllipse(-45, 225, 90, -90);
    painter.restore();

    /* 绘制液体柱(指示) */
    painter.save();
    painter.setBrush(m_liquidColor);
    double val =  400.0 /(m_maxValue - m_minValue)*(m_value - m_minValue);
    painter.drawRect(-13, 205, 40, -val - 25);
    painter.restore();
}

void Thermometer::setValue(double val)
{
    if (val > m_maxValue)
        m_value = m_maxValue;
    else if (val < m_minValue)
        m_value = m_minValue;
    else
        m_value = val;

    thresholdManager();
    update();
}

void Thermometer::setMinValue(int val)
{
    m_minValue = val;
    update();
}

void Thermometer::setMaxValue(int val)
{
    m_maxValue = val;
    update();
}

void Thermometer::setThresholdUp(double val)
{
    m_thresholdUp = val;
    thresholdManager();
    update();
}

void Thermometer::setThresholdDown(double val)
{
    m_thresholdDown = val;
    thresholdManager();
    update();
}

void Thermometer::setUnits(QString str)
{
    m_units = str;
    update();
}

void Thermometer::setScaleMajor(int scale)
{
    m_scaleMajor = scale;
    update();
}

void Thermometer::setScaleMinor(int scale)
{
    m_scaleMinor = scale;
    update();
}

void Thermometer::setBorderColor(QColor color)
{
    m_borderColor = color;
    update();
}

void Thermometer::setForeground(QColor color)
{
    m_foreground = color;
    update();
}

void Thermometer::setBackground(QColor color)
{
    m_background = color;
    update();
}

void Thermometer::setLiquidColor(QColor color)
{
    m_liquidColor = color;
    update();
}

void Thermometer::thresholdManager()
{
    if (!m_thresholdFlag)
    {
        if(m_value > m_thresholdUp)
        {
            m_thresholdFlag = true;
            Q_EMIT thresholdAlarm(AlarmUp);
        }
        else if(m_value < m_thresholdDown)
        {
            m_thresholdFlag = true;
            Q_EMIT thresholdAlarm(AlarmDown);
        }

    }
    else if (m_value <= m_thresholdUp && m_value >= m_thresholdDown)
    {
        m_thresholdFlag = false;
        Q_EMIT thresholdAlarm(AlarmNo);
    }
}

