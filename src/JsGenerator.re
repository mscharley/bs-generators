[@bs.deriving abstract]
type value('a) = {
  [@bs.as "done"]
  done_: bool,
  [@bs.as "value"]
  value_: Js.Undefined.t('a),
};

let valueGet = v => v->value_Get->Js.Undefined.toOption;
let doneGet = done_Get;

[@bs.deriving abstract]
type t('a) = {
  next: unit => value('a),
  return: Js.Undefined.t('a) => value('a),
  throw: exn => unit,
};

type fn('a) = unit => t('a);
type fn1('a, 'b) = 'a => t('b);
type jsFn('a) = (. unit) => t('a);

[@bs.send] external next: (t('a), unit) => value('a) = "";
[@bs.send] external return: (t('a), Js.Undefined.t('a)) => value('a) = "";
[@bs.send] external throw: (t('a), exn) => unit = "";

let simple = next => {
  let iterator =
    t(
      ~next,
      ~return=v => value(~done_=true, ~value_=v),
      ~throw=exn => raise(exn),
    );
  /* Necessary, to flag to JS land that this is iterable. */
  /* @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols */
  [%raw "iterator[Symbol.iterator] = function() { return iterator }"] |> ignore;
  iterator;
};

let fromNext = next =>
  simple(_ =>
    switch (next()) {
    | Some(v) => value(~done_=false, ~value_=Js.Undefined.fromOption(v))
    | None => value(~done_=true, ~value_=Js.undefined)
    }
  );
