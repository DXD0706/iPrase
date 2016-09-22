/////////////////////////////////////////////////////////////////////
// �ļ���                  :    Protocol.h
//-------------------------------------------------------------------
// Ŀ�ļ���Ҫ����          :    ����Э��㲻ͬЭ���༰�����ʹ�����
//                              ����02��101�����࣬��â�����Լ������
//                              07��101��Լ�����࣬104��Լ������
//-------------------------------------------------------------------
// ��������                :    2009.9.3
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>

/////////////////////////ң��///////////////////////
const int _101_M_SP_NA_1 = 0x01;	//=1	����ң��
const int _101_M_SP_TA_1 = 0x02;	//=2	��CP24Time�ĵ���ң��
const int _101_M_DP_NA_1 = 0x03;	//=3	˫��ң��
const int _101_M_DP_TA_1 = 0x04;	//=4	��CP24Time��˫��ң��
const int _101_M_SP_TB_1 = 0x1E;	//=30	��CP56Time�ĵ���ң��
const int _101_M_DP_TB_1 = 0x1F;	//=32	��CP56Time��˫��ң��

/////////////////////////ң��///////////////////////
const int _101_M_ME_NA_1 = 0x09;	//=9	��һ��ң��ֵ
const int _101_M_ME_TA_1 = 0x0A;	//=10	��CP24Time��һ��ң��ֵ
const int _101_M_ME_NB_1 = 0x0B;	//=11	��Ȼ�ң��ֵ
const int _101_M_ME_TB_1 = 0x0C;	//=12	��CP24Time��Ȼ�ң��ֵ
const int _101_M_ME_ND_1 = 0x15;	//=21	����Ʒ�������ʵĹ�һ��ң��ֵ
const int _101_M_ME_TD_1 = 0x22;	//=34	��CP56Time�Ĺ�һ��ң��ֵ
const int _101_M_ME_TE_1 = 0x23;	//=35	��CP56Time�ı�Ȼ�ң��ֵ

/////////////////////////ϵͳ����///////////////////////////////
const int _101_M_EI_NA_1 = 0x46;	//=70	��ʼ������
const int _101_C_IC_NA_1 = 0x64;	//=100	ȫ��
const int _101_C_CI_NA_1 = 0x65;	//=101	�ٵ��
const int _101_C_CS_NA_1 = 0x67;	//=103	��ʱ

/////////////////////////ң��////////////////////////////////
const int _101_C_SC_NA_1 = 0x2D;	//=45	ң�ص������·�
const int _101_C_DC_NA_1 = 0x2E;	//=46	ң��˫�����·�
const int _101_C_RC_NA_1 = 0x2F;    //=47	ң�������·�

/////////////////////////���////////////////////////////////
const int _101_M_IT_NA_1 = 0x0F;	//=15	�ۼ���
const int _101_M_IT_TB_1 = 0x25;	//=37	��CP56Time���ۼ���

/////////////////////////�趨����////////////////////////////////
//const int _101_C_SE_NA_1 = 0x30;	//=48	�趨��һ��ֵ����
const int _101_C_SE_NB_1 = 0x31;	//=49	�趨��Ȼ�ֵ����
//const int _101_C_SE_NC_1 = 0x32;	//=50	�趨�̸���ֵ����

//const int _101_P_ME_NA_1 = 0x6E;	//=110	�趨��һ��ֵ����
const int _101_P_ME_NB_1 = 0x6F;	//=111	�趨��Ȼ�ֵ����
//const int _101_P_ME_NC_1 = 0x70;	//=112	�趨�̸���ֵ����
//const int _101_P_AC_NA_1 = 0x71;	//=113	��������
/////////////////////////�ļ�����////////////////////////////////
const int _101_F_FR_NA_1 = 0x78;	//=
const int _101_F_SR_NA_1 = 0x79;	//=
const int _101_F_SC_NA_1 = 0x7A;	//=
const int _101_F_LS_NA_1 = 0x7B;	//=
const int _101_F_AF_NA_1 = 0x7C;	//=
const int _101_F_SG_NA_1 = 0x7D;	//=
const int _101_F_DR_TA_1 = 0x7E;	//=
////////////////�Զ��岿��/////////////////////////////////////////////
const int _101_ASDU_129	= 0x81;		//=
const int _101_ASDU_130	= 0x82;		//=
const int _101_ASDU_131	= 0x83;		//=
const int _101_ASDU_132	= 0x84;		//=
const int _101_ASDU_133	= 0x85;		//=
const int _101_ASDU_134	= 0x86;		//=
const int _101_ASDU_135	= 0x87;		//=
const int _101_ASDU_136	= 0x88;		//=
const int _101_ASDU_137	= 0x89;		//=
const int _101_ASDU_138	= 0x8A;		//=
const int _101_ASDU_139	= 0x8B;		//=
const int _101_ASDU_140	= 0x8C;		//=
const int _101_ASDU_141	= 0x8D;		//=
const int _101_ASDU_142	= 0x8E;		//=
const int _101_ASDU_143	= 0x9F;		//=
const int _101_ASDU_144	= 0x90;		//=
const int _101_ASDU_145	= 0x91;		//=
const int _101_ASDU_146	= 0x92;		//=
const int _101_ASDU_147	= 0x93;		//=
const int _101_ASDU_148	= 0x94;		//=
const int _101_ASDU_149	= 0x95;		//=
const int _101_ASDU_150	= 0x96;		//=
/////////////////////////////////////////////////////////////

struct _VSQ
{
	unsigned char	SQ:1;
	unsigned char	N:7;
};
struct _COT
{
	unsigned char	T:1;
	unsigned char	PN:1;
	unsigned char	Cot:6;
	unsigned char	OriginAddr;
	unsigned short	C_ADDR;		//Դ��ַ
};

struct CP56Time2a//_TIME7
{
	unsigned char	Year;
	unsigned char	Month;
	unsigned char	Day;
	unsigned char	Hour;
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};
struct CP32Time2a//_TIME4
{
	unsigned char	Hour;
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};

struct CP24Time2a//_TIME4
{
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};

class ASDU 
{
public:
	ASDU() {data.clear();}
	~ASDU(){data.clear();}
	unsigned char	RES1;			//ACD
	unsigned char	RES2;			//Addr
	unsigned char	RES3;
	unsigned char	TYPE_ID;		//asdu��ʶ
	struct _VSQ		VSQ;			//�ɱ�ṹ�޶���
	struct _COT		COT;			//����ԭ��
	unsigned char	ASDUAddr[2];	//Ӧ�÷������ݹ�����ַ
	std::vector<unsigned char> data;
};
class CProtocol02_101
{
public:
	unsigned char linkaddr;
	unsigned char sumcheck;
	unsigned char control_code;
	unsigned char fun_code;
	unsigned char DIR;
	CP56Time2a Time7;
	CP32Time2a Time4;
	CP24Time2a Time3;
	CString strLPDU;
	CString strVSQ;
	CString strCOT;
	CString strSumcheck;

public:
	unsigned char	Control_Data104[8];

private:
	int COT_lenNum;//����ԭ�򳤶Ȳ��� ȡֵ��Χ��1��2��
	int ASDUA_lenNum;//Ӧ�÷������ݵ�Ԫ������ַ���Ȳ���ȡֵ��Χ��1��2��
	int InfoOBJ_lenNum;//��Ϣ�����ַ���Ȳ���ȡֵ��Χ��1��2��3��

public:
	CProtocol02_101(int COT_Num,int ASDUA_Num,int InfoOBJ_Num);
	CProtocol02_101();
	virtual ~CProtocol02_101();
	
	CString ParsePLDU_FCCode(unsigned char dir, unsigned char fccode);
	CString ParseASDU_COT(_COT cot);//����ԭ��
	CString ParseASDU_VSQ(_VSQ vsq);//�ɱ�ṹ�޶���
	
	CString Pmain02_101(unsigned char *pdata, int len);	//����02��101
	CString Pmain97_101(unsigned char *pdata, int len);//����97��101
	CString Pmain104(unsigned char *pdata, int len);//����104


	CString ParseASDU_IEC101_02_M_SP_NA_1(ASDU asdu);//Type = 1   ��ʱ�굥��ң��
	CString ParseASDU_IEC101_02_M_SP_TA_1(ASDU asdu);//Type = 2   ��CP24Time����ң��
	CString ParseASDU_IEC101_02_M_DP_NA_1(ASDU asdu);//Type = 3   ��ʱ��˫��ң��
	CString ParseASDU_IEC101_02_M_DP_TA_1(ASDU asdu);//Type = 4   ��CP24Time˫��ң��

	CString ParseASDU_IEC101_02_M_ME_NA_1(ASDU asdu);//Type = 9   ����ֵ����ʱ��Ĺ�һ��ֵ
	CString ParseASDU_IEC101_02_M_ME_TA_1(ASDU asdu);//Type = 10  ����ֵ����ʱ��Ĺ�һ��ֵ
	CString ParseASDU_IEC101_02_M_ME_NB_1(ASDU asdu);//Type = 11  ����ֵ����ʱ��ı�Ȼ�ֵ
	CString ParseASDU_IEC101_02_M_ME_TB_1(ASDU asdu);//Type = 12  ����ֵ����ʱ��ı�Ȼ�ֵ

	CString ParseASDU_IEC101_02_M_IT_NA_1(ASDU asdu);//Type = 15  �ۼ���

	CString ParseASDU_IEC101_02_M_ME_ND_1(ASDU asdu);//Type = 21  ����ֵ������Ʒ�������ʵĵĹ�һ��ֵ

	CString ParseASDU_IEC101_02_M_SP_TB_1(ASDU asdu);//Type = 30  ��CP56Time����ң�ű�λ����
	CString ParseASDU_IEC101_02_M_DP_TB_1(ASDU asdu);//Type = 31  ��CP56Time˫��ң�ű�λ����

	CString ParseASDU_IEC101_02_M_ME_TD_1(ASDU asdu);//Type = 34  ��CP56Time��һ��ֵ����
	CString ParseASDU_IEC101_02_M_ME_TE_1(ASDU asdu);//Type = 35  ��CP56Time��Ȼ�ֵ����

	CString ParseASDU_IEC101_02_M_IT_TB_1(ASDU asdu);//Type = 37  ��CP56Time�ۼ�������

	CString ParseASDU_IEC101_02_C_SC_NA_1(ASDU asdu);//Type = 45  ������
	CString ParseASDU_IEC101_02_C_DC_NA_1(ASDU asdu);//Type = 46  ˫����
	CString ParseASDU_IEC101_02_C_RC_NA_1(ASDU asdu);//Type = 47  ��������

	CString ParseASDU_IEC101_02_C_SE_NB_1(ASDU asdu);//Type = 49  �趨��Ȼ�����

	CString ParseASDU_IEC101_02_M_EI_NA_1(ASDU asdu);//Type = 70  ��ʼ������

	CString ParseASDU_IEC101_02_C_IC_NA_1(ASDU asdu);//Type = 100 վ���ٻ�����
	CString ParseASDU_IEC101_02_C_CI_NA_1(ASDU asdu);//Type = 101 ���ۼ���

	CString ParseASDU_IEC101_02_C_CS_NA_1(ASDU asdu);//Type = 103 ��ʱ����

	CString ParseASDU_IEC101_02_P_ME_NB_1(ASDU asdu);//Type = 111 ����������Ȼ�ֵ

	CString ParseASDU_QDS(int type,unsigned char qds);//��Ʒ�������ʵĵ�����Ϣtype=1SIQ type=2DIQ type=3QDS type=4QDP 
	CString ParseASDU_DCO(int type,unsigned char StateData);//˫���� type=1DCO type=2RCO
	CString ParseASDU_BCR(unsigned char bcr);//�ۼ���
	unsigned char Add_Sum_Check(unsigned char *data, unsigned char data_len);

};
class CProtocolGMWLGY
{
public:

	CProtocolGMWLGY();
	virtual ~CProtocolGMWLGY();

	CString PmainGMWLGY(unsigned char *pdata, int len);
	
public:
	CString FunStr;
	CString Strtemp;
};