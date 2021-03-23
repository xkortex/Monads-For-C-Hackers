//
// Created by Michael McDermott on 3/23/21.
//

#ifndef MONADS_FOR_C_HACKERS_MONAD_H
#define MONADS_FOR_C_HACKERS_MONAD_H

typedef enum dataType {
    Unknown = 0,
    Int,
    Float,
    /// etc
} DataType;

typedef enum monadType {
    Option,
    Result,
    /// etc
} MonadType;

struct Monad;

/// using a more modern syntax to annotate types since C type's system is a bit daunting to some

/// FmapFunc(d: void*) -> void*; a function that can be to a value. This is a placeholder, since we want an actual input/output type
typedef   void* (*FmapFunc)(void *d , DataType dtype );

/// Bindable(d: void*) -> Monad; a function that takes a value of type T and returns a Monad[T]
typedef   struct monad (*Bindable)(void *d , DataType dtype );

/// Fmap(f: FmapFunc) -> Monad; apply a function to the contents of a Functor/Monad
typedef   struct Monad (*Fmap)(FmapFunc f);


typedef struct monad {
    /// the raw data we want to wrap
    void *data;
    /// the type of the data we are wrapping
    DataType dtype;
    /// function pointer: lift(data, dtype) creates a monad from plain data
    struct monad (*lift)(void *d, DataType dtype);

    /// function pointer: bind chains a function
    struct monad (*bind)(Bindable);

} Monad;

typedef Monad* MonadPtr;

#endif //MONADS_FOR_C_HACKERS_MONAD_H
