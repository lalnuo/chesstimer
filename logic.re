type player = Black | White

type settings = {
  baseTimeInMillis: int,
  additionInMillis: int,
}

type timeLeft = {
  black: int,
  white: int,
}

type gameState = Ongoing | GameOver

type timerState = {
  turn: player,
  settings,
  timeLeft,
  gameState,
}

type timerActions =
  Tick(int) | ChangeTurn | Reset | ChangeBaseTime(int) | ChangeTimeAddition(int)

let tick = (timerState, player, millis) => {
  switch player {
  | Black => {
      ...timerState,
      gameState: Js.Math.max_int(timerState.timeLeft.black - millis, 0) == 0 ? GameOver : Ongoing,
      timeLeft: {
        white: timerState.timeLeft.white,
        black: Js.Math.max_int(timerState.timeLeft.black - millis, 0),
      },
    }
  | White => {
      ...timerState,
      gameState: Js.Math.max_int(timerState.timeLeft.black - millis, 0) == 0 ? GameOver : Ongoing,
      timeLeft: {
        black: timerState.timeLeft.black,
        white: Js.Math.max_int(timerState.timeLeft.white - millis, 0),
      },
    }
  }
}

let handlePress = (timerState, color) => {
  switch color {
  | Black => {
      ...timerState,
      turn: White,
      timeLeft: {
        ...timerState.timeLeft,
        black: timerState.timeLeft.black +
        timerState.settings.additionInMillis,
      },
    }
  | White => {
      ...timerState,
      turn: Black,
      timeLeft: {
        ...timerState.timeLeft,
        white: timerState.timeLeft.white +
        timerState.settings.additionInMillis,
      },
    }
  }
}

let changeBaseTime = (timerState, newTime) => {
  ...timerState,
  settings: {
    ...timerState.settings,
    baseTimeInMillis: newTime,
  },
}

let changeAddition = (timerState, newTime) => {
  ...timerState,
  settings: {
    ...timerState.settings,
    additionInMillis: newTime,
  },
}

let resetGame = timerState => {
      ...timerState,
      gameState: Ongoing,
      timeLeft: {
        black: timerState.settings.baseTimeInMillis,
        white: timerState.settings.baseTimeInMillis,
      },
    }

let reducer = (timerState, action) => {
  switch (action, timerState.turn, timerState.gameState) {
  | (Tick(_), _, GameOver) => timerState
  | (ChangeTurn, _, GameOver) => timerState
  | (Tick(millis), color, Ongoing) => tick(timerState, color, millis)
  | (ChangeTurn, color, Ongoing) => handlePress(timerState, color)
  | (ChangeBaseTime(newTime), _, _) => changeBaseTime(timerState, newTime)
  | (ChangeTimeAddition(newTime), _, _) => changeAddition(timerState, newTime)
  | (Reset, _, _) => resetGame(timerState)
  }
}
