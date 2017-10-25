QT       += core
QT       += gui
QT       += sql
QT       += network
QT       += printsupport

CONFIG += qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MV200
TEMPLATE = app

RESOURCES += \
    resource.qrc

RC_FILE = res.rc

DESTDIR += ./

SOURCES += \
    main.cpp \
    AppHelper.cpp \
    FrameDefine.cpp \
    Parameter.cpp \
    Object/Logic5P/MV_Logic5P.cpp \
    Object/SQLite/sqlite.cpp \
    UI/Widget/MessageBox/messageBox.cpp \
    UI/Login/userLogin.cpp \
    Thread/Scheduler.cpp \
    Thread/TcpServer.cpp \
    UI/Widget/ReactionDisk/ReactionDiskWidget.cpp \
    UI/Widget/ReactionDisk/ReactionGraphicsItem.cpp \
    UI/MainWidget/InitWidget.cpp \
    UI/MainWidget/MV200_MainWidget.cpp \
    Thread/ApplicateThread.cpp \
    UI/Widget/Thermometer/Thermometer.cpp \
    UI/MainWidget/CurveWidget/resultCurveWidget.cpp \
    UI/MainWidget/ApplicationWidget/ApplicationWidget.cpp \
    UI/MainWidget/ApplicationWidget/QCApplication.cpp \
    UI/MainWidget/ApplicationWidget/SampleApplication.cpp \
    UI/MainWidget/ApplicationWidget/SonWidget/ApplicationTaskListWidget.cpp \
    UI/MainWidget/ApplicationWidget/SonWidget/SampleDiskInfoWidget.cpp \
    UI/Widget/IPAddressEdit/IPAddressEdit.cpp \
    UI/Widget/QCustomPlot/qcustomplot.cpp \
    UI/Widget/QFlowLayout/QFlowLayout.cpp \
    UI/Widget/SampleDisk/SampleDiskWidget.cpp \
    UI/MainWidget/ProjectSettingWidget/CalculateProjectWidget.cpp \
    UI/MainWidget/ProjectSettingWidget/ProjectGroupWidget.cpp \
    UI/MainWidget/ProjectSettingWidget/QCSettingWidget.cpp \
    UI/MainWidget/ProjectSettingWidget/RoutineProjectSettingWidget.cpp \
    UI/MainWidget/ProjectSettingWidget/TestOrder.cpp \
    UI/MainWidget/ProjectSettingWidget/CardWidget/ReadCareWidget.cpp \
    UI/MainWidget/SysSettingWidget/SysSetting/SysSettingWidget.cpp \
    UI/MainWidget/SysSettingWidget/LISSetting/CommunicationSetWidget.cpp \
    UI/MainWidget/MaintainWidget/SubInterface/AddNewUserWidget.cpp \
    UI/MainWidget/MaintainWidget/UserManagementWidget.cpp \
    UI/MainWidget/MaintainWidget/VersionWidget.cpp \
    UI/MainWidget/MaintainWidget/SubInterface/Save2HistoryProgressBar.cpp \
    UI/MainWidget/ReportWidget/ReportWidget.cpp \
    UI/MainWidget/ReportWidget/SubInterface/HistoryResultWidget.cpp \
    UI/MainWidget/ResultQueryWidget/ResultQueryWidget.cpp \
    UI/MainWidget/ResultQueryWidget/SonWidget/modifyResultWidget.cpp \
    UI/MainWidget/ResultQueryWidget/SonWidget/QcWestgardWidget.cpp \
    UI/MainWidget/ResultQueryWidget/SonWidget/QueryTermsWidget.cpp \
    UI/MainWidget/ResultQueryWidget/QCWidget.cpp \
    UI/MainWidget/ResultQueryWidget/SampleWidget.cpp \
    UI/MainWidget/WorkStatusWidget/reactionDiskInfoWidget.cpp \
    UI/MainWidget/WorkStatusWidget/workStatusWidget.cpp \
    UI/MainWidget/WorkStatusWidget/sampleInfoWidget.cpp \
    UI/MainWidget/WorkStatusWidget/reagentInfoWidget.cpp \
    Thread/LISCommunicateThread.cpp \
    Object/HL7/changetohl7.cpp \
    UI/MainWidget/MaintainWidget/MaintainWidget.cpp \
    UI/MainWidget/MaintainWidget/SubInterface/ErrorMsgWidget/ErrorMsgWidget.cpp \
    UI/MainWidget/MaintainWidget/SubInterface/ErrorMsgWidget/TermsWidget/TermsWidget.cpp \
    UI/Widget/ExcelBase/ExcelBase.cpp \
    Object/ErrorTable/MV_ErrTable.cpp \
    UI/MainWidget/ApplicationWidget/SonWidget/SelectSampleWidget.cpp

HEADERS += \
    AppHelper.h \
    FrameDefine.h \
    ParamDefine.h \
    Parameter.h \
    Object/Logic5P/MV_Logic5P.h \
    Object/SQLite/sqlite.h \
    UI/Widget/MessageBox/messageBox.h \
    UI/Login/userLogin.h \
    Thread/Scheduler.h \
    Thread/TcpServer.h \
    UI/Widget/ReactionDisk/ReactionDiskWidget.h \
    UI/Widget/ReactionDisk/ReactionGraphicsItem.h \
    UI/MainWidget/InitWidget.h \
    UI/MainWidget/MV200_MainWidget.h \
    Thread/ApplicateThread.h \
    UI/Widget/Thermometer/Thermometer.h \
    UI/MainWidget/CurveWidget/resultCurveWidget.h \
    UI/MainWidget/ApplicationWidget/ApplicationWidget.h \
    UI/MainWidget/ApplicationWidget/QCApplication.h \
    UI/MainWidget/ApplicationWidget/SampleApplication.h \
    UI/MainWidget/ApplicationWidget/SonWidget/ApplicationTaskListWidget.h \
    UI/MainWidget/ApplicationWidget/SonWidget/SampleDiskInfoWidget.h \
    UI/Widget/IPAddressEdit/IPAddressEdit.h \
    UI/Widget/QCustomPlot/qcustomplot.h \
    UI/Widget/QFlowLayout/QFlowLayout.h \
    UI/Widget/SampleDisk/SampleDiskWidget.h \
    UI/MainWidget/ProjectSettingWidget/CalculateProjectWidget.h \
    UI/MainWidget/ProjectSettingWidget/ProjectGroupWidget.h \
    UI/MainWidget/ProjectSettingWidget/QCSettingWidget.h \
    UI/MainWidget/ProjectSettingWidget/RoutineProjectSettingWidget.h \
    UI/MainWidget/ProjectSettingWidget/TestOrder.h \
    UI/MainWidget/ProjectSettingWidget/CardWidget/ReadCareWidget.h \
    UI/MainWidget/SysSettingWidget/SysSetting/SysSettingWidget.h \
    UI/MainWidget/SysSettingWidget/LISSetting/CommunicationSetWidget.h \
    UI/MainWidget/MaintainWidget/SubInterface/AddNewUserWidget.h \
    UI/MainWidget/MaintainWidget/UserManagementWidget.h \
    UI/MainWidget/MaintainWidget/VersionWidget.h \
    UI/MainWidget/MaintainWidget/SubInterface/Save2HistoryProgressBar.h \
    UI/MainWidget/ReportWidget/ReportWidget.h \
    UI/MainWidget/ReportWidget/SubInterface/HistoryResultWidget.h \
    UI/MainWidget/ResultQueryWidget/ResultQueryWidget.h \
    UI/MainWidget/ResultQueryWidget/SonWidget/modifyResultWidget.h \
    UI/MainWidget/ResultQueryWidget/SonWidget/QcWestgardWidget.h \
    UI/MainWidget/ResultQueryWidget/SonWidget/QueryTermsWidget.h \
    UI/MainWidget/ResultQueryWidget/QCWidget.h \
    UI/MainWidget/ResultQueryWidget/SampleWidget.h \
    UI/MainWidget/WorkStatusWidget/reactionDiskInfoWidget.h \
    UI/MainWidget/WorkStatusWidget/workStatusWidget.h \
    UI/MainWidget/WorkStatusWidget/sampleInfoWidget.h \
    UI/MainWidget/WorkStatusWidget/reagentInfoWidget.h \
    Thread/LISCommunicateThread.h \
    Object/HL7/changetohl7.h \
    UI/MainWidget/MaintainWidget/MaintainWidget.h \
    UI/MainWidget/MaintainWidget/SubInterface/ErrorMsgWidget/ErrorMsgWidget.h \
    UI/MainWidget/MaintainWidget/SubInterface/ErrorMsgWidget/TermsWidget/TermsWidget.h \
    UI/Widget/ExcelBase/ExcelBase.h \
    Object/ErrorTable/MV_ErrTable.h \
    UI/MainWidget/ApplicationWidget/SonWidget/SelectSampleWidget.h
