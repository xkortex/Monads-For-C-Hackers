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
  
  
## Other helpful links: 

[Monad laws for regular developers](https://miklos-martin.github.io/learn/fp/2016/03/10/monad-laws-for-regular-developers.html)
  