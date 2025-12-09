#ifndef __DATETIME_H__
#define __DATETIME_H__
#include <time.h>
#include "profile.h"
//#include <wchar.h>


typedef vector<string> STRVECTOR;

class CDateTimeData
{
public:
	CDateTimeData() {m_index = 0;}
public:
	string GetDateTimeString(const struct tm *today, const char *szFormat);
	string GetNowFormatString_First(int index=-1);
	string GetNowFormatString_Next(bool bNext);
	void ReadWriteProfile(int mode, const char *szAppendKey, const char** ppszIni);
	string GetFormatString_First(const time_t ltime, int index=-1);
private:
	STRVECTOR	m_vecstrFormat;
	int			m_index;
};
class CDateTimes
{
public:
	CDateTimes() {};
public:
	enum{
		DT_DATE,
		DT_TIME,
		DT_MAX
	};
	string GetTodayString_First() {return m_data[DT_DATE].GetNowFormatString_First(); }
	string GetTodayString_Next(bool bNext=true) {return m_data[DT_DATE].GetNowFormatString_Next(bNext); }
	string GetThedayString_First(const time_t ltime) {return m_data[DT_DATE].GetFormatString_First(ltime); }
	string GetThisTimeString_First() {return m_data[DT_TIME].GetNowFormatString_First(); }
	string GetThisTimeString_Next(bool bNext=true) {return m_data[DT_TIME].GetNowFormatString_Next(bNext); }
	string GetTheTimeString_First(const time_t ltime) {return m_data[DT_TIME].GetFormatString_First(ltime); }
	void ReadWriteProfile(int mode);
private:
	CDateTimeData	m_data[DT_MAX];
};
#endif //__DATETIME_H__