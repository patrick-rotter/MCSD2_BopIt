import React from "react";

const Title = (props) => {
  return (
    <h1 style={{color: !props.isAlive && "#c94242"}} className="title">
      {props.text}
    </h1>
  );
};

export default Title;
