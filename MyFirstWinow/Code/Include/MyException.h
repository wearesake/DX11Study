#pragma once
#include <exception>
#include <string>
//窗口异常基类：继承std::exception
class MyException : public std::exception{
public:
    MyException(int line, const char* file) noexcept;
    //重写what()
    //在 const 成员函数里，编译器禁止修改普通成员变量。
    const char* what() const noexcept override;
    //虚函数-获得当前异常类型
    virtual const char* GetType() const noexcept;
    int GetLine() const noexcept;
    const std::string& GetFile() const noexcept;
    std::string GetOriginString() const noexcept;

private:
    int line;
    std::string file;
    
protected:
    //whatBuffer 不是私有，而是 受保护的成员，
    //这样做是为了让 子类异常（比如你 Graphics::HrException）也能直接用它来存储 what() 的字符串。
    //避免每个子类都重复定义一个 std::string 缓冲区。
    mutable std::string whatBuffer; //mutable可变的，由于C++ 标准要求 std::exception::what() 必须是 const。而我们又希望字符串可以通过动态拼接改变。
};

