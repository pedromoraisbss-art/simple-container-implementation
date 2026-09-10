#include <cstddef>
#include <iostream>
#include <memory.h>
#include <memory>
#include <type_traits>

template<class t, class allocator = std::allocator<t>>
struct continuos_container{
    using type = t;
    using simple_stack = allocator;
    bool borrow = true;
    constexpr int BorrowType(){
        if(!std::is_integral<type>()){
            borrow = false;
            std::cout << "err";

        }
        return borrow;
    }
    struct iterator{
        type *dyn;
        iterator(type *dyn_) : dyn(dyn_){}

        type& operator*() const{
            return  *dyn;
        }
        type* operator->() const{
            return dyn;
        }
        iterator operator++(){
            ++dyn;
            return *this;
        }
        bool operator==(const iterator &other_obj) const{
            return dyn == other_obj.dyn;
        }
        bool operator !=(const iterator &other_obj) const{
            return dyn != other_obj.dyn;
        }
    };
    type *heap;
    size_t size;
    size_t capacity;

    continuos_container() : heap(nullptr),
        size(0),
        capacity(0){}
    continuos_container(const continuos_container &other) : size(other.size),
        capacity(other.capacity){
        heap = simple_stack().allocate(capacity);
    }

    ~continuos_container(){
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
        if(BorrowType())
            heap[size++] = rvalue;
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
    continuos_container<int> box;
    box.push_back(1);
    box.push_back(2);
    for(const auto &r : box){
        std::cout << r;
    }
    return 0;
}
