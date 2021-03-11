module PrettyMs {
  type t;
  [@bs.module "duration-pretty"] external duration : int => t  = "duration";
  [@bs.send] external format : (t, string) => string = "format";
}

module StartGameView = {
  [@react.component]
  let make = (~startGame) => {
    <button className="actionButton" onClick={_ => startGame()}>
      {React.string("Start the game")}
    </button>;
  };
};

module Reset = {
  [@react.component]
  let make = (~onReset) => {
    <button className="actionButton" onClick={_ => onReset()}>
      {React.string("Reset")}
    </button>;
  };
};


module Timers = {
  [@react.component]
  let make = (~blackTimeLeft, ~whiteTimeLeft, ~onPress, ~activePlayer) => {
    let (blackClassName, whiteClassName) =
      switch (activePlayer) {
      | Logic.Black => ("active", "passive")
      | Logic.White => ("passive", "active")
      };


    <div className="timers" onClick={_ => onPress()}>
      <div className={"timer " ++ blackClassName}>
        {React.string(PrettyMs.format(PrettyMs.duration(blackTimeLeft), "mm:ss"))}
      </div>
      <div className={"timer " ++ whiteClassName}>
        {React.string(PrettyMs.format(PrettyMs.duration(whiteTimeLeft), "mm:ss"))}
      </div>
    </div>;
  };
};

module Gameover = {
  [@react.component]
  let make = (~onReset) => {
    <button className="actionButton" onClick={_ => onReset()}> {React.string("Restart")} </button>;
  };
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => Logic.reducer(state, action),
      Logic.initialState,
    );

  let (intervalId, setIntervalId) = React.useState(() => None);

  let startGame = () => {
    let _ = Belt.Option.map(intervalId, id => Js.Global.clearInterval(id));

    setIntervalId(_ =>
      Some(Js.Global.setInterval(() => dispatch(Logic.Tick(100)), 100))
    );
    
    dispatch(Logic.StartGame);
  };



  <div className="container">
    <Timers
      blackTimeLeft={state.timeLeft.black}
      whiteTimeLeft={state.timeLeft.white}
      activePlayer={state.turn}
      onPress={() => dispatch(Logic.ChangeTurn)}
    />
    <div className="buttons-container">
      {switch (state.gameState) {
       | Ongoing => <Reset onReset={() => dispatch(Logic.Reset)} />
       | GameOver => <Gameover onReset={() => dispatch(Logic.Reset)} />
       | ReadyToStart => <StartGameView startGame />
       }}
    </div>
  </div>;
};
