# CODING STYLE - Amélie Heinrich

Classes, member, functions and variables:
```cpp
class CamelCase
{
public:
    // Constructor and destructors first unless special case
    CamelCase();
    ~CamelCase();

    // Functions are in camel case, parameters too with the first letter in lower case
    void Function(int parameter);

private:
    int mMember; // Put a m before a class member
    static int sStaticMember; // Put a s before a static member
};

CamelCase test; // Variables in lower camel case
test.Function(1);
```

## Guidelines

- Keep the code clean and organised

## Commit guidelines

"[FIX/ADD/REMOVE] Commit message"

## Types

Make sure to use the given types provided by [Core/Common.hpp](Engine/Mnemen/Core/Common.hpp)
