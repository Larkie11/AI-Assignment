#pragma once
#include <string>

using std::string;

class MessageBoard
{
public:
	string message, fromLabel, toLabel;
	MessageBoard();
	~MessageBoard();
	void SetMessage(string _message);
	void SetFromLabel(string label);
	void SetToLabel(string label);
	string GetMsg();
	string GetFromLabel();
	string GetToLabel();
	void Reset();
};