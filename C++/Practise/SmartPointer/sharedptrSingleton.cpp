//shared_ptr manager Singleton

#include <iostream>
#include <memory>
#include <pthread.h>

class Singleton
{
public:
    static std::shared_ptr<Singleton> &getInstance() noexcept;

private:
    Singleton() = default;
    Singleton(const Singleton &other) = delete;
    Singleton(Singleton &&other) = delete;
    Singleton &operator=(const Singleton &other) = delete;
    ~Singleton() = default;

    static void releaseInstance(Singleton *st) noexcept;

    static std::shared_ptr<Singleton> managerPtr;
    static pthread_mutex_t lock;
};

std::shared_ptr<Singleton> Singleton::managerPtr(nullptr);

pthread_mutex_t Singleton::lock;

std::shared_ptr<Singleton> &Singleton::getInstance() noexcept
{
    if (managerPtr.get() == nullptr) {
        pthread_mutex_lock(&lock);
        if (managerPtr.get() == nullptr) {
            //constructor is private
            //cannot use make_shared, have to use shared_ptr
            managerPtr = std::shared_ptr<Singleton>(new Singleton(), releaseInstance);
        }
        pthread_mutex_unlock(&lock);
    }

    return managerPtr;
}

void Singleton::releaseInstance(Singleton *st) noexcept
{
    std::cout << "ReleaseInstance" << std::endl;
    if (managerPtr.get() != nullptr) {
        pthread_mutex_lock(&lock);
        if (managerPtr.get() != nullptr) {
            delete st;
            managerPtr = nullptr;
        }
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    std::shared_ptr<Singleton> sp1 = Singleton::getInstance();
    std::cout << sp1.get() << std::endl;
    std::cout << sp1.use_count()  << std::endl;
    std::shared_ptr<Singleton> sp2 = sp1;
    std::cout << sp2.get() << std::endl;
    std::cout << sp2.use_count()  << std::endl;
    return 0;
}
