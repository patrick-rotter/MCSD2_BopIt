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
    img: "https://merriam-webster.com/assets/mw/images/gallery/gal-wap-slideshow-slide/image1294857992-5173-f5c77f4215a3f8bec22ab164b6e07450@1x.jpg",
    description: "Measure a distance of 7 cm and push the button!",
  },
  {
    cmd: "12",
    module: DISTANCE_CLICK,
    img: "https://merriam-webster.com/assets/mw/images/gallery/gal-wap-slideshow-slide/image1294857992-5173-f5c77f4215a3f8bec22ab164b6e07450@1x.jpg",
    description: "Measure a distance of 11 cm and push the button!",
  },
  {
    cmd: "13",
    module: DISTANCE_CLICK,
    img: "https://merriam-webster.com/assets/mw/images/gallery/gal-wap-slideshow-slide/image1294857992-5173-f5c77f4215a3f8bec22ab164b6e07450@1x.jpg",
    description: "Measure a distance of 23 cm and push the button!",
  },
  {
    cmd: "21",
    module: JOYSTICK_CLICK,
    img: "https://asset.conrad.com/media10/isa/160267/c1/-/de/1893215_RB_01_FB/speedlink-competition-pro-extra-joystick-usb-pc-android-schwarz-rot.jpg",
    description: "Move the Joystick up!",
  },
  {
    cmd: "22",
    module: JOYSTICK_CLICK,
    img: "https://asset.conrad.com/media10/isa/160267/c1/-/de/1893215_RB_01_FB/speedlink-competition-pro-extra-joystick-usb-pc-android-schwarz-rot.jpg",
    description: "Move the Joystick left!",
  },
  {
    cmd: "23",
    module: JOYSTICK_CLICK,
    img: "https://asset.conrad.com/media10/isa/160267/c1/-/de/1893215_RB_01_FB/speedlink-competition-pro-extra-joystick-usb-pc-android-schwarz-rot.jpg",
    description: "Move the Joystick right!",
  },
  {
    cmd: "31",
    module: TOUCH_CLICK,
    img: "https://i.ytimg.com/vi/ba2RsswFNpg/maxresdefault.jpg",
    description: "Touch the lower touch button on the touchpad!",
  },
  {
    cmd: "32",
    module: TOUCH_CLICK,
    img: "https://i.ytimg.com/vi/ba2RsswFNpg/maxresdefault.jpg",
    description: "Touch the upper touch button on the touchpad!",
  },
  {
    cmd: "33",
    module: TOUCH_CLICK,
    img: "https://i.ytimg.com/vi/ba2RsswFNpg/maxresdefault.jpg",
    description: "Slide your finger across the touchpad!",
  },
];

export default challenges

