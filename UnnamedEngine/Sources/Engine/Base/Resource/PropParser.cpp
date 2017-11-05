#include "PropParser.h"

#include <sstream>

enum class DSMState
{
	NULLSTATE,
	START,
	UNPAIRABLESYM, //T
	PAIRABLESYM, //T
	WHITESPACE, //T
	ALPHANUM, //NT
	RELATIVEPATH, //NT
	END // T
};

void PropParser::Tokenize(std::string str, std::vector<UDFToken>& tokens) const
{
	/*
	 * Parse the input string using a DSM
	 * This is easy since the only thing with more than one char is an alphanumeric.
	 */
	DSMState state = DSMState::START;

	std::stringstream tokenVal;

	for(size_t i = 0; i < (str.length() + 1); i++)
	{
		DSMState newState = DSMState::NULLSTATE;

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

		if(c == '/' || c == '\\' || c == '.')
		{
			newState = DSMState::RELATIVEPATH;
		}
		if((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
		{
			if(state == DSMState::RELATIVEPATH)
			{
				newState = DSMState::RELATIVEPATH;
			}
			else
			{
				newState = DSMState::ALPHANUM;
			}
		}
		else if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\x0b') {
			newState = DSMState::WHITESPACE;
		}
		else if(c == '(' || c == ')' || c == '{' || c == '}')
		{
			newState = DSMState::PAIRABLESYM;
		}
		else if(c == ':' || c == '=')
		{
			newState == DSMState::UNPAIRABLESYM;
		}
		else if(c == '$')
		{
			newState == DSMState::END;
		}

		//TODO:  This is a bit harsh, isn't it?
		assert(!(state == DSMState::UNPAIRABLESYM) && newState == DSMState::UNPAIRABLESYM);
		
		if(state == newState && !(state == DSMState::ALPHANUM && newState == DSMState::RELATIVEPATH))
		{
			tokenVal << c;
		}
		else
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
			else if(state == DSMState::WHITESPACE)
			{
				// Immediately disard the token for whitespace
				state = newState;
				tokenVal.clear();
				continue;
			}
			else if(state == DSMState::UNPAIRABLESYM || state == DSMState::PAIRABLESYM)
			{
				//handle this later
				tokenType = TokenType::NONE; 
			}

			tokens.push_back(UDFToken(TokenType::NONE, tokenVal.str()));
			tokenVal.clear();
		}
	}

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