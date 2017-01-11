#pragma once
#include <string>

using std::string;

class MessageBoard
{
public:
	string message, fromLabel, toLabel;
	MessageBoard() { Reset(); }
	void SetMessage(string _message){ message = _message; }
	void SetFromLabel(string label){ fromLabel = label; }
	void SetToLabel(string label){ toLabel = label; }
	string GetMsg() { return message; }
	string GetFromLabel(){ return fromLabel; }
	string GetToLabel(){ return toLabel; }
	void Reset(){
		message = fromLabel = toLabel = "Nil";
	}
};