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