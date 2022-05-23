import React from "react";

const Image = (props) => {
  return (
    <img
      className="challenge-img"
      src={props.url}
      alt="Challenge"
    ></img>
  );
};

export default Image;
