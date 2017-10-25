#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <QObject>
#include <QWidget>

class Q_DECL_EXPORT Thermometer : public QWidget
{
    Q_OBJECT
public:
    //explicit Thermometer(QObject *parent = 0);
    enum AlarmCode {AlarmUp = 1, AlarmDown, AlarmNo};

    explicit Thermometer(QWidget *parent = 0);
    double value() const {return m_value;}
    int minValue() const {return m_minValue;}
    int maxValue() const {return m_maxValue;}
    double thresholdUp() const {return m_thresholdUp;}
    double thresholdDown() const {return m_thresholdDown;}
    QString units()const {return m_units;}
    int scaleMajor() const {return m_scaleMajor;}
    int scaleMinor() const {return m_scaleMinor;}
    QColor borderColor() const {return m_borderColor;}
    QColor foreground() const {return m_foreground;}
    QColor background() const {return m_background;}
    QColor liquidColor() const {return m_liquidColor;}

Q_SIGNALS:
    void thresholdAlarm(enum AlarmCode);

public Q_SLOTS:
    void setValue(double);
    void setMinValue(int);
    void setMaxValue(int);
    void setThresholdUp(double);
    void setThresholdDown(double);
    void setUnits(QString);
    void setScaleMajor(int);
    void setScaleMinor(int);
    void setBorderColor(QColor);
    void setForeground(QColor);
    void setBackground(QColor);
    void setLiquidColor(QColor);

protected:
    void paintEvent(QPaintEvent *event);
    void thresholdManager();

private:
    double m_value;
    int m_minValue;
    int m_maxValue;
    double m_thresholdUp;
    double m_thresholdDown;
    bool m_thresholdFlag;
    QString m_units;
    int m_scaleMajor;
    int m_scaleMinor;
    QColor m_borderColor;
    QColor m_foreground;
    QColor m_background;
    QColor m_liquidColor;
};

#endif // THERMOMETER_H
