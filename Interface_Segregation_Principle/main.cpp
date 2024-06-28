#include <iostream>
/*interface Segregation Principle
 * 其此原則主要就是避免把多個實作的方法集中在同一個interface
 * 因為若一個class只想使用到某一方法，其不得不override所有方法，
 * 而若使其為空，反而會混淆使用者，例如明明有print方法，但不能print*/
/*struct Document;
struct IMachine
{
    virtual void print(Document &doc)=0;
    virtual void scan(Document &doc)=0;
    virtual void fax(Document &doc)=0;
};
struct MFP:IMachine{
    void print(Document &doc) override {

    }

    void scan(Document &doc) override {

    }

    void fax(Document &doc) override {

    }
};
//此時Printer就不得不實作所有function，但printer可能只能print
struct Printer:IMachine
{
    void print(Document &doc) override {

    }

    void scan(Document &doc) override {

    }

    void fax(Document &doc) override {

    }
};*/

/* 當實現了介面分離，任何class只需要繼承所需的interface並實作相應的方法
 * 若要還有兩項interface則可以繼承兩項interface即可
 * */
/* 在此再次補充一下IPrinter &printer 與IPrinter printer的差異
 *
 * Using IPrinter printer (without reference):
 * When you declare IPrinter printer without the reference (&),
 * you're actually creating a member variable printer of type IPrinter.
 * This means that printer will hold an actual object of type IPrinter. In this case,
 * you need to initialize it with an instance of a concrete class that implements the IPrinter interface.
 *
 * Using IPrinter &printer (with reference):
 * When you declare IPrinter &printer with the reference (&),
 * you're creating a member reference printer of type IPrinter.
 * This means that printer will reference (or point to) an existing object that implements the IPrinter interface.
 * In this case,you don't create a new object but rather refer to an object that is provided externally,
 * typically during construction.
 *
 * Polymorphism:
 * By using a reference to an interface,
 * you can work with any object that implements that interface.
 * This allows you to use polymorphism to swap different implementations of the interface at runtime.
 * For example, you can have different types of printers (e.g., LaserPrinter, InkjetPrinter) that all implement the IPrinter interface,
 * and you can pass any of them to the Machine class.
 *
 * Dependency Injection:
 * Using a reference allows for dependency injection,
 * where the concrete implementation of the interface is provided externally,
 * typically through the constructor. T
 * this promotes loose coupling and makes the code more flexible and easier to test.
 *
 * Avoiding Object Slicing:
 * If you were to use a non-reference member like IPrinter printer,
 * you would need to store an actual object of a concrete class that implements the IPrinter interface.
 * This could lead to object slicing if you try to store a derived class object in a base class object.
 * Using a reference avoids this issue because it refers directly to the derived class object.
 *
 * Efficiency:
 * References are typically more efficient than storing objects directly,
 * especially when dealing with large objects.
 * They avoid the overhead of object copying and ensure that changes made to the referred object are reflected immediately.
 *
 *
 *
 *
 * */

struct Document;
struct IPrinter
{
    virtual void print(Document &doc);
};
struct IScanner
{
    virtual void scan(Document &doc);
};

struct printer:IPrinter
{
    void print(Document &doc) override {
        IPrinter::print(doc);
    }
};
struct scanner:IScanner
{
    void scan(Document &doc) override {
        IScanner::scan(doc);
    }
};
struct IMachine:IScanner,IPrinter{};

struct Machine:IMachine
{
    IPrinter &printer;
    IScanner &scanner;
    void print(Document &doc) override {
      printer.print(doc);
    }

    void scan(Document &doc) override {
        scanner.scan(doc);
    }
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
