import React from "react";

const Image = (props) => {
  return (
    <img
      className="challenge-img"
      width="400"
      height="300"
      src={props.url}
      alt="Challenge"
    ></img>
  );
};

export default Image;
