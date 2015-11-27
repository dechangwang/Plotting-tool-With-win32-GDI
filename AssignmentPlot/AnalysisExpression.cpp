#include "AnalysisExpression.h"
#include<iostream>
#include<Windows.h>
#include<math.h>

using namespace std;

AnalysisExpression::AnalysisExpression(void)
{
	
}

void AnalysisExpression:: getPointsByExpresson(TCHAR Expression[255],POINT pointsSet1[numOfPoints],double cxClient,double cyClient,POINT points,double pointsSet[numOfPoints],double pointsY[numOfPoints])
{
	
	int k = 0,ex =0;
	
	TCHAR expre[200];
	expre[ex++] = '$';
	while(Expression[k] != NULL)
	{
		if(Expression[k] ==' ')
			k++;
		expre[ex++] = Expression[k++];
	
	}
	expre[ex++] = '#';
	TCHAR *result = RPExpression(expre);
	TCHAR *p = result;
	TCHAR *beforeP= result;
	beforeP--;
	*beforeP = '$';
	//beforeP++;
	int resLoc = 1;
	//下面在公式中带入点
	
	float sum = 0;
	double _fresult[numOfPoints],_sresult[numOfPoints];//,pointsY[numOfPoints];
	for(int i = numOfPoints/2;i>0;i--)
	{
		pointsSet[numOfPoints/2-i]=_fresult[numOfPoints/2-i]=_sresult[numOfPoints/2-i]=-i*0.1;
		
	}
	for(int i = 0;i<numOfPoints/2;i++)
	{
		pointsSet[numOfPoints/2+i]=_fresult[numOfPoints/2+i]=_sresult[numOfPoints/2+i]=i*0.1;
	}
	float fresult=1.2,sresult=1.2;
	double _tempResult[numOfPoints][10];
	float tempResult[10];  //用来暂存遍历逆波兰式计算的结果
	
	char first,second;
	int pos = 0;
	bool finished  = false;
	bool firstIsF = false,secondIsF = false,Ispower = false;

	while(*p!='#')
	{	

			p++;
			while(*p =='%')
				p++;
			if(*p == '#')
			{
				--p;
				while(*p == '%')
					--p;
				switch (*p)
				{
				case 'x':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = pointsSet[i];
						}
					}
					break;
				case 's':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = sin(pointsSet[i]);
						}
					}
					break;
				case 'c':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = cos(pointsSet[i]);
						}
					}
					break;
				case 't':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = tan(pointsSet[i]);
						}
					}
					break;
				case 'l':
					for(int i = 0;i<numOfPoints;i++)
					{
						if(pointsSet[i] > 0)
							pointsY[i] = log(pointsSet[i]);
						else
							pointsY[i] = 0;
					}
					break;
				case 'f':
					{
							--resLoc;
						for(int i = 0;i<numOfPoints;i++)
						{
							pointsY[i] = _tempResult[i][resLoc];					
						}
					}
					break;
				default:
					{
						for(int i = 0;i<numOfPoints;i++)
						{
							pointsY[i] = *p-48;
						}
					}
					break;
				}

				for(int i = 0;i<numOfPoints;i++)
				{		
					pointsSet1[i].x = pointsSet[i]*cxClient+points.x;
					pointsSet1[i].y = 2*points.y -(pointsY[i]*cyClient+points.y);		
				}

				return;
			}else{
				--p;
				while(*p == '%')
					--p;
			}
		
			//x*x*x+3*x*x/(x+4)
		firstIsF = secondIsF = false;Ispower = false;
		first = *p;
		switch(first)
		{
		case 'x':
			{
				++p;
				if(*p == '^')
				{
					*p = '%';
					++p;
					if(isdigit(*p))
					{
						
						int n = *p-48;
						
						for(int i = 0;i<numOfPoints;i++)
						{
							double sum_ = 1;
							n = *p-48;
							while(n>0)
							{
								sum_ = sum_*(pointsSet[i]);
								--n;
							}
							_fresult[i] = sum_;
							_tempResult[i][resLoc] = sum_;
							
						}
						Ispower = true;
						resLoc++;
						*p = '%';
						--p;--p;
						*p = 'f';
					}
					/*
					++p;
					if(*p=='#')
					{
						
						*p = '1';
						++p;
						*p = '*';
						++p;
						*p='#';
						--p;--p;--p;
					}else{
						--p;
					}
					*/
				}else{
					--p;
					for(int i = 0;i<numOfPoints;i++)
					{
						_fresult[i] = pointsSet[i];
					}
				}
				
				fresult = 1.2;
			}
			break;
		case 's':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = sin(pointsSet[i]);
				}
			}
			fresult = sin(1.2);
			break;
		case 'c':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = cos(pointsSet[i]);
				}
			}
			fresult = cos(1.2);
			break;
		case 't':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = tan(pointsSet[i]);
				}
			}
			fresult = tan(1.2);
			break;
		case 'l':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					if(_fresult[i]<=0)
						_fresult[i] = 0;
					else
						_fresult[i] = log(pointsSet[i]);
				}
			}
			fresult = log(1.2);
			break;
		case 'f':
			{
				--resLoc;
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = _tempResult[i][resLoc];					
				}
			}

			fresult = tempResult[resLoc];
			firstIsF = true;
			break;
			
		default:
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = *p-48;					
				}
			}
			fresult = *p-48;
			break;		
		}
		/*
		if(*p == 's')
		{
			fresult = sin(1.2);
		}*/
		if(Ispower)
		{
			Ispower = false;
			continue;
		}
			
		++p;
		while(*p =='%')
		{
			++p;
			++pos;
		}

		if(*p == '#')
			break;

		if(*p == '*' || *p == '/' ||*p == '+' ||*p == '-')
		{
			second = *beforeP;
			finished = true;
			--p;
		}else{
			second = *p;pos+=1;
			switch(second)
			{
			case 'x':
				{
					
					++p;
				if(*p == '^')
				{
					*p = '%';
					++p;
					if(isdigit(*p))
					{
						int n = *p-48;
						
						for(int i = 0;i<numOfPoints;i++)
						{
							double sum_ = 1.00;
							n = *p-48;
							while(n>0)
							{
								sum_ = sum_*(pointsSet[i]);
								--n;
							}
							_sresult[i] = sum_;
							_tempResult[i][resLoc] = sum_;
							
						}
						resLoc++;
						Ispower = true;
						*p = '%';
						--p;--p;
						*p = 'f';
						--p;
						while(*p == '%')
							p--;
					}
				}else{
					--p;
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = pointsSet[i];
					}
				}
				}
				sresult = 1.2;
				break;
			case 's':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = sin(pointsSet[i]);
					}
				}
				sresult = sin(1.2);
				break;
			case 'c':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = cos(pointsSet[i]);
					}
				}
				sresult = cos(1.2);
				break;
			case 't':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = tan(pointsSet[i]);
					}
				}
				sresult = tan(1.2);
				break;
			case 'l':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						if(pointsSet[i] > 0)
							_sresult[i] = log(pointsSet[i]);
						else
							_sresult[i] = 0;
					}
				}
				sresult = log(1.2);
				break;
			case 'f':
				{
					secondIsF = true;
					--resLoc;
					if(firstIsF)
					{
						for(int i = 0;i<numOfPoints;i++)
							_sresult[i] = _fresult[i];
						
						sresult=fresult; 

						for(int i = 0;i<numOfPoints;i++)
							_fresult[i] = _tempResult[i][resLoc];  
						fresult = tempResult[resLoc];
						//fresult = tempResult[--resLoc];
					}else{

						for(int i = 0;i<numOfPoints;i++)
							_sresult[i] = _tempResult[i][resLoc];  
						sresult = tempResult[resLoc];
					}
						
				}
				
				break;
			default:
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = *p-48;					
					}
				}
				sresult = *p-48;
				break;
		
			}
			/*
			if(*p == 's')
			{
				
				sresult = sin(1.2);x*x*x+3*x*x/(x+4)
			}*/

		}
		if(Ispower)
		{
			Ispower = false;
			continue;
		}
		p++;
		while(*p =='%')
		{
			++p;
			++pos;
		}

		if(!isalpha(*p) && !isdigit(*p))
		{
			if(*p!= '#')
			{
				switch(*p)
				{
				case '*':
					{
						for (int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]*_sresult[i];
					}
					sum = fresult*sresult;
					break;
				case'/':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]/_sresult[i];
					}
					sum = fresult/sresult;
					break;
				case'+':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]+_sresult[i];
					}
					sum =fresult+sresult;
					break;
				case'-':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]-_sresult[i];
					}
					sum = fresult-sresult;
					break;
				}
				fresult = sum;
				sresult = 1.2;
				
				for(int i = 0;i<numOfPoints;i++)
					_tempResult[i][resLoc] = pointsY[i];
				tempResult[resLoc] = sum;
				resLoc = resLoc+1;

				if(finished)
					break;

				*p = '%';
				//--p;
				//*p = '%';
				--p;
				while(*p =='%')
					--p;
				*p = '%';
				--p;
				while(*p =='%')
					--p;
				//if(*p != 'x')
					*p = 'f';	
				--p;
				while(*p =='%')
						--p;
				if(*p =='$')
					p++;
				/*
				if(p == beforeP)
				{
					while(*p =='%')
						--p;
				}
					 
				else
					++p;
					*/
					

			}
		//	temp = pos;
		}else{
			fresult = 1.2;
			sresult = 1.2;
			if(firstIsF && secondIsF)
				resLoc+=2;
			else if(firstIsF || secondIsF)
				resLoc++;
			firstIsF = secondIsF = false;
			for(int i = 0;i<numOfPoints;i++)
			{
				_fresult[i] = pointsSet[i];
				_sresult[i] = pointsSet[i];
			}
			--p;
			while(*p =='%')
				--p;
		}
	}

	for(int i = 0;i<numOfPoints;i++)
	{
		pointsSet1[i].x = pointsSet[i]*cxClient+points.x;
		pointsSet1[i].y = 2*points.y -(pointsY[i]*cyClient+points.y);

	}

	return;		
}
void AnalysisExpression::getPointsByExpresson(TCHAR rpex[],POINT pointsResult[numOfPoints],double cxClient,double cyClient,POINT points,double xmove)
{
	TCHAR *p = rpex;
	TCHAR *beforeP= rpex;
	beforeP--;
	*beforeP = '$';
	//beforeP++;
	double pointsSet[numOfPoints];
	double pointsY[numOfPoints];
	int resLoc = 1;
	//下面在公式中带入点
	
	float sum = 0;
	double _fresult[numOfPoints],_sresult[numOfPoints];//,pointsY[numOfPoints];
	for(int i = numOfPoints/2;i>0;i--)
	{
		pointsSet[numOfPoints/2-i]=_fresult[numOfPoints/2-i]=_sresult[numOfPoints/2-i]=-i*0.02+xmove;
	
	}
	for(int i = 0;i<numOfPoints/2;i++)	
	{
		pointsSet[numOfPoints/2+i]=_fresult[numOfPoints/2+i]=_sresult[numOfPoints/2+i]=i*0.02+xmove;
	}
		
	float fresult=1.2,sresult=1.2;
	double _tempResult[numOfPoints][10];
	float tempResult[10];  //用来暂存遍历逆波兰式计算的结果
	
	char first,second;
	int pos = 0;
	bool finished  = false;
	bool firstIsF = false,secondIsF = false,Ispower = false;

	while(*p!='#')
	{	

			p++;
			while(*p =='%')
				p++;
			if(*p == '#')
			{
				--p;
				while(*p == '%')
					--p;
				switch (*p)
				{
				case 'x':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = pointsSet[i];
						}
					}
					break;
				case 's':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = sin(pointsSet[i]);
						}
					}
					break;
				case 'c':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = cos(pointsSet[i]);
						}
					}
					break;
				case 't':
					{
						for(int i=0;i<numOfPoints;i++)
						{
							pointsY[i] = tan(pointsSet[i]);
						}
					}
					break;
				case 'l':
					for(int i = 0;i<numOfPoints;i++)
					{
						if(pointsSet[i] > 0)
							pointsY[i] = log(pointsSet[i]);
						else
							pointsY[i] = 0;
					}
					break;
				case 'f':
					{
							--resLoc;
						for(int i = 0;i<numOfPoints;i++)
						{
							pointsY[i] = _tempResult[i][resLoc];					
						}
					}
					break;
				default:
					{
						for(int i = 0;i<numOfPoints;i++)
						{
							pointsY[i] = *p-48;
						}
					}
					break;
				}

				for(int i = 0;i<numOfPoints;i++)
				{		
					pointsResult[i].x = pointsSet[i]*cxClient+points.x;
					pointsResult[i].y = 2*points.y -(pointsY[i]*cyClient+points.y);		
				}

				return;
			}else{
				--p;
				while(*p == '%')
					--p;
			}
		
			//x*x*x+3*x*x/(x+4)
		firstIsF = secondIsF = false;Ispower = false;
		first = *p;
		switch(first)
		{
		case 'x':
			{
				++p;
				if(*p == '^')
				{
					*p = '%';
					++p;
					if(isdigit(*p))
					{
						
						int n = *p-48;
						
						for(int i = 0;i<numOfPoints;i++)
						{
							double sum_ = 1;
							n = *p-48;
							while(n>0)
							{
								sum_ = sum_*(pointsSet[i]);
								--n;
							}
							_fresult[i] = sum_;
							_tempResult[i][resLoc] = sum_;
							
						}
						Ispower = true;
						resLoc++;
						*p = '%';
						--p;--p;
						*p = 'f';
					}
				}else{
					--p;
					for(int i = 0;i<numOfPoints;i++)
					{
						_fresult[i] = pointsSet[i];
					}
				}
				
				fresult = 1.2;
			}
			break;
		case 's':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = sin(pointsSet[i]);
				}
			}
			fresult = sin(1.2);
			break;
		case 'c':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = cos(pointsSet[i]);
				}
			}
			fresult = cos(1.2);
			break;
		case 't':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = tan(pointsSet[i]);
				}
			}
			fresult = tan(1.2);
			break;
		case 'l':
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					if(_fresult[i]<=0)
						_fresult[i] = 0;
					else
						_fresult[i] = log(pointsSet[i]);
				}
			}
			fresult = log(1.2);
			break;
		case 'f':
			{
				--resLoc;
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = _tempResult[i][resLoc];					
				}
			}

			fresult = tempResult[resLoc];
			firstIsF = true;
			break;
			
		default:
			{
				for(int i = 0;i<numOfPoints;i++)
				{
					_fresult[i] = *p-48;					
				}
			}
			fresult = *p-48;
			break;		
		}
		/*
		if(*p == 's')
		{
			fresult = sin(1.2);
		}*/
		if(Ispower)
		{
			Ispower = false;
			continue;
		}
			
		++p;
		while(*p =='%')
		{
			++p;
			++pos;
		}

		if(*p == '#')
			break;

		if(*p == '*' || *p == '/' ||*p == '+' ||*p == '-')
		{
			second = *beforeP;
			finished = true;
			--p;
		}else{
			second = *p;pos+=1;
			switch(second)
			{
			case 'x':
				{
					
					++p;
				if(*p == '^')
				{
					*p = '%';
					++p;
					if(isdigit(*p))
					{
						int n = *p-48;
						
						for(int i = 0;i<numOfPoints;i++)
						{
							double sum_ = 1.00;
							n = *p-48;
							while(n>0)
							{
								sum_ = sum_*(pointsSet[i]);
								--n;
							}
							_sresult[i] = sum_;
							_tempResult[i][resLoc] = sum_;
							
						}
						resLoc++;
						Ispower = true;
						*p = '%';
						--p;--p;
						*p = 'f';
						--p;
						while(*p == '%')
							p--;
					}
				}else{
					--p;
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = pointsSet[i];
					}
				}
				}
				sresult = 1.2;
				break;
			case 's':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = sin(pointsSet[i]);
					}
				}
				sresult = sin(1.2);
				break;
			case 'c':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = cos(pointsSet[i]);
					}
				}
				sresult = cos(1.2);
				break;
			case 't':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = tan(pointsSet[i]);
					}
				}
				sresult = tan(1.2);
				break;
			case 'l':
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						if(pointsSet[i] > 0)
							_sresult[i] = log(pointsSet[i]);
						else
							_sresult[i] = 0;
					}
				}
				sresult = log(1.2);
				break;
			case 'f':
				{
					secondIsF = true;
					--resLoc;
					if(firstIsF)
					{
						for(int i = 0;i<numOfPoints;i++)
							_sresult[i] = _fresult[i];
						
						sresult=fresult; 

						for(int i = 0;i<numOfPoints;i++)
							_fresult[i] = _tempResult[i][resLoc];  
						fresult = tempResult[resLoc];
						//fresult = tempResult[--resLoc];
					}else{

						for(int i = 0;i<numOfPoints;i++)
							_sresult[i] = _tempResult[i][resLoc];  
						sresult = tempResult[resLoc];
					}
						
				}
				
				break;
			default:
				{
					for(int i = 0;i<numOfPoints;i++)
					{
						_sresult[i] = *p-48;					
					}
				}
				sresult = *p-48;
				break;
		
			}
			/*
			if(*p == 's')
			{
				
				sresult = sin(1.2);x*x*x+3*x*x/(x+4)
			}*/

		}
		if(Ispower)
		{
			Ispower = false;
			continue;
		}
		p++;
		while(*p =='%')
		{
			++p;
			++pos;
		}

		if(!isalpha(*p) && !isdigit(*p))
		{
			if(*p!= '#')
			{
				switch(*p)
				{
				case '*':
					{
						for (int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]*_sresult[i];
					}
					sum = fresult*sresult;
					break;
				case'/':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]/_sresult[i];
					}
					sum = fresult/sresult;
					break;
				case'+':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]+_sresult[i];
					}
					sum =fresult+sresult;
					break;
				case'-':
					{
						for(int i = 0;i<numOfPoints;i++)
							pointsY[i] = _fresult[i]-_sresult[i];
					}
					sum = fresult-sresult;
					break;
				}
				fresult = sum;
				sresult = 1.2;
				
				for(int i = 0;i<numOfPoints;i++)
					_tempResult[i][resLoc] = pointsY[i];
				tempResult[resLoc] = sum;
				resLoc = resLoc+1;

				if(finished)
					break;

				*p = '%';
				//--p;
				//*p = '%';
				--p;
				while(*p =='%')
					--p;
				*p = '%';
				--p;
				while(*p =='%')
					--p;
				//if(*p != 'x')
					*p = 'f';	
				--p;
				while(*p =='%')
						--p;
				if(*p =='$')
					p++;
				/*
				if(p == beforeP)
				{
					while(*p =='%')
						--p;
				}
					 
				else
					++p;
					*/
					

			}
		//	temp = pos;
		}else{
			fresult = 1.2;
			sresult = 1.2;
			if(firstIsF && secondIsF)
				resLoc+=2;
			else if(firstIsF || secondIsF)
				resLoc++;
			firstIsF = secondIsF = false;
			for(int i = 0;i<numOfPoints;i++)
			{
				_fresult[i] = pointsSet[i];
				_sresult[i] = pointsSet[i];
			}
			--p;
			while(*p =='%')
				--p;
		}
	}

	for(int i = 0;i<numOfPoints;i++)
	{
		pointsResult[i].x = pointsSet[i]*cxClient+points.x;
		pointsResult[i].y = 2*points.y -(pointsY[i]*cyClient+points.y);

	}

	return;		
}

//将数学公式转换为逆波兰式
TCHAR * AnalysisExpression:: RPExpression(TCHAR *expre)
/* 返回表达式e的波兰式 */
{
//栈s1用于存放运算符，栈s2用于存放波兰式
	TCHAR err[]={'E','R','R'};

	TCHAR s1[100],s2[200];
	int top1 = 0,top2 = 0;
	TCHAR *p=expre,ch;
	s1[0] = '#';
	p++;
	if(*p == '-')
	{
		s2[top2++] = *p;
		p++;
	}
		
	for(;*p!='#';p++)//逐个字符访问
	{
		switch(*p)
		{
		case '(':
			s1[top1++] = *p;
			break;
			
		case ')':
			--top1;
			while(s1[top1]!='(')
			{
				s2[top2++] = s1[top1--];
				//top1;
			}
		
			//top1--;
			 
			break;
		case '+':
		case '-':
			{
			bool isInsert = false;			
			while(top1>0)
			{ 
				ch = s1[--top1];
				if(ch =='*' || ch=='/'|ch =='-')
				{
					while(s1[top1] =='*' | s1[top1] =='/'|s1[top1] =='-')
					{
						s2[top2++] = ch;
						--top1;
						ch = s1[top1];
					}
					break;
					
					
				}else
				{
					s1[++top1] = *p;
					isInsert = true;
					top1++;
					break;
				}
			}
			if(!isInsert)
			{
				if(top1==0 && s1[top1] =='#')
					s1[top1++] = *p;
				else{
					s1[++top1] = *p;
					++top1;
				}
			}
	
			}
			break;
		case '*':
		case '/':
			s1[top1++] = *p;
			break;
		case '^':
			s2[top2++] = '^';
			break;
		case 's':
			{
				p++;
				if(*p == 'i')
				{
					p++; 
					if(*p == 'n')
					{
						p++; 
						if(*p == '(')
						{
							p++; 
							if(*p == 'x')
							{
								s2[top2++]='s';
								p++; 
								if(*p == ')')
								{
									//p++; 
									//s2[top2++]='x';
								}else{
									return err;
								}
									
							}

						}else{
							return err;
						}
					}else{
						return err;
					}
				}else{
					return err;
				}
			}
			break;
		case 'c':
			{
				p++;
				if(*p == 'o')
				{
					p++; 
					if(*p == 's')
					{
						p++; 
						if(*p == '(')
						{
							p++; 
							if(*p == 'x')
							{
								s2[top2++]='c';
								p++; 
								if(*p != ')')
								{
									return err;
								}
									
							}

						}else{
							return err;
						}
					}else{
						return err;
					}
				}else{
					return err;
				}
			}
			break;
		case 't':
			{
			p++;
			if(*p == 'a')
			{
				p++; 
				if(*p == 'n')
				{
					p++; 
					if(*p == '(')
					{
						p++; 
						if(*p == 'x')
						{
							s2[top2++]='t';
							p++; 
							if(*p != ')')
							{
								return err;
							}
						}
					}else{
						return err;
					}
				}else{
					return err;
				}
			}else{
				return err;
			}
			}
			break;
		case 'l':
			{
			p++;
			if(*p == 'o')
			{
				p++; 
				if(*p == 'g')
				{
					p++; 
					if(*p == '(')
					{
						p++; 
						if(*p == 'x')
						{
							s2[top2++]='l';
							p++; 
							if(*p != ')')
							{
								return err;
							}
						}
					}else{
						return err;
					}
				}else{
					return err;
				}
			}else{
				return err;
			}
			}
			break;
		default:
			s2[top2++]=*p;
			break;

		}
	}    
 //若栈s1非空，则将栈中元素依次弹出并压入栈s2中
	while(top1>0)
	{
		ch = s1[--top1];
		s2[top2++] = ch;
	}
 	s2[top2++] = '#';
	return s2;
}
AnalysisExpression::~AnalysisExpression(void)
{
}
