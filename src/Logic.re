type player =
  | Black
  | White;

type settings = {
  baseTimeInMillis: int,
  additionInMillis: int,
};

type timeLeft = {
  black: int,
  white: int,
};

type gameState =
  | ReadyToStart
  | Ongoing
  | GameOver;

type timerState = {
  turn: player,
  settings,
  timeLeft,
  gameState,
};

let baseTimeInMillis = 1000 * 60 * 3;

let initialState = {
  turn: White,
  settings: {
    baseTimeInMillis,
    additionInMillis: 1000 * 2,
  },
  timeLeft: {
    black: baseTimeInMillis,
    white: baseTimeInMillis,
  },
  gameState: ReadyToStart,
};

let tick = (timerState, millis) => {
  switch (timerState.turn) {
  | Black => {
      ...timerState,
      gameState:
        Js.Math.max_int(timerState.timeLeft.black - millis, 0) == 0
          ? GameOver : Ongoing,
      timeLeft: {
        white: timerState.timeLeft.white,
        black: Js.Math.max_int(timerState.timeLeft.black - millis, 0),
      },
    }
  | White => {
      ...timerState,
      gameState:
        Js.Math.max_int(timerState.timeLeft.white - millis, 0) == 0
          ? GameOver : Ongoing,
      timeLeft: {
        black: timerState.timeLeft.black,
        white: Js.Math.max_int(timerState.timeLeft.white - millis, 0),
      },
    }
  };
};

let handlePress = (timerState, pressedClock) => {
  switch (timerState.turn, pressedClock) {
  | (Black, Black) => {
      ...timerState,
      turn: White,
      timeLeft: {
        ...timerState.timeLeft,
        black:
          timerState.timeLeft.black + timerState.settings.additionInMillis,
      },
    }
  | (White, White) => {
      ...timerState,
      turn: Black,
      timeLeft: {
        ...timerState.timeLeft,
        white:
          timerState.timeLeft.white + timerState.settings.additionInMillis,
      },
    }
    | (Black, White)=> timerState
    | (White, Black) => timerState
  };
};


let handleStartGameClick = (timerState, pressedClock) => {
  switch (pressedClock) {
    | Black => { ...timerState, turn: Black, gameState: Ongoing }
    | White => { ...timerState, turn: White, gameState: Ongoing }
  }
};


let changeBaseTime = (timerState, newTime) => {
  ...timerState,
  settings: {
    ...timerState.settings,
    baseTimeInMillis: newTime,
  },
};

let changeAddition = (timerState, newTime) => {
  ...timerState,
  settings: {
    ...timerState.settings,
    additionInMillis: newTime,
  },
};

let resetGame = timerState => {
  ...timerState,
  gameState: ReadyToStart,
  timeLeft: {
    black: timerState.settings.baseTimeInMillis,
    white: timerState.settings.baseTimeInMillis,
  },
};

type timerActions =
  | Tick(int)
  | Press(player)
  | Reset
  | ChangeBaseTime(int)
  | ChangeTimeAddition(int) 


let reducer = (timerState, action) => {
  switch (action, timerState.gameState) {
  | (Tick(millis), Ongoing) => tick(timerState, millis)
  | (Press(pressedClock), Ongoing) => handlePress(timerState, pressedClock)
  | (Press(pressedClock), ReadyToStart) => handleStartGameClick(timerState, pressedClock)
  | (Tick(_), _) => timerState
  | (Press(_), _) => timerState
  | (ChangeBaseTime(newTime), _) => changeBaseTime(timerState, newTime)
  | (ChangeTimeAddition(newTime), _) =>
    changeAddition(timerState, newTime)
  | (Reset, _) => resetGame(timerState)
  };
};
