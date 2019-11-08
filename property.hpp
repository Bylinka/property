#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

template<typename U>  concept AddEq = requires(U a) {{ a += a } -> U;};
template<typename U>  concept SubEq = requires(U a) {{ a -= a } -> U;};
template<typename U>  concept MulEq = requires(U a) {{ a *= a } -> U;};
template<typename U>  concept DivEq = requires(U a) {{ a /= a } -> U;};
template<typename U>  concept ModEq = requires(U a) {{ a %= a } -> U;};
template<typename U>  concept OrEq = requires(U a) {{ a |= a } -> U;};
template<typename U>  concept AndEq = requires(U a) {{ a &= a } -> U;};
template<typename U>  concept XorEq = requires(U a) {{ a ^= a } -> U;};
template<typename U>  concept ShrEq = requires(U a) {{ a >>= a } -> U;};
template<typename U>  concept ShlEq = requires(U a) {{ a <<= a } -> U;};

template <typename T>
class property
{
    std::shared_ptr<T> value;
    void (*getter)(T &);
    void (*setter)(T &);
    inline property(const T &ref,bool, void (*getter)(T &) = nullptr, void (*setter)(T &) = nullptr)
        : value(std::shared_ptr<T>((T*)&ref,[](T* value){}))
        , getter(getter)
        , setter(setter)
    {
    }

public:
    inline void set(const T& value){
        *this->value = value;
        if(setter)setter(*this->value);
    }
    inline void set(T&& value){
        *this->value = value;
        if(setter)setter(*this->value);
    }
    inline T& get(){
        if(getter)getter(*this->value);
        return *this->value;
    }
    inline T& get() const{
        if(getter)getter(*this->value);
        return *this->value;
    }
    inline static property<T> reference(const T& value, void (*getter)(T &) = nullptr, void (*setter)(T &) = nullptr){
        return property(value,true,getter,setter);
    }

    inline property(const T &ref, void (*getter)(T &) = nullptr, void (*setter)(T &) = nullptr)
        : value(std::make_shared<T>(ref))
        , getter(getter)
        , setter(setter)
    {
    }
    inline property(T&& value,  void (*getter)(T &) = nullptr, void (*setter)(T &) = nullptr)
        : value(std::make_shared<T>(value))
        , getter(getter)
        , setter(setter)
    {
    }    
    inline property(property & prop)
        : value(prop.value)
        , getter(prop.getter)
        , setter(prop.setter)
    {
    }
    inline property(const property & prop)
        : value(prop.value)
        , getter(prop.getter)
        , setter(prop.setter)
    {
    }
    inline property(property && prop)
        : value(prop.value)
        , getter(prop.getter)
        , setter(prop.setter)
    {
    }
    inline property& operator=(property && prop)
    {
        if(prop.getter)prop.getter(*prop.value);
        set(*prop.value);
    }
    inline property& operator=(const property & prop)
    {
        if(prop.getter)prop.getter(*prop.value);
        set(*prop.value);
    }
    inline property& operator=(const T &value){
        set(value);
        return *this;
    }
    inline property& operator=(T &&value){
        set(value);
        return *this;
    }
    inline operator T&(){
        return get();
    }
    inline friend std::ostream &operator<<(std::ostream &os, property<T> &value)
    {
        return os << value.get();
    }
    inline friend std::ostream &operator<<(std::ostream &os,const property<T> &value)
    {
        return os << value.get();
    }
    inline T* operator ->(){
        return &get();
    }


    template<AddEq U>
    inline property<U>& operator += (const U& value){
        return operator=(value+*this);
    }
    template<SubEq U>
    inline property<U>& operator -= (const U& value){
        return operator=(value-*this);
    }    
    template<MulEq U>
    inline property<U>& operator *= (const U& value){
        return operator=(value* *this);
    }
    template<DivEq U>
    inline property<U>& operator /= (const U& value){
        return operator=(value/ *this);
    }    
    template<ModEq U>
    inline property<U>& operator %= (const U& value){
        return operator=(value%*this);
    }
    template<OrEq U>
    inline property<U>& operator |= (const U& value){
        return operator=(value|*this);
    }    
    template<AndEq U>
    inline property<U>& operator &= (const U& value){
        return operator=(value+*this);
    }
    template<XorEq U>
    inline property<U>& operator ^= (const U& value){
        return operator=(value^*this);
    }    
  
    template<ShrEq U>
    inline property<U>& operator >>= (const U& value){
        return operator=(value>>*this);
    }
    template<ShlEq U>
    inline property<U>& operator <<= (const U& value){
        return operator=(value<<*this);
    }    
  
};

