#ifndef PARAMDEFINE_H
#define PARAMDEFINE_H

#define MAX_AD_COUNT   50   //最多能保存的AD个数

#define REACTION_CUP_COUNT   60  //反应杯的个数
#define SAMPLESLOT_COUNT     10  //每个样本架的样本个数

#define SAMPLE_COUNT         30  //样本个数（30个普通样本）         ??不属于MV200，MV100特有
#define URGENT_COUNT          2  //急诊样本个数（2个急诊样本）       ??不属于MV200，MV100特有

#define REAGENT_COUNT           24  //试剂仓上试剂槽的个数
#define PROJECT_COUNT           40  //项目的上限数
#define COMBINATIONITEMCOUNT    30  //组合项目的上限
#define CALCULATE_PROJECT_COUNT 30  //计算项目的上限个数

#define CHECK_MV100_TIMES_DRAIN     10*60/13  //空闲状态查询到n次后要排一次试剂仓的冷凝水(10min排一次)     ??不属于MV200，MV100特有

#define ROWCOUNT 24              //表格行数

//试剂余量计算
#define BOTTLE_H    8000       //试剂瓶高8cm
#define BOTTLE_V   50000       //试剂瓶容量50ml

#endif // PARAMDEFINE_H

