

// FoxFunc.cpp : Defines the entry point for the DLL application.
//
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include <iostream.h>

#include "stdafx.h"
#include "FoxFunc.h"

float y[15000];
float yy[15000];

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                                         )
{
    switch (ul_reason_for_call)
        {
                case DLL_PROCESS_ATTACH:
                case DLL_THREAD_ATTACH:
                case DLL_THREAD_DETACH:
                case DLL_PROCESS_DETACH:
                        break;
    }
    return TRUE;
}



// FoxFunc.cpp : Defines the entry point for the DLL application.
//
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include <iostream.h>

#include "stdafx.h"
#include "FoxFunc.h"

float y[15000];
float yy[15000];

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                                         )
{
    switch (ul_reason_for_call)
        {
                case DLL_PROCESS_ATTACH:
                case DLL_THREAD_ATTACH:
                case DLL_THREAD_DETACH:
                case DLL_PROCESS_DETACH:
                        break;
    }
    return TRUE;
}

//�������̼۵ľ���,һ����������,��ʾ��������
//���÷���:
//        "FOXFUNC@MYMACLOSE"(5)


float smoothb(int ,int );
float realft(int, int );
int fourit(int , int );

//        !!!!!!!! use "C" for Microsoft visual C++ compiler
//        !!!!!!!! use "C" for Microsoft visual C++ compiler
//        !!!!!!!! use "C" for Microsoft visual C++ compiler


__declspec(dllexport) int WINAPI SMOOTH(CALCINFO* pData)
{
        if(pData->m_pfParam1 && pData->m_pfParam2 &&         //����1,2��Ч
                pData->m_nParam1Start>=0 &&                                        //����1Ϊ������
                pData->m_pfParam3==NULL)                                        //��2������
        {
                const float*  pValue = pData->m_pfParam1;        //����1
                int nFirst = pData->m_nParam1Start;                        //��Чֵ��ʼλ
                float fParam = *pData->m_pfParam2;                        //����2
                int nPeriod = (int)fParam;                        
        
        int nndata =pData->m_nNumData;
        float aa=(float)0.0;


                if( nFirst >= 0 && nPeriod > 0 )
                {   
                //        float fTotal;
                        int i;

   
                        for ( i =nFirst; i < pData->m_nNumData; i++ )
                        {
                                y[i+1] = pValue[i];    //ƽ��
                        }
      
            for ( i = 0; i <15000; i++ )
                        {
                                yy[i+1]=(float)0.0;
                        }


              aa=smoothb(nndata,nPeriod);


            for ( i =nFirst; i < pData->m_nNumData; i++ )
                        {
                                pData->m_pResultBuf[i]=y[i+1];    //ƽ��
                //   pData->m_pResultBuf[i]=aa;    //ƽ��
            //    pData->m_pResultBuf[i]=nndata;  
                        }

                        return 0;
                }
        }
        return -1;
}

