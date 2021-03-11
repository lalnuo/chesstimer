let leftSideKeys = "QWERTASDFGZXCV";
let rightSideKeys = {j|YUIOPHJKLBNMÄÖÅ|j}
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

module Timer = {
  [@react.component]
  let make = (~onPress, ~timeLeft, ~isActive, ~gameState, ~keys) => {
    let activityClassname = isActive ? "active" : "passive";

    let actionText =
      switch (gameState) {
      | Logic.ReadyToStart => "to start the clock"
      | Logic.GameOver => ""
      | Logic.Ongoing => "to finish your turn"
      };

    let prettifiedKeys =
      keys
      |> Js.String.split("")
      |> Array.map(x => String.uppercase_ascii(x) ++ " ")
      |> Array.fold_right((acc, i) => acc ++ i, _, "");

    <div className={"timer-container " ++ activityClassname}>
      <div onClick={_ => onPress(Logic.Black)} className="timer">
        {React.string(PrettyMs.format(PrettyMs.duration(timeLeft), "mm:ss"))}
      </div>
      <div className="button-advice">
        <div className="button-text"> {React.string("Press")} </div>
        <div className="buttons"> {React.string(prettifiedKeys)} </div>
        <div className="button-text"> {React.string(actionText)} </div>
      </div>
    </div>;
  };
};

module Timers = {
  [@react.component]
  let make =
      (~blackTimeLeft, ~whiteTimeLeft, ~onPress, ~activePlayer, ~gameState) => {
    <div className="timers">
      <Timer
        isActive={activePlayer == Logic.Black}
        timeLeft=blackTimeLeft
        gameState
        keys=leftSideKeys
        onPress={_ => onPress(Logic.Black)}
      />
      <Timer
        isActive={activePlayer == Logic.White}
        timeLeft=whiteTimeLeft
        keys=rightSideKeys
        onPress={_ => onPress(Logic.White)}
        gameState
      />
    </div>;
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

  React.useEffect0(() => {
    let handleKey = event => {
      let key = Webapi.Dom.KeyboardEvent.key(event);

      if (Js.String.indexOf(Js.String.toUpperCase(key), leftSideKeys) != -1) {
        dispatch(Logic.Press(Black));
      };
      if (Js.String.indexOf(Js.String.toUpperCase(key), rightSideKeys) != -1) {
        dispatch(Logic.Press(White));
      };
    };
    let document = Webapi.Dom.Document.asEventTarget(Webapi.Dom.document);
    let _ =
      Webapi.Dom.EventTarget.addKeyDownEventListener(handleKey, document);
    None;
  });

  React.useEffect1(
    () => {
      if (state.gameState == Logic.Ongoing) {
        let _ =
          Belt.Option.map(intervalId, id => Js.Global.clearInterval(id));
        setIntervalId(_ =>
          Some(Js.Global.setInterval(() => dispatch(Logic.Tick(100)), 100))
        );
      };
      None;
    },
    [|state.gameState|],
  );

  let onPress = color => {
    dispatch(Logic.Press(color));
  };

  <div className="container">
    <Timers
      blackTimeLeft={state.timeLeft.black}
      whiteTimeLeft={state.timeLeft.white}
      gameState={state.gameState}
      activePlayer={state.turn}
      onPress
    />
    <div className="buttons-container">
      {switch (state.gameState) {
       | Ongoing => <Reset onReset={() => dispatch(Logic.Reset)} />
       | GameOver => <Reset onReset={() => dispatch(Logic.Reset)} />
       | ReadyToStart => React.null
       }}
    </div>
  </div>;
};
