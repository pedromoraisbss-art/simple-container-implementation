#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>

struct node{

};

template<typename dynamic_type, typename Allocator = std::allocator<dynamic_type>>
struct container :  node{
    using type = dynamic_type;
    using alloc_type = Allocator;
    using ptr_obj = dynamic_type *;
    using ref_obj = dynamic_type &;

    type *data_;
    std::size_t size_;
    std::size_t capacity_;

    struct iterator{
        using iterator_category_acess = std::random_access_iterator_tag;
        using diference = std::ptrdiff_t;
        ptr_obj ptr_;
        iterator(ptr_obj ptr) : ptr_(ptr){

        }
        ref_obj operator*() const{
            return  *ptr_;
        }
        ptr_obj operator->() const{
            return ptr_;
        }
        iterator operator++(){
            ++ptr_;
            return *this;
        }
        bool operator==(const iterator &other_obj) const{
            return ptr_ == other_obj.ptr_;
        }
        bool operator !=(const iterator &other_obj) const{
            return ptr_ != other_obj.ptr_;
        }
    };

    container() : data_(nullptr), size_(0), capacity_(0) {
        static_assert(!std::is_void<dynamic_type>());
    }
    container(const container &other): size_(other.size_), capacity_(other.capacity_){
        data_ = alloc_type().allocate(capacity_);
        if(alloc_type().address(data_) ==  nullptr){std::cerr << "nullprt\n";}
        std::copy(other.data_, other.data_ + size_, data_);
    }
    ~container(){
        alloc_type().deallocate(data_, capacity_);
        std::cout << "dealloc container";
    }

    void push_back(const dynamic_type &value){
        if(size_ == capacity_){
            size_t new_alloc = capacity_  ? 2 * capacity_ : 1;
            dynamic_type *new_data = alloc_type().allocate(new_alloc);
            std::copy(data_, data_ + size_, new_data);
            alloc_type().deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_alloc;
        }
        data_[size_++] = value;
    }
    size_t size() const{
        return size_;
    }
    iterator begin(){
        return iterator(data_);
    }
    iterator end(){
        return iterator(data_ + size_);
    }

};

int main(){
    container<int> NewContainer;
    NewContainer.push_back(1);
    NewContainer.push_back(2);
    NewContainer.push_back(3);
    NewContainer.push_back(4);
    for(const auto i : NewContainer){
        std::cout << i << '\n';
    }
    return 0;
}
