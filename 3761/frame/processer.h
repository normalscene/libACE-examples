#ifndef _3761_PROCESS_H_
#define _3761_PROCESS_H_
#include <string>
#include "TcpClient.h"
#include "con3761adp.h"
#include "confirm.h"
#include "frame.h"
using namespace std;

class Processer
{
	uint8_t data[1<<16];
	TcpClient &m_client;
public:
	Processer(TcpClient& client):m_client(client)
	{
	}

	void process(uint8_t *begin, uint8_t* end)
	{
		uint8_t *p = begin;
		uint16_t len = end - begin + 1;
		sCmPacket &pack = *(sCmPacket*)data;

		printf("\nrecv:");
		while(p <= end)
		{
			printf("%02X ", *p++);
		}
		printf("\n");

		eCmErr err = ecm_3761_unpack(begin, len, &pack);
		if(MT_ERR_OK != err)
		{
			printf("unpack error\n");
			show_mt_error(err);
			fflush(stdout);
			return;
		}
		show(begin);
		CommonHeader &head = *(CommonHeader*)begin;
		
		if(head.CON == 1)
		{
			Confirm confirm(m_client);
			confirm.allOk(data);
		}
	}

	void show(uint8_t *buf)
	{
		sCmPacket &pack = *(sCmPacket*)buf;
		CommonHeader com = *(CommonHeader*)buf;

		com.print();
	}

	void show_mt_error(err_t eErr)
	{
		const char *str = NULL;

		switch(eErr)
		{
			case MT_ERR_OK:
				str = "MT_ERR_OK 无错误";
				break;

			case MT_ERR_NULL:
				str = "MT_NULL 指针为空";
				break;
			
			case MT_ERR_OUTRNG:
				str = "MT_OUT_RNG 参数越界";
				break;

			case MT_ERR_NONE:
				str = "MT_NONE 不存在,没有找到";
				break;

			case MT_ERR_IO:
				str = "MT_ERR_IO IO错误";
				break;

			case MT_ERR_RES:
				str = "MT_ERR_RES 资源不足";
				break;

			case MT_ERR_INIT:
				str = "MT_ERR_INIT 没有初始化";
				break;
			
			case MT_ERR_PARA:
				str = "MT_ERR_PARA 参数错误";
				break;

			case MT_ERR_FN:
				str = "MT_ERR_FN 指定Afn 中不支持的 Fn";
				break;

			case MT_ERR_PN:
				str = "MT_ERR_PN 指定Afn Fn 中不支持的 Pn";
				break;

			case MT_ERR_ADDR:
				str = "MT_ERR_ADDR 无效地址";
				break;
			
			case MT_ERR_CTRL:
				str = "MT_ERR_CTRL 控制域错误";
				break;

			case MT_ERR_SEQ:
				str = "MT_ERR_SEQ SEQ域错误";
				break;
			
			case MT_ERR_SEQ2CS:
				str = "MT_ERR_SEQ2CS";
				break;

			case MT_ERR_AFN:
				str = "MT_ERR_AFN AFN 错误";
				break;

			case MT_ERR_UNCOMP:
				str = "MT_ERR_UNCOMP 不完整的协议包";
				break;

			case MT_ERR_0x68:
				str = "MT_ERR_0x68 错误的报文起始";
				break;
			
			case MT_ERR_PROTO:
				str = "MT_ERR_PROTO 错误的协议标识";
				break;
			
			case MT_ERR_CS:
				str = "MT_ERR_CS 错误的检测和";
				break;

			case MT_ERR_0x16:
				str = "MT_ERR_0x16 错误的结尾符";
				break;

			case MT_ERR_LOGIC:
				str = "MT_ERR_LOGIC 错误的处理逻辑";
				break;
			
			case MT_ERR_PACK:
				str = "MT_ERR_PACK 不是一个有效的包";
				break;

			case MT_ERR_PFC:
				str = "序列号不对应";
				break;

			case MT_ERR_TIMEOUT:
				str = "报文超时";
				break;

			case MT_ERR_PRM:
				str = "启动方向错误";
				break;

		   case MT_ERR_TEAM:
				str = "不属于同一组";
				break;

			case MT_ERR_ENCRYPT:
				str = "加密错误";
				break;

			case MT_ERR_DECRYPT:
				str = "解密错误";
				break;

			default:
				str = "未知错误";
				break;
		}
		printf("%s\n", str);
		fflush(stdout);
	}
};

#endif//_3761_PROCESS_H_
