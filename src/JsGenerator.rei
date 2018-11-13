/* Abstract type referencing something implementing the JavaScript Generator interface */
type t('a);
/* Abstract type referencing a single value returned by an interator */
type value('a);

/* fn(int) is the type of the following javascript: function* () { yield 1; } */
type fn('a) = unit => t('a);
type fn1('a, 'b) = 'a => t('b);

/* Check from the returned value if the iterator is complete */
let doneGet: value('a) => bool;
/* Extract the returned value from the iterator. Note, this can be None legitimately if the iterator returns undefined. Always check doneGet for completeness. */
let valueGet: value('a) => option('a);

/* @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Generator#Methods */
let next: t('a) => unit => value('a);
let return: t('a) => Js.Undefined.t('a) => value('a);
let throw: t('a) => exn => unit;

/* Create a generator object from a ReasonML implementation of the next() method */
let fromNext: (unit => option(option('a))) => t('a);
