// AFakeLoggingFramework.cpp : Defines the entry point for the application.
//

#include "AFakeLoggingFramework.h"


using namespace std;

int anotherSimpleFunction(int arg);

int aSimpleFunction(int arg)
{
	int val = arg * anotherSimpleFunction(2);
	val += 1;
	return val + 1; 
}

int yetAnotherVar = aSimpleFunction(3);
int AnotherVar = (2* yetAnotherVar)+1;
int varId = 20 + AnotherVar;

map<int, Event> log_events = {
	{1, {10, "IEC", "this is the first possible event"}},
	{2, {varId, "IEC", "this is the second possible event" }},
	{3, {31 + AnotherVar, "IEC", "this is the third possible event" }},
	{4, {46 + yetAnotherVar, "IEC", "this is the forth possible event" }},
	{5, {52 + aSimpleFunction(10), "IEC", "this is the fifth possible event" }},
	{6, {6 + aSimpleFunction(2), "IEC", "this is the sixth possible event" }},
	{7, {70 + aSimpleFunction(2000), "IEC", "this is the seventh possible event" }},
	{8, {80 + aSimpleFunction(2000), "IEC", "From within the sender" }},
	{9, {90 + aSimpleFunction(2000), "IEC", "From within the receiver" }},
};

int anotherSimpleFunction(int arg)
{
	return arg;
}

class receiver
{
private:
	Logger logger;

public:
	receiver()
	{
		logger.log(9) << "From within the receiver constructor";
	}

	void receive()
	{
		logger.log(9) << "The receiver receives";
	}

	void logAll()
	{
		cout << logger.stream();
	}
};

class sender
{
private:
	Logger logger;

public:
	sender()
	{
		logger.log(8) << "From within the sender constructor";
	}

	void send(receiver r)
	{
		logger.log(8) << "The sender sends";
		r.receive();
	}

	void logAll()
	{
		cout << logger.stream();
	}
};




int main()
{
	Logger logger;

	for (int i = 0; i < 6; i++) {
		logger.log(1) << "--FreeText because I am lazy " << "and again " << "and again ...--";
		logger.log(2) << AdditionalInfo("good", "way");
		logger.log(3) << "--Again FreeText because I am really lazy--";
		logger.log(1) << "--This is new--";
		logger.log(i) << "--This log() call has a variable arg % i-- ";
		logger.log(7) << format::AdditionalInfo("first tag", "first value")
		<< format::AdditionalInfo("second tag", "second value")
			<< format::AdditionalInfo("third tag", "third value");
		logger.log(7) << AdditionalInfo("stream tag", "stream value").stream();
		string x = format::AdditionalInfo("User", "val");
		logger.log(4) << x;
		logger.log(7) << AdditionalInfo("last stream tag", "stream value").stream();
		logger.log(1);

	}
	
	cout<<logger.stream();


	sender s;
	const receiver r;

	s.send(r);
	
	
	return 0;
}
