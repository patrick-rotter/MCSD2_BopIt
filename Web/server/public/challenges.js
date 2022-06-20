const DISTANCE_CLICK = "Distance Click";
const JOYSTICK_CLICK = "Joystick Click";
const TOUCH_CLICK = "Touch Click";

export const gameOver = {
  cmd: "",
  module: "Game over!",
  img: "https://upload.wikimedia.org/wikipedia/commons/thumb/e/e3/Skull-Icon.svg/800px-Skull-Icon.svg.png",
  description: "GG, well played.",
};

const challenges = [
  {
    cmd: "11",
    module: DISTANCE_CLICK,
    img: "https://i.imgur.com/iusmMD6.jpg",
    description: "Measure less than 5 cm!",
  },
  {
    cmd: "12",
    module: DISTANCE_CLICK,
    img: "https://i.imgur.com/418bHAt.jpg",
    description: "Measure more than 15 cm!",
  },
  {
    cmd: "13",
    module: DISTANCE_CLICK,
    img: "https://i.imgur.com/S0c5Tpp.jpg",
    description: "Measure exactly 10 cm!",
  },
  {
    cmd: "21",
    module: JOYSTICK_CLICK,
    img: "https://i.imgur.com/R0bYopx.jpg",
    description: "Move the Joystick up!",
  },
  {
    cmd: "22",
    module: JOYSTICK_CLICK,
    img: "https://i.imgur.com/IP3utHu.jpg",
    description: "Move the Joystick left!",
  },
  {
    cmd: "23",
    module: JOYSTICK_CLICK,
    img: "https://i.imgur.com/0NFvwnr.jpg",
    description: "Move the Joystick right!",
  },
  {
    cmd: "31",
    module: TOUCH_CLICK,
    img: "https://i.imgur.com/ok6YqKC.jpg",
    description: "Touch the lower touch button!",
  },
  {
    cmd: "32",
    module: TOUCH_CLICK,
    img: "https://i.imgur.com/gYAKQhL.jpg",
    description: "Touch the middle touch button!",
  },
  {
    cmd: "33",
    module: TOUCH_CLICK,
    img: "https://i.imgur.com/Q3jLT0Q.jpg",
    description: "Touch the upper touch button!",
  },
];

export default challenges

