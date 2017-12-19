#include "PropParser.h"

#include <sstream>
#include <iostream>
#include <stack>

#include "Engine/Base/Resource/PropTree.h"

enum class DSMState
{
	NULLSTATE,
	START,
	Sym, //T
	ALPHANUM, //NT
	STRING,
	NUMBER,
	DECIMALNUMBER,
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

// An internal version of UDFToken that contains auxillery information
// such as a line number for printing parsing error reports.
struct UDFTokenInternal
{
	UDFTokenInternal(ParsedTokenType type, std::string value, int line) :
		type(type), value(value), line(line) {}
	ParsedTokenType type;
	std::string value;
	int line;
};

PropParser::PropParser()
{
}

std::optional<PropTree> PropParser::Parse(std::string str)
{
	std::vector<UDFToken> tokens;
	Tokenize(str, tokens);
	return(ParseTokens(tokens));
}


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

		// If this flag is set to true, it will use all input since last terminal token
		// to form the lexeme for the token
		bool shouldTerminate = false;
		bool discardCharacter = false;
		
		if(c == '"')
		{
			if(state == DSMState::STRING)
			{
				nextState = DSMState::START;
				shouldTerminate = true;
				discardCharacter = true;
			}
			else
			{
				nextState = DSMState::STRING;
				shouldTerminate = true;
				discardCharacter = true;
			}
		}
		else if(state == DSMState::STRING)
		{
			// There is a edge from String->String for every character
			nextState = DSMState::STRING;
		}
		else if(c >= '0' && c <= '9')
		{
			if(state == DSMState::ALPHANUM)
			{
				nextState = DSMState::ALPHANUM;
			}
			else if(state == DSMState::NUMBER)
			{
				nextState = DSMState::NUMBER;
			}
			else if(state == DSMState::DECIMALNUMBER)
			{
				nextState = DSMState::DECIMALNUMBER;
			}
			else
			{
				nextState = DSMState::NUMBER;
				shouldTerminate = true;
			}
			
		}
		else if(c == '.')
		{
			if(state == DSMState::RELATIVEPATH || state == DSMState::START)
			{
				nextState = DSMState::RELATIVEPATH;
			}
			else if (state == DSMState::NUMBER)
			{
				nextState = DSMState::DECIMALNUMBER;
			}
			else
			{
				shouldTerminate = true;
				nextState =  DSMState::RELATIVEPATH;
			}
		}
		else if(c == '/' || c == '\\' )
		{
			if
			(
				state == DSMState::RELATIVEPATH ||
				state == DSMState::START
			)
			{
				nextState = DSMState::RELATIVEPATH;
			}
			else
			{
				nextState = DSMState::RELATIVEPATH;
				shouldTerminate = true;
			}
		}
		else if
		(
			(
				(c >= 'A' && c <= 'Z') || 
				(c >= 'a' && c <= 'z')
			)  || 
			c == '_'
		)
		{
			if(state == DSMState::RELATIVEPATH)
			{
				nextState = DSMState::RELATIVEPATH;
			}
			else if(state == DSMState::NUMBER || state == DSMState::ALPHANUM || state == DSMState::START)
			{
				nextState = DSMState::ALPHANUM;
			}
			else
			{
				shouldTerminate = true;
				nextState = DSMState::ALPHANUM;
			}
		}
		// Ignore whitespace
		else if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\x0b') {
			if(c == '\n')
			{
				lnCount++;
			}
			continue;
		}
		else if(c == '(' || c == ')' || c== '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == '=' || c == ';' || c== ',')
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
			ParsedTokenType ParsedTokenType = ParsedTokenType::NONE;

			if(state == DSMState::ALPHANUM)
			{
				ParsedTokenType = ParsedTokenType::ALPHANUM;
			}
			else if(state == DSMState::NUMBER)
			{
				ParsedTokenType = ParsedTokenType::NUMBER;
			}
			else if(state == DSMState::DECIMALNUMBER)
			{
				ParsedTokenType = ParsedTokenType::DECIMALNUMBER;
			}
			else if(state == DSMState::STRING)
			{
				ParsedTokenType = ParsedTokenType::STRING;
			}
			else if(state == DSMState::RELATIVEPATH)
			{
				ParsedTokenType = ParsedTokenType::FILEPATH;
			}
			else if(state == DSMState::Sym)
			{
				//handle this later
				ParsedTokenType = ParsedTokenType::NONE; 
			}

			// Strings are currently the only thing that can discard tokens during parsing
			if(tokenVal.str().size() > 0 || ParsedTokenType == ParsedTokenType::STRING)
			{
				internalTokens.push_back(UDFTokenInternal(ParsedTokenType, tokenVal.str(), lnCount));
			}

			tokenVal.clear();
			tokenVal.str(std::string());
		}
		state = nextState;
		if(!discardCharacter)
		{
			tokenVal << c;
		}
	}

	// Replace SYMs with the proper tokens
	for(UDFTokenInternal& t : internalTokens)
	{
		if(t.type == ParsedTokenType::NONE)
		{
			if(t.value == "(")
			{
				t.type = ParsedTokenType::LBRACKET;
			}
			else if(t.value == ")")
			{
				t.type = ParsedTokenType::RBRACKET;
			}
			else if(t.value == "[")
			{
				t.type = ParsedTokenType::LSQUARE;
			}
			else if (t.value == "]")
			{
				t.type = ParsedTokenType::RSQUARE;
			}
			else if(t.value == "{")
			{
				t.type = ParsedTokenType::LCURLY;
			}
			else if(t.value == "}")
			{
				t.type = ParsedTokenType::RCURLY;
			}
			else if(t.value == ":")
			{
				t.type = ParsedTokenType::COLON;
			}
			else if (t.value == ",")
			{
				t.type = ParsedTokenType::COMMA;
			}
			else if(t.value == "=")
			{
				t.type = ParsedTokenType::EQUALS;
			}
			else if(t.value == ";")
			{
				t.type = ParsedTokenType::SEMI;
			}
			else
			{
				//Why did something of ParsedTokenType::None even get past the regular parsing?
				ParserFailed(t.line, t.value);
				failed = true;
				assert(false);
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

bool PropParser::ParseValueTokens(const PropTree& tree, const std::string& identifier, const std::vector<UDFToken>& valueTokens)
{
	assert(valueTokens.size() > 0);
	if(valueTokens[0].type == ParsedTokenType::STRING)
	{
		assert(valueTokens.size() == 0);
	}
	else if(valueTokens[0].type == ParsedTokenType::LSQUARE)
	{
		// Is a vector
	}
	else if(valueTokens[0].type == ParsedTokenType::NUMBER)
	{
		// Is an ints
	}
	else if (valueTokens[0].type == ParsedTokenType::DECIMALNUMBER)
	{
		// Is an ints
	}
	else if(valueTokens[0].type == ParsedTokenType::FILEPATH)
	{
		// Is a filepath
	}
	else
	{
		// Invalid value token.
		assert(false);
	}
	return(false);
}

std::optional<PropTree> PropParser::ParseTokens(std::vector<UDFToken>& tokens)
{
	/* This is (hopefully) simple enough we can get away with ghetto parsing (without a real L** parser)*/
	PropTree root{};

	// We keep track of our current block scope by
	// keeping a stack of the addresses of each 
	// nested PropTree.
	std::stack<PropTree *> propTreeStack;
	propTreeStack.push(&root);

	for(size_t i = 0; i < tokens.size(); i++)
	{
		if(tokens[i].type == ParsedTokenType::ALPHANUM)
		{
			if(tokens[i + 1].type == ParsedTokenType::EQUALS)
			{
				// Parse as leaf until ';'
				const std::string key = tokens[i].value;

				std::vector<UDFToken> valueTokens;
				
				// Parse the leaf
				for(i = i + 2; i < tokens.size(); i++)
				{
					if(tokens[i].type == ParsedTokenType::SEMI)
					{
						break;
					}

					// If we're still trying to parse a value and are at EOF
					// then we're royally screwed.
					if(i == tokens.size() - 1)
					{
						goto FAIL;
					}

					// todo: parse into the actual value once we figure out what exactly should be enumerated
					valueTokens.push_back(tokens[i]);
				}

				auto& topLeaves = propTreeStack.top();
				ParseValueTokens(*topLeaves, key, valueTokens);
			}
			else if(tokens[i+1].type == ParsedTokenType::LCURLY)
			{
				// Create a new subtree, using the value parsed before 
				// the { as a key.
				auto& topComponentMap = propTreeStack.top()->components;

				topComponentMap.emplace(tokens[i].value, PropTree{});

				// Put this new token at the top of the stack
				// The address is (surprisingly) conserved until we decide
				// to kill the unordered_map so taking he address here is legal
				propTreeStack.push(&topComponentMap[tokens[i].value]);

				// We consumed an additional token [i+1] here, so advance
				i++;
			}
		}
		else if(tokens[i].type == ParsedTokenType::RCURLY)
		{
			if(propTreeStack.empty())
			{
				goto FAIL; // this is sure to upset some people.
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