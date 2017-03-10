#ifndef __AZ_LOGGING_FUNCTIONS_H_
#define __AZ_LOGGING_FUNCTIONS_H_

#include <string>
#include <log4cpp/Category.hh>

namespace log4cpp {
	
	class MethodLogger  {
	public:
		MethodLogger( const std::string &aClass, const std::string &aMethod, const std::string &aLogCat );
		virtual ~MethodLogger();
	public:
		std::string getMethod();

	public:
		CategoryStream debugWSignature();
		CategoryStream infoWSignature();
		CategoryStream noticeWSignature();
		CategoryStream warnWSignature();
		CategoryStream errorWSignature();
		CategoryStream critWSignature();
		CategoryStream alertWSignature();
		CategoryStream emerWSignature();
		CategoryStream fatalWSignature();

	private:
		MethodLogger();
		MethodLogger( const MethodLogger & );
		MethodLogger& operator=( const MethodLogger& );
	
	private:
		std::string m_class;
		std::string m_method;
		std::string m_logCat;
		static int  m_level;
	};

	namespace Allianz {
		/**
		 * Fügt die notwendigen Schlüssel in die Registry ein
		 */
		bool setupRegistry();

		/**
		 * ermittelt die Logging Priotiy
		 */
		log4cpp::Priority::Value getLoggingPrioty();

		/**
		 * Convenience Funktion. Baut eine Standardlogging Umgebung auf
		 */
		bool setupLogging();

		/**
		 * schreibt den letzten Fehler GetLastError in den Fehlerstrom
		 */
		void reportLastError( const std::string &prefix = std::string("") );

	}

	namespace logging {
		std::string getCat();
	}

}

#endif