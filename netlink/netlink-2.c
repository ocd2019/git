#include <sys/types.h>  
#include <sys/socket.h>  
#include <asm/types.h>  
#include <linux/netlink.h>  
#include <linux/rtnetlink.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <string.h> 
#include <unistd.h>

typedef int HI_S32;

typedef char HI_CHAR;

#define HI_SUCCESS 0
#define HI_FAILURE (-1)

#define GPS_MIN_DATA_LEN 1024

#define GPS_MAX_DATA_LEN 4096

static HI_CHAR g_readMaxBuf[GPS_MAX_DATA_LEN] = {0};

HI_S32 gpsVerifyReadData(HI_CHAR *gpsSrc)
{
	HI_S32 i = 0;
	HI_S32 j = 0;
	HI_S32 sumLen = 0;
	HI_S32 dataLen = 0;
	HI_S32 result1 = 0;
	HI_S32 result2 = 0;
	HI_S32 result2Tmp = 0;
	HI_S32 s32Ret = HI_FAILURE;

	//printf("\ngpsSrc = %s\n",gpsSrc);
	do
	{
		if(NULL != gpsSrc && NULL != strstr(gpsSrc,"*"))
		{
			result1 = gpsSrc[1];

			for(i = 2;'*' != gpsSrc[i];i++)
			{
				result1 ^= gpsSrc[i];

				sumLen++;
			}

			result2 = 0;

			dataLen = strlen(gpsSrc);

			for(j = 2;j > 0 && i < dataLen - 1;j--)
			{
				i++;
				
				if(gpsSrc[i] >= '0' && gpsSrc[i] <= '9') 
				{
					result2Tmp = gpsSrc[i] - 48;
				}
				else if(gpsSrc[i] >= 'A' && gpsSrc[i] <= 'F')
				{
					result2Tmp = gpsSrc[i] - 55;
				}
				else if(gpsSrc[i] >= 'a' && gpsSrc[i] <= 'f')
				{
					result2Tmp = gpsSrc[i] - 87;
				}

				result2 += result2Tmp << (j - 1) * 4;
			}

			if(result1 != result2)
			{
				//printf("result1:%d is not equal to result2:%d\n",result1,result2);
				return HI_FAILURE;
			}
		}
		else
		{
			//printf("The gps data tail %s is incomplete\n",gpsSrc);
			return HI_FAILURE;
		}

	}while(0);

	sumLen += 7;

	return sumLen;
}

  
int main(int argc, char *argv[])  
{  
    HI_S32 i = 0;
	HI_S32 j = 0;
	HI_S32 tlLen = 0;
	HI_S32 tmpLen = 0;
	HI_S32 headLen = 0;
	HI_S32 tailLen = 0;
	HI_S32 offset = 0;
	char *dataBegin = NULL;
	char *dataEnd = NULL;
	HI_S32 s32Ret = HI_FAILURE;
	//HI_CHAR readMinBuf[GPS_MIN_DATA_LEN] = {0};
	
	HI_CHAR readMinBuf[] = 	"$GNGGA,,,,,,0,00,25.5,,,,,,*64\r\n"                                                                                                      
						   	"$GNGLL,,,,,,V,N*7A\r\n"                                                                                                                  
						   	"$GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,1*01\r\n"                                                                                          
						   	"$GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,4*04\r\n"                                                                                          
						   	"$GPGSV,1,1,00,0*65\r\n"                                                                                                                  
						   	"$BDGSV,1,1,00,0*74\r\n"                                                                                                                  
						   	"$GNRMC,,V,,,,,,,,,,N,V*37\r\n"                                                                                                           
						   	"$GNVTG,,,,,,,,,N*2E\r\n"                                                                                                                 
						   	"$GNZDA,,,,,,*56\r\n"
						   	"$GPTXT,01,01,01,ANTENNA OPEN*25\r\n"
						   	"$GNGGA,,,,,,0,00\r\n";

	s32Ret = strlen(readMinBuf);

	//第一步：拷贝数据到大的缓冲区
	strncpy(g_readMaxBuf + strlen(g_readMaxBuf),readMinBuf,s32Ret);

	//第二步：从大的缓冲区里，从头开始找到数据头
	while(i < s32Ret)
	{
		if(readMinBuf[i + 0] == '$' && readMinBuf[i + 1] == 'G' &&	\
		   readMinBuf[i + 2] == 'N' && readMinBuf[i + 3] == 'G' &&	\
		   readMinBuf[i + 4] == 'G' && readMinBuf[i + 5] == 'A')
		{
		   	//验证数据头是否完整
		   	dataBegin = strstr(g_readMaxBuf,"$GNGGA");
			memset(g_readMaxBuf,0,i);								//从大的缓冲区起始位置至数据头部分的无效数据清零
			memcpy(g_readMaxBuf,dataBegin,strlen(dataBegin));		//将后面的数据往大的缓冲区起始位置移动

		   	headLen = gpsVerifyReadData(dataBegin);					//如果没有找到完整的数据头
		  	if(HI_FAILURE == headLen)
		  	{
				printf("\ng_readMaxBuf = %s\n",g_readMaxBuf);
				return HI_FAILURE;
			}

		   	//printf("\nheadLen = %d\n",headLen);
		   
		   	break;
		}

		i++;
	}

	//第三步：从大的缓冲区里，从头开始找到数据尾
	while(j < s32Ret)
	{
		if(readMinBuf[j + 0] == '$' && readMinBuf[j + 1] == 'G' &&	\
		   readMinBuf[j + 2] == 'P' && readMinBuf[j + 3] == 'T' &&	\
		   readMinBuf[j + 4] == 'X' && readMinBuf[j + 5] == 'T')
		{
		   	//验证数据尾是否完整
		   	dataEnd = strstr(g_readMaxBuf,"$GPTXT");
			
		   	tailLen = gpsVerifyReadData(dataEnd);		//如果没有找到完整的数据尾
		   	if(HI_FAILURE == tailLen) 
		   	{
		   		printf("\ng_readMaxBuf = %s\n",g_readMaxBuf);			//如果没有找到完整的数据尾，继续等待后续的数据
		   		return HI_FAILURE;
		   	}

		   	//printf("\ntailLen = %d\n",tailLen);
		   
		   	break;
		}

		j++;
	}

	//第四步：得到大的缓冲区起始位置至一个完整的数据包结尾之间的长度
	tlLen = j + tailLen;

	printf("\ntlLen = %d\n",tlLen);

	//第五步：确定大的缓冲区包括一个完整的数据包后，开始解析
	//gpsParseReadData(g_readMaxBuf);

	//第六步：解析完后把大的缓冲区起始位置至一个完整的数据包部分清零
	memset(g_readMaxBuf,0,tlLen);

	//第七步：将后面的数据往前移
	memcpy(g_readMaxBuf,g_readMaxBuf + tlLen,strlen(g_readMaxBuf + tlLen));

	printf("\n$$$g_readMaxBuf = %s\n",g_readMaxBuf);
  
    return 0; 
}  

