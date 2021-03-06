#include "main.h"

float Input[4] = {0};
float Output[4] = {0};

extern float Flow[4];
const float Gains[6] = {
 0.01016679592,   0.1177062541,   0.3721269369,   0.3721269369,   0.1177062541,
    0.01016679592
};

float Buffer[4][ORDER+1];        //采样历史数据
/**
  * @brief  读入当前浮点型，加入FIR滤波队列
  * @param  Input: 滤波对象的当前值     
  * @retval 滤波后最新值
  */
void Fir(float Input[],float Output[4])
{
		unsigned int Index;                //下标索引
		for(int i = 0;i < 4;i ++)
		{
			for(Index=ORDER;Index>0;Index--) Buffer[i][Index]=Buffer[i][Index-1];
			Buffer[i][0]=Input[i];
			//计算输出
			for(Index=0;Index<ORDER+1;Index++)
			{
				Output[i]+=Gains[Index]*Buffer[i][Index];
			}
			Flow[i] = Output[i];
			Output[i] = 0;
		}
        
}
