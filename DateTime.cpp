#include "stdafx.h"
#include "The.h"
#include "datetime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CProfile theProfile;

const static char *svpszFormatDate[] = {
	"yy'/'M'/'d",
	"yyyy'/'MM'/'dd",
	"ggyy'年'M'月'd'日'",
	"yyyy'年'M'月'd'日'",	//98年 1月 2日
	"yyyy'年'M'月'd'日('ddd')'",	//98年 1月 2日
	"%m/%d(%a)",		//12/3(fri)
	"%d-%b-%Y",
	""	//end-mark
};
const static char *svpszFormatTime[] = {
	"%H:%M",
	"%H時%M分",
	"%H:%M:%S",
	"%H時%M分%S秒",
	"%p %I:%M",
	"%I:%M %p",
	"%X",
	""	//end-mark
};
#define APPEND_KEYSZ(a,b)	(IsValidDataSz(b) ? ((string(a)+b).c_str()):a)
////////////////////////////////////////////////////////////////////////////
// CDateTimesData
string CDateTimeData::GetNowFormatString_Next(bool bNext) 
{
	if (bNext) {
		m_index++;
	}else{
		m_index--;
	}
	return GetNowFormatString_First(-1);
}

void CDateTimeData::ReadWriteProfile(int mode, const char *szAppendKey, const char** ppszIni)
{
	static char	 stcGeneral[] = "General";
	static char	 stcDateTimeFmt[] = "DateTimeFmt";

	static char	 stcDateTimeIndex[] = "DateTimeIndex";

	if (mode==CProfile::__read) {
		string str;
		for (int i=0; (str = theProfile.ReadString(APPEND_KEYSZ(stcDateTimeFmt, szAppendKey), i)).size()>0; i++)
			m_vecstrFormat.push_back(str);
		if (m_vecstrFormat.size()==0) {
			for (int i=0; ppszIni[i][0]; i++)
				m_vecstrFormat.push_back(string(ppszIni[i]));
		}
		m_index = theProfile.ReadInt(stcGeneral, APPEND_KEYSZ(stcDateTimeIndex, szAppendKey), m_index);
	}else{
        int i;
		for (i=0; i<m_vecstrFormat.size(); i++)
			theProfile.WriteString(APPEND_KEYSZ(stcDateTimeFmt, szAppendKey), i, m_vecstrFormat[i].data());
		theProfile.WriteString(APPEND_KEYSZ(stcDateTimeFmt, szAppendKey), i, "");
		m_index = theProfile.WriteInt(stcGeneral, APPEND_KEYSZ(stcDateTimeIndex, szAppendKey), m_index);
	}
}

string CDateTimeData::GetNowFormatString_First(int index)
{
	time_t ltime;

	time( &ltime );
	return GetFormatString_First(ltime, index);
}

string CDateTimeData::GetFormatString_First(const time_t ltime, int index)
{
	struct tm *thisday = localtime( &ltime );    /* strftime 関数を使って時刻文字列を作成します。 */

	if (index==-1) {
		index = m_index;
	}
	if (index<0)
		index = SIZEMAC(m_vecstrFormat.size());
	else if (index>=m_vecstrFormat.size()) //HACK! if index==-1 then this if is TRUE
		index = 0;

	m_index = index;
	return GetDateTimeString(thisday, m_vecstrFormat[index].data());
}

string CDateTimeData::GetDateTimeString(const struct tm *theday, const char *szFormat)
{
	string str;
	str.resize(strlen(szFormat)<64 ? 256:strlen(szFormat)*2);
	if (*szFormat=='%')
		strftime( (char*)str.data(), str.size(), szFormat, theday );
	else{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);   // 現在の日付を求める
		GetDateFormat( LOCALE_USER_DEFAULT,
					   DATE_USE_ALT_CALENDAR,          // ※オプションの指定
					   &sysTime,
					   szFormat,
					   (char*)str.data(),
					   str.size()); 
	}
	str.resize((char*)memchr(str.data(), 0, str.size())-str.data());
	return str;
}

////////////////////////////////////////////////////////////////////////////
// CDateTimes
void CDateTimes::ReadWriteProfile(int mode)
{
	m_data[DT_DATE].ReadWriteProfile(mode, "_date", svpszFormatDate);
	m_data[DT_TIME].ReadWriteProfile(mode, "_time", svpszFormatTime);
}
