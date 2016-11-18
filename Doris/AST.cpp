#include <utility>	
#include <string>
#include "AST.h"

using std::string;
using std::pair;


ASTNode::ASTNode()
{
}



ASTOR::ASTOR()
{
}

void ASTOR::Push(ASTNode* node)
{
	nodeVec_.push_back(node);
}


ASTCat::ASTCat()
{
}

void ASTCat::Push(ASTNode* node)
{
	nodeVec_.push_back(node);
}



ASTRepeat::ASTRepeat(ASTNode* node, bool greedy, int min, int max) :
	node_(node), greedy_(greedy), min_(min), max_(max)
{
}



ASTCharClass::ASTCharClass()
{
}

void ASTCharClass::Push(pair<char, char> range)
{
	ranges_.push_back(range);
}



ASTCharacter::ASTCharacter(char ch) : 
	ch_(ch)
{
}



ASTBackReference::ASTBackReference(int number) : 
	number_(number)
{
}



ASTAnchor::ASTAnchor(AnchorType type) : 
	type_(type)
{
}



ASTUnnameCapture::ASTUnnameCapture(ASTNode* node, int number) :
	node_(node), number_(number)
{
}



ASTNameCapture::ASTNameCapture(ASTNode* node, string name) :
	node_(node), name_(name)
{
}



ASTPstLookahead::ASTPstLookahead(ASTNode* node) :
	node_(node)
{
}



ASTNgtLookahead::ASTNgtLookahead(ASTNode* node) :
	node_(node)
{
}