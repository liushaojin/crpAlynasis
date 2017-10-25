#include "FrameDefine.h"

MVFrame *gMVFrame;
MVFrame::MVFrame()
{
    //初始化
    CmdFrame = new char[100];
    for(int i = 0; i < 100; i ++)
        CmdFrame[0]  = 0x00;
}

//复位指令
char *MVFrame::getResetCmdFrame()
{
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_SINGLECHIP;     //接收者ID-中位机
    CmdFrame[6]  = CMDID_RESET;         //命令ID-复机指令0x03（以上不可更改）
    CmdFrame[7]  = 0x00;  //指令类型
    CmdFrame[8]  = 0x01;  //样本针、样本搅拌复位。（0x00：不复位，0x01：复位）
    CmdFrame[9]  = 0x01;  //清洗针复位。（0x00：不复位，0x01：复位）
    CmdFrame[10] = 0x01;  //搅拌杆复位。（0x00：不复位，0x01：复位）
    CmdFrame[11] = 0x01;  //试剂针复位。（0x00：不复位，0x01：复位）
    CmdFrame[12] = 0x01;  //样本盘复位。（0x00：不复位，0x01：复位）
    CmdFrame[13] = 0x01;  //反应盘复位。（0x00：不复位，0x01：复位）
    CmdFrame[14] = 0x01;  //试剂盘复位。（0x00：不复位，0x01：复位）
    CmdFrame[15] = 0x00;  //保留
    CmdFrame[16] = 0x00;  //保留
    CmdFrame[17] = 0x00;  //保留
    CmdFrame[18] = 0x00;  //保留
    CmdFrame[19] = 0x00;  //校验和(待计算)
    CmdFrame[20] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}

//样本搅拌杆复位(样本针、样本搅拌复位到无动作干涉)
char *MVFrame::getResetSampleMix()
{
    //"FA 01 01 01 10 49 03 03 00 00 00 00 00 49 FB" //清洗反应杯
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_MOTOR_MIX_SAMPLE;//接收者ID-样本搅拌单元
    CmdFrame[6]  = CMDID_RESET;          //命令ID-复位指令（以上不可更改）
    CmdFrame[7]  = 0x03;  //指令类型(样本针、样本搅拌复位到无动作干涉)
    CmdFrame[8]  = 0x00;  //参数1
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //参数4
    CmdFrame[12] = 0x00;  //参数5
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}


//灌注*样本针*液路
char *MVFrame::getPerfuse_sNeedle(int time)
{
    if(time > MAX_PERFUSE_TIME) time = MAX_PERFUSE_TIME;
    if(time < 0) time = 1;

    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_PROBE_SAMPLE;     //接收者ID-
    CmdFrame[6]  = 0x08;  //命令ID-液路指令
    CmdFrame[7]  = 0x02;  //指令类型-样本针液路灌注
    CmdFrame[8]  = time;  //1~5 S液路灌注
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //保留
    CmdFrame[12] = 0x00;  //保留
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//灌注*试剂针*液路
char *MVFrame::getPerfuse_rNeedle(int time)
{
    if(time > MAX_PERFUSE_TIME) time = MAX_PERFUSE_TIME;
    if(time < 0) time = 1;

    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_PROBE_REAGENT;     //接收者ID-
    CmdFrame[6]  = 0x08;  //命令ID-
    CmdFrame[7]  = 0x02;  //指令类型
    CmdFrame[8]  = time;  //1~5 S液路灌注
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //保留
    CmdFrame[12] = 0x00;  //保留
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//灌注*清洗针*液路（清洗机构）
char *MVFrame::getPerfuse_cNeedle(int time)
{
    if(time > MAX_PERFUSE_TIME) time = MAX_PERFUSE_TIME;
    if(time < 0) time = 1;

    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_CLEANING_NEEDLE;     //接收者ID-
    CmdFrame[6]  = 0x05;  //命令ID-
    CmdFrame[7]  = 0x03;  //指令类型
    CmdFrame[8]  = time;  //参数1
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //保留
    CmdFrame[12] = 0x00;  //保留
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//灌注*搅拌*液路（反应盘上的混匀搅拌杆）
char *MVFrame::getPerfuse_Mix(int time)
{
    if(time > MAX_PERFUSE_TIME) time = MAX_PERFUSE_TIME;
    if(time < 0) time = 1;

    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_MOTOR_MIX;     //接收者ID-
    CmdFrame[6]  = 0x07;  //命令ID-
    CmdFrame[7]  = 0x02;  //指令类型
    CmdFrame[8]  = time;  //1~5 S液路灌注
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //保留
    CmdFrame[12] = 0x00;  //保留
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//灌注*样本搅拌*液路（样本搅拌杆）
char *MVFrame::getPerfuse_sMix(int time)
{
    //FA 01 01 01 10 49 07 05 00 00 00 00 00 62 FB
    if(time > MAX_PERFUSE_TIME) time = MAX_PERFUSE_TIME;
    if(time < 0) time = 1;

    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_MOTOR_MIX_SAMPLE;     //接收者ID-样本搅拌单元
    CmdFrame[6]  = 0x07;  //命令ID-
    CmdFrame[7]  = 0x02;  //指令类型
    CmdFrame[8]  = time;  //1~5 S液路灌注
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //保留
    CmdFrame[12] = 0x00;  //保留
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}


//清洗反应杯指令
char *MVFrame::getCleamCupCmd(int isTurn)
{
    //"FA 01 01 01 10 40 06 00 01 00 14 00 0A 72 FB" //清洗反应杯
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_SINGLECHIP;     //接收者ID-中位机单元
    CmdFrame[6]  = CMDID_CLEANCUP;      //命令ID-清洗反应杯指令（以上不可更改）
    CmdFrame[7]  = 0x00;  //指令类型
    CmdFrame[8]  = isTurn;  //1:反应盘转动  0:反应盘不转动
    CmdFrame[9]  = 0x00;  //清水吸液量高7位 （单位ul）
    CmdFrame[10] = 0x14;  //清水吸液量低7位 （清水量0表示不清洗反应盘）
    CmdFrame[11] = 0x00;  //清洗液吸液量高7位（单位ul）
    CmdFrame[12] = 0x0A;  //清洗液吸液量低7位
    CmdFrame[13] = 0x72;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//测试周期指令
char *MVFrame::getTestCycleCmd()
{
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_SINGLECHIP;     //接收者ID-中位机
    CmdFrame[6]  = CMDID_TESTCYCLE;     //命令ID-测试周期指令0x04（以上不可更改）
    CmdFrame[7]  = 0x00;  //指令类型
    CmdFrame[8]  = 0x01;  //当前反应盘的杯位（0表示不转反应盘）
    CmdFrame[9]  = 0x00;  //清水吸液量高7位
    CmdFrame[10] = 0x00;  //清水吸液量低7位（0不动作）
    CmdFrame[11] = 0x00;  //清液吸液量低7位
    CmdFrame[12] = 0x00;  //清液吸液量低7位（0不动作）
    CmdFrame[13] = 0x00;  //第一试剂吸液位（0不转试剂盘）
    CmdFrame[14] = 0x00;  //第二试剂吸液位（0不转试剂盘）
    CmdFrame[15] = 0x00;  //第三试剂吸液位（0不转试剂盘）
    CmdFrame[16] = 0x00;  //第一试剂吸液量高7位（单位ul）
    CmdFrame[17] = 0x00;  //第一试剂吸液量低7位（吸液量0表示不吸第一试剂）
    CmdFrame[18] = 0x00;  //第二试剂吸液量高7位（单位ul）
    CmdFrame[19] = 0x00;  //第二试剂吸液量低7位（吸液量0表示不吸第二试剂）
    CmdFrame[20] = 0x00;  //转移液体量高7位（单位ul）
    CmdFrame[21] = 0x00;  //转移液体量低7位（转移量0表示不转移液体）
    CmdFrame[22] = 0x00;  //第三试剂吸液量高7位（单位ul）
    CmdFrame[23] = 0x00;  //第三试剂吸液量低7位（吸液量0表示不吸第三试剂）
    CmdFrame[24] = 0x00;  //1—10推到当前试管架对应杯位；11-20推到下一条试管架对应杯位；21自动推试管模式；22推出当前试管架 0不推试管。
    CmdFrame[25] = 0x00;  //样本吸液量高7位（单位ul）
    CmdFrame[26] = 0x00;  //样本吸液量低7位 （吸液量0表示不吸样本）
    CmdFrame[27] = 0x00;  //1全血搅拌， 0不搅拌
    CmdFrame[28] = 0x00;  //1打开AD采集， 0关闭AD采集
    CmdFrame[29] = 0x00;  //1打开条码扫描， 0关闭条码扫描
    CmdFrame[30] = 0x00;  //0推到样本位  1 推到搅拌位
    CmdFrame[31] = 0x00;  //保留
    CmdFrame[32] = 0x00;  //保留
    CmdFrame[33] = 0x00;  //保留
    CmdFrame[34] = 0x00;  //保留
    CmdFrame[35] = 0x00;  //保留
    CmdFrame[36] = 0x00;  //保留
    CmdFrame[37] = 0x00;  //保留
    CmdFrame[38] = 0x00;  //保留
    CmdFrame[39] = 0x00;  //保留
    CmdFrame[40] = 0x00;  //校验和(待计算)
    CmdFrame[41] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
//反应盘AD基本操作指令
char *MVFrame::getTestCycleADCmd()
{
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_REACTION_DISK;  //接收者ID-反应盘单元
    CmdFrame[6]  = CMDID_AD;            //命令ID-反应盘AD基本操作指令0x05（以上不可更改）
    CmdFrame[7]  = 0x01;  //指令类型
    CmdFrame[8]  = 0x3c;  //参数1
    CmdFrame[9]  = 0x0b;  //参数2
    CmdFrame[10] = 0x01;  //参数3
    CmdFrame[11] = 0x06;  //参数4
    CmdFrame[12] = 0x03;  //参数5
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}

//读卡指令（读取Sector扇区的内容）
char *MVFrame::getReadCardCmd(int Sector)
{
    //FA 01 01 01 10 40 07 00 01 00 00 00 00 00 FB
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_SINGLECHIP;     //接收者ID-中位机单元
    CmdFrame[6]  = CMDID_READCARD;      //命令ID-读卡指令0x07（以上不可更改）
    CmdFrame[7]  = 0x00;  //指令类型
    CmdFrame[8]  = Sector;  //扇区号(0~15)
    CmdFrame[9]  = 0x00;  //参数2
    CmdFrame[10] = 0x00;  //参数3
    CmdFrame[11] = 0x00;  //参数4
    CmdFrame[12] = 0x00;  //参数5
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}


//type 0x00:IP地址前两个字节设置
//     0x01:IP地址后两个字节设置
char *MVFrame::getSetServerIP(int type, int ip1, int ip2)
{
    CmdFrame[0]  = FRAME_HEAD;          //帧头
    CmdFrame[1]  = FRAME_MACHINETYPE;   //机器类型
    CmdFrame[2]  = FRAME_TYPE;          //帧类型
    CmdFrame[3]  = FRAME_NO1;           //帧号
    CmdFrame[4]  = UNIT_PC;             //发送者ID-PC
    CmdFrame[5]  = UNIT_SINGLECHIP;     //接收者ID-中位机单元
    CmdFrame[6]  = CMDIT_CONFIG;        //命令ID-配置指令0x02（以上不可更改）
    CmdFrame[7]  = type;  //指令类型
    CmdFrame[8]  = ip1/128;  //参数2
    CmdFrame[9]  = ip1%128;  //参数2
    CmdFrame[10] = ip2/128;  //参数3
    CmdFrame[11] = ip2%128;  //参数4
    CmdFrame[12] = 0x00;     //参数5
    CmdFrame[13] = 0x00;  //校验和(待计算)
    CmdFrame[14] = FRAME_TAIL;  //帧尾
    return CmdFrame;
}
