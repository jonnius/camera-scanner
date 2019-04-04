#ifndef DEBUGGER_H
#define DEBUGGER_H

struct DebugPipe {} debug;

#ifdef NOQT
	#include <iostream>
	inline void print(std::string msg)
	{
		std::cout << msg << std::endl;
	}
	
	template<typename T>
	DebugPipe& operator<<(DebugPipe& pipe, const T& obj)
	{
		std::cout << obj << std::endl;
		return pipe;
	}
#else
	#include <QDebug>
	
	inline void print(std::string msg)
	{
		qDebug() << QString::fromStdString(msg);
	}

	template<typename T>
	DebugPipe& operator<<(DebugPipe& pipe, const T& obj)
	{
		qDebug() << obj;
		return pipe;
	}
#endif

#endif
