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
