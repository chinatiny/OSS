#pragma once
#include <windows.h>
/**
�������Ƕ�λͼ��ʹ��
cswuyg@gmail.com
2014.4.14
*/
/**
DIB�豸�޹�λͼ 
DDB�豸���λͼ [compatible bitmap]

������ͼ�������š�һ�飬��λͼ��ʽ����ϸ���ܡ�
*/

/**
DDBλͼתΪDIBλͼ������ֵ��ʵ��һ��new�������ڴ�����ʹ�ú���Ҫdelete
���̣�ͨ��GetObject���DDBλͼ��Ϣ+��ɫ����Ϣ��䵽BITMAPINFO��Ȼ����GetDIBits���λͼ����
*/
HANDLE DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE Pal );

/**
DIBλͼתDDBλͼ
���̣�������ɫ�壬����CreateDIBitmap����DIB����һ��DDB
*/
HBITMAP DIBToDDB( HANDLE hDIB );


/**
DDBתDIB��DIB��תDDB�Ĳ���
*/
HBITMAP TransBitmap(HBITMAP hbm);

/*
����λͼ���ļ�
���̣�
1��ͨ��λͼ����õ�λͼ��С�����λͼ�ļ�ͷ��λͼ��Ϣͷ��λͼ��ɫ��ռ䣨����еĻ�����
2��ͨ��GetDIBits��DDBλͼ���ݶ�ȡ���ڴ�
3����λͼ�ļ�ͷ��λͼ��Ϣͷ+λͼ��ɫ��ռ�+λͼ����д�뵽�ļ�
param hDDBmap DDBλͼ���
param lpFileName ������ļ���
*/
int SaveBitmapToFile(HBITMAP hDDBmap , LPCTSTR lpFileName);

/**
����λͼ���ļ��ĵڶ���ʵ�֡�
λͼ�ļ���ʵ���ǣ�λͼ�ļ�ͷ +��λͼ��DIB��ʽ��������BITMAPFILEHEADER����ɫ��ռ䡢λͼʵ�����ݣ���
���Կ���������DDBToDIB���õ�DIB��ʽ��Ȼ����ƴ��һ��λͼ�ļ�ͷ�����Ű�����д���ļ���
*/
void SaveBitmapToFile_2( HBITMAP hBitmap, LPCTSTR lpFileName );

/**
������4��������ʹ�ý��в���
*/
void TestBitMap(HWND hWnd);



