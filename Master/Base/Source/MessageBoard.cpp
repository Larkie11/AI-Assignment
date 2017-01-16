#include "MessageBoard.h"

MessageBoard::MessageBoard()
{

}
MessageBoard::~MessageBoard()
{

}

void MessageBoard::SetMessage(string _message){ message = _message; }
void MessageBoard::SetFromLabel(string label){ fromLabel = label; }
void MessageBoard::SetToLabel(string label){ toLabel = label; }
string MessageBoard::GetMsg() { return message; }
string MessageBoard::GetFromLabel(){ return fromLabel; }
string MessageBoard::GetToLabel(){ return toLabel; }
void MessageBoard::Reset(){
	message = fromLabel = toLabel = "Nil";
}