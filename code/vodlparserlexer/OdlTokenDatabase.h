#ifndef INCLUDED_ODL_TOKEN_DATABASE_H
# define INCLUDED_ODL_TOKEN_DATABASE_H

#include "OdlSingleton.h"

#include <hash_map>
#include <vector>
#include <string>

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
namespace TTokenDomain
{
    enum TType
    {
        MISC,
		ODL_DATABASE_NAME,
        __LENGTH__
    };

} // TTokenDatabaseDomain
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
template < int TTokenDomain >
class TTokenDatabase;
//-------------------------------------------------------------------------------
template < int TTokenDomain >
class TToken
{
typedef TTokenDatabase< TTokenDomain > TRelatedTokenDatabase;
friend class TRelatedTokenDatabase;

private:
    TToken(unsigned int parToken) :
        FToken(parToken)
    {
    }
public:
    TToken() :
        FToken(std::numeric_limits< unsigned int >::max())
    {
    }

	TToken(std::string const& parString)
	{
		*this = TTokenDatabase< TTokenDomain >::Instance().Tokenize(parString);
	}

	friend inline bool operator < (TToken const& parLeft, TToken const& parRight)
	{
		return parLeft.FToken < parRight.FToken;
	}

    friend inline bool operator > (TToken const& parLeft, TToken const& parRight)
	{
		return parLeft.FToken > parRight.FToken;
	}

    friend inline bool operator == (TToken const& parLeft, TToken const& parRight)
	{
		return parLeft.FToken == parRight.FToken;
	}

    friend inline bool operator != (TToken const& parLeft, TToken const& parRight)
	{
		return parLeft.FToken != parRight.FToken;
	}

	unsigned int ToInt() const { return FToken; }
	
	std::string const& ToString() const
	{
		return TRelatedTokenDatabase::Instance().ToString(*this);
	}

private:
    unsigned int FToken;
};
//-------------------------------------------------------------------------------
template < int TTokenDomain >
class TTokenDatabase : public TSingleton< TTokenDatabase< TTokenDomain > >
{
public:
    TTokenDatabase() :
        TSingleton< TTokenDatabase >(this)
    {

    }

    TToken< TTokenDomain > Tokenize(std::string const& parName)
    {
        size_t sizeBefore = FTokenGenerator.size();
        unsigned int& tokenValue = FTokenGenerator[parName];
        size_t sizeAfter = FTokenGenerator.size();
        if (sizeBefore < sizeAfter)
        {
            tokenValue = (unsigned int) sizeBefore;
            assert(FTokenValues.size() == sizeBefore);
            FTokenValues.push_back(parName);
        }

        return TToken< TTokenDomain >(tokenValue);
    }

	std::string const& ToString(TToken< TTokenDomain > const& parToken)
	{
		return FTokenValues[parToken.ToInt()];
	}

private:
    std::hash_map< std::string, unsigned int > FTokenGenerator;
    std::vector< std::string > FTokenValues;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
template < int TTokenDomain >
class TTokenPath
{
	typedef TToken< TTokenDomain > TTokenType;
public:

    TTokenPath()
    {

    }

    TTokenPath(char const* parPathAsString)
    {
        bool error = false;
        size_t stringSize = strlen(parPathAsString);
        size_t startToken = 0;
        for (size_t i = 0; i <= stringSize; ++i)
        {
            if (parPathAsString[i] == '/' || parPathAsString[i] == '\0')
            {
                // no empty token.
                if (startToken == i)
                {
                    error = true;
                    break ;
                }

                std::string tokenString(&parPathAsString[startToken], &parPathAsString[i]);
                TTokenType token(tokenString);
                push_back(token);

                startToken = i + 1;
            }
        }

        if (error)
        {
            FTokens.clear();
        }
    }

	TTokenType back() const 
	{ 
		assert(!FTokens.empty());
		return FTokens.back(); 
	}

	void push_back(TTokenType parToken)
	{
		FTokens.push_back(parToken);
	}

	void pop_back()
	{
		FTokens.pop_back();
	}

    size_t size() const { return FTokens.size(); }
    bool empty() const { return FTokens.empty(); }

	TTokenType operator[] (size_t parIndex) const { return FTokens[parIndex]; }

	std::string ToString() const
	{
		std::ostringstream oss;

		for (int i = 0; i < (int) FTokens.size(); ++i)
		{
			TTokenType const& token = FTokens[i];
			std::string tokenAsString = TTokenDatabase< TTokenDomain >::Instance().ToString(token);
			oss << tokenAsString;
			if (i + 1 < (int) FTokens.size())
				oss << '/';
		}
	
		return oss.str();
	}

	friend inline bool operator < (TTokenPath const& parLeft, TTokenPath const& parRight)
	{
		size_t const leftSize = parLeft.size();
		size_t const rightSize = parRight.size();
		size_t const minLength = std::min(leftSize, rightSize);

		for (size_t i = 0; i < minLength; ++i)
		{
			if (parLeft[i] < parRight[i])
				return true;
            else if (parLeft[i] > parRight[i])
                return false;
		}

        if (leftSize < rightSize)
            return true;

		return false;
	}

    friend inline bool operator > (TTokenPath const& parLeft, TTokenPath const& parRight)
	{
		size_t const leftSize = parLeft.size();
		size_t const rightSize = parRight.size();
		size_t const minLength = std::min(leftSize, rightSize);

		for (size_t i = 0; i < minLength; ++i)
		{
			if (parLeft[i] > parRight[i])
				return true;
            else if (parLeft[i] > parRight[i])
                return false;
		}

        if (leftSize > rightSize)
            return true;

		return false;
	}

    friend inline bool operator == (TTokenPath const& parLeft, TTokenPath const& parRight)
	{
		size_t const leftSize = parLeft.size();
		size_t const rightSize = parRight.size();

        if (leftSize != rightSize)
            return false;

		for (size_t i = 0; i < leftSize; ++i)
		{
			if (parLeft[i] != parRight[i])
				return false;
		}

		return true;
	}

protected:
    TTokenType& at(size_t parIndex) { return FTokens[parIndex]; }

private:
	std::vector< TTokenType > FTokens;
};
//-------------------------------------------------------------------------------
class TOdlDatabasePath : public TTokenPath< TTokenDomain::ODL_DATABASE_NAME >
{
    typedef TTokenPath< TTokenDomain::ODL_DATABASE_NAME > parent_type;
public:
    TOdlDatabasePath() : parent_type() {}
    TOdlDatabasePath(char const* parPathAsString) : parent_type(parPathAsString) {}

    bool SetCurrentNameInParentNamespace_ROK()
    {
        if (size() > 1)
        {
            at(size() - 2) = at(size() - 1);
            pop_back();
            return true;
        }

        return false;
    };
};
//-------------------------------------------------------------------------------
typedef TToken< TTokenDomain::ODL_DATABASE_NAME > TOdlDatabaseToken;
//-------------------------------------------------------------------------------
void CreateTokenDatabases();
void DestroyTokenDatabases();
//-------------------------------------------------------------------------------
} // namespace odl


#endif

