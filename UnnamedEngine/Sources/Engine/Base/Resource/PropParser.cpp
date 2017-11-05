#include "PropParser.h"

#include <sstream>

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

void PropParser::Tokenize(std::string str, std::vector<UDFToken>& tokens)
{
	/*
	 * Parse the input string using a DSM
	 * This is easy since the only thing with more than one char is an alphanumeric.
	 */
	DSMState state = DSMState::START;

	std::stringstream tokenVal;

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
			continue;
		}
		else if(c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == '=')
		{
			shouldTerminate = true;
			nextState = DSMState::Sym;
		}
		else if(c == '$')
		{
			shouldTerminate = true;
			nextState == DSMState::END;
		}

		if(shouldTerminate)
		{
			// Generate a new token everytime the DSM state switches
			TokenType tokenType = TokenType::NONE;

			if(state == DSMState::ALPHANUM)
			{
				tokenType = ALPHANUM;
			}
			else if(state == DSMState::RELATIVEPATH)
			{
				tokenType = FILEPATH;
			}
			else if(state == DSMState::Sym)
			{
				//handle this later
				tokenType = TokenType::NONE; 
			}

			tokens.push_back(UDFToken(tokenType, tokenVal.str()));
			
			tokenVal.clear();
			tokenVal.str(std::string());
		}
		state = nextState;
		tokenVal << c;
	}

	// Replace SYMs with the proper tokens
	for(UDFToken& t : tokens)
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
			else if (t.value == "}")
			{
				t.type = TokenType::RCURLY;
			}
			else if (t.value == ":")
			{
				t.type = TokenType::COLON;
			}
			else if (t.value == "=")
			{
				t.type = TokenType::EQUALS;
			}
			else
			{
				//Why did something of TokenType::None even get past the regular parsing?
				assert(false);
			}
		}
	}
}

void PropParser::ParserFailed()
{
	assert(false);
}