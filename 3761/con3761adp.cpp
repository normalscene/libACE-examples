/******************************************************************************

                  版权所有 (C), 2010-2015, 国网电力通信与用电技术分公司

 ******************************************************************************
  文 件 名   : con3761adp.c
  版 本 号   : 初稿
  作    者   : liming
  生成日期   : 2013年4月8日 星期一
  最近修改   :
  功能描述   : 集中器376.1协议解析适配层 源文件 
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月8日 星期一
    作    者   : liming
    修改内容   : 创建文件

******************************************************************************/

#include "con3761adp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define   CM_DEG_ON             // 调试开关
#undef    CM_DEG_ON

#ifdef CM_DEG_ON  
#define DEBUG(format,...) printf("File: " __FILE__ ", Line: %d: " format "\n", __LINE__ -1, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif 


bool g_bcmInit = false;         // 初始化
uint8_t g_ucCmPFCSend = 0;        // 当前应该发送的序号
uint8_t g_ucCmPFCRecv = 0;        // 当前应该接收的序号


sCmdMap g_cmdMap[] = 
{
    // {   适配层命令字,   3761命令字  , 封装命令时的主动性} 
    {CM_CMD_UNKOWN,            CMD_AFN_F_UNKOWN,                MT_PRM_UNKWON},
    //afn 00 确认否认
    {CMD_ALL_OK,               CMD_AFN_0_F1_ALL_OK,             MT_PRM_PASIVE},
    {CMD_ALL_DENY,             CMD_AFN_0_F2_ALL_DENY,           MT_PRM_PASIVE},
    {CMD_ONE_BY_ONE,           CMD_AFN_0_F3_ONE_BY_ONE,         MT_PRM_PASIVE},

    //(1)复位命令（AFN=01H）
    {CMD_HARD_INIT,            CMD_AFN_1_F1_HARD_INIT,          MT_PRM_ACTIVE},
    {CMD_DATA_INIT,            CMD_AFN_1_F2_DATA_INIT,          MT_PRM_ACTIVE},
    {CMD_FACTORY_RESET,        CMD_AFN_1_F3_FACTORY_RESET,      MT_PRM_ACTIVE},
    {CMD_PARA_INIT,            CMD_AFN_1_F4_PARA_INIT,          MT_PRM_ACTIVE},

    //(2) 链路接口检测（AFN=02H）
    {CMD_LOG_IN,               CMD_AFN_2_F1_LOG_IN,             MT_PRM_ACTIVE},
    {CMD_LOG_OUT,              CMD_AFN_2_F2_LOG_OUT,            MT_PRM_ACTIVE},
    {CMD_HEART_BEAT,           CMD_AFN_2_F3_HEART_BEAT,         MT_PRM_ACTIVE},

    /* (3) 设置参数（AFN == 04H） */ 
    //{CMD_CFG_NOT_ELEC,         CMD_AFN_3_F1_RELAY_CTRL,       MT_PRM_ACTIVE},
    {CMD_CFG_TML_UP,           CMD_AFN_4_F1_TML_UP_CFG,         MT_PRM_ACTIVE},
    {CMD_CFG_TML_POWER,        CMD_AFN_4_F10_TML_POWER_CFG,     MT_PRM_ACTIVE},
    {CMD_CFG_ASK_1,            CMD_AFN_4_F38_CFG_ASK_1,         MT_PRM_ACTIVE},
    {CMD_CFG_ASK_2,            CMD_AFN_4_F39_CFG_ASK_2,         MT_PRM_ACTIVE},
    {CMD_CFG_AUTO_1,           CMD_AFN_4_F65_CFG_AUTO_1,        MT_PRM_ACTIVE},
    {CMD_CFG_AUTO_2,           CMD_AFN_4_F66_CFG_AUTO_2,        MT_PRM_ACTIVE},
    {CMD_GO_AUTO_1,            CMD_AFN_4_F67_GOP_AUTO_1,        MT_PRM_ACTIVE},
    {CMD_GO_AUTO_2,            CMD_AFN_4_F68_GOP_AUTO_2,        MT_PRM_ACTIVE},

    /* (4) 控制命令(AFN == 05H） */ 
    {CMD_CTRL_CHECK_TIME,      CMD_AFN_5_F31_CHECK_TIME,        MT_PRM_ACTIVE},
    {CMD_PERMIT_TALK_GO,       CMD_AFN_5_F27_PERMIT_TALK_GO,    MT_PRM_PASIVE},
    {CMD_PERMIT_TALK_STOP,     CMD_AFN_5_F35_PERMIT_TALK_STOP,  MT_PRM_PASIVE},
    {CMD_PERMIT_LINK_ON,       CMD_AFN_5_F38_TML_LINK_ON,       MT_PRM_PASIVE},
    {CMD_PERMIT_LINK_OFF,      CMD_AFN_5_F39_TML_LINK_OFF,      MT_PRM_PASIVE},

    {CMD_AUTO_REPORT_GO,       CMD_AFN_5_F29_AUTO_SAY_GO,       MT_PRM_PASIVE},
    {CMD_AUTO_REPORT_STOP,     CMD_AFN_5_F37_AUTO_SAY_STOP,     MT_PRM_PASIVE},

    /* (5) 请求终端配置及信息 (AFN == 09H） */ 
    {CMD_AFN_TML_VERSION,      CMD_AFN_9_F1_TML_VERSION,       MT_PRM_PASIVE},
    {CMD_AFN_TML_INPUT_OUT,    CMD_AFN_9_F2_TML_INPUT_OUT,     MT_PRM_PASIVE},
    {CMD_TML_OTHER_CFG,        CMD_AFN_9_F3_TML_OTHER_CFG,     MT_PRM_PASIVE},
    {CMD_SUPPORT_PARA_CFG,     CMD_AFN_9_F4_SUPPORT_PARA_CFG,  MT_PRM_PASIVE},
    {CMD_SUPPORT_CTRL_CFG,     CMD_AFN_9_F5_SUPPORT_CTRL_CFG,  MT_PRM_PASIVE},
    {CMD_SUPPORT_ASK1_CFG,     CMD_AFN_9_F6_SUPPORT_ASK1_CFG,  MT_PRM_PASIVE},
    {CMD_SUPPORT_ASK2_CFG,     CMD_AFN_9_F7_SUPPORT_ASK2_CFG,  MT_PRM_PASIVE},
    {CMD_SUPPORT_EVNT_CFG,     CMD_AFN_9_F8_SUPPORT_EVNT_CFG,  MT_PRM_PASIVE},


    /* (6) 查询参数 (AFN == 0AH） */ 
    {CMD_CFG_TML_UP_Q,         CMD_AFN_A_F1_TML_UP_CFG,        MT_PRM_PASIVE},
    {CMD_CFG_TML_POWER_Q,      CMD_AFN_A_F10_TML_POWER_CFG,    MT_PRM_PASIVE},
    {CMD_CFG_ASK_1_Q,          CMD_AFN_A_F38_CFG_ASK_1,        MT_PRM_PASIVE},
    {CMD_CFG_ASK_2_Q,          CMD_AFN_A_F39_CFG_ASK_2,        MT_PRM_PASIVE},
    {CMD_CFG_AUTO_1_Q,         CMD_AFN_A_F65_CFG_AUTO_1,       MT_PRM_PASIVE},
    {CMD_CFG_AUTO_2_Q,         CMD_AFN_A_F66_CFG_AUTO_2,       MT_PRM_PASIVE},
    {CMD_GOP_AUTO_1_Q,         CMD_AFN_A_F67_GOP_AUTO_1,       MT_PRM_PASIVE},
    {CMD_GOP_AUTO_2_Q,         CMD_AFN_A_F68_GOP_AUTO_2,       MT_PRM_PASIVE},


    /* (12) 请求1类数据（AFN=0CH） */
    {CMD_TML_CLOCK,            CMD_AFN_C_F2_TML_CLOCK,         MT_PRM_PASIVE},
    {CMD_CURNT_PQUI,           CMD_AFN_C_F25_POWER_RATE_CUR,   MT_PRM_PASIVE},
    {CMD_DC_RLTM_DATA,         CMD_AFN_C_F73_DC_RLTM_DATA,     MT_PRM_PASIVE},
    {CMD_POWER_CUR,            CMD_AFN_C_F31_POWER_CUR,        MT_PRM_PASIVE},
    {CMD_CURT_HARM_VALUE,      CMD_AFN_C_F57_CURT_HARM_VALUE,  MT_PRM_PASIVE},
    {CMD_CURT_HARM_RATE,       CMD_AFN_C_F58_CURT_HARM_RATE,   MT_PRM_PASIVE},
    {CMD_FRTH_HAVE_POWR_C,     CMD_AFN_C_F129_FRTH_HAVE_POWR_C,MT_PRM_PASIVE},
    {CMD_FRTH_NONE_POWR_C,     CMD_AFN_C_F130_FRTH_NONE_POWR_C,MT_PRM_PASIVE},

    /* (13) 请求2类数据（AFN=0DH） */
    {CMD_UBLN_OVER_D,          CMD_AFN_D_F28_UBLN_OVER_D,    MT_PRM_PASIVE},
    {CMD_UBLN_OVER_M,          CMD_AFN_D_F36_UBLN_OVER_M,    MT_PRM_PASIVE},

    /* 读取非电气量*/
    {CMD_NELC_TEMP,            CMD_AFN_C_F17_TOTAL_POWER_HAVE,    MT_PRM_PASIVE},
    {CMD_NELC_HUMi,            CMD_AFN_C_F18_TOTAL_POWER_NONE,    MT_PRM_PASIVE},

    

    // 在这之上添加新的映射
    {CM_CMD_MAX,            CMD_AFN_FN_MAX}
};


#define  vcmFree(pBuf)\
if(pBuf != NULL)\
{\
    free(pBuf);\
    pBuf = NULL;\
}

#if 0
void vcmFree(const char *pMem)
{
    if(pMem != NULL)
    {
        free(pMem);
        pMem = NULL;
    }

}
#endif
/*****************************************************************************
 函 数 名  : ecm_3761_init
 功能描述  : 协议初始化
 输入参数  : sCmInit * psCmInit  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月8日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eCmErr ecm_3761_init(sCmInit * psCmInit)
{
    if(!psCmInit)
    {
        return MT_ERR_NULL;            
    }
      
    sMtInit sInit;
    sInit.eRole = psCmInit->eRole;
    sInit.ucPermitDelayMinutes = psCmInit->ucPermitDelayMinutes;
    emt_err_t eRet =  eMtInit(&sInit);  
    if(eRet == MT_OK)
    {
        g_bcmInit = true;
    }
    
    return eRet;
}


/*****************************************************************************
 函 数 名  : bcm_valid_3761_packet
 功能描述  : 判断一个buff是不是一个有效的3761帧
 输入参数  : uint8_t  *pinBuf  
             uint16_t usLen    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
bool bcm_valid_3761_packet(uint8_t  *pinBuf, uint16_t usLen)
{
    emt_err_t eRet = emtIsValidPack(pinBuf, usLen);

    if(eRet == MT_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*****************************************************************************
 函 数 名  : vcm_set_ec
 功能描述  : 设置事件计数器
 输入参数  : uint8_t ucEC1  重要事件
             uint8_t ucEC2  一般事件
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void vcm_set_ec(uint8_t ucEC1, uint8_t ucEC2)
{
    vmtSetEC(ucEC1, ucEC2);
}

/*****************************************************************************
 函 数 名  : bcm_set_pw
 功能描述  : 设置密码 , 不足16位，前面自动补0 多于16字符，取最前16字符
 输入参数  : char *pw  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
bool bcm_set_pw(const char *pw)
{
    vmtSetPw(pw);
    return true; 
}

/*****************************************************************************
 函 数 名  : ecm_find_frist_valid_pack
 功能描述  : 找到一个有效帧的位置及长度
 输入参数  : uint8_t  *pinBuf       
             uint16_t usLen         
             uint16_t *pusFirstOff  
             uint16_t *pusFirstLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eCmErr ecm_find_frist_valid_pack(uint8_t  *pinBuf, uint16_t usLen, uint16_t *pusFirstOff, uint16_t *pusFirstLen)
{
    emt_err_t eRet = emtFindValidPack(pinBuf, usLen,  pusFirstOff,  pusFirstLen);
    return eRet;
}


/*****************************************************************************
 函 数 名  : ecm_3761_pack
 功能描述  : 协议封装
 输入参数  : sCmPacket* psCmPacket  
             uint8_t * pBuf           
             uint16_t *pusBufLen       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eCmErr ecm_3761_pack(sCmPacket* psCmPacket, uint8_t * pBuf, uint16_t *pusBufLen)
{
    if(!psCmPacket || !pBuf || !pusBufLen)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() pointer is NULL");
        #endif
        return MT_ERR_NULL;
    }

    char * psPackTmp = (char *)malloc(MT_USER_MAX);
    if(!psPackTmp)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() MT_ERR_IO");
        #endif
        return MT_ERR_IO;
    }

    if(g_bcmInit != true)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() MT_ERR_INIT");
        #endif
        vcmFree(psPackTmp);
        return MT_ERR_INIT;
    }
   
    int      i = 0;
    uint8_t    ucCmdNum = 0;
    emt_afn_t   eAfn, eAfnTmp;
    eMtDir   eDir  = MT_DIR_UNKOWN;
    eMtPRM   ePrm  = MT_PRM_UNKWON;
    emt_err_t   eRet  = MT_OK;
    eMtRole  eRole = MT_ROLE_UNKOWN;
    eMtCmd   emtCmd  = CMD_AFN_F_UNKOWN;
    uint8_t    ucPFC = 0;
    smt_pack_t *psPack = (smt_pack_t *)psPackTmp;

    eRole = emtWhoAmI();
    eDir  = (eRole == MT_ROLE_MASTER) ? MT_DIR_M2S : MT_DIR_S2M;

    ucCmdNum =  psCmPacket->ucCmdNum;
    if(ucCmdNum == 0)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() MT_ERR_PARA");
        #endif
        vcmFree(psPackTmp);
        return MT_ERR_PARA;
    } 
 
    // 获取命令类型及主动性
    emtCmd = eget_mt_cmd(psCmPacket->sCmdData[0].eCmd);
    eAfn   = eGetCmdAfn(emtCmd);
    if(AFN_NULL == eAfn)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() eGetCmdAfn() error!");
        #endif
        vcmFree(psPackTmp);
        return MT_ERR_AFN;
    }

    if(psCmPacket->bActive == false)
    {
        ePrm  = ecm_get_prm(psCmPacket->sCmdData[0].eCmd);
        if(MT_PRM_UNKWON == ePrm)
        {
           #ifdef CM_DEG_ON
            DEBUG("ecm_3761_pack() ecm_get_prm() error!");
            #endif
            vcmFree(psPackTmp);
            return MT_ERR_PRM;
        }
    }
    else
    {
        ePrm = (eMtPRM)true;
    }
   
    // 确认帧发送计数
    if(psCmPacket->bReSend == true)
    {
       ucPFC = g_ucCmPFCSend-2;
    }
    else
    {
        ucPFC = g_ucCmPFCSend;
        g_ucCmPFCRecv = g_ucCmPFCSend + 1;
        g_ucCmPFCSend += 2;
    }

    // 构造emtPack()参数
    psPack->sAddress = psCmPacket->sAddress;
    psPack->eAFN     = eAfn;
    psPack->eDir     = eDir;
    psPack->ePRM     = ePrm;
    psPack->ePos     = MT_POS_SIGLE;
    psPack->ucSeq    = ucPFC & 0x0F;
    psPack->bAcdFcb  = bmt_have_ec(eAfn, eDir);
    psPack->bPW      = bmt_have_pw(eAfn, eDir);
    psPack->bEC      = psPack->bAcdFcb ;
    psPack->bTP      = bmt_have_tp(eAfn, eDir);

    // pw
    // 自动取初始化设置的PW
    
    // tp
    // 自动取得当前的时间
    
    // ec
    // 自动取当前设置的事件计数, 请有事件设置 vcm_set_ec

    

    // 调用 emt_pack()
    psPack->usDataNum =ucCmdNum;

    for(i = 0; i < ucCmdNum; i++)
    {
        emtCmd  = eget_mt_cmd(psCmPacket->sCmdData[i].eCmd);
        eAfnTmp = eGetCmdAfn(emtCmd);
        if(eAfnTmp != eAfn)
        {
            #ifdef CM_DEG_ON
            DEBUG("ecm_3761_pack() sub cmds AFN type is not consistent !");
            #endif
            vcmFree(psPackTmp);
            return MT_ERR_AFN;
        }

        psPack->sData[i].eCmd      = emtCmd;  
        psPack->sData[i].usPN      = psCmPacket->sCmdData[i].usPN; 
        eRet = (emt_err_t)emt_trans_appdata_cm2mt(emtCmd, &(psCmPacket->sCmdData[i].uAppData), &(psPack->sData[i].uApp));

        if(eRet != MT_OK)
        {
            #ifdef CM_DEG_ON
            DEBUG("ecm_3761_pack() emt_trans_appdata_cm2mt()!, %d", eRet);
            #endif
            vcmFree(psPackTmp);
            return eRet;
        } 
    }

    // 调用封装api
     eRet = emt_pack(psPack, pusBufLen, pBuf);
     if(eRet != MT_OK)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_pack() emt_pack() Error = %d !", eRet);
        #endif
        vcmFree(psPackTmp);
        return eRet;
    }

    vcmFree(psPackTmp);
    return MT_OK;   
}

/*****************************************************************************
 函 数 名  : ecm_3761_unpack
 功能描述  : 解析报文
 输入参数  : uint8_t * pBuf           
             uint16_t usBufLen        
             sCmPacket* psCmPacket  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月21日 星期二
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eCmErr ecm_3761_unpack(uint8_t * pBuf, uint16_t usBufLen, sCmPacket* psCmPacket)
{
    if(!pBuf || !psCmPacket)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_unpack() pointer is NULL");
        #endif
        return MT_ERR_NULL;
    }

    if(usBufLen < MT_FRM_LEN_MIN)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_unpack() MT_ERR_UNCOMP");
        #endif
        return MT_ERR_UNCOMP;
    }

    // 判断是否为一个有效帧
    emt_err_t eRet = MT_OK;
    int i = 0;
    //uint8_t ucPFC = 0;
    eRet = emtIsValidPack(pBuf, usBufLen);
    if(eRet != MT_OK)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_unpack() is not a valid frame %d", eRet);
        #endif
        return MT_ERR_PACK;
    }

    char *pUnpackTmp = (char*)malloc(MT_USER_MAX);
    if(!pUnpackTmp)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_unpack() MT_ERR_IO");
        #endif
        return MT_ERR_IO;
    }

    smt_pack_t *psmtPack = (smt_pack_t*)pUnpackTmp;

    eRet = emt_unpack(psmtPack, pBuf, usBufLen);
    if(eRet != MT_OK)
    {
        #ifdef CM_DEG_ON
        DEBUG("ecm_3761_unpack() emt_unpack() Error = %d !", eRet);
        #endif
        vcmFree(pUnpackTmp);
        return eRet;
    }

      psCmPacket->sAddress = psmtPack->sAddress;
      //ucPFC    = psmtPack->ucPFC;

      /*
      if(g_ucCmPFCRecv != ucPFC)
      {
         vcmFree(pUnpackTmp);
         return MT_ERR_PFC;
      }

      // 处理tp逻辑
      if(bMtTimeoutTP(&(psmtPack->sTP)) == true)
      {
          vcmFree(pUnpackTmp);
          return MT_ERR_TIMEOUT;
      }

       */
        
    psCmPacket->ucCmdNum = psmtPack->usDataNum;
    for(i = 0; i < psCmPacket->ucCmdNum; i++)
    {
        psCmPacket->sCmdData[i].eCmd     = eget_cm_cmd(psmtPack->sData[i].eCmd);
        psCmPacket->sCmdData[i].usPN     = psmtPack->sData[i].usPN;
        psCmPacket->sCmdData[i].bApp    = psmtPack->sData[i].bApp;
        eRet = (emt_err_t)emt_trans_appdata_mt2cm(psmtPack->sData[i].eCmd, 
												  &(psmtPack->sData[i].uApp), 
												  &(psCmPacket->sCmdData[i].uAppData));
        if(eRet != MT_OK)
        {
            #ifdef CM_DEG_ON
            DEBUG("ecm_3761_unpack() emt_trans_appdata_mt2cm() Error = %d !", eRet);
            #endif
            vcmFree(pUnpackTmp);
            return eRet;
        }
    }
    
    vcmFree(pUnpackTmp);
    return MT_OK;
    
}


/*****************************************************************************
 函 数 名  : ecm_get_cmd_afn
 功能描述  : 获得命令对应的类
 输入参数  : eCmCmd eCmd  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月21日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
emt_afn_t ecm_get_cmd_afn(eCmCmd eCmd)
{
    emt_afn_t eAfn = AFN_NULL;
    eMtCmd emtCmd = eget_mt_cmd(eCmd);
    eAfn = eGetCmdAfn(emtCmd);
    return eAfn;
}

/*****************************************************************************
 函 数 名  : eget_mt_cmd
 功能描述  : cmCMD 到 MtCMD做适配
 输入参数  : eCmCmd cmCMD  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月28日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtCmd eget_mt_cmd(eCmCmd cmCMD)
{
    eMtCmd emtCMD = CMD_AFN_F_UNKOWN;

    int i,size;
    size = sizeof(g_cmdMap)/sizeof(sCmdMap);

    for(i = 0 ; i < size; i++)
    {
        if(cmCMD == g_cmdMap[i].ecmCMD)
        {
            emtCMD = g_cmdMap[i].emtCMD;  
            break;
        }
    }

    #ifdef CM_DEG_ON
    //DEBUG("[in ]eget_mt_cmd() ecmCMD = %d", cmCMD);
    //DEBUG("[out]eget_mt_cmd() emtCMD = 0x%X", emtCMD);
    #endif
    
    return emtCMD;
}

/*****************************************************************************
 函 数 名  : eget_cm_cmd
 功能描述  : mtCMD 到cmCMD 做适配
 输入参数  : eMtCmd mtCMD  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月28日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eCmCmd eget_cm_cmd(eMtCmd mtCMD)
{
 
    eCmCmd ecmCMD = CM_CMD_UNKOWN;
    int i,size;
    size = sizeof(g_cmdMap)/sizeof(sCmdMap);

    for(i = 0 ; i < size; i++)
    {
        if(mtCMD == g_cmdMap[i].emtCMD)
        {
            ecmCMD = g_cmdMap[i].ecmCMD;  
            break;
        }
    }

    return ecmCMD;
}

/*****************************************************************************
 函 数 名  : ecm_get_prm
 功能描述  : 通过适配层的命令，获得其对应的主动性
 输入参数  : eCmCmd cmCMD  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月29日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtPRM ecm_get_prm(eCmCmd cmCMD)
{ 
    eMtPRM ePrm = MT_PRM_UNKWON;
    int i,size;
    size = sizeof(g_cmdMap)/sizeof(sCmdMap);

    for(i = 0 ; i < size; i++)
    {
        if(cmCMD == g_cmdMap[i].ecmCMD)
        {
            ePrm = g_cmdMap[i].ePrm;  
            break;
        }
    }

    return ePrm;
}

/*****************************************************************************
 函 数 名  : emt_trans_appdata_cm2mt
 功能描述  : 将对应命令的适配层应用层数据转换为协议层数据
 输入参数  : eMtCmd emtCmd       
             uCmApp *pcmAppData              
 输出参数  : uMtApp *pmtAppData  
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月9日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtCmd emt_trans_appdata_cm2mt(eMtCmd emtCmd, uCmApp *pcmAppData, uMtApp *pmtAppData)
{
     if(!pcmAppData || !pmtAppData)
     {
          return (eMtCmd)MT_ERR_NULL;
     }

    int i = 0;
    int j = 0;
    eMtCmd emtCmdTmp;
     
    switch(emtCmd)
    {

        case CMD_AFN_0_F3_ONE_BY_ONE:
        {
            pmtAppData->sOneByOne.ucNum = pcmAppData->scmOneByOne.ucNum;
            emtCmdTmp = eget_mt_cmd(pcmAppData->scmOneByOne.sOne[0].eCmd);
            pmtAppData->sOneByOne.eAFN  = eGetCmdAfn(emtCmdTmp);
            for(i = 0; i < pcmAppData->scmOneByOne.ucNum; i++)
            {
                pmtAppData->sOneByOne.sOne[i].bOk= pcmAppData->scmOneByOne.sOne[i].bOk;
                emtCmdTmp = eget_mt_cmd(pcmAppData->scmOneByOne.sOne[i].eCmd);
                pmtAppData->sOneByOne.sOne[i].eCmd = emtCmdTmp;
                pmtAppData->sOneByOne.sOne[i].usPn = pcmAppData->scmOneByOne.sOne[i].usPN;
            }
        }
        break;

        case CMD_AFN_9_F4_SUPPORT_PARA_CFG:
        {
            pmtAppData->sTmlSuptParaCfg.ucNum = pcmAppData->sTmlSuptParaCfg.ucNum;
            for(i = 0; i < pmtAppData->sTmlSuptParaCfg.ucNum; i++)
            {
                pmtAppData->sTmlSuptParaCfg.eCfgCmd[i] = eget_mt_cmd(pcmAppData->sTmlSuptParaCfg.eCfgCmd[i]);
            }
        }
        break;

        case CMD_AFN_9_F5_SUPPORT_CTRL_CFG:
        {
            pmtAppData->sTmlSuptCtrlCfg.ucNum = pcmAppData->sTmlSuptCtrlCfg.ucNum;
            for(i = 0; i < pmtAppData->sTmlSuptCtrlCfg.ucNum; i++)
            {
                pmtAppData->sTmlSuptCtrlCfg.eCfgCmd[i] = eget_mt_cmd(pcmAppData->sTmlSuptCtrlCfg.eCfgCmd[i]);
            }
        }
        break;

        case CMD_AFN_9_F6_SUPPORT_ASK1_CFG:
        {
            pmtAppData->sTmlSuptAsk1.ucNum = pcmAppData->sTmlSuptAsk1.ucNum;
            for(i = 0; i < pmtAppData->sTmlSuptAsk1.ucNum; i++)
            {
                pmtAppData->sTmlSuptAsk1.eCfgCmd[i] = eget_mt_cmd(pcmAppData->sTmlSuptAsk1.eCfgCmd[i]);
            }
        }
        break;


        case CMD_AFN_9_F7_SUPPORT_ASK2_CFG:
        {
            pmtAppData->sTmlSuptAsk2.ucNum = pcmAppData->sTmlSuptAsk2.ucNum;
            for(i = 0; i < pmtAppData->sTmlSuptAsk2.ucNum; i++)
            {
                pmtAppData->sTmlSuptAsk2.eCfgCmd[i] = eget_mt_cmd(pcmAppData->sTmlSuptAsk2.eCfgCmd[i]);
            }
        }
        break;

        case CMD_AFN_4_F38_CFG_ASK_1:
        case CMD_AFN_A_F38_CFG_ASK_1:
        {
            pmtAppData->sAsk1Cfg.ucBig = pcmAppData->sAsk1Cfg.ucBig;
            pmtAppData->sAsk1Cfg.ucM   = pcmAppData->sAsk1Cfg.ucM;
            for(i = 0; i < pmtAppData->sAsk1Cfg.ucM; i++)
            {
                pmtAppData->sAsk1Cfg.sOne[i].ucSmall = pcmAppData->sAsk1Cfg.sOne[i].ucSmall;
                pmtAppData->sAsk1Cfg.sOne[i].ucFN = pcmAppData->sAsk1Cfg.sOne[i].ucFN;
                for(j = 0; j < pmtAppData->sAsk1Cfg.sOne[i].ucFN; j++)
                {
                    pmtAppData->sAsk1Cfg.sOne[i].eCmd[j] = eget_mt_cmd(pcmAppData->sAsk1Cfg.sOne[i].eCmd[j]);
                }
            }
        }
        break;

        case CMD_AFN_4_F39_CFG_ASK_2:
        case CMD_AFN_A_F39_CFG_ASK_2:
        {
            pmtAppData->sAsk2Cfg.ucBig = pcmAppData->sAsk2Cfg.ucBig;
            pmtAppData->sAsk2Cfg.ucM   = pcmAppData->sAsk2Cfg.ucM;
            for(i = 0; i < pmtAppData->sAsk2Cfg.ucM; i++)
            {
                pmtAppData->sAsk2Cfg.sOne[i].ucSmall = pcmAppData->sAsk2Cfg.sOne[i].ucSmall;
                pmtAppData->sAsk2Cfg.sOne[i].ucFN = pcmAppData->sAsk2Cfg.sOne[i].ucFN;
                for(j = 0; j < pmtAppData->sAsk2Cfg.sOne[i].ucFN; j++)
                {
                    pmtAppData->sAsk2Cfg.sOne[i].eCmd[j] = eget_mt_cmd(pcmAppData->sAsk2Cfg.sOne[i].eCmd[j]);
                }
            }
        }
        break;

        case CMD_AFN_4_F65_CFG_AUTO_1:
        case CMD_AFN_A_F65_CFG_AUTO_1:
        {
            pmtAppData->sCfgAuto_1.eUnit    = pcmAppData->sCfgAuto_1.eUnit;
            pmtAppData->sCfgAuto_1.ucPeriod = pcmAppData->sCfgAuto_1.ucPeriod;
            pmtAppData->sCfgAuto_1.sTime    = pcmAppData->sCfgAuto_1.sTime;
            pmtAppData->sCfgAuto_1.ucR      = pcmAppData->sCfgAuto_1.ucR;
            pmtAppData->sCfgAuto_1.ucN      = pcmAppData->sCfgAuto_1.ucN;

            for(i = 0; i < pmtAppData->sCfgAuto_1.ucN; i++)
            {
               pmtAppData->sCfgAuto_1.sCmdPn[i].usPn = pcmAppData->sCfgAuto_1.sCmdPn[i].usPn;
               pmtAppData->sCfgAuto_1.sCmdPn[i].eCmd = eget_mt_cmd(pcmAppData->sCfgAuto_1.sCmdPn[i].eCmd);
               
            }
        }
        break;

        case CMD_AFN_4_F66_CFG_AUTO_2:
        case CMD_AFN_A_F66_CFG_AUTO_2:
        {
            pmtAppData->sCfgAuto_2.eUnit    = pcmAppData->sCfgAuto_2.eUnit;
            pmtAppData->sCfgAuto_2.ucPeriod = pcmAppData->sCfgAuto_2.ucPeriod;
            pmtAppData->sCfgAuto_2.sTime    = pcmAppData->sCfgAuto_2.sTime;
            pmtAppData->sCfgAuto_2.ucR      = pcmAppData->sCfgAuto_2.ucR;
            pmtAppData->sCfgAuto_2.ucN      = pcmAppData->sCfgAuto_2.ucN;

            for(i = 0; i < pmtAppData->sCfgAuto_2.ucN; i++)
            {
               pmtAppData->sCfgAuto_2.sCmdPn[i].usPn = pcmAppData->sCfgAuto_2.sCmdPn[i].usPn;
               pmtAppData->sCfgAuto_2.sCmdPn[i].eCmd = eget_mt_cmd(pcmAppData->sCfgAuto_2.sCmdPn[i].eCmd);
               
            }
        }
        break;

        default:
        *pmtAppData = *(uMtApp*)pcmAppData;
        break;
    }

 return (eMtCmd)MT_OK;
}

/*****************************************************************************
 函 数 名  : emt_trans_appdata_mt2cm
 功能描述  : 将对应协命令的议层数据转换为适配层应用层数据
 输入参数  : eMtCmd emtCmd       
             uMtApp *pmtAppData  
 输出参数  : uCmApp *pcmAppData  
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月9日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtCmd emt_trans_appdata_mt2cm(eMtCmd emtCmd, uMtApp *pmtAppData, uCmApp *pcmAppData)
{
    if(!pcmAppData || !pmtAppData)
    {
        return (eMtCmd)MT_ERR_NULL;
    }

    int i = 0;
    int j = 0;
    eMtCmd emtCmdTmp;
    uint8_t  ucAFN = 0;

    switch(emtCmd)
    {
        case CMD_AFN_0_F3_ONE_BY_ONE:
        {
            pcmAppData->scmOneByOne.ucNum = pmtAppData->sOneByOne.ucNum;
            ucAFN = (uint8_t)pmtAppData->sOneByOne.eAFN;

            #if 0
            #ifdef CM_DEG_ON  
            DEBUG("ucAFN = %d", ucAFN);
            #endif
            #endif

            for(i = 0; i < pmtAppData->sOneByOne.ucNum; i++)
            {
                pcmAppData->scmOneByOne.sOne[i].bOk = pmtAppData->sOneByOne.sOne[i].bOk;
                emtCmdTmp   = pmtAppData->sOneByOne.sOne[i].eCmd;
                pcmAppData->scmOneByOne.sOne[i].eCmd = eget_cm_cmd(emtCmdTmp); 
                pcmAppData->scmOneByOne.sOne[i].usPN = pmtAppData->sOneByOne.sOne[i].usPn;

                #if 0
                #ifdef CM_DEG_ON  
                DEBUG("scmOneByOne[%d] mtCmd = 0x%X", i, emtCmdTmp );
                DEBUG("scmOneByOne[%d] CMD = %d", i,pcmAppData->scmOneByOne.sOne[i].eCmd  );
                DEBUG("scmOneByOne[%d] Pn = %d", i,pcmAppData->scmOneByOne.sOne[i].usPN );
                DEBUG("scmOneByOne[%d] OK = %s", i,pcmAppData->scmOneByOne.sOne[i].bOk == true ? "true" : "false" );
                #endif
                #endif
            }
        }

        break;

        case CMD_AFN_9_F4_SUPPORT_PARA_CFG:
        {
            pcmAppData->sTmlSuptParaCfg.ucNum = pmtAppData->sTmlSuptParaCfg.ucNum;
            for(i = 0; i < pcmAppData->sTmlSuptParaCfg.ucNum; i++)
            {
                pcmAppData->sTmlSuptParaCfg.eCfgCmd[i] = eget_cm_cmd(pmtAppData->sTmlSuptParaCfg.eCfgCmd[i]);
            }

        }
        break; 

        case CMD_AFN_9_F5_SUPPORT_CTRL_CFG:
        {
            pcmAppData->sTmlSuptCtrlCfg.ucNum = pmtAppData->sTmlSuptCtrlCfg.ucNum;
            for(i = 0; i < pcmAppData->sTmlSuptCtrlCfg.ucNum; i++)
            {
                pcmAppData->sTmlSuptCtrlCfg.eCfgCmd[i] = eget_cm_cmd(pmtAppData->sTmlSuptCtrlCfg.eCfgCmd[i]);
            }

        }
        break; 

        case CMD_AFN_9_F6_SUPPORT_ASK1_CFG:
        {
            pcmAppData->sTmlSuptAsk1.ucNum = pmtAppData->sTmlSuptAsk1.ucNum;
            for(i = 0; i < pcmAppData->sTmlSuptAsk1.ucNum; i++)
            {
                pcmAppData->sTmlSuptAsk1.eCfgCmd[i] = eget_cm_cmd(pmtAppData->sTmlSuptAsk1.eCfgCmd[i]);
            }

        }
        break;  

        case CMD_AFN_9_F7_SUPPORT_ASK2_CFG:
        {
            pcmAppData->sTmlSuptAsk2.ucNum = pmtAppData->sTmlSuptAsk2.ucNum;
            for(i = 0; i < pcmAppData->sTmlSuptAsk2.ucNum; i++)
            {
                pcmAppData->sTmlSuptAsk2.eCfgCmd[i] = eget_cm_cmd(pmtAppData->sTmlSuptAsk2.eCfgCmd[i]);
            }

        }
        break; 

        case CMD_AFN_4_F38_CFG_ASK_1:
        {
            pcmAppData->sAsk1Cfg.ucBig = pmtAppData->sAsk1Cfg.ucBig;
            pcmAppData->sAsk1Cfg.ucM   = pmtAppData->sAsk1Cfg.ucM;
            for(i = 0; i < pcmAppData->sAsk1Cfg.ucM; i++)
            {
                pcmAppData->sAsk1Cfg.sOne[i].ucSmall = pmtAppData->sAsk1Cfg.sOne[i].ucSmall;
                pcmAppData->sAsk1Cfg.sOne[i].ucFN = pmtAppData->sAsk1Cfg.sOne[i].ucFN;
                for(j = 0; j < pcmAppData->sAsk1Cfg.sOne[i].ucFN; j++)
                {
                    pcmAppData->sAsk1Cfg.sOne[i].eCmd[j] = eget_cm_cmd(pmtAppData->sAsk1Cfg.sOne[i].eCmd[j]);
                }
            }
        }
        break;

        case CMD_AFN_4_F39_CFG_ASK_2:
        {
            pcmAppData->sAsk2Cfg.ucBig = pmtAppData->sAsk2Cfg.ucBig;
            pcmAppData->sAsk2Cfg.ucM   = pmtAppData->sAsk2Cfg.ucM;
            for(i = 0; i < pcmAppData->sAsk2Cfg.ucM; i++)
            {
                pcmAppData->sAsk2Cfg.sOne[i].ucSmall = pmtAppData->sAsk2Cfg.sOne[i].ucSmall;
                pcmAppData->sAsk2Cfg.sOne[i].ucFN = pmtAppData->sAsk2Cfg.sOne[i].ucFN;
                for(j = 0; j < pcmAppData->sAsk2Cfg.sOne[i].ucFN; j++)
                {
                    pcmAppData->sAsk2Cfg.sOne[i].eCmd[j] = eget_cm_cmd(pmtAppData->sAsk2Cfg.sOne[i].eCmd[j]);
                }
            }
        }
        break;

        case CMD_AFN_4_F65_CFG_AUTO_1:
        {
            pcmAppData->sCfgAuto_1.eUnit    = pmtAppData->sCfgAuto_1.eUnit;
            pcmAppData->sCfgAuto_1.ucPeriod = pmtAppData->sCfgAuto_1.ucPeriod;
            pcmAppData->sCfgAuto_1.sTime    = pmtAppData->sCfgAuto_1.sTime;
            pcmAppData->sCfgAuto_1.ucR      = pmtAppData->sCfgAuto_1.ucR;
            pcmAppData->sCfgAuto_1.ucN      = pmtAppData->sCfgAuto_1.ucN;

            for(i = 0; i < pcmAppData->sCfgAuto_1.ucN; i++)
            {
               pcmAppData->sCfgAuto_1.sCmdPn[i].usPn = pmtAppData->sCfgAuto_1.sCmdPn[i].usPn;
               pcmAppData->sCfgAuto_1.sCmdPn[i].eCmd = eget_cm_cmd(pmtAppData->sCfgAuto_1.sCmdPn[i].eCmd);
            }
        }
        break;

        case CMD_AFN_4_F66_CFG_AUTO_2:
        {
            pcmAppData->sCfgAuto_2.eUnit    = pmtAppData->sCfgAuto_2.eUnit;
            pcmAppData->sCfgAuto_2.ucPeriod = pmtAppData->sCfgAuto_2.ucPeriod;
            pcmAppData->sCfgAuto_2.sTime    = pmtAppData->sCfgAuto_2.sTime;
            pcmAppData->sCfgAuto_2.ucR      = pmtAppData->sCfgAuto_2.ucR;
            pcmAppData->sCfgAuto_2.ucN      = pmtAppData->sCfgAuto_2.ucN;

            for(i = 0; i < pcmAppData->sCfgAuto_2.ucN; i++)
            {
               pcmAppData->sCfgAuto_2.sCmdPn[i].usPn = pmtAppData->sCfgAuto_2.sCmdPn[i].usPn;
               pcmAppData->sCfgAuto_2.sCmdPn[i].eCmd = eget_cm_cmd(pmtAppData->sCfgAuto_2.sCmdPn[i].eCmd);
            }
        }
        break;
        
        default:
        *pcmAppData = *(uCmApp*)pmtAppData;
        break;
    }

    return (eMtCmd)MT_OK;
}


/*****************************************************************************
 函 数 名  : ncm_3761_pack
 功能描述  : 3761适配层封装接口
 输入参数  : sCmPacket* psCmPacket  
             uint8_t * pBuf           
 输出参数  : 无
 返 回 值  : 长度正值为正确, 负值为错误
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月10日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
int32_t ncm_3761_pack(sCmPacket* psCmPacket, uint8_t * pBuf)
{
    int32_t  nRet  = 0;
    eCmErr eRet  = MT_OK;
    uint16_t usLen = 0;

	#if 0
	char buf[64] = {0};
	if(psCmPacket->sCmdData[0].eCmd == CMD_AFN_TML_VERSION)
	{
		
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"usPN = %d\n", psCmPacket->sCmdData[0].usPN);

		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucFactoryID, 4);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucFactoryID = %s\n", buf);

		memset(buf,0,64);
		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucDeviceID, 8);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucDeviceID = %s\n", buf);

		
		memset(buf,0,64);
		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucSoftwareID, 4);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucSoftwareID = %s\n", buf);


		memset(buf,0,64);
		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucInfoCodeCap, 11);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucInfoCodeCap = %s\n", buf);


		memset(buf,0,64);
		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucProtolID, 4);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucProtolID = %s\n", buf);

		memset(buf,0,64);
		memcpy(buf, psCmPacket->sCmdData[0].uAppData.sTmlVersion.ucHardWareID, 4);
		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucHardWareID = %s\n", buf);


		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"sDateSoftware = %02d%02%02d\n",
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucYY,
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucMM,
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucDD);

		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"sDateHardware = %02d%02%02d\n",
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucYY,
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucMM,
		psCmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucDD);

		frwk_Trace(MID_REAL_DATAPROCESS,TRACE_LEVEL_DEBUG,"ucHardWareID = %s\n", buf);


	}
	#endif


	


    eRet = ecm_3761_pack(psCmPacket, pBuf, &usLen);
    if(eRet == MT_OK)
    {
        nRet = (int32_t)usLen;
    }
    else
    {
        nRet = (-1) * eRet;
    }

    return nRet;
}

/*****************************************************************************
 函 数 名  : ncm_3761_unpack
 功能描述  : 解封装上层接口封装
 输入参数  : uint8_t * pBuf           
             uint16_t usBufLen        
             sCmPacket* psCmPacket  
 输出参数  : 无
 返 回 值  : 正值为正确长度, 负值为错误
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月10日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
int32_t ncm_3761_unpack(uint8_t * pBuf, uint16_t usBufLen, sCmPacket* psCmPacket)
{
    int32_t  nRet  = 0;
    eCmErr eRet  = MT_OK;
    eRet = ecm_3761_unpack(pBuf, usBufLen, psCmPacket);

    if(eRet == MT_OK)
    {
        nRet =  1;
    }
    else
    {
        nRet = -1 * eRet;
    }

    return nRet;
}

/*****************************************************************************
 函 数 名  : ncm_3761_check_and_get_pack
 功能描述  : 3761获得第一个有效帧位置及长度
 输入参数  : uint8_t  *pinBuf       
             uint16_t usLen         
             uint16_t *pusFirstOff  
             uint16_t *pusFirstLen  
 输出参数  : 无
 返 回 值  : 0 是正确, 负值是错误
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月10日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
int32_t ncm_3761_check_and_get_pack(uint8_t  *pinBuf, uint16_t usLen, uint16_t *pusFirstOff, uint16_t *pusFirstLen)
{
    eCmErr eRet  = MT_OK;
    eRet = ecm_find_frist_valid_pack(pinBuf, usLen,  pusFirstOff,  pusFirstLen);
    return (int32_t)((-1)*eRet);
}

