
class count
{
public:
    count():_count(1){}
    void add_count()
    {
        ++_count;
    }
    void reduce_count()
    {
        --_count;
    }
    long get_count()
    {
        return _count;
    }
private:
    long _count;
};
//作为shared_ptr的计数类。每个share_ptr对象绑定一个计数类，从而达到指针共享的目的
template <typename T>
class share_ptr
{
    friend class share_ptr;
private:
    /* data */
    T * _ptr;
    count * _count;
public:
    /*构造函数*/
    explicit share_ptr(T* ptr=nullptr):_ptr(ptr)
    {
        if(ptr)
        {
            _count=new count;
        }
    };
    /*类型转换*/
    share_ptr(const share_ptr<T>& other,T* ptr)
    {
        _ptr=ptr;
        if(_ptr)
        {
            other._count->add_count();
            _count=other._count;
        }
    }
    /*赋值函数*/
    share_ptr & operator=(share_ptr rhs)
    {
        rhs.swap(*this);
        return *this;
    }
    /*拷贝构造函数*/
    share_ptr(const share_ptr&other)
    {
        _ptr=other._ptr;
        if(_ptr)
        {
            other._count->add_count();
            _count=other._count;
        }
    }
    /*上一个函数应该错了。传入值不会是一个未确定类型的对象*/
    share_ptr(const share_ptr<T>&other)
    {
        _ptr=other._ptr;
        if(_ptr)
        {
            other._count->add_count();
            _count=other._count;
        }
    }
    /*移动构造函数，使用移动构造函数，减少临时对象的无效利用*/
    share_ptr(share_ptr<T> &&other)
    {
        _ptr=other._ptr;
        if(_ptr)
        {
            _count=other._count;
            other._ptr=nullptr;
        }
    }
    /*析构函数*/
    ~share_ptr()
    {
        if(_ptr&&_count->reduce_count())
        {
            delete _ptr;
            delete _count;
        }
    }
    /*交换函数，一些辅助函数*/
    void swap(share_ptr& rhs)
    {
        using std::swap;
        swap(_ptr,rhs._ptr);
        swap(_count,rhs._count);
    }
    long use_count()
    {
        if(_ptr)
        {
            return _count->get_count();
        }
        else
        {
            return 0;
        }
    }
    /*
    重载运算符
    */
   T& operator*() const{return *_ptr;}
   T* operator->() const{return _ptr;}
   T* get()
   {
    return _ptr;
   }
};
