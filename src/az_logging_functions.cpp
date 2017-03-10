#include <log4cpp/az_logging_functions.h>

#include <log4cpp/Win32DebugAppender.hh>
#include <log4cpp/BasicLayout.hh>

#include <windows.h>

const char * REG_PATH = "Software\\Allianz\\AAGT\\Silva32\\Trace";
const char * DEFAULT_LEVEL = "0";

namespace log4cpp {

	MethodLogger::MethodLogger( const std::string &aClass, const std::string &aMethod, const std::string &aLogCat ) 
			: m_class(aClass)
			, m_method(aMethod)
			, m_logCat(aLogCat)
	{
		Category &cat = log4cpp::Category::getInstance( m_logCat );
		
		cat.debugStream () << "<" << m_class << "::" << m_method << ">" ;
	}

	MethodLogger::~MethodLogger() 
	{
		Category &cat = log4cpp::Category::getInstance( m_logCat );
		
		cat.debugStream() << "</" << m_class << "::" << m_method << ">";
	}

	std::string MethodLogger::getMethod()
	{
		std::string res;

		res = m_class;
		res += "::";
		res += m_method;

		return res;
	}


	CategoryStream MethodLogger::debugWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.debugStream() << getMethod() << " - ";

		return cat.debugStream();
	}
	
	CategoryStream MethodLogger::infoWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.infoStream() << getMethod() << " - ";

		return cat.infoStream();
	}
	
	CategoryStream MethodLogger::noticeWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.noticeStream() << getMethod() << " - ";

		return cat.noticeStream();
	}
	
	CategoryStream MethodLogger::warnWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.warnStream() << getMethod() << " - ";

		return cat.warnStream();
	}
	
	CategoryStream MethodLogger::errorWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.errorStream() << getMethod() << " - ";

		return cat.errorStream();
	}
	
	CategoryStream MethodLogger::critWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.critStream() << getMethod() << " - ";

		return cat.critStream();
	}
	
	CategoryStream MethodLogger::alertWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.alertStream() << getMethod() << " - ";

		return cat.alertStream();
	}
	
	CategoryStream MethodLogger::emerWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.emergStream() << getMethod() << " - ";

		return cat.emergStream();
	}

	CategoryStream MethodLogger::fatalWSignature()
	{
		Category &cat = Category::getInstance( m_logCat );

		cat.fatalStream() << getMethod() << " - ";

		return cat.fatalStream();
	}


	int  MethodLogger::m_level = 0;
}


bool log4cpp::Allianz::setupRegistry() 
{
	HKEY	hKey;
	LONG	err;
	DWORD	position;

	std::string modul = log4cpp::logging::getCat();

	//check for key in registry...
	if( getLoggingPrioty() != log4cpp::Priority::ERROR )
		return false;

    err = ::RegCreateKeyEx( HKEY_LOCAL_MACHINE, REG_PATH, 0, "", 
		REG_OPTION_NON_VOLATILE,  KEY_WRITE, NULL, &hKey, &position);
	if( ERROR_SUCCESS != err ) {
		RegCloseKey(hKey);
		return false;
    }

    err = ::RegSetValueEx( hKey, modul.c_str(), NULL, REG_SZ, (BYTE*)DEFAULT_LEVEL, sizeof(DEFAULT_LEVEL) );
	if( ERROR_SUCCESS == err ) {
		return false;
	}

	return ERROR_SUCCESS == ::RegCloseKey( hKey );
}


log4cpp::Priority::Value log4cpp::Allianz::getLoggingPrioty()
{
	HKEY	hKey;
	BYTE	data[10];
	LONG	err;
	DWORD	dataLen = sizeof(data);
	DWORD	dwType = REG_SZ;

	std::string			modul = log4cpp::logging::getCat();
	log4cpp::Priority::Value result;

    err = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_READ, &hKey ); 
	if( ERROR_SUCCESS != err ) {
		::RegCloseKey(hKey);
		return log4cpp::Priority::ERROR;
    }

    
	err = ::RegQueryValueEx( hKey, modul.c_str(), 0, &dwType, data, &dataLen );

	if( ERROR_SUCCESS != err ) {
		::RegCloseKey(hKey);
		return log4cpp::Priority::ERROR;
	}

	::RegCloseKey( hKey );

	switch( atol((char*)data) ) {
	case -1:
		result = log4cpp::Priority::FATAL;
		break;
	case 0:
		result = log4cpp::Priority::ERROR;
		break;
	case 1:
		result = log4cpp::Priority::WARN;
		break;
	case 2:
		result = log4cpp::Priority::NOTICE;
		break;
	case 3:
		result = log4cpp::Priority::INFO;
		break;
	case 4:
	default:
		result = log4cpp::Priority::DEBUG;
		break;
	}
	
	return result;
}
 
static log4cpp::Win32DebugAppender *modul_appender;
static log4cpp::BasicLayout *modul_layout;

/**
 *
 */
bool log4cpp::Allianz::setupLogging()
{
	modul_appender = new log4cpp::Win32DebugAppender("default");
	modul_layout = new log4cpp::BasicLayout();
	
	modul_appender->setThreshold( log4cpp::Priority::DEBUG );
	modul_appender->setLayout( modul_layout );

	
	log4cpp::Category::getInstance( logging::getCat() ).setPriority( getLoggingPrioty() );
	log4cpp::Category::getInstance( logging::getCat() ).addAppender( modul_appender );

	return true;
}


void log4cpp::Allianz::reportLastError( const std::string &prefix )
{
	std::string logString = prefix;
	
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	logString += (LPCTSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );

	log4cpp::Category::getInstance( logging::getCat() ).errorStream() 
		<< logString;
}

