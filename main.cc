#include <cstddef>
#include <iostream>
#include <memory.h>
#include <memory>
#include <type_traits>

template<class t = int, class allocator = std::allocator<t>>
struct box{
    using type = t;
    using simple_stack = allocator;
    bool borrow = true;
    constexpr int BorrowType(){
        (std::is_floating_point<type>() ||
            std::is_arithmetic<type>() ||
            std::is_integral<type>()) ? borrow = true : borrow = false;
        return borrow;
    }
    struct iterator{
        type *obj;
        iterator(type *obj) : obj(obj){}

        type& operator*() const{
            return  *obj;
        }
        type* operator->() const{
            return obj;
        }
        iterator operator++(){
            ++obj;
            return *this;
        }
        bool operator==(const iterator &other_obj) const{
            return obj == other_obj.obj;
        }
        bool operator !=(const iterator &other_obj) const{
            return obj != other_obj.obj;
        }
    };
    type *heap;
    size_t size;
    size_t capacity;

    box() : heap(nullptr),
        size(0),
        capacity(0){}
    box(const box &other) : size(other.size),
        capacity(other.capacity){
        heap = simple_stack().allocate(capacity);
    }

    ~box(){
        simple_stack().deallocate(heap, capacity);
    }

    void push_back(const type &rvalue){
        if(size==capacity){
            size_t reallocate = capacity ? 2*capacity : 1;
            type *newObj =  simple_stack().allocate(reallocate);
            std::copy(heap, heap + size, newObj);
            simple_stack().deallocate(heap, capacity);
            heap = newObj;
            capacity = reallocate;
        }
        BorrowType() ? heap[size++] = rvalue :0;
    }
    void pop_back(){
        if(size > 0){
            --size;
        }
    }
    size_t size_() const{
        return size;
    }
    iterator begin(){
        return iterator(heap);
    }
    iterator end(){
        return iterator(heap + size);
    }
};

int main(){
    box<> box;
    box.push_back(1+1);
    box.push_back(2);
    for(const auto &r : box){
        std::cout << r;
    }
    return 0;
}

