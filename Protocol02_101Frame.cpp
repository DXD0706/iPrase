/////////////////////////////////////////////////////////////////////
// �ļ���                  :    Protocol02_101Frame.cpp
//-------------------------------------------------------------------
// Ŀ�ļ���Ҫ����          :    02��101Э����������͵Ĵ�����ʵ��
//-------------------------------------------------------------------
// ��������                :    2009.9.3
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protocol101.h"

CProtocol02_101::CProtocol02_101()
{

}
CProtocol02_101::CProtocol02_101(int COT_Num,int ASDUA_Num,int InfoOBJ_Num)
{
	COT_lenNum = COT_Num;
	ASDUA_lenNum = ASDUA_Num;
	InfoOBJ_lenNum = InfoOBJ_Num;
}
CProtocol02_101::~CProtocol02_101(void)
{

}
CString CProtocol02_101::Pmain02_101(unsigned char *pdata, int len)
{
	unsigned char ucData[1000],iSumcheck;
	memcpy(ucData,pdata,len);
	CString dirstr,fcstr,strASDU,StrASDUtype,StrOBJ,Strtemp,Str02_101,StrDatahead,StrData="",StrCOMAddr;

	strSumcheck.Empty();
	
	if (ucData[len-1]==0x16)
	{
		for (int k=0;k<6;k++)
		{
			Strtemp = StrData;
			StrData.Format("%s%02XH ",Strtemp,ucData[k]);
		}
		StrDatahead.Format("<%s>��ʼ�ַ�(%XH) �����ַ�(%XH)",StrData,ucData[0],ucData[len-1]);
		
		if ((len<6)&&(ucData[0]==0x10))//�̱���
		{
			iSumcheck=Add_Sum_Check(&ucData[1],2);
			if (ucData[3]!=iSumcheck)
			{
				strSumcheck.Format("(%XH)��У�����",iSumcheck);
			}
			control_code = ucData[1];
			linkaddr= ucData[2];
			DIR = control_code>>7;
			fun_code = control_code&0x0f;
			if (DIR==0)
			{
				dirstr="����";
			}
			else
			{
				dirstr="����";
			}
			fcstr = ParsePLDU_FCCode(DIR,fun_code);
			
			Str02_101.Format("��%sվ(%d)%s��",dirstr,linkaddr,fcstr);
		}
		if (len>6&&(ucData[0]==0x68)&&(ucData[3]==0x68))//������
		{
			if (ucData[1]==ucData[2])
			{
				Strtemp = StrDatahead;
				StrDatahead.Format("%s ���ݵ�Ԫ����(%d) ",Strtemp,ucData[1]);
			}
			iSumcheck=Add_Sum_Check(&ucData[4],len-6);
  			if (ucData[len-2]!=iSumcheck)
			{
				strSumcheck.Format("(%XH)��У�����",iSumcheck);
			}
			control_code = ucData[4];
			linkaddr= ucData[5];
			DIR = control_code>>7;
			fun_code = control_code&0x0f;
			if (DIR==0)
			{
				dirstr="����";
			}
			else
			{
				dirstr="����";
			}
			fcstr = ParsePLDU_FCCode(DIR,fun_code);

			ASDU asdu;
			
			asdu.RES1	= ((ucData[4]&0x20)!=0) ? 1 : 0;
			asdu.RES2	= ucData[5];			
			asdu.TYPE_ID= ucData[6];			
			asdu.VSQ.SQ	= ((ucData[7]&0x80)!=0) ? 1 : 0;
			asdu.VSQ.N	= ucData[7]&0x7F;			
			asdu.COT.T	= ((ucData[8]&0x80)!=0) ? 1 : 0;
			asdu.COT.PN	= ((ucData[8]&0x40)!=0) ? 1 : 0;
			asdu.COT.Cot= ucData[8]&0x3F;
			if (COT_lenNum==2)
			{
				asdu.COT.C_ADDR = ucData[9];
			}
			asdu.ASDUAddr[1] =0;
			for (int i=0;i<ASDUA_lenNum;i++)
			{
				asdu.ASDUAddr[i] = ucData[8+COT_lenNum+i];
			}
			StrCOMAddr.Format("������ַ(%d)",(asdu.ASDUAddr[1]<<8)|asdu.ASDUAddr[0]);
			strCOT=ParseASDU_COT(asdu.COT);
			strVSQ=ParseASDU_VSQ(asdu.VSQ);

			StrData ="";
			for (k=0;k<2+COT_lenNum+ASDUA_lenNum;k++)
			{		
				Strtemp = StrData;
				StrData.Format("%s%02XH ",Strtemp,ucData[6+k]);
			}

			asdu.data.clear();
			for (i=8+COT_lenNum+ASDUA_lenNum;i<len-2;i++)
			{
				asdu.data.push_back(ucData[i]);
			}
			//if(len>11)		asdu.data.insert(asdu.data.end(),data->pData.begin()+10,data->pData.end()-2);//��У���
			switch(asdu.TYPE_ID)
			{
			case _101_M_SP_NA_1:	//type=1 ����ʱ��ĵ�����Ϣ---��ѯ������͵ĵ�����Ϣ
				StrASDUtype = "M_SP_NA_1 ��ʱ�굥��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_SP_NA_1(asdu);
				break;
			case _101_M_SP_TA_1:	//type=2 ��ʱ��ĵ�����Ϣ---��ѯ������͵ĵ�����Ϣ
				StrASDUtype = "M_SP_TA_1 ��ʱ�굥��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TA_1(asdu);
				break;
			case _101_M_DP_NA_1:	//type=3 ����ʱ���˫����Ϣ---��ѯ������͵�˫����Ϣ
				StrASDUtype = "M_DP_NA_1 ��ʱ��˫��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_DP_NA_1(asdu);
				break;
			case _101_M_DP_TA_1:	//type=4 ��ʱ���˫����Ϣ---��ѯ������͵�˫����Ϣ
				StrASDUtype = "M_DP_TA_1 ��ʱ��˫��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TA_1(asdu);
				break;

			case _101_M_ME_NA_1:	//type = 9  --����ֵ--��һ��
				StrASDUtype = "M_ME_NA_1 ��һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NA_1(asdu);
				break;	
			case _101_M_ME_TA_1:	//type = 10 ����ֵ--��ʱ��Ĺ�һ��ֵCP24Time2a
				StrASDUtype = "M_ME_TA_1 ��ʱ���һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TA_1(asdu);
				break;
			case _101_M_ME_NB_1:	//type=11  ����ֵ--��Ȼ�---��ѯ������͵�ң����Ϣ
				StrASDUtype = "M_ME_NB_1 ��Ȼ�ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NB_1(asdu);
				break;
			case _101_M_ME_TB_1:	//type=12  ����ֵ--��ʱ��ı�Ȼ�ֵ---ң��Խ��
				StrASDUtype = "M_ME_TB_1 ��ʱ���Ȼ�ֵ";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TB_1(asdu);
				break;

			case _101_M_IT_NA_1:	//type=15 �ۼ���
				StrASDUtype = "M_IT_NA_1 �������";
				StrOBJ = ParseASDU_IEC101_02_M_IT_NA_1(asdu);
				break;	
				
			case _101_M_ME_ND_1:	//type=21 ����Ʒ�������ʵĹ�һ��ֵ
				StrASDUtype = "M_ME_ND_1 ң������";
				StrOBJ = ParseASDU_IEC101_02_M_ME_ND_1(asdu);
				break;
			case _101_M_SP_TB_1:	//type=30 ��ʱ��ĵ�����Ϣ---ң�ű�λ�ĵ�����ϢCP56Time2a
				StrASDUtype = "M_SP_TB_1 ����ң�ű�λ����";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TB_1(asdu);
				break;
			case _101_M_DP_TB_1:	//type=31 ��ʱ���˫����Ϣ---ң�ű�λ��˫����ϢCP56Time2a
				StrASDUtype = "M_DP_TB_1 ˫��ң�ű�λ����";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TB_1(asdu);
				break;


			case _101_M_ME_TD_1:	//type=34  ����ֵ--��ʱ��Ĺ�һ��ֵ---ң��Խ��CP56Time2a
				StrASDUtype = "M_ME_TD_1 ��ʱ���һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TD_1(asdu);
				break;
			case _101_M_ME_TE_1:	//type=35  ����ֵ--��ʱ��ı�Ȼ�ֵ---ң��Խ��CP56Time2a
				StrASDUtype = "M_ME_TE_1 ��ʱ���Ȼ�ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TE_1(asdu);
				break;
			case _101_M_IT_TB_1:	//type=37  ��ʱ����ۼ���
				StrASDUtype = "M_IT_TB_1 �������";
				StrOBJ = ParseASDU_IEC101_02_M_IT_TB_1(asdu);
				break;

			case _101_C_SC_NA_1:	//type=45  ����ң��
				StrASDUtype = "C_SC_NA_1 ����ң������";
				StrOBJ = ParseASDU_IEC101_02_C_SC_NA_1(asdu);
				break;
			case _101_C_DC_NA_1:	//type=46  ˫��ң������
				StrASDUtype = "C_DC_NA_1 ˫��ң������";
				StrOBJ = ParseASDU_IEC101_02_C_DC_NA_1(asdu);
				break;
			case _101_C_RC_NA_1:	//type=47  ң������
				StrASDUtype = "C_RC_NA_1 ң������";
				StrOBJ = ParseASDU_IEC101_02_C_RC_NA_1(asdu);
				break;
				
			case _101_C_SE_NB_1:    //type=49  �趨����
				StrASDUtype = "C_SE_NB_1 �趨��Ȼ�ֵ";
				StrOBJ = ParseASDU_IEC101_02_C_SE_NB_1(asdu);
				break;

			case _101_M_EI_NA_1:	//type = 70 ��ʼ������
 				StrASDUtype = "M_EI_NA_1 ��ʼ������";
				StrOBJ = ParseASDU_IEC101_02_M_EI_NA_1(asdu);
				break;
			case _101_C_IC_NA_1:    //type = 100 ��������
				StrASDUtype = "C_IC_NA_1 ��������";
				StrOBJ = ParseASDU_IEC101_02_C_IC_NA_1(asdu);
				break;
			case _101_C_CI_NA_1:    //type = 101 ��������
				StrASDUtype = "C_CI_NA_1 �ٵ��";
				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
				break;
// 			case _101_C_RD_NA_1:    //type = 102 ������
// 				StrASDUtype = "C_RD_NA_1 ������";
// 				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
// 				break;
			case _101_C_CS_NA_1:    //type = 103 ʱ��ͬ��
				StrASDUtype = "C_CS_NA_1 ʱ��ͬ��";
				StrOBJ = ParseASDU_IEC101_02_C_CS_NA_1(asdu);
				break;

			case _101_P_ME_NB_1:    //type = 103 ʱ��ͬ��
				StrASDUtype = "P_ME_NB_1 ����������Ȼ�";
				StrOBJ = ParseASDU_IEC101_02_P_ME_NB_1(asdu);
				break;

// 			case _101_C_CI_NA_1:
// 				ProcessFunction_IEC101_02_C_CI_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_DD;
// 				break;
// 			case _101_F_FR_NA_1://�ļ�׼������
// 				ProcessFunction_IEC101_02_F_FR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SR_NA_1://��׼������
// 				ProcessFunction_IEC101_02_F_SR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_LS_NA_1://���ĶΡ���
// 				ProcessFunction_IEC101_02_F_LS_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SG_NA_1://�δ���
// 				ProcessFunction_IEC101_02_F_SG_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_DR_TA_1://Ŀ¼
// 				ProcessFunction_IEC101_02_F_DR_TA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_ASDU_129:	//�Լ챨��
// 				ProcessFunction_IEC101_02_ASDU_129(pStationObj,asdu);
// 				break;
// 			case _101_ASDU_130:	//�¼�����
// 				ProcessFunction_IEC101_02_ASDU_130(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_131:	//���ϱ���
// 				ProcessFunction_IEC101_02_ASDU_131(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_132:	//����¼��
// 				ProcessFunction_IEC101_02_ASDU_132(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_133:
// 				ProcessFunction_IEC101_02_ASDU_133(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_134:
// 				ProcessFunction_IEC101_02_ASDU_134(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_135:
// 				ProcessFunction_IEC101_02_ASDU_135(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_136:
// 				ProcessFunction_IEC101_02_ASDU_136(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_137:
// 				ProcessFunction_IEC101_02_ASDU_137(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_138:
// 				ProcessFunction_IEC101_02_ASDU_138(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_139:
// 				ProcessFunction_IEC101_02_ASDU_139(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_140:
// 				ProcessFunction_IEC101_02_ASDU_140(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_141:
// 				ProcessFunction_IEC101_02_ASDU_141(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_142:
// 				ProcessFunction_IEC101_02_ASDU_142(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_143:
// 				ProcessFunction_IEC101_02_ASDU_143(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_144:
// 				ProcessFunction_IEC101_02_ASDU_144(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_145://������ֵ
// 				ProcessFunction_IEC101_02_ASDU_145(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_146:
// 				ProcessFunction_IEC101_02_ASDU_146(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_147://����ֵ�л�
// 				ProcessFunction_IEC101_02_ASDU_147(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_148:
// 				ProcessFunction_IEC101_02_ASDU_148(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_149:
// 				ProcessFunction_IEC101_02_ASDU_149(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_150:
// 				ProcessFunction_IEC101_02_ASDU_150(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
			
			default:StrASDUtype.Format("(%d)��������δ����",asdu.TYPE_ID);
				break;
			}

			strASDU.Format("��<%s>(%d):%s %s %s %s��%s",StrData,asdu.TYPE_ID,StrASDUtype,strVSQ,strCOT,StrCOMAddr,StrOBJ);
			asdu.data.clear();
			Str02_101.Format("��%s ������(%XH):%s%s վ(%d)��\r\n%s%s",StrDatahead,control_code,dirstr,fcstr,linkaddr,strASDU,strSumcheck);	
		}			
	}
	else
	{
		Str02_101.Format("������ѡ��Э�����ͻ������ݸ�ʽ����");
	}
	return Str02_101;
}

CString CProtocol02_101::Pmain97_101(unsigned char *pdata, int len)
{
	return "1";
}


CString CProtocol02_101::Pmain104(unsigned char *pdata, int len)
{
	unsigned char ucData[1000];
	memcpy(ucData,pdata,len);
	CString strASDU="",strAPCI,strData,StrASDUtype,StrOBJ,Strtemp,Str02_101,StrCOMAddr;
	int SendNo,RecvNo;

	strSumcheck.Empty();
	
	if (ucData[0]==0x68)
	{
		strAPCI.Format("��ʼ�ַ�(%XH) APDU����(%d)",ucData[0],ucData[1]);
		for (int k=0;k<6;k++)
		{		
			Control_Data104[k] = ucData[k];
			Strtemp = strData;
			strData.Format("%s%02XH ",Strtemp,Control_Data104[k]);
		}
		Strtemp = strAPCI;
		SendNo = (Control_Data104[2]&0xFE)>>1;
		RecvNo = (Control_Data104[4]&0xFE)>>1;
		if (Control_Data104[2]&0x1)
		{
			if ((Control_Data104[2]&2)==2)//U��ʽ֡
			{
				CString UFrame;
				int StartACT=(Control_Data104[2]&4)>>2;
				int StartCON=(Control_Data104[2]&8)>>3;
				int StopACT=(Control_Data104[2]&16)>>4;
				int StopCON=(Control_Data104[2]&32)>>5;
				int TestACT=(Control_Data104[2]&64)>>6;
				int TestCON=(Control_Data104[2]&128)>>7;
				UFrame.Format("STARTDT:ACT=(%d) CON=(%d) STOPDT:ACT(%d) CON(%d) TESTFR: ACT(%d) CON(%d)",StartACT,StartCON,StopACT,StopCON,TestACT,TestCON);
				strAPCI.Format("<%s> %s U��ʽ֡ %s",strData,Strtemp,UFrame);
			}
			else//S��ʽ֡
			{
				RecvNo = (Control_Data104[2]&0xFE)>>1;
				strAPCI.Format("<%s> %s S��ʽ֡ �������RN(%d)",strData,Strtemp,RecvNo);
			}
		} 
		else//I��ʽ֡
		{
			strAPCI.Format("<%s> %s I��ʽ֡ �������SN(%d) �������RN(%d)",strData,Strtemp,SendNo,RecvNo);
		}
		if (ucData[1]>4)//I��ʽ֡
		{
			ASDU asdu;						
			asdu.TYPE_ID= ucData[6];			
			asdu.VSQ.SQ	= ((ucData[7]&0x80)!=0) ? 1 : 0;
			asdu.VSQ.N	= ucData[7]&0x7F;			
			asdu.COT.T	= ((ucData[8]&0x80)!=0) ? 1 : 0;
			asdu.COT.PN	= ((ucData[8]&0x40)!=0) ? 1 : 0;
			asdu.COT.Cot= ucData[8]&0x3F;
			if (COT_lenNum==2)
			{
				asdu.COT.C_ADDR = ucData[9];
			}
			asdu.ASDUAddr[1]=0;
			for (int i=0;i<ASDUA_lenNum;i++)
			{
				asdu.ASDUAddr[i] = ucData[8+COT_lenNum+i];
			}

			StrCOMAddr.Format("������ַ(%d)",(asdu.ASDUAddr[1]<<8)|asdu.ASDUAddr[0]);

			strCOT=ParseASDU_COT(asdu.COT);
			strVSQ=ParseASDU_VSQ(asdu.VSQ);

			strData ="";
			for (k=0;k<2+COT_lenNum+ASDUA_lenNum;k++)
			{		
				Strtemp = strData;
				strData.Format("%s%02XH ",Strtemp,ucData[6+k]);
			}

			asdu.data.clear();
			for (i=8+COT_lenNum+ASDUA_lenNum;i<len;i++)
			{
				asdu.data.push_back(ucData[i]);
			}
			//if(len>11)		asdu.data.insert(asdu.data.end(),data->pData.begin()+10,data->pData.end()-2);//��У���
			switch(asdu.TYPE_ID)
			{
			case _101_M_SP_NA_1:	//type=1 ����ʱ��ĵ�����Ϣ---��ѯ������͵ĵ�����Ϣ
				StrASDUtype = "M_SP_NA_1 ��ʱ�굥��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_SP_NA_1(asdu);
				break;
			case _101_M_SP_TA_1:	//type=2 ��ʱ��ĵ�����Ϣ---��ѯ������͵ĵ�����Ϣ
				StrASDUtype = "M_SP_TA_1 ��ʱ�굥��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TA_1(asdu);
				break;
			case _101_M_DP_NA_1:	//type=3 ����ʱ���˫����Ϣ---��ѯ������͵�˫����Ϣ
				StrASDUtype = "M_DP_NA_1 ��ʱ��˫��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_DP_NA_1(asdu);
				break;
			case _101_M_DP_TA_1:	//type=4 ��ʱ���˫����Ϣ---��ѯ������͵�˫����Ϣ
				StrASDUtype = "M_DP_TA_1 ��ʱ��˫��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TA_1(asdu);
				break;

			case _101_M_ME_NA_1:	//type = 9  --����ֵ--��һ��
				StrASDUtype = "M_ME_NA_1 ��һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NA_1(asdu);
				break;	
			case _101_M_ME_TA_1:	//type = 10 ����ֵ--��ʱ��Ĺ�һ��ֵCP24Time2a
				StrASDUtype = "M_ME_TA_1 ��ʱ���һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TA_1(asdu);
				break;
			case _101_M_ME_NB_1:	//type=11  ����ֵ--��Ȼ�---��ѯ������͵�ң����Ϣ
				StrASDUtype = "M_ME_NB_1 ��Ȼ�ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NB_1(asdu);
				break;
			case _101_M_ME_TB_1:	//type=12  ����ֵ--��ʱ��ı�Ȼ�ֵ---ң��Խ��
				StrASDUtype = "M_ME_TB_1 ��ʱ���Ȼ�ֵ";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TB_1(asdu);
				break;

			case _101_M_IT_NA_1:	//type=15 �ۼ���
				StrASDUtype = "M_IT_NA_1 �������";
				StrOBJ = ParseASDU_IEC101_02_M_IT_NA_1(asdu);
				break;	
				
			case _101_M_ME_ND_1:	//type=21 ����Ʒ�������ʵĹ�һ��ֵ
				StrASDUtype = "M_ME_ND_1 ң������";
				StrOBJ = ParseASDU_IEC101_02_M_ME_ND_1(asdu);
				break;
			case _101_M_SP_TB_1:	//type=30 ��ʱ��ĵ�����Ϣ---ң�ű�λ�ĵ�����ϢCP56Time2a
				StrASDUtype = "M_SP_TB_1 ����ң�ű�λ����";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TB_1(asdu);
				break;
			case _101_M_DP_TB_1:	//type=31 ��ʱ���˫����Ϣ---ң�ű�λ��˫����ϢCP56Time2a
				StrASDUtype = "M_DP_TB_1 ˫��ң�ű�λ����";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TB_1(asdu);
				break;


			case _101_M_ME_TD_1:	//type=34  ����ֵ--��ʱ��Ĺ�һ��ֵ---ң��Խ��CP56Time2a
				StrASDUtype = "M_ME_TD_1 ��ʱ���һ��ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TD_1(asdu);
				break;
			case _101_M_ME_TE_1:	//type=35  ����ֵ--��ʱ��ı�Ȼ�ֵ---ң��Խ��CP56Time2a
				StrASDUtype = "M_ME_TE_1 ��ʱ���Ȼ�ң��";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TE_1(asdu);
				break;
			case _101_M_IT_TB_1:	//type=37  ��ʱ����ۼ���
				StrASDUtype = "M_IT_TB_1 �������";
				StrOBJ = ParseASDU_IEC101_02_M_IT_TB_1(asdu);
				break;

			case _101_C_SC_NA_1:	//type=45  ����ң��
				StrASDUtype = "C_SC_NA_1 ����ң������";
				StrOBJ = ParseASDU_IEC101_02_C_SC_NA_1(asdu);
				break;
			case _101_C_DC_NA_1:	//type=46  ˫��ң������
				StrASDUtype = "C_DC_NA_1 ˫��ң������";
				StrOBJ = ParseASDU_IEC101_02_C_DC_NA_1(asdu);
				break;
			case _101_C_RC_NA_1:	//type=47  ң������
				StrASDUtype = "C_RC_NA_1 ң������";
				StrOBJ = ParseASDU_IEC101_02_C_RC_NA_1(asdu);
				break;
				
			case _101_C_SE_NB_1:    //type=49  �趨����
				StrASDUtype = "C_SE_NB_1 �趨��Ȼ�ֵ";
				StrOBJ = ParseASDU_IEC101_02_C_SE_NB_1(asdu);
				break;

			case _101_M_EI_NA_1:	//type = 70 ��ʼ������
 				StrASDUtype = "M_EI_NA_1 ��ʼ������";
				StrOBJ = ParseASDU_IEC101_02_M_EI_NA_1(asdu);
				break;
			case _101_C_IC_NA_1:    //type = 100 ��������
				StrASDUtype = "C_IC_NA_1 ��������";
				StrOBJ = ParseASDU_IEC101_02_C_IC_NA_1(asdu);
				break;
			case _101_C_CI_NA_1:    //type = 101 ��������
				StrASDUtype = "C_CI_NA_1 �ٵ��";
				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
				break;
// 			case _101_C_RD_NA_1:    //type = 102 ������
// 				StrASDUtype = "C_RD_NA_1 ������";
// 				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
// 				break;
			case _101_C_CS_NA_1:    //type = 103 ʱ��ͬ��
				StrASDUtype = "C_CS_NA_1 ʱ��ͬ��";
				StrOBJ = ParseASDU_IEC101_02_C_CS_NA_1(asdu);
				break;

			case _101_P_ME_NB_1:    //type = 103 ʱ��ͬ��
				StrASDUtype = "P_ME_NB_1 ����������Ȼ�";
				StrOBJ = ParseASDU_IEC101_02_P_ME_NB_1(asdu);
				break;

// 			case _101_C_CI_NA_1:
// 				ProcessFunction_IEC101_02_C_CI_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_DD;
// 				break;
// 			case _101_F_FR_NA_1://�ļ�׼������
// 				ProcessFunction_IEC101_02_F_FR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SR_NA_1://��׼������
// 				ProcessFunction_IEC101_02_F_SR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_LS_NA_1://���ĶΡ���
// 				ProcessFunction_IEC101_02_F_LS_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SG_NA_1://�δ���
// 				ProcessFunction_IEC101_02_F_SG_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_DR_TA_1://Ŀ¼
// 				ProcessFunction_IEC101_02_F_DR_TA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_ASDU_129:	//�Լ챨��
// 				ProcessFunction_IEC101_02_ASDU_129(pStationObj,asdu);
// 				break;
// 			case _101_ASDU_130:	//�¼�����
// 				ProcessFunction_IEC101_02_ASDU_130(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_131:	//���ϱ���
// 				ProcessFunction_IEC101_02_ASDU_131(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_132:	//����¼��
// 				ProcessFunction_IEC101_02_ASDU_132(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_133:
// 				ProcessFunction_IEC101_02_ASDU_133(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_134:
// 				ProcessFunction_IEC101_02_ASDU_134(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_135:
// 				ProcessFunction_IEC101_02_ASDU_135(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_136:
// 				ProcessFunction_IEC101_02_ASDU_136(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_137:
// 				ProcessFunction_IEC101_02_ASDU_137(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_138:
// 				ProcessFunction_IEC101_02_ASDU_138(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_139:
// 				ProcessFunction_IEC101_02_ASDU_139(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_140:
// 				ProcessFunction_IEC101_02_ASDU_140(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_141:
// 				ProcessFunction_IEC101_02_ASDU_141(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_142:
// 				ProcessFunction_IEC101_02_ASDU_142(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_143:
// 				ProcessFunction_IEC101_02_ASDU_143(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_144:
// 				ProcessFunction_IEC101_02_ASDU_144(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_145://������ֵ
// 				ProcessFunction_IEC101_02_ASDU_145(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_146:
// 				ProcessFunction_IEC101_02_ASDU_146(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_147://����ֵ�л�
// 				ProcessFunction_IEC101_02_ASDU_147(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_148:
// 				ProcessFunction_IEC101_02_ASDU_148(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_149:
// 				ProcessFunction_IEC101_02_ASDU_149(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_150:
// 				ProcessFunction_IEC101_02_ASDU_150(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
			
			default:StrASDUtype.Format("(%d)��������δ����",asdu.TYPE_ID);
				break;
			}

			strASDU.Format("��<%s>(%d):%s %s %s%s��%s",strData,asdu.TYPE_ID,StrASDUtype,strVSQ,strCOT,StrCOMAddr,StrOBJ);
			asdu.data.clear();
		}
		Str02_101.Format("��%s��\r\n%s",strAPCI,strASDU);
	}
	else
	{
		Str02_101.Format("������ѡ��Э�����ͻ������ݸ�ʽ����");
	}
	return Str02_101;
}

unsigned char CProtocol02_101::Add_Sum_Check(unsigned char *data, unsigned char data_len)
{
	unsigned char addcrc;
	for(addcrc=0;data_len>0;data_len--)addcrc+=*data++;
	return addcrc;
}

CString CProtocol02_101::ParsePLDU_FCCode(unsigned char dir, unsigned char fccode)
{
	CString fcstr;
	if (DIR==0)
	{
		switch(fun_code)
		{
		case 0:fcstr.Format(" FC%d:��λ��·",fun_code);
			break;
		case 1:fcstr.Format(" FC%d:��λ����",fun_code);
			break;
		case 2:fcstr.Format(" FC%d:��·����",fun_code);
			break;
		case 3:fcstr.Format(" FC%d:��������",fun_code);
			break;
		case 4:fcstr.Format(" FC%d:��������",fun_code);
			break;
		case 8:fcstr.Format(" FC%d:����Ҫ��",fun_code);
			break;
		case 9:fcstr.Format(" FC%d:����·״̬",fun_code);
			break;
		case 10:fcstr.Format(" FC%d:��1������",fun_code);
			break;
		case 11:fcstr.Format(" FC%d:��2������",fun_code);
			break;
		default:fcstr.Format(" FC%d:��������",fun_code);
			break;
		}
	}
	else
	{
		switch(fun_code)
		{
		case 0:fcstr.Format(" FC%d:ȷ��֡",fun_code);
			break;
		case 1:fcstr.Format(" FC%d:��·æ",fun_code);
			break;
		case 8:fcstr.Format(" FC%d:������Ӧ",fun_code);
			break;
		case 9:fcstr.Format(" FC%d:���ٻ�����",fun_code);
			break;
		default:fcstr.Format(" FC%d:��������",fun_code);
			break;
		}
	}
	return fcstr;
}
CString CProtocol02_101::ParseASDU_COT(_COT cot)
{
	CString PNstr;
	CString addrstr,Strtemp;
	switch(cot.PN)
	{
	case 0:PNstr="�϶�";
		break;
	case 1:PNstr="��";
	    break;
	default:PNstr="����";
	    break;
	}
	switch(cot.Cot)
	{
	case 0:Strtemp.Format("����ԭ��(%d):δ�� %s",cot.Cot,PNstr);
		break;
	case 1:Strtemp.Format("����ԭ��(%d):����orѭ�� %s",cot.Cot,PNstr);
		break;
	case 2:Strtemp.Format("����ԭ��(%d):����ɨ�� %s",cot.Cot,PNstr);
		break;
	case 3:Strtemp.Format("����ԭ��(%d):ͻ�� %s",cot.Cot,PNstr);
		break;
	case 4:Strtemp.Format("����ԭ��(%d):��ʼ�� %s",cot.Cot,PNstr);
		break;
	case 5:Strtemp.Format("����ԭ��(%d):����or������ %s",cot.Cot,PNstr);
		break;
	case 6:Strtemp.Format("����ԭ��(%d):���� %s",cot.Cot,PNstr);
		break;
	case 7:Strtemp.Format("����ԭ��(%d):����ȷ�� %s",cot.Cot,PNstr);
		break;
	case 8:Strtemp.Format("����ԭ��(%d):ֹͣ���� %s",cot.Cot,PNstr);
		break;
	case 9:Strtemp.Format("����ԭ��(%d):ֹͣ����ȷ�� %s",cot.Cot,PNstr);
		break;
	case 10:Strtemp.Format("����ԭ��(%d):������ֹ %s",cot.Cot,PNstr);
		break;
	case 11:Strtemp.Format("����ԭ��(%d):Զ������� %s",cot.Cot,PNstr);
		break;
	case 12:Strtemp.Format("����ԭ��(%d):��������� %s",cot.Cot,PNstr);
		break;
	case 13:Strtemp.Format("����ԭ��(%d):�ļ����� %s",cot.Cot,PNstr);
		break;
	case 20:Strtemp.Format("����ԭ��(%d):��Ӧվ�ٻ� %s",cot.Cot,PNstr);
		break;
	case 21:;
	case 22:;
	case 23:;
	case 24:;
	case 25:;
	case 26:;
	case 27:;
	case 28:;
	case 29:;
	case 30:;
	case 31:;
	case 32:;
	case 33:;
	case 34:;
	case 35:;
	case 36:Strtemp.Format("����ԭ��(%d):��Ӧ��%d���ٻ� %s",cot.Cot,cot.Cot-20,PNstr);
		break;
	case 37:Strtemp.Format("����ԭ��(%d):��Ӧ�ۼ���վ�ٻ� %s",cot.Cot,PNstr);
		break;
	case 38:;
	case 39:;
	case 40:;
	case 41:Strtemp.Format("����ԭ��(%d):��Ӧ�ۼ�����%d���ٻ� %s",cot.Cot,cot.Cot-37,PNstr);
		break;
	case 44:Strtemp.Format("����ԭ��(%d):δ֪���ͱ�ʶ %s",cot.Cot,PNstr);
		break;
	case 45:Strtemp.Format("����ԭ��(%d):δ֪����ԭ�� %s",cot.Cot,PNstr);
		break;
	case 46:Strtemp.Format("����ԭ��(%d):δ֪װ�õ�ַ %s",cot.Cot,PNstr);
		break;
	case 47:Strtemp.Format("����ԭ��(%d):δ֪�����ַ %s",cot.Cot,PNstr);
		break;
	default:Strtemp.Format("����ԭ��(%d):���ݱ��� %s",cot.Cot,PNstr);
		break;
	}
	if (COT_lenNum==2)
	{
		addrstr.Format("%s Դ��ַ(%d)",Strtemp,cot.C_ADDR);
	}
	else
	{
		addrstr = Strtemp;
	}
	return addrstr;
}

CString CProtocol02_101::ParseASDU_VSQ(_VSQ vsq)
{
	CString vsqSQstr="",Strtemp;
	if (vsq.SQ)	
	{
		vsqSQstr="��";
	}
	Strtemp.Format("(%d)%s��������",vsq.N,vsqSQstr);
	return Strtemp;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_NA_1(ASDU asdu)////Type = 1 ����ʱ�굥��ң��
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value;
	unsigned char objTemp[10];
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr=0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		//OBJAddr = asdu.data[0]|(asdu.data[1]<<8);
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			CString	strobjdata = "";
			for (int j=0;j<1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s> ������:%5d %s��",StrTemp,strobjdata,OBJAddr+i,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
				objTemp[k] = asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k);
			}
			//OBJAddr = asdu.data[i*3]|(asdu.data[i*3+1]<<8);
			Value = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			objTemp[k] = Value;
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
			}
			QDSstr = ParseASDU_QDS(1,Value);
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s> ������:%4d %s��",StrTemp,strobjdata,OBJAddr,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_TA_1(ASDU asdu)//type = 2 ��CP24Time2a�ĵ�����Ϣ
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		//OBJAddr = asdu.data[0]|(asdu.data[1]<<8);
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*4+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time3.Min = asdu.data[i*4+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*4+InfoOBJ_lenNum+1]|(asdu.data[i*4+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*4+j]);
			}
			StrTemp = StrRe;		
			StrRe.Format("%s \r\n��<%s>������:%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(4+InfoOBJ_lenNum)+k]<<(8*k));
			}
			//OBJAddr = asdu.data[i*3]|(asdu.data[i*3+1]<<8);
			Value = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time3.Min = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(4+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;		
			StrRe.Format("%s \r\n��<%s>������:%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_NA_1(ASDU asdu)////Type = 3 ����ʱ��˫��ң��
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value;
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr=0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);

			CString	strobjdata = "";
			for (int j=0;j<1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s>������:%d %s��",StrTemp,strobjdata,OBJAddr+i,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d %s��",StrTemp,strobjdata,OBJAddr,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_TA_1(ASDU asdu)//type = 4 ��CP24Time2a��˫����Ϣ
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*4+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time3.Min = asdu.data[i*4+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*4+InfoOBJ_lenNum+1]|(asdu.data[i*4+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*4+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(4+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time3.Min = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(4+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;	
			StrRe.Format("%s \r\n��<%s>������:%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_NA_1(ASDU asdu)//Type = 9 ����ֵ����ʱ��Ĺ�һ��ֵ
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*3+InfoOBJ_lenNum]|(asdu.data[i*3+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*3+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*3+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TA_1(ASDU asdu)//Type = 10 ����ֵ����ʱ��Ĺ�һ��ֵ
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*6+InfoOBJ_lenNum]|(asdu.data[i*6+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*6+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);

			Time3.Min = asdu.data[i*6+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*6+InfoOBJ_lenNum+4]|(asdu.data[i*6+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*6+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);			
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(6+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			Time3.Min = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+4]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(6+InfoOBJ_lenNum)+j]);
			}			
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,ycobj,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_NB_1(ASDU asdu)//Type = 11 ����ֵ����Ȼ�ֵ
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*3+InfoOBJ_lenNum]|(asdu.data[i*3+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*3+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*3+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TB_1(ASDU asdu)//Type = 12 ����ֵ����ʱ��ı�Ȼ�ֵ
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*6+InfoOBJ_lenNum]|(asdu.data[i*6+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*6+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			Time3.Min = asdu.data[i*6+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*6+InfoOBJ_lenNum+4]|(asdu.data[i*6+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*6+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);			
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(6+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			Time3.Min = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+4]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;

			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(6+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s CP24Time(%d:%d.%d)��",StrTemp,strobjdata,ycobj,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_IT_NA_1(ASDU asdu)//Type = 15 �ۼ���
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*5+InfoOBJ_lenNum]|(asdu.data[i*5+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*5+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*5+InfoOBJ_lenNum+3]<<24);
			ValueT = asdu.data[i*5+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(ValueT);

			CString	strobjdata = "";
			for (int j=0;j<5;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*5+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(5+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]<<24);
			ValueT = asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(ValueT);
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+5;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(5+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d %s��",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}	
	return StrRe;
}


CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_ND_1(ASDU asdu)//Type = 21 ����ֵ������Ʒ�������ʵĵĹ�һ��ֵ
{
	CString StrTemp,QZstr,StrRe="";
	int ycobj,ycValue;
	if (asdu.VSQ.SQ)//����
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*2+InfoOBJ_lenNum]|(asdu.data[i*2+InfoOBJ_lenNum+1]<<8);
			
			CString	strobjdata = "";
			for (int j=0;j<2;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*2+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d��",StrTemp,strobjdata,ycobj+i,ycValue);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(2+InfoOBJ_lenNum)+k]<<(8*k));
			}
			//ycobj = asdu.data[i*4]|(asdu.data[i*4+1]<<8);			
			ycValue = asdu.data[i*(InfoOBJ_lenNum+2)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+2)+InfoOBJ_lenNum+1]<<8);
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+2;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(2+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ%d��",StrTemp,strobjdata,ycobj,ycValue);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_TB_1(ASDU asdu)//type = 30 ��CP56Time2a�ĵ�����Ϣ��λ����
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*8+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time7.Year = asdu.data[i*8+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*8+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*8+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*8+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*8+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*8+InfoOBJ_lenNum+1]|(asdu.data[i*8+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*8+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%S>������:%d %s CP24Time(%d.%d%d %d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(8+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time7.Year = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(8+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%s>������:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_TB_1(ASDU asdu)//type = 31 ��CP56Time2a��˫����Ϣ��λ����
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//����
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*8+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time7.Year = asdu.data[i*8+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*8+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*8+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*8+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*8+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*8+InfoOBJ_lenNum+1]|(asdu.data[i*8+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*8+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%s>������:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(8+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time7.Year = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(8+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n��<%s>������:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TD_1(ASDU asdu)//type = 34 ��CP56Time2a�Ĺ�һ��ң��
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//����
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(10+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
			QDS = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,QDS);
			Time7.Year = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x7f;
			Time7.Month = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x0f;
			Time7.Day = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x1f;
			Time7.Hour = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x1f;
			Time7.Min = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x3f;
			int msec = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+10;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(10+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ����");
	}
	return StrRe;
}


CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TE_1(ASDU asdu)//type = 35 ��CP56Time2a�ı�Ȼ�ң��
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//����
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(10+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
			QDS = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,QDS);
			Time7.Year = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x7f;
			Time7.Month = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x0f;
			Time7.Day = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x1f;
			Time7.Hour = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x1f;
			Time7.Min = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x3f;
			int msec = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+10;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(10+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ����");
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_IT_TB_1(ASDU asdu)//type = 37 ��CP56Time2a���ۼ�������
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//����
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(12+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]<<24);
			QDS = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(QDS);
			Time7.Year = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+11]&0x7f;
			Time7.Month = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+10]&0x0f;
			Time7.Day = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x1f;
			Time7.Hour = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x1f;
			Time7.Min = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x3f;
			int msec = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+12;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(12+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n��<%s>������:%d ֵ:%d %s CP56Time(%d��%d��%d��%d:%d:%d.%d)��",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ����");
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_SC_NA_1(ASDU asdu)//Type = 45 ��������
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(1,StateData);
		
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n��<%s>������:%d ֵ:%XH(%s)��",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_DC_NA_1(ASDU asdu)//Type = 46 ˫����
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(2,StateData);

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n��<%s>������:%d ֵ:%XH(%s)��",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_RC_NA_1(ASDU asdu)//Type = ���� ˫����
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(3,StateData);
		
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n��<%s>������:%d ֵ:%XH(%s)��",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_SE_NB_1(ASDU asdu)//Type = 49 �趨����
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr,Value;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
		}
		Value = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
		StateData = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
		
		if (StateData&0x80==0x80)
		{
			Statestr="ѡ��";
		} 
		else
		{
			Statestr="ִ��";
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+3;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n��<%s>������:%d ֵ:%d ״̬:%s��",StrTemp,strobjdata,OBJAddr,Value,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_EI_NA_1(ASDU asdu)//Type = 70 ��ʼ������
{
	CString StrTemp,BSstr,CIOstr,StrRe="";
	int OBJAddr;
	unsigned char qoi=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		qoi = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		if (qoi&0x80 == 0x80)	
		{
			BSstr.Format("δ�ı䵱�ز����ĳ�ʼ��");		
		}
		else
		{
			BSstr.Format("�ı䵱�ز�����ĳ�ʼ��");	
		}
		switch(qoi&0x7f)
		{
		case 0:CIOstr = "���ص�Դ����";
			break;
		case 1:CIOstr = "�����ֶ���λ";
			break;
		case 2:CIOstr = "Զ����λ";
		    break;
		default:CIOstr = "ʹ�ñ���";
		    break;
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n��<%s>������:%d %s %s��",StrTemp,strobjdata,OBJAddr,BSstr,CIOstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_IC_NA_1(ASDU asdu)//Type = 100 �ٻ�����
{
	CString StrTemp,QZstr,StrRe="";
	int OBJAddr;
	unsigned char qoi=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		qoi = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		if (qoi == 0)	
		{
			QZstr.Format("δ��");		
		}
		else if (qoi == 20)
		{
			QZstr.Format("վ�ٻ�");	
		}
		else if((qoi>20)&&(qoi<37))
		{
			QZstr.Format("��%d���ٻ�",qoi-20);	
		}
		else
		{
			QZstr.Format("���ݱ���");	
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;	
		StrRe.Format("%s \r\n��<%s>������:%d %s��",StrTemp,strobjdata,OBJAddr,QZstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_CI_NA_1(ASDU asdu)//Type = 101 ���ۼ���
{
	CString StrTemp,Statestr,StrRe="",RQTstr,FRZstr;
	int OBJAddr,iRAT,iFRZ;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		iRAT = StateData&0x3f;
		switch(iRAT)
		{
		case 0:RQTstr = "û���������";
			break;
		case 1:
		case 2:
		case 3:
		case 4:RQTstr.Format("�����������%��",iRAT);
			break;
		case 5:RQTstr = "�ܵ����������";
			break;
		default:RQTstr = "Э��Ԥ��";
		    break;
		}
		iFRZ = (StateData>>6)&0x3;
		switch(iFRZ)
		{
		case 0:FRZstr = "�޶���";
			break;
		case 1:FRZstr = "����ֵΪ�ۼ���";
			break;
		case 2:FRZstr = "����ֵΪ����";
		    break;
		case 3:FRZstr = "��������λ";
		    break;
		default:
		    break;
		}
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n��<%s>������:%d �޶���:%d(%s)��",StrTemp,strobjdata,OBJAddr,StateData,RQTstr,FRZstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_CS_NA_1(ASDU asdu)//Type = 103 ʱ��ͬ������
{
	CString Statestr,StrRe="",StrTemp;
	int OBJAddr=0;
	unsigned char StateData=0;
	
	for (int k=0;k<InfoOBJ_lenNum;k++)
	{
		OBJAddr += (asdu.data[k]<<(8*k));
	}
	Time7.Year = asdu.data[InfoOBJ_lenNum+6]&0x7f;
	Time7.Month = asdu.data[InfoOBJ_lenNum+5]&0x0f;
	Time7.Day = asdu.data[InfoOBJ_lenNum+4]&0x1f;
	Time7.Hour = asdu.data[InfoOBJ_lenNum+3]&0x1f;
	Time7.Min = asdu.data[InfoOBJ_lenNum+2]&0x3f;
	int msec = asdu.data[InfoOBJ_lenNum]|(asdu.data[InfoOBJ_lenNum+1]<<8);
	Time7.Sec = msec/1000;
	Time7.msec = msec%1000;
	CString	strobjdata = "";
	for (int j=0;j<InfoOBJ_lenNum+6;j++)
	{
		StrTemp = strobjdata;
		strobjdata.Format("%s%02XH ",StrTemp,asdu.data[(6+InfoOBJ_lenNum)+j]);
	}
	StrRe.Format("\r\n��(%s)������:%d CP56Time(%d��%d��%d��%d:%d:%d.%d)��",strobjdata,OBJAddr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_P_ME_NB_1(ASDU asdu)//Type = 111 ����������Ȼ�
{
	CString StrTemp,Statestr,StrRe="",KPAstr,LPCstr,POPstr;
	int OBJAddr,Value;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
		}
		Value = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
		StateData = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
		switch(StateData&0x3f)
		{
		case 0:KPAstr = "δ��";
			break;
		case 1:KPAstr = "����ֵ";
			break;
		case 2:KPAstr = "ƽ��ϵ��";
		    break;
		case 3:KPAstr = "����ֵ����";
		    break;
		case 4:KPAstr = "����ֵ��";
		    break;
		default:
		    break;
		}
		if (StateData&0x40==0x40)
		{
			LPCstr="�ı�";
		} 
		else
		{
			LPCstr="δ�ı�";
		}
		if (StateData&0x80==0x80)
		{
			POPstr="δ����";
		} 
		else
		{
			POPstr="����";
		}
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+3;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n��<%s>������:%d ֵ:%d ״̬(%d KPA:%s LPC:%s POP:%s)��",StrTemp,strobjdata,OBJAddr,Value,StateData,KPAstr,LPCstr,POPstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_BCR(unsigned char bcr)
{
	CString StrRe,CYstr,CAstr,SBstr,IVstr;
	unsigned char sq = bcr&0x1f;
	if (bcr&0x20==0x20)
	{
		CYstr="����Ӧ���ۼ������м��������";
	} 
	else
	{
		CYstr="����Ӧ���ۼ������м�����δ���";
	}
	if (bcr&0x40==0x40)
	{
		CAstr = "�������ѵ���";
	} 
	else
	{
		CAstr = "������δ����";
	}
	if (bcr&0x80==0x80)
	{
		IVstr = "��Ч";
	} 
	else
	{
		IVstr = "��Ч";
	}
	StrRe.Format("˳���:%d CY:%s CA:%s IV:%s",sq,CYstr,CAstr,IVstr);
	return StrRe;
}
CString CProtocol02_101::ParseASDU_QDS(int type,unsigned char diq)
{
	CString StrRe,SPIstr,BLstr,SBstr,NTstr,IVstr;
	//type=1SIQ type=2DIQ type=3QDS type=4QDP
	switch(type)
	{
	case 1:
		{
			if (diq&0x01 == 0)
			{
				SPIstr="��";
			} 
			else
			{
				SPIstr="��";
			}
		}
		break;
	case 2:
		{
			switch(diq&0x3)
			{
			case 0:SPIstr ="�м�̬";
				break;
			case 1:SPIstr ="��";
				break;
			case 2:SPIstr ="��";
				break;
			case 3:SPIstr ="��ȷ��";
				break;
			default:SPIstr ="����";
				break;
			}
		}
		break;
	case 3:
		{
			if (diq&0x01 == 0)
			{
				SPIstr="δ���";
			} 
			else
			{
				SPIstr="���";
			}
		}
	    break;
	case 4:
	    break;
	default:
	    break;
	}
	
	if (diq&0x10 == 0)
	{
		BLstr="δ������";
	} 
	else
	{
		BLstr="������";
	}
	if (diq&0x20 == 0)
	{
		SBstr="δ��ȡ��";
	} 
	else
	{
		SBstr="��ȡ��";
	}
	if (diq&0x40 == 0)
	{
		NTstr="��ǰֵ";
	} 
	else
	{
		NTstr="�ǵ�ǰֵ";
	}
	if (diq&0x80 == 0)
	{
		IVstr="��Ч";
	} 
	else
	{
		IVstr="��Ч";
	}
	StrRe.Format("״̬:%s  BL:%s SB:%s NT:%s IV:%s",SPIstr,BLstr,SBstr,NTstr,IVstr);
	return StrRe;
}

CString CProtocol02_101::ParseASDU_DCO(int type,unsigned char StateData)
{
	CString StrRe,CSstr,QUstr,SEstr;
	unsigned char QOC;
	//type=1SIQ type=2DIQ type=3QDS type=4QDP
	switch(type)
	{
	case 1://˫����
		{
			switch(StateData&0x1)
			{
			case 0:CSstr ="��";
				break;
			case 1:CSstr ="��";
				break;
			default:CSstr ="����";
				break;
			}
		}
		break;
	case 2://˫����
		{
			switch(StateData&0x3)
			{
			case 0:CSstr ="������";
				break;
			case 1:CSstr ="��";
				break;
			case 2:CSstr ="��";
				break;
			case 3:CSstr ="������";
				break;
			default:CSstr ="����";
				break;
			}
		}
		break;
	case 3://��������
		{
			switch(StateData&0x3)
			{
			case 0:CSstr ="������";
				break;
			case 1:CSstr ="��һ��";
				break;
			case 2:CSstr ="��һ��";
				break;
			case 3:CSstr ="������";
				break;
			default:CSstr ="����";
				break;
			}
		}
		break;
	default: CSstr="����";
		break;
	}
	QOC = StateData&0xfc;
	if (QOC&0x80==0x80)
	{
		SEstr = "ѡ��";
	} 
	else
	{
		SEstr = "ִ��";
	}
	switch((QOC&0x7c)>>2)
	{
	case 0:QUstr = "������Ķ���";
		break;
	case 1:QUstr = "������";
		break;
	case 2:QUstr = "������";
	    break;
	case 3:QUstr = "����";
	    break;
	default:QUstr = "Э�����";
	    break;
	}
	StrRe.Format("����:%s QU:%s SE:%s",CSstr,QUstr,SEstr);
	return StrRe;
}