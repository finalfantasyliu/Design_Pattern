#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

using namespace std;
enum class Color {
    red,
    green,
    blue
};

enum class Size {
    small,
    medium,
    large
};

struct Product {
    string name;
    Color color;
    Size size;
};

/* 如果每次要新增一個product屬性就要新增一個filter，
 * 雖然做到了product 與 filter的 separation of concern
 * 但單就ProductFilter其實其也違反了single responsibility principle，
 * 因為每次只要新增一個過濾的property就需要增加他的職責，並且若以open-close principle來看
 * 此prouductFilter會需要一直不斷的更改其內容，違反了closed (to avoid modification that affect clients).
 *
 * */
struct ProductFilter {
    typedef vector<Product *> Items;

    Items by_color(Items items, const Color color) {
        Items result;
        for (auto &i: items) {
            if (i->color == color)
                result.push_back(i);
        }
        return result;
    }

    Items by_size(Items items, const Size size) {
        Items result;
        for (auto &s: items) {
            if (s->size == size)
                result.push_back(s);
        }
        return result;
    }

    Items by_size_and_color(Items items, const Color color, const Size size) {
        Items result;
        for (auto &c: items) {
            if (c->color == color && c->size == size)
                result.push_back(c);
        }
        return result;
    }
};

/* 為了維持open-closed原則，須將參數和filter抽離，
 * 並利用inheritance將實作所需要的參數與filter，增加其擴充性，
 * 之後若需要使此過濾的方法，只要特別實作spec與filter即可使用，
 * 可創造多種filter適應不同的spec。
 * 這不得不提到由Vasiliy Zukanov 對於open-close的解釋
 * Depend on stable abstractions and modify system’s behavior by providing different realizations
 * filter 和 spec是穩定的abstraction
 * 你override的 filter與spec 只是提供不同的理解方式，並沒有更改原始的spec與filter。
 * 以下的class均遵循此現象
 * Betterfilter 實作filter針對product，但沒有更改filter;
 * ColorSpec實作spec 但沒有更改spec
 * SizeSpec實作spec，但沒有更改spec
 * */
template<typename T>
struct Specification {
    virtual bool is_satisfied(T *item) const = 0;
};

template<typename T>
struct AndSpecification : Specification<T> {
    const Specification<T> &first;
    const Specification<T> &second;

    AndSpecification(const Specification<T> &first, const Specification<T> &second) : first(first), second(second) {};

    bool is_satisfied(T *item) const override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }


};

template<typename T>
struct Filter {
    virtual vector<T *> filter(vector<T *> items, Specification<T> &spec) = 0;
};

struct BetterFilter : Filter<Product> {
    vector<Product *> filter(vector<Product *> items, Specification<Product> &spec) override {
        vector<Product *> result;
        for (auto &p: items) {
            if (spec.is_satisfied(p)) {
                result.push_back(p);
            }
        }
        return result;
    }
};
// 還可以使用一個工廠模式取產生相對應的product spec
template<typename T>
struct ProductSpecFactory:Specification<Product>
{
   T property;

    explicit ProductSpecFactory(T Property) : property(Property) {}
    bool is_satisfied(Product*item)const override{
        if(typeid(property)==typeid(Color))
            return item->color==(Color)property;
        else if(typeid(property)==typeid(Size))
            return item->size==(Size)property;

    }
};


struct ColorSpec : Specification<Product> {
    Color color;

    explicit ColorSpec(Color color) : color(color) {};

    bool is_satisfied(Product *item) const override {
        return item->color == color;
    }
};

struct SizeSpec : Specification<Product> {
    Size size;

    explicit SizeSpec(Size size) : size(size) {};

    bool is_satisfied(Product *item) const override {
        return item->size == size;
    }
};


int main() {
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};
    const vector<Product *> all{&apple, &tree, &house};

    BetterFilter bf;
    ColorSpec green(Color::green);
    SizeSpec large(Size::large);
    auto greenThings = bf.filter(all, green);
    for (auto &t: greenThings) {
        cout << t->name << " ";
    }
    cout << endl;
    AndSpecification<Product> greenAndLarge(green, large);
    auto greenAndLargeThings = bf.filter(all, greenAndLarge);
    for (auto &t: greenAndLargeThings) {
        cout << t->name << " ";
    }
    cout<<endl;
    ProductSpecFactory<Color>productSpecFactory1(Color::green);
    ProductSpecFactory<Size>productSpecFactory2(Size::small);
    auto greenThings2=bf.filter(all,productSpecFactory2);
    for (auto &t: greenThings2) {
        cout << t->name << " ";
    }
    cout << endl;
    return 0;
}
