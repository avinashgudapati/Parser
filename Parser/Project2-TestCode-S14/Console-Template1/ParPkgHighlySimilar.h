
struct element
{
	std::string type;
	std::string name;
	size_t lineCount;
	size_t ScopeStartLine;
	size_t ScopeEndLine;
	size_t ScopeComplexity;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ")";
		return temp.str();
	}
};

struct storeScopes
{
	std::vector<std::vector<element>> mainVector;
};
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
	ScopeStack<element> stack;
	Toker* p_Toker;
public:
	Repository(Toker* pToker)
	{
		p_Toker = pToker;
	}
	ScopeStack<element>& scopeStack()
	{
		return stack;
	}
	Toker* Toker()
	{
		return p_Toker;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->lines());
	}
	void setIsInFunction(bool value){
		isInFunction = value;
	}
	bool getIsInFunction(){
		return isInFunction;
	}
	

	void pushFileScopesVector(element value){
		FileScopesVector.push_back(value);
	}
	std::vector<element>& getFileScopesVector(){
		return FileScopesVector;
	}

	void pushmainVector(std::vector<element> value){
		mainVector.push_back(value);
	}
	std::vector<std::vector<element>> getmainVector(){
		return mainVector;
	}
private:
	bool isInFunction = false;
	std::vector<element> FileScopesVector;
	std::vector<std::vector<element>> mainVector;
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

namespace Test1 {
  namespace Test2 {
    class Bracefull
    {
    public:
      Bracefull() { std::cout << "\n  constructing Bracefull"; }
      ~Bracefull() {}
      void inlineTest(size_t count=5)
      {	
        for (size_t i = 0; i < count; ++i)
        {
          if (true)
          {
            std::cout << "\n  inlineTest function";
            if (false)
            {
              std::cout << "\n  won't get here";
            }
          }
          else {
            std::cout << "\n  won't get here";
          }
          std::cout << "\n  ";
          std::vector<std::string> stuff{ "first", "second", "third" };
          for (auto item : stuff)
          {
            std::cout << item << " ";
          }
        }
      }
      void non_inlineTest();

      class nestedClass {
      public:
        nestedClass() { std::cout << "\n  constructing nestedClass"; }
        void say() { std::cout << "\n  nested class say()"; }
      };
    };
  }
  namespace Test3 {
    class Braceless {
    public:
      Braceless() {
        std::cout << "\n  constructing Braceless";
      }
      ~Braceless() {}
      void inlineTest(size_t count = 5)
      {
        for (size_t i = 0; i < count; ++i)
          if (true)
            std::cout << "\n  inlineTest function";
        if (false)
          std::cout << "\n  won't get here";
        else
          std::cout << "\n  will get here";

        std::cout << "\n  ";
        std::vector<std::string> stuff{ "first", "second", "third" };
        for (auto item : stuff)
          std::cout << item << " ";
      }
      void non_inlineTest();
    };
  }
}