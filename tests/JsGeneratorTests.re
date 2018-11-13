open JsGenerator;

/* Create a generator - pretend this comes from the outside */
let jsGen: JsGenerator.jsFn(int) = [%raw "function *() { yield 1; yield 2; yield 3 }"];
let gen = jsGen(.);

/* Iterate over the generator and stop when doneGet returns true */
Js.log("Running the JavaScript generator:");
let i = ref(gen->next());
while (! (i^)->doneGet) {
  Js.log((i^)->valueGet);
  i := gen->next();
}

/* Create a generator - pretend this comes from the outside */
let jsGen: JsGenerator.fn1(int, int) = [%raw "function *(n) { for (let i = 1; i <= n; i++) { yield i; } }"];
let gen = jsGen(5);

/* Iterate over the generator and stop when doneGet returns true */
Js.log("Running the JavaScript generator:");
let i = ref(gen->next());
while (! (i^)->doneGet) {
  Js.log((i^)->valueGet);
  i := gen->next();
}

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
