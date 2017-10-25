#ifndef FRAMEDEFINE_H
#define FRAMEDEFINE_H

//通讯格式：帧头、机器型号、帧类型、发送者ID、接收者ID、命令ID、参数列表、校验和、帧尾

/***************帧头 、 帧尾*************************/
#define FRAME_HEAD        0xfa    //帧头
#define FRAME_TAIL        0xFB    //帧尾

/***************机器型号****************************/
#define FRAME_MACHINETYPE 0x01    //机器型号

/***************帧类型****************************/
#define FRAME_TYPE        0x01    //帧类型

/***************帧号****************************/
#define FRAME_NO1         0x01    //帧号


/*******  帧类型名称       类型取值    说明************/
#define FRAME_COMMAND     0x01    //命令帧
#define FRAME_RESPONSE    0x02	  //命令应答帧
#define FRAME_RESULT      0x03	  //命令结果帧
#define FRAME_DATA        0x04    //数据帧
#define FRAME_WARNING     0x05    //警告信息帧

/******* 单元编号ID(发送者/接收者)    取 值    说明************/
#define UNIT_PC                   0x10    //PC
#define UNIT_SINGLECHIP           0x40    //中位机单元
#define UNIT_PROBE_SAMPLE         0x41    //样本针单元
#define UNIT_CLEANING_NEEDLE      0x42    //清洗针单元
#define UNIT_MOTOR_MIX            0x43    //搅拌杆单元(搅拌混合液)
#define UNIT_PROBE_REAGENT        0x44    //试剂针单元
#define UNIT_TRACK                0x45    //轨道单元
#define UNIT_REACTION_DISK        0x46    //反应盘单元
#define UNIT_REAGENT_DISK         0x47    //试剂盘单元
#define UNIT_TEMPERATURE_CTL      0x48    //温控单元
#define UNIT_MOTOR_MIX_SAMPLE     0x49    //样本搅拌单元

/*******  命令ID名称       类型取值    说明************/
#define CMDIT_CONFIG      0x02    //配置指令
#define CMDID_RESET       0x03    //复位指令
#define CMDID_TESTCYCLE   0x04    //测试周期指令
#define CMDID_AD          0x05    //反应盘AD基本操作指令
#define CMDID_CLEANCUP    0x06    //清洗反应杯指令
#define CMDID_READCARD    0x07    //读卡指令

///*******  采光模式       类型取值    说明************/
//#define MODE_NORMAL_LIGHT  0X01   //正常模式
//#define MODE_STATIC_LIGHT  0X02   //静态模式
//#define MODE_DEBUG_LIGHT   0X03   //调试模式


//测试用
#define TEST_61_CUP   "FA 01 01 01 10 46 04 02 3d 01 00 06 04 00 FB"  //反应盘转61个反应杯
#define TEST_CYCLE_AD "FA 01 01 01 10 46 05 01 3c 0b 01 06 03 00 FB"  //采ad数据
#define TEST_01 "FA 01 01 01 10 40 03 00 00 00 00 00 00 01 00 00 00 00 00 00 FB" //反应盘复位
#define TEST_02 "FA 01 01 01 10 40 04 00 01 00 00 00 00 01 02 03 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 2c FB" //反应盘复位（测试专用：加样完成空跑）
#define SINGLE_CHIP_RESET "FA 01 01 01 10 40 03 00 01 01 01 01 01 01 01 00 00 00 00 00 FB" //中位机复位
#define TEST_CYCLE_FRAME  "FA 01 01 01 10 40 04 00 01 03 11 03 11 01 02 03 01 11 01 22 01 33 00 30 01 00 10 00 01 01 00 00 00 00 00 00 00 00 00 00 2c FB"	//周期指令
#define CLEAN_CUP "FA 01 01 01 10 40 06 00 01 00 14 00 0A 72 FB" //清洗反应杯

/*******  命令名称           取值    说明************/
#define LENGHT_RESET        21     //中位机单元--复位指令长度
#define LENGHT_RESETSAMPLEMIX 15   //样本搅拌单元--复位指令长度
#define LENGHT_PERFUSE_SN   15     //样本针单元--液路灌注指令长度
#define LENGHT_PERFUSE_RN   15     //试剂针单元--液路灌注指令长度
#define LENGHT_PERFUSE_CN   15     //清洗针单元--液路灌注指令长度
#define LENGHT_PERFUSE_M    15     //搅拌杆单元--液路灌注指令长度
#define LENGHT_PERFUSE_SM   15     //样本搅拌单元--液路灌注指令长度
#define LENGHT_TESTCYCLE    42     //中位机单元--周期指令长度
#define LENGHT_CLEANCUP     15     //中位机单元--清洗反应杯指令长度
#define LENGHT_READCARD     15     //中位机单元--读卡指令长度
#define LENGHT_RESET_IP     15     //中位机单元--配制ip指令长度
#define LENGHT_CYCLE_AD     15     //反应盘单元--反应盘AD基本操作指令

#define MAX_PERFUSE_TIME    5  //液路最大灌注的时间（秒）

class MVFrame
{
public:
    MVFrame();
    char *getResetCmdFrame();        //复机指令
    char *getResetSampleMix();       //样本搅拌杆复位(样本针、样本搅拌复位到无动作干涉)
    char *getPerfuse_sNeedle(int time);      //灌注*样本针*液路
    char *getPerfuse_rNeedle(int time);      //灌注*试剂针*液路
    char *getPerfuse_cNeedle(int time);      //灌注*清洗针*液路（清洗机构）
    char *getPerfuse_Mix(int time);          //灌注*搅拌*液路（反应盘上的混匀搅拌杆）
    char *getPerfuse_sMix(int time);         //灌注*样本搅拌*液路（样本搅拌杆）

    char *getCleamCupCmd(int isTurn);//清洗反应杯指令
    char *getTestCycleCmd();         //测试周期指令
    char *getTestCycleADCmd();       //反应盘AD基本操作指令
    char *getReadCardCmd(int Sector);//读卡指令（读取Sector扇区的内容）

    //type 0x00:IP地址前两个字节设置
    //     0x01:IP地址后两个字节设置
    char *getSetServerIP(int type, int ip1, int ip2);

private:
    char *CmdFrame; //

};
extern MVFrame *gMVFrame;
#endif // FRAMEDEFINE_H
