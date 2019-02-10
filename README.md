# bs-generators

[![npm](https://img.shields.io/npm/v/bs-generators.svg)](https://www.npmjs.com/package/bs-generators) 
[![Greenkeeper badge](https://badges.greenkeeper.io/mscharley/bs-generators.svg)](https://greenkeeper.io/)

**Source:** [https://github.com/mscharley/bs-generators](https://github.com/mscharley/bs-generators)  
**Author:** Matthew Scharley  
**Contributors:** [See contributors on GitHub][gh-contrib]  
**Bugs/Support:** [Github Issues][gh-issues]  
**Copyright:** 2018  
**License:** [MIT license][license]  
**Status:** Active

## Synopsis

This package provides bindings to use and create [JavaScripts' Generator objects][mdn-generator], aka `function*() {}`.

Using this library you can consume JavaScript libraries that use `function*` and you can create objects that operate like that JS syntax to send into JavaScript land as well.

A word of warning. This library should only really be considered for interoperation with existing JavaScript code that uses Generators. I do not recommend using these types on their own inside ReasonML/Ocaml codebases.

## Installation

Depending on your package manager of choice,

```shell
npm install bs-generators
```

or

```shell
yarn add bs-generators
```

## Usage

The best API documentation so far is [the interface file](https://github.com/mscharley/bs-generators/blob/master/src/JsGenerator.rei).

### Using a function* defined in JavaScript and provided to your Reason code

```reason
open JsGenerator;

/* Create a generator - pretend this comes from the outside */
let jsGen: JsGenerator.fnU(int) = [%raw "function *() { yield 1; yield 2; yield 3 }"];
let gen = jsGen();

/* Iterate over the generator and stop when doneGet returns true */
Js.log("Running the JavaScript generator:");
let i = ref(gen->next());
while (! (i^)->doneGet) {
  Js.log((i^)->valueGet);
  i := gen->next();
}
```

### Creating a Generator to pass into JavaScript code

```reason
let reGen: JsGenerator.fn(int) = _ => JsGenerator.fromNext({
  /* Create a mutable counter */
  let i = ref(0);
  () => {
    if (i^ < 3) {
      i := i^ + 1;
      /* You can also return Some(None) to send an undefined value to JS without closing the Generator */
      Some(Some(i^));
    }
    else {
      /* Close the Generator */
      None;
    }
  }
});

Js.log("Running the Reason generator:");
[%%raw "for (var x of reGen()) { console.log(x); }"];
```

  [gh-contrib]: https://github.com/mscharley/bs-generators/graphs/contributors
  [gh-issues]: https://github.com/mscharley/bs-generators/issues
  [license]: https://github.com/mscharley/bs-generators/blob/master/LICENSE
  [mdn-generator]: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Generator
