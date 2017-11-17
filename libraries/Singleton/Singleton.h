#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
    public:
        static T& getInstance()
        {
            static T instance;
            return instance;
        }
    protected:
        Singleton() {}
        ~Singleton() {}
    public:
        Singleton(Singleton const &) = delete;
        Singleton& operator=(Singleton const &) = delete;
};

#endif
