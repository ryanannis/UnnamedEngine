#include "PropParser.h"

#include <sstream>
#include <iostream>
#include <stack>

enum class DSMState
{
	NULLSTATE,
	START,
	Sym, //T
	ALPHANUM, //NT
	RELATIVEPATH, //NT
	END // T
};

bool GetTerminal(DSMState state)
{
	return(
		state == DSMState::START ||
		state == DSMState::Sym ||
		state == DSMState::ALPHANUM ||
		state == DSMState::RELATIVEPATH ||
		state == DSMState::END
	);
}

PropParser::PropParser()
{
}

struct UDFTokenInternal
{
	UDFTokenInternal(TokenType type, std::string value, int line) :
		type(type), value(value), line(line) {}
	TokenType type;
	std::string value;
	int line;
};

void PropParser::Tokenize(std::string str, std::vector<UDFToken>& tokens)
{
	/*
	 * Parse the input string using a DSM
	 * This is easy since the only thing with more than one char is an alphanumeric.
	 */
	DSMState state = DSMState::START;

	std::stringstream tokenVal;

	std::vector<UDFTokenInternal> internalTokens;

	bool failed = false;
	int lnCount = 1; //for debugging

	for(size_t i = 0; i < (str.length() + 1); i++)
	{
		DSMState nextState = DSMState::NULLSTATE;

		char c;
		if(i == str.length())
		{
			c = '$';
		}
		else
		{
			c = str[i];
			
			// If the user tries to actually use our EOL character
			// then replace it with an invalid character
			if(c == '$')
			{
				c = '#';
			}
		}

		bool shouldTerminate = false;
		// Relative Path Chars
		if(c == '/' || c == '\\' || c == '.')
		{
			if
			(
				state == DSMState::ALPHANUM ||
				state == DSMState::RELATIVEPATH ||
				state == DSMState::START
			)
			{
				nextState = DSMState::RELATIVEPATH;
			}
			else
			{
				shouldTerminate = true;
			}
		}
		else if((c >= 'A' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
		{
			if(state == DSMState::RELATIVEPATH)
			{
				nextState = DSMState::RELATIVEPATH;
			}
			else if(state == DSMState::ALPHANUM || state == DSMState::START)
			{
				nextState = DSMState::ALPHANUM;
			}
			else
			{
				shouldTerminate = true;
				nextState = DSMState::ALPHANUM;
			}
		}
		else if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\x0b') {
			if(c == '\n')
			{
				lnCount++;
			}
			continue;
		}
		else if(c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == '=' || c == ';')
		{
			shouldTerminate = true;
			nextState = DSMState::Sym;
		}
		else if(c == '$')
		{
			shouldTerminate = true;
			nextState = DSMState::END;
		}
		else
		{
			ParserFailed(lnCount, std::to_string(c));
			failed = true;
			break;
		}

		if(shouldTerminate)
		{
			// Generate a new token everytime the DSM state switches
			TokenType tokenType = TokenType::NONE;

			if(state == DSMState::ALPHANUM)
			{
				tokenType = TokenType::ALPHANUM;
			}
			else if(state == DSMState::RELATIVEPATH)
			{
				tokenType = TokenType::FILEPATH;
			}
			else if(state == DSMState::Sym)
			{
				//handle this later
				tokenType = TokenType::NONE; 
			}

			internalTokens.push_back(UDFTokenInternal(tokenType, tokenVal.str(), lnCount));
			
			tokenVal.clear();
			tokenVal.str(std::string());
		}
		state = nextState;
		tokenVal << c;
	}

	// Replace SYMs with the proper tokens
	for(UDFTokenInternal& t : internalTokens)
	{
		if(t.type == TokenType::NONE)
		{
			if(t.value == "(")
			{
				t.type = TokenType::LBRACKET;
			}
			else if(t.value == ")")
			{
				t.type = TokenType::RBRACKET;
			}
			else if(t.value == "{")
			{
				t.type = TokenType::LCURLY;
			}
			else if(t.value == "}")
			{
				t.type = TokenType::RCURLY;
			}
			else if(t.value == ":")
			{
				t.type = TokenType::COLON;
			}
			else if(t.value == "=")
			{
				t.type = TokenType::EQUALS;
			}
			else if(t.value == ";")
			{
				t.type = TokenType::SEMI;
			}
			else
			{
				//Why did something of TokenType::None even get past the regular parsing?
				ParserFailed(t.line, t.value);
				failed = true;
				break;
			}
		}
	}

	if(!failed)
	{
		for(UDFTokenInternal& t : internalTokens)
		{
			tokens.emplace_back(t.type, t.value);
		}
	}

	return;
}

void PropParser::ParserFailed(int lineNumber, std::string around)
{
	std::cerr << "ERROR:  Parsing failed at line " << lineNumber << " around " << around << std::endl;
}

std::optional<PropTree> PropParser::ParseTokens(std::vector<UDFToken>& tokens)
{
	/* This is (hopefully) simple enough we can get away without a real L** parser*/
	PropTree root("Root");

	//This is only used on components to compare addresses
	std::stack<PropTree *> propTreeStack;
	propTreeStack.push(&root);

	for(size_t i = 0; i < tokens.size(); i++)
	{
		if(tokens[i].type == TokenType::ALPHANUM)
		{
			if(tokens[i + 1].type == TokenType::EQUALS)
			{
				// Parse as leaf until ';'
				PropTreeLeaf leaf;
				leaf.key = tokens[i].value;

				std::stringstream ss;
				// Start parsing leaf
				for(i = i + 2; i < tokens.size(); i++)
				{
					if(tokens[i].type == TokenType::SEMI)
					{
						break;
					}
					else
					{
						// todo: parse into the actual valueonce we figure out what exactly should be enumeratoed
						ss << tokens[i].value;
					}
				}
				leaf.value = ss.str();
				auto& topLeaves = propTreeStack.top()->leaves;
				topLeaves.push_back(leaf);
			}
			else if(tokens[i+1].type == TokenType::LCURLY)
			{
				// Create a nested with the typename we just parsed
				auto& topComponents = propTreeStack.top()->components;
				topComponents.emplace_back(tokens[i].value);

				// Put this new token at the top of the stack
				propTreeStack.push(&(topComponents[topComponents.size() - 1]));

				// We consumed an additional token [i+1] here, so advance
				i++;
			}
		}
		else if(tokens[i].type == TokenType::RCURLY)
		{
			if(propTreeStack.empty())
			{
				goto FAIL;
			}
			propTreeStack.pop();
		}
		else
		{
			goto FAIL;
		}
	}

	return(root);

	// Error handle parsing fail
	{
		FAIL:
		return {};
	}
}