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
    description: "Measure a distance of 7 cm and push the button!",
  },
  {
    cmd: "12",
    module: DISTANCE_CLICK,
    img: "https://i.imgur.com/418bHAt.jpg",
    description: "Measure a distance of 11 cm and push the button!",
  },
  {
    cmd: "13",
    module: DISTANCE_CLICK,
    img: "https://i.imgur.com/S0c5Tpp.jpg",
    description: "Measure a distance of 23 cm and push the button!",
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
    description: "Touch the lower touch button on the touchpad!",
  },
  {
    cmd: "32",
    module: TOUCH_CLICK,
    img: "https://i.imgur.com/gYAKQhL.jpg",
    description: "Touch the middle touch button on the touchpad!",
  },
  {
    cmd: "33",
    module: TOUCH_CLICK,
    img: "https://i.imgur.com/Q3jLT0Q.jpg",
    description: "Touch the upper touch button on the touchpad!",
  },
];

export default challenges

