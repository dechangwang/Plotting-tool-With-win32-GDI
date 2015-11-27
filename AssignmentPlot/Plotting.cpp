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
HWND hButton1,changeColor1,changeColor2,changeColor3,func1,func2,func3,tickDis,hbgColor,changeTickDis;
AnalysisExpression expr;
HBITMAP hbmp;
HBRUSH hbrush;
int xBefore = 0;
int yBefore = 0;
TCHAR  repxpression[100],repxpression2[100],repxpression3[100];
bool hasPlotted = false,hasPlotted2 = false,hasPlotted3 = false;
DWORD rgb_1 = RGB(255,0,0),rgb_2 = RGB(0,255,0),rgb_3 = RGB(0,0,255),bColor = RGB(255,255,250);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
 
	static POINT    points[5];
	static double    xSet[numOfPoints],ySet[numOfPoints];
	static POINT    test[numOfPoints],test2[numOfPoints],test3[numOfPoints];
	static int locationX = 850;
	
	LIST_DATA::iterator it; 
	PAINTSTRUCT     ps;  
  
    switch (message)  
    {  
	
    case WM_SIZE: 
		{
			cxClient = LOWORD(lParam);  
			cyClient = HIWORD(lParam);    
	
			locationX = cxClient*6/8;

			points[0].x = 3*cxClient / 8;
			points[0].y = cyClient / 2;

			points[1].x = 6*cxClient / 8;
			points[1].y = cyClient / 2; 

			points[2].x = 0;
			points[2].y = cyClient / 2; 

			points[3].x =3*cxClient / 8;
			points[3].y = cyClient; 

			points[4].x = 3*cxClient / 8;
			points[4].y = 0;

			int pos = 0;
			
				if(hasPlotted)
				{
					TCHAR tempEx[100];
					while(repxpression[pos]!='#')
					{
						tempEx[pos] = repxpression[pos++];
					}
					tempEx[pos] = '#';
					expr.getPointsByExpresson(tempEx,test,realSpace/space,realSpace/space,points[0],0);
				}
				pos = 0;
				if(hasPlotted2)
				{
					TCHAR tempEx[100];
					while(repxpression2[pos]!='#')
					{
						tempEx[pos] = repxpression2[pos++];
					}
					tempEx[pos] = '#';
					expr.getPointsByExpresson(tempEx,test2,realSpace/space,realSpace/space,points[0],0);
				}
				pos = 0;
				if(hasPlotted3)
				{
					TCHAR tempEx[100];
					while(repxpression3[pos]!='#')
					{
						tempEx[pos] = repxpression3[pos++];
					}
					tempEx[pos] = '#';
					expr.getPointsByExpresson(tempEx,test3,realSpace/space,realSpace/space,points[0],0);
				}
			
			::InvalidateRect(hwnd,NULL,TRUE);
			UpdateWindow(hwnd); 
		}
        return 0; 
	case WM_CREATE:
		{
			//HWND hWnd2  = CreateWindow(L"Hello",L"2",WS_CHILD | WS_OVERLAPPEDWINDOW,10,10,200,200,hwnd,NULL,hInst,NULL);
			//::ShowWindow(hWnd2,SW_SHOW);
		
			hButton1 = CreateWindow(L"BUTTON", L"Plot", WS_CHILD|WS_VISIBLE, locationX-50, 300, 200, 30, hwnd, (HMENU)PLOTING, hInst, 0);
			hbgColor = CreateWindow(L"BUTTON", L"bColor", WS_CHILD|WS_VISIBLE, locationX-50, 350, 200, 30, hwnd, (HMENU)CHANGE_BGCOLOR, hInst, 0);
			changeColor1 = CreateWindow(L"BUTTON", L"color", WS_CHILD|WS_VISIBLE, 757, 100, 50, 30, hwnd, (HMENU)CHANG_COLOR1, hInst, 0);
			changeColor2 = CreateWindow(L"BUTTON", L"color", WS_CHILD|WS_VISIBLE, 757, 150, 50, 30, hwnd, (HMENU)CHANG_COLOR2, hInst, 0);
			changeColor3 = CreateWindow(L"BUTTON", L"color", WS_CHILD|WS_VISIBLE, 757, 200, 50, 30, hwnd, (HMENU)CHANG_COLOR3, hInst, 0);
			changeTickDis = CreateWindow(L"BUTTON",L"cTick",WS_CHILD|WS_VISIBLE,757,250,42,30,hwnd,(HMENU)CHANGE_TICK_DISTANCE,hInst,0);
			func1 = CreateWindow(L"EDIT", L"",  WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL, 800, 100, 300, 30, hwnd, (HMENU)11, hInst, 0);
			func2 = CreateWindow(L"EDIT", L"",  WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL, 800, 150, 300, 30, hwnd, (HMENU)12, hInst, 0);
			func3 = CreateWindow(L"EDIT", L"",  WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL, 800, 200, 300, 30, hwnd, (HMENU)13, hInst, 0);
		    tickDis = CreateWindow(L"EDIT", L"",  WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL, 800, 250, 300, 30, hwnd, (HMENU)14, hInst, 0);

		}break;
   case WM_LBUTTONUP:
   case WM_RBUTTONUP:
		{
			IsMove = false;
		}
		break;
    
	case WM_MOUSEWHEEL:
		{
			int scrollD = (short)HIWORD(wParam)/100;
			realSpace = realSpace + scrollD;
			//recalculate(test,xSet,ySet,points[0],realSpace,space);
			int pos = 0;
			
			if(hasPlotted)
			{
				TCHAR tempEx[100];
				while(repxpression[pos]!='#')
				{
					tempEx[pos] = repxpression[pos++];
				}
				tempEx[pos] = '#';
				expr.getPointsByExpresson(tempEx,test,realSpace/space,realSpace/space,points[0],0);
			}
			pos = 0;
			if(hasPlotted2)
			{
				TCHAR tempEx[100];
				while(repxpression2[pos]!='#')
				{
					tempEx[pos] = repxpression2[pos++];
				}
				tempEx[pos] = '#';
				expr.getPointsByExpresson(tempEx,test2,realSpace/space,realSpace/space,points[0],0);
			}
			pos = 0;
			if(hasPlotted3)
			{
				TCHAR tempEx[100];
				while(repxpression3[pos]!='#')
				{
					tempEx[pos] = repxpression3[pos++];
				}
				tempEx[pos] = '#';
				expr.getPointsByExpresson(tempEx,test3,realSpace/space,realSpace/space,points[0],0);
			}
			InvalidateRect(hwnd,NULL,false);
			UpdateWindow(hwnd);
		}
		break;
   
    case WM_MOUSEMOVE:  
		{
			if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)  
			{ 
				if(IsMove == false)
				{
					IsMove = true;
					xBefore = (int)LOWORD(lParam);
					yBefore = (int)HIWORD(lParam);
				}
				else
				{
					int xPosition = (int)LOWORD(lParam);
					int yPosition = (int)HIWORD(lParam);
					if(xPosition > points[2].x && xPosition < points[1].x && yPosition > points[4].y && yPosition < points[3].y)
					{
						int xMove = xPosition - xBefore;
						int yMove = yPosition - yBefore;
						points[0].x = points[0].x + xMove;
						points[0].y = points[0].y + yMove;
						//cxClient = LOWORD(lParam);  
						//cyClient = HIWORD(lParam);  
						
						xBefore = xPosition;
						yBefore = yPosition;
						int moveX = 0;
						if(xMove<0)
							moveX = 1;
						else if(xMove > 0)
							moveX =2;
						else
							moveX =  0;

						//double perPartMove = space / realSpace;
						if(hasPlotted)
						{
						
							int pos = 0;
							TCHAR tempEx[100];
							while(repxpression[pos]!='#')
							{
								tempEx[pos]=repxpression[pos++]; 
							}
							tempEx[pos] = '#';
							expr.getPointsByExpresson(tempEx,test,realSpace/space,realSpace/space,points[0],5*xMove);
						}
						//recalculate(test,xSet,ySet,points[0],realSpace,space);
						if(hasPlotted2)
						{
							int pos = 0;
							TCHAR tempEx[100];
							while(repxpression2[pos]!='#')
							{
								tempEx[pos]=repxpression2[pos++]; 
							}
							tempEx[pos] = '#';
							expr.getPointsByExpresson(tempEx,test2,realSpace/space,realSpace/space,points[0],5*xMove);
						}

						if(hasPlotted3)
						{
							int pos = 0;
							TCHAR tempEx[100];
							while(repxpression3[pos]!='#')
							{
								tempEx[pos]=repxpression3[pos++]; 
							}
							tempEx[pos] = '#';
							expr.getPointsByExpresson(tempEx,test3,realSpace/space,realSpace/space,points[0],5*xMove);
						}
						
					}
					
					InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd);
				}
		   }
        } 
		break;
    case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)
			{
			case PLOTING:
				{
					TCHAR buf[255];
					SendMessage(func1, WM_GETTEXT, 99, (LPARAM)buf);
				//	expr.getPointsByExpresson(buf,test,realSpace/space,realSpace/space,points[0],xSet,ySet);
					
					int k = 0,ex =0;
					if(buf[0]!='\0')
					{
						
						TCHAR tempEx[100];
						if(preDealRpex(buf,repxpression,tempEx))
						{
							expr.getPointsByExpresson(tempEx,test,realSpace/space,realSpace/space,points[0],0);
							hasPlotted = true;
						}
						
					}
					
					k = 0,ex =0;
					SendMessage(func2, WM_GETTEXT, 99, (LPARAM)buf);
					if(buf[0]!='\0')
					{
						
						TCHAR tempEx[100];
						if(preDealRpex(buf,repxpression2,tempEx))
						{
							expr.getPointsByExpresson(tempEx,test2,realSpace/space,realSpace/space,points[0],0);
							hasPlotted2 = true;
						}
						
					}

					k = 0,ex =0;
					SendMessage(func3, WM_GETTEXT, 99, (LPARAM)buf);
					if(buf[0]!='\0')
					{
						
						TCHAR tempEx[100];
						if(preDealRpex(buf,repxpression3,tempEx))
						{
							expr.getPointsByExpresson(tempEx,test3,realSpace/space,realSpace/space,points[0],0);
							hasPlotted3 = true;
						}
						
					}
                    InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd);  

				}
				break;
			case CHANG_COLOR1:
				{
					CHOOSECOLOR cc; // common dialog box structure
					static COLORREF acrCustClr[16]; // array of custom colors
					
					HBRUSH hbrush; // brush handle
					
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = hwnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;
					cc.rgbResult = rgb_1;
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					if (ChooseColor(&cc)==TRUE) {
						//hbrush = CreateSolidBrush(cc.rgbResult);
						rgb_1 = cc.rgbResult;
						
					}
					InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd); 
				}
				break;
			case CHANG_COLOR2:
				{            
					CHOOSECOLOR cc; // common dialog box structure
					static COLORREF acrCustClr[16]; // array of custom colors
				
					HBRUSH hbrush; // brush handle
					
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = hwnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;
					cc.rgbResult = rgb_2;
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					if (ChooseColor(&cc)==TRUE) {
						hbrush = CreateSolidBrush(cc.rgbResult);
						rgb_2 = cc.rgbResult;
						
					}
					InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd); 
				}
				break;
			case CHANG_COLOR3:
				{            
					CHOOSECOLOR cc; // common dialog box structure
					static COLORREF acrCustClr[16]; // array of custom colors
				
					HBRUSH hbrush; // brush handle
					
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = hwnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;
					cc.rgbResult = rgb_3;
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					if (ChooseColor(&cc)==TRUE) {
						hbrush = CreateSolidBrush(cc.rgbResult);
						rgb_3 = cc.rgbResult;
						
					}
					InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd); 
				}
				break;
			case CHANGE_TICK_DISTANCE:
				{
					TCHAR buf[255];
					SendMessage(tickDis, WM_GETTEXT, 99, (LPARAM)buf);
					if(buf[0]!='\0')
					{
						string temp;
						int i = 0;
						while(buf[i]!='\0')
						{
							temp+=buf[i];
							i++;
						}
						space = atof(temp.data());
						//space = (double)(buf[0]-48);
						InvalidateRect(hwnd,NULL,true);
						UpdateWindow(hwnd);
					}
				}
				break;
			case CHANGE_BGCOLOR:
				{
					CHOOSECOLOR cc; // common dialog box structure
					static COLORREF acrCustClr[16]; // array of custom colors
				
					HBRUSH hbrush; // brush handle
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = hwnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;
					cc.rgbResult = bColor;
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					if (ChooseColor(&cc)==TRUE) {
						hbrush = CreateSolidBrush(cc.rgbResult);
						bColor = cc.rgbResult;		
					}
					InvalidateRect(hwnd,NULL,TRUE);
					UpdateWindow(hwnd); 
				}
				break;
			case ID_FILE_OPEN40003:
				{
					TCHAR szFileName[MAX_PATH] = L"";
					OPENFILENAME file = { 0 };
					file.lStructSize = sizeof(file);

					file.lpstrFile = szFileName;
					file.nMaxFile = MAX_PATH;
					file.lpstrFilter = L"(*.csv)\0*.csv\0 Text Files(*.txt)\0*.txt\0 All Files\0*.*\0\0";
					file.nFilterIndex = 1;//默认选择第一个
					// 弹出打开文件的对话框
					if(::GetOpenFileName(&file))
					{
						OpenCSV(file.lpstrFile);
						InvalidateRect(hwnd,NULL,TRUE);
						UpdateWindow(hwnd); 

					}
					
				}
				break;
			case ID_FILE_SAVEAS:
				{

					TCHAR buf[255];
					::wsprintf(buf, L"%s", " ");
					OPENFILENAME ofn;

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.hInstance = hInst;
					ofn.lpstrInitialDir = L"F:\\";
					ofn.lpstrFile = buf;
					ofn.lpstrDefExt=L"bmp";
					ofn.lpstrFileTitle=L"save";
					ofn.lpstrFilter=L"(*.bmp)\0*.bmp\0 (*.png)\0*.png\0 (*.pdf)\0*.pdf\0 Text Files (*.txt)\0*.txt\0  All Files (*.*)\0*.*\0";
					ofn.nMaxFile = 255;

					GetSaveFileName(&ofn);

					HDC hDC = ::GetDC(hwnd); 

					RECT rect; 
					::GetClientRect(hwnd, &rect); 
					HDC hDCMem = ::CreateCompatibleDC(hDC); 

					HBITMAP hBitMap = ::CreateCompatibleBitmap(hDC, rect.right-2*cxClient/8, rect.bottom); 
					HBITMAP hOldMap = (HBITMAP)::SelectObject(hDCMem, hBitMap); 

					::BitBlt(hDCMem, 0, 0, rect.right-2*cxClient/8, rect.bottom, hDC, 0, 0, SRCCOPY); 
					
					SaveBitmap(hBitMap,ofn.lpstrFile);
					
					::SelectObject(hDCMem, hOldMap); 
					::DeleteObject(hBitMap); 
					::DeleteDC(hDCMem); 
					::DeleteDC(hDC);


				}
				break;
			case ID_FILE_BARCHART:
				{
					TCHAR szFileName[MAX_PATH] = L"";
					OPENFILENAME file = { 0 };
					file.lStructSize = sizeof(file);

					file.lpstrFile = szFileName;
					file.nMaxFile = MAX_PATH;
					file.lpstrFilter = L"(*.csv)\0*.csv\0 Text Files(*.txt)\0*.txt\0 All Files\0*.*\0\0";
					file.nFilterIndex = 1;//默认选择第一个
					// 弹出打开文件的对话框
					if(::GetOpenFileName(&file))
					{
						OpenCSVForBar(file.lpstrFile);
					}
				}
				::InvalidateRect(hwnd, NULL, TRUE);
				break;
			default:
				break;
			}
		}
	
    case WM_PAINT:  
		{
       // InvalidateRect(hwnd, NULL, TRUE);  
        hdc = BeginPaint(hwnd, &ps);
		
		HBRUSH hbr = CreateSolidBrush(bColor);
		HBRUSH hbrOld = (HBRUSH)SelectObject(hdc,hbr);
		Rectangle(hdc, points[2].x, points[4].y, points[1].x, points[3].y);
		::DeleteObject(hbr);
		::DeleteObject(hbrOld);
		
		int tickAmountX = (points[3].y - points[4].y)  / realSpace;
		int tickAmountY = (points[1].x - points[2].x)  / realSpace;

		while(tickAmountX >= 25 || tickAmountY >= 25)				//当缩小时 将坐标轴的单位缩小
		{
			space = space * 2;
			realSpace = realSpace * 2;
			tickAmountX = (points[3].y - points[4].y)  / realSpace;
			tickAmountY = (points[1].x - points[2].x)  / realSpace;
		}
		while(tickAmountX < 8 || tickAmountY < 8)					//当放大使 将坐标轴的单位扩大
		{
			space = space/2;
			realSpace = realSpace / 2;
			tickAmountX = (points[3].y - points[4].y)  / realSpace;
			tickAmountY = (points[1].x - points[2].x)  / realSpace;
		}
		int moveRealX = 0;
		int moveRealY = 0;
		int movePartX = 0;
		int movePartY = 0;
		if(points[0].x < points[2].x)
		{
			moveRealX = (points[2].x - points[0].x) / realSpace * realSpace;
			movePartX = (points[2].x - points[0].x) / realSpace;
		}
		if(points[0].x > points[1].x)
		{
			moveRealX = (points[1].x - points[0].x) / realSpace * realSpace;
			movePartX = -(points[1].x - points[0].x) / realSpace;
		}
		if(points[0].y < points[4].y)
		{
			moveRealY = (points[4].y - points[0].y) / realSpace * realSpace;
			movePartY = (points[4].y - points[0].y) / realSpace;
		}
		if(points[0].y > points[3].y)
		{
			moveRealY = (points[3].y - points[0].y) / realSpace * realSpace;
			movePartY = -(points[3].y - points[0].y) / realSpace;
		}
		int north = (points[0].y - points[4].y + moveRealY) / realSpace;
		int south = (points[3].y - points[0].y - moveRealY) / realSpace;
		int west = (points[0].x - points[2].x + moveRealX) / realSpace;
		int east = (points[1].x - points[0].x - moveRealX) / realSpace;

		for(int i=1; i<=north; i++)		//绘制经过Y轴正半轴上的网格线
		{
			
			MoveToEx(hdc, points[2].x, points[0].y-i*realSpace + moveRealY, NULL);
			LineTo(hdc, points[1].x, points[0].y-i*realSpace + moveRealY);
		}
		
		for(int i=1; i<=south; i++)		//绘制经过Y轴负半轴上的网格线
		{
			MoveToEx(hdc, points[2].x, points[0].y+i*realSpace + moveRealY, NULL);
			LineTo(hdc, points[1].x, points[0].y+i*realSpace + moveRealY);
		}
		for(int i=1; i<=west; i++)    //绘制经过X轴负半轴上的网格线
		{
			MoveToEx(hdc,points[0].x-i*realSpace + moveRealX, points[4].y, NULL);
			LineTo(hdc, points[0].x-i*realSpace + moveRealX, points[3].y);
		}

		
		for(int i=1; i<=east; i++)		//绘制经过X轴正半轴上的网格线
		{
			MoveToEx(hdc,points[0].x+i*realSpace + moveRealX, points[4].y, NULL);
			LineTo(hdc, points[0].x+i*realSpace + moveRealX, points[3].y);
		}
	
		MoveToEx(hdc,points[1].x,points[4].y,NULL);
		LineTo(hdc,points[1].x,points[3].y);
		
		//绘制坐标轴
		hpen = ::CreatePen(PS_SOLID,2, RGB(0, 0, 0));
		::SelectObject(hdc, hpen);
		//X轴
		if(points[0].y > points[4].y && points[0].y < points[3].y)
		{
			MoveToEx(hdc, points[2].x, points[0].y + moveRealY, NULL);
			LineTo(hdc, points[1].x, points[0].y + moveRealY);

		}
		//Y轴
		if(points[0].x > points[2].x && points[0].x < points[1].x)
		{
			MoveToEx(hdc, points[0].x + moveRealX, points[4].y, NULL);
			LineTo(hdc, points[0].x + moveRealX, points[1].x);
		}

		//if(points[0].y > points[4].y && points[0].y < points[3].y)
		//绘制Y轴正半轴上的刻度
		if(points[0].x<points[1].x)
			for(int i=1; i<=north*10; i++)
			{
				
				MoveToEx(hdc, points[0].x-5 + moveRealX, points[0].y-i*realSpace + moveRealY, NULL);
				LineTo(hdc, points[0].x+5 + moveRealX, points[0].y-i*realSpace + moveRealY);

				wchar_t str[10];
				
				//_itow_s((double)((i+movePartY)*space),str,10);
				int number = (i+movePartY)*space<10?1:((i+movePartY)*space<100?2:3);
				_stprintf_s(str,_T("%.3f"),((i+movePartY)*space));
				TextOut(hdc, points[0].x+5 + moveRealX, points[0].y-i*realSpace-3 + moveRealY, str,number+3);

			}
			//绘制X轴正半轴上的刻度
			if(points[0].x<points[1].x)
			for(int i = 1;i<=east*10;i++)
			{
				MoveToEx(hdc,points[0].x+i*realSpace,points[0].y+5,NULL);
				if(points[0].x+i*realSpace<points[1].x){
				LineTo(hdc,points[0].x+i*realSpace,points[0].y-5);

				wchar_t str[10];
				//_itow_s(((i+movePartX)*space),str,10);
				int number = (i+movePartY)*space<10?1:((i+movePartX)*space<100?2:3);
				_stprintf_s(str,_T("%.3f"),((i+movePartY)*space));
				TextOut(hdc,points[0].x+i*realSpace+6,points[0].y+5,str,number+3);
				}

			}
			//绘制Y轴负半轴上的刻度
			if(points[0].x<points[1].x)
			for(int i = 1;i<=south*10;i++)
			{
				MoveToEx(hdc,points[0].x+5,points[0].y+i*realSpace,NULL);
				LineTo(hdc,points[0].x-5,points[0].y+i*realSpace);
				wchar_t str[10];

				int number = (i+movePartY)*space<10?1:((i+movePartY)*space<100?2:3);
				//_itow_s((-(i+movePartY)*space),str,10);
				_stprintf_s(str,_T("%.3f"),(-(i+movePartY)*space));
				TextOut(hdc, points[0].x+5, points[0].y+i*realSpace-3,str,number+4);
			}
			//绘制X轴负半轴上的刻度
			//if(points[0].x<points[1].x)
			for(int i = 1;i<=west*10;i++)
			{
				MoveToEx(hdc,points[0].x-i*realSpace,points[0].y+5,NULL);
				if(points[0].x-i*realSpace < points[1].x){
					LineTo(hdc,points[0].x-i*realSpace,points[0].y-5);
					wchar_t str[10];
					
					int number = (i+movePartY)*space<10?1:((i+movePartY)*space<100?2:3);
					_stprintf_s(str,_T("%.3f"),(-(i+movePartY)*space));
					TextOut(hdc, points[0].x-i*realSpace, points[0].y+5,str,number+4);
				}

			}
			if(points[0].x<points[1].x)
				TextOut(hdc,points[0].x+5,points[0].y+5,L"0",1);
		
		hpen = ::CreatePen(PS_SOLID,2, rgb_1);
		::SelectObject(hdc, hpen);
		if(test != NULL)
		{
			MoveToEx(hdc,test[0].x,test[0].y,NULL);	
			for(int i = 1;i<numOfPoints;i++)
			{

				//drawLine(hdc,test[i-1].x,test[i-1].y,test[i].x,test[i].y,color);
				if((test[i].y - test[i-1].y)<cyClient && (test[i-1].y - test[i].y)>-cyClient && test[i].x<points[1].x)//if(abs(test[i].y - test[i-1].y)<cyClient)
					if(test[i].y ==points[0].y&&test[i-1].y ==points[0].y)
					{}else
						LineTo(hdc,test[i].x,test[i].y);
				//::SetPixel(hdc,test[i].x, test[i].y, color);
				MoveToEx(hdc,test[i].x,test[i].y,NULL);
			}
		
		}
		
		hpen = ::CreatePen(PS_SOLID,2, rgb_2);
		::SelectObject(hdc, hpen);
		if(test2 != NULL)
		{
			MoveToEx(hdc,test2[0].x,test2[0].y,NULL);	
			for(int i = 1;i<numOfPoints;i++)
			{

				//drawLine(hdc,test2[i-1].x,test2[i-1].y,test2[i].x,test2[i].y,color);
				if((test2[i].y - test2[i-1].y)<cyClient && (test2[i-1].y - test2[i].y)>-cyClient && test2[i].x<points[1].x)//if(abs(test2[i].y - test2[i-1].y)<cyClient)
					if(test2[i].y ==points[0].y&&test2[i-1].y ==points[0].y)
					{}else
						LineTo(hdc,test2[i].x,test2[i].y);
				//::SetPixel(hdc,test2[i].x, test2[i].y, color);
				MoveToEx(hdc,test2[i].x,test2[i].y,NULL);
			}
		
		}

		hpen = ::CreatePen(PS_SOLID,2, rgb_3);
		::SelectObject(hdc, hpen);
		if(test3 != NULL)
		{
			MoveToEx(hdc,test3[0].x,test3[0].y,NULL);	
			for(int i = 1;i<numOfPoints;i++)
			{

				//drawLine(hdc,test2[i-1].x,test2[i-1].y,test2[i].x,test2[i].y,color);
				if((test3[i].y - test3[i-1].y)<cyClient && (test3[i-1].y - test3[i].y)>-cyClient && test3[i].x<points[1].x)//if(abs(test2[i].y - test2[i-1].y)<cyClient)
					if(test3[i].y ==points[0].y&&test3[i-1].y ==points[0].y)
					{}else
						LineTo(hdc,test3[i].x,test3[i].y);
				//::SetPixel(hdc,test2[i].x, test2[i].y, color);
				MoveToEx(hdc,test3[i].x,test3[i].y,NULL);
			}
		
		}
		hpen = ::CreatePen(PS_SOLID,2, RGB(100,200,100));
		::SelectObject(hdc, hpen);
		if(!dataFromCSV.empty()){
			it = dataFromCSV.begin();
			MoveToEx(hdc,(*it).x*realSpace/space+points[0].x,2*points[0].y-((*it).y*realSpace/space+points[0].y),NULL);		
			for(;it!=dataFromCSV.end();it++)
			{
				LineTo(hdc,(*it).x*realSpace/space+points[0].x,2*points[0].y-((*it).y*realSpace/space+points[0].y));
				MoveToEx(hdc,(*it).x*realSpace/space+points[0].x,2*points[0].y-((*it).y*realSpace/space+points[0].y),NULL);		
			}
		}
		
		if(!barData.empty())
		{
			HBRUSH hbr = CreateSolidBrush(RGB(0,255,0));
			HBRUSH hbrOld = (HBRUSH)SelectObject(hdc,hbr);
			
			
			map<float,int>::iterator it=barData.begin();
			while(it != barData.end())
			{
				Rectangle(hdc, (it->first-0.2)*realSpace/space+points[0].x, points[0].y, (it->first+0.2)*realSpace/space+points[0].x, 2*points[0].y-((it->second)*realSpace/space+points[0].y));
				MoveToEx(hdc,(it->first-0.2)*realSpace/space+points[0].x,points[0].y,NULL);
				LineTo(hdc,(it->first-0.2)*realSpace/space+points[0].x,2*points[0].y-((it->second)*realSpace/space+points[0].y));
				MoveToEx(hdc,(it->first-0.2)*realSpace/space+points[0].x,2*points[0].y-((it->second)*realSpace/space+points[0].y),NULL);
				LineTo(hdc,(it->first+0.2)*realSpace/space+points[0].x,2*points[0].y-((it->second)*realSpace/space+points[0].y));
				MoveToEx(hdc,(it->first+0.2)*realSpace/space+points[0].x,2*points[0].y-((it->second)*realSpace/space+points[0].y),NULL);
				LineTo(hdc,(it->first+0.2)*realSpace/space+points[0].x,points[0].y);
				
				TCHAR buf [10];
				_itow_s(it->second,buf,10);
				int number = it->second<10?1:(it->second<100?2:3);
				TextOut(hdc,(it->first-0.1)*realSpace/space+points[0].x,2*points[0].y-((it->second+0.5)*realSpace/space+points[0].y)-10,buf,number);
				it++;
			}
			::DeleteObject(hbr);
			::DeleteObject(hbrOld);
		}
		
		//::DeleteDC(hbr);
		::DeleteObject(hpen);
        EndPaint(hwnd, &ps);
		}
        return 0;  
  
    case WM_DESTROY:  
        PostQuitMessage(0);  
        return 0;  
    }  
  
    return DefWindowProc(hwnd, message, wParam, lParam);  
} 

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
	int iBits;			//每个像素所占字节数  
	WORD wBitCount;		//位图中每个像素所占字节数  
	DWORD dwPaletteSize=0, //调色板大小 
	dwBmBitsSize,          //位图中像素字节大小
	dwDIBSize, dwWritten;  //位图文件大小 ， 写入文件字节数  
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr;	//位图文件头结构  
	BITMAPINFOHEADER bi;		//位图信息头结构  
	LPBITMAPINFOHEADER lpbi;	//指向位图信息头结构  

	HANDLE fh, hDib, hPal,hOldPal=NULL; //定义文件，分配内存句柄，调色板句柄  

	//计算位图文件每个像素所占字节数  
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

	//计算调色板大小  
	if (wBitCount <= 8)  
	dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);  

	//设置位图信息头结构  
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;  
	bi.biPlanes = 1;  
	bi.biBitCount = wBitCount;  //1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)
	bi.biCompression = BI_RGB;  //不压缩
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;  

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ;  

	//为位图内容分配内存  
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  

	// 处理调色板  
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
	hDC = ::GetDC(NULL);  
	hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);  
	RealizePalette(hDC);  
	}  

	// 获取该调色板下新的像素值  
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,  
	(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)  
	+dwPaletteSize,  
	(LPBITMAPINFO )  
	lpbi, DIB_RGB_COLORS);  

	//恢复调色板  
	if (hOldPal)  
	{  
	SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
	RealizePalette(hDC);  
	::ReleaseDC(NULL, hDC);  
	}  

	//创建位图文件 

	fh = CreateFile(lpFileName, GENERIC_WRITE,  
	0, NULL, CREATE_ALWAYS,  
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);  

	if (fh == INVALID_HANDLE_VALUE)  
	return FALSE;  

	// 设置位图文件头  
	bmfHdr.bfType = 0x4D42;  
	dwDIBSize = sizeof(BITMAPFILEHEADER)  
	+ sizeof(BITMAPINFOHEADER)  
	+ dwPaletteSize + dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)  
	+ (DWORD)sizeof(BITMAPINFOHEADER)  
	+ dwPaletteSize;  

	// 写入位图文件头  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  

	// 写入位图文件其余内容  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,  
	&dwWritten, NULL);  

	//清除  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  
	return TRUE;  
}
BOOL OpenCSV(LPCTSTR lpFileName)
{
	HANDLE hFile;//定义一个句柄。
	hFile=CreateFile(lpFileName,
	   GENERIC_READ,
	   FILE_SHARE_READ,
	   NULL,
	   OPEN_EXISTING,
	   FILE_ATTRIBUTE_NORMAL,
	   NULL);//使用CreatFile这个API函数打开文件
	DWORD dwDataLen;
	char FileBuffer[1000];
	ReadFile(hFile,FileBuffer,100,&dwDataLen,NULL);//读取数据
	FileBuffer[dwDataLen]=0;//将数组未尾设零。
	CloseHandle(hFile);//关闭句柄
	
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
	//MessageBox(FileContent);//输出结果
}
BOOL OpenCSVForBar(LPCTSTR lpFileName)
{
		HANDLE hFile;//定义一个句柄。
	hFile=CreateFile(lpFileName,
	   GENERIC_READ,
	   FILE_SHARE_READ,
	   NULL,
	   OPEN_EXISTING,
	   FILE_ATTRIBUTE_NORMAL,
	   NULL);//打开文件
	DWORD dwDataLen;
	char FileBuffer[1000];
	ReadFile(hFile,FileBuffer,100,&dwDataLen,NULL);//读取数据
	FileBuffer[dwDataLen]=0;//将数组未尾设零。
	CloseHandle(hFile);//关闭句柄
	
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