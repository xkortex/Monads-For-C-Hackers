# notes and brain dump

Original thought that spawned all of this: 
[Monads for Go Programmers](https://awalterschulze.github.io/blog/post/monads-for-goprogrammers/)

> A pointer: `*T` is a container that may be empty or contain one item;

and this: 
[What the Functor?](https://www.matthewgerstman.com/tech/what-the-functor/)

> For all intents and purposes, all functors in programming are endofunctors. A functor is really just metadata around a value that allows us to map over it.

That was my eureka moment. If you see `M`, think `Metadata + Data`. 

[Reddit thread](https://www.reddit.com/r/functionalprogramming/comments/mbksrl/look_up_in_the_sky_its_a_burrito_its_a_plunger/)

## Implementing a monad

If a Monad is just Data + Metadata, then we ought to be able to implement it in C with a struct, right? 

```
struct monad {
    /// can be anything, the monad is opaque with respect to its data
    void *data; 
    /// some enum or something to represent our data type
    DataType dtype; 
    
    /// function pointer: lift(data, dtype) creates a monad from plain data
    struct monad (*lift)(void *d, DataType dtype);

    /// function pointer: bind chains a function
    struct monad (*bind)(Bindable); 

} Monad; 

``` 

Write some Vtables and switch case over `DataType` and Tada! Monads in C. "But wait!" you exclaim. 
"I am a programmer! I am lazy and hate repeating myself! Surely, I'll have to populate the vtable for 
all those functions! This isn't like monads in `$LANGUAGE`!" Oh. Hrm. Yeah that is kind of a problem.    

## A Tale of Two Monads

In the blagosphere, the word "Monad" is used to represent two, possibly three different things. 
The vast majority of the time, these are used completely interchangeably, and I think it leads to 
much of the confusion about Monads. 
- The "Elegant Monad" - this is a mathematical construct, or a programming. It is pure and perfect. 
    It encapsulates the idea of being able to operate on it and compose with it like water through 
    the reeds, because it is so pure. 
- The "Engineering Monad" - this is an actual data structure. These are things like Rust's `Option`
  and Javascript's `Promise`. 
  
[BuT pRoMiSe IsN't A mOnAd!](https://buzzdecafe.github.io/2018/04/10/no-promises-are-not-monads)
  ... yeah, this is exactly my point. People get into all kinds of internet arguments because
  X is not Monadic enough for the Monad Club. The criteria for the latter is basically,
  "Anything you can bind with and flatmap over": your `Option`s, `Result`s, `Promise`s, etc. 
  Or really, anything you can dot-chain a bunch of functions with to compose a sequence of operations.
  The slightly outlaw nature of the Engineering Monad doesn't prevent it from being used like a 
  monad 99% of the time. "But it's sloppy and technically incorrect!" Have you *seen* driver code?
  Even outlaw monads are better than no monads. 


## brain dump: 

why monads/HKTs? Example with the HTTP dispatch thing and wanting to dependency inject arbitrary methods 
but also enforce the type of those methods, while abstracting over them opaquely, all with type safety.    
  
## Other helpful links: 

[Monad laws for regular developers](https://miklos-martin.github.io/learn/fp/2016/03/10/monad-laws-for-regular-developers.html)
  
  

## The "fivey" problem: 
In most imperative languages, it doesn't make sense to have Some(func).map(5). This is because 
the implementer has to be aware that imperative paradigms cannot intuitively pattern match which 
part of the curried type signature `f -> a -> b` is the caller and which is the callee. 
So for example, this has to be implemented as such in python: 

```python
_default = object()
class Option(object):
    
    def __init__(self, a: A=_default, truthy=False): 
        if a is _default:
            self._inner_value = None
            self._truthy = False
            return 
        self._inner_value = a
        self._truthy = True
        
    def __str__(self):
        return repr(self)
    
    def __repr__(self):
        if self:
            return f'{self.__class__.__name__}({self._inner_value})'
        return f'{self.__class__.__name__}()'        
    def __bool__(self):
        return self._truthy
        
    @classmethod 
    def pure(cls, f: T) -> FT: 
        return cls(f) 
    
    def apply(ff: Fa2b, fa: FA) -> FB: 
        if not isinstance(ff, Option): 
            raise TypeError(f'{type(ff)} is not Option')
            
        if not isinstance(fa, Option): 
            raise TypeError(f'{type(fa)} is not Option')
        if not ff: 
            return Nothing()
        if not fa:
            return Nothing()
        f = ff._inner_value
        a = fa._inner_value
        if isinstance(f, Callable):
            return Some(f(a))
        return Some(a(f))
    
    
    def map(fa: FA, f: Fa2b) -> B:
        return fa.apply(Option(f))
    
class Nothing(Option):
    def __init__(self, a: A = None, truthy=False): 
        self._inner_value = a
        self._truthy = truthy

class Some(Option):
    def __init__(self, a: A, truthy=True): 
        self._inner_value = a
        self._truthy = truthy
```
this is fundamentally ok for now.
A strong type inference language with pattern matching is able to figure out that 
`f` is `Callable[[A], B]` and `a: A`, therefore `f` must be the caller and `a` the arg.
This is effectively "for free" in langs with PM, but has to be concreted in imperative
languages with weaker type compilers.  

Once you have that, you can then do: 
```python
Some(square).map(5)
Some(5).map(square)
```