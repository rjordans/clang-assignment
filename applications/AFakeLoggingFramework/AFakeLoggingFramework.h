// AFakeLoggingFramework.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

using namespace std;

class AdditionalInfo
{
	string key_;
	string value_;
public:
	AdditionalInfo(string k, string v):key_(std::move(k)),value_(std::move(v)){}
	string stream() const {
		if(key_.empty()){
			return value_;
		}
		return "[" + key_ + "=" + value_ + "]";
	}
};
namespace format {
	static string AdditionalInfo(string key, string val)
	{
		return "[" + key + "=" + val + "]";
	}
}

class Event
{
	int id_{};
	string unit_;
	string description_;
	vector<AdditionalInfo> additionalInfos;

public:
	Event() = default;
	Event(const int id, string unit, string description):id_(id), unit_(std::move(unit)), description_(std::move(description)){}

	void operator<<(const AdditionalInfo& ai)
	{
		additionalInfos.push_back(ai);
	}

	Event& operator<<( const string& val)
	{
		additionalInfos.emplace_back("", val);
		return *this;
	}

	string stream() const
	{
		string ret = std::to_string(id_)+ ", " + unit_ + ", " + description_ +" ";
		for(const auto &ai: additionalInfos)
		{
			ret += ai.stream();
		}
		return ret;
	}
	void clone(const Event e) { id_ = e.id_; description_ = e.description_;  unit_ = e.unit_; }
	
};


extern map<int, Event> log_events;

class Logger
{
	
	vector<Event> events;

public:
	Logger() = default;
	//void push_back(const Event e) { events.push_back(e); }
	Event& log(int index)
	{
		events.emplace_back(log_events[index]);
		return events.back();
	}
	string stream() const
	{
		string ret = "this is a trace\n";
		
		for(const auto& e:events)
		{
			ret += e.stream()+"\n";
		}
		return ret;
	}
};