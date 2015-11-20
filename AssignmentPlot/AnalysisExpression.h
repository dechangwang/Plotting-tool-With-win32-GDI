#pragma once
#include<Windows.h>
#include<list>

#define numOfPoints 1000
class AnalysisExpression
{
private:
	char buf[255];
	
	std::list<POINT> points_list;
public:
	AnalysisExpression(void);
	~AnalysisExpression(void);
	void getPointsByExpresson(TCHAR [],POINT [],double cxClient,double cyClient,POINT points,double[],double[]);
	void getPointsByExpresson(TCHAR[],POINT [],double cxClient,double cyClient,POINT points,double xmove);//pointsÎªÔ²ÐÄ
	TCHAR * RPExpression(TCHAR *e);

	

};

