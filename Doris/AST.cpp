#include <utility>	
#include <string>
#include "AST.h"
#include "NFA.h"

using std::string;
using std::pair;
using std::make_pair;

ASTNode::ASTNode()
{
}

pair<NFAState*, NFAState*> ASTNode::ConstructNFA()
{
	return make_pair(nullptr, nullptr);
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



ASTCharClass::ASTCharClass(bool negate) : 
	negate_(negate)
{
}

void ASTCharClass::Push(char begin, char end)
{
	ranges_.push_back(std::make_pair(begin, end));
}



ASTCharacter::ASTCharacter(char ch) : 
	ch_(ch)
{
}



ASTBackReference::ASTBackReference(int number) : 
	number_(number)
{
}



ASTNameReference::ASTNameReference(string name) :
	name_(name)
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