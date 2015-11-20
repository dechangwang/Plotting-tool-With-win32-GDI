#include <windows.h>  
#include <math.h>  
#include <commdlg.h>
#include<list>
#include<map>
#include "AnalysisExpression.h"
#include"dataStruct.h"
#include "resource.h"
#include<string>

typedef std::list<POINT> LISTINT;
typedef std::list<DataStruct> LIST_DATA;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
map<float,int> barData;
void drawLine(HDC hDC, int x1, int y1, int x2, int y2, COLORREF color);
void recalculate(POINT test[],double xSet[],double ySet[],POINT,int cxClient,int cyClient);
void recalculate(POINT test[],double xSet[],double ySet[],POINT,int,double);
bool preDealRpex(TCHAR[],TCHAR[],TCHAR tempEx[100]);
BOOL SaveBitmap(HBITMAP hBitmap, LPCTSTR lpFileName);
BOOL OpenCSV(LPCTSTR lpFileName);
BOOL OpenCSVForBar(LPCTSTR lpFileName);

LIST_DATA dataFromCSV;
HINSTANCE hInst;
BOOL tag = false;
double space = 1;			
int realSpace = 20;
int cxClient, cyClient; 		
bool IsMove = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)  
{  
    static TCHAR    szAppName[] = TEXT("PLOTING");  
    HWND            hwnd;  
    MSG             msg;  
    WNDCLASS        wndclass;
	int width,height;
	
  
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;  
    wndclass.lpfnWndProc    = WndProc;  
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;  
    wndclass.hInstance      = hInstance;  
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);  
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);  
    wndclass.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);  
    wndclass.lpszClassName  = szAppName;  
	hInst = hInstance;
    if (!RegisterClass(&wndclass))  
    {  
        MessageBox(NULL, TEXT("Windows Plotting Assignment1"), szAppName, MB_ICONERROR);  
        return 0;  
    }  
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
    hwnd = CreateWindow(szAppName, TEXT("Windows Plotting"),  
                        WS_OVERLAPPEDWINDOW,  
                        CW_USEDEFAULT, CW_USEDEFAULT,   
                       CW_USEDEFAULT, CW_USEDEFAULT,   
                        NULL, LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)), hInstance, NULL);  
	
  
    ShowWindow(hwnd, iCmdShow);  
    UpdateWindow(hwnd);  
  
    while (GetMessage(&msg, NULL, 0, 0))  
    {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }  
  
    return msg.wParam;  
}  
   
 
HDC             hdc,hMem;  
HPEN            hpen;
HWND hButton1,changeColor1,changeColor2,changeColor3,func1,func2,func3,hbgColor;
AnalysisExpression expr;
HBITMAP hbmp;
HBRUSH hbrush;
int xBefore = 0;
int yBefore = 0;
TCHAR  repxpression[100],repxpression2[100],repxpression3[100];
bool hasPlotted = false,hasPlotted2 = false,hasPlotted3 = false;
DWORD rgb_1 = RGB(255,0,0),rgb_2 = RGB(0,255,0),rgb_3 = RGB(0,0,255),bColor = RGB(255,255,250);


void recalculate(POINT test[numOfPoints],double xSet[numOfPoints],double ySet[numOfPoints],POINT points,int cxClient,int cyClient)
{
	for(int i = 0;i<numOfPoints;i++)
	{		
		test[i].x = xSet[i]*cxClient/40+points.x;
		test[i].y = 2*points.y -(ySet[i]*cyClient/20+points.y);
	}
}
void recalculate(POINT test[numOfPoints],double xSet[numOfPoints],double ySet[numOfPoints],POINT points,int realSpace,double space)
{
	for(int i = 0;i<numOfPoints;i++)
	{		
		test[i].x = xSet[i]*realSpace/space+points.x;
		test[i].y = 2*points.y -(ySet[i]*realSpace/space+points.y);
	}
}
void drawLine(HDC hDC, int x1, int y1, int x2, int y2, COLORREF color)
{

 int dx = (x2 - x1);
 int dy = (y2 - y1);
 COLORREF bgColor;
 int temp;
 float k;


 // X-dominant line
 if (abs(dx) > abs(dy))
 {
  // Ex-change line end points
  if (dx < 0)
  {
   temp = x1;
   x1 = x2;
   x2 = temp;

   temp = y1;
   y1 = y2;
   y2 = temp;
  }
  k = (float)dy / (float)dx;

  int xs;
  float yt = (float)y1;
  float distance;
  UCHAR red, green, blue;


  for (xs=x1; xs<=x2; xs++)
  {
   distance = (float)(yt - (int)(yt));

   bgColor = ::GetPixel(hDC, xs, (int)yt-1);
   red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(color));
   green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(color));
   blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(color));
   ::SetPixel(hDC, xs, (int)yt-1, RGB(red,green,blue));

   //::SetPixel(hDC, xs, (int)yt-1, color);
   ::SetPixel(hDC, xs, (int)yt, color);


   bgColor = ::GetPixel(hDC, xs, (int)yt+1);
   red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(color));
   green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(color));
   blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(color));
   ::SetPixel(hDC, xs, (int)yt+1, RGB(red,green,blue));

   yt += k;
  }

 }
 // Y-dominant line
 else
 {
  // Ex-change line end points
  if (dy < 0)
  {
   temp = x1;
   x1 = x2;
   x2 = temp;

   temp = y1;
   y1 = y2;
   y2 = temp;
  }
  k = (float)dx / (float)dy;

  int ys;
  float xt = (float)x1;
  float distance;
  UCHAR red, green, blue;
  for (ys=y1; ys<=y2; ys++)
  {
   distance = (float)(xt - (int)(xt));

   bgColor = ::GetPixel(hDC, (int)xt-1, ys);
   red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(color));
   green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(color));
   blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(color));
   ::SetPixel(hDC, (int)xt-1, ys, RGB(red,green,blue));

   //::SetPixel(hDC, (int)xt-1, ys, color);
   ::SetPixel(hDC, (int)xt, ys, color);


   bgColor = ::GetPixel(hDC, (int)xt+1, ys);
   red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(color));
   green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(color));
   blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(color));
   ::SetPixel(hDC, (int)xt+1, ys, RGB(red,green,blue));

   xt += k;
  }
 }
}
bool preDealRpex(TCHAR buf[255],TCHAR repxpression[100],TCHAR tempEx[100])
{
	int k = 0,ex =0;
					
	TCHAR expre[200];
	expre[ex++] = '$';
	if(buf[0] != '\0' && buf[0] == '-')
		expre[ex++] = '0';
	while(buf[k] != '\0')
	{
		if(buf[k] ==' ')
			k++;
		expre[ex++] = buf[k++];
	}
	expre[ex++] = '#';
	ex = 0;
	TCHAR *rpex = expr.RPExpression(expre);
	if(*rpex =='E')
		return false;
	while(*rpex != '#')
	{
		repxpression[ex] = *rpex;
		tempEx[ex] = *rpex;
		ex++;
		rpex ++;
	}
	tempEx[ex] = '#';
	repxpression[ex++] = '#';
	return true;
}
BOOL SaveBitmap(HBITMAP hBitmap, LPCTSTR lpFileName )  
{
	HDC hDC; 
	int iBits;			//ÿ��������ռ�ֽ���  
	WORD wBitCount;		//λͼ��ÿ��������ռ�ֽ���  
	DWORD dwPaletteSize=0, //��ɫ���С 
	dwBmBitsSize,          //λͼ�������ֽڴ�С
	dwDIBSize, dwWritten;  //λͼ�ļ���С �� д���ļ��ֽ���  
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr;	//λͼ�ļ�ͷ�ṹ  
	BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ  
	LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ  

	HANDLE fh, hDib, hPal,hOldPal=NULL; //�����ļ��������ڴ�������ɫ����  

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	HDC hWndDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);  
	hDC = ::CreateCompatibleDC( hWndDC ) ;  
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);  

	if (iBits <= 1)  
	wBitCount = 1;  
	else if (iBits <= 4)  
	wBitCount = 4;  
	else if (iBits <= 8)  
	wBitCount = 8;  
	else if (iBits <= 24)  
	wBitCount = 24;  
	else  
	wBitCount = 24 ;  

	//�����ɫ���С  
	if (wBitCount <= 8)  
	dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);  

	//����λͼ��Ϣͷ�ṹ  
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;  
	bi.biPlanes = 1;  
	bi.biBitCount = wBitCount;  //1(�ڰ׶�ɫͼ), 4(16ɫͼ), 8(256ɫ), 24(���ɫͼ)
	bi.biCompression = BI_RGB;  //��ѹ��
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;  

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ;  

	//Ϊλͼ���ݷ����ڴ�  
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  

	// �����ɫ��  
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
	hDC = ::GetDC(NULL);  
	hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);  
	RealizePalette(hDC);  
	}  

	// ��ȡ�õ�ɫ�����µ�����ֵ  
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,  
	(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)  
	+dwPaletteSize,  
	(LPBITMAPINFO )  
	lpbi, DIB_RGB_COLORS);  

	//�ָ���ɫ��  
	if (hOldPal)  
	{  
	SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
	RealizePalette(hDC);  
	::ReleaseDC(NULL, hDC);  
	}  

	//����λͼ�ļ� 

	fh = CreateFile(lpFileName, GENERIC_WRITE,  
	0, NULL, CREATE_ALWAYS,  
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);  

	if (fh == INVALID_HANDLE_VALUE)  
	return FALSE;  

	// ����λͼ�ļ�ͷ  
	bmfHdr.bfType = 0x4D42; // "BM"  
	dwDIBSize = sizeof(BITMAPFILEHEADER)  
	+ sizeof(BITMAPINFOHEADER)  
	+ dwPaletteSize + dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)  
	+ (DWORD)sizeof(BITMAPINFOHEADER)  
	+ dwPaletteSize;  

	// д��λͼ�ļ�ͷ  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  

	// д��λͼ�ļ���������  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,  
	&dwWritten, NULL);  

	//���  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  
	return TRUE;  
}
BOOL OpenCSV(LPCTSTR lpFileName)
{
	HANDLE hFile;//����һ�������
	hFile=CreateFile(lpFileName,
	   GENERIC_READ,
	   FILE_SHARE_READ,
	   NULL,
	   OPEN_EXISTING,
	   FILE_ATTRIBUTE_NORMAL,
	   NULL);//ʹ��CreatFile���API�������ļ�
	DWORD dwDataLen;
	char FileBuffer[1000];
	ReadFile(hFile,FileBuffer,100,&dwDataLen,NULL);//��ȡ����
	FileBuffer[dwDataLen]=0;//������δβ���㡣
	CloseHandle(hFile);//�رվ��
	
	int i = 0;
	//string content(FileBuffer);
	std::string temp="";
	float x,y;
	bool isY = false;
	DataStruct data;
	while(FileBuffer[i] !='\0')
	{
		if(FileBuffer[i]=='\r' || FileBuffer[i]=='\n')
		{	
			y=atof(temp.data());
			i++;
			temp = "";
			data.x = x;
			data.y = y;
			dataFromCSV.push_back(data);
		}
		else if(FileBuffer[i]!=','){
			temp +=FileBuffer[i];
			
		}else{
			
				x = atof(temp.data());
			temp = "";
			
		}
		isY = true;
		i++;
	}
	//MessageBox(FileContent);//������
}
BOOL OpenCSVForBar(LPCTSTR lpFileName)
{
		HANDLE hFile;//����һ�������
	hFile=CreateFile(lpFileName,
	   GENERIC_READ,
	   FILE_SHARE_READ,
	   NULL,
	   OPEN_EXISTING,
	   FILE_ATTRIBUTE_NORMAL,
	   NULL);//���ļ�
	DWORD dwDataLen;
	char FileBuffer[1000];
	ReadFile(hFile,FileBuffer,100,&dwDataLen,NULL);//��ȡ����
	FileBuffer[dwDataLen]=0;//������δβ���㡣
	CloseHandle(hFile);//�رվ��
	
	int i = 0;
	//string content(FileBuffer);
	std::string temp="";
	float x,y;
	
	DataStruct data;
	while(FileBuffer[i] !='\0')
	{
		if(FileBuffer[i]=='\r' || FileBuffer[i]=='\n')
		{	
			y=atof(temp.data());
			barData[y]++;
			i++;
			temp = "";
			
		}else if(FileBuffer[i]==',')
		{
			y=atof(temp.data());
			barData[y]++;
			
			temp = "";
		}else{ 
			
		temp +=FileBuffer[i];
			
		}
		i++;
	}
}