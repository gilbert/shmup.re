module C = Canvas;
module StarField = StarField;
module Ship = Ship;
module Input = Input;


type state = {
  mutable ship: Ship.t,
  startTime: float
};
let gameState = {
  startTime: Js.Date.now (),
  ship: Ship.initialState
};

let setupDraw = fun canvas => {
  let ctx = C.CanvasElement.getContext canvas "2d";
  let rec render = fun () => {
    let now = Js.Date.now ();
    let runTime = now -. gameState.startTime;

    C.clearRect ctx 0. 0. C.width C.height;
    StarField.draw ctx speed::2. runTime;
    StarField.draw ctx offset::(100., 200.) runTime;
    Ship.draw ctx gameState.ship;
    ReasonJs.requestAnimationFrame render;
  };
  let _ = ReasonJs.requestAnimationFrame render;
};


let gameLoop = fun () => {
  let cmds = Input.sample ();
  /* TODO: operate on more ship state than position (eg. bullet state) */
  gameState.ship = Ship.tick gameState.ship cmds;
};


let init = fun () => {
  open ReasonJs.Dom;
  let canvasEl = DocumentRe.querySelector "canvas" document;
  switch canvasEl {
  | Some canv => setupDraw canv
  | None => Js.log "couldnt get canvas"
  };

  Input.bindListeners ();
  Js.Global.setInterval gameLoop 33;
};

init ();
