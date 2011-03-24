#ifndef HELPERS_H
#define HELPERS_H

#include <typeinfo>
#include <QDebug>

#ifdef ENABLE_DEBUG
#define LOG(a)      qDebug() << a ;
#define ENTER(a)    qDebug() << "Enter:  " << typeid(*this).name() << "::" << __FUNCTION__ << a ;
#define EXIT(a)     qDebug() << "Exit:   " << typeid(*this).name() << "::" << __FUNCTION__ << a ;
#define RETURN(a,b) { \
    a tmp = b; \
    qDebug() << "Return: " << typeid(*this).name() << "::" << __FUNCTION__ << ": " << b; \
    return tmp; \
}
#else
#define LOG(a)
#define ENTER(a)
#define EXIT(a)
#define RETURN(a,b) return (b);
#endif



#ifdef ENABLE_HEAPINFO
#ifdef Q_OS_SYMBIAN
#define HEAPINFO(a) { TInt size; TInt mem; TInt limit; \
    size = User::Heap().Size(); \
    User::Heap().AllocSize(mem); \
    limit = User::Heap().MaxLength(); \
    qDebug() << typeid(*this).name() << "::" << __FUNCTION__ << a << "Heapsize:" << size << "Heap allocated:" << mem << "Heap max:" << limit; \
}
#else
#define HEAPINFO(a)
#endif
#else
#define HEAPINFO(a)
#endif


#endif // HELPERS_H
