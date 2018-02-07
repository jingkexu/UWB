#include "device_info.h"  
#include "stm32f10x.h"	
#include "stdio.h"
#include "stmflash.h"	
	
u8 buff_version[19] = "nVer. 2.10 TREK\r\n";
u8 buff_sn[41];
u8 buff_wifipassword[10]="AT+PW=OK\r\n";


System_Para_TypeDef sys_para;
//const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
//#define SIZE sizeof(TEXT_Buffer)	 	//���鳤��
#define PAGE61_ADDR  (0x08000000 + 41 * STM_SECTOR_SIZE) 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define PAGE62_ADDR  (0x08000000 + 42 * STM_SECTOR_SIZE) 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define PAGE63_ADDR  (0x08000000 + 43 * STM_SECTOR_SIZE) 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

u8 Check_cmd(u8* RXbuff)
{
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='N') )
			return SN;
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='W') )
			return SW;
	if( (RXbuff[0]=='d') &&(RXbuff[1]=='e') )
			return DECA;
	return 0;
}

u8 Get_UniqueID(void)
{
	u32 CpuID[3];
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);
	sprintf((unsigned char*)&buff_sn[0],"AT+SN=%08x-%08x-%08x-1-1.0\r\n",CpuID[0],CpuID[1],CpuID[2]);
	return 1;
}

/*
A0   0X08
A1   0X48
A2   0X28
A3   0X68

T0   0X00
T1   0X40
T2   0X20
T3   0X60
T4   0X10
T5   0X50
T6   0X30
T7   0X70
*/
//====================================================================//
// �﷨��ʽ��void Flash_Configuration(void)
// ʵ�ֹ��ܣ�Flash������
// ��    ������
// �� �� ֵ����
// ��    ע����
//====================================================================//

u16 FLAH_BUFF0[16];

void Flash_Configuration(void)
{
	u16 temp[50];
	u8 count=0;
	STMFLASH_Read(PAGE61_ADDR, FLAH_BUFF0, 16);	

	if(FLAH_BUFF0[0] != 0xAAAA)//����ǵ�һ��дFlash
	{
		FLAH_BUFF0[0] = 0XAAAA;
		FLAH_BUFF0[1]++;
		sys_para.start_count = FLAH_BUFF0[1];
		FLAH_BUFF0[2] = 0XAA00;
		sys_para.device_switch = FLAH_BUFF0[2];
		
		FLAH_BUFF0[3] = 'V';
		FLAH_BUFF0[4] = '1';
		FLAH_BUFF0[5] = '.';
		FLAH_BUFF0[6] = '0';
		
		FLAH_BUFF0[7] = 'V';
		FLAH_BUFF0[8] = '1';
		FLAH_BUFF0[9] = '.';
		FLAH_BUFF0[10]= '0';
//		Flash_WriteByte(FLAH_WRITE_BUFF ,16, PAGE63_ADDR);//PAGE63���״�ʹ�ù̶�д��
		STMFLASH_Write(PAGE61_ADDR, FLAH_BUFF0, 16);//PAGE61������ƽʱ�洢��¼
	}
	else//������ǵ�һ��дFlash
	{
		FLAH_BUFF0[1]++;
		sys_para.start_count     =  FLAH_BUFF0[1 ];
		sys_para.device_switch   = 	FLAH_BUFF0[2 ];
		sys_para.hw_version[0]   = 	FLAH_BUFF0[3 ];
		sys_para.hw_version[1]   = 	FLAH_BUFF0[4 ];
		sys_para.hw_version[2]   = 	FLAH_BUFF0[5 ];
		sys_para.hw_version[3]   = 	FLAH_BUFF0[6 ];

		sys_para.sw_version[0]   = 	FLAH_BUFF0[7 ];
		sys_para.sw_version[1]   = 	FLAH_BUFF0[8 ];
		sys_para.sw_version[2]   = 	FLAH_BUFF0[9 ];
		sys_para.sw_version[3]   = 	FLAH_BUFF0[10];
		
		STMFLASH_Write(PAGE61_ADDR, FLAH_BUFF0, 16);	//��ⲻ�ǵ�һ�����ã���PAGE61_ADDR��ȡ���ݲ�װ��
		
//		STMFLASH_Read(PAGE62_ADDR, temp, 20);

	}
}

u8 RouterInfo_Write_to_Flash(u8 OBJ, u8* buffer)
{
  u8 i=0;
	return i;
}

void My_STMFLASH_Write(void)
{
	STMFLASH_Write(PAGE61_ADDR, FLAH_BUFF0, 16);//PAGE61������ƽʱ�洢��¼
}
