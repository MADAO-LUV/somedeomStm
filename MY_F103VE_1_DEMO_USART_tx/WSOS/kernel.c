#include "kernel.h"

WspTsak   pOnhead,pRunhead,pHanghead;
Task   WsTsak[TaskLen];

// ������ָ��
int ( *TsakFun)(void *);


void InitTask(void)
{  char i;
   pOnhead=0,pRunhead=0;pHanghead = 0;
	 for(i=0;i<TaskLen;i++)
	  {
	   WsTsak[i].ID = 0       ;     //  ʹ�ñ�־����
		 WsTsak[i].next = 0     ;
		 WsTsak[i].parameter = 0;
		}
}

char StartTask(char id,int time)  //  ������������
{  
	  WspTsak pt,pt1;
    pt1 = pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                // �ر����ж�			
				if(pt == pHanghead )        // ������ӹ����б���ɾ��
				    pHanghead = pt->next;
				else
					  pt1->next = pt->next; 
				
				 pt->Time = time   ;		
				 if(time == 0)     //  ����ִ������,��ӵ�ִ����   
            AddRunTask(pt) ; 
				 else              //  ��ӵ���ʱ�б�  
            AddOnTask(pt)  ;					 
         
         EN_INTERRUPT		   //  �����ж�
         return WS_Status_OK;       //  ���سɹ�
				
      }
			pt1 = pt;
			pt  = pt->next;
    }
		return WS_Status_ERROR; //  ʧ�ܣ�δ�ҵ���Ӧ����    	
}

#if WS_MSG_EN > 0

char setTaskMsg(char taskid,Message msg)
{
    WspTsak pt;
    pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == taskid)
			{  
         pt->msg  = msg;
         return 1;         //  ���سɹ�			
      }
			pt = pt->next;
    }
		return 0; //  ʧ�ܣ�λ�ҵ���Ӧ����    	
}

#endif

char StopTask(char id)         //  ���������
{
   WspTsak pt,pt1;
	 DI_INTERRUPT                // �ر����ж�	
	 if(pOnhead != 0)            // ��ʱ�б�
	 {  
      pt1 =  pt = pOnhead;
		  while(pt != 0)
			{   
				  if(pt->ID == id)
					{  DI_INTERRUPT                // �ر����ж�		
						 if(pt == pOnhead)          
						  pOnhead = pt->next    ; 
             else 
              pt1->next = pt->next  ; 
           
             AddHangTask(pt)        ; 
             EN_INTERRUPT		              //  �����ж�					 
             return WS_Status_OK;                    //  �������ɹ�������1
          }
					pt1 = pt;
          pt = pt->next;
      }
   }
	 
	 if(pRunhead != 0)  //  �����б�
	 {  
      pt1 =  pt = pRunhead;
		  while(pt != 0)
			{   
				  if(pt->ID == id)
					{  DI_INTERRUPT                // �ر����ж�		
						 if(pt == pRunhead)          
						  pRunhead = pt->next    ; 
             else 
              pt1->next = pt->next  ; 
           
             AddHangTask(pt)        ; 
             EN_INTERRUPT		              //  �����ж�					 
             return WS_Status_OK;                    //  �������ɹ�������1
          }
					pt1 = pt;
          pt = pt->next;
      }
   }
	 
	 
	 EN_INTERRUPT		              //  �����ж�	
	 return WS_Status_ERROR;                  
}

char DeleteTask(char id) //  ɾ������
{
	 WspTsak pt,pt1;
   StopTask(id);
	  DI_INTERRUPT                // �ر����ж�	
	
	 pt1 = pt = pHanghead;
	 while(pt != 0)
		{	
      if(pt->ID == id)
			{  
       			
				if(pt == pHanghead )        // ������ӹ����б���ɾ��
				    pHanghead = pt->next;
				 else
					  pt1->next = pt->next; 
				
				 pt->ID = 0   ;		         //  ɾ��ID
         EN_INTERRUPT		              //  �����ж�		 
         return WS_Status_OK;                   //  ���سɹ�
				
      }
			pt1 = pt;
			pt = pt->next;
    }
	
	  EN_INTERRUPT		              //  �����ж�	
	 return WS_Status_ERROR;  //  �Ҳ�������
}

//  ������ʱ��
char CreateTimer(int (*fun)(void *),int time)
{
	 char i; 
   
   if(time <= 0)   return 0; 	     //  ��Чʱ�䣬���ش���
	
	 for(i=0;i<TaskLen;i++)          //  Ѱ�ҵ�ǰ�����Ƿ��Ѿ�������ʱ��
	 {
		  if(WsTsak[i].fun == fun)  
			{
				WsTsak[i].Time = time;
				return WS_Status_OK;       //  �Ѿ�����������ʱ��
			}
	 } 
	 for(i=0;i<TaskLen;i++)          //  Ѱ�ҿ�����ջ
	 {  
      if(WsTsak[i].ID == 0)
			{  		 
				 WsTsak[i].ID   = -1;      //  ��ʱ������ͳһ�� -1 ID
         WsTsak[i].fun  = (int (*)(void * ))fun;
				 WsTsak[i].Time = time;
				 WsTsak[i].next = 0;

				 if(time == 0)     //  ����ִ������,��ӵ�ִ����   
            AddRunTask(&WsTsak[i]) ; 
				 else              //  ��ӵ���ʱ�б�  
            AddOnTask(&WsTsak[i])  ;
				 
				 //AddOnTask(&WsTsak[i]);	   //  ��ӵ���ʱ�б� 
				 
				 return WS_Status_OK;  
      }
   } 
   return WS_Status_ERROR;
}

//  ��������
char CreateTask(char id,int (*fun)(void *),int time)
{
	 char i;
	
	 for(i=0;i<TaskLen;i++)              //  �鿴ID�Ƿ�ΪΨһ
	  {
       if(WsTsak[i].ID == id)  
				  return WS_Status_BUSY;      //   ����0ʧ��
    }
	 for(i=0;i<TaskLen;i++)              //  Ѱ�ҿ�����ջ
	 {  
      if(WsTsak[i].ID == 0)
			{  
				 WsTsak[i].ID   = id;
         WsTsak[i].fun  = (int (*)(void *))fun;
				 WsTsak[i].Time = time;
				 WsTsak[i].next = 0;
				 
					 if(time > 0)     // �����ʱ�б�
					 {
						 AddOnTask(&WsTsak[i])  ;
					 }
					 else             // ���� ��������
					 {
             AddHangTask(&WsTsak[i]);
           }
			  
			   return WS_Status_OK; 
      }
   }
	 
  return WS_Status_ERROR;   
}

void AddHangTask(WspTsak ptk)  //  ������񵽹����б�
{  WspTsak pt; 
	 ptk->next = 0                ;
	 do
	 {
		 if(pHanghead == 0)
		 {
			 pHanghead = ptk;
       break;			 
		 }
		 
		 pt = pHanghead;           //  ���ұ�β
		 while(pt->next !=0)    
		 {
			 pt = pt->next;
		 }
		 pt->next = ptk;
	 }while(0);
}

void AddOnTask(WspTsak ptk)  //  ������񵽶�ʱ��
{   
	  WspTsak  pt;
	  ptk->next  = 0;
	  do
		{	
			if(pOnhead == 0)
			 {
				 pOnhead = ptk;
				 break;                //  �Ƴ�
			 } 
			 pt = pOnhead;           //  ���ұ�β
			 while(pt->next !=0)    
			 {
				 pt = pt->next;
			 }
			 pt->next = ptk;
	 }while(0);
}
void OnTask(void)
{   
     WspTsak list,list1;      
	   list1 = list = pOnhead;     //  ����ָ��ָ��ǰ���������񣬱���ɾ������������
     while(list != 0)
		 {  
        if(list->Time > 0) list->Time --;  //  ��ʱ������һ
			  if(list->Time == 0)                //  ����ʱʱ�䵽
				{  
           if(list == pOnhead)             //  ��һ������ʱ�䵽  
						  pOnhead = list->next      ;  //  �ı��ͷָ��
           else 
              list1->next = list->next  ;  //  ɾ����ǰ��������
           
           AddRunTask(list)             ;	 //  ��������ӵ���ִ���б�				 
        }		
				list1 = list;       //  ָ����һ������
			  list = list->next;  //  ָ����һ������
     }	 
}

void AddRunTask(WspTsak ptk)
{
   WspTsak  list;
	 ptk->next  =  0;        //   ĩ�������־
	 list = pRunhead;
	 if(list == 0)           //  �ȴ�ִ���б�Ϊ��
	 {
     pRunhead = ptk;  
   }
	 else
	 {
     while(list->next != 0)
		 {
       list = list->next;
     } 
		 list->next  = ptk  ;
   }
}
void RunTask(void)
{    int time;
	   WspTsak  pt;
	   while(pRunhead != 0)         //  �����б�Ϊ��
		 { 
        TsakFun = pRunhead->fun;  //  ִ������
			  time = (*TsakFun)(pRunhead->parameter)    ; 
			  pRunhead->parameter = NULL;                  //  ���������� 
			  if(pRunhead->ID > 0)        //  ����ջ
				{
					pt   = pRunhead->next;
					if(time >0)               //    �������񷵻�ֵ����������ȥ��
					{
						pRunhead->Time = time;  
						AddOnTask(pRunhead)  ;  //  ������񵽶�ʱ�� 
					}
					else                      //   ��������ӵ������б�
					{
						AddHangTask(pRunhead);
					}
          pRunhead = pt	;		//  ִ��һ�����񣬴�������ɾ��һ������	
				}
				else                         //  ��ʱ������ֱ��ȥ��
				{
					pRunhead->ID = 0;          //  ɾ������
					pRunhead->fun = 0;
          pRunhead = pRunhead->next;
        }
     }    
}


//  д�������β�
char WS_OS_Write_Task_Parameter(short int id , void *parameter, int delayTime )
{
     
    WspTsak pt,pt1;
	
	  if(parameter == NULL || id <= 0)  return 0;
	
	  //   ��ѯ�����б�
    pt1 = pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                // �ر����ж�			
				if(pt == pHanghead )        // ������ӹ����б���ɾ��
				    pHanghead = pt->next;
				else
					  pt1->next = pt->next; 
				
				 pt->Time = delayTime      ;	
				 pt->parameter = parameter ;
         				
				 if(delayTime == 0)     //  ����ִ������,��ӵ�ִ����   
            AddRunTask(pt) ; 
				 else              //  ��ӵ���ʱ�б�  
            AddOnTask(pt)  ;					 
         
         EN_INTERRUPT		   //  �����ж�
         return WS_Status_OK;         //  ���سɹ�
				
      }
			pt1 = pt;
			pt  = pt->next;
    }
		
		//  ��ѯ��ʱ�б�
	  pt =	pOnhead ;	
		while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                 // �ر����ж�			
			  pt->parameter = parameter ;
        EN_INTERRUPT		             //  �����ж�
        return WS_Status_OK;         //  ���سɹ�		
      }
			pt  = pt->next;
    }
		return WS_Status_ERROR           ; //  ʧ�ܣ�δ�ҵ���Ӧ����    
}

