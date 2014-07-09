#ifndef  SINGLETONHOLDER_INC
#define  SINGLETONHOLDER_INC

template<class T>
class SingletonHolder
{
public:
    static T* Instance()
    {
        if(!pInstance_)
            pInstance_=new T;
        return pInstance_;
    }
private:
    SingletonHolder(){};
    SingletonHolder(const SingletonHolder&);
    SingletonHolder& operator=(const SingletonHolder&);
    ~SingletonHolder(){};
    static T* pInstance_;
};

template<class T>
T* SingletonHolder<T>::pInstance_=0;


#endif  